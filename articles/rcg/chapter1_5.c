#ifndef _RCG_H_

#define _RCG_H_

//This file can be compiled with the following command: ``gcc articles/rcg/chapter1_3.c -DRCG_EXAMPLE -DRCG_IMPLEMENTATION -lSDL2 -Og -g``

///////////////////////
/// ---
/// title: "Retro Computer Graphics - Chapter 1.3 - Image loading and drawing"
/// ---
///
/// Introduction
/// ---------------------------
///
/// In this article we'll implement a few simple drawing routines.
///
/// The key to writing a fast software renderer is to both draw as few pixels as possible (minimize overdraw) and to do as little work as possible per pixel drawn. For this chapter specifically this means that clipping for the drawn shapes should happen outside of the actual drawing. How
///

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>

#define RCG_XRES 640
#define RCG_YRES 480

#define RCG_FPS 30

typedef enum
{
   RCG_KEY_NONE,

   //Keyboard keys
   RCG_KEY_A, RCG_KEY_B, RCG_KEY_C, RCG_KEY_D, RCG_KEY_E, RCG_KEY_F, RCG_KEY_G,
   RCG_KEY_H, RCG_KEY_I, RCG_KEY_J, RCG_KEY_K, RCG_KEY_L, RCG_KEY_M, RCG_KEY_N,
   RCG_KEY_O, RCG_KEY_P, RCG_KEY_Q, RCG_KEY_R, RCG_KEY_S, RCG_KEY_T, RCG_KEY_U,
   RCG_KEY_V, RCG_KEY_W, RCG_KEY_X, RCG_KEY_Y, RCG_KEY_Z,
   RCG_KEY_0, RCG_KEY_1, RCG_KEY_2, RCG_KEY_3, RCG_KEY_4, RCG_KEY_5,
   RCG_KEY_6, RCG_KEY_7, RCG_KEY_8, RCG_KEY_9, RCG_KEY_COMMA, RCG_KEY_PERIOD,
   RCG_KEY_F1, RCG_KEY_F2, RCG_KEY_F3, RCG_KEY_F4, RCG_KEY_F5, RCG_KEY_F6,
   RCG_KEY_F7, RCG_KEY_F8, RCG_KEY_F9, RCG_KEY_F10, RCG_KEY_F11, RCG_KEY_F12,
   RCG_KEY_UP, RCG_KEY_DOWN, RCG_KEY_LEFT, RCG_KEY_RIGHT,
   RCG_KEY_SPACE, RCG_KEY_TAB, RCG_KEY_LSHIFT, RCG_KEY_RSHIFT, RCG_KEY_LCTRL, RCG_KEY_RCTRL,
   RCG_KEY_LALT, RCG_KEY_RALT, RCG_KEY_INS,
   RCG_KEY_DEL, RCG_KEY_HOME, RCG_KEY_END, RCG_KEY_PGUP, RCG_KEY_PGDN,
   RCG_KEY_BACK, RCG_KEY_ESCAPE, RCG_KEY_RETURN, RCG_KEY_ENTER, RCG_KEY_PAUSE, RCG_KEY_SCROLL,
   RCG_KEY_NP0, RCG_KEY_NP1, RCG_KEY_NP2, RCG_KEY_NP3, RCG_KEY_NP4, RCG_KEY_NP5, RCG_KEY_NP6, RCG_KEY_NP7, RCG_KEY_NP8, RCG_KEY_NP9,
   RCG_KEY_NP_MUL, RCG_KEY_NP_DIV, RCG_KEY_NP_ADD, RCG_KEY_NP_SUB, RCG_KEY_NP_DECIMAL, RCG_KEY_NP_ENTER,

   //Mouse buttons
   //Functions for reading input won't differentiate between mouse and keyboard input.
   //This makes remapping keys easier, which won't be discussed in this series, though.
   RCG_BUTTON_LEFT, RCG_BUTTON_RIGHT, RCG_BUTTON_MIDDLE, RCG_BUTTON_X1, RCG_BUTTON_X2,

   //Last element, will be used as size for mapping array
   RCG_KEY_MAX,
}RCG_key;

typedef struct
{
   uint8_t r, g, b, a;
}RCG_color;

