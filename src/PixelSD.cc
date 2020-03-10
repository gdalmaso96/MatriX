/// \file  PixelSD.cc
/// \brief Implementation of the PixelSD class

#include "PixelSD.hh"
#include "PixelHit.hh"
#include "RunAction.hh"

#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4Box.hh"

#include "Randomize.hh"


class DetectorConstruction;


PixelSD::PixelSD(G4String name, G4String model) : G4VSensitiveDetector(name), fNCells(0), fChannel(0), fNPhotoElectrons(0), fVoltage(56), fDetEffGain(0), fPhotonGain(0){
	fPixelCollection = nullptr;
	collectionName.insert("pixelCollection");
	fPixelCollectionDraw = nullptr;
	collectionName.insert("pixelCollectionDraw");
	
	filename[0] = "";
	filename[1] = "";
	filename[2] = "";
	filename[3] = "";
	fNbOfPixels = 0;
	fModel = model;
}

PixelSD::~PixelSD(){}

G4double PixelSD::GetAbsProbability(G4double val){
	G4double temp = 0;
	G4int N =fDetEffx.size();
	for(int i = 0; i < N; i++){
		if(i == 0){
			if((fDetEffx.at(1) - fDetEffx.at(0)) / 2 > val - fDetEffx.at(0)){
				if(val > fDetEffx.at(0)) temp = fDetEff.at(0);
				break;
			}
		}

		else if(i == N - 1){
			if((fDetEffx.at(N - 1) - fDetEffx.at(N - 2)) / 2 > fDetEffx.at(N - 1) - val){
				if(val < fDetEffx.at(N - 1)) temp = fDetEff.at(N - 1);
				break;
			}
		}

		else{
			if((fDetEffx.at(i) - fDetEffx.at(i - 1)) / 2 > fDetEffx.at(i) - val && 
			   (fDetEffx.at(i + 1) - fDetEffx.at(i)) / 2 > val - fDetEffx.at(i)){
				temp = fDetEff.at(i);
				break;
			}
		}
	}
	return temp * fDetEffGain;
}

void PixelSD::SetDetEffGain(){
	std::vector<G4double> DetEffGain;
	std::vector<G4double> DetEffGainx;
	G4double x, y;

	std::ifstream myfile;
	myfile.open("../tables/" + this->GetFileName(1));
	while(true){
		myfile >> x >> y;
		DetEffGainx.push_back(x); // x is a voltage (over the breakdown threashold)
		DetEffGain.push_back(y);
		if(myfile.eof()) break;
	}
	myfile.close();

	G4double temp = 0;
	G4int N =DetEffGainx.size();
	G4double val = fVoltage - 53;
	for(int i = 0; i < N; i++){
		if(i == 0){
			if((DetEffGainx.at(1) - DetEffGainx.at(0)) / 2 > val - DetEffGainx.at(0)){
				temp = DetEffGain.at(0);
				break;
			}
		}

		else if(i == N - 1){
			if((DetEffGainx.at(N - 1) - DetEffGainx.at(N - 2)) / 2 > DetEffGainx.at(N - 1) - val){
				temp = DetEffGain.at(N - 1);
				break;
			}
		}
		else{
			if((DetEffGainx.at(i) - DetEffGainx.at(i - 1)) / 2 > DetEffGainx.at(i) - val && 
			   (DetEffGainx.at(i + 1) - DetEffGainx.at(i)) / 2 > val - DetEffGainx.at(i)){
				temp = DetEffGain.at(i);
				break;
			}
		}
	}

	fDetEffGain = temp / 0.5;
	DetEffGain.clear();
	DetEffGainx.clear();	
}


void PixelSD::SetPhotonGain(){
	std::vector<G4double> PhotonGain;
	std::vector<G4double> PhotonGainx;
	G4double x, y;
	std::ifstream myfile;
	myfile.open("../tables/" + this->GetFileName(3));
	while(true){
		myfile >> x >> y;
		PhotonGainx.push_back(x); // x is a voltage (over the breakdown threashold)
		PhotonGain.push_back(y);
		if(myfile.eof()) break;
	}
	myfile.close();

	G4double temp = 0;
	G4int N =PhotonGainx.size();
	G4double val = fVoltage - 53;
	for(int i = 0; i < N; i++){
		if(i == 0){
			if((PhotonGainx.at(1) - PhotonGainx.at(0)) / 2 > val - PhotonGainx.at(0)){
				temp = PhotonGain.at(0);
				break;
			}
		}

		else if(i == N - 1){
			if((PhotonGainx.at(N - 1) - PhotonGainx.at(N - 2)) / 2 > PhotonGainx.at(N - 1) - val){
				temp = PhotonGain.at(N - 1);
				break;
			}
		}

		else{
			if((PhotonGainx.at(i) - PhotonGainx.at(i - 1)) / 2 > PhotonGainx.at(i) -val                    &&  (PhotonGainx.at(i + 1) - PhotonGainx.at(i)) / 2 > val - PhotonGainx.at(i)){
				temp = PhotonGain.at(i);
				break;
			}
		}
	}

	fPhotonGain = temp;
	PhotonGain.clear();
	PhotonGainx.clear();
}

