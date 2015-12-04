/**
 * @file model.h
 * @author Nikola Hardi
 * @date November 2015
 * @brief Model interface
 */
#ifndef MODEL_H
#define MODEL_H

#include "model/service_list.h"

/**
 * @brief STB model interface
 */
typedef struct model {
  /**
   * @brief initialize the model
   *
   * @param init config path
   */
	int (*init)(char*);

  /**
   * @brief deinit the model
   */
	int (*deinit)();

  /**
   * @brief Force rescan of channels, reread PAT and SDT tables.
   */
	int (*scan)();

  /**
   * @brief Get descriptor of current chann.
   *
   * @return pointer to the descriptor
   */
	service_item_t* (*get_current_ch)();

  /**
   * @brief Print list of all services.
   *
   * In future, this function should return services list table.
   *
   * @param
   */
	int (*get_ch_list)();

  /**
   * @brief Get current volume level in percents.
   *
   * @param current volume level.
   */
  int (*get_volume)();

  /**
   * @brief Switch to n-th program in service table.
   *
   * @param index of program to switch to
   */
	int (*ch_switch)(int ch);

  /**
   * @brief Switch to next program. Will loop.
   */
	int (*ch_up)();

  /**
   * @brief Switch to previous program. Will loop.
   */
	int (*ch_down)();

  /**
   * @brief Increase volume level by 10 percents.
   */
	int (*vol_up)();

  /**
   * @brief Decrease volume level by 10 percents.
   */
	int (*vol_down)();

  /**
   * @brief Mute or restore old volume level.
   */
	int (*vol_mute)();

} model_t;

#endif
