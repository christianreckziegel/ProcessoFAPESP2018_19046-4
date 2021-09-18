//INITIALIZATION
#include "TesteDetectorConstruction.hh"
#include "TesteMagneticField.hh"
#include "TesteRegionInformation.hh"

#include "G4GlobalMagFieldMessenger.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4ThreeVector.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"

#include "G4VSensitiveDetector.hh"
#include "G4MultiSensitiveDetector.hh"
#include "HelmholtzHit.hh"
#include "HelmholtzSD.hh"
//#include "ElectricFieldHit.hh"
//#include "ElectricFieldSD.hh"

#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4AutoDelete.hh"

#include "G4String.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
//#include "SystemOfUnits.h"
#include "G4ios.hh"
#include "G4Colour.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
//#include "G4BestUnit.hh"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
//WTF
G4ThreadLocal TesteMagneticField* TesteDetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* TesteDetectorConstruction::fFieldMgr = 0;

/*Para criar um volume:
	-sólido: dá forma e dimensões
	-volume lógico: dá propriedades e características físicas, como, materiais, elementos sensitiveis a detectores, campo
	 magnético
	-volume físico: dá a posição do volume
*/

//WTF 1: construtor
TesteDetectorConstruction::TesteDetectorConstruction():
  G4VUserDetectorConstruction(),
  fScoringVolume(0),
  cilindLog(0),
  thgemLog(0)
{}


//WTF 2: destrutor
TesteDetectorConstruction::~TesteDetectorConstruction()
{}



