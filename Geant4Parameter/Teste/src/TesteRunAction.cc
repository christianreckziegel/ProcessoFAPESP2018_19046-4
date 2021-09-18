

#include "TesteRunAction.hh"
#include "TesteRootAnalysis.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"


//constructor
TesteRunAction::TesteRunAction():
  G4UserRunAction()
{;}

//destructor
TesteRunAction::~TesteRunAction()
{}


void TesteRunAction::BeginOfRunAction(const G4Run* aRun)
{
  //inform the runManager to save random number seed
  
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
}


void TesteRunAction::EndOfRunAction(const G4Run*)
{
  //G4RunManager* fRM = G4RunManager::GetRunManager();
  //const G4Event* currentEvent = fRM->GetCurrentEvent();
  //G4int eventID = currentEvent->GetEventID();
  //G4cout << G4endl << G4endl << G4endl << "THE CURRENT EVENT ID IS " << eventID << G4endl << G4endl << G4endl << G4endl;
  //G4int RunID = endRun->GetRunID();
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // Write and close the output file
  analysisManager->Write();
  analysisManager->CloseFile();
  
  TesteRootAnalysis* rootAnalysis = TesteRootAnalysis::Instance();
  rootAnalysis->Close();
  
}

