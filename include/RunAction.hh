/// \file RunAction.hh
/// \brief Definition of RunAction class

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include <vector>

#include "TVector3.h"

class G4Run;
class TFile;
class TTree;
class RunActionMessenger;

/// Run action class
///
/// It prints out the data Tree


class RunAction : public G4UserRunAction {
	public:
		RunAction();
		virtual ~RunAction();

        //virtual G4Run* GenerateRun();
		virtual void BeginOfRunAction(const G4Run*);
		virtual void   EndOfRunAction(const G4Run*);
		
		void SetFileName(G4String name){fName = name;}

		inline TFile* GetFilePtr(){return fData;}
		inline TTree* GetTreePtr(){return fTree;}

		void SetEin (G4double val){fEin  = val;}
		void SetEdep(G4double val){fEdep = val;}
		void SetEdelta(G4double val){fDelta = val;}
		void SetThetaIn(G4double val){fThetaIn = val;}
		void SetTrackLength(G4double val){fTrackLength = val;}
		void SetSurfaceIn(G4int val){fSurf = val;}
		void SetPrimaryChannel(G4int val){fPrimaryChannel = val;}
		void SetID(G4int val){fID = val;}
		void SetChannel(std::vector<G4int> val){fChannel = val;}
		void SetNgammaSec(G4int ngammasec){fNgammaSec = ngammasec;}
		void SetParticleID(G4int particleID){fParticleID = particleID;}
		
		void SetCmdOCT(G4bool cmd){fCmdOCT = cmd;}
		G4bool GetCmdOCT(){return fCmdOCT;}

		void SetCmdDN(G4bool cmd){fCmdDN = cmd;}
		G4bool GetCmdDN(){return fCmdDN;}
		
		void SetCells(std::vector<G4int> val){fCells = val;}
		void SetCellTime(std::vector<G4double> val){fCellTime = val;}
		void SetOCTFlag(std::vector<G4int> val){fOCTflag = val;}
		void SetDNFlag(std::vector<G4int> val){fDNflag = val;}

		// Gun time
		inline void SetGunTimeMean(G4double val){fGunTimeMean = val;}
		inline G4double GetGunTime(){return fGunTime;}
		inline void AdvanceGunTime(){fGunTime+= G4RandExponential::shoot(fGunTimeMean);}

		// Datk noise time
		inline void SetDNTimeMean(G4double val){fDNTimeMean = val;}
		inline G4double GetDNTime(G4int i){return fDNTime.at(i);}
		inline void AdvanceDNTime(G4int i){fDNTime.at(i) += G4RandExponential::shoot(fDNTimeMean);}


	private:
		TFile* fData;
		TTree* fTree;
		// BC400 scorers
		G4double fEin;
		G4double fEdep;
		G4double fDelta;
		G4double fThetaIn;
		G4double fTrackLength;
		G4int fSurf;
		G4int fPrimaryChannel;
		G4int fID;
		std::vector<G4int> fChannel;

		G4int fNgammaSec, fParticleID;

		G4bool fCmdOCT, fCmdDN;
		// SiPM scorers
		std::vector<G4int> fCells;
		std::vector<G4double> fCellTime;
		std::vector<G4int> fOCTflag;
		std::vector<G4int> fDNflag;

		//SiPM time counters
		G4double fGunTime;
		std::vector<G4double> fDNTime;
		G4double fGunTimeMean, fDNTimeMean;



		G4String fName;
		RunActionMessenger* fMessenger;
};

#endif


