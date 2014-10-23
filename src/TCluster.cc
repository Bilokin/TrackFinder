#include "TCluster.hh"
using std::vector;
//ClassImp(MyCalorimeter::TCluster);
namespace MyCalorimeter 
{
	TCluster::TCluster()
	{
		SetEndPoint(-1,-1,-1);
		SetStartPoint(-1,-1,-1);
		myEnergy = 0.0;
	}
	
	TCluster::TCluster(int type, int number, float sigma, float length)
	{
		Initialize(type,number,sigma,length);
	}
	void TCluster::Initialize(int type, int number, float sigma, float length)
	{
		myType = type;
		myLength = length;
		myNumberOfPads = number;
		mySigma = sigma;
	}
	void TCluster::SetEndPoint(int x, int y, int z)
	{
		myEnd[0] = x;
		myEnd[1] = y;
		myEnd[2] = z;
	}
	void TCluster::SetStartPoint(int x, int y, int z)
	{
		myStart[0] = x;
		myStart[1] = y;
		myStart[2] = z;
	}

	void TCluster::SetAngles(vector< float > & angles)
	{
		myAngles[0] = angles[0];
		myAngles[1] = angles[1];
	}
	void TCluster::SetAverageEnergy(float energy)
	{
		myEnergy = energy;
	}
	Float_t TCluster::GetAverageEnergy()
	{
		return myEnergy;
	}
	Float_t * TCluster::GetAngles()
	{
		return myAngles;
	}

	Int_t TCluster::GetType()
	{
		return myType;
	}

	Int_t * TCluster::GetStartPoint()
	{
		return myStart;
	}

	Int_t * TCluster::GetEndPoint()
	{
		return myEnd;
	}

	Int_t TCluster::GetNumberOfPads()
	{
		return myNumberOfPads;
	}

	Float_t TCluster::GetLength()
	{
		return myLength;
	}

	Float_t TCluster::GetSigma()
	{
		return mySigma;
	}


} /* MyCalorimeter */
