package org.odonoghuelab.molecularcontroltoolkit;

import org.odonoghuelab.molecularcontroltoolkit.internal.Connector;
import org.odonoghuelab.molecularcontroltoolkit.internal.KinectConnector;
import org.odonoghuelab.molecularcontroltoolkit.internal.LeapMotionConnector;

/**
 * The enumeration for all possible connectors
 * @author KennySabir
 *
 */
public enum ConnectorType {
	Kinect(KinectConnector.class), LeapMotion(LeapMotionConnector.class);

	/** The class of the connector to instantiate */
	Class< ? extends Connector> typeClass;
	
	/**
	 * Constructor
	 * @param clazz the connector class
	 */
	private ConnectorType(Class< ? extends Connector> clazz) {
		typeClass = clazz;
	}
	
	/**
	 * Create a new instance of the given type class
	 * @return the new instance
	 * @throws InstantiationException
	 * @throws IllegalAccessException
	 */
	protected Connector newInstance() throws InstantiationException, IllegalAccessException {
		return typeClass.newInstance();
	}
}
