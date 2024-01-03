#ifndef _NVBOARD_UART_H
#define _NVBOARD_UART_H

#include <component.h>
#include <string>

class UART : public Component{
private:
  int region_w, region_h;
  std::string str;
public:
  UART(SDL_Renderer *rend, int cnt, int init_val, int ct);
  ~UART();

  virtual void update_gui();
  virtual void update_state();
};

#endif
