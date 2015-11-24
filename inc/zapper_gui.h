#ifndef ZAPPER_GUI_H
#define ZAPPER_GUI_H

typedef void(zapper_gui_init_t)(void);
typedef void(zapper_gui_deinit_t)(void);
typedef void(zapper_gui_draw_box_t)(void);
typedef void(zapper_gui_clear_box_t)(void);

typedef struct zapper_gui {
  zapper_gui_init_t *init;
  zapper_gui_deinit_t *deinit;
  zapper_gui_draw_box_t *draw_box;
  zapper_gui_clear_box_t *clear_box;
  int initialized;
} zapper_gui_t;

#endif
