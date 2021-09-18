
#ifndef TesteSteppingAction_H
#define TesteSteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class TesteSteppingAction : public G4UserSteppingAction
{
public:
  TesteSteppingAction();
  virtual ~TesteSteppingAction();
  
  virtual void UserSteppingAction(const G4Step*);
};

#endif

