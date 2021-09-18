//ACTION CLASS
#include "TestePrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"
#include "G4GenericMessenger.hh"
#include "G4Event.hh"

//construtor
//chamado só uma vez
TestePrimaryGeneratorAction::TestePrimaryGeneratorAction(     	//todos esses parâmetros estão definidos
                                const G4String& particleName,   //em TestePrimaryGeneratorAction.hh
                                G4double energy,
                                G4ThreeVector position,
                                G4ThreeVector momentumDirection):
    G4VUserPrimaryGeneratorAction(), //instantiate the primary generator
    fParticleGun(0)
{
  G4int nofParticles = 1;
  fParticleGun  = new G4ParticleGun(nofParticles);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("mu-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(2000.*MeV); //
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*m,1.*m,0.0005*m)); //original (0.*m,1.*m,0.*m) 0.05cm de altura em z
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.)); //G4ThreeVector(0.1/0.9055,-0.9/0.9055,0.)
}

//constructor 2
TestePrimaryGeneratorAction::TestePrimaryGeneratorAction(double startEnergy):
    G4VUserPrimaryGeneratorAction(), //instantiate the primary generator
    fParticleGun(0)
{
  G4int nofParticles = 1;
  fParticleGun  = new G4ParticleGun(nofParticles);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("mu-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(startEnergy*MeV); //variando de 100 em 100 MeV
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*m,1.*m,0.0005*m)); //original (0.*m,1.*m,0.*m)
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.)); //G4ThreeVector(0.1/0.9055,-0.9/0.9055,0.)
  
  //std::cout << "CONSTRUTOR 2 CHAMADO\n";
  //std::cout << "A energia inicial (argumento) foi de " << startEnergy << "*MeV\n";
}

//destrutor
TestePrimaryGeneratorAction::~TestePrimaryGeneratorAction()
{
  //primary generator object(s) you construct in your G4VUserPrimaryGeneratorAction concrete class MUST be deleted in your destructor
  delete fParticleGun;
}


//função geradora de eventos primários
//chamado no início de todo evento
void TestePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  
  // this function is called at the beginning of event
  //fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
  //fParticleGun->SetParticleMomentum(G4ThreeVector(0.,-10.,0.*MeV));
  //fParticleGun->SetParticleMomentum(G4ThreeVector(G4UniformRand(),G4UniformRand(),G4UniformRand()));
  //sigmaAngle = 45 graus = 0.785398 radianos
  //sigmaAngle = 90 graus = 1.5708 radianos
  //sigmaAngle = 5 graus = 0.087266 radianos
  //sigmaAngle = 1 grau = 0.0174533 radianos
  G4double angle = (G4UniformRand()-0.5)*0.0174533;	//(G4UniformRand()-0.5)*sigmaAngle
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(sin(angle),-cos(angle),0.));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.));	
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
