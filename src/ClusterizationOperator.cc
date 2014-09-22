#include "ClusterizationOperator.hh"
#include <iostream>
namespace MyCalorimeter
{
	//
	//      Constructors
	//
	ClusterizationOperator::ClusterizationOperator()
	{
		myClusterCount = 0;
		myMaximumLayerCut = 15;
		myClusters = new vector< Cluster *>();
	}
	ClusterizationOperator::~ClusterizationOperator()
	{
	}
	//
	//      Methods
	//
	void ClusterizationOperator::Initialize(int LayerCut)
	{
		if (LayerCut > 0) 
		{
			myMaximumLayerCut = LayerCut;
		}
	}
	bool ClusterizationOperator::Clusterize(vector< Pad * > & myInitialPads)
	{
	       /* for (int i = 0; i < endPads->size(); i++)
	        {
	                myInitialPads.push_back(new Pad(*(endPads->at(i))));
	        }*/
		for (int i = myInitialPads.size()-1; i > -1; i--)
		{
			Pad * pad = myInitialPads.at(i);
			if (!hasPadInClusters(pad))
			{
				Cluster * cluster = new Cluster(myClusterCount++);
				std::cout << "Adding another cluster\n";
				myClusters->push_back(cluster);

				clusterizeRecursion(cluster,pad);
				if (cluster->GetNumberOfPads() == 0)
	                	{
	                               myClusters->pop_back();
				       delete cluster;
				       myClusterCount--;
				}
	        	}
		}
	        return false;
	}

	bool ClusterizationOperator::hasPadInClusters(Pad * pad)
	{
	        if (myClusters->size() == 0)
	        {
	                //std::cout << "There are no clusters...\n";
	                return false;
	        }
	        bool result = false;
	        for (int i = 0; i < myClusters->size(); i++)
	        {
	                if (myClusters->at(i)->HasPad(pad))
	                {
	                        //std::cout << "Pad found in another cluster\n";
	                        result = true;
				break;
	        	}
	        }
	        if (!result)
	        {
	                //std::cout << "Pad is unique\n";
	        }
	        return result;
	}

	vector< Pad * > * ClusterizationOperator::getActiveNeighbours(Pad * pad)
	{
	        vector< vector< vector< Pad *> > > * neighbours = pad->GetNeighbours();
	        vector< Pad * > * active = new vector< Pad * >();
	        //std::cout << "Search for active neighbour...\n";
	        int count = 0;
	        for (int k = 0; k < neighbours->size(); k++)
	        {
	                for (int i = 0; i < neighbours->at(k).size(); i++)
	                {
	                        for (int j = 0; j < neighbours->at(k).at(i).size(); j++)
	                        {
	                                // std::cout << "k = " << k << " i = " << i << " j = "<< j << '\n';
	                                // exclude the same pad
	                                if (k==0 && i==1 && j==1)
	                                {
	                                        continue;
	                                }
	                                else
	                                {
	                                        Pad * neighbour =  neighbours->at(k).at(i).at(j);
	                                        if (neighbour->IsActive())
	                                        {
	                                                count++;
	                                                //std::cout << "Found active neighbour #" << k << '.' << i << '.' << j << "\n";
	                                                active->push_back(neighbour);
	                                        }
               	                	}
        	                }
        	        }
        	}
        	//std::cout << "Found " << count << " active neighbours\n";
	        return active;
	}
	
	void ClusterizationOperator::clusterizeRecursion(Cluster * cluster, Pad * pad)
	{
		if (!hasPadInClusters(pad))
		{
			//std::cout << "**Stupid error**\n";
			cluster->AddPad(pad);
		}
		vector< Pad *> * active = getActiveNeighbours(pad);
		//std::cout << "Active neighbours: " << active->size() << '\n';
		if (active->size() == 0 || pad->GetCoordinates()[2] < myMaximumLayerCut)
		{
			//std::cout << "No Neighbours or MaximumLayerCut reached\n";
			return;
		}
		//std::cout << "Begin extracting neighbours and clusterization\n";
		for (int i = 0; i < active->size(); i++)
		{
			Pad * neighbour = active->at(i);
			if (!hasPadInClusters(neighbour))
			{
				std::cout << "Adding pad to cluster\n";
				cluster->AddPad(neighbour);
				clusterizeRecursion(cluster, neighbour);
			}
		}
		delete active;
	}

	vector< Cluster * > *  ClusterizationOperator::GetClusters()
	{
		return myClusters;
	}
}
