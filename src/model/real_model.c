#include <tdp_api.h>
#include <stdint.h>
#include <pthread.h>
#include "model/real_model.h"
#include "parsers/pat_parser.h"
#include "parsers/pmt_parser.h"
#include "parsers/sdt_parser.h"
#include "parsers/init_config_parser.h"
#include <stdio.h>

#define DESIRED_FREQUENCY 754000000
#define BANDWIDTH 8
#define PAT_PID 0
#define SDT_PID 0x11
#define PAT_TABLE_ID 0
#define PMT_TABLE_ID 2
#define SDT_TABLE_ID 0x42

static int32_t current_ch = 0;

static int32_t VideoType;
static int32_t AudioType;

static int stb_scan();
static int stb_get_current_ch();
static int stb_get_ch_list();

static int stb_ch_switch(int ch);
static int stb_ch_up();
static int stb_ch_down();

static int stb_vol_up();
static int stb_vol_down();

static pthread_cond_t status_condition = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t status_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t pat_condition = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t pat_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t pmt_condition = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t pmt_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t sdt_condition = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t sdt_mutex = PTHREAD_MUTEX_INITIALIZER;

static uint32_t player_handle, source_handle, filter_handle;
static uint32_t stream_handle_A, stream_handle_V;

static pat_table_t pat;
static pmt_table_t pmt;
static sdt_table_t sdt;

// ######## CALLBACKS ##############

static int32_t lock_status_callback(t_LockStatus status)
{
	pthread_mutex_lock(&status_mutex);
	pthread_cond_signal(&status_condition);
	pthread_mutex_unlock(&status_mutex);
}

static int32_t demux_pat_callback(uint8_t *buff)
{
	pthread_mutex_lock(&pat_mutex);
	pat_parse(&pat, buff);
	pthread_cond_signal(&pat_condition);
	pthread_mutex_unlock(&pat_mutex);
}

static int32_t demux_pmt_callback(uint8_t *buff)
{
	pthread_mutex_lock(&pmt_mutex);
	pmt_parse(&pmt, buff);
	pthread_cond_signal(&pmt_condition);
	pthread_mutex_unlock(&pmt_mutex);
}

static int32_t demux_sdt_callback(uint8_t *buff)
{
	pthread_mutex_lock(&sdt_mutex);
	sdt_parse(&sdt, buff);
	pthread_cond_signal(&sdt_condition);
	pthread_mutex_unlock(&sdt_mutex);
}
// ##### END OF CALLBACKS ############
int stb_init(char *filepath)
{
	puts("stb init");

	// read whole file
	FILE *f = fopen(filepath, "rb");

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *string = malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = 0;

	init_config_t config;
	parse_init_config(string, &config);

	Tuner_Init();
	Tuner_Register_Status_Callback(lock_status_callback);

	t_Module module;
	if(config.module == INIT_DVB_T)
	{
		module = DVB_T;
	}
	else
	{
		module = DVB_T2;
	}

	if(config.aType == INIT_MPEG)
	{
		AudioType = AUDIO_TYPE_MPEG_AUDIO;
	}
	else
	{
		AudioType = AUDIO_TYPE_DOLBY_AC3;
	}

	if(config.vType == INIT_MPEG2)
	{
		VideoType = VIDEO_TYPE_MPEG2;
	}
	else
	{
		VideoType = VIDEO_TYPE_MPEG4;
	}

	Tuner_Lock_To_Frequency(config.frequency, config.bandwidth, module);

	// TODO: add timed wait
	pthread_mutex_lock(&status_mutex);
	pthread_cond_wait(&status_condition, &status_mutex);
	pthread_mutex_unlock(&status_mutex);

	Player_Init(&player_handle);
	Player_Source_Open(player_handle, &source_handle);

	// set default a/v streams
	Player_Stream_Create(player_handle, source_handle,
		config.vPID, VideoType, &stream_handle_V);

	Player_Stream_Create(player_handle, source_handle,
		config.aPID, AudioType, &stream_handle_A);

	// move to scan
	stb_scan();

	return 0;
}

int stb_deinit()
{
	puts("stb deinit");
	Tuner_Deinit();
	Player_Source_Close(player_handle, source_handle);
	Player_Deinit(player_handle);
	return 0;
}

