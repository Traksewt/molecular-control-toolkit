package org.odonoghuelab.molecularcontroltoolkit;


/**
 * The interface to create an application specific gesture dispatcher 
 * @author KennySabir
 *
 */
public interface GestureListener {


	/** 
	 * Zoom action has been performed
	 * @param zoom relative amount to zoom
	 */
	public abstract void triggerZoom(float zoom);

	/** 
	 * Pan action has been performed
	 * @param panX relative amount to pan in X 
	 * @param panY relative amount to pan in Y 
	 */
	public abstract void triggerPan(float panX, float panY);

	/**
	 * Rotate action has been performed
	 * @param rotateX relative amount to rotate in X 
	 * @param rotateY relative amount to rotate in Y 
 	 * @param rotateZ relative amount to rotate in Z 
	 */
	public abstract void triggerRotate(float rotateX, float rotateY, float rotateZ);

	/**
	 * Reset the view
	 */
	public abstract void reset();

	/**
	 * Zoom to the current selection
	 */
	public abstract void zoomToSelection();
	
	/**
	 * Point the mouse cursor at the given x,y location
	 * @param x x location
	 * @param y y location
	 */
	public abstract void point(float x, float y);

	/**
	 * Simulate a mouse click at the curent mouse cursor
	 */
	public abstract void selectMouseCursor();

}
