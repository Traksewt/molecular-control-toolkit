package org.odonoghuelab.molecularcontroltoolkit.internal;

import org.odonoghuelab.molecularcontroltoolkit.VoiceDispatcher;

/**
 * VoiceDispatcher decorator to allow it to be enabled
 * @author KennySabir
 *
 */
public class EnabledVoiceDispatcher implements VoiceDispatcher, Enabler {

	/** is voice enabled */
	boolean voiceEnabled = true;
	
	/** the decorated dispatcher */
	VoiceDispatcher dispatcher;

	/**
	 * Constructor sets up the dispatcher
	 * @param dispatcher
	 */
	public EnabledVoiceDispatcher(VoiceDispatcher dispatcher) {
		this.dispatcher = dispatcher;
	}

	@Override
	public void setEnable(boolean enabled) {
		voiceEnabled = enabled;

	}

	public boolean isEnabled() {
		return voiceEnabled;
	};

	@Override
	public void color(String type) {
		if (isEnabled()) {
			dispatcher.color(type);
		}
	}

	@Override
	public void copy() {
		if (isEnabled()) {
			dispatcher.copy();
		}
	}

	@Override
	public void paste() {
		if (isEnabled()) {
			dispatcher.paste();
		}
	}

	@Override
	public void select(String value) {
		if (isEnabled()) {
			dispatcher.select(value);
		}

	}

	@Override
	public void selectAll() {
		if (isEnabled()) {
			dispatcher.selectAll();
		}

	}

	public void spin(boolean goCrazy) {
		if (isEnabled()) {
			dispatcher.spin(goCrazy);
		}

	};

	@Override
	public void triggerSpeech(int speechEvent) {
		if (isEnabled()) {
			dispatcher.triggerSpeech(speechEvent);
		}
	}

	@Override
	public void reset() {
		if (isEnabled()) {
			dispatcher.reset();
		}
	}

	@Override
	public void zoomToSelection() {
		if (isEnabled()) {
			dispatcher.zoomToSelection();
		}

	}

	@Override
	public void search(char searchTerm) {
		if (isEnabled()) {
			dispatcher.search(searchTerm);
		}
	}

	
}
