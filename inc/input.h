/**
 * @file input.h
 * @author Nikola Hardi
 * @date November 2015
 * @brief Input interface
 */
#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

/**
 * @brief Standard input interface
 */
typedef struct input {
  /**
   * @brief start input thread
   */
  void (*start)();
  /**
   * @brief Read from input (blocking)
   *
   * @param place where input data will be stored
   *
   * @return  unused
   */
  int  (*read)(uint32_t*);
} input_t;

#endif
