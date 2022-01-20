#ifndef _VFPGA_VGA_H
#define _VFPGA_VGA_H

#include "component.h"

class VGA: public Component{
public:
  VGA(SDL_Renderer *rend);
  virtual void update_gui(int newval);
  virtual void update_state();
};

#endif
