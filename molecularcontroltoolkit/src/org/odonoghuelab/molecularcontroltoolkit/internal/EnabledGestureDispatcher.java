package org.odonoghuelab.molecularcontroltoolkit.internal;

import org.odonoghuelab.molecularcontroltoolkit.GestureDispatcher;

/**
 * GestureDispatcher decorator to allow it to be enabled
 * @author KennySabir
 *
 */
public class EnabledGestureDispatcher implements GestureDispatcher, Enabler{

	
	/** is gestures enabled */
	boolean handsEnabled = true;

	/** the decorated dispatcher */
	GestureDispatcher dispatcher;
	
	/**
	 * Constructor sets up the dispatcher
	 * @param dispatcher
	 */
	public EnabledGestureDispatcher(GestureDispatcher dispatcher) {
		this.dispatcher = dispatcher;
	}

	@Override
	public void triggerPan(int panX, int panY) {
		if (handsEnabled)  {
			dispatcher.triggerPan(panX, panY);
		}

	}

	@Override
	public void triggerZoom(int zoom)
	{
		if (handsEnabled)  {
			dispatcher.triggerZoom(zoom);
		}
	}

	
	@Override
	public void zoomToSelection() {
		if (handsEnabled)  {
			dispatcher.zoomToSelection();
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
			dispatcher.reset();
		}
	}

	@Override
	public void triggerRotate(int x, int y, int z) {
		if (handsEnabled)  {
			dispatcher.triggerRotate(x,y,z);
		}
	}
	
	@Override
	public void point(float x, float y) {
		if (handsEnabled)  {
			dispatcher.point(x,y);
		}
		
	}

	@Override
	public void selectMouseCursor() {
		if (handsEnabled) {
			dispatcher.selectMouseCursor();
		}
	}


}
