#include "stdafx.h"
#include "XYZPoint.h"


XYZPoint::XYZPoint(LONG x, LONG y, USHORT z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

XYZPoint::XYZPoint(void)
{
}


XYZPoint::~XYZPoint(void)
{
}

boolean XYZPoint::within(XYZPoint &other, int distance)
{
	boolean ret = TRUE;
	if (abs(other.m_x - m_x) > distance) 
	{
		ret = FALSE;
	}
	else if (abs(other.m_y - m_y) > distance) 
	{
		ret = FALSE;
	}
	else if (abs(other.m_z - m_z) > distance) 
	{
		ret = FALSE;
	}
	return ret;
}

void XYZPoint::copyFrom(XYZPoint& other)
{
	m_x = other.m_x; 
	m_y = other.m_y; 
	m_z = other.m_z; 
}

void XYZPoint::minus(XYZPoint& source, XYZPoint& other)
{
	m_x = source.m_x - other.m_x; 
	m_y = source.m_y - other.m_y; 
	m_z = source.m_z - other.m_z; 
}
