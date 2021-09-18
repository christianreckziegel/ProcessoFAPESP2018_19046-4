



#ifndef TesteEventAction_h
#define TesteEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"



/// Event action class
class TesteEventAction : public G4UserEventAction
{
  public:
    TesteEventAction();
    virtual ~TesteEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);


  private:
    G4int fHelmholtzCollID;
};


#endif
