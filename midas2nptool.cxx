#include<iostream>
using namespace std;

#include "M2NDetectorManager.h"
#include "M2NMidasInput.h"

int main(void){
  cout << "Hello World" << endl;
  M2N::DetectorManager detector;
  detector.ReadConfiguration("test.txt");
  M2N::MidasInput midas;
  midas.SetDetectorManager(&detector);
  
  midas.SimulateTreat(1000,32,40);
} 
