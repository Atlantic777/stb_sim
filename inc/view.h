#ifndef VIEW_H
#define VIEW_H

typedef struct view {
  void (*log)(char*);
} view_t;

#endif
