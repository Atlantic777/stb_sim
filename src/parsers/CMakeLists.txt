add_library(pat_parser pat_parser.c)
target_link_libraries(pat_parser parser_utils)

add_library(pmt_parser pmt_parser.c)
target_link_libraries(pmt_parser parser_utils)

add_library(sdt_parser sdt_parser.c)
target_link_libraries(sdt_parser parser_utils)

add_library(parser_utils parser_utils.c)
