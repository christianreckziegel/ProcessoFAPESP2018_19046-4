


#ifndef TesteRunAction_h
#define TesteRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

/// Run action class

class TesteRunAction : public G4UserRunAction
{
  public:
    TesteRunAction();
    virtual ~TesteRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

  
  private:
    
};

#endif

