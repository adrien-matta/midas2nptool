#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>
#include "M2NDetectorFactory.h"
int main(int argc , char** argv){
  int return_value;

#ifdef __APPLE__
  std::string CORRECT_LIB_EXTENSION = ".dylib";
  std::string INCORRECT_LIB_EXTENSION = ".so";
#endif
#ifdef __linux__
  std::string INCORRECT_LIB_EXTENSION = ".dylib";
  std::string CORRECT_LIB_EXTENSION = ".so";
#endif
#ifdef __FreeBSD__
  std::string INCORRECT_LIB_EXTENSION = ".dylib";
  std::string CORRECT_LIB_EXTENSION = ".so";
#endif


  // Build the lib list from the argument
  std::string detlist="";
  for(unsigned int i = 1 ; i < argc ; i++){
    detlist+=argv[i];
    detlist+=" ";
  }

  // Generate the Class list with Token for autoloading of the Detector classes
  DIR *dir;
  struct dirent *ent;
  string path = getenv("M2N");
  path += "/lib";
  string lib;
  if ((dir = opendir (path.c_str())) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      lib= ent->d_name ;
      if(lib.find("lib")!=std::string::npos)
        dlopen(lib.c_str(),RTLD_LAZY);
    }
    closedir (dir);
  }

  M2N::DetectorFactory::getInstance()->CreateClassList("ClassList.txt");

  return 0;
}
