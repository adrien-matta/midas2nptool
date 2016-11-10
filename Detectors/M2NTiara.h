#ifndef M2NTIARA
#define M2NTIARA
// STL
#include<string>
using namespace std; 

// ROOT
#include"TTree.h"

//M2N
#include"M2NVDetector.h"

// NPTOOL
#include"TTiaraBarrelData.h"

namespace M2N{
  class Tiara : public M2N::VDetector{

    public:
      Tiara();
      ~Tiara();

    public:
      void Fill(string& token, double value);
      void InitBranch(TTree* tree) {};
      void Clear();

    private:
      TTiaraBarrelData* m_Barrel;

    public: // Static constructor to be passed to the Detector Factory
      static M2N::VDetector* Construct();

  };
}
#endif