typedef struct
{
   int32_t width;
   int32_t height;
   uint8_t data[];
}RCG_texture;

//Creates a window, the framebuffer and initializes keycode LUTs
void RCG_init(const char *title);

//Runs the sdl2 event loop, updates key states
void RCG_update(void);

//Returns, whether the program should keep running
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

//Writes how much the mouse was moved since the last frame into x and y
void RCG_mouse_relative_pos(int *x, int *y);

uint8_t *RCG_framebuffer(void);

//Loads a palette from a .hex file
void RCG_palette_load(const char *path);

//Returns a pointer to the palette
RCG_color *RCG_palette(void);

//Clears the screen to the given color
void RCG_draw_clear(uint8_t color);

//Draws a vertical line
void RCG_draw_line_vertical(int32_t x, int32_t y0, int32_t y1, uint8_t color);

//Draws a horizontal line
void RCG_draw_line_horizontal(int32_t x0, int32_t y, int32_t x1, uint8_t color);

//Draws a line between two points
void RCG_draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t color);

//Draws the outline of a rectangle
void RCG_draw_rectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint8_t color);

//Draws a filled rectangle
void RCG_draw_rectangle_fill(int32_t x, int32_t y, int32_t width, int32_t height, uint8_t color);

void RCG_draw_line_fp(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t color);

RCG_texture *RCG_texture_load(const char *path);
void RCG_draw_texture(const RCG_texture *tex, int32_t x, int32_t y);

//New
uint8_t *RCG_shade_table(uint8_t light);
uint8_t RCG_blend(uint8_t c0, uint8_t c1);
void RCG_draw_texture_ex(const RCG_texture *tex, int32_t x, int32_t y, uint8_t shade, uint8_t trans);
//

#endif

#ifdef RCG_IMPLEMENTATION
#ifndef RCG_IMPLEMENTATION_ONCE

static void rcg_update_viewport(void);

//SDL2
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

static int rcg_running = 1;

static uint8_t *rcg_framebuffer = NULL;

//New
static uint8_t rcg_shade_table[32][256];
static uint8_t rcg_trans_table[256][256];
//


//No need to store the actual color count, since it's limited to 256 colors
//due to pixels being stored in an 8 bit integer
static RCG_color rcg_palette[256];

//New
static void rcg_calculate_colormaps(void);
static uint8_t rcg_pal_find_closest(uint8_t r, uint8_t g, uint8_t b);
//

void RCG_palette_load(const char *path)
{
   FILE *f = fopen(path, "r");
   if(NULL==f)
      return;

   //Read the input file line by line and parse the input using sscanf()
   //Alternatively you could parse the string manually,
   //which due to the simplicity of the format would be very easy to implement.
   char buffer[512];
   int color = 0;
   while(fgets(buffer, 512, f)&&color<256)
   {
      unsigned r = 0;
      unsigned g = 0;
      unsigned b = 0;
      sscanf(buffer, "%2x%2x%2x", &r, &g, &b);
      rcg_palette[color].r = (uint8_t)r;
      rcg_palette[color].g = (uint8_t)g;
      rcg_palette[color].b = (uint8_t)b;
      rcg_palette[color].a = 255;
      color++;
   }

   fclose(f);

   rcg_calculate_colormaps();
}

RCG_color *RCG_palette(void)
{
   return rcg_palette;
}

