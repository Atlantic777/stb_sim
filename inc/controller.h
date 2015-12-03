#ifndef CONTOLLER_H
#define CONTROLLER_H

#define CTRL_DEBUG 0

#if CTRL_DEBUG == 1
  #define LOG(MSG) _view->log("CTRL -> " MSG)
#else
  #define LOG(MSG)
#endif

#include "view.h"
#include <stdint.h>


typedef enum singal {
  SIG_NOOP,
  SIG_NUM_KEY,
  SIG_KILL,
  SIG_TIMEOUT,
	SIG_CH_CHANGE,
	SIG_VOL_CHANGE,
	SIG_INFO
} signal_t;

typedef struct controller {
  void (*start)(char*);
  void (*stop)();
  void (*handler)(uint32_t);
} controller_t;

typedef void(*State)(signal_t, uint32_t);

uint8_t ctrl_init(controller_t *ctrl);
uint8_t ctrl_deinit();

uint8_t ctrl_set_view(view_t *view);

#endif
