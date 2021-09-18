#ifndef TestePrimaryGeneratorAction_h
#define TestePrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

class G4ParticleGun;
class G4Event;

class TestePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction 
//G4VUserPrimaryGeneratorAction concrete class just arranges the way primary particles are generated
{
  public:
    TestePrimaryGeneratorAction(
      const G4String& particleName = "geantino",
      G4double energy = 1.*MeV,
      G4ThreeVector position= G4ThreeVector(0,0,0),
      G4ThreeVector momentumDirection = G4ThreeVector(0,0,1));
    TestePrimaryGeneratorAction(double startEnergy);
    ~TestePrimaryGeneratorAction();

    // methods
    virtual void GeneratePrimaries(G4Event*);

  private:
    // data members
    G4ParticleGun*  fParticleGun; //pointer a to G4 service class
    //actual primary particle generator
};

#endif
