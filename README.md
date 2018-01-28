molecular-control-toolkit
============

The Molecular Control Toolkit for controlling molecular graphics via gesture and voice. Currently supporting Kinect and Leap Motion.


The Molecular Control Toolkit supports Windows for the Leap Motion & Kinect, and Mac for the Leap Motion.


Prequisites
-----------
To run, you will need to install the prerequisites for the gesture device you wish to support.

Kinect: the Kinect SDK and Developer Toolkit
(http://www.microsoft.com/en-us/kinectforwindows/develop/developer-downloads.aspx)

Leap Motion: the Leap Motion SDK
(http://developer.leapmotion.com)

Javadoc
-------
The javadoc is within the repository and also on the web at http://aquaria.ws/mct/javadoc

Code example
------------
// initialise the toolkit
        MolecularControlToolkit molecularControlToolkit = new MolecularControlToolkit();
       
        if (type.equalsIgnoreCase("kinect")){
        	molecularControlToolkit.addConnector(ConnectorType.Kinect);
        }
        if (type.equalsIgnoreCase("leap")){
        	molecularControlToolkit.addConnector(ConnectorType.LeapMotion);
        }
        else {
        	System.out.println("Adding nothing");
        }
        
        MyDispatcher dispatcher = new MyDispatcher();
        molecularControlToolkit.setListeners(dispatcher);
....

public class MyDispatcher implements MolecularControlListener {
…
	//Here is an example of one of the callbacks
	public void triggerRotate(int rotateX, int rotateY, int rotateZ) {
		if (rotateBehaviour != null)
		{
			rotateBehaviour.processStimulus(rotateX, rotateY, rotateZ);
		}
	}
}

