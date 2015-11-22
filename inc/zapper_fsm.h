#ifndef ZAPPER_FSM_H
#define ZAPPER_FSM_H

typedef enum zapp_fsm_state {
  STATE_IDLE,
  STATE_NUM_ENTER,
  STATE_UPDATE,
  STATE_STOP,
} zapp_fsm_state_t;

int  zapp_fsm_init();
int  zapp_fsm_deinit();
int  zapp_fsm_get_current_ch();

void zapp_fsm_key_press(char ev);
int  zapp_fsm_stopped();

#endif