//implementação da classe TesteDetectorConstruction
//retorna um ponteiro para um objeto do tipo G4VPhysicalVolume
G4VPhysicalVolume* TesteDetectorConstruction::Construct(){
	//World box, que contém todos os outros volumes
	G4double world_hx = 6.0*m; //-2 < hx < +2
	G4double world_hy = 6.0*m; //-1.0 < hy < +1.0
	G4double world_hz = 6.0*m; //-0.5 < hz < +0.5
	G4bool fCheckOverlaps = true;
	G4Box* caixaMundo = new G4Box("WorldBox", world_hx, world_hy, world_hz);
	/*
	//definindo uma mistura de gas fracional: ar
	G4double z1, a1, fractionmass1, density1;
	G4String name1, symbol1;
	G4int ncomponents1;

	a1 = 14.01*g/mole;
	G4Element* elN  = new G4Element(name="Nitrogen",symbol="N" , z1= 7., a1);

	a1 = 16.00*g/mole;
	G4Element* elO  = new G4Element(name="Oxygen"  ,symbol="O" , z1= 8., a1);

	density1 = 1.290*mg/cm3;
	G4Material* Air = new G4Material(name="Air  ",density1,ncomponents1=2);
	Air->AddElement(elN, fractionmass1=70*perCent);
	Air->AddElement(elO, fractionmass1=30*perCent);
	*/

	//definindo uma mistura de gas fracional: Ar (70%) + CO2 (30%)
	G4double z, a, fractionmass, density;
	G4String name, symbol;
	G4int ncomponents, natoms;
	//Argon
	a = 39.95*g/mole;
	G4Element* elAr = new G4Element(name="Argon",symbol="Ar" , z=18., a);
	//Argon gas
	density= 1.784*g/L;
	G4Material* ArGas = new G4Material("ArgonGas",z=18.,a,density);
	//Carbon
	a = 12.00*g/mole;
	G4Element* elC =new G4Element(name="Carbon",symbol="C" , z=6., a);
	//Oxygen
	a = 16.00*g/mole;
	G4Element* elO  = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a);
	//Hydrogen
	a = 1.00*g/mole;
	G4Element* elH = new G4Element(name="Hydrogen"  ,symbol="H" , z= 1., a);
	//Fluorine
	a = 18.99*g/mole;
	G4Element* elF = new G4Element(name="Fluorine"  ,symbol="F" , z= 9., a);
	//Xenon
	a = 131.29*g/mole;
	G4Element* Xe = new G4Element(name="Xenon"  ,symbol="Xe" , z= 54., a);
	//Xenon gas
	density= 5.851*g/L;
	G4Material* XeGas = new G4Material("ArgonGas",z=54.,a,density);
	//Neon
	a = 20.17*g/mole;
	G4Element* Ne = new G4Element(name="Neon"  ,symbol="Ne" , z= 10., a);
	//Neon gas
	density= 0.9002*g/L;
	G4Material* NeGas = new G4Material("ArgonGas",z=10.,a,density);
	
	//CO2
	density = 1.976*g/L;
	G4Material* CO2 = new G4Material(name="CO2", density,ncomponents=2);
	CO2->AddElement(elC, natoms=1);
	CO2->AddElement(elO, natoms=2);
	//CH4
	density = 0.717*g/L;
	G4Material* CH4 = new G4Material(name="CH4", density,ncomponents=2);
	CH4->AddElement(elC, natoms=1);
	CH4->AddElement(elH, natoms=4);
	//CF4
	density = 3.72*g/L;
	G4Material* CF4 = new G4Material(name="CF4", density,ncomponents=2);
	CF4->AddElement(elC, natoms=1);
	CF4->AddElement(elF, natoms=4);
	
	//mistura ArGas + CO2
	double porcPrinc = 70*perCent;
	double porcAux = 30*perCent;
	density = porcPrinc*1.784*g/L + porcAux*1.976*g/L;
	G4Material* ArMistCo2 = new G4Material("Mistura Ar + CO2", density,ncomponents=2);
	ArMistCo2->AddMaterial(CO2,fractionmass=porcAux);
	ArMistCo2->AddMaterial(ArGas,fractionmass=porcPrinc);
	
	/*
	//mistura ArGas + CH4
	double porcPrinc = 70*perCent;
	double porcAux = 30*perCent;
	density = porcPrinc*1.784*g/L + porcAux*0.717*g/L;
	G4Material* ArMistCo2 = new G4Material("Mistura Ar + CH4", density,ncomponents=2);
	ArMistCo2->AddMaterial(CH4,fractionmass=porcAux);
	ArMistCo2->AddMaterial(ArGas,fractionmass=porcPrinc);
	*/
	/*
	//mistura ArGas + CF4
	double porcPrinc = 70*perCent;
	double porcAux = 30*perCent;
	density = porcPrinc*1.784*g/L + porcAux*3.72*g/L;
	G4Material* ArMistCo2 = new G4Material("Mistura Ar + CF4", density,ncomponents=2);
	ArMistCo2->AddMaterial(CF4,fractionmass=porcAux);
	ArMistCo2->AddMaterial(ArGas,fractionmass=porcPrinc);
	*/
	/*
	//mistura XeGas + CO2
	double porcPrinc = 90*perCent;
	double porcAux = 10*perCent;
	density = porcPrinc*5.851*g/L + porcAux*1.976*g/L;
	G4Material* ArMistCo2 = new G4Material("Mistura Xe + CO2", density,ncomponents=2);
	ArMistCo2->AddMaterial(CO2,fractionmass=porcAux);
	ArMistCo2->AddMaterial(XeGas,fractionmass=porcPrinc);
	*/
	/*
	//mistura NeGas + CO2
	double porcPrinc = 70*perCent;
	double porcAux = 30*perCent;
	density = porcPrinc*0.9002*g/L + porcAux*1.976*g/L;;
	G4Material* ArMistCo2 = new G4Material("Mistura Ne + CO2", density,ncomponents=2);
	ArMistCo2->AddMaterial(CO2,fractionmass=porcAux);
	ArMistCo2->AddMaterial(NeGas,fractionmass=porcPrinc);
	*/
	//definindo Air from database
	G4NistManager* man = G4NistManager::Instance();
	G4Material* Air  = man->FindOrBuildMaterial("G4_AIR");
	//man = G4NISTManager::GetPointer;
	G4Material* vacuum = man->FindOrBuildMaterial("G4_Galactic");
	
		
	
	//criando um logical volume (mãe)
	G4LogicalVolume* caixaMundoLog = new G4LogicalVolume(caixaMundo, vacuum, "MundoLogico");

	//colocando o physical volume (mãe) (instancia do logical volume no espaço)
	G4VPhysicalVolume* caixaMundoPhys 
	= new G4PVPlacement(0,
	G4ThreeVector(0,0,0),
	caixaMundoLog,
	"VolumeGlobalPhys",
	0,//como já é o logical volume mãe, deve ter um ponteiro para logical volume mãe nulo
	false,
	0,
	fCheckOverlaps); 
	//... = new G4PVPlacement(rotaçao, vetor de posiçao,logical volume, nome do physical volume, logical volume "mãe", operaçoes booleanas, numero de cópia)
	G4Region* defaultRegion = (*(G4RegionStore::GetInstance()))[0];
  	TesteRegionInformation* defaultRInfo = new TesteRegionInformation();
  	defaultRInfo->SetWorld();
  	defaultRInfo->Print();
  	defaultRegion->SetUserInformation(defaultRInfo);
	
	
	
	//Bobina de Hemlholtz
	//world volume filha: cilindro de raio 20cm
	G4double innerRadius = 0.*cm;
   	G4double outerRadius = 100.*cm; //original = 20.*cm
   	G4double hz = outerRadius/2; //metade da altura = outerRadius/2 = 10.*cm
   	G4double startAngle = 0.*deg;
   	G4double spanningAngle = 360.*deg;
	
   	G4Tubs* cilindVol
   	  = new G4Tubs("CilindroVolume",
   	               innerRadius,
   	               outerRadius,
   	               hz,
   	               startAngle,
   	               spanningAngle);
	//logical volume filha
	cilindLog = new G4LogicalVolume(cilindVol, Air, "CilindroLogico"); //definido no .hh
	//physical volume filha
	G4double pos_x = 0.0*meter;
	G4double pos_y = 0.0*meter;
	G4double pos_z = 0.0*meter;

	G4VPhysicalVolume* cilindPhys 
	= new G4PVPlacement(0,                       // no rotation (em relação à mãe)
		      G4ThreeVector(pos_x, pos_y, pos_z),
		                               // translation position
		      cilindLog,              // its logical volume
		      "MeuCilindro",               // its name
		      caixaMundoLog,                // its mother (logical) volume
		      false,                   // no boolean operations
		      0);                      // its copy number
	
	
	
	
	
	
	//exemplo de world volume (filha) metade=15x15x0.35cm3
	G4Box* thgemVol = new G4Box("THGEMVol", 50.*cm, 50.*cm, 0.35*cm);  //original thickness 0.35cm
	//exemplo de logical volume (filha) ArGas or ArMistCo2
	G4LogicalVolume* thgemLog = new G4LogicalVolume(thgemVol, ArGas, "THGEMLogico");
	//colocando o physical volume (filha) (instancia do logical volume no espaço)
	//G4double pos_x = 0.0*meter;
	//G4double pos_y = 0.0*meter;
	//G4double pos_z = 0.0*meter;

	G4VPhysicalVolume* thgemPhys
	= new G4PVPlacement(0,                    // no rotation (em relação à mãe)
		      G4ThreeVector(pos_x, pos_y, pos_z),// translation position
		      thgemLog,                   // its logical volume
		      "MinhaTHGEM",               // its name
		      cilindLog,              // its mother (logical) volume ->caixaMundoLog ?
		      false,                      // no boolean operations
		      0,			  // its copy number
		      fCheckOverlaps);     	  //overlaps checking
	
	
	G4Region* thgemRegion = new G4Region("THGEMRegion");
  	TesteRegionInformation* thgemInfo = new TesteRegionInformation();
        thgemInfo->SetTHGEM();
        thgemRegion->SetUserInformation(thgemInfo);
        thgemLog->SetRegion(thgemRegion);
        thgemRegion->AddRootLogicalVolume(thgemLog);
	
	
	//WTF 3
	fScoringVolume = thgemLog;
	
	
	
	//atributos de visualização
	//volume mãe
	G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	visAttributes->SetVisibility(false);
	caixaMundoLog->SetVisAttributes(visAttributes);
	
	
	//cilindro com aros roxos
	G4VisAttributes* visAttributesHelmholtz = new G4VisAttributes(G4Colour(1.,0.,0.7));   // LightGray 0.9,0.9,0.9
	//visAttributes->SetVisibility(true);			      // purple 1.0, 0.0, 0.7
	visAttributesHelmholtz->SetForceWireframe(true);
    	cilindLog->SetVisAttributes(visAttributesHelmholtz);
    	
    	
    	//thgem com aros amarelos
	//visAttributesTHGEM = new G4VisAttributes(G4Colour(0.9,0.9,0.9));   // LightGray 0.9,0.9,0.9
	//visAttributes->SetVisibility(true);			      // purple 1.0, 0.0, 0.7
    	G4VisAttributes* visAttributesTHGEM = new G4VisAttributes(G4Colour(1.,0.5,0.));
    	visAttributesTHGEM->SetForceWireframe(true);
	visAttributesTHGEM->SetVisibility(true);
	thgemLog->SetVisAttributes(visAttributesTHGEM);
    	
    	
    	
	
	
	// sensitive detectors -----------------------------------------------------
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	G4String SDname;
	HelmholtzSD* thgemSD = new HelmholtzSD(SDname="/Helmholtz","hitsCollection");
	SDman->AddNewDetector(thgemSD);
	thgemLog->SetSensitiveDetector(thgemSD);
	
	//cria o campo (opção 3)
	fMagneticField = new TesteMagneticField(); //constructor tem como padrão fBz 0.5e-4*tesla
	G4double Bmodule = fMagneticField->GetField();
	G4cout << "Magnetic field module: " << Bmodule/tesla << " Tesla" << G4endl;
	/*
	//salvando a intensidade do campo no arquivo "meuOutput.txt"
	ofstream myfile;
        myfile.open("meuOutput.txt", fstream::app);
	myfile << Bmodule/tesla << G4endl;
        myfile.close();
	*/
	fFieldMgr = new G4FieldManager();
	fFieldMgr->SetDetectorField(fMagneticField);
	fFieldMgr->CreateChordFinder(fMagneticField);
	fFieldMgr->GetChordFinder()->SetDeltaChord(0.000001); //miss distance
	//fFieldMgr->SetDeltaIntersection(0.001);		   //delta intersection
	//fFieldMgr->SetDeltaOneStep(0.001);		   //delta one step
	thgemLog->SetFieldManager(fFieldMgr, true);
	
	G4AutoDelete::Register(fMagneticField);
	G4AutoDelete::Register(fFieldMgr);
	
	
	
	
	
	//retorna o physical volume mãe
	return caixaMundoPhys; 
}

