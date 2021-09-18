////G4VUserActionInitialization should include at least one mandatory user action class G4VUserPrimaryGeneratorAction

#ifndef TesteActionInitialization_h
#define TesteActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "TestePrimaryGeneratorAction.hh"


/// Action initialization class.

class TesteActionInitialization : public G4VUserActionInitialization
{
  public:
    TesteActionInitialization();
    TesteActionInitialization(double param);
    virtual ~TesteActionInitialization();

    //virtual void BuildForMaster() const;
    virtual void Build() const;
    virtual void BuildForMaster() const;
    //protected:
    //void SetUserAction(G4VUserPrimaryGeneratorAction*) const;
    private:
    double startEnergy;
};



#endif
