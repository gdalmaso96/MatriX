/// \file  ScinSD.cc
/// \brief Implementation of the ScintSD class

#include "ScintSD.hh"
#include "ScintHit.hh"
#include "RunAction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"

#include "G4Box.hh"
#include "G4GeometryTolerance.hh"
#include "G4VSolid.hh"


#include "TVector3.h"

ScintSD::ScintSD(G4String name) : G4VSensitiveDetector(name), fEin(0), fEdep(0), fDelta(0), fNgammaSec(0){
	fScintCollection = nullptr;
	collectionName.insert("scintCollection");
}

ScintSD::~ScintSD(){}

void ScintSD::Initialize(G4HCofThisEvent* hitsCE){
	fScintCollection = new ScintHitsCollection(SensitiveDetectorName, collectionName[0]);

	// Putting all the hits in the same place
	static G4int hitsCID = -1;
	if(hitsCID<0){
		hitsCID = GetCollectionID(0);
	}
	hitsCE->AddHitsCollection(hitsCID, fScintCollection);
}

///aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "e+" && 
G4bool ScintSD::ProcessHits(G4Step *aStep, G4TouchableHistory*){
	if(aStep->GetTrack()->GetTrackID() == 1){
		G4double edep = aStep->GetTotalEnergyDeposit();
		G4double delta = aStep->GetPostStepPoint()->GetKineticEnergy() - aStep->GetPreStepPoint()->GetKineticEnergy() + edep;
		
		G4TouchableHistory* thePreTouchable = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
		G4VPhysicalVolume* thePrePV = thePreTouchable->GetVolume();
		G4TouchableHistory* thePostTouchable = (G4TouchableHistory*)(aStep->GetPostStepPoint()->GetTouchable());
		G4VPhysicalVolume* thePostPV = thePostTouchable->GetVolume();
		if(thePrePV->GetName() != "Crystal" && thePostPV->GetName() != "Crystal"){
			return false;
		} 

		fEdep += edep;
		fDelta -= delta;
		
		G4double ein = 0, eout = 0;
		G4StepPoint* preStep = aStep->GetPreStepPoint();
		G4StepPoint* postStep = aStep->GetPostStepPoint();
		
		// Counting and classifying photons
		const std::vector<const G4Track*>* secondaries = aStep->GetSecondaryInCurrentStep();
		if(secondaries->size() > 0){
			for(unsigned int i = 0; i < secondaries->size(); i++){
				if(secondaries->at(i)->GetParentID() > 0){
					if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
						fNgammaSec += 1;
						
					}
				}
			}
		}

		// Saving e+ characteristics				
		if(aStep->IsFirstStepInVolume() && fEin == 0){
			ein = preStep->GetKineticEnergy();
			fEin = ein;
			fParticleID = aStep->GetTrack()->GetDynamicParticle()->GetPDGcode();
			return false;
		}
		
		eout = postStep->GetKineticEnergy();
		if(postStep->GetStepStatus() == fGeomBoundary){
			aStep->GetTrack()->SetTrackStatus(fStopAndKill);
			return true;
		}
		else if (eout == 0.){
			aStep->GetTrack()->SetTrackStatus(fStopAndKill);
			return true;
		}
		
		return false;
	}
	else{
		const std::vector<const G4Track*>* secondaries = aStep->GetSecondaryInCurrentStep();
		if(secondaries->size() > 0){
			for(unsigned int i = 0; i < secondaries->size(); i++){
				if(secondaries->at(i)->GetParentID() > 0){
					if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
						fNgammaSec += 1;
					}
				}
			}
		}
		return false;
	}

}

void ScintSD::EndOfEvent(G4HCofThisEvent*){
	ScintHit* Hit = new ScintHit();
	Hit->SetEin(fEin);
	Hit->SetEdep(fEdep);
	Hit->SetEdelta(fDelta);
	Hit->SetNgammaSec(fNgammaSec);
	Hit->SetParticleID(fParticleID);
	fScintCollection->insert(Hit);
	fEdep = 0;
	fEin = 0;
	fDelta = 0;
	fNgammaSec = 0;
}

void ScintSD::clear(){}

void ScintSD::DrawAll(){}

void ScintSD::PrintAll(){}


	
		
