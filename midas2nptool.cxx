#include<iostream>
using namespace std;

#include "M2NDetectorManager.h"
#include "M2NMidasInput.h"
#include "M2NRootOutput.h"

int main(int argc, char** argv){
  /// Parsing argument
  if(argc == 2 && strcmp(argv[1],"-h")==0){
    cout << "midas2nptool data converter, usage is:" << endl; 
    cout << "\t midas2nptool configuration.txt midasfile.dat rootfile.root treename" << endl;
    return 0 ;
  }
  else if(argc !=5){
    cout << "Error: wrong number of argument, usages is:" << endl;
    cout << "\t midas2nptool configuration.txt midasfile.dat rootfile.root treename" << endl;
    exit(1);
  }

  string configfile(argv[1]);
  string midasfile(argv[2]);
  string rootfile(argv[3]);
  string treename(argv[4]);

  /// Framework is starting
  // First of all, create the output singleton
  M2N::RootOutput* root = M2N::RootOutput::getInstance(treename,rootfile);

  M2N::DetectorManager detector;
  detector.ReadConfiguration(configfile);
  
  M2N::MidasInput midas;
  midas.SetDetectorManager(&detector);
  midas.SetFileName(midasfile);
  midas.TreatFile();
  //midas.SimulateTreat(10000,1000,1050);
  root->Destroy();
} 
