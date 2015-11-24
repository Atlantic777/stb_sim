add_executable(test_kb_input test_kb_input.c)
target_link_libraries(test_kb_input ${ZAPP_LIBS})

add_executable(test_ctrl_flow test_ctrl_flow.c)
target_link_libraries(test_ctrl_flow ${ZAPP_LIBS})
