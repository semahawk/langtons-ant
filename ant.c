/*
 *
 * ant.c
 *
 * Created at:  Mon  6 Oct 15:29:33 2014 15:29:33
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "SDL/SDL.h"

#define WINDOW_WIDTH  1366
#define WINDOW_HEIGHT 768
/* in pixels */
#define SEGMENT_SIZE 4
/* number of segments on the horizontal axis */
#define HORIZONTAL_SEGNUM (WINDOW_WIDTH / SEGMENT_SIZE)
/* number of segments on the vertical axis */
#define VERTICAL_SEGNUM (WINDOW_HEIGHT / SEGMENT_SIZE)

#define   SET_COLOR ((Uint32)0xfcfcfc)
#define UNSET_COLOR ((Uint32)0x000000)

enum direction {
  NORTH, SOUTH,
  WEST,  EAST
};

static SDL_Surface *sf;
static SDL_Rect segments[HORIZONTAL_SEGNUM][VERTICAL_SEGNUM];

static Uint32 get_pixel(int x, int y)
{
  Uint32 *pixels = (Uint32 *)sf->pixels;

  return pixels[(y * sf->w) + x];
}

static bool is_segment_set(unsigned x, unsigned y)
{
  if (get_pixel(segments[x][y].x, segments[x][y].y) == SET_COLOR)
    return true;
  else
    return false;
}

static void move_the_ant(int *x, int *y, enum direction *direction)
{
  bool going_left;

  if (is_segment_set(*x, *y))
    going_left = false;
  else
    going_left = true;

  /* hit a set segment, turning 90 deg. left */
  if (going_left){
    switch (*direction){
      case NORTH:
        *direction = WEST;
        *x = *x - 1;
        break;
      case WEST:
        *direction = SOUTH;
        *y = *y - 1;
        break;
      case SOUTH:
        *direction = EAST;
        *x = *x + 1;
        break;
      case EAST:
        *direction = NORTH;
        *y = *y + 1;
        break;
    }
  }
  /* hit an unset segment, turning 90 deg. right */
  else {
    switch (*direction){
      case NORTH:
        *direction = EAST;
        *x = *x + 1;
        break;
      case EAST:
        *direction = SOUTH;
        *y = *y - 1;
        break;
      case SOUTH:
        *direction = WEST;
        *x = *x - 1;
        break;
      case WEST:
        *direction = NORTH;
        *y = *y + 1;
        break;
        break;
    }
  }

  if (*x < 0)
    *x = HORIZONTAL_SEGNUM - 1;
  else if (*x >= HORIZONTAL_SEGNUM)
    *x = 0;

  if (*y < 0)
    *y = VERTICAL_SEGNUM - 1;
  else if (*y >= VERTICAL_SEGNUM)
    *y = 0;
}

static void toggle_segment(int x, int y)
{
  /*assert((x >= 0 && x < HORIZONTAL_SEGNUM) &&*/
         /*(y >= 0 && y <   VERTICAL_SEGNUM));*/

  Uint32 color;

  if (is_segment_set(x, y))
    color = UNSET_COLOR;
  else
    color = SET_COLOR;

  SDL_FillRect(sf, &segments[x][y], color);
  SDL_Flip(sf);
}

int main(void)
{
  srand(time(NULL));
  bool running = true;
  bool paused  = false;

  SDL_Init(SDL_INIT_EVERYTHING);

  sf = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_SWSURFACE);

  {
    int i, x;
    int j, y;

    for (i = 0, x = 0; i < HORIZONTAL_SEGNUM; x += SEGMENT_SIZE, i++){
      for (j = 0, y = 0; j < VERTICAL_SEGNUM; y += SEGMENT_SIZE, j++){
        segments[i][j].x = x;
        segments[i][j].y = y;
        segments[i][j].w = SEGMENT_SIZE;
        segments[i][j].h = SEGMENT_SIZE;
      }
    }
  }

  SDL_Event event;

  /* ant's initial position (in segments) */
  int x = HORIZONTAL_SEGNUM / 2;
  int y = VERTICAL_SEGNUM / 2;

  /* ant's initial direction */
  enum direction dir = NORTH;

  while (running){
    if (SDL_PollEvent(&event)){
      switch (event.type){
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_SPACE)
            /* toggle */
            paused = paused ? false : true;
          else if (event.key.keysym.sym == SDLK_ESCAPE)
            running = false;

          break;

        default:
          break;
      }
    }

    if (paused) continue;

    toggle_segment(x, y);
    move_the_ant(&x, &y, &dir);
  }

  SDL_Quit();

  return 0;
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

