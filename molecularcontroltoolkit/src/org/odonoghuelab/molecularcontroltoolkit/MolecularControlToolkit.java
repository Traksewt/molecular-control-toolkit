package org.odonoghuelab.molecularcontroltoolkit;

import java.util.ArrayList;
import java.util.List;

import org.odonoghuelab.molecularcontroltoolkit.internal.ConnectorInternal;
import org.odonoghuelab.molecularcontroltoolkit.internal.EnabledGestureDispatcher;
import org.odonoghuelab.molecularcontroltoolkit.internal.EnabledVoiceDispatcher;
import org.odonoghuelab.molecularcontroltoolkit.internal.InteractiveType;

/**
 * The main entry point for the Molecular Control Toolkit. Connectors can be registered here.
 * Currently supports LeapMotion and Kinect.
 * Allows for application specific dispatchers to be registered. Supports Gesture and Voice Dispatchers
 * 
 * @author KennySabir
 *
 */
public class MolecularControlToolkit {

	/** The application specific gesture dispatcher */
	EnabledGestureDispatcher gestureDispatcher;

	/** The application specific voice dispatcher */
	EnabledVoiceDispatcher voiceDispatcher;
	
	/** The list of enabled connectors  */
	List<ConnectorInternal> connectors = new ArrayList<ConnectorInternal>();
	
	/**
	 * Sets the listeners
	 * @param gestureListener the gesture listener
	 * @param voiceListener the voice listener
	 */
	public void setListeners(GestureListener gestureListener, VoiceListener voiceListener) {
		this.gestureDispatcher = new EnabledGestureDispatcher(gestureListener);
		this.voiceDispatcher = new EnabledVoiceDispatcher(voiceListener);
	}

	
	/**
	 * Sets the Listeners
	 * @param listener a {@link MolecularControlListener} is both a {@link GestureListener} and a {@link VoiceListener}
	 */
	public void setListeners(MolecularControlListener listener) {
		setListeners(listener, listener);
	}

	/**
	 * Creates and adds a connector of the given ConnectorType
	 * @param type the type to create
	 * @return the new connector interface
	 */
	public Connector addConnector(ConnectorType type) {
		ConnectorInternal connector = null;
		try {
			connector = type.newInstance();
			connectors.add(connector);
		} catch (InstantiationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalAccessException  e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return connector;
	}

	/**
	 * initialise all the connectors
	 */
	public void initialise() {
		for (ConnectorInternal connector : connectors) {
			if (connector.supports(InteractiveType.VOICE)) {
				connector.setVoiceDispatcher(voiceDispatcher);
			}
			if (connector.supports(InteractiveType.GESTURE)) {
				connector.setGestureDispatcher(gestureDispatcher);
			}
			connector.setup();
		}
	}
	
}
