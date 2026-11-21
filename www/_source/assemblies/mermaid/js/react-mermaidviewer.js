'use strict';

class MermaidViewer extends MermaidReactComponent
{
	constructor(props)
	{
		super(props);
		this.state = { svg: '', error: '', pending: true };
		this.revision = 0;
	}
	onMermaidReady()
	{
		this.scheduleRender();
	}
	componentDidUpdate(previous)
	{
		if (previous.source !== this.props.source || previous.renderRequest !== this.props.renderRequest)
		{
			this.scheduleRender();
		}
	}
	componentWillUnmount()
	{
		clearTimeout(this.timer);
		this.revision++;
		super.componentWillUnmount();
	}
	updateRenderState(state)
	{
		this.setState(state);
		if (typeof this.props.onRenderState === 'function')
		{
			this.props.onRenderState(state);
		}
	}
	scheduleRender()
	{
		clearTimeout(this.timer);
		if (!this.isMermaidLoaded())
		{
			return;
		}
		const revision = ++this.revision;
		let source = '';
		if (typeof this.props.source === 'string')
		{
			source = this.props.source;
		}
		this.updateRenderState({ pending: true, error: '' });
		this.timer = setTimeout(() => this.renderDiagram(source, revision), 300);
	}
	async renderDiagram(source, revision)
	{
		const current = () => this.IsMounted() && revision === this.revision;
		try
		{
			if (!source.trim())
			{
				if (current())
				{
					this.updateRenderState({ svg: '', error: '', pending: false });
				}
				return;
			}
			const mermaid = window.MermaidSDK.library;
			if (!current())
			{
				return;
			}
			await mermaid.parse(source);
			if (!current())
			{
				return;
			}
			const result = await mermaid.render('mermaid_' + this.m_strId + '_' + revision, source);
			if (current())
			{
				this.updateRenderState({ svg: result.svg, error: '', pending: false });
			}
		}
		catch (error)
		{
			if (current())
			{
				this.updateRenderState({ error: String(error.message || error), pending: false });
			}
		}
	}
	renderMermaid()
	{
		const visible = this.props.visible !== false;
		const style = {};
		let className = 'mermaidviewer-pane';
		if (visible)
		{
			style.flexGrow = 1;
			if (this.props.sourceVisible)
			{
				style.flexGrow = 100 - this.props.split;
			}
		}
		else
		{
			className += ' mermaidviewer-pane-collapsed';
		}
		return e('section', { style: style, className: className, 'aria-label': 'Diagram preview', 'aria-busy': this.state.pending },
			this.props.header,
			e('div', { className: 'mermaidviewer-preview', hidden: !visible, dangerouslySetInnerHTML: { __html: this.state.svg } }));
	}
}
