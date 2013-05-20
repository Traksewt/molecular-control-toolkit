package org.odonoghuelab.molecularcontroltoolkit.internal;


/**
 * Helper abstract class to setup the dispatchers for a connector
 * @author KennySabir
 *
 */
public abstract class AbstractConnector implements Connector {
	
	
	/** The gesture dispatcher */
	EnabledGestureDispatcher gestureDispatcher;
	
	/** The voice dispatcher */
	EnabledVoiceDispatcher voiceDispatcher;

	/**
	 * force subclasses to implement shutdown()
	 */
	public abstract void shutdown();
	
	
	@Override
	protected void finalize() throws Throwable {
		shutdown();
		super.finalize();
	}
	


	@Override
	public void setGestureDispatcher(EnabledGestureDispatcher dispatcher) {
		this.gestureDispatcher = dispatcher;
	}
	
	@Override
	public EnabledGestureDispatcher getGestureDispatcher() {
		return gestureDispatcher;
	}
	
	@Override
	public void setVoiceDispatcher(EnabledVoiceDispatcher voiceDispatcher) {
		this.voiceDispatcher = voiceDispatcher;
	}
	
	@Override
	public EnabledVoiceDispatcher getVoiceDispatcher() {
		return voiceDispatcher;
	}

}
