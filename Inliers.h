#pragma once

#include "cil/cImg.h"
#include "cvw/cAllViscoms.h"
#include "sensors/sensors.h"
#include <QVector>

class Inliers : public cStation
{
	STATIONDECL(Inliers);
	// constructor
	Inliers();
	// deconstructor
	virtual ~Inliers();
	// Inliers implementation
	void callback(cBufferT<QVector<struct cassandra::sensors::LidarPing>> *i_buffer);
	// Input and output ports 
	PORTGROUP(QVector<struct cassandra::sensors::LidarPing>) m_ipLidar;
	cOPort <QVector<struct cassandra ::sensors::LidarPing>> m_opinliers;
};