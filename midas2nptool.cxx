#include<iostream>
using namespace std;

#include"M2NConfigurationManager.h"


int main(void){
  cout << "Hello World" << endl;
  M2N::ConfigurationManager configManager;
  configManager.ReadConfiguration("pipo");

} 
