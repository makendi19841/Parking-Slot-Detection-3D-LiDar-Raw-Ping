#include "tutorialsPCH.h"
#include "BoundingBox.h"



//-------------------------------------------------------------------------------------------------
BoundingBox::BoundingBox() :
	m_ipValidCells(this, &BoundingBox::callback, "ValidCells"),
	m_opBoundingBox(this, "BoundingBox")

{

}
//-------------------------------------------------------------------------------------------------
BoundingBox::~BoundingBox()
{
}

void BoundingBox::callback(cBufferT<QVector<NewLidarPing>> *i_buffer)
{
	
	// data acquisition
	QVector<NewLidarPing> Valid_Cells = i_buffer->getData();

	std::sort(Valid_Cells.begin(), Valid_Cells.end(), less_than_key());

	//--------------------------------------------------------------------------------------------------------------------------------


//Compute the number of classes-------------------------------------------------------------------------------------------------------
	int temp_label = Valid_Cells[0].label;
	int n_classes = 1;
	for (int i = 0; i < Valid_Cells.size(); i++) {
		if (Valid_Cells[i].label != temp_label) {

			temp_label = Valid_Cells[i].label;
			n_classes = n_classes + 1;
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------


//Create a vector where each cell contains all points of the same class-----------------------------------------------------------
	QVector<NewLidarPing> PointsPerClass(n_classes);


	int kapa = 0;
	int temp_class = Valid_Cells[0].label;
	int points_per_class = 1;


	for (int i = 0; i < Valid_Cells.size(); i++) {

		if (temp_class == Valid_Cells[i].label) {

			for (int j = 0; j < Valid_Cells[i].size(); j++) {

				PointsPerClass[kapa].push_back(Valid_Cells[i][j]);

			}

		}
		else {
			PointsPerClass[kapa].label = kapa;
			kapa++;
			for (int j = 0; j < Valid_Cells[i].size(); j++) {

				PointsPerClass[kapa].push_back(Valid_Cells[i][j]);

			}
			temp_class = Valid_Cells[i].label;
		}

	}
	PointsPerClass[kapa].label = kapa;
	//----------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------------------


	//Compute the Bounding box of each class----------------------------------------------------------------------------------------------
	QVector<NewLidarPing> BBox(PointsPerClass.size());
	int min_Y_index, min_X_index, max_Y_index, max_X_index;


	struct cassandra::sensors::LidarPing minX_Ping;
	struct cassandra::sensors::LidarPing minY_Ping;
	struct cassandra::sensors::LidarPing maxX_Ping;
	struct cassandra::sensors::LidarPing maxY_Ping;

	for (int i = 0; i < PointsPerClass.size(); i++) {


		float32 min_X = 100000;
		float32 min_Y = 100000;
		float32 max_X = -100;
		float32 max_Y = -100;


		for (int j = 0; j < PointsPerClass[i].size(); j++) {
			//Compute the min max of x and y
			if (PointsPerClass[i][j].position.x() > max_X) {
				max_X = PointsPerClass[i][j].position.x();
				max_X_index = j;
			}

			if (PointsPerClass[i][j].position.y() > max_Y) {
				max_Y = PointsPerClass[i][j].position.y();
				max_Y_index = j;
			}

			if (PointsPerClass[i][j].position.x() < min_X) {
				min_X = PointsPerClass[i][j].position.x();
				min_X_index = j;

			}

			if (PointsPerClass[i][j].position.y() < min_Y) {
				min_Y = PointsPerClass[i][j].position.y();
				min_Y_index = j;
			}
			//---------------------------------------------------
			// Compute the mean values of x and y

			BBox[i].meanX += PointsPerClass[i][j].position.x();
			BBox[i].meanY += PointsPerClass[i][j].position.y();

		}
		maxX_Ping = PointsPerClass[i][max_X_index];
		maxY_Ping = PointsPerClass[i][max_Y_index];
		minX_Ping = PointsPerClass[i][min_X_index];
		minY_Ping = PointsPerClass[i][min_Y_index];

		BBox[i].meanX /= PointsPerClass[i].size();
		BBox[i].meanY /= PointsPerClass[i].size();


		// --------------------------------------------------
		//Compute covariance matrix
		for (int j = 0; j < PointsPerClass[i].size(); j++) {

			float32 mul1 = BBox[i].meanX - PointsPerClass[i][j].position.x();
			float32 mul2 = BBox[i].meanY - PointsPerClass[i][j].position.y();

			BBox[i].Cov_xx += mul1 * mul1;
			BBox[i].Cov_xy += mul1 * mul2;
			BBox[i].Cov_yy += mul2 * mul2;
			BBox[i].Cov_yx += mul2 * mul1;

		}

		BBox[i].Cov_xx /= PointsPerClass[i].size();
		BBox[i].Cov_xy /= PointsPerClass[i].size();
		BBox[i].Cov_yy /= PointsPerClass[i].size();
		BBox[i].Cov_yx /= PointsPerClass[i].size();

		//--------------------------------------------------


		BBox[i].push_back(maxX_Ping);
		BBox[i].push_back(minX_Ping);
		BBox[i].push_back(maxY_Ping);
		BBox[i].push_back(minY_Ping);

		BBox[i].label = PointsPerClass[i].label;

	}
	//-----------------------------------------------------------------------
	for (int i = 0; i < BBox.size(); i++) {

		cTracer::cout << BBox[i][0].position.x() << "	" << BBox[i][2].position.y() << "	" << BBox[i].label << TREND;
		cTracer::cout << BBox[i][0].position.x() << "	" << BBox[i][3].position.y() << "	" << BBox[i].label << TREND;
		cTracer::cout << BBox[i][1].position.x() << "	" << BBox[i][2].position.y() << "	" << BBox[i].label << TREND;
		cTracer::cout << BBox[i][1].position.x() << "	" << BBox[i][3].position.y() << "	" << BBox[i].label << TREND;

	}

	//End of bounding boxes and covariance calculation--------------------------------------------------------------------------

	cTracer::cout << "----------------------------------------------------" << "\n";

	m_opBoundingBox.send(BBox, i_buffer->getStamp());
}

STATION(BoundingBox, "tutorials/BoundingBox");

