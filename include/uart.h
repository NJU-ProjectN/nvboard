#ifndef _NVBOARD_UART_H
#define _NVBOARD_UART_H

#include <component.h>
#include <term.h>
#include <string>

class UART : public Component{
private:
  Term *tx_term, *rx_term;
  int state;
  uint16_t divisor;
  uint8_t data;
  std::string rx_input;
  static constexpr const char *rx_input_prompt = "UART TX (Press Enter to issue): ";
  bool tx_update_gui, rx_update_gui;
  uint8_t *p_tx;
public:
  UART(SDL_Renderer *rend, int cnt, int init_val, int ct, int x, int y, int w, int h);
  ~UART();
  void set_divisor(uint16_t d);

  virtual void update_gui();
  virtual void update_state();
  void tx_check();
  void rx_getchar(uint8_t);
};

#endif
