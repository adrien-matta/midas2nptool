add_library(M2NTiara SHARED M2NTiara.cxx)
target_link_libraries(M2NTiara M2NCore ${ROOT_LIBRARIES} -L${NPLIB}/lib NPTiara) 
install(FILES M2NTiara.h DESTINATION  ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
