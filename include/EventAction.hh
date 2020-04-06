/// \file  EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;
class TTree;

class EventAction : public G4UserEventAction{
	public:
		EventAction(RunAction* runAction);
		virtual ~EventAction();

		virtual void BeginOfEventAction(const G4Event*);
		virtual void   EndOfEventAction(const G4Event*);

		inline void SetThetaIn(G4double val){fThetaIn = val;}
		inline void SetSurfaceIn(G4int val){fSurf = val;}
		inline void SetPrimaryChannel(G4int val){fPrimaryChannel = val;}

	private:
		RunAction* fRunAction;
		
		G4int fCollIDScint;
		G4int fCollIDSiPM;
		G4int fCollIDSiPMDraw;
		
		G4int fEvID; // to register each event just once

		G4double fThetaIn;
		G4int fSurf, fPrimaryChannel;

		TTree* fTree;
};

#endif


