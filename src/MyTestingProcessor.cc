#include <cmath>
#include "MyTestingProcessor.hh"
#include <vector>
#include <algorithm>
#include <EVENT/LCIO.h>
#include <EVENT/LCCollection.h>
#include <EVENT/CalorimeterHit.h>
#include <IMPL/CalorimeterHitImpl.h>
#include <IMPL/ClusterImpl.h>
#include <IMPL/LCCollectionVec.h>
#include <values.h>
#include <marlin/ConditionsProcessor.h>
#include <cassert>
#include <UTIL/CellIDDecoder.h>
#include <cstdlib>
#include "MathOperator.hh"
#include <iterator>
using std::cout;
using std::vector;
using std::string;
using std::endl;
using MyCalorimeter::TCluster;
using MyCalorimeter::MathOperator;
namespace CALICE {
	MyTestingProcessor a_MyTestingProcessor_instance;

	MyTestingProcessor::MyTestingProcessor() : Processor("MyTestingProcessor")
	{
		_description = "Processor to test!" ;
	    registerInputCollection( LCIO::CALORIMETERHIT ,
	                             "CalorimeterHitCollection",
	                             "Name of the Calorimeter hit collection"  ,
	                             _colName ,
	                             std::string("ConvCalorimeterHits") ) ;
				         EventNumber = 30;
			     registerProcessorParameter( "EventNumber" ,
			           "The maximum number of hits allowed in the TCMT. This cut reduces the double event background from parasitic muons)" ,
				 EventNumber,
				EventNumber);
				LastFiredPadsNumber = 3;
			registerProcessorParameter( "LastFiredPadsNumber" ,
	                         "The maximum number of hits in last layer",
				                            LastFiredPadsNumber,
				                            LastFiredPadsNumber);
		LastEventsFromLayer = 25;
		registerProcessorParameter( "LastEventsFromLayer",
			"Count events from layer #",
			LastEventsFromLayer,
			LastEventsFromLayer);
		registerProcessorParameter( "RequireInteraction",
		        "Select events with interaction? 0 or 1",
			RequireInteraction,
			RequireInteraction);
		MIPEnergyCut = 2.0;
		registerProcessorParameter( "MIPEnergyCut",
			"MIP Energy Cut",
			MIPEnergyCut,
			MIPEnergyCut);
		
		registerProcessorParameter( "FirstInteractionLayer",
			"Limit on interaction layer",
			FirstInteractionLayer,
			FirstInteractionLayer);
	}
	void MyTestingProcessor::init() 
	{
		string _hfilename = "Test.root";
		hfile = new TFile( _hfilename.c_str(), "RECREATE", _hfilename.c_str() ) ;
	        _Tree = new TTree( "TestTree", "My test tree!" );
	        _Tree->Branch("nhits", &_nhits, "nhits/I");
		_Tree->Branch("interaction", &_interaction, "interaction/I");
	        _Tree->Branch("posx", _posx,"posx[nhits]/I");
	        _Tree->Branch("posy", _posy,"posy[nhits]/I");
	        _Tree->Branch("posz", _posz,"posz[nhits]/I");
		_Tree->Branch("nspads", &_nspads, "nspads/I");
		_Tree->Branch("sposx", _posx2,"sposx[nspads]/I");
		_Tree->Branch("sposy", _posy2,"sposy[nspads]/I");
		_Tree->Branch("sposz", _posz2,"sposz[nspads]/I");
		_Tree->Branch("energy", _energyHit, "energy[nhits]/F");
		_Tree->Branch("energy", _energyHit_exc, "energy_exc[nspads]/F");
		_Tree->Branch("energyDep", _energyDep,"energyDep[30]/F");
		_Tree->Branch("clusterTotal", &_clustersTotal, "clusterTotal/I");
		_Tree->Branch("tracksCount", &_tracksCount, "tracksCount/I");
		_Tree->Branch("showerCount", &_showerCount, "showerCount/I");
		_Tree->Branch("sigma",_sigma,"_sigma[clusterTotal]/F");
		_Tree->Branch("module",_module,"_module[clusterTotal]/F");
		_Tree->Branch("number",_number,"_number[clusterTotal]/I");
		_Tree->Branch("type",_type,"_type[clusterTotal]/I");
		_Tree->Branch("endZ",_endZ,"_endZ[clusterTotal]/I");
		_Tree->Branch("phi",_phi,"_phi[clusterTotal]/F");
		_Tree->Branch("teta",_teta,"_teta[clusterTotal]/F");
		//_Tree->Branch("clusters", _clusters);
		printParameters();
		goodEventCount=0;
		
		int xyNumber = 18;
		int zNumber = 30;
		
		volumeECal = new vector< vector< float > >();
		vector< float > range;
		range.push_back(-90.0);
		range.push_back(90.0);
		volumeECal->push_back(range);
		range[0] = -90.0;
		range[1] = 90.0;
		volumeECal->push_back(range);
		range[0] = 1300.0;
		range[1] = 1500.0;
		std::cout << "Initialization of MyTestingProcessor!\n";
		volumeECal->push_back(range);
		//ECal.Initialize(xyNumber,xyNumber,zNumber,something);
		ECalCopy.Initialize(xyNumber,xyNumber,zNumber,volumeECal);
	}
	void MyTestingProcessor::processRunHeader( LCRunHeader* run)
  	{
 	}
	


