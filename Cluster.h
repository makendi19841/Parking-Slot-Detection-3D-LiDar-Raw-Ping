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
	int Raster_i;
	int Raster_j;


};


class Cluster : public cStation
{
	STATIONDECL(Cluster);
	// constructor
	Cluster();
	// deconstructor
	virtual ~Cluster();
	// Noise removal implementation
	void callback(cBufferT<QVector<QVector<NewLidarPing>>> *i_buffer);
	// Input and output ports 
	PORTGROUP(QVector<QVector<NewLidarPing>>) m_ipRaster;
	cOPort< QVector<NewLidarPing>> m_opCluster;
};
