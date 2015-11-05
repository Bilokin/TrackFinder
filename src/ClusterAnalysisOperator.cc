#include "ClusterAnalysisOperator.hh"
#include <iostream>
#include <cmath>
//////// Recent changes: mySigmas;
//////// TrackLikeness = Module / (N - 1) 
//////// TrackLikeness =Module / (N - 1) + N*myEpsilonTL
namespace MyCalorimeter
{
	//
	//      Constructors
	//
	ClusterAnalysisOperator::ClusterAnalysisOperator()
	{
		myMergingSineCut = 0.15; // NOT CHANGED
	        myDeviationCutForDoubleTracks = 1.2;
		myClusters = NULL;
		myMinimumShowerModule = 5.0;
		myMIPEnergyCut = 2.0;
	        myMaximumLayerCut=15;
	        myEpsilonTL = 0.028; 
		myTrackLikeLengthCut = 15;
	        myTrackLikenessCut = 1.0;
		myTrackLikenessCutForDoubleTracks = 0.55;
	        myInvalidClusterCut = 3; ///NOT CHANGED
		myLastInitMIPSearchLayer = 8;
		myInteractionLayer = -1;
		myModuleCut = 1.9;
	}
	
	ClusterAnalysisOperator::~ClusterAnalysisOperator()
	{
		for (int i = myClusters->size()-1; i > -1; i--) 
		{
			Cluster * cluster = myClusters->at(i);
			//std::cout << "Popback! ";
			myClusters->pop_back();
			//std::cout << "delete... \n";
			delete cluster;
		}
		delete myClusters;
	}
	//
	//      Methods
	//
	void ClusterAnalysisOperator::Initialize(float MIPEnergyCut, int Interaction, float epsilonCut)
	{
		myMIPEnergyCut = MIPEnergyCut;
		myInteractionLayer = Interaction;
		myEpsilonTL = epsilonCut;
	}
	int ClusterAnalysisOperator::GetNumberOfClusters()
	{
	        if (!myClusters) 
	        {
			return 0;
	        }
		return myClusters->size();
	}
	void ClusterAnalysisOperator::AnalyzeAllClusters(vector< Cluster * > * clusters)
	{
		myClusters = clusters;
	         std::cout << "\n\nStarting cluster analysis...\n";
	         for (int i = 0; i < myClusters->size(); i++)
	         {
			Cluster * cluster = myClusters->at(i);
			int number = cluster->GetNumberOfPads();
			if (number < myInvalidClusterCut)
			{
			        std::cout << "Cluster with id " << cluster->GetID() << " set as invalid with "<< number << " pads\n";
			        cluster->SetStatus(INVALID_CLUSTER);
			}
			else
			{
		        	std::cout << "Cluster with id " << cluster->GetID() << " set as valid with " << number <<" pads!\n";
				float energyCut = 0.0;
				/*if (number > myTrackLikeLengthCut && cluster->GetNumberOfPads(myMIPEnergyCut) > myInvalidClusterCut) 
				{
					energyCut = myMIPEnergyCut;
				}*/
			        setEndPoints(cluster, energyCut);
				if (cluster->GetStatus() != MERGED_CLUSTER) 
				{
					analyzeForTrack(cluster, energyCut);
					//mergeCluster(cluster,energyCut);
				}
			}
		}
		FindInitialMIP();
		std::cout << "\nBegin check for merging........................\n";
		for (int i = 0; i < myClusters->size(); i++) 
		{
			Cluster * cluster = myClusters->at(i);
			mergeCluster(cluster);
		}
	}

