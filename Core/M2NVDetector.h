#ifndef M2NVDETECTOR
#define M2NVDETECTOR
// STL
#include<string>
using namespace std; 

// ROOT
#include"TTree.h"
namespace M2N{
  class VDetector{
    public:
      VDetector();
      virtual ~VDetector();

    public:
      virtual void Fill(string& token, double value) {};
      virtual void InitBranch() {};
      virtual void Clear() {};
  };
}
#endif
