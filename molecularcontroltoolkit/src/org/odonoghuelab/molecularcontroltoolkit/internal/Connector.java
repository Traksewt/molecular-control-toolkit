package org.odonoghuelab.molecularcontroltoolkit.internal;


/**
 * The interface all connectors must implement to handle the connector initialisation.
 * @author KennySabir
 *
 */
public interface Connector {

	/** handle the connector setup */
	public void setup();
	
	/**
	 * Set the gesture dispatcher
	 * @param dispatcher
	 */
	public void setGestureDispatcher(EnabledGestureDispatcher dispatcher);
	
	/**
	 * Set the voice dispatcher
	 * @param dispatcher
	 */
	public void setVoiceDispatcher(EnabledVoiceDispatcher dispatcher);

	/**
	 * Gets the enabled gesture dispatcher
	 * @return the gesture dispatcher
	 */
	public EnabledGestureDispatcher getGestureDispatcher();

	/**
	 * Gets the enabled voice dispatcher
	 * @return the voice dispatcher
	 */
	public EnabledVoiceDispatcher getVoiceDispatcher();
	
	/**
	 * returns true if the given interactiveType is supported
	 * @param type
	 * @return true if type is supported
	 */
	public boolean supports(InteractiveType type);
}
