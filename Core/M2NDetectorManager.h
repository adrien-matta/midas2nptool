#ifndef M2NDETECTORMANAGER
#define M2NDETECTORMANAGER

// STL
#include<map>
#include<string>
using namespace std;

// M2N
#include"M2NVDetector.h"

namespace M2N{

  class DetectorManager{

    public:
      DetectorManager();
      ~DetectorManager();

    public:
      void ReadConfiguration(string path);
      void Fill(unsigned short int address, unsigned short int value);
      void InitBranch();
      void Clear();
      int  ADCChannelToAddress(int& ADC, int& channel);
      int  TDCChannelToAddress(int& ADC, int& channel);
      VDetector* GetDetector(string& name);

    private: // maps
      map<int,M2N::VDetector*> m_address;
      map<int,string> m_token;

      map<string,M2N::VDetector*> m_Detector;

    private: // adc
      int m_ADCbase;
      int m_ADCoffset;
    
    private: // tdc
      int m_TDCbase;
      int m_TDCoffset;
  };
}
#endif
