#include "Pad.hh"
#include <vector>
using std::vector;

namespace MyCalorimeter
{
	class Calorimeter
	{
		public:
		//
		//	Constants
		//
			static const int ZNEIGHBOURS=-3;
			static const int XNEIGHBOURS=-1;
			static const int YNEIGHBOURS=-1;
		//
		//	Constructors and destructor
		//
			Calorimeter();
			Calorimeter(int xNumberOfPads, int yNumberOfPads, int zNumberOfPads, vector< vector< float > > * regionForCalorimeter);
			virtual ~Calorimeter ();
		//
		//	Methods
		//
			void Initialize(int xNumberOfPads, int yNumberOfPads, int zNumberOfPads, vector< vector< float > > * regionForCalorimeter);
			Pad * GetPad(int x, int y, int z);
			void LightThePad(const float position[3], float energy);
			void LightThePad(int x, int y, int z, float energy);
			void DimAllPads();
			vector< int > GetDimensions() const;
			bool HasPad(int x, int y, int z);
			vector< Pad* >* GetPads();
			int GetNumberOfActivePadsSince(int z);
			vector< Pad* > GetActivePadsFromLayer(int z);
			vector< float > * GetPadPosition(vector < int > & coordinatesOfPad);
			vector< float > * GetPadPosition(int x, int y, int z);
		private:
		//
		//	Members
		//
			vector< vector< vector< Pad *> > > * myCalPads;
			vector< int > myCalNumberOfPads;
			vector< float > myCalDimensions;
			vector< vector< float > > * myRegionForCalorimeter;
			vector<Pad*>* myPositiveEnergyPads;			
		//
		//	Private methods
		//
			//Pad * findPad(float position[]);
			//Pad * findPad(int x, int y, int z);
			void assertNeighboursPadsTo(int x, int y, int z);
			void assertNeighboursSystem();
			
	};
}
