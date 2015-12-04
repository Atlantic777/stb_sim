find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  set(doxy_main_page mainpage.md)

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)
  configure_file(${doxy_main_page} ${doxy_main_page} @ONLY)

  add_custom_target(docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "build documentation"
    VERBATIM
    )
endif()
