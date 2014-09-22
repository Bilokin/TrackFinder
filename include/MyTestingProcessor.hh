#ifndef _MyTestingProcessor_hh_
#define _MyTestingProcessor_hh_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Calorimeter.hh"
#include "ClusterOperator.hh"
#include <string>
#include <iostream>
#include <marlin/Processor.h>
#include <vector>
#include <EVENT/CalorimeterHit.h>
#include <values.h>
#include <EVENT/MCParticle.h>
#include "TCluster.hh"

#include "TFile.h"
#include "TTree.h"
namespace CALICE {
                                                              
  /** A processor to find the outgoing particles (mip) 
   * in the last layers of the SiW Ecal
   * Author: Bilokin Sviatoslav @ LAL 2012/13
   */

  class MyTestingProcessor : public marlin::Processor
  {
  public:
    Processor*  newProcessor() { return new MyTestingProcessor ; }

    MyTestingProcessor();

    ~MyTestingProcessor() {};


    /** Called at the begin of the job before anything is read.
     * Use to initialize the processor, e.g. book histograms.
     */
    void init();
    /** Called for every run, e.g. overwrite to initialize run dependent 
     *  histograms.
     */
    void processRunHeader( LCRunHeader* run);

    /** Called for every event - the working horse. 
     */
    void processEvent( LCEvent * evtP );

    //Called at the end of the job
    void end();

  protected:

    std::string _colName;              /**< name of the hit input collection (Marlin processor parameter)*/

  private:
	void processCalorimeterHits(int InteractionZ, int numberOfHits);
	//bool applyPrimaryCutsTo(MCParticle * particle);
	//void writeParticle(MCParticle * particle, int i);
	//int applyIntegrationCuts(const vector<MCParticle*> & particle);
	//vector<float> getPointAtCalorimeter(const double * start, const double * end);
	float maxminPosition[3][2];
	std::vector< float > energyDeposition;
        int EventNumber;
        int TotalEventNumber;
	int LastFiredPadsNumber;
	int LastEventsFromLayer;
	int FirstInteractionLayer;
	int RequireInteraction;
	float MIPEnergyCut;
	TFile* hfile ;
        std::string _hfilename ;
        TTree* _Tree;
        //TTree* _MCTree;
        static const int MAXN = 10000 ;
        int _posx[MAXN];   // hit position x, in mm, 0 is the center of ECAL
        int _posy[MAXN];   // hit position y, in mm, 0 is the center of ECAL
        int _posz[MAXN];   // hit position z, in mm, starting at the frount of ECAL
	float _energyHit[MAXN];
	float _energyDep[30];
	int _nMCparticlesTrue;
	int _nMCparticles;
	int _numberMCP_corrected;
	float _energy[MAXN];
	float _energy_kin[MAXN];
	float _mass[MAXN];
	float _zmomentum[MAXN];
	int _charge[MAXN];
	float _distance[MAXN];
	vector< vector< float > > * volumeECal;
	int _tracksCount;
	int _clustersTotal;
	int _showerCount;
	int _count;			
	int _nhits;	
	int goodEventCount;
	int _interaction;
	float _sigma[MAXN];
	float _module[MAXN];
	int _number[MAXN];
	int _type[MAXN];
	int _startY[MAXN];
	int _startX[MAXN];
	int _startZ[MAXN];
	int _endY[MAXN];
	int _endX[MAXN];
	int _endZ[MAXN];

	float _angleCut;
	float _ekinCut;
	float _distanceCut;
	
	MyCalorimeter::TCluster _clusters[MAXN];

	MyCalorimeter::Calorimeter ECalCopy;
	MyCalorimeter::Calorimeter ECal;
	std::vector< std::vector< float > > Coordinates;
  };

}


#endif
