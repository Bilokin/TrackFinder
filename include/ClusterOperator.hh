#include "Cluster.hh"
#include "TCluster.hh"
#include "ClusterAnalysisOperator.hh"
#include "ClusterizationOperator.hh"
#include <vector>
using std::vector;
namespace MyCalorimeter 
{
	class ClusterOperator 
	{
		public:
		//
		//	Constants
		//
	
		//
		//	Constructors
		//
			ClusterOperator ();
			ClusterOperator (float MIPEnergyCut = 2.0, int LayerCut = 15);
			virtual ~ClusterOperator ();
		//
		//	Methods
		//

		int GetNumberOfClusters();
		vector<float> * GetTracklikenessFromClusters();
		vector<float> * GetModuleFromClusters();
		vector<int> * GetNumberOfPadsFromClusters();
		bool Clusterize(vector< Pad * > & endPads);
		void AnalyzeAllClusters();
		int GetTrackLikeClusters();
		int GetShowerLikeClusters();
		vector<TCluster*> * GetTClusters();
		private:
		//
		//	Data
		//i
			ClusterAnalysisOperator myAnalysis;
			ClusterizationOperator myClusterizator;
		//
		//	Private methods
		//
	};
} /* MyCalorimeter */
