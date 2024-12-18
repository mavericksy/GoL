#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600
#define CELL_WIDTH 3
#define COLUMNS SCREEN_WIDTH / CELL_WIDTH
#define ROWS SCREEN_HEIGHT / CELL_WIDTH
#define RATIO 3

#define COLOUR_WHITE 0xffffffff
#define COLOUR_BLACK 0x00000000
#define COLOUR_LIGHT 0xC7F0D8
#define COLOUR_DARK 0x43523D

int game_matrix[ROWS][COLUMNS] = {0};
int next_matrix[ROWS][COLUMNS] = {0};

int draw_grid(SDL_Surface *surface, int columns, int rows) {
  //
  for (int i = 0; i < rows; i++) {
    SDL_Rect line = (SDL_Rect){0, i * CELL_WIDTH, SCREEN_WIDTH, 1};
    SDL_FillRect(surface, &line, COLOUR_DARK);
  }
  //
  for (int i = 0; i < columns; i++) {
    SDL_Rect line = (SDL_Rect){i * CELL_WIDTH, 0, 1, SCREEN_HEIGHT};
    SDL_FillRect(surface, &line, COLOUR_DARK);
  }
  //
  return 0;
}

void draw_cell(SDL_Surface *surface, int cell_x, int cell_y) {
  //
  SDL_Rect cell = (SDL_Rect){cell_x * CELL_WIDTH, cell_y * CELL_WIDTH,
                             CELL_WIDTH, CELL_WIDTH};
  SDL_FillRect(surface, &cell, COLOUR_DARK);
}

void random_position() {
  //
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLUMNS; col++) {
      if ((rand() % RATIO) == 0)
        game_matrix[row][col] = 1;
    }
  }
}

void display_position(SDL_Surface* surface) {
   for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLUMNS; col++) {
        if (game_matrix[row][col] == 1)
            draw_cell(surface, col, row);
    }
  }
}

void calculate_next_position() {
  //
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLUMNS; col++) {
        //
        int cell_count = 0;
        //
        if ((row > 0) && (col > 0))
            if (game_matrix[row - 1][col - 1] == 1)
              cell_count++;
        //
        if ((row > 0) && (game_matrix[row - 1][col] == 1))
            cell_count++;
        //
        if ((row > 0) && (col < (COLUMNS - 1)))
            if (game_matrix[row - 1][col + 1] == 1)
              cell_count++;
        //
        if (col > 0)
            if (game_matrix[row][col - 1] == 1)
              cell_count++;
        //
        if (col < (COLUMNS - 1))
            if (game_matrix[row][col + 1] == 1)
              cell_count++;
        //
        if ((row < (ROWS - 1)) && (col > 0))
            if (game_matrix[row + 1][col - 1] == 1)
              cell_count++;
        //
        if (row < (ROWS - 1))
            if (game_matrix[row + 1][col] == 1)
              cell_count++;
        //
        if (row < (ROWS - 1))
            if (col < (COLUMNS - 1))
              if (game_matrix[row + 1][col + 1] == 1)
                cell_count++;
        //
        //
        if ((game_matrix[row][col] == 0) && (cell_count == 3))
            next_matrix[row][col] = 1;
        else
            next_matrix[row][col] = 0;
        //
        if (game_matrix[row][col] == 1) {
            if ((cell_count == 2) || (cell_count == 3))
              next_matrix[row][col] = 1;
            else
              next_matrix[row][col] = 0;
        }
    }
  }
}

void update_current_position() {
 //
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLUMNS; col++) {
        game_matrix[row][col] = next_matrix[row][col];
    }
  }
}

int main() {
  //
  srand(time(NULL));
  //
  SDL_Init(SDL_INIT_VIDEO);
  char *title = "Game of Life";
  //
  SDL_Window *window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SDL_Surface *surface = SDL_GetWindowSurface(window);
  //
  SDL_Rect back = (SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  SDL_FillRect(surface, &back, COLOUR_LIGHT);
  //
  random_position();
  //
  draw_grid(surface, COLUMNS, ROWS);
  //
  SDL_Event event;
  int run = 1;
  while (run) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        run = 0;
        continue;
      }
    }
    //
    SDL_FillRect(surface, &back, COLOUR_LIGHT);
    //
    draw_grid(surface, COLUMNS, ROWS);
    //
    display_position(surface);
    calculate_next_position();
    update_current_position();
    //
    SDL_UpdateWindowSurface(window);
    SDL_Delay(300);
  }
  //
  //
  exit(0);
}
