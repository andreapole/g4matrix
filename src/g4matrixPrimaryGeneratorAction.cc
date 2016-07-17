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

#include "g4matrixPrimaryGeneratorAction.hh"
// #include "g4matrixPrimaryGeneratorMessenger.hh"

#include "Randomize.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4matrixPrimaryGeneratorAction::g4matrixPrimaryGeneratorAction(ConfigFile& config)
 : G4VUserPrimaryGeneratorAction(), 
   fParticleGun(0)
//    ,fConfig(config)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  //create a messenger for this class
  //fGunMessenger = new g4matrixPrimaryGeneratorMessenger(this);

  //default kinematic
  //
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("gamma");

  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleTime(0.0*ns);
  
  //source 
  //x position
  sourcex = config.read<double>("sourcex");
  G4cout << "Gamma source x position [mm]: " << sourcex << G4endl;
  //y position of source
  sourcey = config.read<double>("sourcey");
  G4cout << "Gamma source y position [mm]: " << sourcey << G4endl;
  //x position of source
  distance = config.read<double>("distance");
  G4cout << "Distance of source from back ESR [mm]: " << distance << G4endl;
  
  //retrieve dimensions of the other elements and calculate the position of the source
  G4double crystalz = config.read<double>("crystalz");
  G4double greaseBack = config.read<double>("greaseBack");
  G4double glassBack = config.read<double>("glassBack");
  G4double airBack = config.read<double>("airBack");
  //multiply for units [mm]
  crystalz   = crystalz*mm;
  greaseBack = greaseBack*mm;
  glassBack  = glassBack*mm;
  airBack    = airBack*mm;
  G4double fakeAir = 0.1*mm;  //fixed to 0.1, it has no physical meaning in our simulation
  G4double sourcez = -(distance + (crystalz/2.0) + greaseBack + glassBack + airBack + fakeAir);
  G4cout << "Gamma source z position [mm]: " << sourcez << G4endl;
  //energy of gammas
  energy = config.read<double>("energy");
  G4cout << "Energy of gamma source [KeV]: " << energy << G4endl;
  //gamma direction
  direction = config.read<int>("direction");
  if(direction == 0)
  {
    G4cout << "Gammas shoot parallel to the z axis" << G4endl;
  }
  else
  {
    G4cout << "Gammas shoot randomly towards the matrix" << G4endl;
  }
  //find the x and y of matrix
  crystalx = config.read<double>("crystalx");
  crystaly = config.read<double>("crystaly");
  ncrystalx = config.read<int>("ncrystalx");
  ncrystaly = config.read<int>("ncrystaly");
  esrThickness = config.read<double>("esrThickness");

  //FIXME this assumes pointlike source. wouldn't it be better to have a 1mm diameter sphere?
  fParticleGun->SetParticlePosition(G4ThreeVector(sourcex,sourcey,sourcez));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(energy*keV);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4matrixPrimaryGeneratorAction::~g4matrixPrimaryGeneratorAction()
{
  delete fParticleGun;
//   delete fGunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4matrixPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  
  G4double halfDiagonal =  sqrt(pow((crystalx + esrThickness) * ncrystalx,2.0) + pow( (crystaly + esrThickness) * ncrystaly ,2.0)) / 2.0;
  G4double angleLimit = atan(halfDiagonal / distance);
  //theta = (G4UniformRand() * 2.0*angleLimit) - angleLimit; //WRONG!!!! this would make cos(theta) uniform, not sin(theta)d(theta)
  
  //find the limit for acos
  double acosMin = cos(angleLimit);
  //so acos will have to be generated uniformely between acosMin and +1
  
  double randomNum =  G4UniformRand()*(1.0 - acosMin)  + (acosMin);
  theta = acos(randomNum); 
  
  phi = G4UniformRand() * 2.0 * CLHEP::pi;
  
  //G4cout << "Theta = " << theta << G4endl;
  //G4cout << "Phi = " << phi << G4endl;
  
  if(direction == 0)
  {
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  }
  else
  {
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(sin(theta)*sin(phi),sin(theta)*cos(phi),cos(theta) ));
  }
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// void g4matrixPrimaryGeneratorAction::SetOptPhotonPolar()
// {
//  G4double angle = G4UniformRand() * 360.0* deg;
//  SetOptPhotonPolar(angle);
// }
// 
// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 
// void g4matrixPrimaryGeneratorAction::SetOptPhotonPolar(G4double angle)
// {
//  if (fParticleGun->GetParticleDefinition()->GetParticleName()!="opticalphoton")
//    {
//      G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
//                "the particleGun is not an opticalphoton" << G4endl;
//      return;
//    }
// 
//  G4ThreeVector normal (1., 0., 0.);
//  G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
//  G4ThreeVector product = normal.cross(kphoton);
//  G4double modul2       = product*product;
//  
//  G4ThreeVector e_perpend (0., 0., 1.);
//  if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
//  G4ThreeVector e_paralle    = e_perpend.cross(kphoton);
//  
//  G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
//  fParticleGun->SetParticlePolarization(polar);
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
