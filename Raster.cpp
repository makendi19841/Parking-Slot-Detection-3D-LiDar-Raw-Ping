#include "tutorialsPCH.h"
#include "Raster.h"

//-------------------------------------------------------------------------------------------------
Raster::Raster() :
	m_ipInliers(this, &Raster::callback, "Inliers"),
	m_opRaster(this, "Raster")

{

}
//-------------------------------------------------------------------------------------------------
Raster::~Raster()
{
}

void Raster::callback(cBufferT<QVector<struct cassandra::sensors::LidarPing>> *i_buffer)
{
	// variables declaration
	QVector3D inliers_XYZ = QVector3D(0.0, 0.0, 0.0);
	QVector3D inliers_rawDist = QVector3D(0.0, 0.0, 0.0);

	// data acquisition
	QVector<struct cassandra::sensors::LidarPing> inliers = i_buffer->getData();


	//Create a Raster-----------------------------------------------------
		//Initialize the parameters you need--------------------------------------
	float32 dx, dy, tile_size;
	tile_size = 0.20;
	float32 maxX = 16;
	float32 minX = -3;
	float32 maxY = 8;
	float32 minY = -8;
	int width = (int)((maxX - (minX)) / tile_size);
	int height = (int)((maxY - (minY)) / tile_size);
	
	QVector<QVector<NewLidarPing>> raster;
	int raster_Index, raster_x, raster_y;
	//End of initialization-------------------------------------------------------------

	//Initialize the Raster cells with data type same with Lidar ping------------------
	for (unsigned int i = 0; i < width; i++) {
		raster.push_back(QVector<NewLidarPing>());

		for (unsigned int j = 0; j < height; j++) {
			raster[i].push_back(NewLidarPing());
		}
	}
	//End of Raster initialization-----------------------------------------------------

	//Feed the Raster with points that came from the sensor based on their position-----------------
	for (int k = 0; k < inliers.size(); k++) {
		dx = maxX - inliers[k].position.x();
		dy = inliers[k].position.y() - minY;
		raster_x = (int)(dx / tile_size);
		raster_y = (int)(dy / tile_size);
		raster[raster_x][raster_y].push_back(inliers[k]);
	}
	//End of Raster feeding--------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	//End of Raster creation-----------------------------------------------------------------------
	cTracer::cout << "----------------------------------------------------" << "\n";


	// Send inlier data to output port-----------------------------------------------------------------
	m_opRaster.send(raster, i_buffer->getStamp());
}

STATION(Raster, "tutorials/Raster");