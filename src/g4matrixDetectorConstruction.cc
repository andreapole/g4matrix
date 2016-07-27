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


#include "g4matrixDetectorConstruction.hh"
#include "G4SDManager.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
// #include "G4MultiFunctionalDetector.hh"
// #include "G4VPrimitiveSensitivity.hh"
// #include "PMTSD.hh"
// #include "WLSPhotonDetSD.hh"

#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4matrixDetectorConstruction::g4matrixDetectorConstruction()
 : G4VUserDetectorConstruction()
{
  //Set default physical dimensions
  //all dimensions in G4 are semi dimensions (so smart...) 
  //so dimensions are defined here, but will have to be divided by 2 when creating the objects
  
  //DEFAUTL DIMENSIONS
  //crystal dimensions
  fCrystal_x = fCrystal_y  =  1.5*mm;
  fCrystal_z =  15.0*mm;
  //number of crystals
  nCrystalsX = 8;
  nCrystalsY = 8;
  //number of detectors
  nDetectorsX = 4;
  nDetectorsY = 4;
  //air layer thickness
  airThinThickness = 0.05*mm;
  mppcGap = 0.2*mm;
  //set relative dimensions
  //air thin layer box dimensions
  fAirThinLayerBox_x = fCrystal_x + airThinThickness*mm;
  fAirThinLayerBox_y = fCrystal_y + airThinThickness*mm;
  fAirThinLayerBox_z = fCrystal_z;
  //grease front dimensions - cry to glass
  fGreaseFrontCryToGlass_x = fAirThinLayerBox_x * nCrystalsX;
  fGreaseFrontCryToGlass_y = fAirThinLayerBox_y * nCrystalsY;
  fGreaseFrontCryToGlass_z = 0.1*mm;
  //glass front dimensions  
  fGlassFront_x = fGreaseFrontCryToGlass_x;
  fGlassFront_y = fGreaseFrontCryToGlass_y;
  fGlassFront_z = 1.0*mm;
  //grease front dimensions - glass to mppc
  fGreaseFrontGlassToMPPC_x = fGreaseFrontCryToGlass_x;
  fGreaseFrontGlassToMPPC_y = fGreaseFrontCryToGlass_x;
  fGreaseFrontGlassToMPPC_z = 0.1*mm;
  
  //single mppc
  fSingleMPPC_x = 3.0*mm;
  fSingleMPPC_y = 3.0*mm;
  fSingleMPPC_z = 0.01*mm;
  //epoxy layer dimensions 
  fEpoxy_x = fMPPCArray_x;
  fEpoxy_y = fMPPCArray_x;
  fEpoxy_z = 0.1*mm;
  
  //single block -> one mppc plus "half of the gap" to the others (so 0.1mm)
  fSingleMPPCBlock_x = fSingleMPPC_x + ((mppcGap/2.0)*mm);
  fSingleMPPCBlock_y = fSingleMPPC_y + ((mppcGap/2.0)*mm);
  fSingleMPPCBlock_z = 0.0;
  //entire mppc array
  //fMPPCArray_x = fGreaseFrontCryToGlass_x;
  //fMPPCArray_y = fGreaseFrontCryToGlass_y;
  fMPPCArray_x = fSingleMPPCBlock_x * nDetectorsX;
  fMPPCArray_y = fSingleMPPCBlock_y * nDetectorsY;
  fMPPCArray_z = fSingleMPPC_z*mm;					//could be fixed to 0.01 mm to make cherenkov in silicon unlikely
  //grease back dimensions - cry to glass
  fGreaseBackCryToGlass_x = fAirThinLayerBox_x * nCrystalsX;
  fGreaseBackCryToGlass_y = fAirThinLayerBox_y * nCrystalsY;
  fGreaseBackCryToGlass_z = 0.1*mm;
  //glass back dimensions  
  fGlassBack_x = fGreaseBackCryToGlass_x;
  fGlassBack_y = fGreaseBackCryToGlass_y;
  fGlassBack_z = 1.0*mm;
  //air layer between back glass and esr
  fAirBack_x = fGlassBack_x;
  fAirBack_y = fGlassBack_y;
  fAirBack_z = 0.1*mm;
  //fake air layer between air world
  fFakeAirBack_x = fAirBack_x;
  fFakeAirBack_y = fAirBack_y;
  fFakeAirBack_z = airThinThickness;					//fixed to esr thickness
  //world dimensions
  //lenght is easy, it will have to accomodate for crystal length plus source distance plus some space (typically x2)
  fExpHall_z = fAirThinLayerBox_z * 2.0 + distance*2.0;
  //in witdh we want it twice the biggest width, so we need to check is the matrix or the mppcs are the biggest ones
  if( (fAirThinLayerBox_x * nCrystalsX * 2.0) > fMPPCArray_x)
    fExpHall_x = fAirThinLayerBox_x * nCrystalsX * 2.0;
  else 
    fExpHall_x = fMPPCArray_x *2.0;
  if((fExpHall_y = fAirThinLayerBox_y * nCrystalsY * 2.0) > fMPPCArray_y)
    fExpHall_y = fAirThinLayerBox_y * nCrystalsY * 2.0;
  else
    fExpHall_y = fMPPCArray_y *2.0;
  
  
  //DEFAULT POSITIONS (the relative ones, for child volumes, are input directly in the volume construction)
  //MATRIX
  //we want to the center of the matrix in 0,0,0, so in ax and y we will have to move the position of each box by half of the total matrix width
  //each air thin layer box will be fAirThinLayerBox_x, so the total lateral witdh is  fAirThinLayerBox_x * nCrystalsX. Same for y
  matrixShiftX = (fAirThinLayerBox_x * nCrystalsX)/2.0 -  fAirThinLayerBox_x/2.0;
  matrixShiftY = (fAirThinLayerBox_y * nCrystalsY)/2.0 -  fAirThinLayerBox_y/2.0;
  matrixShiftZ = 0;
  mppcShiftX = (fSingleMPPCBlock_x * nDetectorsX)/2.0 -  fSingleMPPCBlock_x/2.0;
  mppcShiftY = (fSingleMPPCBlock_y * nDetectorsY)/2.0 -  fSingleMPPCBlock_y/2.0;
  mppcShiftZ = 0;
  //GREASE front from cry to glass
  pGreaseFrontCryToGlass_x = 0.0;
  pGreaseFrontCryToGlass_y = 0.0;
  pGreaseFrontCryToGlass_z = fCrystal_z/2.0 + fGreaseFrontCryToGlass_z/2.0;
  //GLASS front 
  pGlassFront_x = 0.0;
  pGlassFront_y = 0.0;
  pGlassFront_z = pGreaseFrontCryToGlass_z + fGreaseFrontCryToGlass_z/2.0 + fGlassFront_z/2.0;
  //GREASE front from glass to mppc
  pGreaseFrontGlassToMPPC_x = 0.0;
  pGreaseFrontGlassToMPPC_y = 0.0;
  pGreaseFrontGlassToMPPC_z = pGlassFront_z + fGlassFront_z/2.0 + fGreaseFrontGlassToMPPC_z/2.0;
  //EPOXY
  pEpoxy_x = 0.0;
  pEpoxy_y = 0.0;
  pEpoxy_z = pGreaseFrontGlassToMPPC_z + fGreaseFrontGlassToMPPC_z/2.0 + fEpoxy_z/2.0;
  //MPPC ARRAY
  pMPPCArray_x = 0.0;
  pMPPCArray_y = 0.0;
  pMPPCArray_z = pEpoxy_z + fEpoxy_z/2.0 + fMPPCArray_z/2.0;
  //GREASE back from cry to glass
  pGreaseBackCryToGlass_x = 0.0;
  pGreaseBackCryToGlass_y = 0.0;
  pGreaseBackCryToGlass_z = - (fCrystal_z/2.0 + fGreaseBackCryToGlass_z/2.0);
  //GLASS back 
  pGlassBack_x = 0.0;
  pGlassBack_y = 0.0;
  pGlassBack_z = pGreaseBackCryToGlass_z - (fGreaseBackCryToGlass_z/2.0 + fGlassBack_z/2.0);
  //AIR layer between glass and vikuiti
  pAirBack_x = 0.0;
  pAirBack_y = 0.0;
  pAirBack_z = pGlassBack_z - (fGlassBack_z/2.0 + fAirBack_z/2.0);
  //FAKE AIR layer between air layer and world
  pFakeAirBack_x = 0.0;
  pFakeAirBack_y = 0.0;
  pFakeAirBack_z = pAirBack_z - (fAirBack_z/2.0 + fFakeAirBack_z/2.0);
  
  
  fCheckOverlaps = true;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4matrixDetectorConstruction::~g4matrixDetectorConstruction(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* g4matrixDetectorConstruction::Construct()
{
  
  //-------------------------------------------------------------------//
  //                                                                   //
  //                       RELATIVE DIMENSIONS                         //
  //                                                                   //
  //-------------------------------------------------------------------//
  
  //set relative dimensions
  //air thin layer box dimensions
  fAirThinLayerBox_x = fCrystal_x + airThinThickness;  //this is ESR + Crystal. So this dimension is the block that we repeat 
  fAirThinLayerBox_y = fCrystal_y + airThinThickness;
  fAirThinLayerBox_z = fCrystal_z;
  //grease front dimensions - cry to glass
  fGreaseFrontCryToGlass_x = fAirThinLayerBox_x * nCrystalsX;
  fGreaseFrontCryToGlass_y = fAirThinLayerBox_y * nCrystalsY;
  //glass front dimensions  
  fGlassFront_x = fGreaseFrontCryToGlass_x;
  fGlassFront_y = fGreaseFrontCryToGlass_y;
  //grease front dimensions - glass to mppc
  fGreaseFrontGlassToMPPC_x = fGreaseFrontCryToGlass_x;
  fGreaseFrontGlassToMPPC_y = fGreaseFrontCryToGlass_x;
  //single block -> one mppc plus "half of the gap" to the others (so 0.1mm)
  fSingleMPPCBlock_x = fSingleMPPC_x + (mppcGap);
  fSingleMPPCBlock_y = fSingleMPPC_y + (mppcGap);
  fSingleMPPCBlock_z = 0.0;                                     //this is always 0, it's actually useless.
  
  //entire mppc array
  fMPPCArray_x = fSingleMPPCBlock_x * nDetectorsX;
  fMPPCArray_y = fSingleMPPCBlock_y * nDetectorsY;
  fMPPCArray_z = fSingleMPPC_z;
  //epoxy layer dimensions 
  fEpoxy_x = fMPPCArray_x;
  fEpoxy_y = fMPPCArray_y;
  //grease back dimensions - cry to glass
  fGreaseBackCryToGlass_x = fAirThinLayerBox_x * nCrystalsX;
  fGreaseBackCryToGlass_y = fAirThinLayerBox_y * nCrystalsY;
  //glass back dimensions  
  fGlassBack_x = fGreaseBackCryToGlass_x;
  fGlassBack_y = fGreaseBackCryToGlass_y;
  //air layer between back glass and esr
  fAirBack_x = fGlassBack_x;
  fAirBack_y = fGlassBack_y;
  //fake air layer between air world
  fFakeAirBack_x = fAirBack_x;
  fFakeAirBack_y = fAirBack_y;
  //world dimensions
  //lenght is easy, it will have to accomodate for crystal length plus source distance plus some space (typically x2)
  fExpHall_z = fAirThinLayerBox_z * 2.0 + distance*2.0;
  //in witdh we want it twice the biggest width, so we need to check is the matrix or the mppcs are the biggest ones
  if( (fAirThinLayerBox_x * nCrystalsX * 2.0) > fMPPCArray_x)
    fExpHall_x = fAirThinLayerBox_x * nCrystalsX * 2.0;
  else 
    fExpHall_x = fMPPCArray_x *2.0;
  if((fExpHall_y = fAirThinLayerBox_y * nCrystalsY * 2.0) > fMPPCArray_y)
    fExpHall_y = fAirThinLayerBox_y * nCrystalsY * 2.0;
  else
    fExpHall_y = fMPPCArray_y *2.0;
  

  //-------------------------------------------------------------------//
  //                                                                   //
  //                       RELATIVE POSITIONS                          //
  //                                                                   //
  //-------------------------------------------------------------------//
  
  
  //RELATIVE POSITIONS
  //MATRIX
  //we want to the center of the matrix in 0,0,0, so in ax and y we will have to move the position of each box by half of the total matrix width
  //each air thin layer box will be fAirThinLayerBox_x, so the total lateral witdh is  fAirThinLayerBox_x * nCrystalsX. Same for y
  matrixShiftX = (fAirThinLayerBox_x * nCrystalsX)/2.0 -  fAirThinLayerBox_x/2.0;
  matrixShiftY = (fAirThinLayerBox_y * nCrystalsY)/2.0 -  fAirThinLayerBox_y/2.0;
  mppcShiftX = (fSingleMPPCBlock_x * nDetectorsX)/2.0 -  fSingleMPPCBlock_x/2.0;
  mppcShiftY = (fSingleMPPCBlock_y * nDetectorsY)/2.0 -  fSingleMPPCBlock_y/2.0;
  //GREASE front from cry to glass
  pGreaseFrontCryToGlass_z = fCrystal_z/2.0 + fGreaseFrontCryToGlass_z/2.0;
  //GLASS front 
  pGlassFront_z = pGreaseFrontCryToGlass_z + fGreaseFrontCryToGlass_z/2.0 + fGlassFront_z/2.0;
  //GREASE front from glass to mppc
  if(fGreaseFrontCryToGlass_z == 0 && fGlassFront_z == 0 && fGreaseFrontGlassToMPPC_z == 0) //if no volume is put between crystals and mpppcs, we put there an air layer of 0.01mm
  {
    pGreaseFrontGlassToMPPC_z = pGlassFront_z + fGlassFront_z/2.0 + 0.01/2.0;
  }
  else
  {
    pGreaseFrontGlassToMPPC_z = pGlassFront_z + fGlassFront_z/2.0 + fGreaseFrontGlassToMPPC_z/2.0;
  }
  //EPOXY
  if(fGreaseFrontCryToGlass_z == 0 && fGlassFront_z == 0 && fGreaseFrontGlassToMPPC_z == 0) //if no volume is put between crystals and mpppcs, we put there an air layer of 0.01mm
  {
    pEpoxy_z = pGreaseFrontGlassToMPPC_z + 0.01/2.0 + fEpoxy_z/2.0;
  }
  else
  {
    pEpoxy_z = pGreaseFrontGlassToMPPC_z + fGreaseFrontGlassToMPPC_z/2.0 + fEpoxy_z/2.0;
  }
  //MPPC ARRAY
  pMPPCArray_z = pEpoxy_z + fEpoxy_z/2.0 + fMPPCArray_z/2.0;
  //GREASE back from cry to glass
  pGreaseBackCryToGlass_z = - (fCrystal_z/2.0 + fGreaseBackCryToGlass_z/2.0);
  //GLASS back 
  pGlassBack_z = pGreaseBackCryToGlass_z - (fGreaseBackCryToGlass_z/2.0 + fGlassBack_z/2.0);
  //AIR layer between glass and vikuiti
  pAirBack_z = pGlassBack_z - (fGlassBack_z/2.0 + fAirBack_z/2.0);
  //FAKE AIR layer between air layer and world
  pFakeAirBack_z = pAirBack_z - (fAirBack_z/2.0 + fFakeAirBack_z/2.0);
  
  
  //-------------------------------------------------------------------//
  //                                                                   //
  //                            MATERIALS                              //
  //                                                                   //
  //-------------------------------------------------------------------//

  G4double a, z, density;
  G4int nelements;

  // Air - this is the external world
  G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
  G4Material* air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
  air->AddElement(N, 70.*perCent);
  air->AddElement(O, 30.*perCent);

  // Air thin layer
  // we need another air element, same characteristics as the other one, but 
  // different pointer, because we need to declare a surface between this air and esr
  G4Material* airThinLayer = new G4Material("AirThinLayer", density=1.29*mg/cm3, nelements=2);
  airThinLayer->AddElement(N, 70.*perCent);
  airThinLayer->AddElement(O, 30.*perCent);
  
  // LYSO
  G4Element *Lu = new G4Element ("Lutetium",  "Lu",  z = 71., a = 174.97 * g / mole);
  G4Element *Si = new G4Element ("Silicon", "Si", z = 14 , a = 28.09 * g / mole);
  G4Material *LYSO = new G4Material ("LYSO", density = 7.40 * g / cm3, 3, kStateSolid);
  LYSO->AddElement (Lu, 2);
  LYSO->AddElement (Si, 1);
  LYSO->AddElement (O, 5);

  //ESR
  G4Element *H = new G4Element ("Hydrogen", "H", z = 1 , a = 1.01 * g / mole);
  G4Element *C = new G4Element ("Carbon"  , "C", z = 6 , a = 12.01 * g / mole);
  G4Material *ESR = new G4Material ("ESR", density = 1.0 * g / cm3, 3, kStateSolid);
  ESR ->AddElement (H, 1);
  ESR ->AddElement (C, 1);
  ESR ->AddElement (O, 1);
  
  // Optical Grease
  G4Material *Grease = new G4Material ("Grease", density = 1.0 * g / cm3, 3);
  Grease->AddElement (C, 1);
  Grease->AddElement (H, 1);
  Grease->AddElement (O, 1);
  
  //Fused Silica - glass
  //G4Element *Si = new G4Element ("Silicon", "Si", z = 14 , a = 28.09 * g / mole);
  G4Material *Silicio = new G4Material ("Silicio", density = 2.3290 * g / cm3, 1);
  Silicio->AddElement (Si, 1);
  G4Material *SilicioMPPC = new G4Material ("SilicioMPPC", density = 2.3290 * g / cm3, 1);
  SilicioMPPC->AddElement (Si, 1);
  
  
  G4Material *Fused_silica = new G4Material ("Fused_silica", density = 2.2 * g / cm3, 2);
  Fused_silica->AddElement (Si, 1);
  Fused_silica->AddElement (O, 2);
  
  // Epoxy
  G4Material *Epoxy = new G4Material ("Epoxy", density = 1.0 * g / cm3, 3);
  Epoxy->AddElement (C, 1);
  Epoxy->AddElement (H, 1);
  Epoxy->AddElement (O, 1);
  
  //-------------------------------------------------------------------//
  //           Generate & Add Material Properties Table                //
  //-------------------------------------------------------------------//

  // Air
  // photon energy vector
  G4double photonEnergy[] ={ 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV, 2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV, 2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV, 2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV, 2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV, 3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV, 3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV, 3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };
  const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);
  //refractive index
  G4double refractiveIndex2[] = { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 };
  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX", photonEnergy, refractiveIndex2, nEntries);
  //G4cout << "Air G4MaterialPropertiesTable" << G4endl;
  //myMPT2->DumpTable();
  air->SetMaterialPropertiesTable(myMPT2);
  
  // Air Thin Layer
  // same as air
  //G4cout << "Air Thin Layer G4MaterialPropertiesTable" << G4endl;
  //myMPT2->DumpTable();
  airThinLayer->SetMaterialPropertiesTable(myMPT2);
  
  // Optical Grease
  const G4int nEntries1 = 34;
  G4double PhotonEnergy1[nEntries1] = { 0.0001 * eV, 1.0 * eV, 2.034 * eV, 2.068 * eV, 2.103 * eV, 2.139 * eV, 2.177 * eV, 2.216 * eV, 2.256 * eV, 2.298 * eV, 2.341 * eV, 2.386 * eV, 2.433 * eV, 2.481 * eV, 2.532 * eV, 2.585 * eV, 2.640 * eV, 2.697 * eV, 2.757 * eV, 2.820 * eV, 2.885 * eV, 2.954 * eV, 3.026 * eV, 3.102 * eV, 3.181 * eV, 3.265 * eV, 3.353 * eV, 3.446 * eV, 3.545 * eV, 3.649 * eV, 3.760 * eV, 3.877 * eV, 4.002 * eV, 4.136 * eV }; 
  G4double RefractiveIndex1[nEntries1] = { 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47 };
  G4double Absorption1[nEntries1] = { 3.448*m, 3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m, 15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m, 45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m, 52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m, 30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m, 17.500*m, 14.500*m };
  G4MaterialPropertiesTable *Grease_mt = new G4MaterialPropertiesTable();
  Grease_mt->AddProperty("RINDEX"   , PhotonEnergy1, RefractiveIndex1, nEntries1);
  Grease_mt->AddProperty("ABSLENGTH", PhotonEnergy1, Absorption1     , nEntries1);
  Grease->SetMaterialPropertiesTable (Grease_mt);
  
  // Fused silica
  const G4int numi = 5;
  G4double ephotoni[numi] = {1.8233 *eV, 2.0493 *eV,2.4797 *eV, 2.9520 *eV, 3.2627 *eV };
  G4double glass_ReR[numi]= {1.45      , 1.46      ,1.47      ,1.48       , 1.5        };
  G4double glass_ImR[numi]= {1./0.0000001,1./0.0000001,1./0.0000001,1./0.0000001,1./0.0000001};
  G4MaterialPropertiesTable* glass_mt = new G4MaterialPropertiesTable();
  glass_mt->AddProperty("RINDEX"   ,ephotoni , glass_ReR , numi);
  glass_mt->AddProperty("ABSLENGTH",ephotoni , glass_ImR , numi);
  Fused_silica->SetMaterialPropertiesTable(glass_mt);
  
  //Epoxy
  const G4int NUMENTRIES_5 = 3;
  G4double RIND1_Energy[NUMENTRIES_5]    = { 1.0 * eV, 1.84 * eV, 4.08 * eV };
  G4double RIND2_INDEX[NUMENTRIES_5]     = { 1.52    , 1.52     , 1.52      };
  G4MaterialPropertiesTable *epoxy_mt = new G4MaterialPropertiesTable();
  epoxy_mt->AddProperty ("RINDEX", RIND1_Energy, RIND2_INDEX,  NUMENTRIES_5 );
  Epoxy->SetMaterialPropertiesTable (epoxy_mt);
  
  //Silicium detector - To be used if a sensitive detector is decleared, but it's NOT USED presently in this simulation (detection is done always via stepping action)
  G4double Si_Energy[9] = { 1.00 * eV , 2.82 * eV , 2.88 * eV , 2.95 * eV , 3.02 * eV  , 3.10 * eV  , 3.18 * eV  , 3.26 * eV , 4.08 * eV };
  G4double Si_REF[9]    = { 0.1       , 0.15      , 0.18      , 0.2       , 0.22       , 0.3        , 0.4        , 0.6       , 0.8       };
  G4double Si_EFF[9]    = {1          , 1.        ,1.         ,1          ,1           ,1           ,1           ,1          ,1          }; //Enables 'detection' of photons
  G4MaterialPropertiesTable* Si_mt = new G4MaterialPropertiesTable();
  Si_mt->AddProperty("REFLECTIVITY", Si_Energy, Si_REF, 9);
  Si_mt->AddProperty("EFFICIENCY",Si_Energy, Si_EFF,9);   
  
  //LYSO
  
//   G4double FAST_Energy[NUMENTRIES_1] =  { 1.77169 * eV, 1.77266 * eV, 1.77558 * eV, 1.77851 * eV, 1.78145 * eV, 1.78539 * eV, 1.79033 * eV, 1.7963 * eV, 1.80231 * eV, 1.80836 * eV, 1.81445 * eV, 1.82058 * eV, 1.82882 * eV, 1.83401 * eV, 1.84553 * eV, 1.85293 * eV, 1.86147 * eV, 1.869 * eV, 1.87769 * eV,  1.89308 * eV, 1.90536 * eV, 1.92007 * eV, 1.93039 * eV, 1.94901 * eV, 1.95846 * eV, 1.9668 * eV, 1.97884 * eV,        1.99102 * eV,        2.00088 * eV,        2.01209 * eV,        2.02596 * eV,        2.03617 * eV,        2.04519 * eV,        2.0569 * eV,        2.06611 * eV,        2.0794 * eV,        2.09151 * eV,        2.10239 * eV,        2.112 * eV,        2.1231 * eV,        2.13431 * eV,        2.14565 * eV,        2.15566 * eV,        2.16868 * eV,        2.18038 * eV,        2.19519 * eV,        2.21171 * eV,        2.2193 * eV,        2.23619 * eV,        2.23464 * eV,        2.24395 * eV,        2.25806 * eV,        2.27234 * eV,        2.28358 * eV,       2.29493 * eV,        2.30475 * eV,        2.31631 * eV,        2.32463 * eV,        2.33134 * eV,        2.33809 * eV,        2.34487 * eV,        2.35856 * eV,        2.36719 * eV,        2.37939 * eV,        2.38642 * eV,        2.40238 * eV,        2.41134 * eV,        2.424 * eV, 2.43312 * eV,        2.44047 * eV,        2.44786 * eV,        2.46278 * eV,        2.47788 * eV,        2.48741 * eV,        2.49317 * eV,        2.49702 * eV,        2.50282 * eV,        2.50865 * eV,        2.5145 * eV,        2.52038 * eV,        2.52432 * eV,  2.53223 * eV,        2.5362 * eV,        2.54619 * eV,        2.55424 * eV,        2.56031 * eV,        2.56437 * eV,   2.57049 * eV,        2.57663 * eV,        2.58487 * eV,        2.59317 * eV,        2.59734 * eV,        2.60571 * eV,        2.61414 * eV,        2.61414 * eV,        2.61837 * eV,        2.62262 * eV,        2.62475 * eV,        2.62902 * eV,        2.63331 * eV,   2.63545 * eV,        2.63976 * eV,        2.64191 * eV,        2.64841 * eV,        2.65493 * eV,        2.6593 * eV,        2.66149 * eV,        2.66588 * eV,        2.67914 * eV,        2.67914 * eV,        2.68136 * eV,        2.68136 * eV,        2.68359 * eV,  2.68805 * eV,        2.68805 * eV,        2.68805 * eV,        2.69477 * eV,        2.69477 * eV,        2.69702 * eV,        2.70153 * eV,        2.70605 * eV,        2.71286 * eV,        2.71742 * eV,        2.71971 * eV,      2.722 * eV,        2.722 * eV,        2.72429 * eV,  2.72889 * eV, 2.72889 * eV,  2.73351 * eV, 2.73814 * eV,  2.74279 * eV, 2.74512 * eV,  2.74979 * eV, 2.75213 * eV,  2.75447 * eV, 2.75917 * eV,
//     2.75682 * eV, 2.76389 * eV,   2.76626 * eV, 2.76389 * eV,  2.76626 * eV, 2.77338 * eV,  2.77576 * eV, 2.78533 * eV, 2.79255 * eV, 2.79738 * eV, 2.80223 * eV, 2.80466 * eV, 2.80709 * eV, 2.80953 * eV, 2.80953 * eV, 2.81934 * eV, 2.8218 * eV, 2.82673 * eV, 2.83168 * eV, 2.84164 * eV, 2.84916 * eV, 2.85419 * eV, 2.8643 * eV, 2.86684 * eV, 2.87449 * eV, 2.87705 * eV, 2.87961 * eV, 2.88475 * eV, 2.88733 * eV, 2.8925 * eV, 2.89509 * eV, 2.90028 * eV, 2.90549 * eV, 2.90811 * eV, 2.91073 * eV, 2.91335 * eV, 2.91335 * eV, 2.91335 * eV, 2.91861 * eV, 2.92125 * eV, 2.92125 * eV, 2.92389 * eV, 2.92654 * eV, 2.92654 * eV, 2.92919 * eV, 2.92919 * eV, 2.93185 * eV, 2.93451 * eV, 2.93717 * eV, 2.93985 * eV, 2.94252 * eV, 2.9452 * eV, 2.94789 * eV, 2.94789 * eV,  2.94789 * eV,  2.95058 * eV,  2.95868 * eV,  2.96411 * eV,  2.96955 * eV,  2.97228 * eV,  2.97228 * eV,  2.96955 * eV,  2.97228 * eV, 2.97502 * eV, 2.97776 * eV, 2.97502 * eV, 2.9805 * eV,  2.9805 * eV,    2.9805 * eV,    2.98601 * eV,    2.99154 * eV,    2.99431 * eV,    2.99431 * eV,    2.99708 * eV,    2.99431 * eV,    2.99708 * eV,    3.00544 * eV,    3.00824 * eV,    3.00824 * eV,    3.00824 * eV,    3.00824 * eV,    3.01385 * eV,    3.0223 * eV,    3.02797 * eV,    3.03081 * eV,    3.02797 * eV,    3.03365 * eV,    3.03081 * eV,    3.03081 * eV,    3.0365 * eV,    3.03935 * eV,    3.04221 * eV,    3.04795 * eV,    3.04795 * eV,    3.05083 * eV,    3.05371 * eV,   3.05949 * eV,    3.06239 * eV,    3.06529 * eV,    3.0682 * eV,    3.06529 * eV,    3.07112 * eV,    3.0682 * eV,    3.07696 * eV,    3.08283 * eV,    3.0976 * eV,    3.09464 * eV,    3.09464 * eV,    3.10653 * eV,    3.11252 * eV,    3.11852 * eV,    3.12757 * eV,    3.13668 * eV,    3.14583 * eV,    3.15813 * eV,    3.16741 * eV,    3.17675 * eV,    3.20828 * eV,    3.23719 * eV,    3.26664 * eV,    3.28656 * eV,    3.31351 * eV,    3.34783 * eV,    3.38287 * eV,  };
//   G4double FAST_COMPONENT[NUMENTRIES_1] = {    0.011691,    0.011691,    0.011691,    0.0146138,    0.0146138,    0.0146138,    0.011691,    0.011691,    0.00876827,    0.00876827,    0.00584551,    0.00584551,    0.00584551,    0.00292276,    0.00876827,    0.0146138,    0.0146138,   0.0146138,    0.0204593,    0.023382,    0.0263048,    0.0204593,    0.0204593,    0.023382,    0.0292276,    0.0321503,    0.0350731,    0.0379958,    0.0379958,    0.0379958,    0.0350731,    0.0379958,    0.0409186,    0.0438413,    0.0526096,    0.0584551,    0.0643006,    0.0730689,   0.0730689,    0.0818372,    0.0906054,    0.0964509,    0.0993737,    0.105219,    0.111065,    0.122756,    0.125678,    0.146138,    0.146138,    0.160752,    0.157829,    0.163674,    0.184134,    0.192902,    0.20167,    0.219207,    0.230898,    0.242589,    0.25428,    0.265971,   0.274739,    0.292276,    0.306889,    0.315658,    0.321503,    0.350731,    0.368267,    0.385804,    0.397495,    0.415031,    0.432568,    0.458873,    0.482255,    0.496868,    0.514405,    0.529019,    0.549478,    0.564092,    0.581628,    0.593319,    0.602088,    0.616701,   0.637161,    0.660543,    0.681002,    0.71023,    0.736534,    0.756994,    0.777453,    0.806681,    0.844676,    0.868058,    0.891441,    0.9119,    0.938205,    0.955741,    0.984969,    1.0142,    1.03173,    1.05511,    1.07557,    1.11649,    1.13695,    1.15741,    1.17495,   1.19248,    1.21002,    1.22756,    1.27432,    1.2977,    1.31524,    1.32985,    1.36785,    1.40292,    1.39415,    1.4,    1.41754,    1.44092,    1.47015,    1.48476,    1.50814,    1.5286,    1.54906,    1.56952,    1.58998,    1.61921,    1.63967,    1.66597,    1.68935,    1.71566,   1.73904,    1.76242,    1.77996,    1.80042,    1.8238,    1.83549,    1.85303,    1.8618,    1.87933,    1.89979,    1.91733,    1.92902,    1.95825,    1.98163,    2.01378,    2.03424,    2.0547,    2.07808,    2.09562,    2.11023,    2.12484,    2.13361,    2.15407,    2.15699,   2.15992,    2.16576,    2.16868,    2.16868,    2.16284,    2.15699,    2.14823,    2.13946,    2.12484,    2.11023,    2.08977,    2.06639,    2.04593,    2.02839,    2.01086,    1.98455,    1.96409,    1.94948,    1.93194,    1.91733,    1.90271,    1.87641,    1.86472,    1.8501,   1.83841,    1.82088,    1.79749,    1.77119,    1.75073,    1.73027,    1.70689,    1.68058,    1.65428,    1.6309,    1.60167,    1.57244,    1.55491,    1.53152,    1.50522,    1.47891,    1.45261,    1.43215,    1.40877,    1.38831,    1.362,    1.33862,    1.31232,    1.28601,   1.27432,    1.25678,    1.21587,    1.19541,    1.17203,    1.14864,    1.12234,    1.10772,    1.08434,    1.06096,    1.0142,    0.987891,    0.967432,    0.938205,    0.9119,    0.879749,    0.853445,    0.82714,    0.786221,    0.765762,    0.739457,    0.716075,    0.681002,   0.660543,    0.637161,    0.60501,    0.581628,    0.552401,    0.531942,    0.505637,    0.485177,    0.458873,    0.435491,    0.412109,    0.379958,    0.356576,    0.336117,    0.309812,    0.280585,    0.25428,    0.207516,    0.175365,    0.157829,    0.13737,    0.119833,   0.0993737,    0.0759916,    0.0613779,    0.0526096,    0.0350731,    0.0263048,    0.011691,    0.00876827,    0.00876827,    0.011691,    0.011691,    0.011691,    0.00876827,    0.011691, };
  const G4int fastcomponent_NUMENTRIES = fastenergy.size();
  const G4int slowcomponent_NUMENTRIES = slowenergy.size();
  G4double FAST_Energy[fastcomponent_NUMENTRIES];
  G4double FAST_Component[fastcomponent_NUMENTRIES];
  G4double SLOW_Energy[slowcomponent_NUMENTRIES];
  G4double SLOW_Component[slowcomponent_NUMENTRIES];
  for(int i = 0; i < fastcomponent_NUMENTRIES; i++)
  {
    FAST_Energy[i] = fastenergy[i] * eV;
    FAST_Component[i] = fastcomponent[i];
  }
  for(int i = 0; i < slowcomponent_NUMENTRIES; i++)
  {
    SLOW_Energy[i] = slowenergy[i] * eV;
    SLOW_Component[i] = slowcomponent[i] ;
  }
  
  
  
  const G4int NUMENTRIES_2 = 31;
  G4double RIND_Energy[NUMENTRIES_2] = {3.5428571429*eV,3.4444444444*eV,3.3513513514*eV,3.2631578947*eV,3.1794871795*eV,3.1*eV,3.0243902439*eV,2.9523809524*eV,2.8837209302*eV,2.8181818182*eV,2.7555555556*eV,2.6956521739*eV,2.6382978723*eV,2.5833333333*eV,2.5306122449*eV,2.48*eV,2.431372549*eV,2.3846153846*eV,2.3396226415*eV,2.2962962963*eV,2.2545454545*eV,2.2142857143*eV,2.1754385965*eV,2.1379310345*eV,2.1016949153*eV,2.0666666667*eV,2.0327868852*eV,2*eV,1.9682539683*eV,1.9375*eV,1.9076923077*eV};
  G4double RIND_INDEX[NUMENTRIES_2] = { 1.8810632555,1.8660592056,1.8551403831,1.8468340056,1.8403005296,1.8350261081,1.8306784679,1.8270331239,1.8239328873,1.8212643626,1.8189436598,1.8169073619,1.815106624,1.8135032038,1.812066725,1.810772748,1.8096013826,1.808536274,1.8075638479,1.8066727422,1.8058533729,1.8050975982,1.804398457,1.8037499611,1.8031469314,1.8025848653,1.8020598293,1.8015683722,1.8011074531,1.8006743824,1.8002667721 };
  const G4int NUMENTRIES_3 = 9;
  G4double ABS_Energy[NUMENTRIES_3] = { 1.00 * eV , 2.82 * eV , 2.88 * eV , 2.95 * eV , 3.02 * eV  , 3.10 * eV  , 3.18 * eV  , 3.26 * eV , 4.08 * eV };
  G4double ABS_LENGTH[NUMENTRIES_3] = { 438.*mm   , 438.*mm   , 413.*mm   , 375.*mm   , 263.*mm    , 87.5 * mm  , 11.5 * mm  , 1.0 * mm  , 1.0 * mm  };
  const G4int NUMENTRIES_4 = 2;
  G4double RayleighEner[2] = {2.2543*eV, 4.08 *eV};
  G4double Rayleigh[NUMENTRIES_2] = {200*mm, 88*mm};
  G4MaterialPropertiesTable *MPT_LYSO = new G4MaterialPropertiesTable();
  if(fastratio != 0)// add a fast component only if the relative ratio of this component is != 0
  {
    MPT_LYSO->AddProperty ("FASTCOMPONENT", FAST_Energy , FAST_Component,  fastcomponent_NUMENTRIES);
    MPT_LYSO->AddConstProperty ("FASTSCINTILLATIONRISETIME", fastrisetime * ns);
    MPT_LYSO->AddConstProperty ("FASTTIMECONSTANT", fastdecaytime * ns);
    MPT_LYSO->AddConstProperty ("YIELDRATIO", fastratio);
  }
  else
  {
    MPT_LYSO->AddConstProperty ("YIELDRATIO", 0);
  }
  
  MPT_LYSO->AddProperty ("SLOWCOMPONENT", SLOW_Energy , SLOW_Component,  slowcomponent_NUMENTRIES);
  MPT_LYSO->AddConstProperty ("SLOWSCINTILLATIONRISETIME", slowrisetime * ns);
  MPT_LYSO->AddConstProperty ("SLOWTIMECONSTANT", slowdecaytime * ns);
  
  MPT_LYSO->AddProperty ("RINDEX",        RIND_Energy , RIND_INDEX,      NUMENTRIES_2);
  MPT_LYSO->AddProperty ("ABSLENGTH",     ABS_Energy  , ABS_LENGTH,      NUMENTRIES_3);
  MPT_LYSO->AddProperty ("RAYLEIGH",      RayleighEner, Rayleigh,        NUMENTRIES_4);
  MPT_LYSO->AddConstProperty ("SCINTILLATIONYIELD", lightyield / MeV);
  MPT_LYSO->AddConstProperty ("RESOLUTIONSCALE", resolutionScale);
  
  LYSO->SetMaterialPropertiesTable (MPT_LYSO);
  G4cout << "FASTSCINTILLATIONRISETIME = " << fastrisetime << G4endl;
  G4cout << "FASTTIMECONSTANT = " << fastdecaytime << G4endl;
  G4cout << "YIELDRATIO = " << fastratio << G4endl;
  G4cout << "SLOWTIMECONSTANT = " << slowdecaytime << G4endl;
  G4cout << "SLOWSCINTILLATIONRISETIME = " << slowrisetime << G4endl;  
  G4cout << "FAST_ENERGY = ";
  for(int i = 0; i < fastcomponent_NUMENTRIES ; i++)
  {
    G4cout << FAST_Energy[i] << " , ";
  }
  G4cout << G4endl;
  G4cout << "FAST_COMPONENT = ";
  for(int i = 0; i < fastcomponent_NUMENTRIES ; i++)
  {
    G4cout << FAST_Component[i] << " , ";
  }
  G4cout << G4endl;
  G4cout << "SLOW_ENERGY = ";
  for(int i = 0; i < slowcomponent_NUMENTRIES ; i++)
  {
    G4cout << SLOW_Energy[i] << " , ";
  }
  G4cout << G4endl;
  G4cout << "SLOW_COMPONENT = ";
  for(int i = 0; i < slowcomponent_NUMENTRIES ; i++)
  {
    G4cout << SLOW_Component[i] << " , ";
  }
  G4cout << G4endl; 
  
  //ESR
  const G4int NUMvikuiti = 29;
  G4double vikuiti_energy_im[NUMvikuiti] = { 1.25*eV, 1.31*eV, 1.34*eV, 1.38*eV, 1.42*eV, 1.46*eV, 1.51*eV, 1.60*eV, 1.66*eV, 1.75*eV, 1.80*eV, 1.90*eV, 1.95*eV, 2.05*eV, 2.10*eV, 2.20*eV, 2.26*eV, 2.35*eV, 2.45*eV, 2.51*eV, 2.71*eV, 2.81*eV, 3.01*eV, 3.21*eV, 3.41*eV, 3.55*eV, 3.71*eV, 3.91*eV, 4.14*eV };
//   G4double vikuiti_RIndex_im[NUMvikuiti] = 
//   {
//     9.49/1.47, 8.88/1.47, 8.49/1.47, 8.30/1.47, 8.18/1.47, 8.22/1.47, 8.31/1.47,
//     8.60/1.47, 8.62/1.47, 8.39/1.47, 8.21/1.47, 7.82/1.47, 7.65/1.47, 7.31/1.47,
//     7.15/1.47, 6.85/1.47, 6.69/1.47, 6.42/1.47, 6.15/1.47, 6.03/1.47, 5.58/1.47,
//     5.38/1.47, 5.02/1.47, 4.71/1.47, 4.43/1.47, 4.24/1.47, 4.06/1.47, 3.84/1.47, 3.61/1.47
//     
//   };
  //we specify the imaginary index WITHOUT dividing by 1.47 because in this matrix 
  // esr is in air contact with everything, so n1=1
  G4double vikuiti_RIndex_im[NUMvikuiti] = { 9.49, 8.88, 8.49, 8.30, 8.18, 8.22, 8.31, 8.60, 8.62, 8.39, 8.21, 7.82, 7.65, 7.31, 7.15, 6.85, 6.69, 6.42, 6.15, 6.03, 5.58, 5.38, 5.02, 4.71, 4.43, 4.24, 4.06, 3.84, 3.61 };
  G4double vikuiti_energy_re[NUMvikuiti] = { 1.24*eV, 1.27*eV, 1.31*eV, 1.34*eV, 1.38*eV, 1.42*eV, 1.46*eV, 1.51*eV, 1.55*eV, 1.60*eV, 1.66*eV, 1.71*eV, 1.77*eV, 1.84*eV, 1.91*eV, 1.99*eV, 2.07*eV, 2.16*eV, 2.26*eV, 2.37*eV, 2.48*eV, 2.62*eV, 2.76*eV, 2.92*eV, 3.11*eV, 3.31*eV, 3.55*eV, 3.82*eV, 4.14*eV };
//   G4double vikuiti_RIndex_re[NUMvikuiti] = 
//   {
//     0.08/1.47, 0.08/1.47, 0.08/1.47, 0.08/1.47, 0.08/1.47, 0.08/1.47, 0.08/1.47,
//     0.1/1.47, 0.6/1.47, 1.2/1.47, 0.4/1.47, 0.23/1.47, 0.17/1.47, 0.26/1.47, 1.3/1.47,
//     0.24/1.47, 0.11/1.47, 0.19/1.47, 0.24/1.47, 0.25/1.47, 0.87/1.47, 0.595/1.47,
//     0.18/1.47, 0.178/1.47, 0.6/1.47, 8.0/1.47, 8.0/1.47, 8.0/1.47, 8.0/1.47
//   };
  G4double vikuiti_RIndex_re[NUMvikuiti] = { 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.1, 0.6, 1.2, 0.4, 0.23, 0.17, 0.26, 1.3, 0.24, 0.11, 0.19, 0.24, 0.25, 0.87, 0.595, 0.18, 0.178, 0.6, 8.0, 8.0, 8.0, 8.0 };
  const G4int NUMu = 2;
  G4double pp[NUMu] = {2.038*eV, 4.144*eV};
//   G4double specularlobe[NUMu] = {0.0, 0.0};
//   G4double specularspike[NUMu] = {0, 0};
//   G4double backscatter[NUMu] = {0., 0.};
//   G4double reflectivity[NUMu] = {0.98, 0.98};
  //G4double reflectivity[NUMu] = {0.925, 0.925};
  //G4MaterialPropertiesTable *ESR_mt = new G4MaterialPropertiesTable();
  //now, the esr as a MATERIAL as only an index of refraction, i.e. nothing will happen to the photon while travelling it 
  //all the processes (reflection, transmission, eventual absorption) will happen at the boundary with air (going in or going out)
  //TODO check this in the G4 source code
  //ESR_mt->AddProperty ("REALRINDEX",        vikuiti_energy_re,  vikuiti_RIndex_re,      NUMvikuiti);
  //ESR_mt->AddProperty ("IMAGINARYRINDEX",        vikuiti_energy_im,  vikuiti_RIndex_im,    NUMvikuiti);
  //ESR_mt->AddProperty ("REFLECTIVITY",        pp,  reflectivity,      NUMu);
  //ESR_mt->AddProperty("SPECULARLOBECONSTANT",pp,specularlobe,NUMu);
  //ESR_mt->AddProperty("SPECULARSPIKECONSTANT",pp,specularspike,NUMu);
  //ESR_mt->AddProperty("BACKSCATTERCONSTANT",pp,backscatter,NUMu);
  //ESR->SetMaterialPropertiesTable (ESR_mt);
  //now the surface made of esr
  //we want to specify only Re(R), Im(R) and a Transmittance 
  //reflection will then be perfectly specular
//   G4double specularlobei[NUMu] = {0.0, 0.0};
//   G4double specularspikei[NUMu] = {0, 0};
//   G4double backscatteri[NUMu] = {0., 0.};
//   G4double reflectivityi[NUMu] = {0.0, 0.00};
  G4MaterialPropertiesTable *ESR_surf = new G4MaterialPropertiesTable();
  //ESR_surf->AddProperty("SPECULARLOBECONSTANT",pp,specularlobei,NUMu);
  //ESR_surf->AddProperty("SPECULARSPIKECONSTANT",pp,specularspikei,NUMu);
  //ESR_surf->AddProperty("BACKSCATTERCONSTANT",pp,backscatteri,NUMu);
  G4double FixedTransmittance_surf[NUMu] = {esrTransmittance, esrTransmittance};
  
  G4double esrTransmittance_energy[] = { 2.7008152174*eV, 2.7304945055*eV, 2.7608333333*eV, 2.7918539326*eV,	2.8235795455*eV, 2.8560344828*eV, 2.889244186*eV , 2.9232352941*eV,2.9580357143*eV, 2.9936746988*eV, 3.0301829268*eV, 3.0675925926*eV, 3.1059375*eV   , 3.1452531646*eV, 3.1855769231*eV, 3.2269480519*eV, 3.2694078947*eV};
  G4double esrTransmittance_values[] = {0.0024,0.0025,0.002 ,0.0024,0.0015,0.0011,0.0011,0.0011,0.0009,0.0007,0.001 ,0.0016,0.005 ,0.0164,0.0303,0.0365,0.0389};
  const G4int numEsrTrans = sizeof(esrTransmittance_energy)/sizeof(G4double);
  assert(sizeof(esrTransmittance_energy) == sizeof(esrTransmittance_values));
  
  //modfication for double surface. since the Transmittance measured is for a real material, with 2 surfaces, we need to scale the measurement to have a correct result when we use the two of them
  //this is anyway a big question mark..
  for(int esr_n = 0; esr_n < numEsrTrans ; esr_n++)
  {
    esrTransmittance_values[esr_n] = pow(esrTransmittance_values[esr_n],0.6666666666666666666);
  }
  
  ESR_surf->AddProperty ("REALRINDEX",        vikuiti_energy_re,  vikuiti_RIndex_re,  NUMvikuiti);
  ESR_surf->AddProperty ("IMAGINARYRINDEX",   vikuiti_energy_im,  vikuiti_RIndex_im,  NUMvikuiti);
  if(esrTransmittance == -1)
  {
    ESR_surf->AddProperty ("TRANSMITTANCE", esrTransmittance_energy , esrTransmittance_values,  numEsrTrans);
  }
  else
  {
    ESR_surf->AddProperty ("TRANSMITTANCE", pp , FixedTransmittance_surf,  NUMu);
  }
  
  //------------------------------------//
  //            DEPOLISHING             //
  //------------------------------------//
  
  // -------- Lateral faces ------------//
  
  //when crystal is depolished, we can define a surface between crystal and air thin layer
  G4MaterialPropertiesTable *crystalDepolished_surf = new G4MaterialPropertiesTable();
  const G4int Ndepo = 2;
  G4double depoEnergy[Ndepo] = {2.038*eV, 4.144*eV};
  G4double specularlobeVector[Ndepo] = {latspecularlobe, latspecularlobe};
  G4double specularspikeVector[Ndepo] = {latspecularspike, latspecularspike};
  G4double backscatterVector[Ndepo] = {latbackscattering,latbackscattering};
  crystalDepolished_surf->AddProperty("SPECULARLOBECONSTANT",depoEnergy,specularlobeVector,Ndepo);
  crystalDepolished_surf->AddProperty("SPECULARSPIKECONSTANT",depoEnergy,specularspikeVector,Ndepo);
  crystalDepolished_surf->AddProperty("BACKSCATTERCONSTANT",depoEnergy,backscatterVector,Ndepo);
  if(latsurfaceroughness != 0)
    crystalDepolished_surf->AddConstProperty("SURFACEROUGHNESS", latsurfaceroughness); 
  
  // ---- Front and Back faces --------//
  
  G4MaterialPropertiesTable *crystalReal_surf = new G4MaterialPropertiesTable();
  G4double specularlobeRealVector[Ndepo] = {realspecularlobe, realspecularlobe};
  G4double specularspikeRealVector[Ndepo] = {realspecularspike, realspecularspike};
  G4double backscatterRealVector[Ndepo] = {realbackscattering,realbackscattering};
  crystalReal_surf->AddProperty("SPECULARLOBECONSTANT",depoEnergy,specularlobeRealVector,Ndepo);
  crystalReal_surf->AddProperty("SPECULARSPIKECONSTANT",depoEnergy,specularspikeRealVector,Ndepo);
  crystalReal_surf->AddProperty("BACKSCATTERCONSTANT",depoEnergy,backscatterRealVector,Ndepo);
  if(realsurfaceroughness != 0)
    crystalReal_surf->AddConstProperty("SURFACEROUGHNESS", realsurfaceroughness); 
  
  
  //silicon
  G4double siliconRindexEnergy[76] = 
  {
    1.2407002545,
    1.2532325803,
    1.2660206679,
    1.2790724273,
    1.2923960984,
    1.3060002679,
    1.3198938878,
    1.3340862952,
    1.3485872332,
    1.3634068731,
    1.3785558383,
    1.3940452298,
    1.4098866528,
    1.4260922466,
    1.4426747145,
    1.4596473582,
    1.4770241125,
    1.4948195837,
    1.5130490909,
    1.5317287093,
    1.5508753181,
    1.5705066513,
    1.5906413519,
    1.6112990318,
    1.6325003349,
    1.654267006	,
    1.6766219655,
    1.6995893897,
    1.7231947979,
    1.7474651472,
    1.772428935	,
    1.7981163109,
    1.8245591978,
    1.8517914246,
    1.8798488705,
    1.9087696223,
    1.9385941477,
    1.9693654833,
    2.0011294428,
    2.0339348435,
    2.0678337575,
    2.1028817873,
    2.1391383698,
    2.1766671132,
    2.2155361688,
    2.2558186446,
    2.2975930639,
    2.3409438764,
    2.3859620279,
    2.4327455971,
    2.481400509	,
    2.5320413357,
    2.5847921969,
    2.6397877755,
    2.6971744663,
    2.7571116767,
    2.8197733057,
    2.8853494291,
    2.954048225	,
    3.0260981817,
    3.1017506363,
    3.1812827039,
    3.2650006698,
    3.3532439311,
    3.4463895958,
    3.54485787	,
    3.6491183956,
    3.7596977409,
    3.8771882953,
    4.0022588855,
    4.135667515	,
    4.2782767397,
    4.4310723375,
    4.5951861278,
    4.7719240558,
    4.962801018
  };
  G4double siliconRindexValue[76] = 
  {
    3.57,
    3.574,
    3.578,
    3.582,
    3.587,
    3.592,
    3.597,
    3.602,
    3.608,
    3.614,
    3.62 ,
    3.626,
    3.632,
    3.638,
    3.644,
    3.65 ,
    3.656,
    3.662,
    3.668,
    3.674,
    3.681,
    3.688,
    3.696,
    3.705,
    3.714,
    3.723,
    3.732,
    3.741,
    3.751,
    3.762,
    3.774,
    3.787,
    3.8  ,
    3.815,
    3.83 ,
    3.844,
    3.861,
    3.879,
    3.895,
    3.916,
    3.939,
    3.962,
    3.986,
    4.015,
    4.044,
    4.077,
    4.11 ,
    4.15 ,
    4.192,
    4.239,
    4.293,
    4.348,
    4.416,
    4.491,
    4.577,
    4.676,
    4.793,
    4.925,
    5.091,
    5.305,
    5.587,
    5.976,
    6.548,
    6.863,
    6.014,
    5.483,
    5.293,
    5.179,
    5.102,
    5.074,
    5.055,
    4.426,
    3.052,
    2.129,
    1.8  ,
    1.694
  };
  G4double siliconAbsLenghtValue[76] =
  {
    1.56E-02,
    1.26E-02,
    1.04E-02,
    8.77E-03,
    7.46E-03,
    6.37E-03,
    5.46E-03,
    4.76E-03,
    4.17E-03,
    3.68E-03,
    3.27E-03,
    2.92E-03,
    2.61E-03,
    2.31E-03,
    2.08E-03,
    1.87E-03,
    1.69E-03,
    1.55E-03,
    1.41E-03,
    1.29E-03,
    1.18E-03,
    1.08E-03,
    9.90E-04,
    9.09E-04,
    8.40E-04,
    7.69E-04,
    7.04E-04,
    6.49E-04,
    6.02E-04,
    5.65E-04,
    5.26E-04,
    4.88E-04,
    4.52E-04,
    4.20E-04,
    3.88E-04,
    3.56E-04,
    3.29E-04,
    3.06E-04,
    2.84E-04,
    2.62E-04,
    2.42E-04,
    2.23E-04,
    2.05E-04,
    1.88E-04,
    1.73E-04,
    1.56E-04,
    1.42E-04,
    1.27E-04,
    1.14E-04,
    1.03E-04,
    9.01E-05,
    7.87E-05,
    6.76E-05,
    5.81E-05,
    4.76E-05,
    3.92E-05,
    3.22E-05,
    2.55E-05,
    2.00E-05,
    1.48E-05,
    1.05E-05,
    6.67E-06,
    3.41E-06,
    1.43E-06,
    9.80E-07,
    9.62E-07,
    9.17E-07,
    8.55E-07,
    7.81E-07,
    6.94E-07,
    5.78E-07,
    4.46E-07,
    4.24E-07,
    4.59E-07,
    5.08E-07,
    5.43E-07
  };
  const G4int nBinsSilicon = sizeof(siliconRindexEnergy)/sizeof(G4double);
  assert(sizeof(siliconRindexEnergy) == sizeof(siliconRindexValue));
  G4MaterialPropertiesTable *Silicon_mt = new G4MaterialPropertiesTable();
  G4double siliconAbsLenghtEnergy[] = {1*eV,2*eV,3*eV};
  //G4double siliconAbsLenghtValue[]  = {0.1*mm,0.1*mm,0.1*mm};
  Silicon_mt->AddProperty ("RINDEX", siliconRindexEnergy , siliconRindexValue, nBinsSilicon );
  Silicon_mt->AddProperty ("ABSLENGTH" , siliconAbsLenghtEnergy , siliconAbsLenghtValue , 3);
  Silicio->SetMaterialPropertiesTable(Silicon_mt);
  SilicioMPPC->SetMaterialPropertiesTable(Silicon_mt);
  
  //-------------------------------------------------------------------//
  //                                                                   //
  //                             VOLUMES                               //
  //                                                                   //
  //-------------------------------------------------------------------//

  //simple crystal, wrapped in esr but with air between crystal and esr
  //1. we want to have a dielectric_dielectric surface between crystal and air
  //so we just need to specify the real indexes of refraction of both materials
  //and we don't need to create a surface
  //2. we want a dielectric_metal surface between air and esr, thus we need to declare a surface
  //between the two. We want this surface to have reflectivity calculated from the indexes of refraction 
  //real and imaginary of esr and the real of air, although G4 makes a mistake and doesn't take into account the 
  //index of air. But since air has index = 1, here it doesn't matter. 
  
  // The experimental Hall
  G4Box* expHall_box = new G4Box("World",fExpHall_x/2.0,fExpHall_y/2.0,fExpHall_z/2.0);
  G4LogicalVolume* expHall_log = new G4LogicalVolume(expHall_box,airThinLayer,"World",0,0,0);
  expHall_log->SetVisAttributes (G4VisAttributes::GetInvisible());
  G4VPhysicalVolume* expHall_phys = new G4PVPlacement(0,G4ThreeVector(),expHall_log,"World",0,false,fCheckOverlaps);
  
  
  //new structure: 
  //the fundamental block is the esr box as before, but
  //it is a air box that has not one but 5 boxes inside:
  //the crystal in the center, and 4 boxes of air or other material in contact with the lateral surfs of the crystal
  //then a surface is defined between crystal and each one of these surfs
  
  
  
  //----------------
  //OLD VERSION
  //----------------
  
//   // the esr box
// //   G4Box* esr_box = new G4Box("Esr",fAirThinLayerBox_x/2.0,fAirThinLayerBox_x/2.0,fAirThinLayerBox_x/2.0);
// //   G4LogicalVolume* esr_log = new G4LogicalVolume(esr_box,air,"Esr",0,0,0);
// //   G4VPhysicalVolume* esr_phys = new G4PVPlacement(0,G4ThreeVector(),esr_log,"Esr",expHall_log,false,0);
// 
//   // the air thin layer box
//   // now we do it in a "matrix" way with pointers. This is hell.
//   	
  //airboxes
  G4Box*** airThinLayer_box					= new G4Box** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) airThinLayer_box[i]	= new G4Box* [nCrystalsY];
  G4LogicalVolume*** airThinLayer_log 				= new G4LogicalVolume** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) airThinLayer_log[i]	= new G4LogicalVolume* [nCrystalsY];
  G4VPhysicalVolume*** airThinLayer_phys			= new G4VPhysicalVolume** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) airThinLayer_phys[i]	= new G4VPhysicalVolume* [nCrystalsY];
  //crystals
  G4Box*** crystal_box						= new G4Box** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) crystal_box[i]		= new G4Box* [nCrystalsY];
  G4LogicalVolume*** crystal_log				= new G4LogicalVolume** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) crystal_log[i]		= new G4LogicalVolume* [nCrystalsY];
  G4VPhysicalVolume*** crystal_phys 				= new G4VPhysicalVolume** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) crystal_phys[i]		= new G4VPhysicalVolume* [nCrystalsY];
  //4 air volumes
  G4Box**** lateral_box						= new G4Box*** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) 
  {
      lateral_box[i]           	= new G4Box** [nCrystalsY];
      for(int j = 0 ; j < nCrystalsY ; j++)
          lateral_box[i][j]     = new G4Box* [4];
  }
  G4LogicalVolume**** lateral_log				= new G4LogicalVolume*** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) 
  {
      lateral_log[i]           	= new G4LogicalVolume** [nCrystalsY];
      for(int j = 0 ; j < nCrystalsY ; j++)
          lateral_log[i][j]     = new G4LogicalVolume* [4];
  }
  G4VPhysicalVolume**** lateral_phys				= new G4VPhysicalVolume*** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) 
  {
      lateral_phys[i]           	= new G4VPhysicalVolume** [nCrystalsY];
      for(int j = 0 ; j < nCrystalsY ; j++)
          lateral_phys[i][j]     = new G4VPhysicalVolume* [4];
  }
  //4 esr volumes
  G4Box**** esr_box						= new G4Box*** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) 
  {
      esr_box[i]           	= new G4Box** [nCrystalsY];
      for(int j = 0 ; j < nCrystalsY ; j++)
          esr_box[i][j]     = new G4Box* [4];
  }
  G4LogicalVolume**** esr_log				= new G4LogicalVolume*** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) 
  {
      esr_log[i]           	= new G4LogicalVolume** [nCrystalsY];
      for(int j = 0 ; j < nCrystalsY ; j++)
          esr_log[i][j]     = new G4LogicalVolume* [4];
  }
  G4VPhysicalVolume**** esr_phys				= new G4VPhysicalVolume*** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) 
  {
      esr_phys[i]           	= new G4VPhysicalVolume** [nCrystalsY];
      for(int j = 0 ; j < nCrystalsY ; j++)
          esr_phys[i][j]     = new G4VPhysicalVolume* [4];
  }
  
  G4OpticalSurface**** opCrystalToAirThinLayerSurface		= new G4OpticalSurface*** [nCrystalsX];
  G4OpticalSurface**** opCrystalToAirThinLayerSurfaceInv		= new G4OpticalSurface*** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) 
  {
      opCrystalToAirThinLayerSurface[i]           	= new G4OpticalSurface** [nCrystalsY];
      opCrystalToAirThinLayerSurfaceInv[i]           	= new G4OpticalSurface** [nCrystalsY];
      for(int j = 0 ; j < nCrystalsY ; j++)
      {
          opCrystalToAirThinLayerSurface[i][j]     = new G4OpticalSurface* [4];
          opCrystalToAirThinLayerSurfaceInv[i][j]     = new G4OpticalSurface* [4];
      }
  }
  
  G4OpticalSurface**** opEsrToAirThinLayerSurface		= new G4OpticalSurface*** [nCrystalsX];
  G4OpticalSurface**** opEsrToAirThinLayerSurfaceInv		= new G4OpticalSurface*** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) 
  {
      opEsrToAirThinLayerSurface[i]           	= new G4OpticalSurface** [nCrystalsY];
      opEsrToAirThinLayerSurfaceInv[i]           	= new G4OpticalSurface** [nCrystalsY];
      for(int j = 0 ; j < nCrystalsY ; j++)
      {
         opEsrToAirThinLayerSurface[i][j]     = new G4OpticalSurface* [4];
         opEsrToAirThinLayerSurfaceInv[i][j]     = new G4OpticalSurface* [4];
      }
  }
  G4OpticalSurface**** opAirThinLayerToWorldSurface				= new G4OpticalSurface*** [nCrystalsX];
  G4OpticalSurface**** opAirThinLayerToWorldSurfaceInv				= new G4OpticalSurface*** [nCrystalsX];
  for(int i = 0 ; i < nCrystalsX ; i++) 
  {
      opAirThinLayerToWorldSurface[i]	= new G4OpticalSurface** [nCrystalsY];
      opAirThinLayerToWorldSurfaceInv[i]	= new G4OpticalSurface** [nCrystalsY];
      for(int j = 0 ; j < nCrystalsY ; j++)
      {
          opAirThinLayerToWorldSurface[i][j]     = new G4OpticalSurface* [4];
          opAirThinLayerToWorldSurfaceInv[i][j]     = new G4OpticalSurface* [4];
      }
  }