void TesteDetectorConstruction::ConstructSDandField()
{
/*
	// sensitive detectors -----------------------------------------------------
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	G4String SDname;

	HelmholtzSD* thgemSD = new HelmholtzSD(SDname="/Helmholtz","hitsCollection");
	SDman->AddNewDetector(thgemSD);
	cilindLog->SetSensitiveDetector(thgemSD);
*/	
	
	//SDname = "/ElectricField";
	//G4VSensitiveDetector* ElectricField = new ElectricFieldSD(SDname="ElectricField");
	//SDman->AddNewDetector(ElectricField);
	//thgemLog->SetSensitiveDetector(Helmholtz);
	
	
	//campo magnetico dentro do cilindro
	//cria o campo (opção 1)
	/*
	G4ThreeVector fieldValue = G4ThreeVector(0.,0.,0.5*mT);
	fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
	fMagFieldMessenger->SetVerboseLevel(1);
	*/
	/*
	//cria o campo (opção 2)
	G4UniformMagField* magField = new G4UniformMagField(G4ThreeVector(0.*tesla,0.*tesla,0.5e-4*tesla)); //no sentido e direção do eixo z 
	//cria field manager e define como campo padrão
	G4FieldManager* fieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
	fieldMgr->SetDetectorField(magField);
	//cria objetos para calcular a tragetória
	fieldMgr->CreateChordFinder(magField);
	//muda a precisão do volume de intersecção
	//fieldMgr->GetChordFinder()->SetDeltaChord(1.e-5);
	//seleciona volume lógico com o campo
	thgemLog->SetFieldManager(fieldMgr, true);
	//registra o campo e o manager para serem deletados automaticamente
	G4AutoDelete::Register(magField);
	G4AutoDelete::Register(fieldMgr);
	*/
/*	
	//cria o campo (opção 3)
	fMagneticField = new TesteMagneticField(); //constructor tem como padrão fBz 0.5e-4*tesla
	G4double Bmodule = fMagneticField->GetField();
	G4cout << "THAT'S THE MAGNETIC FIELD MODULE: " << Bmodule/tesla << " tesla" << G4endl;
	//salvando a intensidade do campo no arquivo "meuOutput.txt"
	ofstream myfile;
        myfile.open("meuOutput.txt", fstream::app);
	myfile << Bmodule/tesla << G4endl;
        myfile.close();
	
	fFieldMgr = new G4FieldManager();
	fFieldMgr->SetDetectorField(fMagneticField);
	fFieldMgr->CreateChordFinder(fMagneticField);
	
	//fElectricField =new TesteElectricField();
	//fFieldMgr->SetDetectorField(fElectricField);
	//fFieldMgr->CreateChordFinder(fElectricField);
	
	fFieldMgr->GetChordFinder()->SetDeltaChord(0.000001); //miss distance
	//fFieldMgr->SetDeltaIntersection(0.001);		   //delta intersection
	//fFieldMgr->SetDeltaOneStep(0.001);		   //delta one step
	cilindLog->SetFieldManager(fFieldMgr, true);
*/	
	
	
	//campo elétrico dentro do cilindro
	/*
	fElectricField =new TesteElectricField(); //constructor tem como padrão fEz 10.0*kilovolt/cm
	electFieldMgr = new G4FieldManager();
	electFieldMgr->SetDetectorField(fElectricField);
	electFieldMgr->CreateChordFinder(fElectricField);
	electFieldMgr->GetChordFinder()->SetDeltaChord(1.e-2);
	thgemLog->SetFieldManager(electFieldMgr, true)
	
	G4AutoDelete::Register(fElectricField);
	G4AutoDelete::Register(electFieldMgr);
	*/
	
	//G4AutoDelete::Register(fElectricField);
	//G4AutoDelete::Register(fMagneticField);
	//G4AutoDelete::Register(fFieldMgr);
	

}



