#ifndef ZAPPER_GUI_GL_H
#define ZAPPER_GUI_GL_H

#include "zapper_gui.h"

extern zapper_gui_t Zapper_gui_gl;

void zapper_gui_gl_init();
void zapper_gui_gl_deinit();
void draw();
void draw_box();
void clear_box();

#endif
