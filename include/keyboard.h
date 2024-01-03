#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <queue>
#include <component.h>

#define NOT_A_KEY -1
#define CLK_NUM 10
#define PS2_START 0
#define PS2_DATA_0 1
#define PS2_DATA_7 8
#define PS2_PARTIAL 9
#define PS2_STOP 10

#define UINT2_XOR(a) (((a) >> 1) ^ ((a) & 0b1))
#define UINT4_XOR(a) UINT2_XOR(((a) >> 2) ^ ((a) & 0b11))
#define UINT8_XOR(a) UINT4_XOR(((a) >> 4) ^ ((a) & 0b1111))

class KEYBOARD : public Component{
  private:
    std::queue <uint8_t> all_keys;
    int data_idx;
    int left_clk;
    int cur_key;

  public:
    KEYBOARD(SDL_Renderer *rend, int cnt, int init_val, int ct);
    ~KEYBOARD();
    void push_key(uint8_t scancode, bool is_keydown);
    virtual void update_state();
};

#endif
