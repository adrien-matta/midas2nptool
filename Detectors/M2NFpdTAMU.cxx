// STL
#include <iostream>
using namespace std;

// M2N
#include "M2NFpdTAMU.h"
#include "M2NDetectorFactory.h"
#include "M2NRootOutput.h"

////////////////////////////////////////////////////////////////////////////////
M2N::FpdTAMU::FpdTAMU(){
  m_Data= new TFPDTamuData();
}

////////////////////////////////////////////////////////////////////////////////
M2N::FpdTAMU::~FpdTAMU(){
}
////////////////////////////////////////////////////////////////////////////////
void M2N::FpdTAMU::Clear(){
  m_Data->Clear();
}
////////////////////////////////////////////////////////////////////////////////
void M2N::FpdTAMU::Fill(string& token, double value){

  if(token.compare(0,5,"AWIRE")==0){

    // Extract Row 0->n = upstream -> downstream
    string row_str = token.substr(7,1);
    int row = atoi(row_str.c_str()); 

    // Extract Side (0=Left, 1=Right) 
    string col_str = token.substr(10,1);
    int col = atoi(col_str.c_str()); 

    // Check energy or time
    bool energy = true;
    if(token.compare(12,1,"T")==0) 
      energy = false;

    if(energy){
        m_Data->Set_AWire_E(row,col,value);
    }
    else{
        m_Data->Set_AWire_T(row,col,value);
    } 
  }

  if(token.compare(0,5,"PLAST")==0){

    // Extract Side (0=Left, 1=Right) 
    string col_str = token.substr(10,1);
    int col = atoi(col_str.c_str()); 

    // Check energy or time
    bool energy = true;
    if(token.compare(12,1,"T")==0) 
      energy = false;

    if(energy){
        m_Data->Set_Plast_E(col,value);
    }
    else{
        m_Data->Set_Plast_T(col,value);
    } 
  }

    if(token.compare(0,5,"DELTA")==0){

    // Extract Row 0->n = upstream -> downstream
    string row_str = token.substr(7,1); // supports many rows
    int row = atoi(row_str.c_str());

    // Check energy or time
    bool energy = true;
    if(token.compare(12,1,"T")==0) 
      energy = false;

    if(energy){
        m_Data->Set_Delta_E(row,value);
    }
    else{
        m_Data->Set_Delta_T(row,value);
    } 
  }

  if(token.compare(0,5,"MICRO")==0){

    // Extract Row 0->n = upstream -> downstream
    string row_str = token.substr(7,1);
    int row = atoi(row_str.c_str()); 

    // Extract Col 0->n = Leftmost -> Rightmost (looking downstream from the top)
    string col_str = token.substr(10,1);
    int col = atoi(col_str.c_str()); 

    // Check energy or time
    bool energy = true;
    if(token.compare(12,1,"T")==0) 
      energy = false;

    if(energy){
        m_Data->Set_Micro_E(row,col,value);
    }
    else{
        m_Data->Set_Micro_T(row,col,value);
    } 
  }

}
////////////////////////////////////////////////////////////////////////////////
void M2N::FpdTAMU::InitBranch(){
  M2N::RootOutput::getInstance()->GetTree()->Branch("FpdTAMU","TFPDTamuData",&m_Data);
};

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
M2N::VDetector* M2N::FpdTAMU::Construct(){
  return (M2N::VDetector*) new FpdTAMU();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_m2nfpdtamu{
  public:
    proxy_m2nfpdtamu(){
      M2N::DetectorFactory::getInstance()->AddToken("FpdTAMU","FpdTAMU");
      M2N::DetectorFactory::getInstance()->AddDetector("FpdTAMU",M2N::FpdTAMU::Construct);
    }
};

proxy_m2nfpdtamu p_m2nfpdtamu;
}

