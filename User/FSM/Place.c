//
// Created by BXW on 2024/7/29.
//

#include "Place.h"

const chess_position chess_grid[9]={
        {0,0},
        {1,1},
        {2,2},
        {3,3},
        {4,4},
        {5,5},
        {6,6},
        {7,7},
        {8,8}
};

void place_chess(const uint8_t position){
    M2006_SetPosition(1,chess_grid[position].x_axis);
    M2006_SetPosition(2,chess_grid[position].y_axis);
}