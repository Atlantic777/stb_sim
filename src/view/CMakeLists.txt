add_library(console console.c)
# add_library(gl_view gl_view.c)
add_library(dfb_view dfb_view.c)

target_link_libraries(dfb_view directfb direct fusion)
