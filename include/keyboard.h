#ifndef _VFPGA_KEYBOARD_H
#define _VFPGA_KEYBOARD_H

#include <queue>
#include <assert.h>
#include <nvboard.h>
#include <map>
#include <at_scancode.h>

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
    uint64_t* ps2_clk;
    uint64_t* ps2_dat;

  public:
    KEYBOARD(SDL_Renderer *rend, int cnt, int init_val, int it, int ct);
    ~KEYBOARD();
    void push_key(uint8_t scancode, bool is_keydown);
    virtual void update_state();
};

#endif
