/**
 * @file service_list.h
 * @author Nikola Hardi
 * @date November 2015
 * @brief List of services as STB model sees them
 */
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

/**
 * @brief Table of available services used internally in the
 * STB model.
 *
 * Has information on number of services and an array of
 * service items. Each service item provides details about
 * the service.
 */
struct service_table  {
  int cnt;
  service_item_t *items;
};

/**
 * @brief Service item is an entry in the service table.
 * It holds data joined from PAT and SDT.
 */
struct service_item {
  uint8_t  position;
  uint16_t program_number;
  uint16_t pmt_pid;

  char *name;
  service_type_t type;
};

/**
 * @brief Init service table with data from a PAT table
 *
 * @param pat table with data to be read
 * @param srv table into which data needs to be stored
 *
 * @return
 */
int pat2services(pat_table_t *pat, service_table_t *srv);

// TODO
service_item_t* service_get_by_ch_num(service_table_t *srv, int ch);
service_item_t* service_get_by_index(service_table_t *srv, int idx);

// TODO
// R: get entry by ch num
// handle mismatches
// [ ] set name (strncpy)
// [ ] set type
/**
 * @brief [UNUSED] add SDT data to service table.
 *
 * After service table is initialized with data from PAT,
 * add data from SDT, too. PAT and SDT data can be joined
 * by the program number.
 *
 * @param srv service table into which data needs to be stored
 * @param sdt SDT table with data to be read
 *
 * @return
 */
int service_add_sdt(service_table_t *srv, sdt_table_t *sdt);

#endif
