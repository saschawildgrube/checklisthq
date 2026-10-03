<?php
// Standalone local check: php -d extension=mbstring test-placeholder-scanning.php
// Optional arguments: baseline wdk_website.inc and baseline wdk_string.inc.
// Extract the real methods to avoid bootstrapping sessions, databases or HTTP.
function sourceFunction($source, $name)
{
    $tokens = token_get_all($source);
    for ($i = 0; $i < count($tokens); $i++) {
        if (!is_array($tokens[$i]) || $tokens[$i][0] !== T_FUNCTION) continue;
        $j = $i + 1;
        while (is_array($tokens[$j]) && $tokens[$j][0] === T_WHITESPACE) $j++;
        if (!is_array($tokens[$j]) || $tokens[$j][1] !== $name) continue;
        $code = '';
        $depth = 0;
        $opened = false;
        for (; $i < count($tokens); $i++) {
            $token = $tokens[$i];
            $code .= is_array($token) ? $token[1] : $token;
            if ($token === '{') { $depth++; $opened = true; }
            if ($token === '}' && --$depth === 0 && $opened) return $code;
        }
    }
    throw new RuntimeException('Function not found: '.$name);
}

function ArrayCount($value) { return is_array($value) ? count($value) : 0; }
$root = dirname(__DIR__, 3);
require $root.'/wdk/wdk_string.inc';
eval(sourceFunction(file_get_contents($root.'/wdk/wdk_xml.inc'), 'GetAttributeFromXMLTag'));
class PlaceholderProbe
{
    public $m_arrayGenericOutputItems = array('ICON' => array('id'), 'URL' => array('content'), 'EMPTY' => array());
    public $calls = array();
    function Error($message) { throw new RuntimeException('Malformed placeholder'); }
    function OnRenderGenericOutputItem($id, $attributes)
    {
        $this->calls[] = array($id, $attributes);
        if ($id === 'EMPTY') return '';
        return '['.$id.':'.implode('|', $attributes).']';
    }
}
eval('class CurrentProbe extends PlaceholderProbe {'.sourceFunction(file_get_contents($root.'/wdk/wdk_website.inc'), 'ReplaceGenericOutputItems').'}');
if (isset($argv[1])) {
    eval('class BaselineProbe extends PlaceholderProbe {'.sourceFunction(file_get_contents($argv[1]), 'ReplaceGenericOutputItems').'}');
    eval(str_replace('function ReplaceTags_TagLoop(', 'function BaselineTagLoop(', sourceFunction(file_get_contents($argv[2]), 'ReplaceTags_TagLoop')));
}
function runProbe($class, $input)
{
    $probe = new $class();
    try { $output = $probe->ReplaceGenericOutputItems($input); }
    catch (RuntimeException $e) { $output = 'ERROR:'.$e->getMessage(); }
    return array($output, $probe->calls);
}
function checkEqual($actual, $expected)
{
    if ($actual !== $expected) throw new RuntimeException('Output or callback order changed');
}
$cases = array('', 'Plain text', 'Grüße 日本語 😀', '{ICON}', '{ICON}{URL}', '{ICONIC}{ICON}',
    '{UNKNOWN}{EMPTY}', '{ICON id="ä😀"}', "é{ICON\tid=\"中\"}", "{ICON\nid=\"x\"}",
    "{ICON\r\nid=\"x\"}", '{ICON id="x"}{ICON id="x"}', '{ICONx', '{ICON ',
    'é{ICON id="x"', '{URL content="{ICON}"}', '{icon}', '{ICON}尾{', '{ICON}😀{URL content="é"}');
checkEqual(runProbe('CurrentProbe', 'Grüße 😀{ICON id="中"}尾')[0], 'Grüße 😀[ICON:中]尾');
checkEqual(runProbe('CurrentProbe', '{ICONIC}{EMPTY}{ICON}')[0], '{ICONIC}{EMPTY}[ICON:]');
mt_srand(1000);
for ($i = 0; $i < 500; $i++) {
    $input = '';
    for ($j = 0; $j < 8; $j++) $input .= $cases[mt_rand(0, 18)];
    $cases[] = $input;
}
foreach ($cases as $input) {
    $actual = runProbe('CurrentProbe', $input);
    if (class_exists('BaselineProbe')) checkEqual($actual, runProbe('BaselineProbe', $input));
}
// Sequential replacement must still expand tags introduced by earlier entries.
$tags = array('A' => '{B}', 'B' => 'ä😀');
for ($i = 0; $i < 20; $i++) $tags['unused'.$i] = 'unused';
foreach (array('{A}', 'é{A}{B}尾', '{UNKNOWN}', str_repeat('😀 text ', 10000).'{A}') as $input) {
    checkEqual(ReplaceTags_TagLoop($input, $tags, '{', '}'), str_replace(array('{A}', '{B}'), array('{B}', 'ä😀'), $input));
    if (function_exists('BaselineTagLoop')) checkEqual(ReplaceTags_TagLoop($input, $tags, '{', '}'), BaselineTagLoop($input, $tags, '{', '}'));
}
echo 'PASS: '.count($cases).' generic-placeholder cases plus sequential tag-loop checks'.PHP_EOL;
// Compare scanner cost on a page comparable in size to the 1,000-task response.
$large = str_repeat('<li>Grüße 😀 task with action links</li>', 45000).'{ICON id="check"}';
foreach (array('BaselineProbe', 'CurrentProbe') as $class) {
    if (!class_exists($class)) continue;
    $probe = new $class();
    for ($i = 0; $i < 50; $i++) $probe->m_arrayGenericOutputItems['UNUSED'.$i] = array();
    $start = microtime(true);
    $output = $probe->ReplaceGenericOutputItems($large);
    printf("%s: %.4f seconds (%d bytes)\n", $class, microtime(true) - $start, strlen($large));
    if ($class === 'BaselineProbe') $baselineOutput = $output;
    elseif (isset($baselineOutput)) checkEqual($output, $baselineOutput);
}
