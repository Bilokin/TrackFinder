#ifndef _Cluster_hh_
#define _Cluster_hh_
#include "Pad.hh"
#include "IAssignable.hh"
#include "TCluster.hh"
#include <vector>
using std::vector;
namespace MyCalorimeter
{
	enum ClusterType
	{
		UNDEFINED_CLUSTER,
		INVALID_CLUSTER,
		BLOBLIKE_CLUSTER,
		TRACKLIKE_CLUSTER,
		TWOMIPSLIKE_CLUSTER,
		SHOWERLIKE_CLUSTER,
		MERGED_CLUSTER,
		INITIALMIP_CLUSTER
	};

	class Cluster : public IAssignable 
	{
		public:
	//
	//	Constants
	//
	//
	//	Constructors
	//
		Cluster (int id);
		virtual ~Cluster ();
	//
	//	Methods
	//
		void Assign(TObject & object);
		void SetPropertiesForSave(float length, float sigma, int number, float phi, float teta);

		void AddPad(Pad * pad);
		void DiscardLastPad();
		int GetID();
		vector< int > * GetStart();
		vector< int > * GetEnd();
		int GetNumberOfPads(float energyCut = 0.0);
		vector< Pad * > GetAllPads();
		vector< Pad * > GetPads(float energyCut);
		vector< Cluster * > & GetMerged();
		const std::vector< float > * GetAngles() const;
		void Merge(Cluster * cluster);
		bool HasPad(Pad * pad);
		bool HasPad(int x, int y, int z);
		void SetEndPoints(vector< int > * one, vector < int > * two);
		ClusterType GetStatus() const;
		void SetStatus(ClusterType type);
		private:
		int myID;
		vector< int > * myStart;
		vector< int > * myEnd;
		vector< Pad * > myPads;
		vector< Cluster * > myMerged;
		
		float myLength;
		float mySigma;
		int myNumberAfterCut;
		std::vector< float > myAngles;
		ClusterType myType;
	};
}
#endif