void RCG_init(const char *title)
{
   Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
   SDL_Init(flags);

   //Calculate max possible window scale
   //This code is not necessarily needed, sine the window will be resizable
   //It's only here to make the window not be tiny on high-res displays
   int scale = 1;
   int max_x, max_y;
   SDL_Rect max_size;
   SDL_GetDisplayUsableBounds(0, &max_size);
   max_x = max_size.w / RCG_XRES;
   max_y = max_size.h / RCG_YRES;
   scale = (max_x>max_y)?max_y:max_x;

   //Create window, renderer and texture for framebuffer
   rcg_sdl_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RCG_XRES * scale, RCG_YRES * scale, SDL_WINDOW_RESIZABLE);
   rcg_sdl_renderer = SDL_CreateRenderer(rcg_sdl_window, -1, SDL_RENDERER_ACCELERATED);
   SDL_SetRenderDrawColor(rcg_sdl_renderer, 0, 0, 0, 0);
   rcg_sdl_texture = SDL_CreateTexture(rcg_sdl_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, RCG_XRES, RCG_YRES);
   SDL_SetTextureBlendMode(rcg_sdl_texture, SDL_BLENDMODE_NONE);

   //Implementation and purpose will be discussed later in this article.
   rcg_update_viewport();

   rcg_framedelay = SDL_GetPerformanceFrequency() / RCG_FPS;

   rcg_framebuffer = malloc(RCG_XRES * RCG_YRES);
   memset(rcg_framebuffer, 0, RCG_XRES * RCG_YRES);

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
   rcg_frametime = SDL_GetPerformanceCounter() - rcg_framestart;
   if(rcg_framedelay>rcg_frametime)
      SDL_Delay((uint32_t)(((rcg_framedelay - rcg_frametime) * 1000) / SDL_GetPerformanceFrequency()));
   rcg_framestart = SDL_GetPerformanceCounter();

   rcg_mouse_wheel = 0;
   memcpy(rcg_old_key_state, rcg_new_key_state, sizeof(rcg_new_key_state));

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
   SDL_LockTexture(rcg_sdl_texture, NULL, &data, &stride);

   const RCG_color * restrict pal = RCG_palette();
   const uint8_t * restrict src = RCG_framebuffer();
   RCG_color * restrict pix = data;
   for(int i = 0; i<RCG_XRES * RCG_YRES; i++)
      pix[i] = pal[src[i]];
   SDL_UnlockTexture(rcg_sdl_texture);

   //Here we actually render the texture to the screen, using the rcg_view_* values calculated by rcg_update_viewport() for positioning the texture
   SDL_Rect dst_rect;
   dst_rect.x = rcg_view_x;
   dst_rect.y = rcg_view_y;
   dst_rect.w = rcg_view_width;
   dst_rect.h = rcg_view_height;
   SDL_RenderCopy(rcg_sdl_renderer, rcg_sdl_texture, NULL, &dst_rect);

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
   SDL_GetMouseState(x, y);

   *x -= rcg_view_x;
   *y -= rcg_view_y;
   *x = (int)((float)*x / rcg_pixel_scale);
   *y = (int)((float)*y / rcg_pixel_scale);

   if(*x>=RCG_XRES)
      *x = RCG_XRES - 1;
   if(*y>=RCG_YRES)
      *y = RCG_YRES - 1;

   if(*x<0)
      *x = 0;
   if(*y<0)
      *y = 0;
}

void RCG_mouse_relative_pos(int *x, int *y)
{
   SDL_GetRelativeMouseState(x, y);
}

uint8_t *RCG_framebuffer(void)
{
   return rcg_framebuffer;
}

/// Implementation
/// ---------------------------
///
/// RCG_draw_clear() can be implemented with a single call to memset. We won't use this function very often later, since we usually redraw the entire screen anyway, clearing it would only hurt performance.
///<C
void RCG_draw_clear(uint8_t color)
{
   memset(rcg_framebuffer, color, RCG_XRES * RCG_YRES);
}
///>

/// RCG_draw_line_vertical() and RCG_draw_line_horizontal().
///
/// You might wonder why these two functions exist, when there is also going to be a general purpose line drawing function RCG_draw_line(). Generally, specialized drawing functions are usually faster than general purpose ones. If you know an often drawn shape is within certain constraints, it's faster to write a specialized function for drawing it.
///
/// For example, we'll use these functions in the early sub chapters of the raycasting renderer for drawing solid colored walls.
///
///<C
void RCG_draw_line_vertical(int32_t x, int32_t y0, int32_t y1, uint8_t color)
{
   //Swap the two y coordinates if necessary, placing the upper (smaller) one in y0.
   //Doing this makes the later clipping simpler, since we can make assumptions about the values of y0 and y1.
   if(y0>y1)
   {
      int32_t t = y0;
      y0 = y1;
      y1 = t;
   }

   //Early out the line if it's completely outside the screen
   if(x<0||x>=RCG_XRES||y0>=RCG_YRES||y1<0)
      return;

   //Clip the line to the screen boundaries
   if(y0<0)
      y0 = 0;
   if(y1>=RCG_YRES)
      y1 = RCG_YRES - 1;

   //Now, we draw the line
   // We access the framebuffer using a pointer, which we increment after every pixel written.
   // In my experience, the compiler optimizes code better if you access arrays by incrementing a pointer.
   uint8_t * restrict dst = &RCG_framebuffer()[y0 * RCG_XRES + x];
   for(int y = y0; y<=y1; y++)
   {
      *dst = color;
      dst += RCG_XRES;
   }
}
///>

