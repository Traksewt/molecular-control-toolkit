package org.odonoghuelab.molecularcontroltoolkit;

import org.odonoghuelab.molecularcontroltoolkit.internal.EnabledGestureDispatcher;
import org.odonoghuelab.molecularcontroltoolkit.internal.EnabledVoiceDispatcher;
import org.odonoghuelab.molecularcontroltoolkit.internal.InteractiveType;

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
	 * @return
	 */
	public EnabledGestureDispatcher getGestureDispatcher();

	/**
	 * Gets the enabled voice dispatcher
	 * @return
	 */
	public EnabledVoiceDispatcher getVoiceDispatcher();
	
	/**
	 * returns true if the given interactiveType is supported
	 * @param type
	 * @return true if type is supported
	 */
	public boolean supports(InteractiveType type);
}
