#include "stdafx.h"
#include "ArmPosition.h"


ArmPosition::ArmPosition(void)
{
}


ArmPosition::~ArmPosition(void)
{
}

boolean  ArmPosition::within(ArmPosition& other, int distance)
{
	boolean ret = TRUE;
	if (!m_leftHand.within(other.m_leftHand, distance))
	{
		ret = FALSE;
	}
	else if (!m_rightHand.within(other.m_rightHand, distance))
	{
		ret = FALSE;
	}

	return ret;
}

boolean ArmPosition::awayFromBody(int distance)
{
	boolean ret = false;
	int bodySeparation = m_body.m_z - distance;
	if( m_rightHand.m_z < bodySeparation || m_leftHand.m_z < bodySeparation)
	{
		ret = true;
	}
//	wprintf(L"body sep: %d, left: %d, right: %d, ret %s\n", m_body.m_z, m_leftHand.m_z, m_rightHand.m_z, ret?L"true":L"false");
	return ret;
}

void ArmPosition::copyFrom(ArmPosition& other)
{
	m_leftHand.copyFrom(other.m_leftHand);
	m_rightHand.copyFrom(other.m_rightHand);
	m_body.copyFrom(other.m_body);
	m_startTime = other.m_startTime;

}

void ArmPosition::minus(ArmPosition& source, ArmPosition& other)
{
	m_leftHand.minus(source.m_leftHand, other.m_leftHand);
	m_rightHand.minus(source.m_rightHand, other.m_rightHand);
	m_body.minus(source.m_body, other.m_body);
	m_startTime = source.m_startTime - other.m_startTime;

}
