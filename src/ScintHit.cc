/// \file  ScintHit.cc
/// \brief Implementation of the ScintHit class

#include "ScintHit.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

G4ThreadLocal G4Allocator<ScintHit>* ScintHitAllocator = nullptr;

ScintHit::ScintHit() : fEin(0.), fEdep(0.), fDelta(0), fNgammaSec(0), fParticleID(0), fPhysVol(nullptr){}

ScintHit::ScintHit(G4VPhysicalVolume* pVol) : fPhysVol(pVol){}

ScintHit::~ScintHit(){}

ScintHit::ScintHit(const ScintHit &right) : G4VHit(){
	fEin  = right.fEin;
	fEdep = right.fEdep;
	fDelta = right.fDelta;
	fNgammaSec = right.fNgammaSec;
	fParticleID = right.fParticleID;
	fPhysVol = right.fPhysVol;
}

const ScintHit& ScintHit::operator=(const ScintHit &right){
	fEin  = right.fEin;
	fEdep = right.fEdep;
	fDelta = right.fDelta;
	fNgammaSec = right.fNgammaSec;
	fParticleID = right.fParticleID;
	fPhysVol = right.fPhysVol;
	return* this;
}

G4bool ScintHit::operator==(const ScintHit &) const{
	return false;
}

void ScintHit::Draw(){}

void ScintHit::Print(){}


