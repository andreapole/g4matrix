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

// Make this appear first!
#include "G4Timer.hh"

#include "g4matrixRunAction.hh"

#include "G4Run.hh"
#include "TROOT.h"
// #include "B4Analysis.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include <TTree.h>
#include <TFile.h>
#include "CreateTree.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4matrixRunAction::g4matrixRunAction(/*float* aFloat*/)
 : G4UserRunAction(),
   fTimer(0)
   //,pFloat(aFloat)
{
  fTimer = new G4Timer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4matrixRunAction::~g4matrixRunAction()
{
  delete fTimer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4matrixRunAction::BeginOfRunAction(const G4Run* aRun)
{
  
  gROOT->ProcessLine("#include <vector>"); //this is needed otherwise ROOT will complain about not knowing what a std::vector is...
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  fTimer->Start();
  
  CreateTree::Instance()->Run = aRun->GetRunID();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4matrixRunAction::EndOfRunAction(const G4Run* aRun)
{
  
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent() << " " << *fTimer << G4endl;

}