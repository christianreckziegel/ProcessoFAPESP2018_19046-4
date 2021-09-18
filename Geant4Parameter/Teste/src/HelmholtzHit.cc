

#include "HelmholtzHit.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4ios.hh"


G4ThreadLocal G4Allocator<HelmholtzHit> * HelmholtzHitAllocator = 0;

//1st constructor
HelmholtzHit::HelmholtzHit():
   G4VHit(),
   fTrackID(-1),
   fChamberNb(-1),
   fEdep(0.), 
   fPos(G4ThreeVector()),
   fEdepByATrack(0.),
   momentum_direction(G4ThreeVector()),
   particleName(""),
   processName("")
   //fTrackInfo()
{;}

//destructor
HelmholtzHit::~HelmholtzHit()
{;}

//2nd constructor
HelmholtzHit::HelmholtzHit(const HelmholtzHit& right):
   G4VHit()
{
  fTrackID   = right.fTrackID;
  fChamberNb = right.fChamberNb;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
  fEdepByATrack = right.fEdepByATrack;
  //fTrackInfo = right.fTrackInfo;
  momentum_direction = right.momentum_direction;
  particleName = right.particleName;
  processName = right.processName;
  
}

G4bool HelmholtzHit::operator==(const HelmholtzHit& right) const
{
  return ( this == &right ) ? true : false;
}


//draw NECESSARIO VERIFICAR
void HelmholtzHit::Draw()
{
  /*
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Transform3D trans(fRot,fPos);
    G4VisAttributes attribs;
    const G4VisAttributes* pVA = fPLogV->GetVisAttributes();
    if(pVA) attribs = *pVA;
    G4Colour colour(1.,0.,0.);
    attribs.SetColour(colour);
    attribs.SetForceWireframe(false);
    attribs.SetForceSolid(true);
    pVVisManager->Draw(*fPLogV,attribs,trans);
  }
  */
  
  
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fPos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
  
  
}



//Print Hits information
void HelmholtzHit::Print()
{
  /*G4cout
     << "  trackID: " << fTrackID << " chamberNb: " << fChamberNb
     << "Edep: "
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << "fEdepByATrack: "
     << std::setw(7) << G4BestUnit(fEdepByATrack,"Energy")
     << " Position: "
     << std::setw(7) << G4BestUnit( fPos,"Length")
     << G4endl;*/
}



