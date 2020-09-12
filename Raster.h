#pragma once

#include "cil/cImg.h"
#include "cvw/cAllViscoms.h"
#include "sensors/sensors.h"
#include <QVector>


struct NewLidarPing : QVector<struct cassandra::sensors::LidarPing>
{
public:
	bool isVisited = false;
	int label;

};

class Raster : public cStation
{
	STATIONDECL(Raster);
	// constructor
	Raster();
	// deconstructor
	virtual ~Raster();
	// Noise removal implementation
	void callback(cBufferT<QVector<struct cassandra::sensors::LidarPing>> *i_buffer);
	// Input and output ports 
	PORTGROUP(QVector<struct cassandra::sensors::LidarPing>) m_ipInliers;
	cOPort<QVector<QVector<NewLidarPing>>> m_opRaster;
};

