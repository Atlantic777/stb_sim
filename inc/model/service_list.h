#ifndef SERVICE_LIST_H
#define SERVICE_LIST_H

#include <stdint.h>
#include "parsers/pat_parser.h"
#include "parsers/pmt_parser.h"
#include "parsers/sdt_parser.h"

typedef struct service_table service_table_t;
typedef struct service_item service_item_t;

typedef enum service_type service_type_t;

enum service_type {
  VIDEO_SERVICE = 1,
  AUDIO_SERVICE = 2,
  OTHER_SERVICE = 3
};

struct service_table  {
  int cnt;
  service_item_t *items;
};

struct service_item {
  uint8_t  position;
  uint16_t program_number;
  uint16_t pmt_pid;

  char *name;
  service_type_t type;
};

// TODO
// [ ] allocate memory
// [ ] copy pmt pid's
int pat2services(pat_table_t *pat, service_table_t *srv);

// TODO
service_item_t* service_get_by_ch_num(service_table_t *srv, int ch);
service_item_t* service_get_by_index(service_table_t *srv, int idx);

// TODO
// R: get entry by ch num
// handle mismatches
// [ ] set name (strncpy)
// [ ] set type
int service_add_desc(service_table_t *srv, sdt_table_t *sdt);

#endif
