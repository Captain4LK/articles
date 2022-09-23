#ifndef _RCG_H_

#define _RCG_H_

//This file can be compiled with the following command: ``gcc articles/rcg/chapter1_3.c -DRCG_EXAMPLE -DRCG_IMPLEMENTATION -lSDL2 -Og -g``

///////////////////////
/// ---
/// title: "Retro Computer Graphics - Chapter 1.3 - Basic drawing"
/// ---
///
/// Introduction
/// ---------------------------
///
/// In this article we'll implement a few simple drawing routines.
///
/// Instead of using a 24bit color framebuffer directly, the framebuffer instead contains indices into an array of colors (the color palette). Since our framebuffer is an array of 8 bit integers, we have a maximum of 255 (0 is reserved for transparancy) color availible. This might seem too low to create 3d graphics, including basic lighting, at first, but in later articles we are going to discuss techniques helping to use these colors to their maximum.
///
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#define RCG_XRES 640
#define RCG_YRES 480

#define RCG_FPS 30

typedef enum 
{
   RCG_KEY_NONE,

   //Keyboard keys
   RCG_KEY_A,RCG_KEY_B,RCG_KEY_C,RCG_KEY_D,RCG_KEY_E,RCG_KEY_F,RCG_KEY_G,
   RCG_KEY_H,RCG_KEY_I,RCG_KEY_J,RCG_KEY_K,RCG_KEY_L,RCG_KEY_M,RCG_KEY_N,
   RCG_KEY_O,RCG_KEY_P,RCG_KEY_Q,RCG_KEY_R,RCG_KEY_S,RCG_KEY_T,RCG_KEY_U,
   RCG_KEY_V,RCG_KEY_W,RCG_KEY_X,RCG_KEY_Y,RCG_KEY_Z,
   RCG_KEY_0,RCG_KEY_1,RCG_KEY_2,RCG_KEY_3,RCG_KEY_4,RCG_KEY_5,
   RCG_KEY_6,RCG_KEY_7,RCG_KEY_8,RCG_KEY_9, RCG_KEY_COMMA, RCG_KEY_PERIOD,
   RCG_KEY_F1,RCG_KEY_F2,RCG_KEY_F3,RCG_KEY_F4,RCG_KEY_F5,RCG_KEY_F6,
   RCG_KEY_F7,RCG_KEY_F8,RCG_KEY_F9,RCG_KEY_F10,RCG_KEY_F11,RCG_KEY_F12,
   RCG_KEY_UP,RCG_KEY_DOWN,RCG_KEY_LEFT,RCG_KEY_RIGHT,
   RCG_KEY_SPACE,RCG_KEY_TAB,RCG_KEY_LSHIFT,RCG_KEY_RSHIFT,RCG_KEY_LCTRL,RCG_KEY_RCTRL,
   RCG_KEY_LALT, RCG_KEY_RALT, RCG_KEY_INS,
   RCG_KEY_DEL,RCG_KEY_HOME,RCG_KEY_END,RCG_KEY_PGUP,RCG_KEY_PGDN,
   RCG_KEY_BACK,RCG_KEY_ESCAPE,RCG_KEY_RETURN,RCG_KEY_ENTER,RCG_KEY_PAUSE,RCG_KEY_SCROLL,
   RCG_KEY_NP0,RCG_KEY_NP1,RCG_KEY_NP2,RCG_KEY_NP3,RCG_KEY_NP4,RCG_KEY_NP5,RCG_KEY_NP6,RCG_KEY_NP7,RCG_KEY_NP8,RCG_KEY_NP9,
   RCG_KEY_NP_MUL,RCG_KEY_NP_DIV,RCG_KEY_NP_ADD,RCG_KEY_NP_SUB,RCG_KEY_NP_DECIMAL, RCG_KEY_NP_ENTER,

   //Mouse buttons
   //Functions for reading input won't differentiate between mouse and keyboard input.
   //This makes remapping keys easier, which won't be discussed in this series, though.
   RCG_BUTTON_LEFT,RCG_BUTTON_RIGHT,RCG_BUTTON_MIDDLE,RCG_BUTTON_X1,RCG_BUTTON_X2,

   //Last element, will be used as size for mapping array
   RCG_KEY_MAX,
}RCG_key;

