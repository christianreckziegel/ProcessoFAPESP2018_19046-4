

#ifndef HelmholtzSD_h
#define HelmholtzSD_h 1

#include "G4VSensitiveDetector.hh"
#include "HelmholtzHit.hh"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;


class HelmholtzSD : public G4VSensitiveDetector
{

public:
  HelmholtzSD(const G4String& name, const G4String& hitsCollectionName);
  virtual ~HelmholtzSD();
  
  virtual void Initialize(G4HCofThisEvent*HCE);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);
  
private:
  HelmholtzHitsCollection *fHelCollection;
  
};

#endif