/// RCG_draw_line_horizontal() is basically the same, with x and y coordinates being swapped.
///<C
void RCG_draw_line_horizontal(int32_t x0, int32_t y, int32_t x1, uint8_t color)
{
   if(x0>x1)
   {
      int32_t t = x0;
      x0 = x1;
      x1 = t;
   }

   if(y<0||y>=RCG_YRES||x0>=RCG_XRES||x1<0)
      return;

   if(x0<0)
      x0 = 0;
   if(x1>=RCG_XRES)
      x1 = RCG_XRES - 1;

   uint8_t * restrict dst = &RCG_framebuffer()[y * RCG_XRES + x0];
   for(int x = x0; x<=x1; x++)
      *(dst++) = color;
}
///>

/// RCG_draw_rectangle() can be implemented using our new vertical and horizontal line drawing functions.
///<C
void RCG_draw_rectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint8_t color)
{
   //The offsets in the coordinates are to make sure the individual lines don't overlap
   RCG_draw_line_horizontal(x, y, x + width, color);
   RCG_draw_line_horizontal(x, y + height, x + width, color);
   RCG_draw_line_vertical(x, y + 1, y + height - 1, color);
   RCG_draw_line_vertical(x + width, y + 1, y + height - 1, color);
}
///>

/// RCG_draw_rectangle_fill() is a little more complicated since it requires clipping in two dimensions instead of one like for the lines.
///
/// The clipping is admittedly more complicated than it needs to be, we are however going to reuse this clipping code when drawing sprites (hence the draw_start and draw_end instead of just a clipped width/height).
///<C
void RCG_draw_rectangle_fill(int32_t x, int32_t y, int32_t width, int32_t height, uint8_t color)
{
   //these variables are positions inside the drawn rectangle
   //i.e. width = draw_end_x-draw_start_x
   int32_t draw_start_y = 0;
   int32_t draw_start_x = 0;
   int32_t draw_end_x = width;
   int32_t draw_end_y = height;

   //Here we clip the source rectangle, changing
   //its width and height to fit the screen

   //if the starting coordinates are n below zero
   //we start drawing the rectangle n pixels later
   if(x<0)
      draw_start_x = -x;
   if(y<0)
      draw_start_y = -y;

   //if the end coordinates are m above the width/height
   //we stop drawing the rectangle m pixels earlier
   if(x + draw_end_x>RCG_XRES)
      draw_end_x = width + (RCG_XRES - x - draw_end_x);
   if(y + draw_end_y>RCG_YRES)
      draw_end_y = height + (RCG_YRES - y - draw_end_y);

   //Now we clamp the drawn position of the rectangle
   //Only clamping to 0 is necessary, the rest is taken care
   //of by the previous clipping
   x = x<0?0:x;
   y = y<0?0:y;

   //Drawing is essentially the same as for the lines, just in two dimensions
   //instead. Additionally we need to calculate
   //the amount to increment the pointer every line by (dst_tsep)
   uint8_t * restrict dst = &RCG_framebuffer()[y * RCG_XRES + x];
   int dst_step = RCG_XRES - (draw_end_x - draw_start_x);
   for(int y1 = draw_start_y; y1<draw_end_y; y1++, dst += dst_step)
      for(int x1 = draw_start_x; x1<draw_end_x; x1++, dst++)
         *dst = color;
}
///>
///
/// Alternatively you could implement RCG_draw_rectangle_fill() by drawing horizontal lines for every y pixel of the rectangle.
///

void RCG_draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t color)
{
   RCG_draw_line_fp(x0 * 256 + 128, y0 * 256 + 128, x1 * 256 + 128, y1 * 256 + 128, color);
}

