add_library(M2NTiara SHARED M2NTiara.cxx)
add_library(M2NGeTAMU SHARED M2NGeTAMU.cxx)

target_link_libraries(M2NTiara M2NCore ${ROOT_LIBRARIES} -L${NPLIB}/lib NPTiara) 
target_link_libraries(M2NGeTAMU M2NCore ${ROOT_LIBRARIES} -L${NPLIB}/lib NPGeTAMU) 

install(FILES M2NTiara.h M2NGeTAMU.h DESTINATION  ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
