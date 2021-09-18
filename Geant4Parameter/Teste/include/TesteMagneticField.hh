



#ifndef TesteMagneticField_H
#define TesteMagneticField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"

class G4GenericMessenger;

/// Magnetic field

class TesteMagneticField : public G4MagneticField
{
  public:
    TesteMagneticField();
    virtual ~TesteMagneticField();
    
    virtual void GetFieldValue(const G4double point[4],double* bField ) const;
    
    void SetField(G4double val) { fBz = val; }
    G4double GetField() const { return fBz; }
    
  private:
    void DefineCommands();

    G4GenericMessenger* fMessenger;
    G4double fBz;
};


#endif