static uint8_t rcg_clip_outcode(int32_t l, int32_t u, int32_t r, int32_t d, int32_t x, int32_t y)
{
   uint8_t code = 0;

   if(x<l)
      code |= 1;
   else if(x>r)
      code |= 2;

   if(y<u)
      code |= 4;
   if(y>d)
      code |= 8;

   return code;
}

static int rcg_clip_line(int32_t l, int32_t u, int32_t r, int32_t d, int32_t *x0, int32_t *y0, int32_t *x1, int32_t *y1)
{
   uint8_t code0 = rcg_clip_outcode(l, u, r, d, *x0, *y0);
   uint8_t code1 = rcg_clip_outcode(l, u, r, d, *x1, *y1);

   for(;;)
   {
      if(code0==0&&code1==0)
         return 1;

      if(code0 & code1)
         return 0;

      uint8_t code_out = code1>code0?code1:code0;
      int32_t x, y;
      int32_t dx = *x1 - *x0;
      int32_t dy = *y1 - *y0;

      if(code_out & 8)
      {
         x = (int32_t)(*x0 + ((int64_t)dx * (d - *y0)) / dy);
         //x = *x0 + RvR_fix24_div(RvR_fix24_mul(dx, d - *y0), RvR_non_zero(dy));
         y = d;
      }
      else if(code_out & 4)
      {
         x = (int32_t)(*x0 + ((int64_t)dx * (u - *y0)) / dy);
         //x = *x0 + RvR_fix24_div(RvR_fix24_mul(dx, u - *y0), RvR_non_zero(dy));
         y = u;
      }
      else if(code_out & 2)
      {
         x = r;
         y = (int32_t)(*y0 + ((int64_t)dy * (r - *x0)) / dx);
         //y = *y0 + RvR_fix24_div(RvR_fix24_mul(dy, r - *x0), RvR_non_zero(dx));
      }
      else
      {
         x = l;
         y = (int32_t)(*y0 + ((int64_t)dy * (l - *x0)) / dx);
         //y = *y0 + RvR_fix24_div(RvR_fix24_mul(dy, l - *x0), RvR_non_zero(dx));
      }

      if(code_out==code0)
      {
         *x0 = x;
         *y0 = y;
         code0 = rcg_clip_outcode(l, u, r, d, *x0, *y0);
      }
      else
      {
         *x1 = x;
         *y1 = y;
         code1 = rcg_clip_outcode(l, u, r, d, *x1, *y1);
      }
   }

   return 0;
}

void RCG_draw_line_fp(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t color)
{
   if(!rcg_clip_line(0, 0, RCG_XRES * 256 - 1, RCG_YRES * 256 - 1, &x0, &y0, &x1, &y1))
      return;

   int32_t dx = x1 - x0;
   int32_t dy = y1 - y0;
   int32_t x = x0 >> 8;
   int32_t y = y0 >> 8;
   int32_t db;
   int32_t ds;
   int32_t fracb;
   int32_t fracs;
   int32_t steps;
   int32_t stepb;
   int32_t togox = (x1 >> 8) - (x0 >> 8);
   int32_t togoy = (y1 >> 8) - (y0 >> 8);
   int32_t togob;
   int32_t togos;

   if(dx>=0)
   {
      if(dy>=0)
      {
         db = dx;
         ds = dy;
         fracb = x0 & 255;
         fracs = y0 & 255;
         togob = togox;
         togos = togoy;
         stepb = 1;
         steps = RCG_XRES;
      }
      else
      {
         db = dx;
         ds = -dy;
         fracb = x0 & 255;
         fracs = 255 - (y0 & 255);
         togob = togox;
         togos = -togoy;
         stepb = 1;
         steps = -RCG_XRES;
      }
   }
   else
   {
      if(dy>=0)
      {
         db = -dx;
         ds = dy;
         fracb = 255 - (x0 & 255);
         fracs = y0 & 255;
         togob = -togox;
         togos = togoy;
         stepb = -1;
         steps = RCG_XRES;
      }
      else
      {
         db = -dx;
         ds = -dy;
         fracb = 255 - (x0 & 255);
         fracs = 255 - (y0 & 255);
         togob = -togox;
         togos = -togoy;
         stepb = -1;
         steps = -RCG_XRES;
      }
   }

   if(db<ds)
   {
      int32_t tmp;
      tmp = db; db = ds; ds = tmp;
      tmp = fracb; fracb = fracs; fracs = tmp;
      tmp = togob; togob = togos; togos = tmp;
      tmp = stepb; stepb = steps; steps = tmp;
   }

   uint8_t * restrict dst = &RCG_framebuffer()[y * RCG_XRES + x];
   int32_t dist = (int32_t)((int64_t)(fracs - 128) * db - (int64_t)(fracb - 128) * ds) / 256;
   int32_t togo = togob;
   while(togo>0)
   {
      if(dist>db / 2)
      {
         dst += steps;
         dist -= db;
      }

      *dst = color;
      dst += stepb;
      dist += ds;
      togo--;
   }
}


