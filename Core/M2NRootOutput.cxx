#include "M2NRootOutput.h"
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
M2N::RootOutput* M2N::RootOutput::m_instance = 0 ;

////////////////////////////////////////////////////////////////////////////////
M2N::RootOutput* M2N::RootOutput::getInstance(string TreeName,string FileName){
  if(!m_instance)
    m_instance = new M2N::RootOutput(TreeName,FileName);

  return m_instance;
}

////////////////////////////////////////////////////////////////////////////////
void M2N::RootOutput::Destroy(){
  if(m_instance){
    delete m_instance;
    m_instance = 0 ;
  }
}

////////////////////////////////////////////////////////////////////////////////
M2N::RootOutput::RootOutput(){
  m_Tree= 0 ;
  m_File= 0 ;
}
////////////////////////////////////////////////////////////////////////////////
M2N::RootOutput::RootOutput(string TreeName,string FileName){
  m_File= new TFile(FileName.c_str(),"RECREATE");
  m_Tree= new TTree(TreeName.c_str(), "midas2nptool tree") ;
}
////////////////////////////////////////////////////////////////////////////////
M2N::RootOutput::~RootOutput(){
  if(m_File){
    m_File->Write();
    m_File->Close();
  }
}








