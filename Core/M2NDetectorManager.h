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
      int  ADCChannelToAddress(int ADC, int Channel);
      int  TDCChannelToAddress(int ADC, int Channel);
      VDetector* GetDetector(string name);

    private: // maps
      map<int,M2N::VDetector*> m_ADC;
      map<int,M2N::VDetector*> m_TDC;
      map<string,M2N::VDetector*> m_Detector;

    private: // adc
      int m_ADCbase;
      int m_ADCoffset;
    
    private: // tdc
      int m_TDCbase;
      int m_TDCoffset;

    private: // Tree
      string m_TreeName;
  };


}
#endif
