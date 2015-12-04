/**
 * @file norm_codes.h
 * @author Nikola Hardi
 * @date November 2015
 *
 * @brief Table of input key codes which controller "undestands"
 *
 * Key code values vary depending on input device. A table
 * like this is needed for standardization purposes. Those
 * key codes have values which controller can decode and fire
 * propper signals.
 *
 * There are 4 groups of key codes:
 *  1. Number keys from 0 to 9
 *  2. Control keys (exit, info...)
 *  3. Program change keys (ch up, ch down)
 *  4. Volume keys (vol up, vol down)
 */
#ifndef CTRL_NORM_CODES_H
#define CTRL_NORM_CODES_H

/**
 * @brief Table of normalized key codes
 */
typedef enum norm_codes {
  N_KEY_0 = 0,
  N_KEY_1,
  N_KEY_2,
  N_KEY_3,
  N_KEY_4,
  N_KEY_5,
  N_KEY_6,
  N_KEY_7,
  N_KEY_8,
  N_KEY_9,

  N_KEY_EXIT = 18,
	N_KEY_INFO = 19,

  N_KEY_CH_UP = 20,
  N_KEY_CH_DN = 21,

  N_KEY_VOL_UP = 22,
  N_KEY_VOL_DN = 23,

} norm_codes_t;

#endif
