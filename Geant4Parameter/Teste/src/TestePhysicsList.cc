

#include "TestePhysicsList.hh"

#include "G4MuonMinus.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4ProductionCutsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4EmPenelopePhysics.hh"
//#include "G4EmStandardPhysics.hh"

#include "G4PenelopeComptonModel.hh"
#include "G4ComptonScattering.hh"

#include "G4PenelopePhotoElectricModel.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4RayleighScattering.hh"
#include "G4PenelopeRayleighModel.hh"

#include "G4PenelopeIonisationModel.hh"
#include "G4eIonisation.hh"

#include "G4PenelopeBremsstrahlungModel.hh"
#include "G4eBremsstrahlung.hh"

#include "G4UniversalFluctuation.hh"

#include "G4ParticleTypes.hh"
#include "G4ProcessManager.hh"

#include "G4eMultipleScattering.hh"

#include "G4VAtomDeexcitation.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4LossTableManager.hh"
#include "G4EmProcessOptions.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"


//constructor
TestePhysicsList::TestePhysicsList():
  G4VModularPhysicsList(){
  SetVerboseLevel(1);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // EM physics
  //RegisterPhysics(new G4EmStandardPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());
  
  // Livermore physics
  RegisterPhysics(new G4EmLivermorePhysics);
}

//destructor

TestePhysicsList::~TestePhysicsList()
{ 
}


//set energy cuts
void TestePhysicsList::SetCuts()
{
  //futuramente usar 250*eV com Garfield++
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(250*eV, 10*GeV); 
  
  
}  
