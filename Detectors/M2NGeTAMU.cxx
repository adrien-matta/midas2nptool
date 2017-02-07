// STL
#include <iostream>
using namespace std;

// M2N
#include "M2NGeTAMU.h"
#include "M2NDetectorFactory.h"
#include "M2NRootOutput.h"

////////////////////////////////////////////////////////////////////////////////
M2N::GeTAMU::GeTAMU(){
  m_Data= new TGeTAMUData();
}

////////////////////////////////////////////////////////////////////////////////
M2N::GeTAMU::~GeTAMU(){
}
////////////////////////////////////////////////////////////////////////////////
void M2N::GeTAMU::Clear(){
  m_Data->Clear();
}
////////////////////////////////////////////////////////////////////////////////
void M2N::GeTAMU::Fill(string& token, double value){
  if(token.compare(0,6,"CLOVER")==0){
    // Extract Clover Number
    string clv_str = token.substr(6,2);
    int clv = atoi(clv_str.c_str()); 

    // Check if core or segment
    bool core = true;
    if(token.compare(9,7,"SEGMENT")==0)
      core = false;

    // Extract Segment/Cristal Number
    string seg_str = token.substr(16,2);
    int seg = atoi(seg_str.c_str()); 

    // Check energy or time
    bool energy = true;
    if(token.compare(19,1,"T")==0)
      energy = false;

    // Check if low gain
    bool energyLG = false;
    if(token.size()>=23 && token.compare(21,2,"LG")==0)
      energyLG = true;

    if(core){
      if(energy)
		if(energyLG)
        m_Data->SetCoreELowGain(clv,seg,value);
	  	else
        m_Data->SetCoreE(clv,seg,value);
      else
        m_Data->SetCoreT(clv,seg,value);
    }
    else{
      if(energy)
		if (energyLG)
        m_Data->SetSegmentELowGain(clv,seg,value);
		else
        m_Data->SetSegmentE(clv,seg,value);
      else
        m_Data->SetSegmentT(clv,seg,value);        
    } 
  }
}
////////////////////////////////////////////////////////////////////////////////
void M2N::GeTAMU::InitBranch(){
  M2N::RootOutput::getInstance()->GetTree()->Branch("GeTAMU","TGeTAMUData",&m_Data);
};

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
M2N::VDetector* M2N::GeTAMU::Construct(){
  return (M2N::VDetector*) new GeTAMU();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_m2ngetamu{
  public:
    proxy_m2ngetamu(){
      M2N::DetectorFactory::getInstance()->AddToken("GeTAMU","GeTAMU");
      M2N::DetectorFactory::getInstance()->AddDetector("GeTAMU",M2N::GeTAMU::Construct);
    }
};

proxy_m2ngetamu p_m2ngetamu;
}