//Additionally, we'll need a way to represent color, the rendering itself will be palettized, max 256 colors, but we'll still need to store a color palette in 24bit color
typedef struct
{
   uint8_t r,g,b,a;
}RCG_color;

//Creates a sdl window, the framebuffer and initializes keycode LUTs
void RCG_init(const char *title);

//Runs the sdl2 event loop, updates key states
void RCG_update(void);

//Returns, whether the app should keep running
int RCG_running(void);

//Makes RCG_running return false
void RCG_quit(void);

//Uploads the framebuffer to the screen
void RCG_render_present(void);

//Hides the cursor and only reports relative mouse positions
void RCG_mouse_relative(int relative);

//Hides the cursor
void RCG_mouse_show(int show);

//Returns whether a key is held down
int RCG_key_down(RCG_key key);

//Returns whether a key has been pressed this frame
int RCG_key_pressed(RCG_key key);

//Returns whether a key has been released this frame
int RCG_key_released(RCG_key key);

//Returns how much the mouse was scrolled this frame
int RCG_mouse_wheel_scroll(void);

//Writes the mouses pos into x and y
void RCG_mouse_pos(int *x, int *y);

//Writes how much the mouse was moved into x and y
void RCG_mouse_relative_pos(int *x, int *y);

//Returns a pointer to the framebuffer
uint8_t *RCG_framebuffer(void);

//Loads a palette from a .hex file
void RCG_palette_load(const char *path);

//Returns a pointer to the palette
RCG_color *RCG_palette(void);

/// Function prototypes
/// ---------------------------
///<C
//
void RCG_draw_clear(uint8_t color);

void RCG_draw_line_vertical(int x, int y0, int y1, uint8_t color);

void RCG_draw_line_horizontal(int x0, int y, int x1, uint8_t color);

void RCG_draw_line(int x0, int y0, int x1, int y1, uint8_t color);

void RCG_draw_rectangle(int x, int y, int width, int height, uint8_t color);

void RCG_draw_rectangle_fill(int x, int y, int width, int height, uint8_t color);
///>

#endif

#ifdef RCG_IMPLEMENTATION
#ifndef RCG_IMPLEMENTATION_ONCE

static void rcg_update_viewport(void);

static SDL_Texture *rcg_sdl_texture;
static SDL_Renderer *rcg_sdl_renderer;
static SDL_Window *rcg_sdl_window;

//Input
static uint8_t rcg_key_map[SDL_NUM_SCANCODES];
static uint8_t rcg_mouse_map[6];
static uint8_t rcg_new_key_state[RCG_KEY_MAX];
static uint8_t rcg_old_key_state[RCG_KEY_MAX];
static int rcg_mouse_wheel;

//framebuffer drawing position
static int rcg_view_x;
static int rcg_view_y;
static int rcg_view_width;
static int rcg_view_height;
static float rcg_pixel_scale;

//fps limiting
//These could be static variables in RCG_update() instead, but we might want to read them for benchmarking later
static uint64_t rcg_frametime;
static uint64_t rcg_framedelay;
static uint64_t rcg_framestart;

static uint8_t *rcg_framebuffer = NULL;
static int rcg_running = 1;

static RCG_color rcg_palette[256];

void RCG_palette_load(const char *path)
{
   FILE *f = fopen(path,"r");
   if(NULL==f)
      return;

   //Read the input file line by line and parse the input using sscanf()
   //Alternatively you could not use sscanf() and parse the string manually,
   //which due to the simple format of the file would be very easy to implement in this case
   char buffer[512];
   int color = 0;
   while(fgets(buffer,512,f))
   {
      unsigned r,g,b;
      sscanf(buffer,"%2x%2x%2x",&r,&g,&b);
      rcg_palette[color].r = r;
      rcg_palette[color].g = g;
      rcg_palette[color].b = b;
      rcg_palette[color].a = 255;
      color++;
   }

   fclose(f);
}

RCG_color *RCG_palette(void)
{
   return rcg_palette;
}

