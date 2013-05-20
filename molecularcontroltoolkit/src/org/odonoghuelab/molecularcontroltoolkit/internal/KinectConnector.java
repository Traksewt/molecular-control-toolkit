package org.odonoghuelab.molecularcontroltoolkit.internal;


/**
 * The Connector for the Kinect interface. Supports Gestures & Voice.
 * This class connects to a native C++ DLL that accesses the Kinect API.
 * A thread is created in the C++ DLL that will monitor Kinect events.
 * When a Kinect event occurs, the KinectConnector is called using JNI to dispatch
 * the message. A single instance is tracked upon creation through a static variable
 * because JNI had trouble accessing the java objects from a newly created thread.
 * @author KennySabir
 *
 */
public class KinectConnector extends AbstractConnector {
	
	/** Setup the Kinect interface */
	private native void registerKinect();
	
	/** Shutdown the Kinect interface */
	private native void deregisterKinect();

	/** Holder variable to the current KinectConnector. Used for the static method callbacks */
	private static KinectConnector instance = null;
	
	/** The X & Y rotation speed */
	static final float ROTATION_XY_SCALE = 0.15f;

	/** The Z rotation speed */
	static final float ROTATION_Z_SCALE = 1.15f;

	@Override
	public void shutdown() {
		deregisterKinect();
	}
	
	/**
	 * Constructor, store a reference to this object on the static variable instance
	 */
	public KinectConnector() {
		instance = this;
	}
	
	@Override
	protected void finalize() throws Throwable {
		deregisterKinect();
		super.finalize();
	}
	
	/**
	 * Setup the Kinect & DLL
	 */
	public void setup()
	{
		System.out.println("Initialising Kinect from Java");
		System.loadLibrary("KinectMolecularControl");
		registerKinect();
	}

	/** 
	 * Zoom action has been performed
	 * @param zoom relative amount to zoom
	 */
	public static void triggerZoom(int zoom)
	{
		instance.getGestureDispatcher().triggerZoom(zoom);
	}
	
	
	/** 
	 * Pan action has been performed
	 * @param panX relative amount to pan in X 
	 * @param panY relative amount to pan in Y 
	 */
	public static void triggerPan(int panX, int panY)
	{
		instance.getGestureDispatcher().triggerPan(panX, panY);
	}

	/**
	 * Rotate action has been performed
	 * @param rotateX relative amount to rotate in X 
	 * @param rotateY relative amount to rotate in Y 
 	 * @param rotateZ relative amount to rotate in Z 
	 */
	public static void triggerRotate(int rotateX, int rotateY, int rotateZ)
	{
		instance.getGestureDispatcher().triggerRotate((int) (rotateX * ROTATION_XY_SCALE), (int)(rotateY * ROTATION_XY_SCALE), (int)( rotateZ * ROTATION_Z_SCALE));
	}
	
	/**
	 * Speech event occurred, currently NOT USED.
	 * @param speechEvent
	 */
	public static void triggerSpeech(int speechEvent)
	{
		instance.getVoiceDispatcher().triggerSpeech(speechEvent);
	}

	/**
	 * Enable/Disable voice commands
	 * @param enabled
	 */
	public static void enableVoice(boolean enabled)
	{
		instance.getVoiceDispatcher().setEnable(enabled);
	}

	/**
	 * Enable/Disable gesture commands
	 * @param enabled
	 */
	public static void enableHands(boolean enabled)
	{
		instance.getGestureDispatcher().setEnable(enabled);
	}

	/**
	 * Reset the view
	 */
	public static void reset()
	{
		instance.getVoiceDispatcher().reset();
	}

	/**
	 * Search for a term
	 * @param searchTerm
	 */
	public static void search(char searchTerm)
	{
		instance.getVoiceDispatcher().search(searchTerm);
	}

	/**
	 * Zoom to the current selection
	 */
	public static void zoom()
	{
		instance.getVoiceDispatcher().zoomToSelection();
	}

	/**
	 * Point the mouse cursor at the given x,y location
	 * @param x
	 * @param y
	 */
	public static void point(float x, float y)
	{
		instance.getGestureDispatcher().point(x, y);
	}

	/**
	 * Set the coloring
	 * @param type the colour type
	 */
	public static void color(String type)
	{
		instance.getVoiceDispatcher().color(type);
	}
	
	/**
	 * Rotate
	 * @param goCrazy true if the spin should be crazy fast.
	 */
	public static void spin(boolean goCrazy)
	{
		instance.getVoiceDispatcher().spin(goCrazy);
	}
	
	/**
	 * copy the current selection to the clipboard
	 */
	public static void copy() {
		instance.getVoiceDispatcher().copy();
		
	}
	
	/**
	 * paste the current selection from the clipboard
	 */
	public static void paste() {
		instance.getVoiceDispatcher().paste();
	}
	
	/**
	 * select all residues
	 */
	public static void selectAll()
	{
		instance.getVoiceDispatcher().selectAll();
	}

	/**
	 * Select a specific target
	 * @param value
	 */
	public static void select(String value)
	{
		instance.getVoiceDispatcher().select(value);
		
	}
	
	@Override
	public boolean supports(InteractiveType type) {
		if (type == InteractiveType.VOICE || type == InteractiveType.GESTURE) {
			return true;
		}
		return false;
	}
}