//As exercise?
//My code is bad lol
RCG_texture *RCG_texture_load(const char *path)
{
   //This is NOT safe code
   FILE *f = fopen(path,"rb");
   if(f==NULL)
      return NULL;

   uint8_t header = 0;
   uint16_t ignore16 = 0;
   uint8_t ignore8 = 0;
   uint8_t encoding = 0;
   uint8_t plane_bits = 0;
   uint16_t min_x = 0;
   uint16_t min_y = 0;
   uint16_t max_x = 0;
   uint16_t max_y = 0;
   uint8_t num_planes = 0;

   int32_t width;
   int32_t height;

   fread(&header,1,1,f);
   fread(&ignore8,1,1,f);
   fread(&encoding,1,1,f);
   fread(&plane_bits,1,1,f);
   fread(&min_x,2,1,f);
   fread(&min_y,2,1,f);
   fread(&max_x,2,1,f);
   fread(&max_y,2,1,f);
   fread(&ignore16,2,1,f);
   fread(&ignore16,2,1,f);
   for(int i = 0;i<48;i++)
      fread(&ignore8,1,1,f);
   fread(&ignore8,1,1,f);
   fread(&num_planes,1,1,f);
   fread(&ignore16,2,1,f);
   fread(&ignore16,2,1,f);
   fread(&ignore16,2,1,f);
   fread(&ignore16,2,1,f);
   for(int i = 0;i<54;i++)
      fread(&ignore8,1,1,f);

   width = max_x-min_x+1;
   height = max_y-min_y+1;
   assert(header==0x0a);
   assert(encoding==0x1);
   assert(plane_bits==0x8);
   assert(num_planes==0x1);
   assert(width>0);
   assert(height>0);

   RCG_texture *tex = malloc(sizeof(*tex)+sizeof(*tex->data)*width*height);
   tex->width = width;
   tex->height = height;

   int32_t cur = 0;
   int32_t run_len = 0;
   while(cur<width*height)
   {
      uint8_t byte = 0;
      fread(&byte,1,1,f);
      if(run_len==0&&(byte&0xc0)==0xc0)
      {
         run_len = byte&0x3f;
      }
      else
      {
         if(run_len==0)
            run_len = 1;

         for(int i = 0;i<run_len;i++)
            tex->data[cur++] = byte;

         run_len = 0;
      }
   }

   fclose(f);

   return tex;
}

void RCG_draw_texture(const RCG_texture *tex, int32_t x, int32_t y)
{
   //Clip source
   int32_t draw_start_y = 0;
   int32_t draw_start_x = 0;
   int32_t draw_end_x = tex->width;
   int32_t draw_end_y = tex->height;
   if(x<0)
      draw_start_x = -x;
   if(y<0)
      draw_start_y = -y;
   if(x + draw_end_x>RCG_XRES)
      draw_end_x = tex->width + (RCG_XRES - x - draw_end_x);
   if(y + draw_end_y>RCG_YRES)
      draw_end_y = tex->height + (RCG_YRES - y - draw_end_y);

   //Clip dst
   x = x<0?0:x;
   y = y<0?0:y;

   const uint8_t *src = &tex->data[draw_start_x + draw_start_y * tex->width];
   uint8_t *dst = &RCG_framebuffer()[x + y * RCG_XRES];
   int32_t src_step = -(draw_end_x - draw_start_x) + tex->width;
   int32_t dst_step = RCG_XRES - (draw_end_x - draw_start_x);

   for(int y1 = draw_start_y; y1<draw_end_y; y1++, dst += dst_step, src += src_step)
      for(int x1 = draw_start_x; x1<draw_end_x; x1++, src++, dst++)
         *dst = *src?*src:*dst;
}