static int stb_get_vol()
{
  return 0;
}

int stb_scan()
{
	// parse PAT
	Demux_Register_Section_Filter_Callback(demux_pat_callback);
	Demux_Set_Filter(player_handle, PAT_PID, PAT_TABLE_ID, &filter_handle);

	// TODO: add timed wait
	pthread_mutex_lock(&pat_mutex);
	pthread_cond_wait(&pat_condition, &pat_mutex);
	pthread_mutex_unlock(&pat_mutex);

	Demux_Unregister_Section_Filter_Callback(demux_pat_callback);
	Demux_Free_Filter(player_handle, filter_handle);

	puts("Got PAT!");

	// parse SDT
	/*
	Demux_Register_Section_Filter_Callback(demux_sdt_callback);
	Demux_Set_Filter(player_handle, SDT_PID, SDT_TABLE_ID, &filter_handle);

	pthread_mutex_lock(&sdt_mutex);
	pthread_cond_wait(&sdt_condition, &sdt_mutex);
	pthread_mutex_unlock(&sdt_mutex);

	Demux_Unregister_Section_Filter_Callback(demux_sdt_callback);
	Demux_Free_Filter(player_handle, filter_handle);
	*/

	puts("Got SDT!");

  // TODO: fill services table
  // [ ] create services table from a pat table
  // [ ] fill services table with sdt info
}

int stb_get_current_ch()
{
}

int stb_get_ch_list()
{
	pat_print(&pat);
}

// TODO: make it thread safe
// update current ch
// spawn a new thread
// call zapp handler
int stb_ch_switch(int ch)
{
	// TODO: ugly
  // read from services table
	program_desc_t *prog_desc;
	pat_get_entry(ch, &pat, &prog_desc);

	Demux_Register_Section_Filter_Callback(demux_pmt_callback);
	Demux_Set_Filter(player_handle,
		prog_desc->program_pid, PMT_TABLE_ID, &filter_handle);

	// TODO: timed wait for pmt parser
	pthread_mutex_lock(&pmt_mutex);
	pthread_cond_wait(&pmt_condition, &pmt_mutex);
	pthread_mutex_unlock(&pmt_mutex);

	Demux_Unregister_Section_Filter_Callback(demux_pmt_callback);
	Demux_Free_Filter(player_handle, filter_handle);

	// extract A/V ES PIDs
	int audio_pid = pmt_get_audio_pid(&pmt);
	int video_pid = pmt_get_video_pid(&pmt);

  // not working always, better flag
  // separate those handlers
	Player_Stream_Remove(player_handle, source_handle, stream_handle_V);
	Player_Stream_Remove(player_handle, source_handle, stream_handle_A);

	current_ch = 1;

  // don't set video stream if service is not of a video type
	Player_Stream_Create(player_handle, source_handle,
		video_pid, VIDEO_TYPE_MPEG2, &stream_handle_V);

	Player_Stream_Create(player_handle, source_handle,
		audio_pid, AUDIO_TYPE_MPEG_AUDIO, &stream_handle_A);
}

int stb_ch_up()
{
  // TODO: current ch bump by mod +1 - mod is services cnt
  // update current ch
  // switch to current ch
}

int stb_ch_down()
{
  // TODO: current ch bump by mod -1 - mod is services cnt
  // update current ch
  // switch to current ch
}

int stb_vol_up()
{
  // TODO: limit up volume when -Db is 0
  // update volume +10
  // log formula
  // call driver
}

int stb_vol_down()
{
  // TODO: limit up volume when -Db is small enough
  // update volume +10
  // log formula
  // call driver
}

// TODO: add this to model iface
int stb_vol_mute()
{
  // TODO: set volume to 0
  // log formula
  // call driver
}

void stb_model_init(model_t *model)
{
	model->init	= stb_init;
	model->deinit	= stb_deinit;

	model->scan	= stb_scan;

	model->get_current_ch = stb_get_current_ch;
	model->ch_up	= stb_ch_up;
	model->ch_down	= stb_ch_down;

	model->vol_up	= stb_vol_up;
	model->vol_down	= stb_vol_down;
	model->get_volume = stb_get_vol;

	model->ch_switch = stb_ch_switch;
}
