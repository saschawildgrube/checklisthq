'use strict';

class MermaidEditor extends MermaidReactComponent
{
	constructor(props)
	{
		super(props);
		let code = '';
		if (typeof props.code === 'string')
		{
			code = props.code;
		}
		this.state = { source: code, svg: '', error: '', pending: true, fullscreen: false, sourceVisible: true, previewVisible: true, split: 50, renderRequest: 0 };
	}
	componentDidMount()
	{
		super.componentDidMount();
		this.escape = event => { if (event.key === 'Escape' && this.state.fullscreen) this.setFullscreen(false); };
		document.addEventListener('keydown', this.escape);
	}
	componentDidUpdate(previous)
	{
		if (previous.code !== this.props.code)
		{
			let code = '';
			if (typeof this.props.code === 'string')
			{
				code = this.props.code;
			}
			this.changeSource(code);
		}
	}
	componentWillUnmount()
	{
		document.removeEventListener('keydown', this.escape);
		if (this.state.fullscreen) document.body.style.overflow = this.previousBodyOverflow;
		super.componentWillUnmount();
	}
	setFullscreen(enabled)
	{
		if (enabled === this.state.fullscreen) return;
		if (enabled)
		{
			this.previousBodyOverflow = document.body.style.overflow;
			document.body.style.overflow = 'hidden';
		}
		else document.body.style.overflow = this.previousBodyOverflow;
		this.setState({ fullscreen: enabled });
	}
	changeSource(source)
	{
		this.setState({ source: source, pending: true, error: '' });
	}
	updateRenderState(state, code)
	{
		this.setState(state, () => {
			if (state.pending === false && state.error === '' && state.svg && code === this.state.source)
			{
				const callback = this.getCallback(this.props.callbackUpdate);
				if (typeof callback === 'function')
				{
					callback(code);
				}
			}
		});
	}
	getCallback(callback)
	{
		if (typeof callback === 'string')
		{
			callback = window[callback];
		}
		if (typeof callback === 'function')
		{
			return callback;
		}
		return null;
	}
	save()
	{
		const callback = this.getCallback(this.props.callbackSave);
		if (callback)
		{
			callback(this.state.source);
		}
	}
	download(contents, type, filename)
	{
		const url = URL.createObjectURL(new Blob([contents], { type: type }));
		const link = document.createElement('a');
		link.href = url;
		link.download = filename;
		document.body.appendChild(link);
		link.click();
		link.remove();
		setTimeout(() => URL.revokeObjectURL(url), 1000);
	}
	togglePane(pane)
	{
		this.resizing = false;
		this.setState(state => {
			const next = { sourceVisible: state.sourceVisible, previewVisible: state.previewVisible };
			next[pane] = !next[pane];
			if (!next.sourceVisible && !next.previewVisible) next[pane === 'sourceVisible' ? 'previewVisible' : 'sourceVisible'] = true;
			return next;
		});
	}
	paneHeader(label, pane)
	{
		const direction = (pane === 'sourceVisible') === this.state[pane] ? 'left' : 'right';
		const action = (this.state[pane] ? 'Hide ' : 'Show ') + label;
		return e('div', { className: 'mermaideditor-heading' },
			e('span', { hidden: !this.state[pane] }, label),
			e('button', { type: 'button', className: 'mermaideditor-pane-toggle', title: action, 'aria-label': action, 'aria-expanded': this.state[pane], onClick: () => this.togglePane(pane) },
				e('span', { className: 'fa fa-chevron-' + direction, 'aria-hidden': true })));
	}
	startResize(event)
	{
		if (event.button !== 0) return;
		event.preventDefault();
		event.currentTarget.setPointerCapture(event.pointerId);
		this.resizing = true;
	}
	resize(event)
	{
		if (!this.resizing || !this.panes) return;
		const rect = this.panes.getBoundingClientRect();
		if (rect.width > 0) this.setState({ split: Math.max(25, Math.min(75, (event.clientX - rect.left) / rect.width * 100)) });
	}
	button(label, action, disabled = false)
	{
		return e('button', { type: 'button', onClick: action, disabled: disabled }, label);
	}
	renderMermaid()
	{
		const code = this.state.source;
		let viewLabel = 'Full screen';
		if (this.state.fullscreen)
		{
			viewLabel = 'Normal view';
		}
		return e('div', { className: 'mermaideditor-editor' + (this.state.fullscreen ? ' mermaideditor-fullscreen' : '') },
			e('div', { className: 'mermaideditor-toolbar', role: 'toolbar', 'aria-label': 'Mermaid editor controls' },
				this.button('Download source', () => this.download(this.state.source, 'text/plain;charset=utf-8', 'diagram.mmd')),
				this.button('Download SVG', () => this.download(this.state.svg, 'image/svg+xml;charset=utf-8', 'diagram.svg'), !this.state.svg || this.state.pending || !!this.state.error),
				e('a', { className: 'mermaideditor-docs', href: 'https://mermaid.js.org/intro/', target: '_blank', rel: 'noopener noreferrer' }, 'Mermaid docs'),
				e('span', { className: 'mermaideditor-view-controls' },
					this.button(viewLabel, () => this.setFullscreen(!this.state.fullscreen)),
					this.getCallback(this.props.callbackSave) && e('button', { type: 'button', className: 'mermaideditor-save', onClick: () => this.save() }, 'Save'))),
			e('div', { className: 'mermaideditor-panes', ref: element => { this.panes = element; } },
				e('section', { style: this.state.sourceVisible ? { flexGrow: this.state.previewVisible ? this.state.split : 1 } : {}, className: 'mermaideditor-pane' + (this.state.sourceVisible ? '' : ' mermaideditor-pane-collapsed'), 'aria-label': 'Mermaid source' },
					this.paneHeader('Mermaid source', 'sourceVisible'),
					e('textarea', { hidden: !this.state.sourceVisible, value: this.state.source, wrap: 'off', spellCheck: false, autoCapitalize: 'off', autoCorrect: 'off', 'aria-label': 'Mermaid source', 'aria-invalid': !!this.state.error, onChange: event => this.changeSource(event.target.value) })),
				this.state.sourceVisible && this.state.previewVisible && e('div', {
					className: 'mermaideditor-divider', role: 'separator', tabIndex: 0, 'aria-label': 'Resize editor panes', 'aria-orientation': 'vertical', 'aria-valuemin': 25, 'aria-valuemax': 75, 'aria-valuenow': Math.round(this.state.split),
					onPointerDown: event => this.startResize(event), onPointerMove: event => this.resize(event),
					onPointerUp: () => { this.resizing = false; }, onPointerCancel: () => { this.resizing = false; }, onLostPointerCapture: () => { this.resizing = false; },
					onKeyDown: event => {
						if (event.key === 'ArrowLeft' || event.key === 'ArrowRight')
						{
							event.preventDefault();
							this.setState(state => ({ split: Math.max(25, Math.min(75, state.split + (event.key === 'ArrowLeft' ? -5 : 5))) }));
						}
					}
				}),
				e(MermaidViewer, {
					source: this.state.source, renderRequest: this.state.renderRequest, visible: this.state.previewVisible,
					onRenderState: state => this.updateRenderState(state, code),
					sourceVisible: this.state.sourceVisible, split: this.state.split,
					header: this.paneHeader('Diagram preview', 'previewVisible') })),
			e('div', { className: 'mermaideditor-status', role: 'status' },
				this.state.pending ? 'Rendering...' : this.state.error ? 'Preview is out of date' : this.state.svg ? 'Diagram rendered' : 'Enter Mermaid source to begin'),
			this.state.error && e('div', { className: 'mermaideditor-error', role: 'alert' },
				e('p', {}, 'Unable to render this source. The preview retains the last valid diagram. Correct the source to update the preview.'),
				e('pre', {}, this.state.error)));
	}
}
