#include <sstream>

#include "TesteRootAnalysis.hh"
#include "HelmholtzHit.hh"
//
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4EventManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

static TesteRootAnalysis* instance = 0;
// Constructor
TesteRootAnalysis::TesteRootAnalysis()
{
  //RECREATE create a new file, if the file already exists it will be overwritten. (the original program was this way)
  //NEW or CREATE create a new file and open it for writing, if the file already exists the file is not opened.
  fFile = new TFile("particles.root","RECREATE");
  fTTree = new TTree("Particles","Tree that contains primary electron info");
  //Branches: energy, position, direction, event number
  //static HIT hit;
  fTTree->Branch("electron_branch", &electron, "energy/D:x/F:y/F:z/F:dx/F:dy/F:dz/F:eventNumber/I");
  
  fEnergyDep = new TFile("energyDepStep.root","RECREATE");
  tEnergyDep = new TTree("Ionization","Tree that contains energy deposition when ionization occurs.");
  tEnergyDep->Branch("stepEnergy_branch", &edep, "energy/D");
}

// Destructor

TesteRootAnalysis::~TesteRootAnalysis()
{}

// Instancia um objeto da classe TesteRootAnalysis
TesteRootAnalysis* TesteRootAnalysis::Instance()
{
  if (instance == 0 )
  {
    instance = new TesteRootAnalysis();
  }
  return instance;
}

// Write secondary data in file
void TesteRootAnalysis::Write(G4double kinEnergy, G4ThreeVector position, G4ThreeVector momentum, G4int eventNum)
{

  electron.energy = kinEnergy/eV;
  electron.x = position.getX()/cm;
  electron.y = position.getY()/cm;
  electron.z = position.getZ()/cm;
  electron.dx = momentum.getX();
  electron.dy = momentum.getY();
  electron.dz = momentum.getZ();
  electron.eventNumber = eventNum;
  
  fTTree->Fill();
  //fFile->Write();
  //G4cout << hit.eDep << G4endl;
}

// Write energy in step in file2
void TesteRootAnalysis::Write2(G4double DepEnergy){
  edep = DepEnergy/eV;
  tEnergyDep->Fill();
}

// Close files and trees
void TesteRootAnalysis::Close()
{
  fTTree->Print();
  fFile->Write();
  fFile->Close();
  
  tEnergyDep->Print();
  fEnergyDep->Write();
  fEnergyDep->Close();
}


