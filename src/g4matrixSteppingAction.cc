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
     G4cout << "Number of Scintillation Photons in previous event: "
            << fScintillationCounter << G4endl;
     G4cout << "Number of Cerenkov Photons in previous event: "
            << fCerenkovCounter << G4endl;
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
    //BEGIN of debug part (flood maps not symmetric issue)
//     G4OpBoundaryProcessStatus boundaryStatus=Undefined;
//     static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;
//     
//     //find the boundary process only once
//     if(!boundary)
//     {
//       G4ProcessManager* pm = track->GetDefinition()->GetProcessManager();
//       G4int nprocesses = pm->GetProcessListLength();
//       G4ProcessVector* pv = pm->GetProcessList();
//       G4int i;
//       for( i=0;i<nprocesses;i++)
//       {
// 	if((*pv)[i]->GetProcessName()=="OpBoundary")
// 	{
// 	  boundary = (G4OpBoundaryProcess*)(*pv)[i];
// 	  break;
// 	}
//       }
//     }
//     boundaryStatus = boundary->GetStatus();
//     
//     if((materialName == "AirThinLayer") && (boundaryStatus == Transmission))
//     {
//       G4ThreeVector TransmissionPosition = track->GetStep()->GetPostStepPoint()->GetPosition(); //get the position vector
//       CreateTree::Instance()->TransmissionX.push_back(TransmissionPosition.getX());
//       CreateTree::Instance()->TransmissionY.push_back(TransmissionPosition.getY());
//       CreateTree::Instance()->TransmissionZ.push_back(TransmissionPosition.getZ());   
//     }
//     
    //END of debug part (flood maps not symmetric issue) 
    
    //BEGIN of test part        
    G4OpBoundaryProcessStatus boundaryStatus=Undefined;
    static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;
    
    //find the boundary process only once
    if(!boundary)
    {
      G4ProcessManager* pm = track->GetDefinition()->GetProcessManager();
      G4int nprocesses = pm->GetProcessListLength();
      G4ProcessVector* pv = pm->GetProcessList();
      G4int i;
      for( i=0;i<nprocesses;i++)
      {
	if((*pv)[i]->GetProcessName()=="OpBoundary")
	{
	  boundary = (G4OpBoundaryProcess*)(*pv)[i];
	  break;
	}
      }
    }
    boundaryStatus = boundary->GetStatus();
    
    if(/*(PreMaterialName == "Epoxy") && */(PostMaterialName == "SilicioMPPC") && (boundaryStatus == FresnelRefraction)) //whatever enters a detector volume
    {
      //G4cout << PreMaterialName << " " << PostMaterialName << G4endl;
      G4String detectorName = step->GetPostStepPoint()->GetPhysicalVolume()->GetName();
      int numb;
      std::istringstream ( detectorName ) >> numb;
      
      //take into account quantum efficiency
      G4double rand = G4UniformRand();
      //record the photon, if QE allows it
      if(rand < quantumEff) //absorb the optical photon, save the relevant data
      {
	CreateTree::Instance()->DetectorHit[numb]++;
	
	G4ThreeVector OnDetectorPosition = track->GetStep()->GetPostStepPoint()->GetPosition(); //get the position vector
	G4ThreeVector PreOnDetectorMomentum = track->GetStep()->GetPreStepPoint()->GetMomentumDirection(); //get the momentum, unit vector
	G4ThreeVector PostOnDetectorMomentum = track->GetStep()->GetPostStepPoint()->GetMomentumDirection(); //get the momentum, unit vector
	G4double globalTime = track->GetGlobalTime();
	
	CreateTree::Instance()->PositionX.push_back(OnDetectorPosition.getX());
	CreateTree::Instance()->PositionY.push_back(OnDetectorPosition.getY());
	CreateTree::Instance()->PositionZ.push_back(OnDetectorPosition.getZ());
	
	CreateTree::Instance()->PreMomentumX.push_back(PreOnDetectorMomentum.getX()/*/CLHEP::nm*/);
	CreateTree::Instance()->PreMomentumY.push_back(PreOnDetectorMomentum.getY()/*/CLHEP::nm*/);
	CreateTree::Instance()->PreMomentumZ.push_back(PreOnDetectorMomentum.getZ()/*/CLHEP::nm*/);
	
	CreateTree::Instance()->PostMomentumX.push_back(PostOnDetectorMomentum.getX()/*/CLHEP::nm*/);
	CreateTree::Instance()->PostMomentumY.push_back(PostOnDetectorMomentum.getY()/*/CLHEP::nm*/);
	CreateTree::Instance()->PostMomentumZ.push_back(PostOnDetectorMomentum.getZ()/*/CLHEP::nm*/);
	
	//BEGIN of DEBUG for negative PostMomentumZ
// 	if(PostOnDetectorMomentum.getZ() < 0)
// 	{
// 	  G4cout << "----------------------------------------------------------------------------------------------------------------" << G4endl;
// 	  G4cout << "PreMaterialName = " << PreMaterialName << G4endl;
// 	  G4cout << "PostMaterialName = " << PostMaterialName << G4endl;
// 	  G4cout << "Position = " << OnDetectorPosition.getX() << " " << OnDetectorPosition.getY() << " " << OnDetectorPosition.getZ() << G4endl;
// 	  G4cout << "----------------------------------------------------------------------------------------------------------------" << G4endl;
// 	}
	//END of DEBUG for negative PostMomentumZ
	
	
	//save the process that created the photon
	if(track->GetCreatorProcess()->GetProcessName() == "Scintillation")
	  CreateTree::Instance()->PhotonType.push_back(0);
	else if(track->GetCreatorProcess()->GetProcessName() == "Cerenkov")
	  CreateTree::Instance()->PhotonType.push_back(1);
	else
	  CreateTree::Instance()->PhotonType.push_back(2);
	
	CreateTree::Instance()->GlobalTime.push_back(globalTime/CLHEP::ns);  
	CreateTree::Instance()->PhotonEnergy.push_back(track->GetDynamicParticle()->GetTotalEnergy()/CLHEP::eV);
  
	
      }
      //kill the photon
      track->SetTrackStatus(fStopAndKill); 
    }
    //END of test part
    
    
    //BEGIN of standard method