//New

uint8_t *RCG_shade_table(uint8_t light)
{
   return rcg_shade_table[light];
}

uint8_t RCG_blend(uint8_t c0, uint8_t c1)
{
   return rcg_trans_table[c0][c1];
}

static void rcg_calculate_colormaps(void)
{
   //shade table
   for(int c = 0; c<256;c++)
   {
      for(int s = 0;s<32;s++)
      {
         //Don't fade transparency index
         if(c==0)
         {
            rcg_shade_table[s][c] = 0;
            continue;
         }

         uint8_t r = (uint8_t)((rcg_palette[c].r*(31-s))/31);
         uint8_t g = (uint8_t)((rcg_palette[c].g*(31-s))/31);
         uint8_t b = (uint8_t)((rcg_palette[c].b*(31-s))/31);

         rcg_shade_table[s][c] = rcg_pal_find_closest(r,g,b);

         rcg_shade_table[s][c] = rcg_pal_find_closest(r,g,b);
      }
   }

   //trans table
   for(int i = 0;i<256;i++)
   {
      for(int j = 0;j<256;j++)
      {
         //0 is always fully transparent
         if(i==0)
         {
            rcg_trans_table[i][j] = (uint8_t)j;
            continue;
         }
         if(j==0)
         {
            rcg_trans_table[i][j] = (uint8_t)i;
            continue;
         }

         int32_t t = 2048/3;
         uint8_t r = (uint8_t)((t*rcg_palette[i].r+(1024-t)*rcg_palette[j].r)/1024);
         uint8_t g = (uint8_t)((t*rcg_palette[i].g+(1024-t)*rcg_palette[j].g)/1024);
         uint8_t b = (uint8_t)((t*rcg_palette[i].b+(1024-t)*rcg_palette[j].b)/1024);

         rcg_trans_table[i][j] = rcg_pal_find_closest(r,g,b);
      }
   }
}

static uint8_t rcg_pal_find_closest(uint8_t r, uint8_t g, uint8_t b)
{
   int32_t best_dist = INT32_MAX;
   uint8_t best_index = 0;

   for(int i = 0; i<256; i++)
   {
      int32_t dist = 0;
      dist += (r - rcg_palette[i].r) * (r - rcg_palette[i].r);
      dist += (g - rcg_palette[i].g) * (g - rcg_palette[i].g);
      dist += (b - rcg_palette[i].b) * (b - rcg_palette[i].b);

      if(dist<best_dist)
      {
         best_index = (uint8_t)i;
         best_dist = dist;
      }
   }

   return best_index;
}

void RCG_draw_texture_ex(const RCG_texture *tex, int32_t x, int32_t y, uint8_t shade, uint8_t trans)
{
   //Clip source
   int32_t draw_start_y = 0;
   int32_t draw_start_x = 0;
   int32_t draw_end_x = tex->width;
   int32_t draw_end_y = tex->height;
   if(x<0)
      draw_start_x = -x;
   if(y<0)
      draw_start_y = -y;
   if(x + draw_end_x>RCG_XRES)
      draw_end_x = tex->width + (RCG_XRES - x - draw_end_x);
   if(y + draw_end_y>RCG_YRES)
      draw_end_y = tex->height + (RCG_YRES - y - draw_end_y);

   //Clip dst
   x = x<0?0:x;
   y = y<0?0:y;

   const uint8_t *src = &tex->data[draw_start_x + draw_start_y * tex->width];
   uint8_t *dst = &RCG_framebuffer()[x + y * RCG_XRES];
   int32_t src_step = -(draw_end_x - draw_start_x) + tex->width;
   int32_t dst_step = RCG_XRES - (draw_end_x - draw_start_x);
   const uint8_t *col = RCG_shade_table(shade);

   if(trans==0)
   {
      for(int y1 = draw_start_y; y1<draw_end_y; y1++, dst += dst_step, src += src_step)
         for(int x1 = draw_start_x; x1<draw_end_x; x1++, src++, dst++)
            *dst = *src?col[*src]:*dst;
   }
   else if(trans==1)
   {
      for(int y1 = draw_start_y; y1<draw_end_y; y1++, dst += dst_step, src += src_step)
         for(int x1 = draw_start_x; x1<draw_end_x; x1++, src++, dst++)
            *dst = RCG_blend(col[*src],*dst);
   }
   else
   {
      for(int y1 = draw_start_y; y1<draw_end_y; y1++, dst += dst_step, src += src_step)
         for(int x1 = draw_start_x; x1<draw_end_x; x1++, src++, dst++)
            *dst = RCG_blend(*dst,col[*src]);
   }
}
//

