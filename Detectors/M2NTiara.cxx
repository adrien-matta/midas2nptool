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
  m_Hyball= new TTiaraHyballData();
}

////////////////////////////////////////////////////////////////////////////////
M2N::Tiara::~Tiara(){
}
////////////////////////////////////////////////////////////////////////////////
void M2N::Tiara::Clear(){
  m_Barrel->Clear();
  m_Hyball->Clear();
}
////////////////////////////////////////////////////////////////////////////////
void M2N::Tiara::Fill(string& token, double value){
if(token.compare(0,1,"B")==0){ // Barrel
    bool front = true;
    bool up = true;
    bool energy = true;

    // Extract Barrel Number
    string b_str = token.substr(1,2);
    int b = atoi(b_str.c_str()); 

	// Extraxt Back or front 
    if(token.compare(4,2,"BK")==0)
      front = false;
	else // Extract Up or down on same strip 
		if(token.compare(4,2,"DW")==0)
		  up = false;
    
	// Extract Strip Number
    string s_str = token.substr(6,2);
    int s = 0 ; 
    //if(s_str!="XX")
      s = atoi(s_str.c_str());
    //else
      //front = false;

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

  else if(token.compare(0,1,"H")==0){ // Hyball
    bool front = true;
    bool energy = true;
    // Extract Wedge Number
    string w_str = token.substr(1,2);
    int w = atoi(w_str.c_str()); 

    //Extract Ring or Sector
    if(token.compare(4,2,"SC")==0)
      front = false;
 
    // Extract Strip Number
    string s_str = token.substr(6,2);
    int s = atoi(s_str.c_str());

    // Energy or Time
    if(token.compare(9,1,"T")==0)
      energy = false;

    if(energy){
      if(front)
        m_Hyball->SetRingE(w,s,value);
      else
        m_Hyball->SetSectorE(w,s,value);
    }
    else{
      if(front)
        m_Hyball->SetRingT(w,s,value);
      else
        m_Hyball->SetSectorT(w,s,value);
    }
  }  
}
////////////////////////////////////////////////////////////////////////////////
void M2N::Tiara::InitBranch(){
  // Barrel
  M2N::RootOutput::getInstance()->GetTree()->Branch("TiaraBarrel","TTiaraBarrelData",&m_Barrel);
  // Hyball
  M2N::RootOutput::getInstance()->GetTree()->Branch("TiaraHyball","TTiaraHyballData",&m_Hyball);


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

