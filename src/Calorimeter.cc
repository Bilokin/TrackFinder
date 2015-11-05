//#include "Pad.h"
#include "Calorimeter.hh"
//#include "ViewController.h"
#include <iostream>

#include <cmath>


namespace MyCalorimeter
{
		//
		//	Constructors and destructor
		//
			Calorimeter::Calorimeter() 
			{
				myShowerLimit = 6;
			}
			Calorimeter::Calorimeter(int xNumberOfPads, int yNumberOfPads, int zNumberOfPads, vector< vector< float > > * regionForCalorimeter)
			{
				myShowerLimit = 6; //CHANGED 6
				Initialize(xNumberOfPads, yNumberOfPads, zNumberOfPads, regionForCalorimeter);
			}
			Calorimeter::~Calorimeter () 
			{

			}
		//
		//	Methods
		//
			void Calorimeter::Initialize(int xNumberOfPads, int yNumberOfPads, int zNumberOfPads, vector< vector< float > > * regionForCalorimeter)
			{
			ExcludeAllShowers = true;
			vector<float> * PadDimensions = new vector< float>();
			myPositiveEnergyPads = new vector< Pad* >();
			myTrackPads = new vector< Pad* >();
			myCalNumberOfPads.push_back(xNumberOfPads);
			myCalNumberOfPads.push_back(yNumberOfPads);
			myCalNumberOfPads.push_back(zNumberOfPads);
			myRegionForCalorimeter = regionForCalorimeter;
			
			for (unsigned int i = 0; i < 3; i += 1)
			{
				if (regionForCalorimeter->at(i).at(1)>regionForCalorimeter->at(i).at(0))
				{
					myCalDimensions.push_back(regionForCalorimeter->at(i).at(1)-regionForCalorimeter->at(i).at(0));
					
				}
				else
				{
					myCalDimensions.push_back(regionForCalorimeter->at(i).at(0)-regionForCalorimeter->at(i).at(1));
				};
				PadDimensions->push_back(myCalDimensions[i]/myCalNumberOfPads[i]);
			}
			
			vector< vector< vector< Pad *> >  > * vvv = new vector< vector< vector< Pad * > > >();

			for(int i = 0; i < xNumberOfPads; i++)
			{
				vector< vector< Pad  *> > vv;
				for (int j = 0; j < yNumberOfPads; j++)
				{
					vector< Pad *> v;
					for (int k = 0; k < zNumberOfPads; k++)
					{
						Pad * pad = new Pad(i,j,k,  PadDimensions);
						v.push_back(pad);
					}
				vv.push_back(v);
				}
				vvv->push_back(vv);
			}
			myCalPads = vvv;
			assertNeighboursSystem();
			}
			
			void Calorimeter::checkIfShowerPad(Pad * pad)
			{
				vector< vector< vector< Pad *> > > * neighbours = pad->GetNeighbours();
				vector< Pad * > active;
				for (int i = 0; i < neighbours->size(); i++)
				{
					for (int j = 0; j < neighbours->at(i).size(); j++) 
					{
						for (int k = 0; k < neighbours->at(i).at(j).size(); k++) 
						{
							if (neighbours->at(i).at(j).at(k)->GetEnergy() > 0.0)
							{
								active.push_back(neighbours->at(i).at(j).at(k));
							}
							
						}
					}
				}
				if (active.size() > myShowerLimit)
				{
					for (int j = 0; j < active.size(); j++)
					{
						active[j]->SetIsShowerPad(true);
					}
				}
			}
			
			Pad * Calorimeter::GetPad(int x, int y, int z)
			{
				return myCalPads->at(x).at(y).at(z);
			}
		
			vector<Pad *>* Calorimeter::GetPads()
			{
				return myPositiveEnergyPads;	
			}
			vector< Pad * > * Calorimeter::GetTrackPads()
			{
				if (myTrackPads->size() == 0) 
				{
					for (int i = 0; i < myPositiveEnergyPads->size(); i++) 
					{
						if (myPositiveEnergyPads->at(i)->IsActive()) 
						{
							myTrackPads->push_back(myPositiveEnergyPads->at(i));
						}
					}
				}
				return myTrackPads;
			}
			vector< Pad * > * Calorimeter::GetInteractionPads()
			{
				vector< Pad * > * result = new vector< Pad * >();
				for (int i = 0; i < myPositiveEnergyPads->size(); i++) 
				{
					if (!myPositiveEnergyPads->at(i)->IsActive()) 
					{
						result->push_back(myPositiveEnergyPads->at(i));
					}
				}
				return result;
			}
			int Calorimeter::GetNumberOfActivePadsSince(int z)
                        {
                         	//vector< Pad * > * tmp = new vector<Pad * >();
				int count = 0;
				for(int i = 0; i < myPositiveEnergyPads->size(); i++)
				{
					if(myPositiveEnergyPads->at(i)->GetCoordinates().at(2)>z)
					{
						count++;
						//tmp->push_back(myPositiveEnergyPads->at(i));
					}
				}
				return count;
				//return tmp;
                        }
			
			vector< Pad* > Calorimeter::GetActivePadsFromLayer(int z)
			{
				vector< Pad * > tmp;
				for(int i = 0; i < myPositiveEnergyPads->size(); i++)
				{
					Pad * pad = myPositiveEnergyPads->at(i);
				        if(pad->GetCoordinates().at(2) == z && pad->IsActive())
				        {
						tmp.push_back(pad);
				        }
				}
				return tmp;

			}

