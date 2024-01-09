#ifndef __UART_H__
#define __UART_H__

#include <component.h>
#include <term.h>
#include <string>

class UART : public Component{
private:
  Term *tx_term, *rx_term;
  int tx_state, rx_state;
  uint16_t divisor;
  uint8_t tx_data, rx_data;
  std::string rx_input;
  std::string rx_sending_str;
  static constexpr const char *rx_input_prompt = "(Press Enter to issue): ";
  bool tx_update_gui, rx_update_gui;
  uint8_t *p_tx;
public:
  UART(SDL_Renderer *rend, int cnt, int init_val, int ct, int x, int y, int w, int h);
  ~UART();
  void set_divisor(uint16_t d);

  virtual void update_gui();
  virtual void update_state();
  void tx_receive();
  void rx_send();
  void rx_getchar(uint8_t ch);
  void rx_term_focus(bool v);
};

#endif
