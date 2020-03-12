/// \file DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"


DetectorMessenger::DetectorMessenger(DetectorConstruction* Det) : G4UImessenger(), fDetectorConstruction(Det){
	fMatrixDirectory = new G4UIdirectory("/Matrix/");
	fMatrixDirectory->SetGuidance("UI commands specific to this simulation.");
	
	fDetDirectory = new G4UIdirectory("Matrix/det/");
	fDetDirectory->SetGuidance("Detector construction control");
	
	fCrysSizeCmd = new G4UIcmdWithADoubleAndUnit("/Matrix/det/CrysSize", this);
	fCrysSizeCmd->SetGuidance("Define BC400 crystal size");
	fCrysSizeCmd->SetParameterName("CrysSize", false);
	fCrysSizeCmd->SetUnitCategory("Length");
	fCrysSizeCmd->AvailableForStates(G4State_Idle);
	
	fCrysSizeCmd3 = new G4UIcmdWith3VectorAndUnit("/Matrix/det/CrysSize3", this);
	fCrysSizeCmd3->SetGuidance("Define BC400 crystal size");
	fCrysSizeCmd3->SetParameterName("CrysSizeX", "CrysSizeY", "CrysSizeZ", false);
	fCrysSizeCmd3->SetUnitCategory("Length");
	fCrysSizeCmd3->AvailableForStates(G4State_Idle);
	
	fSiPMmodelCmd = new G4UIcmdWithAString("/Matrix/det/SiPMmodel", this);
	fSiPMmodelCmd->SetGuidance("Set SiPM model");
	fSiPMmodelCmd->SetParameterName("model", false);
	fSiPMmodelCmd->SetCandidates("75PE || 50PE || 25PE || 75CS || 50CS || 25CS");
	fSiPMmodelCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	fCrysMaterialCmd = new G4UIcmdWithAString("/Matrix/det/CrysMaterial", this);
	fCrysMaterialCmd->SetGuidance("Set scintillating material");
	fCrysMaterialCmd->SetParameterName("material", false);
	fCrysMaterialCmd->SetCandidates("BC400 || LYSO");
	fCrysMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	fMaskCmd = new G4UIcmdWithABool("/Matrix/det/Mask", this);
	fMaskCmd->SetGuidance("Put absorbing mask on MatriX");
	fMaskCmd->SetParameterName("mask", false);
	fMaskCmd->AvailableForStates(G4State_Idle);
}

DetectorMessenger::~DetectorMessenger(){
	delete fCrysSizeCmd;
	delete fCrysSizeCmd3;
	delete fSiPMmodelCmd;
	delete fMaskCmd;
	delete fCrysMaterialCmd;
	delete fDetDirectory;
	delete fMatrixDirectory;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue){
/*
	if(command == fCrysSizeCmd){
		if(fCrysSizeCmd3->GetNew3VectorValue(newValue)){
			fDetectorConstruction->SetCrystalSize(fCrysSizeCmd3->GetNew3VectorValue(newValue));
		}
		else{
			fDetectorConstruction->SetCrystalSize(fCrysSizeCmd->GetNewDoubleValue(newValue));
		}
	}
*/

	if(command == fCrysSizeCmd){
		fDetectorConstruction->SetCrystalSize(fCrysSizeCmd->GetNewDoubleValue(newValue));
	}
	
	else if (command == fCrysSizeCmd3){
		fDetectorConstruction->SetCrystalSize(fCrysSizeCmd3->GetNew3VectorValue(newValue));
	}

	else if(command == fSiPMmodelCmd){
		fDetectorConstruction->SetSiPMmodel(newValue);
	}

	else if(command == fCrysMaterialCmd){
		fDetectorConstruction->SetCrystalMaterial(newValue);
	}

	else if(command == fMaskCmd){
		fDetectorConstruction->SetMask(fMaskCmd->GetNewBoolValue(newValue));
	}
}



