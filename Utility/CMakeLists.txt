add_executable(m2n_installer m2n_installer.cxx)
target_link_libraries(m2n_installer M2NCore)
install(PROGRAMS m2n_installer DESTINATION ${CMAKE_BINARY_OUTPUT_DIRECTORY})
