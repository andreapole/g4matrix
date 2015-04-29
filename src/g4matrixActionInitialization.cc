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
// $Id: g4matrixActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file g4matrixActionInitialization.cc
/// \brief Implementation of the g4matrixActionInitialization class

#include "g4matrixActionInitialization.hh"
#include "g4matrixPrimaryGeneratorAction.hh"
#include "g4matrixDetectorConstruction.hh"
#include "g4matrixRunAction.hh"
#include "g4matrixEventAction.hh"
#include "g4matrixSteppingAction.hh"
#include "g4matrixStackingAction.hh"
#include "g4matrixSteppingVerbose.hh"
#include "ConfigFile.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4matrixActionInitialization::g4matrixActionInitialization(ConfigFile& config)
 : G4VUserActionInitialization(),
   fConfig(config)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4matrixActionInitialization::~g4matrixActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4matrixActionInitialization::BuildForMaster() const
{
  SetUserAction(new g4matrixRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4matrixActionInitialization::Build() const
{
  
  
  SetUserAction(new g4matrixPrimaryGeneratorAction(fConfig));
  g4matrixRunAction* runAction = new g4matrixRunAction();
  SetUserAction(runAction);
  g4matrixEventAction* eventAction = new g4matrixEventAction(runAction);
  SetUserAction(eventAction);
  SetUserAction(new g4matrixSteppingAction(fConfig));
  SetUserAction(new g4matrixStackingAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VSteppingVerbose*
               g4matrixActionInitialization::InitializeSteppingVerbose() const
{
  return new g4matrixSteppingVerbose();
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