//     if(track->GetTrackStatus()==fStopAndKill) //if it just died here
//     {
//       if(PostMaterialName == "SilicioMPPC" )
//       {
// 	//G4cout << PreMaterialName << " " << PostMaterialName << G4endl;
// 	G4String detectorName = step->GetPostStepPoint()->GetPhysicalVolume()->GetName();
// 	int numb;
// 	std::istringstream ( detectorName ) >> numb;
// 	
// 	//take into account quantum efficiency
// 	G4double rand = G4UniformRand();
// 	
// 	if(rand < quantumEff) //absorb the optical photon, save the relevant data
// 	{
// 	  CreateTree::Instance()->DetectorHit[numb]++;
// 	  
// 	  G4ThreeVector OnDetectorPosition = track->GetStep()->GetPostStepPoint()->GetPosition(); //get the position vector
// 	  G4ThreeVector OnDetectorMomentum = track->GetStep()->GetPostStepPoint()->GetMomentumDirection(); //get the momentum, unit vector
// 	  G4double globalTime = track->GetGlobalTime();
// 	  
// 	  CreateTree::Instance()->PositionX.push_back(OnDetectorPosition.getX());
//           CreateTree::Instance()->PositionY.push_back(OnDetectorPosition.getY());
// 	  CreateTree::Instance()->PositionZ.push_back(OnDetectorPosition.getZ());
// 	  
// 	  CreateTree::Instance()->MomentumX.push_back(OnDetectorMomentum.getX()/*/CLHEP::nm*/);
//           CreateTree::Instance()->MomentumY.push_back(OnDetectorMomentum.getY()/*/CLHEP::nm*/);
// 	  CreateTree::Instance()->MomentumZ.push_back(OnDetectorMomentum.getZ()/*/CLHEP::nm*/);
// 	  
// 	  CreateTree::Instance()->GlobalTime.push_back(globalTime/CLHEP::ns);
// 	}
//       } 
//     }
    //END of standard method
  }
  else //there's only gammas and electrons. for them, we want to know where they left energy (opticalphoton don't really leave any)
  {
    G4double edep;
    edep = step->GetTotalEnergyDeposit()/CLHEP::MeV; //check if there was an energy deposition
    if(edep != 0) //if there was en dep, save the data
    {
      if(materialName == "LYSO") //if the electron is interacting with the detector, it does a huge number of cherenkov
      {
	
	//get the deposition process name
// 	G4OpBoundaryProcessStatus boundaryStatus=Undefined;
// 	static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;
	
	//find the boundary process only once
// 	if(!boundary)
// 	{
// 	  G4ProcessManager* pm = track->GetDefinition()->GetProcessManager();
// 	  G4int nprocesses = pm->GetProcessListLength();
// 	  G4ProcessVector* pv = pm->GetProcessList();
// 	  G4int i;
	  
// 	  for( i=0;i<nprocesses;i++)
// 	  {
// 	    if((*pv)[i]->GetProcessName()=="OpBoundary")
// 	    G4cout << (*pv)[i]->GetProcessType() << " " << (*pv)[i]->GetProcessName() << G4endl;
// 	    {
// 	      boundary = (G4OpBoundaryProcess*)(*pv)[i];
// 	      break;
// 	    }
// 	  }
// 	}
// 	boundaryStatus = boundary->GetStatus();
	
	
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
