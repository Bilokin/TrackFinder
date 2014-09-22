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
			Calorimeter::Calorimeter() {}
			Calorimeter::Calorimeter(int xNumberOfPads, int yNumberOfPads, int zNumberOfPads, vector< vector< float > > * regionForCalorimeter)
			{
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
			vector<float> * PadDimensions = new vector< float>();
			PositiveEnergyPads = new vector< Pad* >();
			CalNumberOfPads.push_back(xNumberOfPads);
			CalNumberOfPads.push_back(yNumberOfPads);
			CalNumberOfPads.push_back(zNumberOfPads);
			RegionForCalorimeter = regionForCalorimeter;
			
			for (unsigned int i = 0; i < 3; i += 1)
			{
				if (regionForCalorimeter->at(i).at(1)>regionForCalorimeter->at(i).at(0))
				{
					CalDimensions.push_back(regionForCalorimeter->at(i).at(1)-regionForCalorimeter->at(i).at(0));
					
				}
				else
				{
					CalDimensions.push_back(regionForCalorimeter->at(i).at(0)-regionForCalorimeter->at(i).at(1));
				};
				//controller.View("CalDimensions: ", CalDimensions[i]);
				PadDimensions->push_back(CalDimensions[i]/CalNumberOfPads[i]);
				//controller.View("PadDimensions: ", PadDimensions->at(i));
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
				//controller.View("New vector with vector with pad size: ", vv.size());
				}
			vvv->push_back(vv);
			//controller.View("New vector with vector with vector with pad size: ", vvv->size());
			}
			CalPads = vvv;
			assertNeighboursSystem();
			}
			
			
			
			Pad * Calorimeter::GetPad(int x, int y, int z)
			{
				return CalPads->at(x).at(y).at(z);
			}
		
			vector<Pad *>* Calorimeter::GetPads()
			{
				return PositiveEnergyPads;	
			}
			
			int Calorimeter::GetNumberOfActivePadsSince(int z)
                        {
                         	//vector< Pad * > * tmp = new vector<Pad * >();
				int count = 0;
				for(int i = 0; i < PositiveEnergyPads->size(); i++)
				{
					if(PositiveEnergyPads->at(i)->GetCoordinates().at(2)>z)
					{
						count++;
						//tmp->push_back(PositiveEnergyPads->at(i));
					}
				}
				return count;
				//return tmp;
                        }
			
			vector< Pad* > Calorimeter::GetActivePadsFromLayer(int z)
			{
				vector< Pad * > tmp;
				for(int i = 0; i < PositiveEnergyPads->size(); i++)
				{
				        if(PositiveEnergyPads->at(i)->GetCoordinates().at(2) == z)
				        {
				                tmp.push_back(PositiveEnergyPads->at(i));
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
					
						//std::cout << "RegionForCalorimeter->at("<<i<<")[0]: " << RegionForCalorimeter->at(i)[0] << '\n';
						//std::cout << "position["<<i<<"]: " << position[i] << '\n';
						//std::cout<< std::floor((position[i] - RegionForCalorimeter->at(i)[0])/(CalDimensions[i]/CalNumberOfPads[i])) << std::endl;
						positionInArray.push_back( (int)((position[i] - RegionForCalorimeter->at(i)[0])/(CalDimensions[i]/CalNumberOfPads[i])) );
						//std::cout << "Region["<<i<<"]: " << position[i]- RegionForCalorimeter->at(i)[0] << '\n';
						//std::cout << "CalNumberOfPads["<<i<<"]: " << CalNumberOfPads[i] << '\n';
					}
				LightThePad(positionInArray[0],positionInArray[1],positionInArray[2],energy);
				}
			}
			
			void Calorimeter::LightThePad(int x, int y, int z, float energy)
			{
				Pad * pad = Calorimeter::GetPad(x,y,z);
			//	std::cout << "Z of pad: " << pad->GetCoordinates()[2] << " X of pad: " << pad->GetCoordinates()[0] <<  " X of pad: " << pad->GetCoordinates()[0]
				if(pad->GetEnergy()==0.0) PositiveEnergyPads->push_back(pad);
				pad->SetEnergy(energy+pad->GetEnergy());
			}
			
			vector< float > * Calorimeter::GetPadPosition(vector < int > & coordinatesOfPad)
			{
				//ViewController controller;
				vector < float > * position = new vector < float >();
				for (unsigned int i = 0; i < 3; i += 1)
				{
					//controller.View("CalDimensions: ", CalDimensions[i]);
					//std::cout<< CalDimensions[i]/CalNumberOfPads[i] << std::endl;
					position->push_back( std::floor(RegionForCalorimeter->at(i)[0]+coordinatesOfPad[i]*(CalDimensions[i]/CalNumberOfPads[i])));
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
				for (unsigned int i = 0; i < CalNumberOfPads[0]; i += 1)
				{
					for (unsigned int j = 0; j < CalNumberOfPads[1]; j += 1)
					{
						for (unsigned int k = 0; k < CalNumberOfPads[2]; k += 1)
						{
							CalPads->at(i).at(j).at(k)->SetEnergy(0.0);
						}
					}
				}
				PositiveEnergyPads->clear();
			}
			
			vector< int > Calorimeter::GetDimensions() const
			{
				return CalNumberOfPads;
			}
			
			bool Calorimeter::HasPad(int x, int y, int z)
			{
				return (x>=0 && x < CalNumberOfPads[0]) && (y>=0 && y < CalNumberOfPads[1]) && (z>=0 && z < CalNumberOfPads[2]);
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
				for (int i = 0; i < CalNumberOfPads[0]; i++)
				{
					for (int j = 0; j < CalNumberOfPads[1]; j++)
					{
						for (int k = 0; k < CalNumberOfPads[2]; k++)
						{
							assertNeighboursPadsTo(i,j,k);
						}
					}
				}
		}	
}



