void RCG_init(const char *title)
{
   Uint32 flags = SDL_INIT_VIDEO|SDL_INIT_EVENTS;
   SDL_Init(flags);

   //Calculate max possible window scale
   //This code is not necessarily needed, sine the window will be resizable
   //It's only here to make the window not be tiny on high-res displays
   int scale = 1;
   int max_x,max_y;
   SDL_Rect max_size;
   SDL_GetDisplayUsableBounds(0,&max_size);
   max_x = max_size.w/RCG_XRES;
   max_y = max_size.h/RCG_YRES;
   scale = (max_x>max_y)?max_y:max_x;

   //Create window, renderer and texture for framebuffer
   rcg_sdl_window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,RCG_XRES*scale,RCG_YRES*scale,SDL_WINDOW_RESIZABLE);
   rcg_sdl_renderer = SDL_CreateRenderer(rcg_sdl_window,-1,SDL_RENDERER_ACCELERATED);
   SDL_SetRenderDrawColor(rcg_sdl_renderer,0,0,0,0);
   rcg_sdl_texture = SDL_CreateTexture(rcg_sdl_renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STREAMING,RCG_XRES,RCG_YRES);
   SDL_SetTextureBlendMode(rcg_sdl_texture,SDL_BLENDMODE_NONE);

   rcg_framedelay = SDL_GetPerformanceFrequency()/RCG_FPS;

   //Implementation and purpose will be discussed later in this article.
   rcg_update_viewport();

   rcg_framebuffer = malloc(RCG_XRES*RCG_YRES);
   memset(rcg_framebuffer,0,RCG_XRES*RCG_YRES);

   rcg_key_map[0x00] = RCG_KEY_NONE;
   rcg_key_map[SDL_SCANCODE_A] = RCG_KEY_A;
   rcg_key_map[SDL_SCANCODE_B] = RCG_KEY_B;
   rcg_key_map[SDL_SCANCODE_C] = RCG_KEY_C;
   rcg_key_map[SDL_SCANCODE_D] = RCG_KEY_D;
   rcg_key_map[SDL_SCANCODE_E] = RCG_KEY_E;
   rcg_key_map[SDL_SCANCODE_F] = RCG_KEY_F;
   rcg_key_map[SDL_SCANCODE_G] = RCG_KEY_G;
   rcg_key_map[SDL_SCANCODE_H] = RCG_KEY_H;
   rcg_key_map[SDL_SCANCODE_I] = RCG_KEY_I;
   rcg_key_map[SDL_SCANCODE_J] = RCG_KEY_J;
   rcg_key_map[SDL_SCANCODE_K] = RCG_KEY_K;
   rcg_key_map[SDL_SCANCODE_L] = RCG_KEY_L;
   rcg_key_map[SDL_SCANCODE_M] = RCG_KEY_M;
   rcg_key_map[SDL_SCANCODE_N] = RCG_KEY_N;
   rcg_key_map[SDL_SCANCODE_O] = RCG_KEY_O;
   rcg_key_map[SDL_SCANCODE_P] = RCG_KEY_P;
   rcg_key_map[SDL_SCANCODE_Q] = RCG_KEY_Q;
   rcg_key_map[SDL_SCANCODE_R] = RCG_KEY_R;
   rcg_key_map[SDL_SCANCODE_S] = RCG_KEY_S;
   rcg_key_map[SDL_SCANCODE_T] = RCG_KEY_T;
   rcg_key_map[SDL_SCANCODE_U] = RCG_KEY_U;
   rcg_key_map[SDL_SCANCODE_V] = RCG_KEY_V;
   rcg_key_map[SDL_SCANCODE_W] = RCG_KEY_W;
   rcg_key_map[SDL_SCANCODE_X] = RCG_KEY_X;
   rcg_key_map[SDL_SCANCODE_Y] = RCG_KEY_Y;
   rcg_key_map[SDL_SCANCODE_Z] = RCG_KEY_Z;

   rcg_key_map[SDL_SCANCODE_F1] = RCG_KEY_F1;
   rcg_key_map[SDL_SCANCODE_F2] = RCG_KEY_F2;
   rcg_key_map[SDL_SCANCODE_F3] = RCG_KEY_F3;
   rcg_key_map[SDL_SCANCODE_F4] = RCG_KEY_F4;
   rcg_key_map[SDL_SCANCODE_F5] = RCG_KEY_F5;
   rcg_key_map[SDL_SCANCODE_F6] = RCG_KEY_F6;
   rcg_key_map[SDL_SCANCODE_F7] = RCG_KEY_F7;
   rcg_key_map[SDL_SCANCODE_F8] = RCG_KEY_F8;
   rcg_key_map[SDL_SCANCODE_F9] = RCG_KEY_F9;
   rcg_key_map[SDL_SCANCODE_F10] = RCG_KEY_F10;
   rcg_key_map[SDL_SCANCODE_F11] = RCG_KEY_F11;
   rcg_key_map[SDL_SCANCODE_F12] = RCG_KEY_F12;

   rcg_key_map[SDL_SCANCODE_DOWN] = RCG_KEY_DOWN;
   rcg_key_map[SDL_SCANCODE_LEFT] = RCG_KEY_LEFT;
   rcg_key_map[SDL_SCANCODE_RIGHT] = RCG_KEY_RIGHT;
   rcg_key_map[SDL_SCANCODE_UP] = RCG_KEY_UP;
   rcg_key_map[SDL_SCANCODE_RETURN] = RCG_KEY_ENTER;

   rcg_key_map[SDL_SCANCODE_BACKSPACE] = RCG_KEY_BACK;
   rcg_key_map[SDL_SCANCODE_ESCAPE] = RCG_KEY_ESCAPE;
   rcg_key_map[SDL_SCANCODE_TAB] = RCG_KEY_TAB;
   rcg_key_map[SDL_SCANCODE_DELETE] = RCG_KEY_DEL;
   rcg_key_map[SDL_SCANCODE_HOME] = RCG_KEY_HOME;
   rcg_key_map[SDL_SCANCODE_END] = RCG_KEY_END;
   rcg_key_map[SDL_SCANCODE_PAGEUP] = RCG_KEY_PGUP;
   rcg_key_map[SDL_SCANCODE_PAGEDOWN] = RCG_KEY_PGDN;
   rcg_key_map[SDL_SCANCODE_INSERT] = RCG_KEY_INS;
   rcg_key_map[SDL_SCANCODE_LSHIFT] = RCG_KEY_LSHIFT;
   rcg_key_map[SDL_SCANCODE_RSHIFT] = RCG_KEY_RSHIFT;
   rcg_key_map[SDL_SCANCODE_LCTRL] = RCG_KEY_LCTRL;
   rcg_key_map[SDL_SCANCODE_RCTRL] = RCG_KEY_RCTRL;
   rcg_key_map[SDL_SCANCODE_RALT] = RCG_KEY_RALT;
   rcg_key_map[SDL_SCANCODE_LALT] = RCG_KEY_LALT;
   rcg_key_map[SDL_SCANCODE_SPACE] = RCG_KEY_SPACE;

   rcg_key_map[SDL_SCANCODE_0] = RCG_KEY_0;
   rcg_key_map[SDL_SCANCODE_1] = RCG_KEY_1;
   rcg_key_map[SDL_SCANCODE_2] = RCG_KEY_2;
   rcg_key_map[SDL_SCANCODE_3] = RCG_KEY_3;
   rcg_key_map[SDL_SCANCODE_4] = RCG_KEY_4;
   rcg_key_map[SDL_SCANCODE_5] = RCG_KEY_5;
   rcg_key_map[SDL_SCANCODE_6] = RCG_KEY_6;
   rcg_key_map[SDL_SCANCODE_7] = RCG_KEY_7;
   rcg_key_map[SDL_SCANCODE_8] = RCG_KEY_8;
   rcg_key_map[SDL_SCANCODE_9] = RCG_KEY_9;
   
   rcg_key_map[SDL_SCANCODE_COMMA] = RCG_KEY_COMMA;
   rcg_key_map[SDL_SCANCODE_PERIOD] = RCG_KEY_PERIOD;

   rcg_key_map[SDL_SCANCODE_KP_0] = RCG_KEY_NP0;
   rcg_key_map[SDL_SCANCODE_KP_1] = RCG_KEY_NP1;
   rcg_key_map[SDL_SCANCODE_KP_2] = RCG_KEY_NP2;
   rcg_key_map[SDL_SCANCODE_KP_3] = RCG_KEY_NP3;
   rcg_key_map[SDL_SCANCODE_KP_4] = RCG_KEY_NP4;
   rcg_key_map[SDL_SCANCODE_KP_5] = RCG_KEY_NP5;
   rcg_key_map[SDL_SCANCODE_KP_6] = RCG_KEY_NP6;
   rcg_key_map[SDL_SCANCODE_KP_7] = RCG_KEY_NP7;
   rcg_key_map[SDL_SCANCODE_KP_8] = RCG_KEY_NP8;
   rcg_key_map[SDL_SCANCODE_KP_9] = RCG_KEY_NP9;
   rcg_key_map[SDL_SCANCODE_KP_MULTIPLY] = RCG_KEY_NP_MUL;
   rcg_key_map[SDL_SCANCODE_KP_PLUS] = RCG_KEY_NP_ADD;
   rcg_key_map[SDL_SCANCODE_KP_DIVIDE] = RCG_KEY_NP_DIV;
   rcg_key_map[SDL_SCANCODE_KP_MINUS] = RCG_KEY_NP_SUB;
   rcg_key_map[SDL_SCANCODE_KP_PERIOD] = RCG_KEY_NP_DECIMAL;
   rcg_key_map[SDL_SCANCODE_KP_ENTER] = RCG_KEY_NP_ENTER;

   rcg_mouse_map[SDL_BUTTON_LEFT] = RCG_BUTTON_LEFT;
   rcg_mouse_map[SDL_BUTTON_RIGHT] = RCG_BUTTON_RIGHT;
   rcg_mouse_map[SDL_BUTTON_MIDDLE] = RCG_BUTTON_MIDDLE;
   rcg_mouse_map[SDL_BUTTON_X1] = RCG_BUTTON_X1;
   rcg_mouse_map[SDL_BUTTON_X2] = RCG_BUTTON_X2;
}

