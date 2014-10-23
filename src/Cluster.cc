#include <iostream>
#include "Cluster.hh"

namespace MyCalorimeter
{
	//
	//	Constructors
	//
	Cluster::Cluster (int id) 
	{
		//myMerged = new vector< Cluster *>();
		myStart = NULL;
		myEnd = NULL;
		myID = id;
		myType = UNDEFINED_CLUSTER;
	}
        Cluster::~Cluster ()
	{
		delete myStart;
		delete myEnd;
		myPads.clear();
	}		
	//
	//	Methods
	//SHOWERLIKE_CLUSTER,
	void Cluster::Assign(TObject & object)
	{
		TCluster & cluster = (TCluster&) object;
		if (myType != INVALID_CLUSTER && myType != MERGED_CLUSTER) 
		{
			cluster.SetEndPoint(myEnd->at(0),myEnd->at(1),myEnd->at(2));
			cluster.SetStartPoint(myStart->at(0),myStart->at(1),myStart->at(2));
			cluster.SetAngles(myAngles);
			cluster.Initialize(myType,myNumberAfterCut,mySigma,myLength);
			float average = 0.0;
			for (int i = 0; i < myPads.size(); i++) 
			{
				average += myPads[i]->GetEnergy();
			}
			average /= (float)myPads.size();
			//std::cout << "Energy: " << average << '\n';
			cluster.SetAverageEnergy(average);
		}
		else 
		{
			if (myType == INVALID_CLUSTER) 
			{
				cluster.Initialize(myType,2,0.0,0.0);
			}
			if (myType == MERGED_CLUSTER) 
			{
				cluster.Initialize(myType,myPads.size(),0.0,0.0);
			}
				
		}
	}
	void Cluster::AddPad(Pad * pad)
	{
		myPads.push_back(pad);
	}

	void Cluster::DiscardLastPad()
	{
		myPads.pop_back();
	}
	const std::vector< float > * Cluster::GetAngles() const
	{
		return &myAngles;
	}
	vector< Pad * > Cluster:: GetAllPads()
	{
		return myPads;
	}

	vector< Pad * > Cluster::GetPads(float energyCut)
	{
		vector< Pad * > result;
		if (energyCut == 0.0) 
		{
			return myPads;
		}
		for (int i = 0; i < myPads.size(); i++) 
		{
			if (myPads[i]->GetEnergy() < energyCut) 
			{
				result.push_back(myPads[i]);
			}
		}
		return result;
	}

	bool Cluster::HasPad(Pad * pad)
	{
		int x = pad->GetCoordinates()[0];
		int y = pad->GetCoordinates()[1];
		int z = pad->GetCoordinates()[2];
		bool result = this->HasPad(x,y,z);
		return result;
	}
	bool Cluster::HasPad(int x, int y, int z)
	{	
		if (myPads.size() == 0) 
		{
			return false;
		}
		for (int i = 0; i < myPads.size(); i++) 
		{
			if (myPads[i]->GetCoordinates()[0] == x && myPads[i]->GetCoordinates()[1] == y && myPads[i]->GetCoordinates()[2] == z) 
			{
				return true;
			}
		}
		return false;
	}
	ClusterType Cluster::GetStatus() const
	{
		return myType;
	}

	void Cluster::Merge(Cluster * cluster)
	{
		//cluster->SetStatus(MERGED_CLUSTER);
		myMerged.push_back(cluster);
	}

	vector< Cluster * > & Cluster::GetMerged()
	{
		return myMerged;
	}

	int Cluster::GetID()
	{
		return myID;
	}

	 vector< int > * Cluster::GetStart()
	 {
	 	return myStart;
	 }
	 vector< int > * Cluster::GetEnd()
	 {
		return myEnd;
	 }


	void Cluster::SetEndPoints(vector< int > * one, vector < int > * two)
	{
		myStart = one;
		myEnd = two;
	}

	
	int Cluster::GetNumberOfPads(float energyCut)
	{
		if (energyCut == 0.0) 
		{
			return myPads.size();
		}
		int count = 0;
		for (int i = 0; i < myPads.size(); i++) 
		{
			if (myPads[i]->GetEnergy() < energyCut) 
			{
				count++;
			}
		}
		return count;
	}

	void Cluster::SetPropertiesForSave(float length, float sigma, int number, float phi, float teta)
	{
		myLength = length;
		mySigma = sigma;
		myNumberAfterCut = number;
		myAngles.push_back(phi);
		myAngles.push_back(teta);
	}
	
	void Cluster::SetStatus(ClusterType type)
	{
		myType = type;
	}
}