	void ClusterAnalysisOperator::analyzeForTrack(Cluster * cluster, float energyCut)
	{
		vector< int > * start = cluster->GetStart();
		vector< int > * end = cluster->GetEnd();
		const Pad * startPad = cluster->GetStartPad();
		const Pad * endPad = cluster->GetEndPad();
		if (start && end) 
		{
			vector < int > direction;
			for (int i = 0; i < 3; i++) 
			{
				direction.push_back(start->at(i) - end->at(i));
			}
			float module = MathOperator::getModule(direction);
			if (module < myModuleCut) 
			{
				cluster->SetStatus(INVALID_CLUSTER);
				return;
			}
			int numberOfPads = cluster->GetNumberOfPads(energyCut);
			std::cout << "Start analyzing for tracks... Cluster has " << numberOfPads << " mip pads out of " << cluster->GetNumberOfPads() << " pads total\n";
			std::cout << "Cluster module: " << module << '\n';
			float trackLikeness = module / (float)( numberOfPads - 1 ) + myEpsilonTL*numberOfPads;
			vector< float > directionInt = MathOperator::getDirection(*start, *end);
			vector< float > directionFloat = MathOperator::getDirection(startPad->GetRealCoordinates(), endPad->GetRealCoordinates());
			vector< float > angles = MathOperator::getAngles(directionFloat);
			cluster->SetInnerAngles(MathOperator::getAngles(directionInt));
			cluster->SetPropertiesForSave(module,trackLikeness - myEpsilonTL*numberOfPads,numberOfPads, angles[0], angles[1]);
			float deviation = getDeviationOfCluster(cluster, directionInt, start, energyCut);
			if (numberOfPads == 3 && deviation > 0.3) 
			{
				std::cout << "3 pads cluster with deviation " << deviation << " marked as NOT tracklike\n";
				cluster->SetStatus(INVALID_CLUSTER);
				return;
			}
			if (trackLikeness > myTrackLikenessCut)// - myInvalidClusterCut))
			{
				std::cout << "Cluster set as track like with " << trackLikeness << " probability and deviation " << deviation << '\n';
				cluster->SetStatus(TRACKLIKE_CLUSTER);
			
				if (( deviation < myDeviationCutForDoubleTracks ) && ( module > (float)myMaximumLayerCut ) && ( trackLikeness - myEpsilonTL*numberOfPads < myTrackLikenessCutForDoubleTracks) ) 
				{
					std::cout << "Cluster set as DOUBLE track like with " << trackLikeness << " probability and deviation " << deviation << "\n";
					cluster->SetStatus(TWOMIPSLIKE_CLUSTER);
				}
				return;
			}
			//else 
			{
				//else
				{
					if (( module > myMinimumShowerModule) && (trackLikeness < myTrackLikenessCut) ) 
					{
						std::cout << "Cluster set as showerlike with " << trackLikeness << " probability and deviation " << deviation << "\n";
						cluster->SetStatus(SHOWERLIKE_CLUSTER);
					}
					else 
					{
						std::cout << "Cluster set as NOT track like with " << trackLikeness << " probability and deviation " << deviation << "\n";
						cluster->SetStatus(BLOBLIKE_CLUSTER);
					}
				}
			}
		}
	}

	void ClusterAnalysisOperator::FindInitialMIP()
	{
		/* Due to problem in Interaction finder.
		 * if (myInteractionLayer < 3 && myInteractionLayer > -1) 
		{
			std::cout << "Interaction Layer is " << myInteractionLayer << ", so initial mip searches does not run.\n";
			return;
		}*/
		int minZ = 30;
		int minID = -1;
		int secondHalf = myLastInitMIPSearchLayer;
		for (int i = 0; i < myClusters->size(); i++) 
		{
			Cluster * cluster = myClusters->at(i);
			if ((cluster->GetStatus() == SHOWERLIKE_CLUSTER || cluster->GetStatus() == TRACKLIKE_CLUSTER || cluster->GetStatus() == BLOBLIKE_CLUSTER) && cluster->GetInnerAngles()->at(1) < 0.333333 && cluster->GetEnd()->at(2) < secondHalf) 
			{
				if (cluster->GetEnd()->at(2) < minZ) 
				{
					minZ = cluster->GetEnd()->at(2);
					minID = cluster->GetID();
				}
			}
		}
		if (minID > 0) 
		{
			for (int i = 0; i < myClusters->size(); i++) 
			{
				Cluster * cluster = myClusters->at(i);
				if (cluster->GetID() == minID) 
				{
					cluster->SetStatus(INITIALMIP_CLUSTER);
				}
			}
			std::cout << "Found an initial MIP with ID " << minID << '\n';
		}
	}

