/// \file  SteppingAction.hh
/// \brief Definition of the SteppingAction class

#ifndef SteppingAction_H
#define SteppingACtion_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class EventAction;

class SteppingAction : public G4UserSteppingAction
{
  public:

    SteppingAction(EventAction*);
    virtual ~SteppingAction();
    virtual void UserSteppingAction(const G4Step*);

  private:

    EventAction*       fEventAction;
};

#endif
