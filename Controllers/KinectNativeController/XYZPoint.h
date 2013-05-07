#pragma once
class XYZPoint
{
public:
	XYZPoint(void);
	XYZPoint(LONG x, LONG y, USHORT z);

	~XYZPoint(void);
	boolean within(XYZPoint&, int);
	void copyFrom(XYZPoint& other);
	void minus(XYZPoint& source, XYZPoint& other);
//protected: 
	LONG m_x;
	LONG m_y;
	SHORT m_z;
};

