#include<iostream>
using namespace std;

#include"M2NDetectorManager.h"


int main(void){
  cout << "Hello World" << endl;
  M2N::DetectorManager configManager;
  configManager.ReadConfiguration("test.txt");

} 