	void MyTestingProcessor::processEvent( LCEvent * evtP ) 
	{
		if(evtP)
		{
			try
			{
				float energy = 0.0;
				int count = 0;
				LCCollection* col_hits = evtP->getCollection( _colName ) ;
				CellIDDecoder<CalorimeterHit> cd(col_hits);
				
				int IsInteraction = col_hits->parameters().getIntVal("isInteraction");
				int InteractionZ = col_hits->parameters().getIntVal("InteractionLayer");
				if (!RequireInteraction) 
				{
					IsInteraction = 1;
					FirstInteractionLayer = 55;
				}
				if(IsInteraction && InteractionZ < FirstInteractionLayer)
				{
					int numberOfHits = col_hits->getNumberOfElements();
					TotalEventNumber++;
					for ( int hit_i = 0 ; hit_i < numberOfHits ; hit_i++ )
					{
						vector< float > v;
						CalorimeterHit *aHit = dynamic_cast<CalorimeterHit*>( col_hits->getElementAt( hit_i ));
			        	        int waferx = cd(aHit)["S-1"];
				                int wafery = cd(aHit)["M"]-1;
				                int padx = cd(aHit)["I"]-1;
				                int pady = cd(aHit)["J"]-1;
						ECalCopy.LightThePad(waferx * 6 + padx,wafery * 6 + pady, cd(aHit)["K-1"], aHit->getEnergy());
						energy += aHit->getEnergy();
					}
					if(ECalCopy.GetNumberOfActivePadsSince(LastEventsFromLayer)>LastFiredPadsNumber) 
					{
						processCalorimeterHits(InteractionZ, numberOfHits);
					}
					else
					{
						 std::cout<< "Event does not match the selection\n";
					}
					ECalCopy.DimAllPads();

					if(count > 0.0) cout << "Total: " << count << '\n';
					energyDeposition.push_back(energy);
				}
			}
			catch(lcio::DataNotAvailableException err)
			{
//				cout<< "Exception: no collection!\n";
			}
		}
	}

	void MyTestingProcessor::writeCalorimeter(int numberOfHits)
	{
		vector< MyCalorimeter::Pad * > * padsToWrite = ECalCopy.GetPads();
		_nhits = numberOfHits;
		float totalEnergy = 0.0;
		for (int i = 0; i < numberOfHits; i++)
		{
		        MyCalorimeter::Pad * pad = padsToWrite->at(i);
			vector< int > points = pad->GetCoordinates();
			_energyDep[points.at(2)] += pad->GetEnergy();
			_energyHit[i] = pad->GetEnergy();
			_posx[i] = points.at(0);
			_posy[i] = points.at(1);
			_posz[i] = points.at(2);
			totalEnergy += pad->GetEnergy();
		}
		float selectedEnergy = 0.0;
		vector< MyCalorimeter::Pad * > * anotherPads = ECalCopy.GetTrackPads();
		_nspads = anotherPads->size();
		for (int i = 0; i < _nspads; i++) 
		{
			MyCalorimeter::Pad * pad = anotherPads->at(i);
			vector< int > points = pad->GetCoordinates();
			_posx2[i] = points.at(0);
			_posy2[i] = points.at(1);
			_posz2[i] = points.at(2);
			_energyHit_exc[i] = pad->GetEnergy();
			selectedEnergy += pad->GetEnergy();

		}
		std::cout <<  numberOfHits - _nspads << " out of " << numberOfHits << " pads were excluded.\n";
		std::cout << selectedEnergy/totalEnergy*100.0 << "\% of energy is selected.\n";

	}