void RCG_update(void)
{
   rcg_frametime = SDL_GetPerformanceCounter()-rcg_framestart;
   if(rcg_framedelay>rcg_frametime)
      SDL_Delay(((rcg_framedelay-rcg_frametime)*1000)/SDL_GetPerformanceFrequency());
   rcg_framestart = SDL_GetPerformanceCounter();

   rcg_mouse_wheel = 0;
   memcpy(rcg_old_key_state,rcg_new_key_state,sizeof(rcg_new_key_state));

   SDL_Event event;
   while(SDL_PollEvent(&event))
   {
      switch(event.type)
      {
      case SDL_QUIT:
         RCG_quit();
         break;
      case SDL_KEYDOWN:
         if(event.key.state==SDL_PRESSED)
            rcg_new_key_state[rcg_key_map[event.key.keysym.scancode]] = 1;
         break;
      case SDL_KEYUP:
         if(event.key.state==SDL_RELEASED)
            rcg_new_key_state[rcg_key_map[event.key.keysym.scancode]] = 0;
         break;
      case SDL_MOUSEBUTTONDOWN:
         if(event.button.state==SDL_PRESSED)
            rcg_new_key_state[rcg_mouse_map[event.button.button]] = 1;
         break;
      case SDL_MOUSEBUTTONUP:
         if(event.button.state==SDL_RELEASED)
            rcg_new_key_state[rcg_mouse_map[event.button.button]] = 0;
         break;       
      case SDL_MOUSEWHEEL:
         rcg_mouse_wheel = event.wheel.y;
         break;
      case SDL_WINDOWEVENT:
         rcg_update_viewport();
         break;
      }
   }
}

