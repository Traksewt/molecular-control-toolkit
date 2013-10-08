package org.odonoghuelab.molecularcontroltoolkit.internal;
/******************************************************************************\
* Copyright (C) 2012-2013 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

import java.awt.IllegalComponentStateException;
import java.util.Timer;
import java.util.TimerTask;


import com.leapmotion.leap.Controller;
import com.leapmotion.leap.Frame;
import com.leapmotion.leap.Gesture;
import com.leapmotion.leap.GestureList;
import com.leapmotion.leap.Hand;
import com.leapmotion.leap.Listener;
import com.leapmotion.leap.Pointable;
import com.leapmotion.leap.Vector;

/**
 * The LeapMotionConnector accesses the Leap Motion Controller using the Leap Motion Java API.
 * @author KennySabir
 *
 */
public class LeapMotionConnector extends AbstractConnector {

	/** the leap motion controller interface */
	Controller controller;
	
	/** The custom leap motion listener for callbacks */
	LeapMotionListener listener;
	
	/**
	 * Shutdown the interface
	 */
	public void shutdown() {
        controller.removeListener(listener);
	}
	
	/**
	 * Setup the interface
	 */
	public void setup()
	{
		System.err.println("Initialising Leap Motion from Java");
		if (controller != null) {
			shutdown();
		}
		else {
	        controller = new Controller();
		}

		listener = new LeapMotionListener();

        // Have the sample listener receive events from the controller
        controller.addListener(listener);
	}

	/**
	 * The Leap Motion Listener to handle the callbacks for events
	 * @author KennySabir
	 *
	 */
	class LeapMotionListener extends Listener {
		
		/** Store the last frame to monitor rotation and translation movements */
		Frame lastFrame = null;
		
		/** Store the lastX for custom pointing algorithm */
		float lastX = -1;
		
		/** Store the lastY for custom pointing algorithm */
		float lastY = -1;
		
		/** constant for scaling pointing in the x axis */
		float X_SCALE = 150;

		/** constant for scaling pointing in the y axis */
		float Y_SCALE = -400;
		
		/** constant for offsetting pointing in the y axis */
		float Y_OFFSET = 400;

		/*
		 * (non-Javadoc)
		 * @see com.leapmotion.leap.Listener#onInit(com.leapmotion.leap.Controller)
		 */
		public void onInit(Controller controller) {
	        System.out.println("Initialized");
	    }

		/*
		 * (non-Javadoc)
		 * @see com.leapmotion.leap.Listener#onConnect(com.leapmotion.leap.Controller)
		 */
	    public void onConnect(Controller controller) {
	        System.out.println("Connected");
	        controller.enableGesture(Gesture.Type.TYPE_SCREEN_TAP);
	        controller.enableGesture(Gesture.Type.TYPE_KEY_TAP);
	    }

	    /*
	     * (non-Javadoc)
	     * @see com.leapmotion.leap.Listener#onDisconnect(com.leapmotion.leap.Controller)
	     */
	    public void onDisconnect(Controller controller) {
	        //Note: not dispatched when running in a debugger.
	        System.out.println("Disconnected");
	    }

	    /*
	     * (non-Javadoc)
	     * @see com.leapmotion.leap.Listener#onExit(com.leapmotion.leap.Controller)
	     */
	    public void onExit(Controller controller) {
	        System.out.println("Exited");
	    }

	    /*
	     * (non-Javadoc)
	     * @see com.leapmotion.leap.Listener#onFrame(com.leapmotion.leap.Controller)
	     */
	    public void onFrame(Controller controller) {
	        // Get the most recent frame and report some basic information
	        Frame frame = controller.frame();
	        if (frame.hands().count() == 1 ) {
	        	if (frame.fingers().count() > 3 && lastFrame != null) {
	        		// rotate
    	        	lastY = -1;
    	        	lastX = -1;

	        		Hand hand = frame.hands().frontmost();
//	        		float rotateProb = hand.rotationProbability(lastFrame);
	        		Vector translation = hand.translation(lastFrame);
	        		getGestureDispatcher().triggerPan((int) translation.getX(), -(int) translation.getY());
	        		getGestureDispatcher().triggerZoom((int) -translation.getZ());
//        			Matrix matrix = hand.rotationMatrix(lastFrame);
    				int ROTATION_SCALE = 500;
    				float xRotation = hand.rotationAngle(lastFrame, new Vector(1,0,0));
    				float yRotation = hand.rotationAngle(lastFrame, new Vector(0,1,0));
    				float zRotation = hand.rotationAngle(lastFrame, new Vector(0,0,1));
    				getGestureDispatcher().triggerRotate((int) -(ROTATION_SCALE * xRotation), -(int) (ROTATION_SCALE * yRotation) , -(int)(ROTATION_SCALE * zRotation));

	        	}
	        	else if (frame.fingers().count() > 0){
	        		
		        	try {
			        	Pointable finger = frame.fingers().frontmost();
			        	Vector hit = controller.locatedScreens().closestScreenHit(finger).intersect(finger,true);
			        	float zVel = finger.tipVelocity().getZ();
			    		int absZVel = (int) Math.abs(zVel);
			    		float x = hit.getX();
			    		float y = hit.getY();
			    		
//			        	Pointable lastFinger = lastFrame.fingers().frontmost();
//			        	Vector lastHit = controller.locatedScreens().closestScreenHit(lastFinger).intersect(lastFinger,true);
//			        	System.out.println(finger + ": hit, last hit, xy: " + hit.getX() + ", " + hit.getY() );
			        	if (absZVel > 50 && lastY != -1) {
			        		float scale = (float) 2500 / (absZVel*absZVel);
				        	x = (hit.getX() - lastX) * scale + lastX;
				        	y = (hit.getY() - lastY) * scale + lastY;
			        	}
			        	lastY = y;
			        	lastX = x;
			        	if (!Float.isNaN(x) && !Float.isNaN(y)) {
				        	getGestureDispatcher().point(x, y);
			        	}
		        	}
		        	catch (IllegalComponentStateException e) {
		        		e.printStackTrace();
		        	}
		        }
	        	else {
		        	lastY = -1;
		        	lastX = -1;

	        	}
	
		        GestureList gestures = frame.gestures();
		        for (int i = 0; i < gestures.count(); i++) {
		            Gesture gesture = gestures.get(i);
	
		            switch (gesture.type()) {
		                case TYPE_SWIPE:
		                	getGestureDispatcher().reset();
		                    break;
		                case TYPE_SCREEN_TAP:
	
		                	getGestureDispatcher().selectMouseCursor();
		                	Timer timer = new Timer();
		                	// add a delay so the app can process the mouse clicks.
							timer.schedule(new TimerTask() {
								
								@Override
								public void run() {
				                	getGestureDispatcher().zoomToSelection();
								}
							}, 200);
		                    break;
		                case TYPE_KEY_TAP:
		                	getGestureDispatcher().zoomToSelection();
		                    break;
		                default:
		                    System.out.println("Unknown gesture type.");
		                    break;
		            }
		        }
	        }
	        lastFrame = frame;
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
