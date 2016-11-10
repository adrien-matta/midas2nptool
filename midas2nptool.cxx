#include<iostream>
using namespace std;

#include "M2NDetectorManager.h"
#include "M2NMidasInput.h"
#include "M2NRootOutput.h"

int main(void){
  M2N::RootOutput* root = M2N::RootOutput::getInstance("M2NTree","run.root");

  M2N::DetectorManager detector;
  detector.ReadConfiguration("test.txt");
  
  M2N::MidasInput midas;
  midas.SetDetectorManager(&detector);
  midas.SetFileName("sample/R40_0");
 // midas.TreatFile();
 midas.SimulateTreat(10000,1000,1050);
  root->Destroy();
} 
