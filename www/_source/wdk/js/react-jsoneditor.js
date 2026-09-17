'use strict';

// A separate draft lets users type incomplete numbers and property names.
class JsonEditorField extends React.Component
{
	constructor(props) { super(props); this.state = { draft: props.value, error: '' }; }
	componentDidUpdate(previous)
	{
		if (previous.value !== this.props.value && !this.focused)
			this.setState({ draft: this.props.value, error: '' });
	}
	commit()
	{
		const error = this.props.onCommit(this.state.draft);
		this.setState({ error: error || '' });
	}
	render()
	{
		return e('span', { className: this.props.className },
			e('input', {
				ref: this.props.inputRef,
				value: this.state.draft, 'aria-label': this.props.label,
				'aria-invalid': !!this.state.error, title: this.state.error,
				onFocus: () => { this.focused = true; },
				onChange: event => { this.setState({ draft: event.target.value, error: '' }); if (this.props.onInput) this.props.onInput(event.target.value); },
				onBlur: () => { this.focused = false; this.commit(); },
				onKeyDown: event => {
					if (event.key === 'Enter') { event.preventDefault(); this.commit(); }
					if (event.key === 'Escape') this.setState({ draft: this.props.value, error: '' });
				}
			}),
			this.state.error && e('span', { className: 'jsoneditor-error', role: 'alert' }, this.state.error));
	}
}

// Uncommitted rows are UI drafts and never appear in the JSON document.
class JsonEditorNewRow extends React.Component
{
	constructor(props) { super(props); this.state = { name: '', value: '', error: '', type: null }; }
	commit()
	{
		if (!this.state.name && !this.state.value && this.state.type === null) return;
		const error = this.props.onAdd(this.state.name, this.state.value, this.state.type);
		if (error) this.setState({ error: error });
		else this.setState({ name: '', value: '', error: '', type: null });
	}
	render()
	{
		return e('div', { className: 'jsoneditor-row jsoneditor-new-row', onBlur: event => { if (!event.currentTarget.contains(event.relatedTarget)) this.commit(); }, onKeyDown: event => { if (event.key === 'Enter') { event.preventDefault(); this.commit(); } } },
			this.props.array ? e('span', {}, 'New item') : e('input', { className: 'jsoneditor-key', placeholder: 'New key', 'aria-label': 'New property name', value: this.state.name, onChange: event => this.setState({ name: event.target.value, error: '' }) }),
			e('input', { className: 'jsoneditor-value', placeholder: 'Value', 'aria-label': 'New value', value: this.state.value, onChange: event => this.setState({ value: event.target.value, error: '', type: null }) }),
			e('select', { 'aria-label': 'New value type', value: this.state.type || this.props.detectType(this.state.value), onChange: event => {
				const type = event.target.value, defaults = { string: this.state.value, number: '0', boolean: 'false', null: 'null', object: '{}', array: '[]' };
				this.setState({ type: type, value: defaults[type] });
			} }, ['string', 'number', 'boolean', 'null', 'object', 'array'].map(type => e('option', { key: type, value: type }, type))),
			this.state.error && e('span', { className: 'jsoneditor-error', role: 'alert' }, this.state.error));
	}
}

