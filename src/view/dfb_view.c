#include <pthread.h>
#include <stdio.h>
#include "view/dfb_view.h"
#include <directfb.h>
#include <string.h>
#include <time.h>
#include <signal.h>

static timer_t render_timer_id;
static struct sigevent render_ev;
static struct itimerspec render_timer_spec, render_timer_spec_old;

static int show_num_input = 0;
static int show_info_bar = 0;

static IDirectFB *dfbInterface = NULL;
static DFBSurfaceDescription surfaceDesc;
static IDirectFBSurface *primary = NULL;

static int screenWidth  = -1;
static int screenHeight = -1;

int pos = 0;

static void dfb_view_log(char *msg)
{
	puts(msg);
}

static void render_number_input()
{
	if(show_num_input)
	{
		primary->SetColor(primary, 0xff, 0x00, 0x00, 0xff);
		primary->FillRectangle(primary, 100, 100, 100, 20);

    // TODO: ch numbers and dashes (text)
	}
}

static void render_info_bar()
{
	if(show_info_bar) {
		primary->SetColor(primary, 0x00, 0x00, 0xff, 0xff);
		primary->FillRectangle(primary, 0, 1000, 1920, 50);

    // TODO:
    // - ch number
    // - ch position
    // - name
    // - type
    // - A/V pid
	}
}

static void render_volume()
{
}

static void *dfb_render_loop(void *args)
{
	// do clear
	primary->SetColor(primary, 0x00, 0x00, 0x00, 0x00);
	primary->FillRectangle(primary, 0, 0, screenWidth, screenHeight);

	render_number_input();
	render_info_bar();
	render_volume();

	// primary->SetColor(primary, 0x00, 0xff, 0x00, 0xff);
	// primary->FillRectangle(primary, pos, 0, 100, screenHeight);
	primary->Flip(primary, NULL, 0);

	pos++;

	if(pos > 100) pos = 0;
}

static void dfb_view_start()
{
	dfb_view_log("starting dfb view...");

	char **argv = malloc(sizeof(char*));
	*argv = malloc(sizeof(char)*32);
	strcpy(*argv, "nothing");
	int argc = 1;

	DirectFBInit(&argc, &argv);

	DirectFBCreate(&dfbInterface);
	dfbInterface->SetCooperativeLevel(dfbInterface, DFSCL_FULLSCREEN);

	surfaceDesc.flags = DSDESC_CAPS;
	surfaceDesc.caps = DSCAPS_PRIMARY | DSCAPS_FLIPPING;
	dfbInterface->CreateSurface(dfbInterface, &surfaceDesc, &primary);
	primary->GetSize(primary, &screenWidth, &screenHeight);

	memset(&render_timer_spec, 0, sizeof(struct itimerspec));
	render_timer_spec.it_value.tv_sec = 0;
	render_timer_spec.it_value.tv_nsec = 16000000;
	render_timer_spec.it_interval.tv_sec = 0;
	render_timer_spec.it_interval.tv_nsec = 50000000;

	render_ev.sigev_notify = SIGEV_THREAD;
	render_ev.sigev_notify_function = dfb_render_loop;
	render_ev.sigev_value.sival_ptr = NULL;
	render_ev.sigev_notify_attributes = NULL;

	timer_create(CLOCK_REALTIME, &render_ev, &render_timer_id);
	timer_settime(render_timer_id, 0, &render_timer_spec, &render_timer_spec_old);

	// primary->Release(primary);
	// dfbInterface->Release(dfbInterface);
}

static void dfb_show_num_input(char *buff)
{
	show_num_input = 1;
}

static void dfb_hide_num_input()
{
	show_num_input = 0;
}

// TODO: accept service item
// TODO: if NULL, show last
static void dfb_show_info_bar(int ch)
{
	show_info_bar = 1;
}

static void dfb_hide_info_bar()
{
	show_info_bar = 0;
}

// TODO: valume view items
static void dfb_show_volume(int vol)
{
}

static void dfb_hide_volume()
{
}

int dfb_view_init(view_t *view)
{
	view->log = dfb_view_log;
	view->start = dfb_view_start;

	view->show_num_input = dfb_show_num_input;
	view->hide_num_input = dfb_hide_num_input;

	view->show_info_bar = dfb_show_info_bar;
	view->hide_info_bar = dfb_hide_info_bar;


  // TODO: register volume handlers

	return 0;
}

// TODO: propper deinit
int dfb_view_deinit()
{
}
