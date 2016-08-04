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

    private:
      map<int,string> m_adc;
      map<int,string> m_tdc;

  };


}
