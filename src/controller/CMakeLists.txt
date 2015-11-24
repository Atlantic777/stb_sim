add_library(zapper_fsm zapper_fsm.c zapper_fsm_states.c)
target_link_libraries(zapper_fsm pthread rt)

add_library(controller controller.c)
