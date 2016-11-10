#ifndef M2NDETECTORFACTORY
#define M2NDETECTORFACTORY

#include"M2NVDetector.h"
#include<map>
#include<string>

typedef M2N::VDetector* (*ClassDetectorFactoryFn)();

namespace M2N{
class DetectorFactory{
  private:
      DetectorFactory();
      ~DetectorFactory();

  public:
      static DetectorFactory* getInstance();
      M2N::VDetector* Construct(std::string Token);
      void ReadClassList(std::string FileList);
      void AddToken(std::string Token, std::string LibName);
      void AddDetector(std::string Token, ClassDetectorFactoryFn fn);
      void CreateClassList(string FileName);
  private:
      static DetectorFactory* m_Instance;
      // Map that Match the Token with Constructor, once the lib is loaded
      std::map<std::string,ClassDetectorFactoryFn> m_Construct;
      // Map that Match the Token with the Lib to be loaded
      std::map<std::string,std::string> m_TokenLib;
};
}
#endif
