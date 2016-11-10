add_library(M2NCore SHARED M2NDetectorManager.cxx M2NVDetector.cxx)
target_link_libraries(M2NCore ${ROOT_LIBRARIES}) 
install(FILES M2NDetectorManager.h M2NVDetector.h DESTINATION  ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
