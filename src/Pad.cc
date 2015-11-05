#include "Pad.hh"

using MyCalorimeter::Pad;
		const float Pad::X1 = 2.5;
		const float Pad::X2 = 1.295;

	//
	//	Constructors and destructor
	//
		Pad::Pad (int x, int y, int z)
		{
			myCoordinates.push_back(x);
			myCoordinates.push_back(y);
			myCoordinates.push_back(z);
			myEnergy = 0.0;
			isShowerPad = false;
		}
		
		Pad::Pad (int x, int y, int z, vector<float> * sizes)
		{
			myCoordinates.push_back(x);
			myCoordinates.push_back(y);
			myCoordinates.push_back(z);
			myEnergy = 0.0;
		}
		
		Pad::~Pad ()
		{
			delete myNeighbours;
		}
		
	//
	//	Methods
	//
		bool Pad::operator==(const Pad& another)
		{
			if (myCoordinates[0] == another.GetCoordinates()[0] &&
			    myCoordinates[1] == another.GetCoordinates()[1] &&
			    myCoordinates[2] == another.GetCoordinates()[2]) 
			{
				return true;
			}
			return false;
		}
		void Pad::SetIsShowerPad(bool value)
		{
			isShowerPad = value;
		}
		const vector<int> & Pad::GetCoordinates() const
		{
			return myCoordinates;
		}
		void Pad::SetRealCoordinates(float x, float y, float z)
		{
			int layer = myCoordinates[2];
			myRealCoordinates.push_back(round(x -2.0 - layer%2 * X1 - (int)((float)(layer%10 - layer % 2)/2.0) * X2));
			myRealCoordinates.push_back(y);
			myRealCoordinates.push_back(z);
		}
		const vector<float> & Pad::GetRealCoordinates() const
		{
			return myRealCoordinates;
		}
		float Pad::GetEnergy() const
		{
			return myEnergy;
		}
		void Pad::SetEnergy(float someEnergy)
		{
			myEnergy = someEnergy;
		}
		void Pad::Dim()
		{
			myEnergy = 0.0;
			isShowerPad = false;
		}
		vector<vector<vector<Pad*> > > * Pad::GetNeighbours()
		{
			return myNeighbours;
		}
		bool Pad::IsActive() const
		{
			return myEnergy > 0.0 && !isShowerPad;
		}

		void Pad::AddNeighbours(vector< vector< vector< Pad*> > > * array )
		{
			myNeighbours = array;
		}
		

















