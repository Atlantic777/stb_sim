add_executable(test_kb_input test_kb_input.c)
target_link_libraries(test_kb_input ${ZAPP_LIBS})

add_executable(test_ctrl_flow test_ctrl_flow.c)
target_link_libraries(test_ctrl_flow ${ZAPP_LIBS})

add_executable(test_cli_view test_cli_view.c)
target_link_libraries(test_cli_view ${ZAPP_LIBS})

add_executable(test_afb_gl_view test_afb_gl_view.c)
target_link_libraries(test_afb_gl_view ${ZAPP_LIBS})
