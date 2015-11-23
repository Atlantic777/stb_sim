#ifndef ZAPPER_FSM_H
#define ZAPPER_FSM_H

typedef enum zapp_fsm_signal {
  SIG_NOOP,
  SIG_KEY_DOWN,
  SIG_KEY_TIMEOUT,
  SIG_MAX
} zapp_fsm_signal_t;

typedef void (*State)(char);

void state_idle(char ev);
void state_num_enter(char ev);
void state_stop(char ev);
void state_update(char ev);

int  zapp_fsm_init();
int  zapp_fsm_deinit();
int  zapp_fsm_stopped();
int  zapp_fsm_get_current_ch();

void zapp_fsm_key_press(char ev);


#endif
