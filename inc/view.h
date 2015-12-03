#ifndef VIEW_H
#define VIEW_H

#include <stdint.h>
#include "model/service_list.h"

typedef struct view {
  void (*log)(char*);

  void (*show_num_input)(char*);
  void (*hide_num_input)();

  void (*show_info_bar)(service_item_t*);
  void (*hide_info_bar)();

  void (*show_volume)(uint8_t vol);
  void (*hide_volume)();

  void (*start)();
} view_t;

#endif
