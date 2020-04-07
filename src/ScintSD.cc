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

ScintSD::ScintSD(G4String name) : G4VSensitiveDetector(name), fEin(0), fEdep(0), fDelta(0), fThetaIn(0), fTrackLength(0), fSurfaceIn(0), fPrimaryChannel(0), fNgammaSec(0){
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
		fTrackLength += aStep->GetStepLength();

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
		if(preStep->GetStepStatus() == fGeomBoundary && fEin == 0){
			ein = preStep->GetKineticEnergy();
			fEin = ein;
			fParticleID = aStep->GetTrack()->GetDynamicParticle()->GetPDGcode();

			G4double kCarTolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
			G4double dimensionx = ((G4Box*) thePreTouchable->GetVolume(0)->GetLogicalVolume()->GetSolid())->GetXHalfLength();
			G4double dimensiony = ((G4Box*) thePreTouchable->GetVolume(0)->GetLogicalVolume()->GetSolid())->GetYHalfLength();
			G4double dimensionz = ((G4Box*) thePreTouchable->GetVolume(0)->GetLogicalVolume()->GetSolid())->GetZHalfLength();
			G4ThreeVector worldPos = aStep->GetPreStepPoint()->GetPosition();
			G4ThreeVector localPos = thePreTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);
			G4AffineTransform momentumTransform = thePreTouchable->GetHistory()->GetTopTransform();
			momentumTransform.SetNetTranslation(G4ThreeVector(0,0,0));
			G4ThreeVector momentumDir = momentumTransform.TransformPoint(aStep->GetPreStepPoint()->GetMomentumDirection());


			/// Surfaces:
			///          - x>0: DS right. Surf = 0
			///          - x<0: DS left.  Surf = 1
			///          - y>0: DS up.    Surf = 2
			///          - y<0: DS down.  Surf = 3
			///          - z>0: DS front. Surf = 4
			///          - z<0: DS back.  Surf = 5
			if(std::fabs(localPos.x() - dimensionx) < kCarTolerance &&
			   momentumDir.x() < 0){
				fThetaIn = acos(-momentumDir.x());
				fSurfaceIn = 0;
			}
			else if(std::fabs(localPos.x() + dimensionx) < kCarTolerance &&
			   momentumDir.x() > 0){
				fThetaIn = acos(momentumDir.x());
				fSurfaceIn = 1;
			}
			else if(std::fabs(localPos.y() - dimensiony) < kCarTolerance &&
			   momentumDir.y() < 0){
				fThetaIn = acos(-momentumDir.y());
				fSurfaceIn = 2;
			}
			else if(std::fabs(localPos.y() + dimensiony) < kCarTolerance &&
			   momentumDir.y() > 0){
				fThetaIn = acos(momentumDir.y());
				fSurfaceIn = 3;
			}
			else if(std::fabs(localPos.z() - dimensionz) < kCarTolerance &&
			   momentumDir.z() < 0){
				fThetaIn = acos(-momentumDir.z());
				fSurfaceIn = 4;
			}
			else if(std::fabs(localPos.z() + dimensionz) < kCarTolerance &&
			   momentumDir.z() > 0){
				fThetaIn = acos(momentumDir.z());
				fSurfaceIn = 5;
			}
			fPrimaryChannel = thePreTouchable->GetReplicaNumber(1);


			return false;
		}
		
		eout = postStep->GetKineticEnergy();
		if(postStep->GetStepStatus() == fGeomBoundary){
//			aStep->GetTrack()->SetTrackStatus(fStopAndKill);
			return true;
		}
		else if (eout == 0.){
//			aStep->GetTrack()->SetTrackStatus(fStopAndKill);
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
	Hit->SetThetaIn(fThetaIn);
	Hit->SetTrackLength(fTrackLength);
	Hit->SetSurfaceIn(fSurfaceIn);
	Hit->SetPrimaryChannel(fPrimaryChannel);
	Hit->SetNgammaSec(fNgammaSec);
	Hit->SetParticleID(fParticleID);
	fScintCollection->insert(Hit);
	fEdep = 0;
	fEin = 0;
	fDelta = 0;
	fTrackLength = 0;
	fNgammaSec = 0;
}

void ScintSD::clear(){}

void ScintSD::DrawAll(){}

void ScintSD::PrintAll(){}


	
		