void RCG_render_present(void)
{
   //Clear the screen (we would have garbage outside of the framebuffer if we wouldn't)
   SDL_RenderClear(rcg_sdl_renderer);

   //SDL textures need to be locked to copy data to them
   void *data;
   int stride;
   SDL_LockTexture(rcg_sdl_texture,NULL,&data,&stride);

   const RCG_color * restrict pal = RCG_palette();
   const uint8_t * restrict src = RCG_framebuffer();
   RCG_color * restrict pix = data;
   for(int i = 0;i<RCG_XRES*RCG_YRES;i++)
      pix[i] = pal[src[i]];

   SDL_UnlockTexture(rcg_sdl_texture);

   //Here we actually render the texture to the screen, using the rcg_view_* values calculated by rcg_update_viewport() for positioning the texture
   SDL_Rect dst_rect;
   dst_rect.x = rcg_view_x;
   dst_rect.y = rcg_view_y;
   dst_rect.w = rcg_view_width;
   dst_rect.h = rcg_view_height;
   SDL_RenderCopy(rcg_sdl_renderer,rcg_sdl_texture,NULL,&dst_rect);

   //Actuall show what was rendered
   SDL_RenderPresent(rcg_sdl_renderer);
}

int RCG_running(void)
{
   return rcg_running;
}

