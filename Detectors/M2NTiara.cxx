// STL
#include <iostream>
using namespace std;

// M2N
#include "M2NTiara.h"
#include "M2NDetectorFactory.h"
#include "M2NRootOutput.h"

////////////////////////////////////////////////////////////////////////////////
M2N::Tiara::Tiara(){
  m_Barrel= new TTiaraBarrelData();
}

////////////////////////////////////////////////////////////////////////////////
M2N::Tiara::~Tiara(){
}
////////////////////////////////////////////////////////////////////////////////
void M2N::Tiara::Clear(){
  m_Barrel->Clear();

}
////////////////////////////////////////////////////////////////////////////////
void M2N::Tiara::Fill(string& token, double value){
  if(token.compare(0,0,"B")){ // Barrel
    bool front = true;
    bool up = true;
    bool energy = true;
    
    // Extract Barrel Number
    string b_str = token.substr(1,2);
    int b = atoi(b_str.c_str()); 

    // Extract String Number
    string s_str = token.substr(6,2);
    int s = 0 ; 
    if(s_str!="XX")
      s = atoi(s_str.c_str()); 
    else
      front = false;
   
     if(token.compare(5,2,"DW")==0)
      up = false;

    // Energy or Time
    if(token.compare(9,1,"T")==0)
      energy = false;

    // Fill in the class 
    if(energy){
      if(front){
        if(up){
          m_Barrel->SetFrontUpstreamE(b,s,value);
        }
        else
          m_Barrel->SetFrontDownstreamE(b,s,value);
      }
      else
        m_Barrel->SetBackE(b,value);
    }
    else{
      if(front){
        if(up)
          m_Barrel->SetFrontUpstreamT(b,s,value);
        else
          m_Barrel->SetFrontDownstreamT(b,s,value);
      }
      else
        m_Barrel->SetBackT(b,value);
    }

  }
  
  else if(token.compare(0,0,"H")){ // Hyball
    // to do
  }  
}
////////////////////////////////////////////////////////////////////////////////
void M2N::Tiara::InitBranch(){
  // Barrel
  M2N::RootOutput::getInstance()->GetTree()->Branch("TiaraBarrel","TTiaraBarrelData",&m_Barrel);

};

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
M2N::VDetector* M2N::Tiara::Construct(){
  return (M2N::VDetector*) new Tiara();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_m2ntiara{
  public:
    proxy_m2ntiara(){
      M2N::DetectorFactory::getInstance()->AddToken("Tiara","Tiara");
      M2N::DetectorFactory::getInstance()->AddDetector("Tiara",M2N::Tiara::Construct);
    }
};

proxy_m2ntiara p_m2ntiara;
}

