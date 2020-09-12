#include "tutorialsPCH.h"
#include "Cluster.h"
#include <algorithm>

//-------------------------------------------------------------------------------------------------
Cluster::Cluster() :
	m_ipRaster(this, &Cluster::callback, "Raster"),
	m_opCluster(this, "ValidCells")

{

}
//-------------------------------------------------------------------------------------------------
Cluster::~Cluster()
{
}

void Cluster::callback(cBufferT<QVector<QVector<NewLidarPing>>> *i_buffer)
{
	// variables declaration
	QVector3D inliers_XYZ = QVector3D(0.0, 0.0, 0.0);
	QVector3D inliers_rawDist = QVector3D(0.0, 0.0, 0.0);

	// data acquisition
	QVector<QVector<NewLidarPing>> Raster = i_buffer->getData();
		

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

	//QVector<QVector<NewLidarPing>> raster;
	QVector<QVector<QVector<NewLidarPing>>>Nwraster;
	QVector<NewLidarPing> Valid_Cells;
	int Raster_Index, Raster_x, Raster_y;
	//End of initialization-------------------------------------------------------------

	int C = 0;
	Raster[0][0].label = C;
	//Cluster the cells-------------------------------------------------------------------------
	for (unsigned int i = 1; i < width - 1; i++) {
		for (unsigned int j = 1; j < height - 1; j++) {

			if (Raster[i][j].size() > 0) {
				//Check if you have already visit a cell if yes give the label to neighbors-----------
				if (Raster[i][j].label > 0) {
					//Check if neighbor 1 has points--------------------------------------------------
					if (Raster[i + 1][j].size() > 0) {
						if (Raster[i + 1][j].label == 0) {

							Raster[i + 1][j].label = Raster[i][j].label;
						}
						else
						{
							Raster[i][j].label = Raster[i + 1][j].label;
						}

					}
					//Check if neighbor 2 has points--------------------------------------------------
					if (Raster[i][j + 1].size() > 0) {

						if (Raster[i][j + 1].label == 0) {

							Raster[i][j + 1].label = Raster[i][j].label;
						}
						else
						{
							Raster[i][j].label = Raster[i][j + 1].label;
						}



					}
					//Check if neighbor 3 has points--------------------------------------------------
					if (Raster[i + 1][j + 1].size() > 0) {

						if (Raster[i + 1][j + 1].label == 0) {

							Raster[i + 1][j + 1].label = Raster[i][j].label;
						}
						else
						{
							Raster[i][j].label = Raster[i + 1][j + 1].label;
						}

					}
					//-------------------------------------------------------------------------------

				}
				if (Raster[i + 1][j].label > 0) {
					Raster[i][j].label = Raster[i + 1][j].label;
				}
				if (Raster[i + 1][j - 1].label > 0) {
					Raster[i][j].label = Raster[i + 1][j - 1].label;
				}
				if (Raster[i + 1][j + 1].label > 0) {
					Raster[i][j].label = Raster[i + 1][j + 1].label;
				}
				if (Raster[i - 1][j].label > 0) {
					Raster[i][j].label = Raster[i - 1][j].label;
				}
				if (Raster[i - 1][j - 1].label > 0) {
					Raster[i][j].label = Raster[i - 1][j - 1].label;
				}
				if (Raster[i][j - 1].label > 0) {
					Raster[i][j].label = Raster[i][j - 1].label;
				}
				if (Raster[i][j + 1].label > 0) {
					Raster[i][j].label = Raster[i][j + 1].label;
				}
				if (Raster[i - 1][j + 1].label > 0) {
					Raster[i][j].label = Raster[i - 1][j + 1].label;
				}
				if (Raster[i][j].label == 0)
				{
					Raster[i][j].label = C;
					//Check if neighbor 1 has points--------------------------------------------------
					if (Raster[i + 1][j].size() > 0) {
						Raster[i + 1][j].label = Raster[i][j].label;
					}
					if (Raster[i + 1][j + 1].size() > 0) {
						Raster[i + 1][j + 1].label = Raster[i][j].label;
					}
					if (Raster[i + 1][j - 1].size() > 0) {
						Raster[i + 1][j - 1].label = Raster[i][j].label;
					}

					if (Raster[i][j].size() > 0) {
						Raster[i][j].label = Raster[i][j].label;
					}
					if (Raster[i][j + 1].size() > 0) {
						Raster[i][j + 1].label = Raster[i][j].label;
					}
					if (Raster[i][j - 1].size() > 0) {
						Raster[i][j - 1].label = Raster[i][j].label;
					}

					if (Raster[i - 1][j].size() > 0) {
						Raster[i - 1][j].label = Raster[i][j].label;
					}
					if (Raster[i - 1][j + 1].size() > 0) {
						Raster[i - 1][j + 1].label = Raster[i][j].label;
					}
					if (Raster[i - 1][j - 1].size() > 0) {
						Raster[i - 1][j - 1].label = Raster[i][j].label;
					}

				}


				Raster[i][j].Raster_i = i;
				Raster[i][j].Raster_j = j;

				Valid_Cells.push_back(Raster[i][j]);

        	}
			else if ((Raster[i + 1][j].size() == 0) && (Raster[i][j + 1].size() == 0) && (Raster[i + 1][j + 1].size() == 0)) {
				C = C + 1;
			}

		}
	}

		   	  
	cTracer::cout << "----------------------------------------------------" << "\n";


	// Send inlier data to output port--------------------------------
	m_opCluster.send(Valid_Cells, i_buffer->getStamp());
}

STATION(Cluster, "tutorials/Cluster");