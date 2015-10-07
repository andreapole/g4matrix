#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "globals.hh"

class CreateTree
{
  private:

  TTree*              ftree;
  TString             fname;

  Bool_t              HITS;
  Bool_t              ABSORPTIONS;

  static const Int_t  MaxNum = 2000000;
  static const Int_t  MaxNumPro = 100;
  
  int nCrystalsX, nCrystalsY, nDetectorsX, nDetectorsY;

  public:

  CreateTree(TString name,int x, int y, int z, int k);
  ~CreateTree();  
  
  
  TTree*              GetTree() const { return ftree; };
  TString             GetName() const { return fname;};
  Int_t               Fill() { return this->GetTree()->Fill(); };
  Bool_t              Write();
  void                Clear();
  static CreateTree*  Instance() { return fInstance; };
  static CreateTree*  fInstance;

  Bool_t              Hits() const { return this->HITS; };
  Bool_t              Absorptions() const { return this->ABSORPTIONS; };

  long int            Seed;
  Int_t               Run;
  Int_t               Event;
  Int_t               NumOptPhotons;
  Int_t               NumCherenkovPhotons;
  Float_t             totalEnergyDeposited;
  
  
  Short_t*            DetectorHit;          
//   int            DetectorHit[16];
  
  
  std::vector<float>*  CryEnergyDeposited;   
  std::vector<float>** pCryEnergyDeposited;
  
  std::vector<float>*  PosXEnDep; 
  std::vector<float>** pPosXEnDep;
  std::vector<float>*  PosYEnDep; 
  std::vector<float>** pPosYEnDep;
  std::vector<float>*  PosZEnDep; 
  std::vector<float>** pPosZEnDep;
  
  std::vector<float> PositionX;
  std::vector<float> *pPositionX;
  std::vector<float> PositionY;
  std::vector<float> *pPositionY;
  std::vector<float> PositionZ;
  std::vector<float> *pPositionZ;
  
  std::vector<float> PreMomentumX;
  std::vector<float> *pPreMomentumX;
  std::vector<float> PreMomentumY;
  std::vector<float> *pPreMomentumY;
  std::vector<float> PreMomentumZ;
  std::vector<float> *pPreMomentumZ;
  
  std::vector<float> PostMomentumX;
  std::vector<float> *pPostMomentumX;
  std::vector<float> PostMomentumY;
  std::vector<float> *pPostMomentumY;
  std::vector<float> PostMomentumZ;
  std::vector<float> *pPostMomentumZ;
  
  std::vector<float> GlobalTime;
  std::vector<float> *pGlobalTime;
  
  std::vector<int> PhotonType;
  std::vector<int> *pPhotonType;
  
  std::vector<float> PhotonEnergy;
  std::vector<float> *pPhotonEnergy;
  
 
};
