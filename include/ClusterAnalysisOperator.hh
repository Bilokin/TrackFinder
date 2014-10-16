#include <vector>
#include "Cluster.hh"
#include "MathOperator.hh"
using std::vector;

namespace MyCalorimeter 
{
	class ClusterAnalysisOperator 
	{
		public:
		//
		//	Constants
		//
	
		//
		//	Constructors
		//
			ClusterAnalysisOperator ();
			virtual ~ClusterAnalysisOperator ();
		//
		//	Methods
		//
			void Initialize(float MIPEnergyCut = 2.0, int Interaction = 0);
			void AnalyzeAllClusters(vector< Cluster * > * clusters);
			int GetTrackLikeClusters();
			int GetShowerLikeClusters();
			int GetNumberOfClusters();
			
			vector< Cluster * > * GetClusters();
			void FindInitialMIP();
			vector<float> * GetModuleFromClusters();
			vector<int> * GetNumberOfPadsFromClusters();
			vector<float> * GetTracklikenessFromClusters();
		private:
		//
		//	Data
		//
			float myMergingSineCut;
			float myMinimumShowerModule;
			float myEpsilonTL;
			float myTrackLikenessCutForDoubleTracks;
			float myDeviationCutForDoubleTracks;
			float myMIPEnergyCut;
			int myInvalidClusterCut;
			float myTrackLikenessCut;
			int myTrackLikeLengthCut;
			int myMaximumLayerCut;
			int myDeviationIndex;
			int myLastInitMIPSearchLayer;
			int myInteractionLayer;
			vector< Cluster * > * myClusters;
			vector<int> * myPads;
			vector<float> * myModules;
			vector<float> * mySigmas;
		//
		//	Private methods
		//
			//Geometry
			//float getModule(vector< int > & v);
			//float getModule(vector< float > & v);
			//vector< float > getDirection(vector< int > & vectorPoint1, vector< int > & vectorPoint2);
			//vector< float > * vectorProduct(vector< float > & v1, vector< float > & v2);
			//float getDistanceTo(vector< int > & vectorPoint1, vector< float > & vector1, vector< int > * pointOfLine );
			
			void setEndPoints(Cluster * cluster,  float energyCut = 0.0);
			vector< vector < int > > getEndPoints(Cluster * cluster, float energyCut = 0.0);
			float getDeviationOfCluster(Cluster * cluster, vector< float > & vector1, vector< int > * start, float energyCut = 0.0);
			bool checkIfClustersCanMerge(vector<int> *start, vector<int> *end , vector< float >  & direction, Cluster * another, float energyCut = 0.0);
			float getMeanDistance(vector<int> * from, Cluster * to);
			//old
			vector< vector < int > > getEndPoints(vector< Pad *>& pads);
			vector< float > getDirection(vector< vector < int > > points);
			float getModule(vector< vector < int > > points);

			float getObservable(Cluster * cluster);
			void analyzeForTrack(Cluster * cluster, float energyCut = 0.0);
			void mergeCluster(Cluster * cluster,float energyCut = 0.0);
	};
} /* ClusterAnalysisOperator */
