
#include"M2NDetectorFactory.h"
#include<dlfcn.h>
#include<fstream>
#include<iostream>
#include<stdlib.h>

M2N::DetectorFactory* M2N::DetectorFactory::m_Instance = 0;
////////////////////////////////////////////////////////////////////////////////
M2N::DetectorFactory::DetectorFactory(){

}

/////////////////////////////////////////////////////////////////////////////////
M2N::DetectorFactory::~DetectorFactory(){
  m_Instance = 0 ;
}

////////////////////////////////////////////////////////////////////////////////
M2N::DetectorFactory* M2N::DetectorFactory::getInstance(){
  if(!m_Instance){
    m_Instance = new DetectorFactory();
  }

  return m_Instance;

}
////////////////////////////////////////////////////////////////////////////////
void M2N::DetectorFactory::ReadClassList(std::string FileList){
    std::ifstream InFile(FileList.c_str());

    if(!InFile.is_open()){
      cout << "ERROR: Detector Class List file " << FileList << " Not found" << endl;
      exit(1);
    }

    string Token, LibName;
    while(InFile >> Token >> LibName)
      m_TokenLib[Token] = LibName; 
}
////////////////////////////////////////////////////////////////////////////////
void M2N::DetectorFactory::CreateClassList(std::string FileList){
  ofstream outFile(FileList.c_str());
  std::map<string,string>::iterator it;

  for(it = m_TokenLib.begin();it!=m_TokenLib.end();it++){
    outFile << it->first << " " << it->second << std::endl;
  }


}
////////////////////////////////////////////////////////////////////////////////
M2N::VDetector* M2N::DetectorFactory::Construct(std::string Token){
  std::map<std::string,ClassDetectorFactoryFn>::iterator it;
  if(m_Construct.find(Token)!=m_Construct.end())
    return  m_Construct[Token]();

  else if( m_TokenLib.find(Token)!=m_TokenLib.end()){
    // Add absolute path to library name
    std::string path = getenv("M2N"); 
    std::string libName = path+"/lib/"+m_TokenLib[Token];
    dlopen(libName.c_str(),RTLD_NOW | RTLD_GLOBAL);
    char* LibError = dlerror();
	if(m_Construct.find(Token)!=m_Construct.end())
      		return  m_Construct[Token]();
  
    else{
      
      std::cout << "Warning: Detector with Token " << Token << " has no Constructor or no Library" << std::endl;
      if(LibError){
        std::cout << "Library loading failes with error: " << std::endl;
        std::cout << LibError << std::endl << std::endl;
      }
      
      return NULL;
    }
  }

  return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void M2N::DetectorFactory::AddDetector(std::string Token, ClassDetectorFactoryFn fn){
  m_Construct[Token] = fn;
}
////////////////////////////////////////////////////////////////////////////////
void M2N::DetectorFactory::AddToken(std::string Token, std::string LibName){
  // Make sure the lib name is correct:
  // Strip it from lib and NP and extension
  std::string remword = "lib";
  size_t pos;
  if((pos=LibName.find(remword))!=std::string::npos){
    LibName.swap(LibName.erase(pos,remword.length()));
  }
  
  remword = "M2N";
  if((pos=LibName.find(remword))!=std::string::npos){
    LibName.swap(LibName.erase(pos,remword.length()));
  }
  
  remword = ".so";
  if((pos=LibName.find(remword))!=std::string::npos){
    LibName.swap(LibName.erase(pos,remword.length()));
  }

  remword = ".dylib";
  if((pos=LibName.find(remword))!=std::string::npos){
    LibName.swap(LibName.erase(pos,remword.length()));
  }


  // Create the full name:
  LibName = "libM2N"+LibName + ".so";
  m_TokenLib[Token] = LibName;
}
