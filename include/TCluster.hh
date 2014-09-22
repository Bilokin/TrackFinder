#ifndef _TCluster_hh
#define _TCluster_hh
#include "TObject.h"
#include "Rtypes.h"
#include "TSystem.h"
namespace MyCalorimeter
{
	class TCluster : public TObject
	{
	public:
		//
		//	Constructor & Destructor
		//
		TCluster ();
		TCluster (int type, int number, float sigma, float length);
		virtual ~TCluster () {};
		//
		//	Methods
		//
		void Initialize(int type, int number, float sigma, float length);
		void SetEndPoint(int x, int y, int z);
		void SetStartPoint(int x, int y, int z);

		Int_t GetType();
		Int_t * GetStartPoint();
		Int_t * GetEndPoint();
		Int_t GetNumberOfPads();
		Float_t GetLength();
		Float_t GetSigma();
		
	private:
		//
		//	Methods
		//
	
		//
		//	Data
		//
		static const int DIM = 3 ;
		Int_t myType;
		Int_t myStart[DIM];
		Int_t myEnd[DIM];
		Float_t myLength;
		Int_t myNumberOfPads;
		Float_t mySigma;
		/* data */
	public:
//		ClassDef(MyCalorimeter::TCluster,1)
	};
} /* TCluster */
#endif
