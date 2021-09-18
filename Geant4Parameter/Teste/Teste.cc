
#include "G4UImanager.hh"

//#ifdef G4MULTITHREADED
//#include "G4MTRunManager.hh"
//#else
#include "G4RunManager.hh"
//#endif

#include "G4ScoringManager.hh"
#include "G4UIcommand.hh"
#include "FTFP_BERT.hh"
#include "TestePhysicsList.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"
//construir com G4VUserDetectorConstruction
#include "TesteDetectorConstruction.hh"

#include "TesteSteppingAction.hh"
#include "TesteEventAction.hh"
#include "TesteRunAction.hh"


//construir com G4VUserActionInitialization
//user must instantiate and register a concrete G4VUserPrimaryGeneratorAction subclass, which generates the primary particles for each event
#include "TesteActionInitialization.hh"  
#include "TestePrimaryGeneratorAction.hh"

//argc = argument count
//argv = argument vector
int main(int argc,char** argv)
{
  double startEnergy = atof(argv[1]);
  G4long myseed = 345354;
  // construct the default run manager
  //#ifdef G4MULTITHREADED
  //G4MTRunManager* runManager = new G4MTRunManager;
  //#else
  //G4RunManager* runManager = new G4RunManager;   //controla o fluxo do programa e os loops do evento
  //#endif 
  G4RunManager* runManager = new G4RunManager;
  /* informar para o runManager:
  how the detector should be constructed
  all the particles and all the physics processes to be simulated
  how the primary particle(s) in an event should be produced
  any additional requirements of the simulation.*/
  G4Random::setTheSeed(myseed);
  
  // Activate UI-command base scorer
  G4ScoringManager * scManager = G4ScoringManager::GetScoringManager();
  scManager->SetVerboseLevel(1);
  
  /* set mandatory user initialization classes */
  //descreve detector: geometria, materiais, definição de regiões sensitivas, esquemas de readout das regiões sensitivas
  runManager->SetUserInitialization(new TesteDetectorConstruction);
  
  //descreve partículas usadas, e processos físicos a serem simulados
  runManager->SetUserInitialization(new TestePhysicsList);
  //G4VModularPhysicsList* physicsList = new FTFP_BERT;
  //physicsList->SetVerboseLevel(1);
  //runManager->SetUserInitialization(physicsList);
  //G4VModularPhysicsList* physicsList = new G4EmLivermorePhysics();

  
  
  
  //descreve o primary generator action
  //runManager->SetUserAction(new TestePrimaryGeneratorAction);
  //NA VERDADE: todos os userAction devem ser definidos em ActionInitialization.cc e não em main()
  
  

  //define user action classes chamadas durante a simulação, e uma user action obrigatória que define partículas primárias
  runManager->SetUserInitialization(new TesteActionInitialization(startEnergy));
  //runManager->SetUserInitialization(new TesteActionInitialization(2000.));


  /* set mandatory user action class (opcionais dependendo da sua simulação) 
  na verdade, devem ser definidas em ActionInitialization*/
  //runManager->SetUserAction(new TesteSteppingAction);
  //runManager->SetUserAction(new TesteEventAction());
  //runManager->SetUserAction(new TesteRunAction);
  
  //initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  
  // initialize G4 kernel
  runManager->Initialize();

  // get the pointer to the UI manager and set verbosities
  G4UImanager* UI = G4UImanager::GetUIpointer();
  UI->ApplyCommand("/run/verbose 1");
  UI->ApplyCommand("/event/verbose 1");
  UI->ApplyCommand("/tracking/verbose 1");
  
  //TesteMySession* LoggedSession = new TesteMySession("meuOutput.txt", "meuErrput.txt");
  //UI->SetCoutDestination(LoggedSession);
  //UI->SetCoutDestination(new TesteMySession("meuOutput.txt", "meuErrput.txt")); 
  
  
/*
  //MODO INTERATIVO COM QT
  // Detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }
  // Process macro or start UI session
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }
  else { 
    // interactive mode
    UI->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }
*/

  // start a run
  int numberOfEvent = 100;
  runManager->BeamOn(numberOfEvent);

  // job termination
  //delete LoggedSession;
  delete visManager;
  delete runManager;
  return 0;
}
