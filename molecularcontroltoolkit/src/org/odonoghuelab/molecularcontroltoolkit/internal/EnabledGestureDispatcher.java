package org.odonoghuelab.molecularcontroltoolkit.internal;

import org.odonoghuelab.molecularcontroltoolkit.GestureListener;

/**
 * GestureDispatcher decorator to allow it to be enabled
 * @author KennySabir
 *
 */
public class EnabledGestureDispatcher implements GestureListener, Enabler{

	
	/** is gestures enabled */
	boolean handsEnabled = true;

	/** the decorated listener */
	GestureListener listener;
	
	/**
	 * Constructor sets up the dispatcher
	 * @param dispatcher
	 */
	public EnabledGestureDispatcher(GestureListener dispatcher) {
		this.listener = dispatcher;
	}

	@Override
	public void triggerPan(int panX, int panY) {
		if (handsEnabled)  {
			listener.triggerPan(panX, panY);
		}

	}

	@Override
	public void triggerZoom(int zoom)
	{
		if (handsEnabled)  {
			listener.triggerZoom(zoom);
		}
	}

	
	@Override
	public void zoomToSelection() {
		if (handsEnabled)  {
			listener.zoomToSelection();
		}
	}

	@Override
	public boolean isEnabled() {
		// TODO Auto-generated method stub
		return false;
	}
	
	@Override
	public void setEnable(boolean enabled)
	{
		handsEnabled = enabled;
		System.out.println("Enabling hands: " + enabled);
	}

	@Override
	public void reset() {
		if (handsEnabled)  {
			listener.reset();
		}
	}

	@Override
	public void triggerRotate(int x, int y, int z) {
		if (handsEnabled)  {
			listener.triggerRotate(x,y,z);
		}
	}
	
	@Override
	public void point(float x, float y) {
		if (handsEnabled)  {
			listener.point(x,y);
		}
		
	}

	@Override
	public void selectMouseCursor() {
		if (handsEnabled) {
			listener.selectMouseCursor();
		}
	}


}
