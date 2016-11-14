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
    string clv_str = token.substr(1,2);
    int clv = atoi(clv_str.c_str()); 

    // Extract Segment Number
    string seg_str = token.substr(17,2);
    int seg = atoi(seg_str.c_str()); 

    // Check energy or time
    bool energy = true;
    if(token.compare(20,1,"T")==0)
      energy = false;

    

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

