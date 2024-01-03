#ifndef _NVBOARD_UART_H
#define _NVBOARD_UART_H

#include <component.h>
#include <term.h>

class UART : public Component{
private:
  Term *term;
public:
  UART(SDL_Renderer *rend, int cnt, int init_val, int ct, int x, int y, int w, int h);
  ~UART();

  virtual void update_gui();
  virtual void update_state();
};

#endif