	void MyTestingProcessor::processCalorimeterHits(int InteractionZ, int numberOfHits)
	{
		goodEventCount++;
		writeCalorimeter(numberOfHits);
		if (InteractionZ < 0 || InteractionZ > ECalCopy.GetDimensions()[2])
		{
		        InteractionZ = 15;
		}
		MyCalorimeter::ClusterOperator opera(MIPEnergyCut, 8);
		std::cout<< "===============================================\n";
		std::cout<< "Processing event #" << goodEventCount << '\n';
		std::cout<< "===============================================\n";
		std::cout<< "Begin Clusterization\n";
		for (int i = ECalCopy.GetDimensions()[2]-1; i > 8; i--)
		{
			vector< MyCalorimeter::Pad * > pads = ECalCopy.GetActivePadsFromLayer(i);
			std::cout<< "Processing layer " << i << " with " << pads.size() << " pads...\n";
			opera.Clusterize(pads);
		}
		opera.AnalyzeAllClusters();
		_clustersTotal = opera.GetNumberOfClusters();
		_tracksCount = opera.GetTrackLikeClusters();
		_showerCount = opera.GetShowerLikeClusters();
		_interaction = InteractionZ;
		vector<TCluster*> * clusters = opera.GetTClusters();
		for (int i = 0; i < clusters->size(); i++) 
		{
			_clusters[i] = *(clusters->at(i));
			_module[i] = clusters->at(i)->GetLength();
			_number[i] = clusters->at(i)->GetNumberOfPads();
			_sigma[i] = clusters->at(i)->GetSigma();
			_type[i] =  clusters->at(i)->GetType();
			_phi[i] = clusters->at(i)->GetAngles()[0];
			_teta[i] = clusters->at(i)->GetAngles()[1];
			if (_number[i] > 2) 
			{
				_endZ[i] = clusters->at(i)->GetEndPoint()[2];
			}
			else 
			{
				_endZ[i] = -1;
			}
		}

		std::cout<< "Number of clusters: " << _clustersTotal << '\n';
		std::cout<< "Number of track like clusters: " << _tracksCount << '\n';
		std::cout<< "Number of shower like clusters: " << _showerCount << '\n';
		_Tree->Fill();
		for (int i = 0; i < 30; i++)
		{
		        _energyDep[i]=0.0;
		}

	}
	void MyTestingProcessor::end() 
	{
		cout << "EventNumber: " << EventNumber << "\n";
		hfile->cd();
		hfile->Write();
		hfile->Close();
	}
  

	/*vector<float> MyTestingProcessor::getPointAtCalorimeter(const double * start, const double * end)
	{
		int count = 0;
		vector<float> result;
		//std::cout << "Check if particle cross calorimeter volume\n";
		for (int i = 0; i < 2; i++) 
		{
			float tan = (start[i]-end[i])/(start[2] - end[2]);
			float offset = end[i]-end[2]*tan;
			float distance = (volumeECal->at(2)[0])* tan+offset;
			std::cout << "offset1:" << offset<< '\n';
			//std::cout << "offset2:" <<  start[i]-start[2]*tan<< '\n';
			std::cout << "Coordinate at ECal start: "<< distance << '\n';
			result.push_back(distance);
			if (distance > volumeECal->at(i)[0] && distance < volumeECal->at(i)[1]) 
			{
				count++;
			}
		}
		if (count > 1) 
		{
			std::cout << "Particle cross calorimeter volume!\n";
			//return true;
		}
		else 
		{
			std::cout << "Particle DOES NOT cross calorimeter volume!\n";
			//return false;
		}
		return result;
		
	}*/
}
