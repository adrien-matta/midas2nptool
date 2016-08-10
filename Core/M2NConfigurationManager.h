#include<map>
#include<string>
using namespace std;

namespace M2N{

  class ConfigurationManager{

    public:
      ConfigurationManager();
      ~ConfigurationManager();

    public:
      void ReadConfiguration(string path);

    private: // maps
      map<int,string> m_ADC;
      map<int,string> m_TDC;

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
