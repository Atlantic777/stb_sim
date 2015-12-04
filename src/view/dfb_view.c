#include <pthread.h>
#include <stdio.h>
#include "view/dfb_view.h"
#include "model/service_list.h"
#include <directfb.h>
#include <string.h>
#include <time.h>
#include <signal.h>

static timer_t render_timer_id;
static struct sigevent render_ev;
static struct itimerspec render_timer_spec, render_timer_spec_old;

static int show_num_input = 0;
static int show_info_bar = 0;
static int show_volume_level = 0;

static IDirectFB *dfbInterface = NULL;
static DFBSurfaceDescription surfaceDesc;
static IDirectFBSurface *primary = NULL;

static IDirectFBFont *fontInterface = NULL;
static DFBFontDescription fontDescription;

static int screenWidth  = -1;
static int screenHeight = -1;

static service_item_t *current_ch;
static char ch_info_str[256];
static int vol_level = 0;

IDirectFBImageProvider *provider;
IDirectFBSurface *logoSurface = NULL;
int32_t logoHeight, logoWidth;

int pos = 0;

static void dfb_view_log(char *msg)
{
	puts(msg);
}

static void render_number_input()
{
	if(show_num_input)
	{
    // TODO: bg color, position and text
		primary->SetColor(primary, 0xff, 0x00, 0x00, 0xff);
		primary->FillRectangle(primary, 100, 100, 100, 20);

    // primary->SetColor(primary, 0xff, 0xff, 0xff, 0xff);
    // primary->DrawString(primary, -1, num_input_buff,
    //    1640, 920, DSTF_LEFT);
	}
}

static void render_info_bar()
{
	if(show_info_bar) {
    // TODO: bg color, posotion and text
		primary->SetColor(primary, 0x00, 0x00, 0xff, 0xff);
		primary->FillRectangle(primary, 384, 60, 384*3, 240);

    // primary->SetColor(primary, 0xff, 0xff, 0xff, 0xff);
    // write channel
    // primary->DrawString(primary, -1, )
    // write prog number
    // write ttx support
    // write service name
    // write service type
    /*
		primary->SetColor(primary, 0xff, 0xff, 0xff, 0xff);
		primary->DrawString(primary, ch_info_str, -1, 100, 100, DSTF_LEFT);
    */
	}
}

static void render_volume()
{
  if(show_volume_level)
  {
    // TODO: vol position
    // blit propper image from array
    // to standard position
  }
}

static void *dfb_render_loop(void *args)
{
	// do clear
	primary->SetColor(primary, 0x00, 0x00, 0x00, 0x00);
	primary->FillRectangle(primary, 0, 0, screenWidth, screenHeight);

	render_number_input();
	render_info_bar();
	render_volume();

	primary->Flip(primary, NULL, 0);
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

	fontDescription.flags = DFDESC_HEIGHT;
	fontDescription.height = 48;

	dfbInterface->CreateFont(dfbInterface, "/home/galois/fonts/DejaVuSans.ttf",
		&fontDescription, &fontInterface);

	/*
	dfbInterface->CreateImageProvider(dfbInterface, "dfblogo_alpha.png", &provider);
	provider->GetSurfaceDescription(provider, &surfaceDesc);
	dfbInterface->CreateSurface(dfbInterface, &surfaceDesc, &logoSurface);

	provider->RenderTo(provider, logoSurface, NULL);
	provider->Release(provider);
	logoSurface->GetSize(logoSurface, &logoWidth, &logoHeight);
	*/

	primary->SetFont(primary, fontInterface);
}

static void dfb_show_num_input(char *buff)
{
	puts("show num input");
	show_num_input = 1;
}

static void dfb_hide_num_input()
{
	puts("hide num input");
	show_num_input = 0;
}

static void dfb_show_info_bar(service_item_t *ch)
{
	puts("show_info_bar");
	show_info_bar = 1;
	current_ch = ch;
	sprintf(ch_info_str, "%d - %d", ch->position, ch->program_number);
}

static void dfb_hide_info_bar()
{
	puts("hide info bar");
	show_info_bar = 0;
}

// TODO: valume view items
static void dfb_show_volume(int vol)
{
  show_volume_level = 1;
  vol_level = vol/10;
}

static void dfb_hide_volume()
{
  show_volume_level = 0;
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
  view->show_volume = dfb_show_volume;
  view->hide_volume = dfb_hide_volume;

	return 0;
}

// TODO: propper deinit
int dfb_view_deinit()
{
}
