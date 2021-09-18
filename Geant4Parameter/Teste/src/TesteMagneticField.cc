


#include "TesteMagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//constructor
TesteMagneticField::TesteMagneticField():
  G4MagneticField(), 
  fMessenger(nullptr), fBz(0.5*tesla) //0.5e-4*tesla = 0.00005*tesla
{
  // define commands for this class
  DefineCommands();
}

//destructor

TesteMagneticField::~TesteMagneticField()
{ 
  delete fMessenger; 
}

void TesteMagneticField::GetFieldValue(const G4double [4],double *bField) const
{
  bField[0] = 0.;
  bField[1] = 0.;
  bField[2] = fBz;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TesteMagneticField::DefineCommands()
{
  // Define /Teste/field command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this, 
                                      "/Teste/magneticField", 
                                      "Field control");

  // fieldValue command 
  auto& valueCmd
    = fMessenger->DeclareMethodWithUnit("value","tesla",
                                &TesteMagneticField::SetField, 
                                "Set field strength.");
  valueCmd.SetParameterName("magneticField", true);
  valueCmd.SetDefaultValue("0.5");
}
