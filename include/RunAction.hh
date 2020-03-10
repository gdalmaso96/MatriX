/// \file RunAction.hh
/// \brief Definition of RunAction class

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
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
		void SetID(G4int val){fID = val;}
		void SetChannel(G4int val){fChannel = val;}
		void SetNgammaSec(int ngammasec){fNgammaSec = ngammasec;}
		
		void SetNCells(G4int val){fNCells = val;}
		void SetNPhotoElectrons(G4double val){fNPhotoElectrons = val;}
		void SetCells(std::vector<G4int> val){fCells = val;}
		void SetCellTime(std::vector<G4double> val){fCellTime = val;}

	private:
		TFile* fData;
		TTree* fTree;
		// BC400 scorers
		G4double fEin;
		G4double fEdep;
		G4double fDelta;
		G4int fID;
		G4int fChannel;

		G4int fNgammaSec;

		// SiPM scorers
		G4int fNCells;
		G4double fNPhotoElectrons;
		std::vector<G4int> fCells;
		std::vector<G4double> fCellTime;


		G4String fName;
		RunActionMessenger* fMessenger;
};

#endif


