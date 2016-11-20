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
#include"TFPDTamuData.h"

namespace M2N{
  class FpdTAMU : public M2N::VDetector{

    public:
      FpdTAMU();
      ~FpdTAMU();

    public:
      void Fill(string& token, double value);
      void InitBranch();
      void Clear();

    private:
      TFPDTamuData* m_Data;

    public: // Static constructor to be passed to the Detector Factory
      static M2N::VDetector* Construct();

  };
}
#endif
