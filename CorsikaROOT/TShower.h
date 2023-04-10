#ifndef _INCLUDE_CORSIKAIO_TSHOWER_
#define _INCLUDE_CORSIKAIO_TSHOWER_

#include <TObject.h>
#include <TClonesArray.h> // HERE instead of TTree, store particle data as TClonesArray with Class "TParticle", "TCherenkov", "TLongitudinal"

// HERE added headers to read TParticle, TCherenkov and TLongitudinal
#include <crs/MRunHeader.h>
#include <crs/MRunEnd.h>
#include <crs/MEventHeader.h>
#include <crs/MEventEnd.h>
#include <crs/MParticleBlock.h>
#include <crs/MLongitudinalBlock.h> 

namespace crs2r {
  class TC2R;
};

namespace crsIO {

  /** 
      \class TShower
      \brief All available info for a CORSIKA shower.

      All available info for one simulated CORSIKA event.

      \author Ralf Ulrich
      \date Thu Feb  3 13:04:50 CET 2005
      \version $Id: TShower.h 5116 2016-01-04 19:09:04Z darko $
  */

  class TShower : public TObject {
    
    friend class crs2r::TC2R;

  public:
    TShower ();
    ~TShower ();

    void Init();
    void End();
    
    // HERE Returns TClonesArray of particle data in single shower (no pointer used, since TClonesArray pointer is not fully functional in ROOT v5.34...)
    TClonesArray GetParticles () const {return fParticles;}
    TClonesArray GetLongitudinals () const {return fLongitudinal;}
    TClonesArray GetCherenkovs () const {return fCherenkov;} //

  protected:
    void Clear ();

    void AddRunHeader (const crs::MRunHeader &SubBlock);
    void AddRunEnd (const crs::MRunEnd &RunEnd);
    void AddEventHeader (const crs::MEventHeader &SubBlock);
    void AddEventEnd (const crs::MEventEnd &SubBlock);
    // HERE functions for adding particle (longitidinal) info to TClonesArray
    void AddLongitudinal (const crs::MLongitudinalBlock &SubBlock);
    void AddParticle(const crs::MParticleBlock &Data); //
 
  private:
    // HERE # of Particle (and others) per shower
    int fNParticles;
    int fNLong;
    int fNCherenkov; //
    
  public:
    // HERE TClonesArray for storing particle (and other) data per shower. Not using pointer, need to find better way than this.
    TClonesArray fParticles = TClonesArray("crsIO::TParticle", 1000); 
    TClonesArray fLongitudinal = TClonesArray("crsIO::TLongitudinal", 100000);
    TClonesArray fCherenkov = TClonesArray("crsIO::TCherenkov", 100000); //
    
    int EventID;

    float Energy;            // GeV
    float StartingAltitude;  // g/cm2
    int FirstTarget;
    float FirstHeight;        // cm
    float Theta;               // rad
    float Phi;             // rad


    int RandomSeed [10]; // #max = 10
    int RandomOffset [10]; // #max = 10
    
    // statistics
    float nPhotons;        // weighted
    float nElectrons;      // weighted
    float nHadrons;
    float nMuons;
    int nParticlesWritten; // +cherenkov -muonaddinfos
    int nPhotonsWritten;
    int nElectronsWritten;
    int nHadronsWritten;
    int nMuonsWritten;


    // NKG info
    //std::vector <float> LateralDistribution;


    // Gaisser Hillas charged particles
    float GH_Nmax;
    float GH_t0;
    float GH_tmax;
    float GH_a;
    float GH_b;
    float GH_c;
    float GH_Chi2;
	

    int nPreshower;
	
    //int nPhotonsWritten;
    //int nElectronsWritten;
    //int nHadronsWritten;
    //int nMuonsWritten;
	

	
    // ... NKG
	
    // -------
    int CPUtime; // [s]
	
	
    ClassDef (TShower, 2)  
  };

};

#endif
