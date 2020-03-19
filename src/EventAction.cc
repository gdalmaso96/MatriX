/// \file  EventAction.cc
/// \brief Implementation of the EventAction class

#include "TTree.h"

#include "RunAction.hh"
#include "EventAction.hh"
#include "ScintHit.hh"
#include "PixelHit.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4THitsMap.hh"
#include "G4SystemOfUnits.hh"
#include <sys/resource.h>

 

struct rusage usage;
EventAction::EventAction(RunAction* runAction) : G4UserEventAction(), fRunAction(runAction), fCollIDScint(-1), fCollIDSiPM(-1), fCollIDSiPMDraw(-1), fEvID(-1){}

EventAction::~EventAction(){}

void EventAction::BeginOfEventAction(const G4Event*){}

void EventAction::EndOfEventAction(const G4Event* event){
	// Hits collections
	G4HCofThisEvent*HCE = event->GetHCofThisEvent();
	if(!HCE) return;
	// Get hits collections IDs
	if(fCollIDScint < 0){
		G4SDManager* SDMan = G4SDManager::GetSDMpointer();
		fCollIDScint = SDMan->GetCollectionID("scintCollection");
	}
	
	ScintHitsCollection* ScintHitCollection = (ScintHitsCollection*) (HCE->GetHC(fCollIDScint));

	if(fCollIDSiPM < 0){
		G4SDManager* SDMan = G4SDManager::GetSDMpointer();
		fCollIDSiPM = SDMan->GetCollectionID("pixelCollection");
	}
	
	PixelHitsCollection* PixelHitCollection = (PixelHitsCollection*) (HCE->GetHC(fCollIDSiPM));
	
	ScintHit* scintHit;
	G4int N = ScintHitCollection->entries();
	PixelHit* pixelHit;
	G4int N1 = PixelHitCollection->entries();
	assert(N == N1);
	for(int i = 0; i < N; i++){
		scintHit = (*ScintHitCollection)[i];
		pixelHit = (*PixelHitCollection)[i];
		if(fEvID < 0){
			fEvID = event->GetEventID();
			fRunAction->SetEin(scintHit->GetEin());
			fRunAction->SetEdep(scintHit->GetEdep());
			fRunAction->SetEdelta(scintHit->GetEdelta());
			fRunAction->SetID(fEvID);
			fRunAction->SetNgammaSec(scintHit->GetNgammaSec());
			
			fRunAction->SetChannel(pixelHit->GetChannel());
			fRunAction->SetCells(pixelHit->GetCells());
			fRunAction->SetCellTime(pixelHit->GetCellTime());
			fRunAction->SetOCTFlag(pixelHit->GetOCTFlag());
			fRunAction->SetDNFlag(pixelHit->GetDNFlag());
			(fRunAction->GetTreePtr())->Fill();
		}
		scintHit->Clear();
		pixelHit->Clear();
	}
/*
      if (0 == getrusage(RUSAGE_SELF, &usage)) {

         std::cout << "Memory Usage (MB): " << usage.ru_maxrss / 1024. << std::endl;

      }
*/
	fRunAction->AdvanceGunTime();
	fEvID = -1;
}
