#include <keyboard.h>
#include <nvboard.h>
#include <macro.h>
#include <assert.h>

KEYBOARD* kb;

#define FILL_KEYMAP_FIRST(a) keymap_first[SDL_PREFIX(a)] = GET_FIRST(AT_PREFIX(a));
#define FILL_KEYMAP_DECOND(a) keymap_second[SDL_PREFIX(a)] = GET_SECOND(AT_PREFIX(a));

static int keymap_first[256] = {};
static int keymap_second[256] = {};

void keymap_init(){
  MAP(SCANCODE_LIST, FILL_KEYMAP_FIRST)
  MAP(SCANCODE_LIST, FILL_KEYMAP_DECOND)
}

int sdl2at(int scancode, int is_first){
  return is_first? keymap_first[scancode] : keymap_second[scancode];
}

KEYBOARD::KEYBOARD(SDL_Renderer *rend, int cnt, int init_val, int it, int ct):
  Component(rend, cnt, init_val, it, ct),
  data_idx(0), left_clk(0), cur_key(NOT_A_KEY),
  ps2_clk(&input_map[PS2_CLK]), ps2_dat(&input_map[PS2_DAT]){
    keymap_init();
  }


void KEYBOARD::push_key(uint8_t sdl_key, bool is_keydown){
  uint8_t at_key = sdl2at(sdl_key, 1);
  if(at_key == 0xe0){
    all_keys.push(0xe0);
    at_key = sdl2at(sdl_key, 0);
  }
  if(!is_keydown) all_keys.push(0xf0);
  all_keys.push(at_key);
}

void KEYBOARD::update_state(){
  if(cur_key == NOT_A_KEY){
    if(all_keys.empty()) return;
    cur_key = all_keys.front();
    assert(data_idx == 0);
    left_clk = CLK_NUM;
    *ps2_clk == 1;
  }

  if(left_clk == 0){
    *ps2_clk = !*ps2_clk;
    left_clk = CLK_NUM;
    if(*ps2_clk){
      assert(!all_keys.empty());
      *ps2_dat = (data_idx == PS2_PARTIAL) ? !UINT8_XOR(all_keys.front()) : \
                 (data_idx == PS2_STOP) ? 1 : \
                 ((data_idx >= PS2_DATA_0) && (data_idx <= PS2_DATA_7)) ? (cur_key & 1) : 0;
      if((data_idx >= PS2_DATA_0) && (data_idx <= PS2_DATA_7)) cur_key >>= 1;
      data_idx ++;
    } else if(data_idx == 11){
      data_idx = 0;
      cur_key = NOT_A_KEY;
      all_keys.pop();
    }
  }
  else{
    left_clk --;
  }
}
