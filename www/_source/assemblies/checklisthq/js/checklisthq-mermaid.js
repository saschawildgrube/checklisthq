'use strict';

function initializeChecklistHQMermaidEditors()
{
	for (const diagram of document.querySelectorAll('div.mermaid[data-checklisthq-mermaid-url]'))
	{
		if (diagram.dataset.checklisthqMermaidInitialized)
		{
			continue;
		}
		diagram.dataset.checklisthqMermaidInitialized = 'true';
		const container = document.createElement('div');
		container.style.position = 'relative';
		diagram.before(container);
		container.append(diagram);
		const button = document.createElement('button');
		button.type = 'button';
		button.textContent = 'Edit';
		button.className = 'mermaideditor-edit';
		button.setAttribute('aria-label', 'Edit Mermaid diagram');
		button.style.position = 'absolute';
		button.style.top = '0.5rem';
		button.style.right = '0.5rem';
		button.style.zIndex = '1';
		container.append(button);
		button.addEventListener('click', () => {
			const editor = document.createElement('div');
			editor.className = 'wdk-mermaideditor';
			container.append(editor);
			RenderReactElement(e(MermaidEditor, {
				code: decodeURIComponent(diagram.dataset.checklisthqMermaidCode),
				callbackSave: code => {
					const form = document.createElement('form');
					form.method = 'post';
					form.action = diagram.dataset.checklisthqMermaidUrl;
					const content = document.createElement('textarea');
					content.name = diagram.dataset.checklisthqMermaidField;
					content.value = code;
					form.append(content);
					form.hidden = true;
					document.body.append(form);
					form.submit();
				}
			}), editor);
			diagram.hidden = true;
			button.hidden = true;
		});
	}
}

if (document.readyState === 'loading')
{
	document.addEventListener('DOMContentLoaded', initializeChecklistHQMermaidEditors, { once: true });
}
else
{
	initializeChecklistHQMermaidEditors();
}
