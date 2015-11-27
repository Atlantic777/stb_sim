#ifndef AFB_H
#define AFB_H

typedef struct afb {
  void (*init)();
  void (*clear)();
  void (*write_text)(int x, int y, char *text);
  void (*set_color)(int r, int g, int b, int a);
  void (*draw_rect)(int x, int y, int w, int h);
  // void (*blit_bmp)()
} afb_t;

#endif
