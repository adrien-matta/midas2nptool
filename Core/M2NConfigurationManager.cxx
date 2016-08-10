#include"M2NConfigurationManager.h"
#include<iostream>
#include<fstream>
using namespace std;
////////////////////////////////////////////////////////////////////////////////
M2N::ConfigurationManager::ConfigurationManager(){
  m_ADCbase = -1 ;
  m_ADCoffset = -1 ;
  m_TDCbase = -1 ;
  m_TDCoffset = -1 ;
  m_TreeName = "M2NTree";
}
////////////////////////////////////////////////////////////////////////////////
M2N::ConfigurationManager::~ConfigurationManager(){
}
////////////////////////////////////////////////////////////////////////////////
void M2N::ConfigurationManager::ReadConfiguration(string path){
  ifstream infile(path.c_str());
  if(infile.is_open()){
    cout << "**** Reading configuration file : " << path << " ****" << endl;  
  }
  else{
    cout << "**** ERROR: Configuration file : " <<  path << " not found ****"<< endl;
    exit(1);
  }


  string key,buffer, detector,token;
  int value,channel;
  // Reading setting line
  infile >> key; 
    while(key!="MAP"){
      if(key == "ADC"){
        infile >> token >> value;
        if(token == "BASE")
          m_ADCbase = value;
        else if(token == "OFFSET")
          m_ADCoffset = value;
      } 

      else if(key == "TDC"){
        infile >> token >> value;
        if(token == "BASE")
          m_TDCbase = value;
        else if(token == "OFFSET")
          m_TDCoffset = value;
      }
     
      else if(key == "TREE"){
        infile >> token >> buffer;
        if(token == "NAME")
          m_TreeName = buffer;
      }

      else{
        cout << "**** INFO : Token unknown : " << token << " ****" << endl;
      }
      infile >> key ;
    }

  if(m_ADCbase!=-1){
    cout << "**** ADC setting : Base = " << m_ADCbase << " Offset = " << m_ADCoffset << " ****" << endl; 
  }

  if(m_TDCbase!=-1){
    cout << "**** TDC setting : Base = " << m_TDCbase << " Offset = " << m_TDCoffset << " ****" << endl; 
  }


  while(infile >> key >> value >> buffer >> channel >> detector >> token){
    cout << key << " " << value << " " << buffer << " " << channel << " " << detector << " " << token << endl;
  }

}



