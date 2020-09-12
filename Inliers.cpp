#include "tutorialsPCH.h"
#include "Inliers.h"
#include "math.h"



//-------------------------------------------------------------------------------------------------


Inliers::Inliers() :
	m_ipLidar(this, &Inliers::callback, "LidarPing"),
	m_opinliers(this, "Inliers")
	

{

}
//-------------------------------------------------------------------------------------------------
Inliers::~Inliers()
{
}
//-------------------------------------------------------------------------------------------------


void Inliers::callback(cBufferT<QVector<struct cassandra::sensors::LidarPing>> *i_buffer)
{
	// data acquisition
	QVector<struct cassandra::sensors::LidarPing> ldping = i_buffer->getData();
	// variables declaration
	QVector<struct cassandra::sensors::LidarPing> ROI, inlier;

	
	//Create a ROI--------------------------------------------------------------------------------------------------------  
		for (int i = 0; i < ldping.size(); i = i + 1)
		{
			QVector3D position = ldping[i].position;

			//We created a ROI in order to keep and process only the points close to the car
			if (ldping[i].position.x() < 16.0 && ldping[i].position.x() > -3.0  && ldping[i].position.y() < 8.0 && ldping[i].position.y() > -8.0 && ldping[i].position.z() < 2.0 &&  ldping[i].position.z() > 0.15 && ldping[i].distance > 1.0)
			{

				ROI.push_back(ldping[i]);
			}
			//-----------------------------------------------------------------------------------------
	     }
	//End of ROI--------------------------------------------------------------------------------------------------------------

	
	//We compute how many neighbors each point has using statistical removal outlier------------------------------------------------------------
	
	for (int i = 0; i < ROI.size(); i = i + 1)
	{
		int count = 0;
		for (int j = 0; j < ROI.size(); j = j + 1)
		{

			//Compute the distance from point i to all other points (j->size) 
			if (i != j)
			{
				float32 dist = ROI[i].position.distanceToPoint(ROI[j].position);

				//Check how many points are close to the point i
				if (dist <= 0.7)
				{
					count++;
				}
				//---------------------------------------------
			}
			//--------------------------------------------------------------
		}

		//If the point i has more than 5 close points print the coordinates(We need to create a new vector to save the inliers
		//because here I just print them out but we will use for further process the new vector)
		if (count > 5) {

			// save inliers satisfying all criteria
			inlier.push_back(ROI[i]);

		}

	}
	//End of Statistical removal outlier--------------------------------------------------------------


	
	/*for (int i = 0; i < inlier.size(); i = i + 1) 
		{
			cTracer::cout << inlier[i].position.x() << "	" << inlier[i].position.y() << "	" << inlier[i].position.z() << TREND;
		
		}*/
		
		
	//cTracer::cout << "-----------------------------------INLIERS---------------------------------------------- " << TREND;
	m_opinliers.send(inlier, i_buffer->getStamp());
	
}

STATION(Inliers, "tutorials/Inliers");