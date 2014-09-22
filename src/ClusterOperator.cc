#include "ClusterOperator.hh"
#include <iostream>
#include <cmath>
namespace MyCalorimeter 
{
	//
	//	Constructors
	//
	ClusterOperator::ClusterOperator()
	{

	}
	ClusterOperator::ClusterOperator(float MIPEnergyCut, int LayerCut)
	{
		myClusterizator.Initialize(LayerCut);
		myAnalysis.Initialize(MIPEnergyCut);
	}
	ClusterOperator::~ClusterOperator()
	{
	}
	//
	//	Methods
	//
	int ClusterOperator::GetNumberOfClusters()	
	{
		return myAnalysis.GetNumberOfClusters();
	}
	
	vector<float> * ClusterOperator::GetTracklikenessFromClusters()
	{
		return myAnalysis.GetTracklikenessFromClusters();
	}

	vector<float> * ClusterOperator::GetModuleFromClusters()
	{
	        return myAnalysis.GetModuleFromClusters();
	}

	vector<int> * ClusterOperator::GetNumberOfPadsFromClusters()
	{
	        return myAnalysis.GetNumberOfPadsFromClusters();
	}


	bool ClusterOperator::Clusterize(vector< Pad * > & endPads)
	{
		return myClusterizator.Clusterize(endPads);
	}
	
	int ClusterOperator::GetShowerLikeClusters()
	{
	        return myAnalysis.GetShowerLikeClusters();
	}


	void ClusterOperator::AnalyzeAllClusters()
	{
		myAnalysis.AnalyzeAllClusters(myClusterizator.GetClusters());
	}
	
	int ClusterOperator::GetTrackLikeClusters()
	{
		return myAnalysis.GetTrackLikeClusters();
	}
	vector<TCluster*> * ClusterOperator::GetTClusters()
	{
		vector<Cluster*> * clusters = myAnalysis.GetClusters();
		vector<TCluster*> * result = new vector<TCluster*>();
		for (int i = 0; i < clusters->size(); i++) 
		{
			TObject * tcluster = new TCluster();
			clusters->at(i)->Assign(*tcluster);
			result->push_back((TCluster*)tcluster);
		}
		return result;
	}
} /* MyCalorimeter */
