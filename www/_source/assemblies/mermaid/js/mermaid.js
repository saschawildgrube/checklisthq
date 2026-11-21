'use strict';

if (!window.MermaidSDK)
{
	window.MermaidSDK = { library: null, error: '' };
	import('https://cdn.jsdelivr.net/npm/mermaid@12.3.0/dist/mermaid.esm.min.mjs').then(module => {
		module.default.initialize(
			{
				startOnLoad: true,
				securityLevel: 'strict',
				suppressErrorRendering: true,
				theme: 'dark',
				fontFamily: 'Consolas, Courier New, monospace'
			});
		window.MermaidSDK.library = module.default;
		window.dispatchEvent(new Event('mermaid-sdk-ready'));
	}).catch(error => {
		window.MermaidSDK.error = String(error.message || error);
		window.dispatchEvent(new Event('mermaid-sdk-error'));
	});
}
