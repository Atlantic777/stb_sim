add_library(keyboard keyboard.c)

add_library(remote remote.c)
target_link_libraries(remote pthread)
