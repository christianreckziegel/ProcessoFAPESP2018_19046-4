#ifndef TesteRootAnalysis_h
#define TesteRootAnalysis_h 1

// Include root files
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "g4root.hh"

// Include geant4 files
#include "HelmholtzHit.hh"

typedef struct {
    Double_t energy;
    //Int_t pdgID;
    Float_t x, y, z;
    Float_t dx, dy, dz;
    Int_t eventNumber;
} PARTICLE;

class TesteRootAnalysis 
{
public: 
  virtual ~TesteRootAnalysis();
  
  static TesteRootAnalysis* Instance();
  void Write(G4double kinEnergy, G4ThreeVector position, G4ThreeVector momentum, G4int eventNum);
  void Close();
  void Write2(G4double DepEnergy);

protected:
  TesteRootAnalysis(); 
  
private:
  TTree* fTTree;
  TFile* fFile;
  int fNevents;
  PARTICLE electron;
  
  TTree* tEnergyDep;
  TFile* fEnergyDep;
  G4double edep;
  
};
#endif
