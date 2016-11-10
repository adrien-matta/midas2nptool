
// STL
#include<iostream>
#include<fstream>
using namespace std;

// M2M
#include"M2NDetectorManager.h"
#include"M2NDetectorFactory.h"
////////////////////////////////////////////////////////////////////////////////
M2N::DetectorManager::DetectorManager(){
  m_ADCbase = -1 ;
  m_ADCoffset = -1 ;
  m_TDCbase = -1 ;
  m_TDCoffset = -1 ;
  m_TreeName = "M2NTree";
  M2N::DetectorFactory::getInstance()->ReadClassList("ClassList.txt");
}
////////////////////////////////////////////////////////////////////////////////
M2N::DetectorManager::~DetectorManager(){
}
////////////////////////////////////////////////////////////////////////////////
void M2N::DetectorManager::ReadConfiguration(string path){
  ifstream infile(path.c_str());
  if(infile.is_open()){
    cout << "**** Reading configuration file : " << path << " ****" << endl;  
  }
  else{
    cout << "**** ERROR: Configuration file : " <<  path << " not found ****"<< endl;
    exit(1);
  }


  string key,buffer, detector,token;
  int value,channel,module;
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


  while(infile >> key >> module >> buffer >> channel >> detector >> token){
    cout << key << " " << module<< " " << buffer << " " << channel << " " << detector << " " << token << endl;
   
   M2N::VDetector* Det = GetDetector(detector); 
 
    if(key == "ADC"){
      ADCChannelToAddress(module,channel);
    }
    else if(key == "TDC"){
      TDCChannelToAddress(module,channel);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
M2N::VDetector* M2N::DetectorManager::GetDetector(string name){
    // look if the detector already exist in the map
    if(m_Detector.find(name)!=m_Detector.end()){
      return m_Detector[name];
    } 
    else{
      m_Detector[name]=M2N::DetectorFactory::getInstance()->Construct(name);
      return m_Detector[name];
    }
}

////////////////////////////////////////////////////////////////////////////////
int  M2N::DetectorManager::ADCChannelToAddress(int ADC, int Channel){
  return (ADC*m_ADCbase+Channel+m_TDCoffset);
}

////////////////////////////////////////////////////////////////////////////////
int  M2N::DetectorManager::TDCChannelToAddress(int TDC, int Channel){
  return (TDC*m_TDCbase+Channel+m_TDCoffset);
}

