add_library(keyboard keyboard.c)
add_library(zapper_fsm zapper_fsm.c zapper_fsm_states.c)
target_link_libraries(zapper_fsm pthread rt)

add_library(zapper_gui_gl zapper_gui_gl.c)

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
