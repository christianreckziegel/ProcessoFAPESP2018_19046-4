
#include "TesteRootAnalysis.hh"
#include "HelmholtzSD.hh"
#include "HelmholtzHit.hh"
#include "G4SDManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"

#include "TesteRegionInformation.hh"
#include <cstdio>
#include <iostream>
#include <fstream>

using namespace std;

//constructor
HelmholtzSD::HelmholtzSD(const G4String& name,const G4String& hitsCollectionName):
   G4VSensitiveDetector(name),
   fHelCollection(0)
{
  collectionName.insert(hitsCollectionName);
}

//destructor
HelmholtzSD::~HelmholtzSD()
{;}

//Initialize() chamado no inicio de cada evento
void HelmholtzSD::Initialize(G4HCofThisEvent* HCE)
{
  //create this collection
  fHelCollection = new HelmholtzHitsCollection(SensitiveDetectorName, collectionName[0]); 
  
  // Add this collection in hce
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  HCE->AddHitsCollection( HCID, fHelCollection );
  
}
G4int number_ioniz = 0;
G4int eventNum = 1;

//ProcessHits chamado para cada step do volume sensitivo
//ProcessHits() chamado por steppingmanager, quando anda um step no logical volume que tem um ponteiro para este sensitive detector
//um ou mais objetos G4VHit serão construidos, se este step for importante para o detector
G4bool HelmholtzSD::ProcessHits(G4Step*aStep,G4TouchableHistory*)
{
  // get region information
  G4StepPoint * prePoint = aStep->GetPreStepPoint();
  G4LogicalVolume * preLV = prePoint->GetPhysicalVolume()->GetLogicalVolume();
  TesteRegionInformation* preRInfo = (TesteRegionInformation*)(preLV->GetRegion()->GetUserInformation());
  G4StepPoint * postPoint = aStep->GetPostStepPoint();
  G4LogicalVolume * postLV = postPoint->GetPhysicalVolume()->GetLogicalVolume();
  TesteRegionInformation* postRInfo = (TesteRegionInformation*)(postLV->GetRegion()->GetUserInformation());
  
  
  //getting step muon's energy lost
  G4double energyStepMuon = prePoint->GetTotalEnergy()-postPoint->GetTotalEnergy();
  
  G4Track* track = aStep->GetTrack();
  double Energy = track->GetKineticEnergy();
  const G4ParticleDefinition* particle = (track->GetParticleDefinition());
  const G4String particleName = particle->GetParticleName();
  //Geting process name that the particle underwent
  //G4String processName = ((aStep->GetPostStepPoint())->GetProcessDefinedStep())->GetProcessName();
  G4ThreeVector momentum = track->GetMomentumDirection();
  double px = momentum.x();
  double py = momentum.y();
  double pz = momentum.z();
  double mModule = sqrt( pow(px,2) + pow(py,2) + pow(pz,2) );
  double angx = acos(px/mModule);
  double angy = acos(py/mModule);
  double angz = acos(pz/mModule);
  
  // get mean number of ions along step
  //G4double ionMean;
  // energy deposit (of ionization step?)
  G4double edep;
  // energy deposit (of hit of interaction)
  G4double edep1 = aStep->GetTotalEnergyDeposit();
  if(edep1 == 0) { return false; }
  
  //getting secondaries (electrons) information
  const std::vector<const G4Track*>* secondary = aStep->GetSecondaryInCurrentStep();
  
  int number_secondaries = (*secondary).size();
  
  if (particleName=="mu-" && number_secondaries!=0) {
        //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        G4double kinEnergy = 0;
        G4ThreeVector secondaryPosition;
        G4ThreeVector momentumDirection;
        for (int i=0; i<number_secondaries; i++) {
            const G4ParticleDefinition* secondaryDefinition = (*secondary)[i]->GetParticleDefinition();
            G4String secondaryName = secondaryDefinition->GetParticleName();
            
            if (secondaryName == "e-") {
            	
                //Kinectic energy of the secondary
                kinEnergy = (*secondary)[i]->GetKineticEnergy();
                secondaryPosition = (*secondary)[i]->GetPosition();
                momentumDirection = (*secondary)[i]->GetMomentumDirection();
		//ionization energy deposit (of step?)
		//edep = energyStepMuon - aStep->GetNonIonizingEnergyDeposit() - kinEnergy; 
		//deve ser igual à expressao abaixo
                edep = aStep->GetTotalEnergyDeposit() - aStep->GetNonIonizingEnergyDeposit();
                
                G4cout << std::setw(10) << G4BestUnit(kinEnergy,"Energy") << G4endl;

                TesteRootAnalysis* rootAnalysis = TesteRootAnalysis::Instance();
                rootAnalysis->Write(kinEnergy, secondaryPosition, momentumDirection, eventNum);
                rootAnalysis->Write2(edep);
                number_ioniz = number_ioniz +1;
                G4cout << "There were " << number_ioniz << " ionizations in the event number " << eventNum << ".\n";
            }
        }
  }
  
  
  // check if it is leaving the THGEM volume
  if((preRInfo->IsTHGEM()) && !(postRInfo->IsTHGEM()))
  {
    
    G4cout << "Leaving the THGEM -> there were " << number_ioniz << " ionization points." << G4endl;
    //G4cout << "Saving data into file numberIoniz.txt ...\n";
    
    if(number_ioniz > 0){
      //open file for energy loss
      //ofstream ELfile;
      //ELfile.open("ionization.txt", fstream::app);
      //ELfile << number_ioniz << G4endl;
      //close energy loss file
      //ELfile.close();
    }
    //open file for energy loss
    /*ofstream ELfile;
    ELfile.open("ionization.txt", fstream::app);
    ELfile << number_ioniz << G4endl;
    //close energy loss file
    ELfile.close();*/
    
    G4cout << "Reseting the number of ionizations and adding +1 to the event number...\n";
    number_ioniz = 0;
    G4cout << "END OF EVENT " << eventNum << G4endl;
    eventNum = eventNum + 1;
    
  }
  
  
  
  //Storing the hit (interation) information of the current step
  HelmholtzHit* newHit = new HelmholtzHit();
  newHit->SetTrackID( aStep->GetTrack()->GetTrackID() );
  newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
  newHit->SetEdep(edep1);
  newHit->SetPos( aStep->GetPostStepPoint()->GetPosition() );
  newHit->SetParticleName(particleName);
  //newHit->SetProcessName(processName);
  fHelCollection->insert( newHit );
  //newHit->Print();
  
  
  
  
  
  return true;
}



//end of event
void HelmholtzSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) { 
     G4int nofHits = fHelCollection->entries();
     /*G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits 
            << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHelCollection)[i]->Print();*/
  }
}

