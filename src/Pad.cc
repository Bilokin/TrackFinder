#include "Pad.hh"

using MyCalorimeter::Pad;


	//
	//	Constructors and destructor
	//
		Pad::Pad (int x, int y, int z)
		{
			coordinates.push_back(x);
			coordinates.push_back(y);
			coordinates.push_back(z);
			energy = 0.0;
			isShowerPad = false;
		}
		
		Pad::Pad (int x, int y, int z, vector<float> * sizes)
		{
			coordinates.push_back(x);
			coordinates.push_back(y);
			coordinates.push_back(z);
			energy = 0.0;
		}
		
		Pad::~Pad ()
		{
			delete neighbours;
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
			return coordinates;
		}
		float Pad::GetEnergy() const
		{
			return energy;
		}
		void Pad::SetEnergy(float someEnergy)
		{
			energy = someEnergy;
		}
		void Pad::Dim()
		{
			energy = 0.0;
			isShowerPad = false;
		}
		vector<vector<vector<Pad*> > > * Pad::GetNeighbours()
		{
			return neighbours;
		}
		bool Pad::IsActive() const
		{
			return energy > 0.0 && !isShowerPad;
		}

		void Pad::AddNeighbours(vector< vector< vector< Pad*> > > * array )
		{
			neighbours = array;
		}
		

















