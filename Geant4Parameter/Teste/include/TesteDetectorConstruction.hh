#ifndef TesteDetectorConstruction_h  // #ifndef = se TesteDetectorConstruction_h não tiver sido definida, executar código abaixo
#define TesteDetectorConstruction_h 1  //define TesteDetectorConstruction_h como condição verdadeira

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include <vector>
#include "G4VisAttributes.hh"
#include "TesteMagneticField.hh"
//#include "TesteElectricField.hh"
#include "G4FieldManager.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.
//herança: TesteDetectorConstruction é um tipo de G4VUserDetectorConstruction
class TesteDetectorConstruction : public G4VUserDetectorConstruction 
{
  public:
    TesteDetectorConstruction();
    virtual ~TesteDetectorConstruction();
    
    //função é escondida, caso uma classe derivada tenha a mesma função
    virtual G4VPhysicalVolume* Construct(); //definida em TesteDetectorConstruction.cc
    
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    virtual void ConstructSDandField(); //sensitive detector e campo
    
  //protected permite acesso da informação pela classe base, nesse caso G4VUserDetectorConstruction
  protected:
    G4LogicalVolume* fScoringVolume;
    G4LogicalVolume* cilindLog;
    G4LogicalVolume* thgemLog;
    static G4ThreadLocal TesteMagneticField* fMagneticField;
    static G4ThreadLocal G4FieldManager* fFieldMgr;
    //static G4ThreadLocal TesteElectricField* fElectricField;
    //static G4ThreadLocal G4FieldManager* electFieldMgr;
};


#endif

