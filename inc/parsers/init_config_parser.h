/**
 * @file init_config_parser.h
 * @author Nikola Hardi
 * @date November 2015
 * @brief Initial model configuration parser
 */
#ifndef INIT_CONFIG_PARSER_H
#define INIT_CONFIG_PARSER_H

#include <stdint.h>

#define INIT_DVB_T  0
#define INIT_DVB_T2 1
#define INIT_AC3		2
#define INIT_MPEG		3
#define INIT_MPEG2  4
#define INIT_MPEG4  5

#define module_1	"DVB-T"
#define module_2	"DVB-T2"

#define audio_1		"ac3"
#define audio_2		"mpeg"

#define video_1		"mpeg2"
#define video_2		"mpeg4"

typedef struct init_config init_config_t;

/**
 * @brief Initial configuration for STB model
 *
 * Sets frequency, bandwidth and module (DVB-T or DVB-T2).
 * Sets default audio and video stream PIDs.
 * Sets audio and video encoding types for current session.
 */
struct init_config {
  /**
   * @brief frequency in Hz
   */
	uint32_t	frequency;
  /**
   * @brief Bandwidth in MHz
   */
	uint8_t 	bandwidth;
  /**
   * @brief DVB standard
   *
   * Supported values:
   *  - DVB-T
   *  - DVB-T2
   */
	uint8_t		module;

  /**
   * @brief Default audio PID
   */
	uint16_t	aPID;
  /**
   * @brief Default video PID
   */
	uint16_t	vPID;

  /**
   * @brief Audio stream encoding type
   *
   * Suported values:
   *  - ac3
   *  - mpeg
   */
	uint8_t		aType;
  /**
   * @brief Video stream encoding type
   *
   * Supported values:
   *  - mpeg2
   *  - mpeg4
   */
	uint8_t		vType;
};

/**
 * @brief Parse initial configuration data from a null
 * terminated string.
 *
 * @param c string with raw data
 * @param out pointer to struct which needs to be initialized
 *
 * @return
 */
int8_t parse_init_config(char *c, init_config_t *out);

#endif
