#include <tdp_api.h>
#include "model.h"

void lock_status_callback()
{
}

int stb_init()
{
	// init tuner
	// register tuner status callback
	// tuner lock to frequency
	// player source open
	// move to scan
	puts("stb init");
	Tuner_Init();
	Tuner_Register_Status_Callback(lock_status_callback);
	return 0;
}

int stb_deinit()
{
	puts("stb deinit");
	Tuner_Deinit();
	return 0;
}

/*
int stb_scan();

int stb_get_current_ch();
int stb_get_ch_list();

int stb_ch_switch(int ch);
int stb_ch_up();
int stb_ch_down();

int stb_vol_up();
int stb_vol_down();
*/

void stb_model_init(model_t *model)
{
	model->init	= stb_init;
	model->deinit	= stb_deinit;

	/*
	model->scan	= stb_scan;

	model->get_current_ch = stb_get_current_ch;
	model->ch_up	= stb_ch_up;
	model->ch_down	= stb_ch_down;
	
	model->vol_up	= stb_vol_up;
	model->vol_down	= stb_vol_down;
	*/
}
