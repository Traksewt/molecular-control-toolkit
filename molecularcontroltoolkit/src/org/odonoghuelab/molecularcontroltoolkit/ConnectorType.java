package org.odonoghuelab.molecularcontroltoolkit;

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
	 * @param clazz
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
	public Connector newInstance() throws InstantiationException, IllegalAccessException {
		return typeClass.newInstance();
	}
}