//   G4OpticalSurface*** opCrystalToAirThinLayerSurface				= new G4OpticalSurface** [nCrystalsX];
//     for(int i = 0 ; i < nCrystalsX ; i++) opCrystalToAirThinLayerSurface[i]	= new G4OpticalSurface* [nCrystalsY];
  
  G4int crystalNumber = 0;
  
  for(G4int i = 0 ; i < nCrystalsX ; i++)
  {
    for(G4int j = 0 ; j < nCrystalsY ; j++)
    {
      //1. create the air box
      std::stringstream name;
      name << "AirThinLayer_" << i << "_" << j; 
      //box volume
      airThinLayer_box[i][j]  = new G4Box(name.str().c_str(),fAirThinLayerBox_x/2.0,fAirThinLayerBox_y/2.0,fAirThinLayerBox_z/2.0);
//       G4cout << name.str() << " Dimensions" << "\t" << fAirThinLayerBox_x/2.0 << "\t" << fAirThinLayerBox_y/2.0 << "\t" << fAirThinLayerBox_z/2.0 << G4endl;
      //logical volume
      airThinLayer_log[i][j]  = new G4LogicalVolume(airThinLayer_box[i][j],airThinLayer,name.str().c_str(),0,0,0);
      G4VisAttributes* EsrVisulizationAttribute = new G4VisAttributes(G4Colour(1.0,0.0,1.0)); //magenta
      airThinLayer_log[i][j]->SetVisAttributes(EsrVisulizationAttribute); // we also set here the visualization colors
      //airThinLayer_log[i][j]->SetVisAttributes (G4VisAttributes::GetInvisible());
      //and we place the box in space 
      airThinLayer_phys[i][j] = new G4PVPlacement(0,G4ThreeVector(i*fAirThinLayerBox_x - matrixShiftX ,j*fAirThinLayerBox_y - matrixShiftY,matrixShiftZ),airThinLayer_log[i][j],name.str().c_str(),expHall_log,false,0);
//       G4cout << name.str() << " Position" << "\t" << i*fAirThinLayerBox_x - matrixShiftX  << "\t" << j*fAirThinLayerBox_y - matrixShiftY << "\t" << matrixShiftZ << G4endl;
      
      //2. create the crystal
      name.str("");
      name << "Crystal_" << i << "_" << j;
      //box volumes
      crystal_box[i][j]  = new G4Box(name.str().c_str(),fCrystal_x/2.0,fCrystal_y/2.0,fCrystal_z/2.0);
      //logical volumes
      crystal_log[i][j]  = new G4LogicalVolume(crystal_box[i][j],LYSO,name.str().c_str(),0,0,0);
      G4VisAttributes* CryVisulizationAttribute = new G4VisAttributes(G4Colour(0.0,1.0,1.0)); //cyan
      crystal_log[i][j]->SetVisAttributes(CryVisulizationAttribute); // we also set here the visualization colors
      // and we place the box in space 
      // it is assigned to its mother volume, the air thin layer, so it just has to be placed in the middle of it
      //for the physical volumes, we use as name just a number (makes analysis easier)
      name.str("");
      name << crystalNumber;
      crystal_phys[i][j] = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),crystal_log[i][j],name.str().c_str(),airThinLayer_log[i][j],false,fCheckOverlaps);
      crystalNumber++;
      
      //3. create the 4 volumes
      // arranged this way wrt the crystal 
      //
      //       k=0
      // k=3 crystal k=1
      //       k=2
      //
      // positions and dimensions
      // remember airThinThickness is the nominal esr thickness aka the gap between crystals
      // we want these volumes to be half of the air space between esr volumes (which we will create later)
      // and crystal. This space is set by the config file to airgap. so the value is airgap/4.0 (remember that geant4 wants half dims)
      
      double dim_x[4]   = {fCrystal_x/2.0,airgap/4.0,fCrystal_x/2.0,airgap/4.0}; 
      double dim_y[4]   = {airgap/4.0,fCrystal_y/2.0,airgap/4.0,fCrystal_y/2.0};
      
      double shift_x[4] = {0,fCrystal_x/2.0 + airgap/4.0,0,-(fCrystal_x/2.0 + airgap/4.0)};
      double shift_y[4] = {fCrystal_y/2.0 + airgap/4.0,0,-(fCrystal_y/2.0 + airgap/4.0),0};