void RCG_quit(void)
{
   rcg_running = 0;
}

void RCG_mouse_relative(int relative)
{
   SDL_SetRelativeMouseMode(relative);
}

void RCG_mouse_show(int show)
{
   SDL_ShowCursor(show?SDL_ENABLE:SDL_DISABLE);
}

int RCG_key_down(RCG_key key)
{
   return rcg_new_key_state[key];
}

int RCG_key_pressed(RCG_key key)
{
   return rcg_new_key_state[key]&&!rcg_old_key_state[key];
}

int RCG_key_released(RCG_key key)
{
   return !rcg_new_key_state[key]&&rcg_old_key_state[key];
}

int RCG_mouse_wheel_scroll(void)
{
   return rcg_mouse_wheel;
}

void RCG_mouse_pos(int *x, int *y)
{
   SDL_GetMouseState(x,y);

   *x-=rcg_view_x;
   *y-=rcg_view_y;
   *x = *x/rcg_pixel_scale;
   *y = *y/rcg_pixel_scale;

   if(*x>=RCG_XRES)
     *x = RCG_XRES-1;
   if(*y>=RCG_YRES)
     *y = RCG_YRES-1;

   if(*x<0)
     *x = 0;
   if(*y<0)
     *y = 0;
}

void RCG_mouse_relative_pos(int *x, int *y)
{
   SDL_GetRelativeMouseState(x,y);
}

uint8_t *RCG_framebuffer(void)
{
   return rcg_framebuffer;
}

/// Implementation
/// ---------------------------
///<C
void RCG_draw_clear(uint8_t color)
{
   memset(rcg_framebuffer,color,RCG_XRES*RCG_YRES);
}

void RCG_draw_line_vertical(int x, int y0, int y1, uint8_t color)
{
   if(y0>y1)
   {
      int t = y0;
      y0 = y1;
      y1 = t;
   }

   if(x<0||x>=RCG_XRES||y0>=RCG_YRES||y1<0)
      return;

   if(y0<0)
      y0 = 0;
   if(y1>=RCG_YRES)
      y1 = RCG_YRES-1;

   uint8_t * restrict dst = &RCG_framebuffer()[y0*RCG_XRES+x];
   for(int y = y0;y<=y1;y++)
   {
      *dst = color;
      dst+=RCG_XRES;
   }
}

void RCG_draw_line_horizontal(int x0, int y, int x1, uint8_t color)
{
   if(x0>x1)
   {
      int t = x0;
      x0 = x1;
      x1 = t;
   }
	   
   if(y<0||y>=RCG_YRES||x0>=RCG_XRES||x1<0)
      return;

   if(x0<0)
      x0 = 0;
   if(x1>=RCG_XRES)
      x1 = RCG_XRES-1;

   uint8_t * restrict dst = &RCG_framebuffer()[y*RCG_XRES+x0];
   for(int x = x0;x<=x1;x++)
      *(dst++) = index;
}

void RCG_draw_line(int x0, int y0, int x1, int y1, uint8_t color)
{
}

