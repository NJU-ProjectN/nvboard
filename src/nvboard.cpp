#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

#include <nboard.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <assert.h>

SDL_Window *main_window = nullptr;
SDL_Renderer *main_renderer = nullptr;

std::string nboard_home;

static int read_event_flag = true;
static int render_flag = true;
static void alarm_sig_handler(int signum){
    read_event_flag = true;
    render_flag = true;
}

void nvboard_init(){
    printf("nvboard v0.2\n");
    // init SDL and SDL_image
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_PNG);

    main_window = SDL_CreateWindow("nvboard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH * 2, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    main_renderer = SDL_CreateRenderer(main_window, -1, 
    #ifdef VSYNC
        SDL_RENDERER_PRESENTVSYNC |
    #endif
    #ifdef HARDWARE_ACC
        SDL_RENDERER_ACCELERATED |
    #else
        SDL_RENDERER_SOFTWARE |
    #endif
        0
    );
    
    // To avoid the SDL bugs on hby's linux
    //usleep(200000);

    nboard_home = getenv("NBOARD_HOME");
    
    load_background(main_renderer);
    load_texture(main_renderer);
    //dbg_wait_esc("finish loading bg");
    init_components(main_renderer);
    init_gui(main_renderer);
    //dbg_wait_esc("finish init gui");
    init_input();
    init_output();

    dut_update();
    update_components(main_renderer);
    //dbg_wait_esc("finish update");
    struct sigaction s;
    memset(&s, 0, sizeof(s));
    s.sa_handler = alarm_sig_handler;
    int ret = sigaction(SIGVTALRM, &s, NULL);
    assert(ret == 0);

    struct itimerval it = {};
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 1000000 / 60;
    it.it_interval = it.it_value;
    ret = setitimer(ITIMER_VIRTUAL, &it, NULL);
    assert(ret == 0);
}

void nvboard_update(output_pin pin, int val){
    output_map[pin] = val;
    update_components(main_renderer);
    if(render_flag) {
        SDL_RenderPresent(main_renderer);
        render_flag = false;
    }
}

int nvboard_getval(input_pin pin){
    return input_map[pin];
}

void nvboard_quit(){
    delete_components();
    SDL_DestroyWindow(main_window);
    SDL_DestroyRenderer(main_renderer);
    IMG_Quit();
    SDL_Quit();
}


void nvboard_render() {
    SDL_RenderPresent(main_renderer);
}

int nvboard_event_handler(){
    if(!read_event_flag) return 0;
    read_event_flag = false;
    int ev = read_event();
    if(ev != -1){
        update_components(main_renderer);
    }
    return ev;
}