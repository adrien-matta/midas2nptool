add_library(M2NCore SHARED M2NDetectorManager.cxx M2NVDetector.cxx M2NDetectorFactory.cxx M2NMidasInput.cxx M2NRootOutput.cxx)
target_link_libraries(M2NCore ${ROOT_LIBRARIES}) 
install(FILES M2NDetectorManager.h M2NVDetector.h M2NDetectorFactory.h M2NMidasInput.h M2NRootOutput.h DESTINATION  ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