void RCG_draw_rectangle(int x, int y, int width, int height, uint8_t color)
{
   RCG_draw_line_horizontal(x,y,x+width,color);
   RCG_draw_line_horizontal(x,y+height,x+width,color);
   RCG_draw_line_vertical(x,y+1,y+height-1,color);
   RCG_draw_line_vertical(x+width,y+1,y+height-1,color);
}

void RCG_draw_rectangle_fill(int x, int y, int width, int height, uint8_t color)
{
   //Clip src rect
   int draw_start_y = 0;
   int draw_start_x = 0;
   int draw_end_x = width;
   int draw_end_y = height;

   if(x<0)
      draw_start_x = -x;
   if(y<0)
      draw_start_y = -y;
   if(x+draw_end_x>RCG_XRES)
      draw_end_x = width+(RCG_XRES-x-draw_end_x);
   if(y+draw_end_y>RCG_YRES)
      draw_end_y = height+(RCG_YRES-y-draw_end_y);
    
   //Clip dst rect
   x = x<0?0:x;
   y = y<0?0:y;

   uint8_t * restrict dst = &RCG_core_framebuffer()[y*RCG_XRES+x];
   int dst_step = RCG_XRES-(draw_end_x-draw_start_x);
    
   for(int y1 = draw_start_y;y1<draw_end_y;y1++,dst+=dst_step)
      for(int x1 = draw_start_x;x1<draw_end_x;x1++,dst++)
         *dst = index;
}
///>

static void rcg_update_viewport(void)
{
   int window_width = 1;
   int window_height = 1;

   SDL_GetWindowSize(rcg_sdl_window,&window_width,&window_height);

   float ratio = (float)window_width/(float)window_height;

   if(ratio>(float)RCG_XRES/(float)RCG_YRES)
   {
      rcg_view_height = window_height;
      rcg_view_width = ((float)RCG_XRES/(float)RCG_YRES)*(float)window_height;
   }
   else
   {
      rcg_view_width = window_width;
      rcg_view_height = ((float)RCG_YRES/(float)RCG_XRES)*(float)window_width;
   }

   rcg_view_x = (window_width-rcg_view_width)/2;
   rcg_view_y = (window_height-rcg_view_height)/2;

   rcg_pixel_scale = (float)rcg_view_width/(float)RCG_XRES;
}

#endif
#endif

#ifdef RCG_EXAMPLE

/// Example code
/// ---------------------------
///
/// This articles example code loads a palette, a slightly modified version of the one used by freedoom to be exact (you can find it in the Download section) and fills the framebuffer with ascending indices, repeating the palette many times. 
///
/// At this point you theoretically already have everything for making a basic game, the following chapters will lay the baseline for creating 3d graphics.
///<C
int main(int argc, char **argv)
{
   RCG_init("retro computer graphics - chapter 1.3");
   RCG_palette_load("freedoom.hex");

   while(RCG_running())
   {
      RCG_update();

      RCG_draw_clear(1);

      if(RCG_key_pressed(RCG_KEY_ESCAPE))
         RCG_quit();

      RCG_render_present();
   }

   return 0;
}

///>
/// ![This is what you should be seeing](/image/rcg_chapter1_3_0.png)
///
/// Download
/// ---------------------------
///
/// Download this articles source code here: [chapter1_3.c](https://raw.githubusercontent.com/Captain4LK/articles/master/articles/rcg/chapter1_3.c)
///
/// ---------------------------
/// Article Series:
///   * [retro computer graphics - Introduction](/rcg/)
///   * [retro computer graphics - Chapter 1.1 - Initial setup: graphics output and input](/rcg/chapter1_1)
///   * [retro computer graphics - Chapter 1.2 - Color palettes](/rcg/chapter1_2)
///   * retro computer graphics - Chapter 1.3 - Basic drawing
///   * [retro computer graphics - Part 5 - Image loading and drawing](part2.html)
///   * [retro computer graphics - Part 6 - Basic math routines](part2.html)
///   * [retro computer graphics - Part 7 - Colormaps: lighting and transparency](part3.html)
///

#endif
