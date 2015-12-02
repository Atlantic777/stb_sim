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
} signal_t;

typedef struct controller {
  void (*start)();
  void (*stop)();
  void (*handler)(uint32_t);
} controller_t;

typedef void(*State)(signal_t, uint32_t);

int ctrl_init(controller_t *ctrl);
int ctrl_deinit();

int ctrl_set_view(view_t *view);

#endif
