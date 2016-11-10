add_library(M2NDetectors SHARED M2NTiara.cxx)
target_link_libraries(M2NDetectors M2NCore ${ROOT_LIBRARIES}) 
install(FILES M2NTiara.h DESTINATION  ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