//       double dim_x[4]   = {fCrystal_x/2.0,airThinThickness/4.0,fCrystal_x/2.0,airThinThickness/4.0}; 
//       double dim_y[4]   = {airThinThickness/4.0,fCrystal_y/2.0,airThinThickness/4.0,fCrystal_y/2.0};
//       bool latdepo_sideBySide[4] = {true,false,true,false};

      for(int k = 0 ; k < 4 ; k++)
      {
          name.str("");
          name << "Lateral_" << i << "_" << j << "_" << k;
          lateral_box[i][j][k]  = new G4Box(name.str().c_str(),dim_x[k],dim_y[k],fAirThinLayerBox_z/2.0);
//           G4cout << name.str() << G4endl;
          //logical volume
          lateral_log[i][j][k]  = new G4LogicalVolume(lateral_box[i][j][k],airThinLayer,name.str().c_str(),0,0,0);
          G4VisAttributes* lateralVisulizationAttribute = new G4VisAttributes(G4Colour(1.0,0.0,0.0)); //red
          lateral_log[i][j][k]->SetVisAttributes(lateralVisulizationAttribute); // we also set here the visualization colors
          //airThinLayer_log[i][j]->SetVisAttributes (G4VisAttributes::GetInvisible());
          //and we place the box in space 
          lateral_phys[i][j][k] = new G4PVPlacement(0,G4ThreeVector(shift_x[k],shift_y[k],0),lateral_log[i][j][k],name.str().c_str(),airThinLayer_log[i][j],false,0);
//           G4cout << name.str() << " Position" << "\t" << i*fAirThinLayerBox_x - matrixShiftX  << "\t" << j*fAirThinLayerBox_y - matrixShiftY << "\t" << matrixShiftZ << G4endl;
          //define the surface here, for depolishing
          if(latdepo_sideBySide[k])
          {
              std::stringstream Surfname;
              Surfname << "CrystalToAirThinLayerSurface_" << i << "_" << j << "_" << k; 
              opCrystalToAirThinLayerSurface[i][j][k] = new G4OpticalSurface(Surfname.str().c_str());
              opCrystalToAirThinLayerSurface[i][j][k]->SetType(dielectric_dielectric);
              opCrystalToAirThinLayerSurface[i][j][k]->SetFinish(ground);
              opCrystalToAirThinLayerSurface[i][j][k]->SetModel(unified);
              opCrystalToAirThinLayerSurface[i][j][k]->SetSigmaAlpha(latsigmaalpha);
              opCrystalToAirThinLayerSurface[i][j][k]->SetMaterialPropertiesTable(crystalDepolished_surf); //this sets the surface roughness
              new G4LogicalBorderSurface(Surfname.str().c_str(),crystal_phys[i][j],lateral_phys[i][j][k],opCrystalToAirThinLayerSurface[i][j][k]);
//               G4cout << Surfname.str() << G4endl;
              //and the reverse...
              Surfname << "Inv";
              opCrystalToAirThinLayerSurfaceInv[i][j][k] = new G4OpticalSurface(Surfname.str().c_str());
              opCrystalToAirThinLayerSurfaceInv[i][j][k]->SetType(dielectric_dielectric);
              opCrystalToAirThinLayerSurfaceInv[i][j][k]->SetFinish(ground);
              opCrystalToAirThinLayerSurfaceInv[i][j][k]->SetModel(unified);
              opCrystalToAirThinLayerSurfaceInv[i][j][k]->SetSigmaAlpha(latsigmaalpha);
              opCrystalToAirThinLayerSurfaceInv[i][j][k]->SetMaterialPropertiesTable(crystalDepolished_surf); //this sets the surface roughness
              new G4LogicalBorderSurface(Surfname.str().c_str(),lateral_phys[i][j][k],crystal_phys[i][j],opCrystalToAirThinLayerSurfaceInv[i][j][k]);
          }
      }
      //4. create the volumes of esr-air, the real structure of the matrix
      // also create the esr surfaces      
      double dim_x_esr[4];
      double dim_y_esr[4];
      //shifts are the same, except for a very peculiar case in the 4 corners
      double shift_x_esr[4] = {0,(fCrystal_x/2.0 + airgap + (airThinThickness/2.0 - airgap)/2.0),0,-(fCrystal_x/2.0 + airgap + (airThinThickness/2.0 - airgap)/2.0)};
      double shift_y_esr[4] = {(fCrystal_y/2.0 + airgap + (airThinThickness/2.0 - airgap)/2.0),0,-(fCrystal_y/2.0 + airgap + (airThinThickness/2.0 - airgap)/2.0),0};
      
      //FIXME HORRIBLE IMPLEMENTATION!
      //thickness of the esr boxes never changes, once the airgap and airThinThickness are decided by the user
      //given the k=0,1,2,3 disposition (see depolished above), thickness is dim_x for 1 and 3, dim_y for 0 and 2
      dim_y_esr[0] = airThinThickness/2.0 - airgap;
      dim_x_esr[1] = airThinThickness/2.0 - airgap;
      dim_y_esr[2] = airThinThickness/2.0 - airgap;
      dim_x_esr[3] = airThinThickness/2.0 - airgap;
      
      //now the other relevant dimension is instead divided in more cases
      
      //and the edges. the esr edges of the matrix (the 4 big foils closing the external part of the matrix) have no gaps
      //so we need to make them long as the airThinLayer_box if the volumes are on the edge of the matrix
      // of course it is not needed for cases when the dimensions is already with no gap
      if((esrgapx == 0) && (esrgapy == 0))  // no gap at all, we choose one direction to take the full airThinLayer_box width, the other just in touch, since they will merge anyway
      {
          // we choose y as the long direction, so the volumes taking the entire lenght are 1 and 3
          dim_y_esr[1] = fAirThinLayerBox_y;
          dim_y_esr[3] = fAirThinLayerBox_y;
          // the other volumes take instead the full lenght minus the thickness of the other 2 perpendicular volumes, with no esr gap
          dim_x_esr[0] = fAirThinLayerBox_x - 2.0*(airThinThickness/2.0 - airgap);
          dim_x_esr[2] = fAirThinLayerBox_x - 2.0*(airThinThickness/2.0 - airgap);
          //so here no need to change anything for the edges 
      }
      else if(esrgapx > 0 && esrgapy == 0) // x direction with esrgap, y without 
      {
          // y is the long direction, so the volumes taking the entire lenght are 1 and 3
          dim_y_esr[1] = fAirThinLayerBox_y;
          dim_y_esr[3] = fAirThinLayerBox_y;
          // the other volumes instead have the full lenght minus 2*x_thickness - 2*esrgap
          dim_x_esr[0] = fAirThinLayerBox_x - 2.0*(airThinThickness/2.0 - airgap) - 2.0*(esrgapx);
          dim_x_esr[2] = fAirThinLayerBox_x - 2.0*(airThinThickness/2.0 - airgap) - 2.0*(esrgapx);
          //here instead the x dimensions will need to be adjusted only if we are on the edge
          if(j==0) //first row of crystals, the x dim will need to have no gap if k = 2
              dim_x_esr[2] = fAirThinLayerBox_x - 2.0*(airThinThickness/2.0 - airgap);
          if(j==(nCrystalsY-1))//last row of crystals, the x dim will need to have no gap if k = 0
              dim_x_esr[0] = fAirThinLayerBox_x - 2.0*(airThinThickness/2.0 - airgap);
          
      }
      else if(esrgapx == 0 && esrgapy > 0)// y direction with esrgap, x without 
      {
          // y is the short direction, so the volumes 1 and 3 have the full lenght minus 2*y_thickness - 2*esrgap
          dim_y_esr[1] = fAirThinLayerBox_y- 2.0*(airThinThickness/2.0 - airgap) - 2.0*(esrgapy);
          dim_y_esr[3] = fAirThinLayerBox_y- 2.0*(airThinThickness/2.0 - airgap) - 2.0*(esrgapy);
          // the other volumes have full lenght
          dim_x_esr[0] = fAirThinLayerBox_x;
          dim_x_esr[2] = fAirThinLayerBox_x;
          //here instead the y dimensions will need to be adjusted only if we are on the edge
          if(i==0) //first row of crystals, the x dim will need to have no gap if k = 3
              dim_y_esr[3] = fAirThinLayerBox_y - 2.0*(airThinThickness/2.0 - airgap);
          if(i==(nCrystalsX-1))//last row of crystals, the x dim will need to have no gap if k = 1
              dim_y_esr[1] = fAirThinLayerBox_y - 2.0*(airThinThickness/2.0 - airgap);
      }
      else if(esrgapx > 0 && esrgapy > 0)
      {
          //both directions have esrgap
          dim_y_esr[1] = fAirThinLayerBox_y- 2.0*(airThinThickness/2.0 - airgap) - 2.0*(esrgapy);
          dim_y_esr[3] = fAirThinLayerBox_y- 2.0*(airThinThickness/2.0 - airgap) - 2.0*(esrgapy);
          // the other volumes have full lenght
          dim_x_esr[0] = fAirThinLayerBox_x- 2.0*(airThinThickness/2.0 - airgap) - 2.0*(esrgapx);
          dim_x_esr[2] = fAirThinLayerBox_x- 2.0*(airThinThickness/2.0 - airgap) - 2.0*(esrgapx);
          //here instead the x AND y dimensions will need to be adjusted only if we are on the edge
          //again, we choose y to take the full lenght in case of double adjustment
          if(i==0) //first row of crystals, the x dim will need to have no gap if k = 3
          {
              dim_y_esr[3] = fAirThinLayerBox_y;
              //corners are very special. we need to add a thickness equal to the esrthickness and a shift to align them properly - only for horizontal volumes
              if(j== 0) 
              {
                  dim_x_esr[2] = fAirThinLayerBox_x - 1.0*(airThinThickness/2.0 - airgap);
                  shift_x_esr[2] += (airThinThickness/2.0 - airgap)/2.0;
              }
              if(j== nCrystalsY-1) 
              {
                  dim_x_esr[0] = fAirThinLayerBox_x - 1.0*(airThinThickness/2.0 - airgap); 
                  shift_x_esr[0] += (airThinThickness/2.0 - airgap)/2.0;
              }
          }
          if(i==(nCrystalsX-1))//last row of crystals, the x dim will need to have no gap if k = 1
          {
              dim_y_esr[1] = fAirThinLayerBox_y;
              if(j==0) 
              {
                  dim_x_esr[2] = fAirThinLayerBox_x - 1.0*(airThinThickness/2.0 - airgap);
                  shift_x_esr[2] += -(airThinThickness/2.0 - airgap)/2.0;
              }
              if(j==(nCrystalsY-1)) 
              {    
                  dim_x_esr[0] = fAirThinLayerBox_x - 1.0*(airThinThickness/2.0 - airgap);
                  shift_x_esr[0] += -(airThinThickness/2.0 - airgap)/2.0;
              }
          }
          if(j== 0 && i!= 0 && i!= (nCrystalsX-1))             dim_x_esr[2] = fAirThinLayerBox_x;
          if(j== nCrystalsY-1&& i!= 0 && i!= (nCrystalsX-1))   dim_x_esr[0] = fAirThinLayerBox_x;
      }
        
      
      for(int k = 0 ; k < 4 ; k++)
      {
          name.str("");
          name << "EsrBox_" << i << "_" << j << "_" << k;
          esr_box[i][j][k]  = new G4Box(name.str().c_str(),dim_x_esr[k]/2.0,dim_y_esr[k]/2.0,fAirThinLayerBox_z/2.0);
//           G4cout << name.str() << G4endl;
          //logical volume
          esr_log[i][j][k]  = new G4LogicalVolume(esr_box[i][j][k],airThinLayer,name.str().c_str(),0,0,0);
          G4VisAttributes* esrboxVisulizationAttribute = new G4VisAttributes(G4Colour(0.0,0.0,1.0)); //blue
          esr_log[i][j][k]->SetVisAttributes(esrboxVisulizationAttribute); // we also set here the visualization colors
          //airThinLayer_log[i][j]->SetVisAttributes (G4VisAttributes::GetInvisible());
          //and we place the box in space 
          esr_phys[i][j][k] = new G4PVPlacement(0,G4ThreeVector(shift_x_esr[k],shift_y_esr[k],0),esr_log[i][j][k],name.str().c_str(),airThinLayer_log[i][j],false,0);
//           G4cout << name.str() << " Position" << "\t" << i*fAirThinLayerBox_x - matrixShiftX  << "\t" << j*fAirThinLayerBox_y - matrixShiftY << "\t" << matrixShiftZ << G4endl;
        
          if(lateralEsr)
          {
              std::stringstream Surfname;
              Surfname << "opEsrToAirThinLayerSurface_" << i << "_" << j << "_" << k; 
              opEsrToAirThinLayerSurface[i][j][k] = new G4OpticalSurface(Surfname.str().c_str());
              opEsrToAirThinLayerSurface[i][j][k]->SetType(dielectric_metal);
              opEsrToAirThinLayerSurface[i][j][k]->SetFinish(polished);
              opEsrToAirThinLayerSurface[i][j][k]->SetModel(unified);
//               opEsrToAirThinLayerSurface[i][j][k]->SetSigmaAlpha(latsigmaalpha);
              opEsrToAirThinLayerSurface[i][j][k]->SetMaterialPropertiesTable(ESR_surf); //this sets the surface roughness
              new G4LogicalBorderSurface(Surfname.str().c_str(),airThinLayer_phys[i][j],esr_phys[i][j][k],opEsrToAirThinLayerSurface[i][j][k]);
//               G4cout << Surfname.str() << G4endl;
              
              Surfname << "_Inv";
              //also the inverse direction needs to be declared, for the same surface between the same two volumes. What is wrong with G4 developers?
              opEsrToAirThinLayerSurfaceInv[i][j][k] = new G4OpticalSurface(Surfname.str().c_str());
              opEsrToAirThinLayerSurfaceInv[i][j][k]->SetType(dielectric_metal);
              opEsrToAirThinLayerSurfaceInv[i][j][k]->SetFinish(polished);
              opEsrToAirThinLayerSurfaceInv[i][j][k]->SetModel(unified);
//               opEsrToAirThinLayerSurface[i][j][k]->SetSigmaAlpha(latsigmaalpha);
              opEsrToAirThinLayerSurfaceInv[i][j][k]->SetMaterialPropertiesTable(ESR_surf); //this sets the surface roughness
              new G4LogicalBorderSurface(Surfname.str().c_str(),esr_phys[i][j][k],airThinLayer_phys[i][j],opEsrToAirThinLayerSurfaceInv[i][j][k]);
              
              //we need a surface towards the world, but only for the edge esr boxes.
              //same as before to find them
              std::vector<int> indexEsr;
              if(i==0) indexEsr.push_back(3);
              if(j==0) indexEsr.push_back(2);
              if(i==(nCrystalsX-1)) indexEsr.push_back(1);
              if(j==(nCrystalsY-1)) indexEsr.push_back(0);
                  
              for(int t = 0; t < indexEsr.size(); t++) // first column, the k=3 esr volumes need to have interface with world
              {
                  std::stringstream SurfToWorldName;
                  SurfToWorldName << "opAirThinLayerToWorldSurface_" << i << "_" << j << "_" << indexEsr[t]; 
                  opAirThinLayerToWorldSurface[i][j][indexEsr[t]] = new G4OpticalSurface(SurfToWorldName.str().c_str());
                  opAirThinLayerToWorldSurface[i][j][indexEsr[t]]->SetType(dielectric_metal);
                  opAirThinLayerToWorldSurface[i][j][indexEsr[t]]->SetFinish(polished);
                  opAirThinLayerToWorldSurface[i][j][indexEsr[t]]->SetModel(unified);
                  opAirThinLayerToWorldSurface[i][j][indexEsr[t]]->SetMaterialPropertiesTable(ESR_surf);
                  new G4LogicalBorderSurface(SurfToWorldName.str().c_str(),esr_phys[i][j][indexEsr[t]],expHall_phys,opAirThinLayerToWorldSurface[i][j][indexEsr[t]]);
                  
                  //nothing is outside of matrix, so in principle we don't need to define the inverse surface. But hei, you never know...
                  SurfToWorldName << "_Inv";
                  opAirThinLayerToWorldSurfaceInv[i][j][indexEsr[t]] = new G4OpticalSurface(SurfToWorldName.str().c_str());
                  opAirThinLayerToWorldSurfaceInv[i][j][indexEsr[t]]->SetType(dielectric_metal);
                  opAirThinLayerToWorldSurfaceInv[i][j][indexEsr[t]]->SetFinish(polished);
                  opAirThinLayerToWorldSurfaceInv[i][j][indexEsr[t]]->SetModel(unified);
                  opAirThinLayerToWorldSurfaceInv[i][j][indexEsr[t]]->SetMaterialPropertiesTable(ESR_surf);
                  new G4LogicalBorderSurface(SurfToWorldName.str().c_str(),expHall_phys,esr_phys[i][j][indexEsr[t]],opAirThinLayerToWorldSurfaceInv[i][j][indexEsr[t]]);
                  
                  
              }
          }
      }    
    } 
  }
  
  //volumes in front of the matrix
  // we need to pass the one in contact with the matrix to the realdepolished optical surface
  G4VPhysicalVolume* volumeInFront;
  bool volumeInFrontIsSet = false;
  
  if(fGreaseFrontCryToGlass_z != 0)
  {
    //grease front - between crystal and glass
    G4Box* greaseFrontCryToGlass_box = new G4Box("GreaseFrontCryToGlass",fGreaseFrontCryToGlass_x/2.0,fGreaseFrontCryToGlass_y/2.0,fGreaseFrontCryToGlass_z/2.0);
    G4cout << "GreaseFrontCryToGlass dimensions = " << "\t" << fGreaseFrontCryToGlass_x << "\t" << fGreaseFrontCryToGlass_y << "\t" << fGreaseFrontCryToGlass_z << G4endl;
    G4LogicalVolume* greaseFrontCryToGlass_log = new G4LogicalVolume(greaseFrontCryToGlass_box,Grease,"GreaseFrontCryToGlass",0,0,0);
    G4VPhysicalVolume* greaseFrontCryToGlass_phys = new G4PVPlacement(0,G4ThreeVector(pGreaseFrontCryToGlass_x,pGreaseFrontCryToGlass_y,pGreaseFrontCryToGlass_z),greaseFrontCryToGlass_log,"GreaseFrontCryToGlass",expHall_log,false,fCheckOverlaps);
    G4VisAttributes* GreaseFrontCryToGlassVisulizationAttribute = new G4VisAttributes(G4Colour(0.0,0.0,1.0)); //blue
    greaseFrontCryToGlass_log->SetVisAttributes(GreaseFrontCryToGlassVisulizationAttribute); // we also set here the visualization colors
    
    //assign this volume to the volume in front, then set the flag to true
    volumeInFront = greaseFrontCryToGlass_phys;
    volumeInFrontIsSet = true;
  }
  
  if(fGlassFront_z != 0)
  {
    //glass front
    G4Box* glassFront_box = new G4Box("GlassFront",fGlassFront_x/2.0,fGlassFront_y/2.0,fGlassFront_z/2.0);
    G4LogicalVolume* glassFront_log = new G4LogicalVolume(glassFront_box,Fused_silica,"GlassFront",0,0,0);
    G4VPhysicalVolume* glassFront_phys = new G4PVPlacement(0,G4ThreeVector(pGlassFront_x,pGlassFront_y,pGlassFront_z),glassFront_log,"GlassFront",expHall_log,false,fCheckOverlaps);
    G4VisAttributes* GlassFrontVisulizationAttribute = new G4VisAttributes(G4Colour(1.0,0.0,1.0)); //magenta
    glassFront_log->SetVisAttributes(GlassFrontVisulizationAttribute); // we also set here the visualization colors
    
    //assign this volume to the volume in front, then set the flag to true
    if(!volumeInFrontIsSet)
    {
      volumeInFront = glassFront_phys;
      volumeInFrontIsSet = true;
    }
    
  }
  
  if(fGreaseFrontGlassToMPPC_z != 0)
  {
    //grease front - between glass and mppc
    G4Box* greaseFrontGlassToMPPC_box = new G4Box("GreaseFrontGlassToMPPC",fGreaseFrontGlassToMPPC_x/2.0,fGreaseFrontGlassToMPPC_x/2.0,fGreaseFrontGlassToMPPC_z/2.0);
    G4LogicalVolume* greaseFrontGlassToMPPC_log = new G4LogicalVolume(greaseFrontGlassToMPPC_box,Grease,"GreaseFrontGlassToMPPC",0,0,0);
    G4VPhysicalVolume* greaseFrontGlassToMPPC_phys = new G4PVPlacement(0,G4ThreeVector(pGreaseFrontGlassToMPPC_x,pGreaseFrontGlassToMPPC_y,pGreaseFrontGlassToMPPC_z),greaseFrontGlassToMPPC_log,"GreaseFrontGlassToMPPC",expHall_log,false,fCheckOverlaps);
    G4VisAttributes* GreaseFrontGlassToMPPCVisulizationAttribute = new G4VisAttributes(G4Colour(0.0,0.0,1.0)); //blue
    greaseFrontGlassToMPPC_log->SetVisAttributes(GreaseFrontGlassToMPPCVisulizationAttribute); // we also set here the visualization colors
    //assign this volume to the volume in front, then set the flag to true
    if(!volumeInFrontIsSet)
    {
      volumeInFront = greaseFrontGlassToMPPC_phys;
      volumeInFrontIsSet = true;
    }
    
  }
  
  //if all the 3 layers between crystals and mppcs are removed, put an air layer taking grease front cry to glass but using air or air_thin as material
  if(fGreaseFrontCryToGlass_z == 0 && fGlassFront_z == 0 && fGreaseFrontGlassToMPPC_z == 0)
  {
    G4Box* greaseFrontGlassToMPPC_box = new G4Box("AirGapCrystalMPPC",fGreaseFrontGlassToMPPC_x/2.0,fGreaseFrontGlassToMPPC_y/2.0,0.01/2.0);
    G4LogicalVolume* greaseFrontGlassToMPPC_log = new G4LogicalVolume(greaseFrontGlassToMPPC_box,airThinLayer,"AirGapCrystalMPPC",0,0,0);
    G4VPhysicalVolume* greaseFrontGlassToMPPC_phys = new G4PVPlacement(0,G4ThreeVector(pGreaseFrontGlassToMPPC_x,pGreaseFrontGlassToMPPC_y,pGreaseFrontGlassToMPPC_z),greaseFrontGlassToMPPC_log,"AirGapCrystalMPPC",expHall_log,false,fCheckOverlaps);
    G4VisAttributes* GreaseFrontGlassToMPPCVisulizationAttribute = new G4VisAttributes(G4Colour(0.0,0.0,1.0)); //blue
    greaseFrontGlassToMPPC_log->SetVisAttributes(GreaseFrontGlassToMPPCVisulizationAttribute); // we also set here the visualization colors
    if(!volumeInFrontIsSet)
    {
      volumeInFront = greaseFrontGlassToMPPC_phys;
      volumeInFrontIsSet = true;
    }
  }
  
  if(fEpoxy_z != 0)
  {
    //epoxy layer
    G4Box* epoxy_box = new G4Box("Epoxy",fEpoxy_x/2.0,fEpoxy_y/2.0,fEpoxy_z/2.0);
    G4LogicalVolume* epoxy_log = new G4LogicalVolume(epoxy_box,Epoxy,"Epoxy",0,0,0);
    G4VPhysicalVolume* epoxy_phys = new G4PVPlacement(0,G4ThreeVector(pEpoxy_x,pEpoxy_y,pEpoxy_z),epoxy_log,"Epoxy",expHall_log,false,fCheckOverlaps);
    G4VisAttributes* EpoxyVisualizationAttribute = new G4VisAttributes(G4Colour(1.0,0.0,0.0)); //red
    epoxy_log->SetVisAttributes(EpoxyVisualizationAttribute); // we also set here the visualization colors
  }
  
  //mppc array
  G4Box* MPPCArray_box = new G4Box("MPPCArray",fMPPCArray_x/2.0,fMPPCArray_y/2.0,fMPPCArray_z/2.0);
  G4LogicalVolume* MPPCArray_log = new G4LogicalVolume(MPPCArray_box,Silicio,"MPPCArray",0,0,0);
  G4VPhysicalVolume* MPPCArray_phys = new G4PVPlacement(0,G4ThreeVector(pMPPCArray_x,pMPPCArray_y,pMPPCArray_z),MPPCArray_log,"MPPCArray",expHall_log,false,fCheckOverlaps);
  G4VisAttributes* MPPCArrayVisualizationAttribute = new G4VisAttributes(G4Colour(1.0,0.0,0.0)); //green
  MPPCArray_log->SetVisAttributes(MPPCArrayVisualizationAttribute); // we also set here the visualization colors
  //output position 
  G4cout << "MPPC array position" << G4endl;
  G4cout << pMPPCArray_x << "\t" << pMPPCArray_y << "\t" << pMPPCArray_z << G4endl;
  
  //detectors
  G4Box*** detector_box						= new G4Box** [nDetectorsX];
  for(int i = 0 ; i < nDetectorsX ; i++) detector_box[i]	= new G4Box* [nDetectorsY];
  G4LogicalVolume*** detector_log				= new G4LogicalVolume** [nDetectorsX];
  for(int i = 0 ; i < nDetectorsX ; i++) detector_log[i]	= new G4LogicalVolume* [nDetectorsY];
  G4VPhysicalVolume*** detector_phys				= new G4VPhysicalVolume** [nDetectorsX];
  for(int i = 0 ; i < nDetectorsX ; i++) detector_phys[i]	= new G4VPhysicalVolume* [nDetectorsY];
  
