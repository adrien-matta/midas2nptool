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
   map<string,M2N::VDetector*>::iterator it;

  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
    delete it->second;
  }

}
////////////////////////////////////////////////////////////////////////////////
void M2N::DetectorManager::Fill(int address, int value){
  //cout << address << " " << value << endl;
  if(m_address.find(address)!=m_address.end())
    m_address[address]->Fill(m_token[address],value); 
}
////////////////////////////////////////////////////////////////////////////////
void M2N::DetectorManager::InitBranch(){
  map<string,M2N::VDetector*>::iterator it;

  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
    it->second->InitBranch();
  }
}

////////////////////////////////////////////////////////////////////////////////
void M2N::DetectorManager::Clear(){
  map<string,M2N::VDetector*>::iterator it;

  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
    it->second->Clear();
  }
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
 
   else if(key.compare(0,1,"%")==0){
      infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n')  ;
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
    if(key.compare(0,1,"%")!=0){
      cout << key << " " << module<< " " << buffer << " " << channel << " " << detector << " " << token << " -> ";
      
      M2N::VDetector* Det = GetDetector(detector); 
      if(!Det){
        cout << endl ;
        cout << "ERROR: Detector " << detector << " is not supported " << endl;
        exit(1);
      }

      int address=-1;
      if(key == "ADC")
        address = ADCChannelToAddress(module,channel);

      else if(key == "TDC")
        address = TDCChannelToAddress(module,channel);

      else{
        cout << "ERROR : Incorrect key \"" << key << "\" given to Detector Manager. Valid value are ADC and TDC " << endl;
        exit(1);
      }
      cout << address << endl; 
      m_address[address] = Det;
      m_token[address] = token;
    }
  }
  InitBranch();
}

////////////////////////////////////////////////////////////////////////////////
M2N::VDetector* M2N::DetectorManager::GetDetector(string& name){
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
int  M2N::DetectorManager::ADCChannelToAddress(int& ADC, int& Channel){
  return ((ADC-1)*m_ADCbase+Channel+m_ADCoffset);
}

////////////////////////////////////////////////////////////////////////////////
int  M2N::DetectorManager::TDCChannelToAddress(int& TDC, int& Channel){
  return ((TDC-1)*m_TDCbase+Channel+m_TDCoffset);
}

