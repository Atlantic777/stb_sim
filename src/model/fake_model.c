#include <stdint.h>
#include <pthread.h>
#include "model/fake_model.h"
#include "parsers/pat_parser.h"
#include "parsers/pmt_parser.h"
/* #include "model/fake_stb.h" */
/* #include "parsers/example_pat.h" */
/* #include "parsers/example_pmt.h" */
#include <stdio.h>

#define DESIRED_FREQUENCY 754000000
#define BANDWIDTH 8
#define PAT_PID 0
#define PAT_TABLE_ID 0
#define PMT_TABLE_ID 2

static int32_t current_ch = 0;

static int stb_fake_scan();
static int stb_fake_get_current_ch();
static int stb_fake_get_ch_list();

static int stb_fake_ch_switch(int ch);
static int stb_fake_ch_up();
static int stb_fake_ch_down();

static int stb_fake_vol_up();
static int stb_fake_vol_down();

static pthread_cond_t status_condition = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t status_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t pat_condition = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t pat_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t pmt_condition = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t pmt_mutex = PTHREAD_MUTEX_INITIALIZER;

static uint32_t player_handle, source_handle, filter_handle;
static uint32_t stream_handle_A, stream_handle_V;

static pat_table_t pat;
static pmt_table_t pmt;

typedef uint32_t t_LockStatus;

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

	puts("PAT parsed");
	pat_print(&pat);
}

static int32_t demux_pmt_callback(uint8_t *buff)
{
	pthread_mutex_lock(&pmt_mutex);
	pmt_parse(&pmt, buff);
	pthread_cond_signal(&pmt_condition);
	pthread_mutex_unlock(&pmt_mutex);

	puts("PMT parsed");
	pmt_print(&pmt);
}
// ##### END OF CALLBACKS ############

int stb_fake_init()
{
	puts("stb init");
	/* Tuner_Init(); */ // fail randomly
	/* Tuner_Register_Status_Callback(lock_status_callback); */
	/* Tuner_Lock_To_Frequency(DESIRED_FREQUENCY, BANDWIDTH, DVB_T); */ // random wait

	// TODO: add timed wait
	/* pthread_mutex_lock(&status_mutex); */
	/* pthread_cond_wait(&status_condition, &status_mutex); */
	/* pthread_mutex_unlock(&status_mutex); */

	/* Player_Init(&player_handle); */ // fail randomly
	/* Player_Source_Open(player_handle, &source_handle); */ // fail randomly

	// move to scan
	/* stb_fake_scan(); */

	return 0;
}

int stb_fake_deinit()
{
	puts("stb deinit");
	/* Tuner_Deinit(); */
	/* Player_Source_Close(player_handle, source_handle); */
	/* Player_Deinit(player_handle); */
	return 0;
}

int stb_fake_scan()
{
	/* Demux_Register_Section_Filter_Callback(demux_pat_callback); */
	/* Demux_Set_Filter(player_handle, PAT_PID, PAT_TABLE_ID, &filter_handle); */
  // sched callback call with random wait or never, do repeat until deregister

	// TODO: add timed wait
	pthread_mutex_lock(&pat_mutex);
	pthread_cond_wait(&pat_condition, &pat_mutex);
	pthread_mutex_unlock(&pat_mutex);

	/* Demux_Unregister_Section_Filter_Callback(demux_pat_callback); */
	/* Demux_Free_Filter(player_handle, filter_handle); */
}

int stb_fake_get_current_ch()
{
}

int stb_fake_get_ch_list()
{
	pat_print(&pat);
}

int stb_fake_ch_switch(int ch)
{
	puts("switching chann");
	/* program_desc_t *prog_desc; */
	/* pat_get_entry(ch, &pat, &prog_desc); */
  /*  */
	current_ch = ch;

	/* Demux_Register_Section_Filter_Callback(demux_pmt_callback); */
	/* Demux_Set_Filter(player_handle, */
	/* 	prog_desc->program_pid, PMT_TABLE_ID, &filter_handle); */

	// TODO: timed wait for pmt parser
	/* pthread_mutex_lock(&pmt_mutex); */
	/* pthread_cond_wait(&pmt_condition, &pmt_mutex); */
	/* pthread_mutex_unlock(&pmt_mutex); */

	/* Demux_Unregister_Section_Filter_Callback(demux_pmt_callback); */
	/* Demux_Free_Filter(player_handle, filter_handle); */

	/* pmt_print(&pmt); */

	// extract A/V ES PIDs
	/* int audio_pid = pmt_get_audio_pid(&pmt); */
	/* int video_pid = pmt_get_video_pid(&pmt); */
  /*  */
	/* printf("Audio pid: %d\n", audio_pid); */
	/* printf("Video pid: %d\n", video_pid); */

	/* if(current_ch != 0) */
	/* { */
		// remove current stream
		/* Player_Stream_Remove(player_handle, source_handle, stream_handle_V); */
		/* Player_Stream_Remove(player_handle, source_handle, stream_handle_A); */
	/* } */

	/* Player_Stream_Create(player_handle, source_handle, */
	/* 	video_pid, VIDEO_TYPE_MPEG2, &stream_handle_V); */
  /*  */
	/* Player_Stream_Create(player_handle, source_handle, */
	/* 	audio_pid, AUDIO_TYPE_MPEG_AUDIO, &stream_handle_A); */
}

int stb_fake_ch_up()
{

}

int stb_fake_ch_down()
{

}

int stb_fake_vol_up()
{
}

int stb_fake_vol_down()
{
}

static int stb_fake_get_vol()
{
  return 0;
}

void stb_fake_model_init(model_t *model)
{
	model->init	= stb_fake_init;
	model->deinit	= stb_fake_deinit;

	model->scan	= stb_fake_scan;

	model->get_current_ch = stb_fake_get_current_ch;
	model->ch_up	= stb_fake_ch_up;
	model->ch_down	= stb_fake_ch_down;

	model->vol_up	= stb_fake_vol_up;
	model->vol_down	= stb_fake_vol_down;
  model->get_volume = stb_fake_get_vol;

	model->ch_switch = stb_fake_ch_switch;
}