//   G4Box* detector_box[nDetectorsX][nDetectorsY];
//   G4LogicalVolume* detector_log[nDetectorsX][nDetectorsY]; 
//   G4VPhysicalVolume* detector_phys[nDetectorsX][nDetectorsY];
  G4int detectorNum = 0;
  //all the 16 mppc detectors
  for(G4int i = 0 ; i < nDetectorsX ; i++)
  {
    for(G4int j = 0 ; j < nDetectorsY ; j++)
    {
      std::stringstream name;
      name << "Detector_" << i << "_" << j; 
      G4cout << name.str() << "\t";
      //box volume
      detector_box[i][j]  = new G4Box(name.str().c_str(),fSingleMPPC_x/2.0,fSingleMPPC_y/2.0,fSingleMPPC_z/2.0);
      //logical volume
      detector_log[i][j]  =  new G4LogicalVolume(detector_box[i][j],SilicioMPPC,name.str().c_str(),0,0,0);
      G4VisAttributes* DetectorVisualizationAttribute = new G4VisAttributes(G4Colour(1.0,0.0,0.0)); //red
      detector_log[i][j]->SetVisAttributes(DetectorVisualizationAttribute); // we also set here the visualization colors
      //and we place the box in space 
      name.str("");
      name << detectorNum;
      G4cout << i*fSingleMPPCBlock_x - mppcShiftX << "\t" << j*fSingleMPPCBlock_y - mppcShiftY << "\t" << mppcShiftZ << G4endl;
      detector_phys[i][j] = new G4PVPlacement(0,G4ThreeVector(i*fSingleMPPCBlock_x - mppcShiftX ,j*fSingleMPPCBlock_y - mppcShiftY,mppcShiftZ),detector_log[i][j],name.str().c_str(),MPPCArray_log,false,0);
      detectorNum++;
    }
  }
  
  //volumes on the back of the matrix
  // we need to pass the one in contact with the matrix to the realdepolished optical surface
  G4VPhysicalVolume* volumeOnTheBack;
  bool volumeOnTheBackIsSet = false;
  
  if(fGreaseBackCryToGlass_z != 0)
  {
    //grease back - between crystal and glass
    G4Box* greaseBackCryToGlass_box = new G4Box("GreaseBackCryToGlass",fGreaseBackCryToGlass_x/2.0,fGreaseBackCryToGlass_y/2.0,fGreaseBackCryToGlass_z/2.0);
    //G4LogicalVolume* greaseBackCryToGlass_log = new G4LogicalVolume(greaseBackCryToGlass_box,Grease,"GreaseBackCryToGlass",0,0,0); //material is grease
    G4LogicalVolume* greaseBackCryToGlass_log = new G4LogicalVolume(greaseBackCryToGlass_box,Grease,"GreaseBackCryToGlass",0,0,0); //FIXME material is air_thin
    G4VPhysicalVolume* greaseBackCryToGlass_phys = new G4PVPlacement(0,G4ThreeVector(pGreaseBackCryToGlass_x,pGreaseBackCryToGlass_y,pGreaseBackCryToGlass_z),greaseBackCryToGlass_log,"GreaseBackCryToGlass",expHall_log,false,fCheckOverlaps);
    G4VisAttributes* GreaseBackCryToGlassVisulizationAttribute = new G4VisAttributes(G4Colour(0.0,0.0,1.0)); //blue
    greaseBackCryToGlass_log->SetVisAttributes(GreaseBackCryToGlassVisulizationAttribute); // we also set here the visualization colors
    
    volumeOnTheBack = greaseBackCryToGlass_phys;
    volumeOnTheBackIsSet = true;
    
  }
  
  if(fGlassBack_z != 0)
  {
    //glass back
    G4Box* glassBack_box = new G4Box("GlassBack",fGlassBack_x/2.0,fGlassBack_y/2.0,fGlassBack_z/2.0);
    G4LogicalVolume* glassBack_log = new G4LogicalVolume(glassBack_box,Fused_silica,"GlassBack",0,0,0); //material is air_thin
    //G4LogicalVolume* glassBack_log = new G4LogicalVolume(glassBack_box,Fused_silica,"GlassBack",0,0,0); //material is glass
    G4VPhysicalVolume* glassBack_phys = new G4PVPlacement(0,G4ThreeVector(pGlassBack_x,pGlassBack_y,pGlassBack_z),glassBack_log,"GlassBack",expHall_log,false,fCheckOverlaps);
    G4VisAttributes* GlassBackVisulizationAttribute = new G4VisAttributes(G4Colour(0.0,1.0,0.0)); //green
    glassBack_log->SetVisAttributes(GlassBackVisulizationAttribute); // we also set here the visualization colors
    if(!volumeOnTheBackIsSet)
    {
      volumeOnTheBack = glassBack_phys;
      volumeOnTheBackIsSet = true;
    }
    
  }
  
  //air layer between back glass and vikuiti
  G4Box* airBack_box = new G4Box("AirBack",fAirBack_x/2.0,fAirBack_y/2.0,fAirBack_z/2.0);
  G4LogicalVolume* airBack_log = new G4LogicalVolume(airBack_box,airThinLayer,"AirBack",0,0,0);
  G4VPhysicalVolume* airBack_phys = new G4PVPlacement(0,G4ThreeVector(pAirBack_x,pAirBack_y,pAirBack_z),airBack_log,"AirBack",expHall_log,false,fCheckOverlaps);
  G4VisAttributes* AirBackVisualizationAttribute = new G4VisAttributes(G4Colour(1.0,0.0,1.0)); //green
  airBack_log->SetVisAttributes(AirBackVisualizationAttribute); // we also set here the visualization colors
  if(!volumeOnTheBackIsSet)
  {
    volumeOnTheBack = airBack_phys;
    volumeOnTheBackIsSet = true;
  }
  
  
  
  //the vikuiti on the back. Volume defined as air, it will have esr surfaces to the airgap and to the world
  //since it's defined as air, is back esr is set to false then it will just act as air
  G4Box* fakeAirBack_box = new G4Box("BackEsrReflector",fFakeAirBack_x/2.0,fFakeAirBack_y/2.0,fFakeAirBack_z/2.0);
  G4LogicalVolume* fakeAirBack_log = new G4LogicalVolume(fakeAirBack_box,airThinLayer,"BackEsrReflector",0,0,0);
  G4VPhysicalVolume* fakeAirBack_phys = new G4PVPlacement(0,G4ThreeVector(pFakeAirBack_x,pFakeAirBack_y,pFakeAirBack_z),fakeAirBack_log,"BackEsrReflector",expHall_log,false,fCheckOverlaps);
  G4VisAttributes* FakeAirBackVisualizationAttribute = new G4VisAttributes(G4Colour(1.0,0.0,1.0)); //magenta
  fakeAirBack_log->SetVisAttributes(FakeAirBackVisualizationAttribute); // we also set here the visualization colors
  
  
  
  //-------------------------------------------------------------------//
  //                                                                   //
  //                            SURFACES                               //
  //                                                                   //
  //-------------------------------------------------------------------//
