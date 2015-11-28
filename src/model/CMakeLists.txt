add_library(model model.c)
target_link_libraries(model pat_parser pmt_parser)
