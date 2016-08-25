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
// $Id: g4matrixSteppingAction.cc 71007 2013-06-09 16:14:59Z maire $
//
/// \file g4matrixSteppingAction.cc
/// \brief Implementation of the g4matrixSteppingAction class

#include "g4matrixSteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
// #include <sstream>
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "g4matrixEventAction.hh"
// #include "B4Analysis.hh"
#include "CreateTree.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


g4matrixSteppingAction::g4matrixSteppingAction(
                      ConfigFile& config)
: G4UserSteppingAction()
    //fEventAction(eventAction)
{ 
  fScintillationCounter = 0;
  fCerenkovCounter      = 0;
  fEventNumber = -1;
  fOpticalPhotonsStopped = 0;
  fGammaStopped = 0;
  fElectronStopped = 0;
  
  quantumEff = config.read<double>("quantumEff");
  
  
  //ch[] = {0};
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4matrixSteppingAction::~g4matrixSteppingAction()
{ ; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4matrixSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4int eventNumber = G4RunManager::GetRunManager()->
                                              GetCurrentEvent()->GetEventID();

  if (eventNumber != fEventNumber) {
     /*G4cout << "Number of Scintillation Photons in previous event: "
            << fScintillationCounter << G4endl;
     G4cout << "Number of Cerenkov Photons in previous event: "
            << fCerenkovCounter << G4endl;*/
     fEventNumber = eventNumber;
     fScintillationCounter = 0;
     fCerenkovCounter = 0;
  }
  
  
	
  G4Track* track = step->GetTrack();
  G4String ParticleName = track->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();  
  G4String materialName = step->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetName();
  
  G4VPhysicalVolume* thePrePV  = step->GetPreStepPoint() ->GetPhysicalVolume();
  G4VPhysicalVolume* thePostPV = step->GetPostStepPoint()->GetPhysicalVolume();
                     
  G4String PreMaterialName ;
  G4String PostMaterialName ;
  
  if(thePrePV) PreMaterialName = thePrePV->GetLogicalVolume()->GetMaterial()->GetName();
  if(thePostPV) PostMaterialName = thePostPV->GetLogicalVolume()->GetMaterial()->GetName();
			       
			       
  //getting information from the particles
  
  //for opticalphoton, we want to know where they stopped 
  if (ParticleName == "opticalphoton") //if it's an opticalphoton
  {
    //kill the photon
    track->SetTrackStatus(fStopAndKill);
  }
  else //there's only gammas and electrons. for them, we want to know where they left energy (opticalphoton don't really leave any)
  {
    G4double edep = 0;
    edep = step->GetTotalEnergyDeposit()/CLHEP::MeV; //check if there was an energy deposition
    /*
    G4ThreeVector positionVectorOut = track->GetStep()->GetPostStepPoint()->GetPosition(); //get the position vector
    G4cout << ParticleName << " ID=" << track->GetTrackID() << " ParentID=" << track->GetParentID() << 
    " - En.Dep " << edep << " - " ;
    G4cout << "(" << positionVectorOut.getX() << "," << positionVectorOut.getY() << "," << positionVectorOut.getZ() << ") - ";
    G4cout << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
    */
    if(track->GetTrackID()==1 && ParticleName == "gamma" && materialName == "LYSO")
        {
          G4String crystalName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
          int numb;
          std::istringstream ( crystalName ) >> numb;

          if(step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "compt")
          {
            G4ThreeVector positionVectorCompt = track->GetStep()->GetPostStepPoint()->GetPosition(); //get the position vector
            CreateTree::Instance()->PosComptX[numb].push_back(positionVectorCompt.getX());
            CreateTree::Instance()->PosComptY[numb].push_back(positionVectorCompt.getY());
            CreateTree::Instance()->PosComptZ[numb].push_back(positionVectorCompt.getZ());
            G4double comptTime = track->GetGlobalTime(); //get time
            CreateTree::Instance()->TimeCompt[numb].push_back(comptTime/CLHEP::ns);;
            std::cout << "compt" << std::endl;
          }
          else if(step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "phot")
          {
            G4ThreeVector positionVectorPhot = track->GetStep()->GetPostStepPoint()->GetPosition(); //get the position vector
            CreateTree::Instance()->PosPhotX[numb].push_back(positionVectorPhot.getX());
            CreateTree::Instance()->PosPhotY[numb].push_back(positionVectorPhot.getY());
            CreateTree::Instance()->PosPhotZ[numb].push_back(positionVectorPhot.getZ());
            G4double photTime = track->GetGlobalTime(); //get time
            CreateTree::Instance()->TimePhot[numb].push_back(photTime/CLHEP::ns);   
            std::cout << "phot" << std::endl; 
          }
        }
    if(edep != 0) //if there was en dep, save the data
    {
      if(materialName == "LYSO") //if the electron is interacting with the detector, it does a huge number of cherenkov
      {
	
      	//add total energy deposited
      	CreateTree::Instance()->totalEnergyDeposited += edep;
      	//take crystal name
      	G4String crystalName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
      	//convert it to int
      	int numb;
      	std::istringstream ( crystalName ) >> numb;
      	//add energy deposited in this step to this crystal
      	CreateTree::Instance()->CryEnergyDeposited[numb].push_back(edep);
      	//add position of deposited energy
      	G4ThreeVector positionVector = track->GetStep()->GetPreStepPoint()->GetPosition(); //get the position vector
      	//call the methods to fill the position vectors
      	CreateTree::Instance()->PosXEnDep[numb].push_back(positionVector.getX());
      	CreateTree::Instance()->PosYEnDep[numb].push_back(positionVector.getY());
      	CreateTree::Instance()->PosZEnDep[numb].push_back(positionVector.getZ());
        //add global time
        G4double globalTime = track->GetGlobalTime();
        CreateTree::Instance()->CryGlobalTime[numb].push_back(globalTime/CLHEP::ns); 
      
      }
    }
  }

  
  if (ParticleName == "opticalphoton") return;
  const std::vector<const G4Track*>* secondaries =
                                            step->GetSecondaryInCurrentStep();
  if (secondaries->size()>0) {
     for(unsigned int i=0; i<secondaries->size(); ++i) {
        if (secondaries->at(i)->GetParentID()>0) {
           if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition()
               == G4OpticalPhoton::OpticalPhotonDefinition()){
              if (secondaries->at(i)->GetCreatorProcess()->GetProcessName()
               == "Scintillation")
	              {
              		CreateTree::Instance()->NumOptPhotons++;
              		fScintillationCounter++;
	              }
                if (secondaries->at(i)->GetCreatorProcess()->GetProcessName()
                == "Cerenkov")
	              {
              		fCerenkovCounter++;
              		CreateTree::Instance()->NumCherenkovPhotons++;
	              }
            }
        }
     }
  }
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
