#ifndef M2NMIDASINPUT
#define M2NMIDASINPUT

// M2N
#include"M2NDetectorManager.h"

// Root
#include"TTree.h"
#include"TH1.h"

// STL
#include<string>
using namespace std;

// Define the size of long and int, needed to set correctly the header size
// Work for GCC need test on Mac OS
#if __GNUC__
#if __x86_64__ || __ppc64__
#define M2N_64SYSTEM 
#else
#define M2N_32SYSTEM
#endif
#endif

namespace M2N{

  // MIDAS file data block header
  struct s_data_header {
    char*   header_id = new char[9];     //   contains the string  EBYEDATA
    #ifdef M2N_64SYSTEM
    int header_sequence;  //   within the file
    #else
    long header_sequence; //   within the file
    #endif
    short int header_stream;    //   data acquisition stream number (in the range 1=>4)
    short int header_tape;      //   =1
    short int header_MyEndian;  //   written as a native 1 by the tape server
    short int header_DataEndian; //   written as a native 1 in the hardware structure of the data following
    #ifdef M2N_64SYSTEM
    int header_dataLen;   //   total length of useful data following the header in bytes
    #else
    long header_dataLen;  //   total length of useful data following the header in bytes
    #endif
  }; 
  typedef struct s_data_header DATA_HEADER;


  class MidasInput{
    public: // Constructor
      MidasInput();
      ~MidasInput();

    public: // Setter
      void SetDetectorManager(M2N::DetectorManager* mgr);
      void SetFileName(string filename); 

    public: // Heavy work
      void TreatFile();
      bool ReadBlockHeader(ifstream& fin);
      bool ReadBlock(ifstream& fin);
      unsigned short ReadWord(ifstream& fin);
      unsigned short ReadMiniWord(ifstream& fin);

      unsigned short BitMask(unsigned short a, unsigned short b);
      unsigned short Swap(unsigned short value);

    public: // Generate tag to simulate a TreatFile 
      void SimulateTreat(int event, int cmin=0 , int cmax=65536, int vmin=0 , int vmax=65536);

    private: 
      M2N::DetectorManager* m_DetectorManager;
      TTree* m_Tree;
      DATA_HEADER m_header;
      TH1* m_HitPattern;
      string m_FileName;
      unsigned int m_blocks;
      unsigned int m_events;
      unsigned int m_bytes_read;
  };
}



#endif