			void Calorimeter::LightThePad(const float position[3], float energy)
			{
				if(energy > 0.0)
				{
					vector < int > positionInArray;
					for (unsigned int i = 0; i < 3; i += 1)
					{
					
						//std::cout << "myRegionForCalorimeter->at("<<i<<")[0]: " << myRegionForCalorimeter->at(i)[0] << '\n';
						//std::cout << "position["<<i<<"]: " << position[i] << '\n';
						//std::cout<< std::floor((position[i] - myRegionForCalorimeter->at(i)[0])/(myCalDimensions[i]/myCalNumberOfPads[i])) << std::endl;
						positionInArray.push_back( (int)((position[i] - myRegionForCalorimeter->at(i)[0])/(myCalDimensions[i]/myCalNumberOfPads[i])) );
						//std::cout << "Region["<<i<<"]: " << position[i]- myRegionForCalorimeter->at(i)[0] << '\n';
						//std::cout << "myCalNumberOfPads["<<i<<"]: " << myCalNumberOfPads[i] << '\n';
					}
				LightThePad(positionInArray[0],positionInArray[1],positionInArray[2],energy);
				}
			}
			
			void Calorimeter::LightThePad(int x, int y, int z, float energy, const float * realposition)
			{
				Pad * pad = Calorimeter::GetPad(x,y,z);
			//	std::cout << "Z of pad: " << pad->GetCoordinates()[2] << " X of pad: " << pad->GetCoordinates()[0] <<  " X of pad: " << pad->GetCoordinates()[0]
				if(pad->GetEnergy()==0.0) myPositiveEnergyPads->push_back(pad);
				pad->SetEnergy(energy+pad->GetEnergy());
				if (pad->GetRealCoordinates().size() == 0 && realposition) 
				{
					pad->SetRealCoordinates(realposition[0], realposition[1], realposition[2]);
				}
				if (ExcludeAllShowers) 
				{
					checkIfShowerPad(pad);
				}
			}
			
			vector< float > * Calorimeter::GetPadPosition(vector < int > & coordinatesOfPad)
			{
				vector < float > * position = new vector < float >();
				for (unsigned int i = 0; i < 3; i += 1)
				{
					//std::cout<< myCalDimensions[i]/myCalNumberOfPads[i] << std::endl;
					position->push_back( std::floor(myRegionForCalorimeter->at(i)[0]+coordinatesOfPad[i]*(myCalDimensions[i]/myCalNumberOfPads[i])));
				}
				return position;
			}
			
			vector< float > * Calorimeter::GetPadPosition(int x, int y, int z)
			{
				vector < int > coordinates;
				coordinates.push_back(x);
				coordinates.push_back(y);
				coordinates.push_back(z);
				return Calorimeter::GetPadPosition(coordinates);
			}
			
			void Calorimeter::DimAllPads()
			{
				for (int i = 0; i < myPositiveEnergyPads->size(); i++)
				{
					myPositiveEnergyPads->at(i)->Dim();
				}
				/*for (unsigned int i = 0; i < myCalNumberOfPads[0]; i += 1)
				{
					for (unsigned int j = 0; j < myCalNumberOfPads[1]; j += 1)
					{
						for (unsigned int k = 0; k < myCalNumberOfPads[2]; k += 1)
						{
							myCalPads->at(i).at(j).at(k)->SetEnergy(0.0);
						}
					}
				}*/
				myPositiveEnergyPads->clear();
				myTrackPads->clear();
			}
			
			vector< int > Calorimeter::GetDimensions() const
			{
				return myCalNumberOfPads;
			}
			
			bool Calorimeter::HasPad(int x, int y, int z)
			{
				return (x>=0 && x < myCalNumberOfPads[0]) && (y>=0 && y < myCalNumberOfPads[1]) && (z>=0 && z < myCalNumberOfPads[2]);
			}

			void Calorimeter::assertNeighboursPadsTo(int x, int y, int z)
			{
				int count = 0;
				vector< vector< vector< Pad * > >  > * neighbours = new vector< vector< vector< Pad *> >  >();
				Pad * targetPad = GetPad(x,y,z);
				for (int k = 0; k > ZNEIGHBOURS; k--)
				{
					vector< vector< Pad  *> > vv;
					for (int i = XNEIGHBOURS; i < 1-XNEIGHBOURS; i++)
					{
						vector< Pad *> v;
						for (int j = YNEIGHBOURS; j < 1-YNEIGHBOURS; j++)
						{
							//std::cout << "Pad #" << x+i << '.'<< y+j << '.' << z+k << ": " << HasPad(x+i,y+j,z+k) << '\n';
							if (HasPad(x+i,y+j,z+k))
							{
								Pad * neighbour = GetPad(x+i,y+j,z+k);
								count++;
								v.push_back(neighbour);
							}
						}
						vv.push_back(v);
					}
					neighbours->push_back(vv);
				};
				//std::cout << count << " neighbours asserted;\n";
				targetPad->AddNeighbours(neighbours);
			}
			void Calorimeter::assertNeighboursSystem()
			{
				for (int i = 0; i < myCalNumberOfPads[0]; i++)
				{
					for (int j = 0; j < myCalNumberOfPads[1]; j++)
					{
						for (int k = 0; k < myCalNumberOfPads[2]; k++)
						{
							assertNeighboursPadsTo(i,j,k);
						}
					}
				}
		}	
}



















