#ifndef REMOTE_CODES_H
#define REMOTE_CODES_H

typedef enum rc_codes {
	RC_NUM_1 = 2,
	RC_NUM_2,
	RC_NUM_3,
	RC_NUM_4,
	RC_NUM_5,
	RC_NUM_6,
	RC_NUM_7,
	RC_NUM_8,
	RC_NUM_9,
	RC_NUM_0 = 11,

  RC_KEY_EXIT = 116,

  RC_KEY_CH_UP = 62,
  RC_KEY_CH_DN = 61,

  RC_KEY_VOL_UP = 63,
  RC_KEY_VOL_DN = 64,

	RC_KEY_MUTE = 60,

	RC_KEY_INFO = 102,
} rc_codes_t;

#endif
