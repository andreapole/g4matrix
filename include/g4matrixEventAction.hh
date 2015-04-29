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
// $Id: g4matrixEventAction.hh 75215 2013-10-29 16:07:06Z gcosmo $
// 
/// \file g4matrixEventAction.hh
/// \brief Definition of the g4matrixEventAction class

#ifndef g4matrixEventAction_h
#define g4matrixEventAction_h 1

#include "G4UserEventAction.hh"
#include "g4matrixRunAction.hh"
#include "globals.hh"


/// Event action class
///
/// It defines data members to hold the energy deposit and track lengths
/// of charged particles in Absober and Gap layers:
/// - fEnergyAbs, fEnergyGap, fTrackLAbs, fTrackLGap
/// which are collected step by step via the functions
/// - AddAbs(), AddGap()

//make it dummy

class g4matrixEventAction : public G4UserEventAction
{
  public:
    g4matrixEventAction(g4matrixRunAction* runAction);
    virtual ~g4matrixEventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);
   
  private:
    g4matrixRunAction* fRunAction;
};

// // inline functions
// inline void g4matrixEventAction::AddEnergy(G4double e)
// {
//   TotalEnergyDeposited += e;
// }
// 
// inline void g4matrixEventAction::AddCryEnergy(G4int i, float e)
// {
//   CryEnergyDeposited[i].push_back(e);
// }
// 
// inline void g4matrixEventAction::AddPosXEnergy(G4int i, float e)
// {
//   PosXEnDep[i].push_back(e);
// }
// inline void g4matrixEventAction::AddPosYEnergy(G4int i, float e)
// {
//   PosYEnDep[i].push_back(e);
// }
// inline void g4matrixEventAction::AddPosZEnergy(G4int i, float e)
// {
//   PosZEnDep[i].push_back(e);
// }
// 
// inline void g4matrixEventAction::AddDetectorHit(G4int i)
// {
//   detectorHit[i]++;
// }


/*
inline void g4matrixEventAction::AddAbs(G4double de, G4double dl) {
  fEnergyAbs += de; 
  fTrackLAbs += dl;
}

inline void g4matrixEventAction::AddGap(G4double de, G4double dl) {
  fEnergyGap += de; 
  fTrackLGap += dl;
}*/
                     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
