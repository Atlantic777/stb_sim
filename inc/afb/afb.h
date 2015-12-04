/**
 * @file afb.h
 * @author Nikola Hardi
 * @date November 2015
 * @brief [UNUSED] Abstract Frame Buffer -
 * interface to various graphic libraries
 *
 * Each implementation of AFB should support basic
 * graphic operations like:
 *  - clear framebuffer
 *  - set color
 *  - draw a rectangle
 *  - write text
 *  - blit bitmap image
 */
#ifndef AFB_H
#define AFB_H

/**
 * @brief [UNUSED]
 * Interface to various graphic libraries (OpenGL, DirectFB)
 */
typedef struct afb {
  void (*init)();
  void (*clear)();
  void (*write_text)(int x, int y, char *text);
  void (*set_color)(int r, int g, int b, int a);
  void (*draw_rect)(int x, int y, int w, int h);
  // void (*blit_bmp)()
} afb_t;

#endif
