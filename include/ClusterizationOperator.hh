#include "Cluster.hh"
#include <vector>
using std::vector;

namespace MyCalorimeter 
{
	class ClusterizationOperator 
	{
		public:
		//
		//	Constants
		//
	
		//
		//	Constructors
		//
			ClusterizationOperator ();
			virtual ~ClusterizationOperator ();
		//
		//	Methods
		//
			 void Initialize(int LayerCut);
			 bool Clusterize(vector< Pad * > & endPads);
			 vector< Cluster * > * GetClusters();
		private:
		//
		//	Data
		//
			int myClusterCount;
			int myMaximumLayerCut;
			vector < Pad * > myInitialPads;
			vector< Cluster * > * myClusters;
		//
		//	Private methods
		//
			void clusterizeRecursion(Cluster * cluster, Pad * pad);
			bool hasPadInClusters(Pad * pad);
			vector< Pad * > * getActiveNeighbours(Pad * pad);
	};
} /* MyCalorimeter */
