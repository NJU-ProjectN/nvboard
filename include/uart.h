#ifndef _NVBOARD_UART_H
#define _NVBOARD_UART_H

#include <component.h>
#include <term.h>

class UART : public Component{
private:
  Term *term;
  int state;
  uint16_t divisor;
  uint16_t divisor_cnt;
  uint8_t data;
  bool need_update_gui;
public:
  UART(SDL_Renderer *rend, int cnt, int init_val, int ct, int x, int y, int w, int h);
  ~UART();
  void set_divisor(uint16_t d);

  virtual void update_gui();
  virtual void update_state();
  void check_tx();
};

#endif
