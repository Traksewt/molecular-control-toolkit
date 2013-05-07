#pragma once

#include "XYZPoint.h"
#include <time.h>

class ArmPosition
{
public:
	ArmPosition(void);
	~ArmPosition(void);
	void setLeft(XYZPoint);
	void setRight(XYZPoint);
	boolean within(ArmPosition&, int);
	boolean awayFromBody(int distance);
	void minus(ArmPosition& source, ArmPosition& other);

	void copyFrom(ArmPosition& other);
	XYZPoint m_leftHand;
	XYZPoint m_rightHand;
	XYZPoint m_body;
	int m_startTime;
};

