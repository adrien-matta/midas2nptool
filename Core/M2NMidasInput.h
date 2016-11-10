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

    public: // Generate tag to simulate a TreatFile 
      void SimulateTreat(int event, int cmin=0 , int cmax=65536, int vmin=0 , int vmax=65536);


    private: 
      M2N::DetectorManager* m_DetectorManager;
      TTree* m_Tree;
      string m_FileName;
  };
}



#endif
