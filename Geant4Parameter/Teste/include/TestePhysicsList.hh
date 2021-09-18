

#ifndef TestePhysicsList_h
#define TestePhysicsList_h 1

#include "G4VModularPhysicsList.hh"

/// Modular physics list
///
/// It includes the folowing physics builders
/// - G4DecayPhysics
/// - G4RadioactiveDecayPhysics
/// - G4EmStandardPhysics

class TestePhysicsList: public G4VModularPhysicsList
{
public:
  TestePhysicsList();
  virtual ~TestePhysicsList();
  
  virtual void SetCuts();
};



#endif

