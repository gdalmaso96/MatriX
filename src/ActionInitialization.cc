/// \file  ActionInitialization.cc
/// \brief Implementation on the ActionInitialization class

#include "ActionInitialization.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization(){}

ActionInitialization::~ActionInitialization(){}

void ActionInitialization::BuildForMaster() const{
    SetUserAction(new RunAction);
}

void ActionInitialization::Build() const{
    RunAction* runAction = new RunAction();
    SetUserAction(runAction);

    EventAction * eventAction = new EventAction(runAction);
    SetUserAction(eventAction);
    SetUserAction(new SteppingAction(eventAction));
    SetUserAction(new PrimaryGeneratorAction);
}


