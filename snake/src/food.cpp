#include "stdlib.h"
#include "config.h"

void createFood(int* bg, int food[2], const int& nx, const int& ny){
    if( food[0] == 0){
        food[0] = rand()%(nx-2) + 1;//j'ai mis 1 à la place de 2
        food[1] = rand()%(ny-2) + 1;//idem
        bg[food[1]*nx+food[0]] = 2;

    }
}

bool eatFood(int food[2], int* snake){
    if( (food[0]== snake[0]) && (food[1] == snake[SNAKE_LEN]) ){
        food[0] = 0;
        food[1] = 0;
        return true;
    }
    return false;
}