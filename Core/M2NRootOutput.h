#ifndef M2NROOTOUTPUT
#define M2NROOTOUTPUT

// STL
#include <string>
using namespace std;

// Root
#include "TTree.h"
#include "TFile.h"


namespace M2N{

  class RootOutput{
    private:
      RootOutput();
      RootOutput(string TreeName, string FileName);
      ~RootOutput();
   
    private:
      static RootOutput* m_instance;

    public:
      static RootOutput* getInstance(string TreeName="",string FileName="");
      void Destroy();
 
    public:
      inline TTree* GetTree() {return m_Tree;};
      inline TFile* GetFile() {return m_File;};

    private:
      TTree* m_Tree;
      TFile* m_File;


  };


}


#endif
