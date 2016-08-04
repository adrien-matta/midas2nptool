add_library(M2NCore SHARED M2NConfigurationManager.cxx)
target_link_libraries(M2NCore ${ROOT_LIBRARIES}) 
install(FILES M2NConfigurationManager.h DESTINATION  ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