static void rcg_update_viewport(void)
{
   int window_width = 1;
   int window_height = 1;

   SDL_GetWindowSize(rcg_sdl_window, &window_width, &window_height);

   float ratio = (float)window_width / (float)window_height;

   if(ratio>(float)RCG_XRES / (float)RCG_YRES)
   {
      rcg_view_height = window_height;
      rcg_view_width = (int)(((float)RCG_XRES / (float)RCG_YRES) * (float)window_height);
   }
   else
   {
      rcg_view_width = window_width;
      rcg_view_height = (int)(((float)RCG_YRES / (float)RCG_XRES) * (float)window_width);
   }

   rcg_view_x = (window_width - rcg_view_width) / 2;
   rcg_view_y = (window_height - rcg_view_height) / 2;

   rcg_pixel_scale = (float)rcg_view_width / (float)RCG_XRES;
}

/// Exercise
/// ---------------------------
///
/// You might have noticed that I didn't cover RCG_draw_line() in the last section. That's because it will be your exercise for this article. Don't worry, the source code of this article has an implementation, too, if you are too lazy to do it yourself.
///
/// Notes:
/// * Wikipedia articles:
///   * [Line drawing algorithm](https://en.wikipedia.org/wiki/Line_drawing_algorithm)
///   * [Digital differential analyzer](https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm))
///   * [Bresenham's line algorithm](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm), the one I implemented here
///   * [Xiaolin Wu's line algorithm](https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm)
/// * Bonus challenges (both implemented in this chapters source code):
///   * clip the line before rendering, getting rid of per pixel bounds checks
///   * make a version that takes fixed point numbers as parameters and render the line with sub-pixel accuracy
///

#endif
#endif

#ifdef RCG_EXAMPLE

/// Example code
/// ---------------------------
///
/// This articles example code demonstrates the various drawing functions discussed in this chapter.
///
///<C
int main(int argc, char **argv)
{
   RCG_init("retro computer graphics - chapter 1.3");
   RCG_palette_load("freedoom.hex");

   RCG_texture *tex =RCG_texture_load("monke.pcx");

   while(RCG_running())
   {
      RCG_update();

      RCG_draw_clear(1);

      for(int y = 0;y<32;y++)
      {
         for(int x = 0;x<256;x++)
            RCG_framebuffer()[(y+8)*RCG_XRES+x+8] = RCG_shade_table((uint8_t)y)[x];
      }

      for(int y = 0;y<256;y++)
      {
         for(int x = 0;x<256;x++)
            RCG_framebuffer()[(y+48)*RCG_XRES+x+8] = RCG_blend((uint8_t)x,(uint8_t)y);
      }

      RCG_draw_texture(tex,280,8);
      RCG_draw_texture_ex(tex,320,128,15,1);

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
/// Download this articles source code here: [chapter1_4.c](https://raw.githubusercontent.com/Captain4LK/articles/master/articles/rcg/chapter1_4.c)
///
/// ---------------------------
/// Article Series:
///   * [retro computer graphics - Introduction](/rcg/)
///   * [retro computer graphics - Chapter 1.1 - Initial setup: graphics output and input](/rcg/chapter1_1)
///   * [retro computer graphics - Chapter 1.2 - Color palettes](/rcg/chapter1_2)
///   * retro computer graphics - Chapter 1.3 - Basic drawing
///   * [retro computer graphics - Chapter 1.4 - Basic math routines](/rcg/chapter1_4)
///

#endif
