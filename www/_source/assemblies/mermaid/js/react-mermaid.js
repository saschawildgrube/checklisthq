'use strict';

class MermaidReactComponent extends WDKReactComponent
{
	constructor(props)
	{
		super(props);
		this.handleMermaidReady = this.handleMermaidReady.bind(this);
		this.handleMermaidError = this.handleMermaidError.bind(this);
	}
	componentDidMount()
	{
		super.componentDidMount();
		window.addEventListener('mermaid-sdk-ready', this.handleMermaidReady);
		window.addEventListener('mermaid-sdk-error', this.handleMermaidError);
		if (this.isMermaidLoaded())
		{
			this.onMermaidReady();
		}
	}
	componentWillUnmount()
	{
		window.removeEventListener('mermaid-sdk-ready', this.handleMermaidReady);
		window.removeEventListener('mermaid-sdk-error', this.handleMermaidError);
		super.componentWillUnmount();
	}
	isMermaidLoaded()
	{
		return !!(window.MermaidSDK && window.MermaidSDK.library);
	}
	handleMermaidReady()
	{
		if (this.IsMounted())
		{
			this.forceUpdate();
			this.onMermaidReady();
		}
	}
	handleMermaidError()
	{
		if (this.IsMounted())
		{
			this.forceUpdate();
		}
	}
	onMermaidReady()
	{
	}
	render()
	{
		if (!this.isMermaidLoaded())
		{
			let message = 'Mermaid SDK has not been loaded.';
			if (window.MermaidSDK && window.MermaidSDK.error)
			{
				message = 'Unable to load the Mermaid SDK: ' + window.MermaidSDK.error;
			}
			return e('div', { className: 'mermaid-error', role: 'alert' }, message);
		}
		return this.renderMermaid();
	}
}
