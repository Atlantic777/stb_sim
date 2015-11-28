add_library(console console.c)

if(NOT DEFINED CROSS_COMPILE)
  add_library(gl_view gl_view.c)
  target_link_libraries(gl_view GL GLU glut X11)
endif()

if(DEFINED CROSS_COMPILE)
  add_library(dfb_view dfb_view.c)
  target_link_libraries(dfb_view directfb direct fusion)
endif()

