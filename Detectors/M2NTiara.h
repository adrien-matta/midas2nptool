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
#include"TTiaraHyballData.h"

namespace M2N{
  class Tiara : public M2N::VDetector{

    public:
      Tiara();
      ~Tiara();

    public:
      void Fill(string& token, double value);
      void InitBranch();
      void Clear();

    private:
      TTiaraBarrelData* m_Barrel;
      TTiaraHyballData* m_Hyball;

    public: // Static constructor to be passed to the Detector Factory
      static M2N::VDetector* Construct();

  };
}
#endif
