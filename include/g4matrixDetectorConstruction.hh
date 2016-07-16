//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef g4matrixDetectorConstruction_h
#define g4matrixDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class g4matrixDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    g4matrixDetectorConstruction();
    virtual ~g4matrixDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    
    void SetCrystalDimensions(G4double x, G4double y, G4double z) { fCrystal_x = x *mm ; fCrystal_y = y*mm ; fCrystal_z =z*mm; };
    void SetNumberOfCrystals(G4int nx, G4int ny) {nCrystalsX = nx; nCrystalsY = ny;};
    void SetThinAirThickness(G4double x) { airThinThickness = x*mm; };
    void SetLateralEsr(G4bool x) {lateralEsr = x;};
    void SetBackEsr(G4bool x) {backEsr = x;};
    void SetGreaseFrontOne(G4double x) { fGreaseFrontCryToGlass_z = x*mm; };
    void SetGreaseFrontTwo(G4double x) { fGreaseFrontGlassToMPPC_z = x*mm; };
    void SetGlassFront(G4double x) { fGlassFront_z = x*mm; };
    void SetEpoxy(G4double x) { fEpoxy_z = x*mm; };
    void SetMppcDimensions(G4double x, G4double y, G4double z) { fSingleMPPC_x = x *mm ; fSingleMPPC_y = y*mm ; fSingleMPPC_z =z*mm; };
    void SetMppcGap(G4double x) {mppcGap = x*mm;}
    void SetNumberOfMPPC(G4int nx, G4int ny) {nDetectorsX = nx; nDetectorsY = ny;}
    void SetGreaseBack(G4double x) { fGreaseBackCryToGlass_z = x*mm; };
    void SetGlassBack(G4double x) { fGlassBack_z = x*mm; };
    void SetAirBack(G4double x) { fAirBack_z = x*mm; };
    void SetLightYield(G4double x) { lightyield = x; };
    void SetRiseTime(G4double x) { risetime = x; };
    void SetDecayTime(G4double x) { decaytime = x; };
    
    void SetLateralDepolished(G4bool x) { latdepolished = x; };
    void SetLateralSurfaceSigmaAlpha(G4double x){latsigmaalpha = x;};
    void SetLateralSurfaceSpecularLobe(G4double x){latspecularlobe = x;};
    void SetLateralSurfaceSpecularSpike(G4double x){latspecularspike = x;};
    void SetLateralSurfaceBackScattering(G4double x){latbackscattering= x;};
    void SetLateralSurfaceRoughness(G4double x) { latsurfaceroughness = x * m; };
    
    void SetRealDepolished(G4bool x) { realdepolished = x; };
    void SetRealSurfaceSigmaAlpha(G4double x){realsigmaalpha = x;};
    void SetRealSurfaceSpecularLobe(G4double x){realspecularlobe = x;};
    void SetRealSurfaceSpecularSpike(G4double x){realspecularspike = x;};
    void SetRealSurfaceBackScattering(G4double x){realbackscattering= x;};
    void SetRealSurfaceRoughness(G4double x) { realsurfaceroughness = x * m; };
    
    void SetResolutionScale(G4double x){resolutionScale = x;};
    
    void SetFastEnergy   (std::vector<double> x)   {fastenergy = x;};
    void SetFastComponent(std::vector<double> x){fastcomponent = x;};
    void SetSlowEnergy   (std::vector<double> x)   {slowenergy = x;};
    void SetSlowComponent(std::vector<double> x){slowcomponent = x;};
   
    void SetFastRiseTime (G4double x){fastrisetime = x;};
    void SetFastDecayTime(G4double x){fastdecaytime= x;};
    void SetFastRatio    (G4double x){fastratio    = x;};
    void SetSlowRiseTime (G4double x){slowrisetime = x;};
    void SetSlowDecayTime(G4double x){slowdecaytime= x;};
    
    void SetEsrTransmittance(G4double x) { esrTransmittance = x; };
    void SetSourceDistance(G4double x) { distance = x *mm ; };
    void SetLatDepoSideBySide(G4bool b0, G4bool b1,G4bool b2, G4bool b3 )
    {
        latdepo_sideBySide[0] = b0; 
        latdepo_sideBySide[1] = b1;
        latdepo_sideBySide[2] = b2;
        latdepo_sideBySide[3] = b3;
    };
    
    void SetEsrGapX(G4double x) { esrgapx = x *mm ; };
    void SetEsrGapY(G4double x) { esrgapy = x *mm ; };
    void SetAirGap(G4double x) { airgap = x *mm ; };
    
  private:

    G4double fCrystal_x;				//x dimensions of crystals
    G4double fCrystal_y;				//y dimensions of crystals
    G4double fCrystal_z;				//z dimensions of crystals    
    G4int nCrystalsX;					//number of crystals rows
    G4int nCrystalsY;					//number of crystals columns
    G4int nDetectorsX;					//number of detectors rows
    G4int nDetectorsY;					//number of detectors column
    G4double airThinThickness;				//thickness of air layer around crystals. in reality it would be the vikuiti thickness (see below) 
    G4double mppcGap;					//gap between single mppc detectors
    G4double fAirThinLayerBox_x;			//x dim of air box around crystals. In reality is the dimensions of the vikuiti box (but vikuiti is "confined" to a surface)
    G4double fAirThinLayerBox_y;			//y dim of air box around crystals. In reality is the dimensions of the vikuiti box (but vikuiti is "confined" to a surface)
    G4double fAirThinLayerBox_z;			//x dim of air box around crystals. In reality is the dimensions of the vikuiti box (but vikuiti is "confined" to a surface)
    
    G4double fExpHall_x;				//x dimensions of world
    G4double fExpHall_y;				//y dimensions of world
    G4double fExpHall_z;				//z dimensions of world
    
    G4double fGreaseFrontCryToGlass_x;			//dimensions of grease layer in front (i.e. on the mppc side) between crystals and glass
    G4double fGreaseFrontCryToGlass_y;
    G4double fGreaseFrontCryToGlass_z;
    
    G4double fGlassFront_x;				//dimensions of glass layer in front (i.e. on the mppc side)
    G4double fGlassFront_y;
    G4double fGlassFront_z;
    
    G4double fGreaseFrontGlassToMPPC_x;			//dimensions of grease layer in front (i.e. on the mppc side) between glass and mppc
    G4double fGreaseFrontGlassToMPPC_y;
    G4double fGreaseFrontGlassToMPPC_z;
    
    G4double fEpoxy_x;					//dimensions of epoxy layer on the mppc
    G4double fEpoxy_y;
    G4double fEpoxy_z;
    
    G4double fMPPCArray_x;				//dimensions of the entire mppc array (16 detectors + dead space)
    G4double fMPPCArray_y;
    G4double fMPPCArray_z;
    
    G4double fSingleMPPC_x;				//dimensions of the single mppc
    G4double fSingleMPPC_y;
    G4double fSingleMPPC_z;
    
    G4double fSingleMPPCBlock_x;			//dimensions of single mppc plus half of the gap to the other mppc
    G4double fSingleMPPCBlock_y;
    G4double fSingleMPPCBlock_z;
    
    
    G4double fGreaseBackCryToGlass_x;			//dimensions of grease layer on the back (i.e. opposite to the mppc side) between crystals and glass
    G4double fGreaseBackCryToGlass_y;
    G4double fGreaseBackCryToGlass_z;
    
    G4double fGlassBack_x;				//dimensions of glass layer on the back (i.e. opposite to the mppc side)
    G4double fGlassBack_y;
    G4double fGlassBack_z;
    
    G4double fAirBack_x;				//dimensions of air on the back of back glass (esr is in dry contact with glass)
    G4double fAirBack_y;
    G4double fAirBack_z;
    
    G4double fFakeAirBack_x;				//dimensions of fake air on the back of back glass, useful to properly define the esr reflector there
    G4double fFakeAirBack_y;
    G4double fFakeAirBack_z;
    G4double esrgapx;
    G4double esrgapy;
    G4double airgap;
    
    
    //POSITIONS
    
    G4double matrixShiftX;				//shift required to put the matrix in the center of the world
    G4double matrixShiftY;
    G4double matrixShiftZ;
    
    G4double mppcShiftX;				//shift required to put the mppc array in the center of the world
    G4double mppcShiftY;
    G4double mppcShiftZ;
    
    G4double pGreaseFrontCryToGlass_x;			//positions of grease layer in front (i.e. on the mppc side) between crystals and glass
    G4double pGreaseFrontCryToGlass_y;
    G4double pGreaseFrontCryToGlass_z;
    
    G4double pGlassFront_x;				//positions of glass layer in front (i.e. on the mppc side)
    G4double pGlassFront_y;
    G4double pGlassFront_z;
    
    G4double pGreaseFrontGlassToMPPC_x;			//positions of grease layer in front (i.e. on the mppc side) between glass and mppc
    G4double pGreaseFrontGlassToMPPC_y;
    G4double pGreaseFrontGlassToMPPC_z;
    
    G4double pEpoxy_x;					//positions of epoxy layer on the mppc
    G4double pEpoxy_y;
    G4double pEpoxy_z;
    
    G4double pMPPCArray_x;				//positions of the entire mppc array (16 detectors + dead space)
    G4double pMPPCArray_y;
    G4double pMPPCArray_z;
    
    G4double pGreaseBackCryToGlass_x;			//positions of grease layer on the back (i.e. opposite to the mppc side) between crystals and glass
    G4double pGreaseBackCryToGlass_y;
    G4double pGreaseBackCryToGlass_z;
    
    G4double pGlassBack_x;				//positions of glass layer on the back (i.e. opposite to the mppc side)
    G4double pGlassBack_y;
    G4double pGlassBack_z;
    
    G4double pAirBack_x;				//positions of air on the back of back glass (esr is in dry contact with glass)
    G4double pAirBack_y;
    G4double pAirBack_z;
    
    G4double pFakeAirBack_x;				//positions of fake air on the back of back glass, useful to properly define the esr reflector there
    G4double pFakeAirBack_y;
    G4double pFakeAirBack_z;
    
    G4double lightyield;				//photons per MeV for LYSO
    G4double risetime;
    G4double decaytime;
    
    G4bool   latdepolished;
    G4double latsigmaalpha;
    G4double latspecularlobe;
    G4double latspecularspike;
    G4double latbackscattering;
    G4double latsurfaceroughness;
    
    G4bool   realdepolished;
    G4double realsigmaalpha;
    G4double realspecularlobe;
    G4double realspecularspike;
    G4double realbackscattering;
    G4double realsurfaceroughness;
    
    G4double esrTransmittance;
    G4double resolutionScale;
    G4double distance;
    
    std::vector<double>  fastenergy,fastcomponent,slowenergy,slowcomponent;
    G4double fastrisetime ;
    G4double fastdecaytime;
    G4double fastratio    ;
    G4double slowrisetime ;
    G4double slowdecaytime;
    
    
    G4bool lateralEsr;
    G4bool backEsr;
    
    G4bool latdepo_sideBySide[4];
    
    G4bool fCheckOverlaps;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*g4matrixDetectorConstruction_h*/
