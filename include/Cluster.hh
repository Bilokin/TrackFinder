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
		UNDEFINED_CLUSTER,    //0
		INVALID_CLUSTER,      //1
		BLOBLIKE_CLUSTER,     //2
		TRACKLIKE_CLUSTER,    //3
		TWOMIPSLIKE_CLUSTER,  //4
		SHOWERLIKE_CLUSTER,   //5
		MERGED_CLUSTER,       //6 
		INITIALMIP_CLUSTER    //7
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
		const Pad * GetStartPad() const ;
		const Pad * GetEndPad() const;
		int GetNumberOfPads(float energyCut = 0.0);
		vector< Pad * > GetAllPads();
		vector< Pad * > GetPads(float energyCut);
		vector< Cluster * > & GetMerged();
		void SetInnerAngles(std::vector< float > angles);
		const std::vector< float > * GetInnerAngles() const;
		const std::vector< float > * GetAngles() const;
		void Merge(Cluster * cluster);
		bool HasPad(Pad * pad);
		bool HasPad(int x, int y, int z);
		void SetEndPoints(vector< int > * one, vector < int > * two);
		void SetEndPads( Pad * start, Pad * end);
		ClusterType GetStatus() const;
		void SetStatus(ClusterType type);
		private:
		int myID;
		vector< int > * myStart;
		vector< int > * myEnd;
		vector< Pad * > myPads;
		vector< Cluster * > myMerged;
		Pad * myStartPad;		
		Pad * myEndPad;		
		float myLength;
		float mySigma;
		int myNumberAfterCut;
		std::vector< float > myAngles;
		std::vector< float > myInnerAngles;
		ClusterType myType;
	};
}
#endif

