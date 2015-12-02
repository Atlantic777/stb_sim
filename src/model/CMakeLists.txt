if(DEFINED CROSS_COMPILE)
  add_library(real_model real_model.c)
  target_link_libraries(real_model 
		pat_parser 
		pmt_parser 
		sdt_parser
		pthread tdp OSAL shm PEAgent
	)
endif()

if(NOT DEFINED CROSS_COMPILE)
  add_library(fake_model fake_model.c)
  target_link_libraries(fake_model pat_parser pmt_parser)
endif()
