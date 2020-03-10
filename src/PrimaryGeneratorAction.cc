/// \file  PrimaryGeneraotrAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(0){
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    //default particle kinematic
    auto particle = G4ParticleTable::GetParticleTable()->FindParticle("e+");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
    fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
    fParticleGun->SetParticleEnergy(2.2*MeV);
}


PrimaryGeneratorAction::~PrimaryGeneratorAction(){
    delete fParticleGun;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
    G4ParticleDefinition* particle = fParticleGun->GetParticleDefinition();
	    
    // In order to avoid dependence of PrimaryGeneratorAction
    // on DetectorConstruction class we get world volume from G4LogicalVolumeStore.

    G4double world_size = 0;
    auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");

    G4Box* worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());

    world_size = worldBox->GetXHalfLength();

    // Set gun position
    double x = 0, y = 0;
    x = G4RandGauss::shoot(0, 2.5 * CLHEP::cm);
    y = G4RandGauss::shoot(0, 2.5 * CLHEP::cm);
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticlePosition(G4ThreeVector(x, y, world_size));
    fParticleGun->GeneratePrimaryVertex(anEvent);
}


