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
	public abstract void triggerZoom(int zoom);

	/** 
	 * Pan action has been performed
	 * @param panX relative amount to pan in X 
	 * @param panY relative amount to pan in Y 
	 */
	public abstract void triggerPan(int panX, int panY);

	/**
	 * Rotate action has been performed
	 * @param rotateX relative amount to rotate in X 
	 * @param rotateY relative amount to rotate in Y 
 	 * @param rotateZ relative amount to rotate in Z 
	 */
	public abstract void triggerRotate(int rotateX, int rotateY, int rotateZ);

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
	 * @param x
	 * @param y
	 */
	public abstract void point(float x, float y);

	/**
	 * Simulate a mouse click at the curent mouse cursor
	 */
	public abstract void selectMouseCursor();

}
