package org.odonoghuelab.molecularcontroltoolkit;

import org.odonoghuelab.molecularcontroltoolkit.internal.ConnectorInternal;
import org.odonoghuelab.molecularcontroltoolkit.internal.KinectConnector;
import org.odonoghuelab.molecularcontroltoolkit.internal.LeapMotionConnector;
import org.odonoghuelab.molecularcontroltoolkit.internal.LeapMotionJSConnector;

/**
 * The enumeration for all possible connectors
 * @author KennySabir
 *
 */
public enum ConnectorType {
	Kinect(KinectConnector.class), LeapMotion(LeapMotionConnector.class), LeapMotionJS(LeapMotionJSConnector.class);

	/** The class of the connector to instantiate */
	Class< ? extends ConnectorInternal> typeClass;
	
	/**
	 * Constructor
	 * @param clazz the connector class
	 */
	private ConnectorType(Class< ? extends ConnectorInternal> clazz) {
		typeClass = clazz;
	}
	
	/**
	 * Create a new instance of the given type class
	 * @return the new instance
	 * @throws InstantiationException problem creating new instance
	 * @throws IllegalAccessException problem creating new instance
	 */
	protected ConnectorInternal newInstance() throws InstantiationException, IllegalAccessException {
		return typeClass.newInstance();
	}
}
