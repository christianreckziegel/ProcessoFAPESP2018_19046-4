
#include "TesteSteppingAction.hh"
#include "TesteRegionInformation.hh"
//#include "TesteTrackInformation.hh"
#include <iomanip> //set precision of energy in cout

#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Region.hh"
#include "G4SteppingManager.hh"
#include "G4SystemOfUnits.hh"

//#include "TesteMySession.hh"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include "G4UIsession.hh"

#include "TVector3.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

using namespace std;

//constructor
TesteSteppingAction::TesteSteppingAction():
    G4UserSteppingAction()
{;}

//destructor
TesteSteppingAction::~TesteSteppingAction()
{;}

//user stepping action
void TesteSteppingAction::UserSteppingAction(const G4Step * theStep)
{
  // Suspend a track if it is entering into the Helmholtz

  // check if it is alive
  G4Track * theTrack = theStep->GetTrack();
  if(theTrack->GetTrackStatus()!=fAlive) { return; }

  // get region information
  G4StepPoint * thePrePoint = theStep->GetPreStepPoint();
  G4LogicalVolume * thePreLV = thePrePoint->GetPhysicalVolume()->GetLogicalVolume();
  TesteRegionInformation* thePreRInfo = (TesteRegionInformation*)(thePreLV->GetRegion()->GetUserInformation());
  G4StepPoint * thePostPoint = theStep->GetPostStepPoint();
  G4LogicalVolume * thePostLV = thePostPoint->GetPhysicalVolume()->GetLogicalVolume();
  TesteRegionInformation* thePostRInfo = (TesteRegionInformation*)(thePostLV->GetRegion()->GetUserInformation());
  
  /*
  //open a file for energy loss
  TFile *fEnLoss = new TFile("energyLoss.root");
  //create TTree for energy loss
  TTree *fEnLossTree = new TTree("EnergyLoss","Tree that contains initial and final energy of muon in the THGEM");
  //create branches for initial and final muon energy
  fEnLossTree->Branch("enLoss_branch", &energyLoss, "Ei/D:Ef/D");*/
  
  //open file for energy loss
  //ofstream ELfile;
  //ELfile.open("energyLoss.txt", fstream::app);
  
  G4ThreeVector iMomentum;
  G4ThreeVector fMomentum;
  double ipx;
  double ipy;
  double ipz;
  double fpx;
  double fpy;
  double fpz;
  double iModule;
  double fModule;
  double iEnergy;
  double fEnergy;
  
  
  iEnergy = theTrack->GetKineticEnergy();
  fEnergy = theTrack->GetKineticEnergy();
  // check if it is entering the THGEM volume
  if(!(thePreRInfo->IsTHGEM()) && (thePostRInfo->IsTHGEM()) && (iEnergy > 10.))
  {
    iEnergy = theTrack->GetKineticEnergy();
    G4cout << "Muon entering the THGEM with " << iEnergy << "MeV." << G4endl;
    
    //ELfile << iEnergy/MeV << "\t";
    //ELfile << std::setprecision(8) << iEnergy/MeV << "\t";
  }
  // check if it is leaving the THGEM volume
  if((thePreRInfo->IsTHGEM()) && !(thePostRInfo->IsTHGEM()) && (fEnergy > 10.))
  {
    /*TFile *myFile = TFile::Open("particles.root");
    TTree* myTree = (TTree*) myFile->Get("Particles");
    TBranch* electron_branch = myTree->GetBranch("electron_branch");
    int n_events = myTree->GetEntries(); //get number of ionizations
    fEnergy = theTrack->GetKineticEnergy();
    G4cout << "There were " << n_events << " ionizations.\n";
    G4cout << "Muon leaving the THGEM with " << fEnergy << "MeV." << G4endl;
    ELfile << std::setprecision(8) << fEnergy/MeV;
    ELfile << "\t" << n_events << G4endl;*/
    
    fEnergy = theTrack->GetKineticEnergy();
    G4cout << "Muon leaving the THGEM with " << fEnergy << "MeV." << G4endl;
    //ELfile << std::setprecision(8) << fEnergy/MeV << endl;
    //close energy loss file
    //ELfile.close();
    
  }
  
  
  
}
