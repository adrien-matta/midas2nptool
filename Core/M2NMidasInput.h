#ifndef M2NMIDASINPUT
#define M2NMIDASINPUT

// M2N
#include"M2NDetectorManager.h"
// STL
#include<string>
using namespace std;

namespace M2N{

  class MidasInput{
    public: // Constructor
      MidasInput();
      ~MidasInput();
  
    public: // Setter
      void SetDetectorManager(M2N::DetectorManager* mgr);
      void SetFileName(string filename); 

    public: // Heavy work
      void TreatFile();

    private: 
      M2N::DetectorManager* m_DetectorManager;
      string m_FileName;
  };
}



#endif