//   
//   // define a surface between each crystal and its own air layer
//   if(latdepolished)
//   {
// //     G4OpticalSurface* opCrystalToAirThinLayerSurface[nCrystalsX][nCrystalsY];
//     G4OpticalSurface*** opCrystalToAirThinLayerSurface				= new G4OpticalSurface** [nCrystalsX];
//     for(int i = 0 ; i < nCrystalsX ; i++) opCrystalToAirThinLayerSurface[i]	= new G4OpticalSurface* [nCrystalsY];
//     
//     for(G4int i = 0 ; i < nCrystalsX ; i++)
//     {
//       for(G4int j = 0 ; j < nCrystalsY ; j++)
//       {
// 	std::stringstream name;
// 	name << "CrystalToAirThinLayerSurface_" << i << "_" << j; 
// 	opCrystalToAirThinLayerSurface[i][j] = new G4OpticalSurface(name.str().c_str());
// 	opCrystalToAirThinLayerSurface[i][j]->SetType(dielectric_dielectric);
// 	opCrystalToAirThinLayerSurface[i][j]->SetFinish(ground);
// 	opCrystalToAirThinLayerSurface[i][j]->SetModel(unified);
// 	opCrystalToAirThinLayerSurface[i][j]->SetSigmaAlpha(latsigmaalpha);
// 	opCrystalToAirThinLayerSurface[i][j]->SetMaterialPropertiesTable(crystalDepolished_surf); //this sets the surface roughness
// 	new G4LogicalBorderSurface(name.str().c_str(),crystal_phys[i][j],airThinLayer_phys[i][j],opCrystalToAirThinLayerSurface[i][j]);
//       }
//     }
//   }
//   
//   
  // now define a depolished surface also between the front and back of the crystals and the neighbour volumes. 
  // It if useful if we want to use depolishing to characterize the imperfection of a polished surface. 
  // In this case in fact even the front and back faces of the crystal, although nominally polished, will be simulated as
  // slightly unpolished
  
  //DEBUG check - what are the front and back volumes?
  G4cout << "Volume in front = " << volumeInFront->GetName() << G4endl;
  G4cout << "Volume on the back = " << volumeOnTheBack->GetName() << G4endl;
  
  if(realdepolished)
  {
    
    G4OpticalSurface*** CrystalFrontRealSurface				= new G4OpticalSurface** [nCrystalsX];
    G4OpticalSurface*** CrystalFrontRealSurfaceInv				= new G4OpticalSurface** [nCrystalsX];
    for(int i = 0 ; i < nCrystalsX ; i++) 
    {
      CrystalFrontRealSurface[i]	= new G4OpticalSurface* [nCrystalsY];
      CrystalFrontRealSurfaceInv[i]	= new G4OpticalSurface* [nCrystalsY];
    }
    
    G4OpticalSurface*** CrystalBackRealSurface				= new G4OpticalSurface** [nCrystalsX];
    G4OpticalSurface*** CrystalBackRealSurfaceInv				= new G4OpticalSurface** [nCrystalsX];
    for(int i = 0 ; i < nCrystalsX ; i++) 
    {
      CrystalBackRealSurface[i]	= new G4OpticalSurface* [nCrystalsY];
      CrystalBackRealSurfaceInv[i]	= new G4OpticalSurface* [nCrystalsY];
    }
    
//     G4OpticalSurface* CrystalFrontRealSurface[nCrystalsX][nCrystalsY];
//     G4OpticalSurface* CrystalBackRealSurface[nCrystalsX][nCrystalsY];
    for(G4int i = 0 ; i < nCrystalsX ; i++)
    {
      for(G4int j = 0 ; j < nCrystalsY ; j++)
      {
	//front 
	std::stringstream name;
	name << "CrystalFrontRealSurface_" << i << "_" << j; 
	CrystalFrontRealSurface[i][j] = new G4OpticalSurface(name.str().c_str());
	CrystalFrontRealSurface[i][j]->SetType(dielectric_dielectric);
	CrystalFrontRealSurface[i][j]->SetFinish(ground);
	CrystalFrontRealSurface[i][j]->SetModel(unified);
	CrystalFrontRealSurface[i][j]->SetSigmaAlpha(realsigmaalpha);
	CrystalFrontRealSurface[i][j]->SetMaterialPropertiesTable(crystalReal_surf); //this sets the surface roughness
	new G4LogicalBorderSurface(name.str().c_str(),crystal_phys[i][j],volumeInFront,CrystalFrontRealSurface[i][j]);
        //and inverse...
        name << "_Inv"; 
        CrystalFrontRealSurfaceInv[i][j] = new G4OpticalSurface(name.str().c_str());
	CrystalFrontRealSurfaceInv[i][j]->SetType(dielectric_dielectric);
	CrystalFrontRealSurfaceInv[i][j]->SetFinish(ground);
	CrystalFrontRealSurfaceInv[i][j]->SetModel(unified);
	CrystalFrontRealSurfaceInv[i][j]->SetSigmaAlpha(realsigmaalpha);
	CrystalFrontRealSurfaceInv[i][j]->SetMaterialPropertiesTable(crystalReal_surf); //this sets the surface roughness
	new G4LogicalBorderSurface(name.str().c_str(),volumeInFront,crystal_phys[i][j],CrystalFrontRealSurfaceInv[i][j]);
	
	//back
	name.str("");
	name << "CrystalBackRealSurface_" << i << "_" << j; 
	CrystalBackRealSurface[i][j] = new G4OpticalSurface(name.str().c_str());
	CrystalBackRealSurface[i][j]->SetType(dielectric_dielectric);
	CrystalBackRealSurface[i][j]->SetFinish(ground);
	CrystalBackRealSurface[i][j]->SetModel(unified);
	CrystalBackRealSurface[i][j]->SetSigmaAlpha(realsigmaalpha);
	CrystalBackRealSurface[i][j]->SetMaterialPropertiesTable(crystalReal_surf); //this sets the surface roughness
	new G4LogicalBorderSurface(name.str().c_str(),crystal_phys[i][j],volumeOnTheBack,CrystalBackRealSurface[i][j]);
        //and inverse..
        name << "_Inv"; 
        CrystalBackRealSurfaceInv[i][j] = new G4OpticalSurface(name.str().c_str());
	CrystalBackRealSurfaceInv[i][j]->SetType(dielectric_dielectric);
	CrystalBackRealSurfaceInv[i][j]->SetFinish(ground);
	CrystalBackRealSurfaceInv[i][j]->SetModel(unified);
	CrystalBackRealSurfaceInv[i][j]->SetSigmaAlpha(realsigmaalpha);
	CrystalBackRealSurfaceInv[i][j]->SetMaterialPropertiesTable(crystalReal_surf); //this sets the surface roughness
	new G4LogicalBorderSurface(name.str().c_str(),volumeOnTheBack,crystal_phys[i][j],CrystalBackRealSurfaceInv[i][j]);
      }      
    }    
  }

  //also the esrboxes, if they are there, should have a final esr surface with the volumes in front and on the back
  if(lateralEsr)
  {
      
      G4OpticalSurface**** opEsrToFrontSurface	= new G4OpticalSurface*** [nCrystalsX];
      G4OpticalSurface**** opEsrToFrontSurfaceInv	= new G4OpticalSurface*** [nCrystalsX];
      for(int i = 0 ; i < nCrystalsX ; i++) 
      {
          opEsrToFrontSurface[i]	= new G4OpticalSurface** [nCrystalsY];
          opEsrToFrontSurfaceInv[i]	= new G4OpticalSurface** [nCrystalsY];
          for(int j = 0 ; j < nCrystalsY ; j++)
          {
              opEsrToFrontSurface[i][j]     = new G4OpticalSurface* [4];
              opEsrToFrontSurfaceInv[i][j]     = new G4OpticalSurface* [4];
          }
      }
      G4OpticalSurface**** opEsrToBackSurface	= new G4OpticalSurface*** [nCrystalsX];
      G4OpticalSurface**** opEsrToBackSurfaceInv	= new G4OpticalSurface*** [nCrystalsX];
      for(int i = 0 ; i < nCrystalsX ; i++) 
      {
          opEsrToBackSurface[i]	= new G4OpticalSurface** [nCrystalsY];
          opEsrToBackSurfaceInv[i]	= new G4OpticalSurface** [nCrystalsY];
          for(int j = 0 ; j < nCrystalsY ; j++)
          {
              opEsrToBackSurface[i][j]     = new G4OpticalSurface* [4];
              opEsrToBackSurfaceInv[i][j]     = new G4OpticalSurface* [4];
          }
      }
      
      
      for(G4int i = 0 ; i < nCrystalsX ; i++)
      {
          for(G4int j = 0 ; j < nCrystalsY ; j++)
          {
              for(G4int k = 0 ; k < 4 ; k++)
              {
                  std::stringstream EsrToFrontname;
                  EsrToFrontname << "opEsrToFrontSurface" << i << "_" << j << "_" << k; 
                  opEsrToFrontSurface[i][j][k] = new G4OpticalSurface(EsrToFrontname.str().c_str());
                  opEsrToFrontSurface[i][j][k]->SetType(dielectric_metal);
                  opEsrToFrontSurface[i][j][k]->SetFinish(polished);
                  opEsrToFrontSurface[i][j][k]->SetModel(unified);
                  opEsrToFrontSurface[i][j][k]->SetMaterialPropertiesTable(ESR_surf); 
                  new G4LogicalBorderSurface(EsrToFrontname.str().c_str(),volumeInFront,esr_phys[i][j][k],opEsrToFrontSurface[i][j][k]);
                  //and inverse...
                  EsrToFrontname << "_Inv";
                  opEsrToFrontSurfaceInv[i][j][k] = new G4OpticalSurface(EsrToFrontname.str().c_str());
                  opEsrToFrontSurfaceInv[i][j][k]->SetType(dielectric_metal);
                  opEsrToFrontSurfaceInv[i][j][k]->SetFinish(polished);
                  opEsrToFrontSurfaceInv[i][j][k]->SetModel(unified);
                  opEsrToFrontSurfaceInv[i][j][k]->SetMaterialPropertiesTable(ESR_surf); 
                  new G4LogicalBorderSurface(EsrToFrontname.str().c_str(),esr_phys[i][j][k],volumeInFront,opEsrToFrontSurfaceInv[i][j][k]);
                  
                  
                  std::stringstream EsrToBackname;
                  EsrToBackname << "opEsrToBackSurface" << i << "_" << j << "_" << k; 
                  opEsrToBackSurface[i][j][k] = new G4OpticalSurface(EsrToBackname.str().c_str());
                  opEsrToBackSurface[i][j][k]->SetType(dielectric_metal);
                  opEsrToBackSurface[i][j][k]->SetFinish(polished);
                  opEsrToBackSurface[i][j][k]->SetModel(unified);
                  opEsrToBackSurface[i][j][k]->SetMaterialPropertiesTable(ESR_surf); 
                  new G4LogicalBorderSurface(EsrToBackname.str().c_str(),volumeOnTheBack,esr_phys[i][j][k],opEsrToBackSurface[i][j][k]);
                  //and inverse...
                  EsrToBackname << "_Inv";
                  opEsrToBackSurfaceInv[i][j][k] = new G4OpticalSurface(EsrToBackname.str().c_str());
                  opEsrToBackSurfaceInv[i][j][k]->SetType(dielectric_metal);
                  opEsrToBackSurfaceInv[i][j][k]->SetFinish(polished);
                  opEsrToBackSurfaceInv[i][j][k]->SetModel(unified);
                  opEsrToBackSurfaceInv[i][j][k]->SetMaterialPropertiesTable(ESR_surf); 
                  new G4LogicalBorderSurface(EsrToBackname.str().c_str(),esr_phys[i][j][k],volumeOnTheBack,opEsrToBackSurfaceInv[i][j][k]);
                  
              }
          }
      }
  }
  

  if(backEsr)
  {
    //back air to back esr 
    G4OpticalSurface* opAirBackToEsr = new G4OpticalSurface("AirBackToEsr");
    opAirBackToEsr->SetType(dielectric_metal);
    opAirBackToEsr->SetFinish(polished);
    opAirBackToEsr->SetModel(unified);
    opAirBackToEsr->SetMaterialPropertiesTable(ESR_surf);
    new G4LogicalBorderSurface("AirBackToEsr",airBack_phys,fakeAirBack_phys,opAirBackToEsr);
    //and the inverse...
    G4OpticalSurface* opEsrToAirBack = new G4OpticalSurface("EsrToAirBack");
    opEsrToAirBack->SetType(dielectric_metal);
    opEsrToAirBack->SetFinish(polished);
    opEsrToAirBack->SetModel(unified);
    opEsrToAirBack->SetMaterialPropertiesTable(ESR_surf);
    new G4LogicalBorderSurface("EsrToAirBack",fakeAirBack_phys,airBack_phys,opEsrToAirBack);
    
    
    //esrBack to the world
    G4OpticalSurface* opEsrToWorld = new G4OpticalSurface("opEsrToWorld");
    opEsrToWorld->SetType(dielectric_metal);
    opEsrToWorld->SetFinish(polished);
    opEsrToWorld->SetModel(unified);
    opEsrToWorld->SetMaterialPropertiesTable(ESR_surf);
    new G4LogicalBorderSurface("opEsrToWorld",fakeAirBack_phys,expHall_phys,opEsrToWorld);
    //and the inverse... (not needed but for completness...)
    G4OpticalSurface* opWorldToEsr = new G4OpticalSurface("opWorldToEsr");
    opWorldToEsr->SetType(dielectric_metal);
    opWorldToEsr->SetFinish(polished);
    opWorldToEsr->SetModel(unified);
    opWorldToEsr->SetMaterialPropertiesTable(ESR_surf);
    new G4LogicalBorderSurface("opWorldToEsr",expHall_phys,fakeAirBack_phys,opWorldToEsr);
    
    
    
  }
  
  //always return the physical World
  return expHall_phys;
}