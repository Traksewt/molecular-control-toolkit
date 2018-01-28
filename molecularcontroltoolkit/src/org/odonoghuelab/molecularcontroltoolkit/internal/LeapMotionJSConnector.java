package org.odonoghuelab.molecularcontroltoolkit.internal;

import org.odonoghuelab.molecularcontroltoolkit.TunnellingConnector;



/**
 * The LeapMotionConnector accesses the Leap Motion Controller using the Leap Motion Java API.
 * @author KennySabir
 *
 */
public class LeapMotionJSConnector extends AbstractConnector implements TunnellingConnector {

	/**
	 * Shutdown the interface
	 */
	public void shutdown() {
	}
	
	/**
	 * Setup the interface
	 */
	public void setup()
	{
		System.err.println("Initialising Leap Motion JS");
	}
	
	public void tunnel(String method, float[] args) {
		if (method.equalsIgnoreCase("triggerZoom")) {
			getGestureDispatcher().triggerZoom(args[0]);
		}
		else if (method.equalsIgnoreCase("triggerPan")) {
			getGestureDispatcher().triggerPan(args[0], args[1]);
		}
		else if (method.equalsIgnoreCase("triggerRotate")) {
			getGestureDispatcher().triggerRotate( args[0], args[1], args[2]);
		}
		else if (method.equalsIgnoreCase("reset")) {
			getGestureDispatcher().reset();
		}
		else if (method.equalsIgnoreCase("zoomToSelection")) {
			getGestureDispatcher().zoomToSelection();
		}
		else if (method.equalsIgnoreCase("selectMouseCursor")) {
			getGestureDispatcher().selectMouseCursor();
		}
		
	}

	
	@Override
	public boolean supports(InteractiveType type) {
		if (type == InteractiveType.GESTURE) {
			return true;
		}
		return false;
	}

}
