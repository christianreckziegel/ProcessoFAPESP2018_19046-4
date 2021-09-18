

#ifndef HelmholtzHit_h
#define HelmholtzHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
//#include "TesteTrackInformation.hh"

#include "G4Types.hh"

class G4AttDef;
class G4AttValue;

/// Helmholtz hit
///
/// It records:
/// - the strip ID
/// - the particle time
/// - the strip logical volume, its position and rotation

class HelmholtzHit : public G4VHit 
//hit is a snapshot of the physical interaction of a track in the sensitive region of a detector
//serve para armazenar informações como: position and time of the step, momentum and energy of the track, energy
// deposition of the step, geometrical information
{
public:
    HelmholtzHit();
    HelmholtzHit(const HelmholtzHit&);
    virtual ~HelmholtzHit();

    // operators
    const HelmholtzHit& operator=(const HelmholtzHit&);
    G4bool operator==(const HelmholtzHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetChamberNb(G4int chamb)      { fChamberNb = chamb; };
    //void SetEdep     (G4double de)      { fEdep = de; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
    void SetMomentumDirection(G4ThreeVector momento) {momentum_direction = momento;};
    void SetParticleName(G4String name) {particleName = name;};
    void SetProcessName(G4String name) {processName = name;};
    
    //energy deposition
    inline void SetEdep(G4double de) { fEdep = de; fEdepByATrack = de; }
    inline void AddEdep(G4double de) { fEdep += de; fEdepByATrack += de; }
    inline G4double GetEdep() { return fEdep; }
    inline G4double GetEdepByATrack() { return fEdepByATrack; }
    inline void ClearEdepByATrack() { fEdepByATrack = 0.; }
    //inline void ClearEdepByATrack() { fEdepByATrack = 0.; fTrackInfo = TesteTrackInformation(); }
    
    //inline void SetTrackInformation(const G4Track* aTrack)
    //{
    //  TesteTrackInformation* anInfo = (TesteTrackInformation*)(aTrack->GetUserInformation());
    //  fTrackInfo = *anInfo;
    //}
    //inline TesteTrackInformation* GetTrackInformation() { return &fTrackInfo; }

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetChamberNb() const   { return fChamberNb; };
    //G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };
    G4ThreeVector GetMomentumDirection(){ return momentum_direction;};
    G4String GetParticleName() {return particleName;};
    G4String GetProcessName() {return processName;};
  
  
  
private:
    //some data members
    G4double fEdepByATrack;
    //TesteTrackInformation fTrackInfo;
    
    G4int         fTrackID;
    G4int         fChamberNb;
    G4double      fEdep;
    G4ThreeVector fPos;
    G4ThreeVector momentum_direction;
    G4String particleName;
    G4String processName;
};


//G4Allocator
//every Hit object is stored in the HitsCollection accessible through G4Event (analysis) or G4SDManager (filtering)
//for each concrete hit class you must also prepare a concrete class derived from G4VHitsCollection
typedef G4THitsCollection<HelmholtzHit> HelmholtzHitsCollection;

extern G4ThreadLocal G4Allocator<HelmholtzHit>* HelmholtzHitAllocator; 	//G4Allocator is a class for fast allocation of
									//objects to the heap through the paging 
									//mechanism

inline void* HelmholtzHit::operator new(size_t)
{
    if (!HelmholtzHitAllocator)
        HelmholtzHitAllocator = new G4Allocator<HelmholtzHit>;
    return (void*)HelmholtzHitAllocator->MallocSingle();
}

inline void HelmholtzHit::operator delete(void*aHit)
{
    HelmholtzHitAllocator->FreeSingle((HelmholtzHit*) aHit);
}



#endif
