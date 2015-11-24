#ifndef VIEW_H
#define VIEW_H

typedef struct view {
  void (*log)(char*);

  void (*show_num_input)(char*);
  void (*hide_num_input)();

  void (*show_info_bar)(int chan);
  void (*hide_info_bar)();

  void (*start)();
} view_t;

#endif
