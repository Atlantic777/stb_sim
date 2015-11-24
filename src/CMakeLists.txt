add_subdirectory(model)
add_subdirectory(view)
add_subdirectory(controller)
add_subdirectory(parsers)
add_subdirectory(input)
add_subdirectory(afb)

add_executable(stb_sim main.c)
target_link_libraries(stb_sim
  keyboard
  zapper_fsm
  zapper_gui_gl
  GL
  GLU
  glut
  pthread
)