	void ClusterAnalysisOperator::mergeCluster(Cluster * cluster,float energyCut)
	{
		if(myClusters->size() > 1) 
		{
			//Cluster * cluster = myClusters->at(i); 
			if ((cluster->GetStatus() == TRACKLIKE_CLUSTER) || (cluster->GetStatus() == TWOMIPSLIKE_CLUSTER) ) 
			{
				//std::cout << "Cluster with id "<<  cluster->GetID() << " is track\n";
				vector< int > * start = cluster->GetStart();
				vector< int > * end = cluster->GetEnd();
				if (start && end)
				{
					vector< float > direction = MathOperator::getDirection(*start, *end);
					for (int j = 0; j < myClusters->size(); j++) 
					{
						Cluster * another = myClusters->at(j);
						if (another->GetID() != cluster->GetID()) 
						{
							bool checkPassed = checkIfClustersCanMerge(start,end,direction,another, energyCut);
							if (checkPassed) 
							{
								std::cout << "!!!Merging clusters: " << another->GetID() << " to " << cluster->GetID()  << " !!!\n"; 
								cluster->Merge(another);
								if (another->GetStatus() == TWOMIPSLIKE_CLUSTER) 
								{
									cluster->SetStatus(TWOMIPSLIKE_CLUSTER);
								}
								another->SetStatus(MERGED_CLUSTER);
							}
						}
					}
				}
			}
		}
	}
	
	float ClusterAnalysisOperator::getDeviationOfCluster(Cluster * cluster, vector< float > & direction, vector< int > * start, float energyCut)
	{
		float deviation = 0.0;
		vector< Pad * > pads = cluster->GetPads(energyCut);
		int number = pads.size();
		for (int i = 0; i < number; i++)                             
		{
			vector< int > point = pads[i]->GetCoordinates();
		        deviation += MathOperator::getDistanceTo(point, direction, start);
		}
		deviation /= number;
		return deviation;
	}

	bool ClusterAnalysisOperator::checkIfClustersCanMerge(vector<int> *start, vector<int> *end , vector< float > & direction, Cluster * another, float energyCut)
	{
		if (start && end && another) 
		{
			if (another->GetNumberOfPads() > myTrackLikeLengthCut) 
			{
				return false; // Delete for muons!!!
			}
			if (another->GetStatus() == INITIALMIP_CLUSTER || another->GetStatus() == MERGED_CLUSTER) 
			{
				return false;
			}
			float anotherDeviation = getDeviationOfCluster(another, direction, start, energyCut);
		 	float distanceFromEnd = getMeanDistance(end, another);
			float distanceFromStart = getMeanDistance(start, another);
			float sine = anotherDeviation / distanceFromEnd;
			vector < float > vector1;
			for (int i = 0; i < 3; i++)
	                {
	                        vector1.push_back((float)(start->at(i) - end->at(i)));
	                }
	                float module = MathOperator::getModule(vector1);
			float observable = sqrt(distanceFromEnd*distanceFromEnd-anotherDeviation*anotherDeviation)+sqrt(distanceFromStart*distanceFromStart-anotherDeviation*anotherDeviation) - 0.5; /// NOT CHANGED
			
			//std::cout << "Cluster with id " <<  another->GetID() << " has sine angle of " << sine << " and observable " << observable << " (" << module <<")\n";
			bool result = ( sine < myMergingSineCut ) && ( observable > module);
			return result;
		}
		return false;
	}

	vector< vector < int > > ClusterAnalysisOperator::getEndPoints(vector< Pad * > & pads)
	{
		float distancePrevious = 0.0;
		//vector< Pad * > pads = cluster->GetAllPads();
		vector< vector < int > > result;
		vector< int > point = pads[0]->GetCoordinates();
		vector< int > initial(point);
		vector< int > final(point);
		//std::cout << "ClusterAnalysisOperator::getEndPoints\n";
		for (int i = 0; i < pads.size(); i++)
		{
			for (int j = i+1; j < pads.size(); j++)
			{
				float distance = 0.0;
				for (int k = 0; k < 3; k++)
				{
					distance += (pads[i]->GetCoordinates()[k] - pads[j]->GetCoordinates()[k]) * (pads[i]->GetCoordinates()[k] - pads[j]->GetCoordinates()[k]);
				}
				if (distance > distancePrevious)
				{
					for (int k = 0; k < 3; k++)
					{
						initial[k] = pads[i]->GetCoordinates()[k];
						final[k] = pads[j]->GetCoordinates()[k];
					}
					distancePrevious = distance;
				}
			}
		}
		result.push_back(initial);
		result.push_back(final);
		return result;
	}

	vector< vector < int > > ClusterAnalysisOperator::getEndPoints(Cluster * cluster, float energyCut)
	{
		vector< Pad * > pads = cluster->GetPads(energyCut);
		return getEndPoints(pads);
	}

