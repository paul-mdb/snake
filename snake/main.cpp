#include <iostream>
#include "sys/ioctl.h"

#include "stdlib.h"

#include "config.h"
#include "internals.h"
#include "display.h"
#include "food.h"


void backgroundSetup( const int nx, const int ny, int background[] ){
  for (int i=0; i<nx; i++){
    background[i] = 1;
    background[i+nx*(ny-1)] = 1;
  }
  for (int j=0; j<ny; j++){
    background[j*nx] = 1;
    background[j*nx + nx-1] = 1;
  }
}

void add_snake( int snake[2*SNAKE_LEN], int background[], int snl, const int nx, const int ny ){
  for (int i = 0; i < snl; i++) {
    background[snake[i] + nx*snake[i + SNAKE_LEN]] = 1;
  }
}

void remove_snake( int snake[2*SNAKE_LEN], int background[], int snake_len, const int nx, const int ny ){
  for (int i = 0; i < snake_len; i++) {
    background[snake[i] + nx*snake[i + SNAKE_LEN]] = 0;
  }
}

void snake_movement( char key, int dxdy [] ){
  if (key == 's'){// 's' = gauche
    dxdy[0] = -1;
    dxdy[1] = 0;
  }

  else if (key == 'f'){// 'f' = droite
    dxdy[0] = 1;
    dxdy[1] = 0;
  }
  else if (key == 'd'){// 'd' = bas
    dxdy[0] = 0;
    dxdy[1] = 1;
  }
  else if (key == 'e'){// 'e' = haut
    dxdy[0] = 0;
    dxdy[1] = -1;
  }
}

bool verifyBorder( int snake[2*SNAKE_LEN], int snl, const int nx, const int ny ){
  bool boo = true;
  boo = (1<=snake[0]) && (snake[0]<nx-1) && (1<=snake[SNAKE_LEN]) && (snake[SNAKE_LEN]<ny-1);
  /*
  for (int i=2; i<snl; i++){
    boo = boo && !( (snake[i]==snake[0]) && (snake[i+SNAKE_LEN]==snake[SNAKE_LEN]) );
  }
  */
  return boo;
}

void setupSnake( int snake[2*SNAKE_LEN], int snake_len ){
  int x_init = 10;
  int y_init = 10;
  for (int i=0; i<snake_len; i++){
    snake[i] = x_init +i;
    snake[i+SNAKE_LEN] = y_init;
  }
}

void update_snake_coordinates( int snake[2*SNAKE_LEN], int snl, int dxdy[] ){
    for (int i=snl; i > 0; i--){
      snake[i] = snake[i-1];
      snake[i+SNAKE_LEN] = snake[i+SNAKE_LEN-1];
    }
    snake[0] = snake[0] + dxdy[0];
    snake[SNAKE_LEN] = snake[SNAKE_LEN] + dxdy[1];
  }

void startGame(const int& nx, const int& ny, int& snl, int* snake, int* bg){
    char key;
    int dxdy[2] = {1,0};
    int food[2] = {0,0};
    int score = 0;
    int lap = 300;

    createFood( bg, food, nx, ny );

    while( true ){
        internal::frameSleep(lap);
        if( internal::keyEvent() ){
            std::cin >> key; 
            snake_movement(key, dxdy);
        }
        backgroundClear();
        add_snake( snake, bg, snl, nx, ny );
        printFrame(nx, ny, bg);
        remove_snake(snake, bg, snl, nx, ny);
        
        bool out =  verifyBorder(snake, snl, nx, ny);
        if( out == false ){
            std::cout << "Bravo ! Vous avez effectué un score de ";
            std::cout << score << std::endl;
            std::cerr << "" << std::endl;
            exit(1);
        }
        
        bool eat = eatFood(food, snake);
        if(eat){
            createFood(bg, food, nx, ny);
            snl++;
            score++;
            if (lap > 30){
              lap = lap - 10;
            }
        }
        update_snake_coordinates(snake, snl, dxdy);

    }
}



int main(){
    const int nx = 50;
    const int ny = 25;
    

    int snake_len = 4;
    int background[nx*ny] ;
    for( int i=0; i<nx*ny; i++){
        background[i] = -1;
    }
    int snake[2*SNAKE_LEN];

    backgroundSetup(nx, ny, background);
    setupSnake(snake, snake_len);


    startGame(nx, ny, snake_len, snake, background);

}