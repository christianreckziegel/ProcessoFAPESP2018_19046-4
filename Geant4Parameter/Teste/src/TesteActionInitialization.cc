//INITIALIZATION
//Action Initialization mais simples
//G4VUserActionInitialization should include at least one mandatory user action class G4VUserPrimaryGeneratorAction

//Within the G4UserActionInitialization class ( User Action Initialization), the user must instantiate and register a concrete G4VUserPrimaryGeneratorAction subclass, which generates the primary particles for each event

#include "TesteActionInitialization.hh"
#include "G4VUserActionInitialization.hh"
#include "TestePrimaryGeneratorAction.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "TesteRunAction.hh"
#include "TesteEventAction.hh"
#include "TesteSteppingAction.hh"
//#include "TesteTrackingAction.hh"
//#include "TesteStackingAction.hh"


//constructor
TesteActionInitialization::TesteActionInitialization():
G4VUserActionInitialization(),
startEnergy(0.)
{}
//constructor 2
TesteActionInitialization::TesteActionInitialization(double param):
G4VUserActionInitialization(),
startEnergy(param)
{}
//destructor
TesteActionInitialization::~TesteActionInitialization()
{}
//caso de Multi-threading
void TesteActionInitialization::BuildForMaster() const
{
  TesteRunAction* runAction = new TesteRunAction;
  SetUserAction(runAction);
}

//Build() methods should be used for defining user action classes for worker threads as well as for the sequential mode
void TesteActionInitialization::Build() const
{
  //All user action classes must be defined through the protected method SetUserAction()
  //G4VUserPrimaryGeneratorAction* meuPrimaryGeneratorAction = new TestePrimaryGeneratorAction;
  //SetUserAction(meuPrimaryGeneratorAction);
  SetUserAction(new TestePrimaryGeneratorAction(startEnergy));
  //função já definida em G4VUserActionInitialization
  
  TesteRunAction* runAction = new TesteRunAction;
  SetUserAction(runAction);
  
  TesteEventAction* eventAction = new TesteEventAction();
  SetUserAction(eventAction);
  
  //SetUserAction(new TesteStackingAction);
  
  //SetUserAction(new TesteTrackingAction);
  
  SetUserAction(new TesteSteppingAction);
  
  //OBS:
  //Previously, the user action classes were defined in the main.cc but now we define it the ActionInitialization.cc . 
  //For my simulation i had to migrate PriamaryGeneratorAction, RunAction and EventAction to ActionInitialization and add a line.
  
  
}
