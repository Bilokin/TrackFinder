#include "Pad.hh"

using MyCalorimeter::Pad;


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
		void Pad::SetIsShowerPad(bool value)
		{
			isShowerPad = value;
		}
		vector<int> & Pad::GetCoordinates()
		{
			return myCoordinates;
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
		

