void PixelSD::DefineProperties(){
	G4int i = 0, j = 0; //i is for the pitch, j is for the window material;
	if (fModel == "75PE") {i = 0; j = 1;}
	else if (fModel == "50PE") {i = 1; j = 1;}
	else if (fModel == "25PE") {i = 2; j = 1;}
	else if (fModel == "75CS") {i = 0; j = 0;}
	else if (fModel == "50CS") {i = 1; j = 0;}
	else if (fModel == "25CS") {i = 2; j = 0;};

	SetNbOfPixels(Model::NbPixelsX[i] * Model::NbPixelsY[i]);

	filename[0] = Model::eff_name[i + j * 3];
	filename[1] = Model::eff_gain_name[i];
	filename[2] = Model::OCT_gain_name[i];
	filename[3] = Model::photon_gain_name[i];
		
	std::ifstream myfile;
	myfile.open("../tables/" + this->GetFileName(0));
	G4double x, y;
	while(true){
		myfile >> x >> y;
		fDetEffx.push_back(1239.84197/x); // x is the light wave length
		fDetEff.push_back(y);
		if(myfile.eof()) break;
	}
	myfile.close();
	std::reverse(fDetEffx.begin(), fDetEffx.end());
	std::reverse(fDetEff.begin(), fDetEff.end());

	fFillFactor = Model::FillFactor[i];
	
	SetVoltage(53 + Model::OVoltage[i]);
	SetDetEffGain();
	SetPhotonGain();
	firstStep.clear();
	for(i = 0; i < this->GetNbOfPixels(); i++) firstStep.push_back(0);
}

void PixelSD::Initialize(G4HCofThisEvent* hitsCE){
	fPixelCollection = new PixelHitsCollection(SensitiveDetectorName, collectionName[0]);

	static G4int hitsCID =-1;
	if(hitsCID < 0) hitsCID = GetCollectionID(0);
	hitsCE->AddHitsCollection(hitsCID, fPixelCollection);

	fPixelCollectionDraw = new PixelHitsCollection(SensitiveDetectorName, collectionName[1]);
	
	static G4int hitsCIDDraw =-1;
	if(hitsCIDDraw < 0) hitsCIDDraw = GetCollectionID(1);
	hitsCE->AddHitsCollection(hitsCIDDraw, fPixelCollectionDraw);

}


G4bool PixelSD::ProcessHits(G4Step *aStep, G4TouchableHistory*){
	if(aStep->GetTrack()->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){

		G4TouchableHandle theTouchable = aStep->GetPreStepPoint()->GetTouchableHandle();
		G4double kCarTolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
		G4ThreeVector stppos1= aStep->GetPreStepPoint()->GetPosition();
    		G4ThreeVector localpos1 = 
      			theTouchable->GetHistory()->GetTopTransform().TransformPoint(stppos1);
		G4VPhysicalVolume* physVol = aStep->GetPreStepPoint()->GetTouchable()->GetVolume(0);
		G4double dimensions = - ((G4Box*) physVol->GetLogicalVolume()->GetSolid())->GetZHalfLength();

		if(aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary && 
		   std::fabs(localpos1.z() + dimensions) < kCarTolerance){

//		if(aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary){
			G4double energy = aStep->GetPreStepPoint()->GetKineticEnergy();
			G4double a = G4UniformRand();
			G4double p = GetAbsProbability(energy/CLHEP::eV);
			if(a < p){ // SiPM Fill Factor
				G4int replica = aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(0);
				fChannel = aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(2);
//				G4VPhysicalVolume* physVol = aStep->GetPreStepPoint()->GetTouchable()->GetVolume(0);
				G4VPhysicalVolume* physVolM = aStep->GetPreStepPoint()->GetTouchable()->GetVolume(1);
				G4VPhysicalVolume* physVolGM = aStep->GetPreStepPoint()->GetTouchable()->GetVolume(2);
				G4int nhitdraw = fPixelCollectionDraw->entries();
				PixelHit* hit = nullptr;
				for(G4int i = 0; i < nhitdraw; i++){
					if((*fPixelCollectionDraw)[i]->GetPixelNumber()==replica){
						hit = (*fPixelCollectionDraw)[i];
						break;
					}
				}
				if(hit == nullptr){
					hit = new PixelHit();
					hit->SetPixelNumber(replica);
					hit->SetPixelPhysVol(physVol);
					hit->SetPixelPhysVolMother(physVolM);
					hit->SetPixelPhysVolGMother(physVolGM);
					hit->SetDrawit(true);
					fPixelCollectionDraw->insert(hit);
				}
				if(firstStep[replica] == 0) fNCells += 1;
				fNPhotoElectrons += fPhotonGain;
				firstStep.at(replica) = 1;
				fCells.push_back(replica);
				fCellTime.push_back(aStep->GetPreStepPoint()->GetGlobalTime());
				}
		}
		aStep->GetTrack()->SetTrackStatus(fStopAndKill);
	}
	
	return false;
}

void PixelSD::EndOfEvent(G4HCofThisEvent*){
	PixelHit* Hit = new PixelHit();
	Hit->SetChannel(fChannel);
	Hit->SetNCells(fNCells);
	Hit->SetNPhotoElectrons(fNPhotoElectrons);
	Hit->SetCells(fCells);
	Hit->SetCellTime(fCellTime);
	fPixelCollection->insert(Hit);
	fNCells = 0;
	fNPhotoElectrons = 0;
	fCells.clear();
	fCellTime.clear();

	std::fill(firstStep.begin(), firstStep.end(),0);
}

void PixelSD::clear(){}

void PixelSD::DrawAll(){}

void PixelSD::PrintAll(){}


