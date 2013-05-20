package org.odonoghuelab.molecularcontroltoolkit.internal;

import org.odonoghuelab.molecularcontroltoolkit.VoiceListener;

/**
 * VoiceDispatcher decorator to allow it to be enabled
 * @author KennySabir
 *
 */
public class EnabledVoiceDispatcher implements VoiceListener, Enabler {

	/** is voice enabled */
	boolean voiceEnabled = true;
	
	/** the decorated listener */
	VoiceListener listener;

	/**
	 * Constructor sets up the dispatcher
	 * @param dispatcher
	 */
	public EnabledVoiceDispatcher(VoiceListener dispatcher) {
		this.listener = dispatcher;
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
			listener.color(type);
		}
	}

	@Override
	public void copy() {
		if (isEnabled()) {
			listener.copy();
		}
	}

	@Override
	public void paste() {
		if (isEnabled()) {
			listener.paste();
		}
	}

	@Override
	public void select(String value) {
		if (isEnabled()) {
			listener.select(value);
		}

	}

	@Override
	public void selectAll() {
		if (isEnabled()) {
			listener.selectAll();
		}

	}

	public void spin(boolean goCrazy) {
		if (isEnabled()) {
			listener.spin(goCrazy);
		}

	};

	@Override
	public void triggerSpeech(int speechEvent) {
		if (isEnabled()) {
			listener.triggerSpeech(speechEvent);
		}
	}

	@Override
	public void reset() {
		if (isEnabled()) {
			listener.reset();
		}
	}

	@Override
	public void zoomToSelection() {
		if (isEnabled()) {
			listener.zoomToSelection();
		}

	}

	@Override
	public void search(char searchTerm) {
		if (isEnabled()) {
			listener.search(searchTerm);
		}
	}

	
}