class JsonEditor extends WDKReactComponent
{
	constructor(props)
	{
		super(props);
		this.state = { raw: '{}', data: {}, error: '', rawVisible: true, treeVisible: true, collapsed: {}, revision: 0 };
	}
	type(value) { return value === null ? 'null' : Array.isArray(value) ? 'array' : typeof value; }
	// Preserve every character, including incomplete JSON, for the highlighting layer.
	tokens(raw)
	{
		const pattern = /"(?:\\.|[^"\\])*"|-?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][+-]?\d+)?|true\b|false\b|null\b|\{\s*\}|\[\s*\]|[{}\[\],:]|\s+|./g;
		const tokens = [];
		let match;
		while ((match = pattern.exec(raw)) !== null)
		{
			const text = match[0], start = match.index;
			let type = 'plain', selectable = false, from = start, to = start + text.length;
			if (text[0] === '"' && text.length > 1)
			{
				type = /^\s*:/.test(raw.slice(to)) ? 'key' : 'string';
				selectable = true; from++; to--;
			}
			else if (/^-?\d/.test(text)) { type = 'number'; selectable = true; }
			else if (/^(true|false|null)$/.test(text)) { type = text === 'null' ? 'null' : 'boolean'; selectable = true; }
			else if (/^[{}\[\],:]/.test(text)) { type = 'punctuation'; selectable = text.length > 1; }
			tokens.push({ text: text, start: start, end: start + text.length, from: from, to: to, type: type, selectable: selectable });
		}
		return tokens;
	}
	syncRawScroll()
	{
		if (this.rawInput && this.rawHighlight)
		{
			this.rawHighlight.scrollTop = this.rawInput.scrollTop;
			this.rawHighlight.scrollLeft = this.rawInput.scrollLeft;
		}
	}
	componentDidUpdate() { this.syncRawScroll(); }
	rawKeyDown(event)
	{
		if (!event.ctrlKey && !event.metaKey && !event.altKey && !event.isComposing)
		{
			const input = event.currentTarget, start = input.selectionStart, end = input.selectionEnd;
			if (!this.insideString(input.value.slice(0, start)))
			{
				if (event.key === '{' || event.key === '[')
				{
					event.preventDefault();
					const closing = event.key === '{' ? '}' : ']';
					this.parse(input.value.slice(0, start) + event.key + input.value.slice(start, end) + closing + input.value.slice(end));
					this.setState({}, () => input.setSelectionRange(start + 1, end + 1));
					return;
				}
				if ((event.key === '}' || event.key === ']') && start === end && input.value[start] === event.key)
				{
					event.preventDefault(); input.setSelectionRange(start + 1, start + 1); return;
				}
			}
		}
		if (event.key === 'Enter' && !event.ctrlKey && !event.metaKey && !event.altKey && !event.isComposing)
		{
			event.preventDefault();
			const input = event.currentTarget, edit = this.indent(input.value, input.selectionStart, input.selectionEnd);
			this.parse(edit.raw);
			this.setState({}, () => { input.setSelectionRange(edit.caret, edit.caret); this.syncRawScroll(); });
			return;
		}
		if (event.key !== 'Tab' || event.ctrlKey || event.metaKey || event.altKey || event.isComposing) return;
		const input = event.currentTarget;
		const tokens = this.tokens(input.value).filter(token => token.selectable);
		const start = input.selectionStart, end = input.selectionEnd;
		const current = tokens.findIndex(token => start >= token.start && end <= token.end && start < token.end);
		let next;
		if (current !== -1) next = tokens[current + (event.shiftKey ? -1 : 1)];
		else if (event.shiftKey) next = tokens.slice().reverse().find(token => token.end <= start);
		else next = tokens.find(token => token.start >= end);
		// At either boundary, normal Tab navigation leaves the editor.
		if (!next) return;
		event.preventDefault();
		input.setSelectionRange(next.from, next.to);
		const before = input.value.slice(0, next.from).split('\n');
		const style = window.getComputedStyle(input);
		const lineHeight = parseFloat(style.lineHeight);
		const top = (before.length - 1) * lineHeight;
		if (top < input.scrollTop || top + lineHeight > input.scrollTop + input.clientHeight)
			input.scrollTop = Math.max(0, top - input.clientHeight / 2);
		const canvas = document.createElement('canvas'), context = canvas.getContext('2d');
		if (context)
		{
			context.font = style.font;
			const left = context.measureText(before[before.length - 1].replace(/\t/g, '    ')).width;
			if (left < input.scrollLeft || left > input.scrollLeft + input.clientWidth - 40)
				input.scrollLeft = Math.max(0, left - input.clientWidth / 2);
		}
		this.syncRawScroll();
	}
	indent(raw, start, end)
	{
		let before = raw.slice(0, start);
		const after = raw.slice(end);
		const line = before.slice(before.lastIndexOf('\n') + 1), base = (line.match(/^[\t ]*/) || [''])[0];
		const unit = (raw.match(/\n(\t+| +)\S/) || [null, '  '])[1];
		const quoted = this.insideString(before);
		if (!quoted && this.needsComma(before) && !/^\s*,/.test(after)) before = before.replace(/\s*$/, ',');
		const opener = !quoted && /[\[{]\s*$/.test(before);
		const indentation = base + (opener ? (unit[0] === '\t' ? '\t' : unit) : '');
		const newline = '\n' + indentation;
		const closing = opener && /^\s*[\]}]/.test(after) ? '\n' + base : '';
		return { raw: before + newline + closing + (closing ? after.replace(/^\s*/, '') : after), caret: before.length + newline.length };
	}
	insideString(text)
	{
		let quoted = false, escaped = false;
		for (const character of text) { if (escaped) escaped = false; else if (character === '\\' && quoted) escaped = true; else if (character === '"') quoted = !quoted; }
		return quoted;
	}
	needsComma(text)
	{
		const stack = [];
		// Track whether the current object/array has just received a complete value.
		const parts = text.match(/"(?:\\.|[^"\\])*"|-?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][+-]?\d+)?|true\b|false\b|null\b|[^\s]/g) || [];
		for (const part of parts)
		{
			const current = stack[stack.length - 1];
			if (part === '{' || part === '[') stack.push({ kind: part, expect: part === '{' ? 'key' : 'value' });
			else if (part === '}' || part === ']') { stack.pop(); if (stack.length) stack[stack.length - 1].expect = 'comma'; }
			else if (current)
			{
				if (part === ',') current.expect = current.kind === '{' ? 'key' : 'value';
				else if (part === ':' && current.expect === 'colon') current.expect = 'value';
				else if (part[0] === '"' && current.expect === 'key') current.expect = 'colon';
				else if (current.expect === 'value' && /^(?:"|-?\d|true$|false$|null$)/.test(part)) current.expect = 'comma';
				else current.expect = 'invalid';
			}
		}
		return stack.length > 0 && stack[stack.length - 1].expect === 'comma';
	}
	infer(text)
	{
		try { const value = JSON.parse(text); if (typeof value === 'number' && !Number.isFinite(value)) return text; return value; }
		catch (error) { return text; }
	}
	valueText(value) { return typeof value === 'string' ? value : JSON.stringify(value); }
	parse(raw)
	{
		try
		{
			const data = JSON.parse(raw, (key, value) => {
				if (typeof value === 'number' && !Number.isFinite(value)) throw new Error('Numbers must be finite.');
				return value;
			});
			this.setState(state => ({ raw: raw, data: data, error: '', collapsed: {}, revision: state.revision + 1 }));
		}
		catch (error) { this.setState({ raw: raw, error: error.message }); }
	}
	get(data, path) { return path.reduce((value, key) => value[key], data); }
	// Define own properties so keys such as __proto__ remain ordinary JSON data.
	put(object, key, value)
	{
		Object.defineProperty(object, key, { value: value, enumerable: true, configurable: true, writable: true });
	}
	change(path, value, remove = false)
	{
		if (this.state.error) return;
		let data = JSON.parse(JSON.stringify(this.state.data));
		if (!path.length) data = remove ? null : value;
		else
		{
			const parent = this.get(data, path.slice(0, -1));
			const key = path[path.length - 1];
			if (remove) { if (Array.isArray(parent)) parent.splice(key, 1); else delete parent[key]; }
			else this.put(parent, key, value);
		}
		this.setState({ data: data, raw: JSON.stringify(data, null, 2) });
	}
	rename(path, name)
	{
		const oldName = path[path.length - 1];
		if (name === oldName) return '';
		const parentPath = path.slice(0, -1);
		const parent = this.get(this.state.data, parentPath);
		if (Object.prototype.hasOwnProperty.call(parent, name)) return 'This property already exists.';
		const replacement = {};
		Object.keys(parent).forEach(key => this.put(replacement, key === oldName ? name : key, parent[key]));
		this.change(parentPath, replacement);
		return '';
	}
	add(path, after = null, name = null, initial = '', focus = false)
	{
		if (this.state.error) return 'Correct the raw JSON before adding a property.';
		const value = JSON.parse(JSON.stringify(this.get(this.state.data, path)));
		if (Array.isArray(value)) value.splice(after === null ? 0 : after + 1, 0, initial);
		else
		{
			if (name === null) { name = 'newKey'; let suffix = 1; while (Object.prototype.hasOwnProperty.call(value, name)) name = 'newKey' + suffix++; }
			if (Object.prototype.hasOwnProperty.call(value, name)) return 'This property already exists.';
			const replacement = {};
			if (after === null) this.put(replacement, name, initial);
			Object.keys(value).forEach(key => { this.put(replacement, key, value[key]); if (key === after) this.put(replacement, name, initial); });
			this.change(path, replacement);
			this.setState(state => ({ collapsed: Object.assign({}, state.collapsed, { [JSON.stringify(path)]: false }) }), () => { if (focus) this.focusRow(path.concat(name)); });
			return '';
		}
		this.change(path, value);
		this.setState(state => ({ collapsed: Object.assign({}, state.collapsed, { [JSON.stringify(path)]: false }) }), () => { if (focus) this.focusRow(path.concat(after === null ? 0 : after + 1)); });
		return '';
	}
	focusRow(path)
	{
		const input = this.rowInputs && this.rowInputs[JSON.stringify(path)];
		if (input) { input.focus(); input.select(); }
	}
	rememberInput(id, input)
	{
		if (!this.rowInputs) this.rowInputs = {};
		this.rowInputs[id] = input;
	}
	togglePane(pane)
	{
		this.setState(state => {
			const next = { rawVisible: state.rawVisible, treeVisible: state.treeVisible };
			next[pane] = !next[pane];
			if (!next.rawVisible && !next.treeVisible) next[pane === 'rawVisible' ? 'treeVisible' : 'rawVisible'] = true;
			return next;
		});
	}
	button(label, onClick, props = {}) { return e('button', Object.assign({ type: 'button', onClick: onClick }, props), label); }
	node(value, path, parentIsArray)
	{
		const type = this.type(value), branch = type === 'object' || type === 'array';
		const id = JSON.stringify(path), closed = !!this.state.collapsed[id];
		const label = path.length ? String(path[path.length - 1]) : 'root';
		const defaults = { string: '', number: 0, boolean: false, null: null, object: {}, array: [] };
		return e('div', { key: id },
			e('div', { className: 'jsoneditor-row jsoneditor-type-' + type },
				branch && this.button(closed ? '+' : '−', () => this.setState(state => ({ collapsed: Object.assign({}, state.collapsed, { [id]: !closed }) })), { 'aria-expanded': !closed, 'aria-label': (closed ? 'Expand ' : 'Collapse ') + label }),
				path.length && !parentIsArray ? e(JsonEditorField, { className: 'jsoneditor-key', value: label, label: 'Property name', inputRef: input => this.rememberInput(id, input), onCommit: name => this.rename(path, name) }) : e('strong', {}, label),
				e(JsonEditorField, { className: 'jsoneditor-value', value: this.valueText(value), label: 'Value of ' + label, inputRef: parentIsArray ? input => this.rememberInput(id, input) : undefined, onInput: draft => this.change(path, this.infer(draft)), onCommit: draft => { this.change(path, this.infer(draft)); return ''; } }),
				e('select', { value: type, 'aria-label': 'Type of ' + label, onChange: event => this.change(path, event.target.value === 'string' ? this.valueText(value) : defaults[event.target.value]) },
					Object.keys(defaults).map(name => e('option', { key: name, value: name }, name))),
				branch && e('span', {}, Object.keys(value).length + (type === 'array' ? ' items' : ' properties')),
				branch && this.button(e('span', { className: 'fa fa-plus', 'aria-hidden': true }), () => this.add(path, null, null, '', true), { className: 'jsoneditor-add', title: type === 'array' ? 'Add item' : 'Add property', 'aria-label': (type === 'array' ? 'Add item to ' : 'Add property to ') + label }),
				path.length > 0 && this.button(e('span', { className: 'fa fa-plus', 'aria-hidden': true }), () => this.add(path.slice(0, -1), path[path.length - 1], null, '', true), { className: 'jsoneditor-add', title: 'Insert after ' + label, 'aria-label': 'Insert after ' + label }),
				path.length > 0 && this.button(e('span', { className: 'fa fa-remove', 'aria-hidden': true }), () => this.change(path, null, true), { className: 'jsoneditor-delete', title: 'Delete ' + label, 'aria-label': 'Delete ' + label })),
			branch && !closed && e('div', { className: 'jsoneditor-children' }, Object.keys(value).map(key => this.node(value[key], path.concat(type === 'array' ? Number(key) : key), type === 'array')),
				e(JsonEditorNewRow, { key: 'draft', array: type === 'array', detectType: text => this.type(this.infer(text)), onAdd: (name, text, selectedType) => {
					const keys = Object.keys(value), after = keys.length ? (type === 'array' ? value.length - 1 : keys[keys.length - 1]) : null;
					return this.add(path, after, name, selectedType === 'string' ? text : this.infer(text));
				} })));
	}
	paneHeader(label, pane)
	{
		const direction = (pane === 'rawVisible') === this.state[pane] ? 'left' : 'right';
		return e('div', { className: 'jsoneditor-pane-header' },
			this.button(e('span', { className: 'fa fa-chevron-' + direction, 'aria-hidden': true }), () => this.togglePane(pane), { className: 'jsoneditor-pane-toggle', title: (this.state[pane] ? 'Hide ' : 'Show ') + label, 'aria-label': (this.state[pane] ? 'Hide ' : 'Show ') + label, 'aria-expanded': this.state[pane] }));
	}
	render()
	{
		return e('div', {},
			e('div', { className: 'jsoneditor-toolbar' },
				this.button('Format JSON', () => this.setState({ raw: JSON.stringify(this.state.data, null, 2) }), { disabled: !!this.state.error })),
			this.state.error && e('p', { className: 'jsoneditor-error', role: 'alert' }, 'Invalid JSON: ' + this.state.error + ' The tree shows the last valid JSON. Correct the raw text to resume editing.'),
			e('div', { className: 'jsoneditor-panes' },
				e('section', { className: 'jsoneditor-pane' + (this.state.rawVisible ? '' : ' jsoneditor-pane-collapsed'), 'aria-label': 'Raw JSON' },
					this.paneHeader('Raw JSON', 'rawVisible'),
					e('div', { className: 'jsoneditor-raw-container', hidden: !this.state.rawVisible },
						e('pre', { className: 'jsoneditor-highlight', 'aria-hidden': true, ref: element => { this.rawHighlight = element; } },
							this.tokens(this.state.raw).map(token => e('span', { key: token.start, className: 'jsoneditor-token-' + token.type }, token.text)), '\n'),
						e('textarea', { className: 'jsoneditor-raw', value: this.state.raw, wrap: 'off', spellCheck: false, autoCapitalize: 'off', autoCorrect: 'off', 'aria-label': 'Raw JSON. Tab selects the next key or value; Shift+Tab selects the previous one. At either end, Tab leaves the editor.', 'aria-invalid': !!this.state.error, ref: element => { this.rawInput = element; }, onScroll: () => this.syncRawScroll(), onKeyDown: event => this.rawKeyDown(event), onChange: event => this.parse(event.target.value) }))),
				e('section', { className: 'jsoneditor-pane' + (this.state.treeVisible ? '' : ' jsoneditor-pane-collapsed'), 'aria-label': 'JSON tree' },
					this.paneHeader('Tree editor', 'treeVisible'), e('div', { className: 'jsoneditor-tree', hidden: !this.state.treeVisible }, e('fieldset', { disabled: !!this.state.error, key: this.state.revision }, this.node(this.state.data, [], false))))));
	}
}
