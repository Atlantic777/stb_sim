add_library(keyboard keyboard.c)
add_library(zapper_fsm zapper_fsm.c)
target_link_libraries(zapper_fsm pthread rt)

add_executable(stb_sim main.c)
target_link_libraries(stb_sim keyboard zapper_fsm)
