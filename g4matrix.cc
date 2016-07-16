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
//
//     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "g4matrixPhysicsList.hh"

#include "g4matrixDetectorConstruction.hh"
#include "g4matrixActionInitialization.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

// #include "ConfigFile.hh"
#include "ConfigFile.h"
#include "CreateTree.hh"

#include <TNamed.h>
#include <TString.h>
#include <time.h>

#include "TRandom1.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " g4matrix [-m macro ] [-u UIsession] [-t nThreads] [-r seed] "
           << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

long int CreateSeed();


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{

  if(argc<2) {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " g4matrix <config_file> [-m macro ] [-r seed] [-o output]" << G4endl;
    G4cerr << "   note: -m -r -o options are optional" << G4endl;
    exit(0);
  }
  
  //----------------------------------------------------------//
  //  Parse the config file                                   //
  //----------------------------------------------------------//
  
  std::cout<<"\n"<<std::endl;
  std::cout<<"###########################################################"<<std::endl;  
  std::cout<<"#                                                         #"<<std::endl;
  std::cout<<"#           New Clear PEM matrix simulation               #"<<std::endl;  
  std::cout<<"#                                                         #"<<std::endl;  
  //std::cout<<"#                                                         #"<<std::endl;  
  //std::cout<<"#                                                         #"<<std::endl;
  //std::cout<<"#                                                         #"<<std::endl;  
  std::cout<<"###########################################################"<<std::endl;
  std::cout<<"\n\n"<<std::endl;
  std::cout<<"=====>   C O N F I G U R A T I O N   <====\n"<<std::endl;
   
  //config file
  G4cout << "Configuration file: '" << argv[1] << "'"<< G4endl;
  ConfigFile config(argv[1]);
  
  //output root file
  std::string filename;
  config.readInto(filename,"output");
  //G4cout << "Writing data to file '" << filename << ".root '..."<< G4endl;

  //run macro
  G4String macro;
  config.readInto(macro,"macro");

  //seed for random generator
  G4long myseed = config.read<long int>("seed");
  
  //type of session
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif
  
  //if passed by command line, override the filename, macro and myseed (the others are not important for us)
  for (int i = 0; i < argc ; i++)
  {
    if(std::string(argv[i]) == std::string("-m")) macro   = argv[i+1];
    else if(std::string(argv[i]) == std::string("-r")) myseed  = atoi(argv[i+1]);
    else if(std::string(argv[i]) == std::string("-o")) filename = argv[i+1];
  }
  
  if(myseed==-1) 
  {
     G4cout<<"Creating random seed..."<<G4endl;   
     myseed=CreateSeed();
  }
  
  G4cout << "Reading macro '" << macro << "' ..." << G4endl;
  G4cout<< "Random seed : "<< myseed <<G4endl;
  G4cout << "Output file '" << filename << ".root '..."<< G4endl;
  
  //read crystal dimensions
  G4double crystalx = config.read<double>("crystalx");
  G4double crystaly = config.read<double>("crystaly");
  G4double crystalz = config.read<double>("crystalz");
  G4cout << "Crystal x length [mm]: " << crystalx << G4endl;
  G4cout << "Crystal y length [mm]: " << crystaly << G4endl;
  G4cout << "Crystal z length [mm]: " << crystalz << G4endl;
  
  G4int ncrystalx = config.read<int>("ncrystalx");
  G4int ncrystaly = config.read<int>("ncrystaly");
  G4cout << "Number crystal in x direction: " << ncrystalx << G4endl;
  G4cout << "Number crystal in y direction: " << ncrystaly << G4endl;
  
  //esr thickness
  G4double esrThickness = config.read<double>("esrThickness");
  G4cout << "ESR thickness [mm]: " << esrThickness << G4endl;

  //esr lateral and back existance
  G4bool lateralEsr = (bool) config.read<int>("lateralEsr");
  G4bool backEsr = (bool) config.read<int>("backEsr");
  if(lateralEsr)
    G4cout << "Crystal(s) with lateral ESR " << G4endl;
  else
    G4cout << "No lateral ESR " << G4endl;
  if(lateralEsr)
    G4cout << "Crystal(s) with back ESR " << G4endl;
  else
    G4cout << "No back ESR " << G4endl;
  
  //grease front from matrix to glass
  G4double greaseFront1 = config.read<double>("greaseFront1");
  G4cout << "Grease layer between matrix and front glass [mm]: " << greaseFront1 << G4endl;
  //glass front thickness
  G4double glassFront = config.read<double>("glassFront");
  G4cout << "Front glass light guide thickness [mm]: " << glassFront << G4endl;
  //grease front from glass to mppc
  G4double greaseFront2 = config.read<double>("greaseFront2");
  G4cout << "Grease layer between front glass and mppc [mm]: " << greaseFront2 << G4endl;
  //epoxy
  G4double epoxy = config.read<double>("epoxy");
  G4cout << "Epoxy layer of mppc [mm]: " << epoxy << G4endl;
  //mppc dimensions
  G4double mppcx = config.read<double>("mppcx");
  G4double mppcy = config.read<double>("mppcy");
  G4double mppcz = config.read<double>("mppcz");
  G4double mppcGap = config.read<double>("mppcGap");
  G4cout << "Mppc x dimension [mm]: " << mppcx << G4endl;
  G4cout << "Mppc y dimension [mm]: " << mppcy << G4endl;
  G4cout << "Mppc z dimension [mm]: " << mppcz << G4endl;
  G4cout << "Gap between MPPC detectors [mm]: " << mppcGap << G4endl;
  G4int nmppcx = config.read<int>("nmppcx");
  G4int nmppcy = config.read<int>("nmppcy");
  G4cout << "Number mppc in x direction: " << nmppcx << G4endl;
  G4cout << "Number mppcs in y direction: " << nmppcy << G4endl;
  
  //grease back from matrix to glass
  G4double greaseBack = config.read<double>("greaseBack");
  G4cout << "Grease layer between matrix and back glass [mm]: " << greaseBack << G4endl;
  //glass back
  G4double glassBack = config.read<double>("glassBack");
  G4cout << "Back glass light guide thickness [mm]: " << glassBack << G4endl;
  //grease back from matrix to glass
  G4double airBack = config.read<double>("airBack");
  G4cout << "Air layer between back glass and esr [mm]: " << airBack << G4endl;
  
  //material etc
  G4double lightyield = config.read<double>("lightyield");
  G4cout << "Scintillator Light Yield [Ph/MeV]: " << lightyield << G4endl;
  // energy resolution
  G4double resolutionScale = config.read<double>("resolutionScale");
  // rise time and decay time
  G4double esrgapx = config.read<double>("esrgapx",0);
  G4double esrgapy = config.read<double>("esrgapy",0);
  G4double airgap = config.read<double>("airgap",0.01);
  
  G4double fastrisetime = config.read<double>("fastrisetime",0);   // can be optional
  G4double fastdecaytime = config.read<double>("fastdecaytime",0); // can be optional
  G4double fastratio = config.read<double>("fastratio",0);         // can be optional
  G4double slowrisetime = config.read<double>("slowrisetime");
  G4double slowdecaytime = config.read<double>("slowdecaytime");
  
  // read and parse the emission spectra
  std::string default_energy = "1.77169,1.77266,1.77558,1.77851,1.78145,1.78539,1.79033,1.7963,1.80231,1.80836,1.81445,1.82058,1.82882,1.83401,1.84553,1.85293,1.86147,1.869,1.87769,1.89308,1.90536,1.92007,1.93039,1.94901,1.95846,1.9668,1.97884,1.99102,2.00088,2.01209,2.02596,2.03617,2.04519,2.0569,2.06611,2.0794,2.09151,2.10239,2.112,2.1231,2.13431,2.14565,2.15566,2.16868,2.18038,2.19519,2.21171,2.2193,2.23619,2.23464,2.24395,2.25806,2.27234,2.28358,2.29493,2.30475,2.31631,2.32463,2.33134,2.33809,2.34487,2.35856,2.36719,2.37939,2.38642,2.40238,2.41134,2.424,2.43312,2.44047,2.44786,2.46278,2.47788,2.48741,2.49317,2.49702,2.50282,2.50865,2.5145,2.52038,2.52432,2.53223,2.5362,2.54619,2.55424,2.56031,2.56437,2.57049,2.57663,2.58487,2.59317,2.59734,2.60571,2.61414,2.61414,2.61837,2.62262,2.62475,2.62902,2.63331,2.63545,2.63976,2.64191,2.64841,2.65493,2.6593,2.66149,2.66588,2.67914,2.67914,2.68136,2.68136,2.68359,2.68805,2.68805,2.68805,2.69477,2.69477,2.69702,2.70153,2.70605,2.71286,2.71742,2.71971,2.722,2.722,2.72429,2.72889,2.72889,2.73351,2.73814,2.74279,2.74512,2.74979,2.75213,2.75447,2.75917,2.75682,2.76389,2.76626,2.76389,2.76626,2.77338,2.77576,2.78533,2.79255,2.79738,2.80223,2.80466,2.80709,2.80953,2.80953,2.81934,2.8218,2.82673,2.83168,2.84164,2.84916,2.85419,2.8643,2.86684,2.87449,2.87705,2.87961,2.88475,2.88733,2.8925,2.89509,2.90028,2.90549,2.90811,2.91073,2.91335,2.91335,2.91335,2.91861,2.92125,2.92125,2.92389,2.92654,2.92654,2.92919,2.92919,2.93185,2.93451,2.93717,2.93985,2.94252,2.9452,2.94789,2.94789,2.94789,2.95058,2.95868,2.96411,2.96955,2.97228,2.97228,2.96955,2.97228,2.97502,2.97776,2.97502,2.9805,2.9805,2.9805,2.98601,2.99154,2.99431,2.99431,2.99708,2.99431,2.99708,3.00544,3.00824,3.00824,3.00824,3.00824,3.01385,3.0223,3.02797,3.03081,3.02797,3.03365,3.03081,3.03081,3.0365,3.03935,3.04221,3.04795,3.04795,3.05083,3.05371,3.05949,3.06239,3.06529,3.0682,3.06529,3.07112,3.0682,3.07696,3.08283,3.0976,3.09464,3.09464,3.10653,3.11252,3.11852,3.12757,3.13668,3.14583,3.15813,3.16741,3.17675,3.20828,3.23719,3.26664,3.28656,3.31351,3.34783,3.38287";
  std::string default_component = "0.011691,0.011691,0.011691,0.0146138,0.0146138,0.0146138,0.011691,0.011691,0.00876827,0.00876827,0.00584551,0.00584551,0.00584551,0.00292276,0.00876827,0.0146138,0.0146138,0.0146138,0.0204593,0.023382,0.0263048,0.0204593,0.0204593,0.023382,0.0292276,0.0321503,0.0350731,0.0379958,0.0379958,0.0379958,0.0350731,0.0379958,0.0409186,0.0438413,0.0526096,0.0584551,0.0643006,0.0730689,0.0730689,0.0818372,0.0906054,0.0964509,0.0993737,0.105219,0.111065,0.122756,0.125678,0.146138,0.146138,0.160752,0.157829,0.163674,0.184134,0.192902,0.20167,0.219207,0.230898,0.242589,0.25428,0.265971,0.274739,0.292276,0.306889,0.315658,0.321503,0.350731,0.368267,0.385804,0.397495,0.415031,0.432568,0.458873,0.482255,0.496868,0.514405,0.529019,0.549478,0.564092,0.581628,0.593319,0.602088,0.616701,0.637161,0.660543,0.681002,0.71023,0.736534,0.756994,0.777453,0.806681,0.844676,0.868058,0.891441,0.9119,0.938205,0.955741,0.984969,1.0142,1.03173,1.05511,1.07557,1.11649,1.13695,1.15741,1.17495,1.19248,1.21002,1.22756,1.27432,1.2977,1.31524,1.32985,1.36785,1.40292,1.39415,1.4,1.41754,1.44092,1.47015,1.48476,1.50814,1.5286,1.54906,1.56952,1.58998,1.61921,1.63967,1.66597,1.68935,1.71566,1.73904,1.76242,1.77996,1.80042,1.8238,1.83549,1.85303,1.8618,1.87933,1.89979,1.91733,1.92902,1.95825,1.98163,2.01378,2.03424,2.0547,2.07808,2.09562,2.11023,2.12484,2.13361,2.15407,2.15699,2.15992,2.16576,2.16868,2.16868,2.16284,2.15699,2.14823,2.13946,2.12484,2.11023,2.08977,2.06639,2.04593,2.02839,2.01086,1.98455,1.96409,1.94948,1.93194,1.91733,1.90271,1.87641,1.86472,1.8501,1.83841,1.82088,1.79749,1.77119,1.75073,1.73027,1.70689,1.68058,1.65428,1.6309,1.60167,1.57244,1.55491,1.53152,1.50522,1.47891,1.45261,1.43215,1.40877,1.38831,1.362,1.33862,1.31232,1.28601,1.27432,1.25678,1.21587,1.19541,1.17203,1.14864,1.12234,1.10772,1.08434,1.06096,1.0142,0.987891,0.967432,0.938205,0.9119,0.879749,0.853445,0.82714,0.786221,0.765762,0.739457,0.716075,0.681002,0.660543,0.637161,0.60501,0.581628,0.552401,0.531942,0.505637,0.485177,0.458873,0.435491,0.412109,0.379958,0.356576,0.336117,0.309812,0.280585,0.25428,0.207516,0.175365,0.157829,0.13737,0.119833,0.0993737,0.0759916,0.0613779,0.0526096,0.0350731,0.0263048,0.011691,0.00876827,0.00876827,0.011691,0.011691,0.011691,0.00876827,0.011691"; 
  
  std::string fastenergy_s,fastcomponent_s,slowenergy_s,slowcomponent_s;
  std::vector <std::string> fastenergy_f,fastcomponent_f,slowenergy_f,slowcomponent_f;
  std::vector<double> fastenergy,fastcomponent,slowenergy,slowcomponent;
  
  fastenergy_s    = config.read<std::string>("fastenergy",default_energy);
  fastcomponent_s = config.read<std::string>("fastcomponent",default_component);
  slowenergy_s    = config.read<std::string>("slowenergy",default_energy);
  slowcomponent_s = config.read<std::string>("slowcomponent",default_component);
  config.split( fastenergy_f, fastenergy_s, "," );
  config.split( fastcomponent_f, fastcomponent_s, "," );
  config.split( slowenergy_f, slowenergy_s, "," );
  config.split( slowcomponent_f, slowcomponent_s, "," );
  for(int i = 0 ; i < fastenergy_f.size() ; i++)
  {
    config.trim(fastenergy_f[i]);
    fastenergy.push_back(atof(fastenergy_f[i].c_str()));
  }
  for(int i = 0 ; i < fastcomponent_f.size() ; i++)
  {
    config.trim(fastcomponent_f[i]);
    fastcomponent.push_back(atof(fastcomponent_f[i].c_str()));
  }
  for(int i = 0 ; i < slowenergy_f.size() ; i++)
  {
    config.trim(slowenergy_f[i]);
    slowenergy.push_back(atof(slowenergy_f[i].c_str()));
  }
  for(int i = 0 ; i < slowcomponent_f.size() ; i++)
  {
    config.trim(slowcomponent_f[i]);
    slowcomponent.push_back(atof(slowcomponent_f[i].c_str()));
  }
  assert( fastenergy.size() == fastcomponent.size() );
  assert( slowenergy.size() == slowcomponent.size() );
  
  std::string latedepo_str;
  
  std::vector <std::string> latedepo_f;
  std::vector <G4bool> LatDepoSideBySide;
  
  latedepo_str    = config.read<std::string>("latdepolishedSideBySide","0,0,0,0");
  config.split( latedepo_f, latedepo_str, "," );
  for(int i = 0 ; i < latedepo_f.size() ; i++)
  {
    config.trim(latedepo_f[i]);
    LatDepoSideBySide.push_back((bool) atoi(latedepo_f[i].c_str()));
  }
  
  //lateral depolishing
//   G4bool latdepolished = (bool) config.read<int>("latdepolished");
  G4double latsurfaceroughness = config.read<double>("latsurfaceroughness");
  G4double latsigmaalpha = config.read<double>("latsigmaalpha");
  G4double latspecularlobe = config.read<double>("latspecularlobe");
  G4double latspecularspike = config.read<double>("latspecularspike");
  G4double latbackscattering = config.read<double>("latbackscattering");
  G4cout << "Crystal surface finish: "; 
  for(int i = 0 ; i < LatDepoSideBySide.size() ; i++)
  {
      G4cout << G4endl;
      G4cout << "--------------------" << G4endl;
      G4cout << "| Side " << i << G4endl;
      G4cout << "--------------------" << G4endl;
      if(LatDepoSideBySide[i])
      {
          
          G4cout << "Lateral depolishing:" << G4endl;
          G4cout << "Sigma Alpha [radiants]: " << latsigmaalpha << G4endl;
          G4cout << "Specular Lobe:  " << latspecularlobe << G4endl;
          G4cout << "Specular Spike: " << latspecularspike << G4endl;
          G4cout << "Back Scattering: " << latbackscattering << G4endl;
          if(latsurfaceroughness != 0)
              G4cout << "Surface roughness [nm]: " << latsurfaceroughness << G4endl; 
      }
      else
      {
          G4cout << "Lateral: perfect polished" << G4endl;
      }
      G4cout << G4endl;
  }
  
  //front and back depolishing (for real polishing state)
  G4bool realdepolished = (bool) config.read<int>("realdepolished");
  G4double realsurfaceroughness = config.read<double>("realsurfaceroughness");
  G4double realsigmaalpha = config.read<double>("realsigmaalpha");
  G4double realspecularlobe = config.read<double>("realspecularlobe");
  G4double realspecularspike = config.read<double>("realspecularspike");
  G4double realbackscattering = config.read<double>("realbackscattering");
  G4cout << "Front and Back crystal surfaces finish: "; 
  if(realdepolished)
  {
    G4cout << "Front and Back depolishing:" << G4endl;
    G4cout << "Sigma Alpha [radiants]: " << realsigmaalpha << G4endl;
    G4cout << "Specular Lobe:  " << realspecularlobe << G4endl;
    G4cout << "Specular Spike: " << realspecularspike << G4endl;
    G4cout << "Back Scattering: " << realbackscattering << G4endl;
    if(realsurfaceroughness != 0)
      G4cout << "Surface roughness [nm]: " << realsurfaceroughness << G4endl; 
  }
  else
  {
    G4cout << " Front and Back: perfect polished" << G4endl;
  }
  
  
  G4double esrTransmittance = config.read<double>("esrTransmittance");
  if(esrTransmittance == -1)
  {
    G4cout << "Using esr transmittance values from measurement" << G4endl;
  }
  else
  {
    G4cout << "Esr transmittance fixed to " << esrTransmittance << " for all wavelenghts" << G4endl;
  }
  
  
  // quantum eff of the detectors
  G4double quantumEff = config.read<double>("quantumEff");
  
  G4cout << "Resolution Scale: " << resolutionScale << G4endl; 
//   G4cout << "here" << G4endl;
  G4cout << "Quantum efficiency: " << quantumEff << G4endl; 
  
  //distance of source from back of the module
  G4double distance = config.read<double>("distance");
  
  
  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  if ( nThreads > 0 ) runManager->SetNumberOfThreads(nThreads);
#else
  G4RunManager * runManager = new G4RunManager;
#endif

  // Seed the random number generator manually
  G4Random::setTheSeed(myseed);

  
  //create output ttree
//   CreateTree* mytree = new CreateTree("StandardTree",ncrystalx,ncrystaly,nmppcx,nmppcy,true,true);
  
  CreateTree* mytree = new CreateTree("StandardTree",ncrystalx,ncrystaly,nmppcx,nmppcy);
  //((CreateTree*)mytree)->SetModuleElements(ncrystalx,ncrystaly,nmppcx,nmppcy);
  //mytree->SetModuleElements(ncrystalx,ncrystaly,nmppcx,nmppcy);
  
  //save the seed in the ttree
  CreateTree::Instance()->Seed = myseed;
  
  //create the detector construction
  g4matrixDetectorConstruction* detector = new g4matrixDetectorConstruction();
  
  //set the parameters of detector
  ((g4matrixDetectorConstruction*)detector)->SetCrystalDimensions(crystalx,crystaly,crystalz);
  ((g4matrixDetectorConstruction*)detector)->SetNumberOfCrystals(ncrystalx,ncrystaly);
  ((g4matrixDetectorConstruction*)detector)->SetThinAirThickness(esrThickness);
  ((g4matrixDetectorConstruction*)detector)->SetLateralEsr(lateralEsr);
  ((g4matrixDetectorConstruction*)detector)->SetBackEsr(backEsr);
  ((g4matrixDetectorConstruction*)detector)->SetGreaseFrontOne(greaseFront1);
  ((g4matrixDetectorConstruction*)detector)->SetGreaseFrontTwo(greaseFront2);
  ((g4matrixDetectorConstruction*)detector)->SetGlassFront(glassFront);
  ((g4matrixDetectorConstruction*)detector)->SetEpoxy(epoxy);
  ((g4matrixDetectorConstruction*)detector)->SetMppcDimensions(mppcx,mppcy,mppcz);
  ((g4matrixDetectorConstruction*)detector)->SetMppcGap(mppcGap);
  ((g4matrixDetectorConstruction*)detector)->SetNumberOfMPPC(nmppcx,nmppcy);
  ((g4matrixDetectorConstruction*)detector)->SetGreaseBack(greaseBack);
  ((g4matrixDetectorConstruction*)detector)->SetGlassBack(glassBack);
  ((g4matrixDetectorConstruction*)detector)->SetAirBack(airBack);
  ((g4matrixDetectorConstruction*)detector)->SetLightYield(lightyield);
  ((g4matrixDetectorConstruction*)detector)->SetEsrGapX(esrgapx);
  ((g4matrixDetectorConstruction*)detector)->SetEsrGapY(esrgapy);
  ((g4matrixDetectorConstruction*)detector)->SetAirGap(airgap);
  
  ((g4matrixDetectorConstruction*)detector)->SetLatDepoSideBySide(LatDepoSideBySide[0],LatDepoSideBySide[1],LatDepoSideBySide[2],LatDepoSideBySide[3]);
  
  ((g4matrixDetectorConstruction*)detector)->SetFastRiseTime(fastrisetime);
  ((g4matrixDetectorConstruction*)detector)->SetFastDecayTime(fastdecaytime);
  ((g4matrixDetectorConstruction*)detector)->SetFastRatio(fastratio);
  ((g4matrixDetectorConstruction*)detector)->SetSlowRiseTime(slowrisetime);
  ((g4matrixDetectorConstruction*)detector)->SetSlowDecayTime(slowdecaytime);
  
  ((g4matrixDetectorConstruction*)detector)->SetFastEnergy(fastenergy);
  ((g4matrixDetectorConstruction*)detector)->SetFastComponent(fastcomponent);
  ((g4matrixDetectorConstruction*)detector)->SetSlowEnergy(slowenergy);
  ((g4matrixDetectorConstruction*)detector)->SetSlowComponent(slowcomponent);
  
  
//   ((g4matrixDetectorConstruction*)detector)->SetLateralDepolished(latdepolished);
  ((g4matrixDetectorConstruction*)detector)->SetLateralSurfaceRoughness(latsurfaceroughness);
  ((g4matrixDetectorConstruction*)detector)->SetLateralSurfaceSigmaAlpha(latsigmaalpha);
  ((g4matrixDetectorConstruction*)detector)->SetLateralSurfaceSpecularLobe(latspecularlobe);
  ((g4matrixDetectorConstruction*)detector)->SetLateralSurfaceSpecularSpike(latspecularspike);
  ((g4matrixDetectorConstruction*)detector)->SetLateralSurfaceBackScattering(latbackscattering);
  
  ((g4matrixDetectorConstruction*)detector)->SetRealDepolished(realdepolished);
  ((g4matrixDetectorConstruction*)detector)->SetRealSurfaceRoughness(realsurfaceroughness);
  ((g4matrixDetectorConstruction*)detector)->SetRealSurfaceSigmaAlpha(realsigmaalpha);
  ((g4matrixDetectorConstruction*)detector)->SetRealSurfaceSpecularLobe(realspecularlobe);
  ((g4matrixDetectorConstruction*)detector)->SetRealSurfaceSpecularSpike(realspecularspike);
  ((g4matrixDetectorConstruction*)detector)->SetRealSurfaceBackScattering(realbackscattering);
  
  
  ((g4matrixDetectorConstruction*)detector)->SetEsrTransmittance(esrTransmittance);
  ((g4matrixDetectorConstruction*)detector)->SetSourceDistance(distance);
  ((g4matrixDetectorConstruction*)detector)->SetResolutionScale(resolutionScale);
  
  // Set mandatory initialization classes
  //
  // Detector construction
  runManager-> SetUserInitialization(detector);
  // Physics list
  runManager-> SetUserInitialization(new g4matrixPhysicsList);
  // User action initialization
  runManager->SetUserInitialization(new g4matrixActionInitialization(config));
  // Initialize G4 kernel
  //
  runManager->Initialize();

#ifdef G4VIS_USE
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer(); 
   
  if ( macro.size() ) {
     // Batch mode
     G4String command = "/control/execute ";
     UImanager->ApplyCommand(command+macro);
  }
  else // Define UI session for interactive mode
  {
#ifdef G4UI_USE
     G4UIExecutive * ui = new G4UIExecutive(argc,argv,session);
#ifdef G4VIS_USE
     UImanager->ApplyCommand("/control/execute vis.mac");
#else
     UImanager->ApplyCommand("/control/execute g4matrix.in");
#endif
     if (ui->IsGUI())
        UImanager->ApplyCommand("/control/execute gui.mac");
     ui->SessionStart();
     delete ui;
#endif
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

  
  
  
#ifdef G4VIS_USE
  delete visManager;
    
#endif
  delete runManager;
  
  //output root file
  std::stringstream outfileName;
  outfileName << filename << ".root";

  TFile* outfile = new TFile(TString(outfileName.str().c_str()),"RECREATE");
  outfile->cd();
  G4cout<<"Writing tree to file "<< outfileName.str() <<" ..."<<G4endl;
  mytree->GetTree()->Write();
  outfile->Write();
  outfile->Close();
  
  
  return 0;
}


long int CreateSeed()
{
  TRandom1 rangen;
  long int ss = time(0);
  std::cout<<"Time : "<<ss<<std::endl;
  ss+=getpid();
  std::cout<<"PID  : "<<getpid()<<std::endl;
  //
  FILE * fp = fopen ("/proc/uptime", "r");
  int uptime,upsecs;
  if (fp != NULL)
    {
      char buf[BUFSIZ];
      int res;
      char *b = fgets (buf, BUFSIZ, fp);
      if (b == buf)
        {
          /* The following sscanf must use the C locale.  */
          setlocale (LC_NUMERIC, "C");
          res = sscanf (buf, "%i", &upsecs);
          setlocale (LC_NUMERIC, "");
          if (res == 1)
            uptime = (time_t) upsecs;
        }

      fclose (fp);
    }

  std::cout<<"Uptime: "<<upsecs<<std::endl;
  ss+=upsecs;
  //
  std::cout<<"Seed for srand: "<<ss<<std::endl;
  srand(ss);
  rangen.SetSeed(rand());
  long int seed = round(1000000*rangen.Uniform());
  return seed;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
