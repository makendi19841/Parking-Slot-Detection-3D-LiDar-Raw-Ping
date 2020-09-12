#pragma once

#include "cil/cImg.h"
#include "cvw/cAllViscoms.h"
#include "sensors/sensors.h"
#include <QVector>
#include <algorithm>


struct NewLidarPing : QVector<struct cassandra::sensors::LidarPing>
{
public:
	bool isVisited = false;
	int label;
	int Raster_i;
	int Raster_j;

	float32 meanX = 0.0;
	float32 meanY = 0.0;
	float32 Cov_xx = 0.0;
	float32 Cov_xy = 0.0;
	float32 Cov_yy = 0.0;
	float32 Cov_yx = 0.0;

};

struct less_than_key
{
	inline bool operator() (const NewLidarPing& struct1, const NewLidarPing& struct2)
	{
		return (struct1.label < struct2.label);
	}
};


class BoundingBox : public cStation
{
	STATIONDECL(BoundingBox);
	// constructor
	BoundingBox();
	// deconstructor
	virtual ~BoundingBox();
	// Noise removal implementation
	void callback(cBufferT<QVector<NewLidarPing>> *i_buffer);
	// Input and output ports 
	PORTGROUP(QVector<NewLidarPing> ) m_ipValidCells;
	cOPort<QVector<NewLidarPing>> m_opBoundingBox;
};
