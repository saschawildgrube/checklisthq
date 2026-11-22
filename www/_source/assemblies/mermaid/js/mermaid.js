'use strict';

if (!window.MermaidSDK)
{
	window.MermaidSDK = { library: null, error: '' };

	const whenDocumentReady = callback => {
		if (document.readyState === 'loading')
		{
			document.addEventListener('DOMContentLoaded', callback, { once: true });
		}
		else
		{
			callback();
		}
	};

	const showDiagramError = (node, message) => {
		node.textContent = message;
		node.setAttribute('role', 'alert');
		node.setAttribute('aria-busy', 'false');
		node.classList.add('mermaid-ready', 'mermaid-error');
	};

	const renderPlainDiagrams = async () => {
		for (const node of document.querySelectorAll('.mermaid:not(.mermaid-ready)'))
		{
			node.setAttribute('aria-busy', 'true');
			try
			{
				await window.MermaidSDK.library.run({ nodes: [node] });
				if (!node.querySelector('svg'))
				{
					throw new Error('No diagram was rendered.');
				}
				node.setAttribute('aria-busy', 'false');
				node.classList.add('mermaid-ready');
			}
			catch (error)
			{
				console.error('Mermaid diagram rendering failed:', error);
				showDiagramError(node, 'Unable to render this Mermaid diagram.');
			}
		}
	};

	Promise.resolve().then(() => {
		if (!window.mermaid)
		{
			throw new Error('The local Mermaid distribution has not been loaded.');
		}
		window.mermaid.initialize(
			{
				startOnLoad: false,
				securityLevel: 'strict',
				suppressErrorRendering: true,
				theme: 'dark',
				fontFamily: 'Consolas, Courier New, monospace'
			});
		window.MermaidSDK.library = window.mermaid;
		window.dispatchEvent(new Event('mermaid-sdk-ready'));
		whenDocumentReady(renderPlainDiagrams);
	}).catch(error => {
		console.error('Mermaid SDK loading failed:', error);
		window.MermaidSDK.error = String(error.message || error);
		window.dispatchEvent(new Event('mermaid-sdk-error'));
		whenDocumentReady(() => {
			for (const node of document.querySelectorAll('.mermaid:not(.mermaid-ready)'))
			{
				showDiagramError(node, 'Unable to load the Mermaid renderer.');
			}
		});
	});
}
