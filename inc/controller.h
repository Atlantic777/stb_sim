/**
 * @file controller.h
 * @author Nikola Hardi
 * @date November 2015
 * @brief Application behaviour, central logic part
 */
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


/**
 * @brief Signals used internally by controller.
 */
typedef enum singal {
  SIG_NOOP,
  SIG_NUM_KEY,
  SIG_KILL,
  SIG_TIMEOUT,
	SIG_CH_CHANGE,
	SIG_VOL_CHANGE,
	SIG_INFO
} signal_t;

/**
 * @brief Interface to zapper controller.
 */
typedef struct controller {
  void (*start)(char*);
  void (*stop)();
  void (*handler)(uint32_t);
} controller_t;

/**
 * @brief FSM state implementation function type.
 *
 * @param signal what happened
 * @param uint32_t optional argument
 */
typedef void(*State)(signal_t, uint32_t);

/**
 * @brief Controller interface initialization
 *
 * @param ctrl structure to be initialized
 *
 * @return
 */
uint8_t ctrl_init(controller_t *ctrl);
uint8_t ctrl_deinit();

uint8_t ctrl_set_view(view_t *view);

#endif