	int ClusterAnalysisOperator::GetTrackLikeClusters()
	{
		if (!myClusters) 
		{
			return 0;
		}
		int count = 0;
		for (int i = 0; i < myClusters->size(); i++)
		{
			Cluster * cluster = myClusters->at(i);
			if (cluster->GetStatus() == TRACKLIKE_CLUSTER)
			{
			        count++;
			}
			if (cluster->GetStatus() == TWOMIPSLIKE_CLUSTER) 
			{
				count += 2;
			}
		}
		return count;
	}

	int ClusterAnalysisOperator::GetShowerLikeClusters()
	{
		if (!myClusters)
		{
			return 0;
		}
		int count = 0;
		for (int i = 0; i < myClusters->size(); i++)
		{
			Cluster * cluster = myClusters->at(i);
			if (cluster->GetStatus() == SHOWERLIKE_CLUSTER) 
			{
				count++;
			}
		}
		return count;
	}

	vector<float> * ClusterAnalysisOperator::GetTracklikenessFromClusters()
	{
		return mySigmas;
	}

	vector<float> * ClusterAnalysisOperator::GetModuleFromClusters()
	{
		return myModules; 
	}

	vector<int> * ClusterAnalysisOperator::GetNumberOfPadsFromClusters()
	{
		return myPads;
	}
	
	vector< Cluster * > *  ClusterAnalysisOperator::GetClusters()
	{
		return myClusters;
	}
	vector< Pad* > ClusterAnalysisOperator::getEndPads(Cluster * cluster)
	{
		vector< Pad* > result;
		float distancePrevious = 0.0;
		Pad * start = NULL;
		Pad * end = NULL;
		if (!cluster) 
		{
			return result;
		}
		vector <Pad*> pads = cluster->GetAllPads();
		vector< int > point = pads[0]->GetCoordinates();
		vector< int > initial(point);
		vector< int > final(point);
		for (int i = 0; i < pads.size(); i++)
		{
			for (int j = i+1; j < pads.size(); j++)
			{
				float distance = 0.0;
				for (int k = 0; k < 3; k++)
				{
					distance += (pads[i]->GetCoordinates()[k] - pads[j]->GetCoordinates()[k]) * (pads[i]->GetCoordinates()[k] - pads[j]->GetCoordinates()[k]);
				}
				if (distance > distancePrevious)
				{
					//for (int k = 0; k < 3; k++)
					//{
					//	initial[k] = pads[i]->GetCoordinates()[k];
					//	final[k] = pads[j]->GetCoordinates()[k];
					//}
					start = pads[i];
					end = pads[j];
					distancePrevious = distance;
				}
			}
		}
		result.push_back(start);
		result.push_back(end);
		return result;
	}

	void ClusterAnalysisOperator::setEndPoints(Cluster * cluster, float energyCut)
	{
		if (cluster) 
		{
			//vector< vector< int > > points = getEndPoints(cluster, energyCut);
			vector< Pad* > pads = getEndPads(cluster);
			 //std::cout << "Setting end points... start: ";
			 //for (int i = 0; i < 3; i++) 
			 //{
			 	//std::cout << points[0][i] << "-" << pads[0]->GetCoordinates()[i] << ' ';
			 //}
			 //std::cout << "end: ";
			 //for (int i = 0; i < 3; i++) 
			 //{	
			 	//std::cout << points[1][i] << "-" << pads[1]->GetCoordinates()[i]<< ' ';
			 //}
			 //std::cout << " \n";
			cluster->SetEndPoints(new vector< int >(pads[0]->GetCoordinates()), new vector< int >(pads[1]->GetCoordinates()));
			cluster->SetEndPads(pads[0] ,pads[1]);
		}
	}

	float ClusterAnalysisOperator::getMeanDistance(vector< int > * end, Cluster * to)
	{
		//vector< int > * from end = from->GetEnd();
		float result = 0.0;
		if (end) 
		{
			vector< Pad * >  pads = to->GetAllPads();
			for (int i = 0; i < to->GetNumberOfPads(); i++) 
			{
				vector < int > distance;
				for (int j = 0; j < 3; j++) 
				{
					distance.push_back(pads[i]->GetCoordinates()[j] - end->at(j));
				}
				result += MathOperator::getModule(distance);
			}
			result /= to->GetNumberOfPads();
		}
		return result;
	}
}/* MyCalorimeter*/	
