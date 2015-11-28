add_executable(test_kb_input test_kb_input.c)
target_link_libraries(test_kb_input keyboard controller pthread rt)

add_executable(test_ctrl_flow test_ctrl_flow.c)
target_link_libraries(test_ctrl_flow keyboard console controller pthread rt)

add_executable(test_cli_view test_cli_view.c)
target_link_libraries(test_cli_view keyboard console controller pthread rt)

if(NOT DEFINED CROSS_COMPILE)
  add_executable(test_afb_gl_view test_afb_gl_view.c)
  target_link_libraries(test_afb_gl_view keyboard controller gl_view)
endif()

if(DEFINED CROSS_COMPILE)
  add_executable(test_dfb_view test_dfb_view.c)
  target_link_libraries(test_dfb_view keyboard dfb_view controller pthread rt)

  add_executable(test_model_and_dfb_view test_model_and_dfb_view.c)
  target_link_libraries(test_model_and_dfb_view
    remote
    dfb_view
    controller
    model
    tdp
    OSAL
    shm
    PEAgent
  )
endif()
