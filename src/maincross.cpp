#include "maincross.h"

ed::MainCross::MainCross()
{
    verticalUp.x = workTableStartX + (workTableSide >> 1);
    verticalUp.y = workTableStartY;
    verticalDown.x = verticalUp.x;
    verticalDown.y = workTableStartY + workTableSide;

    horizontalLeft.x = workTableStartX;
    horizontalLeft.y = workTableStartY + (workTableSide >> 1);
    horizontalRight.x = workTableStartX + workTableSide;
    horizontalRight.y = horizontalLeft.y;
}

void ed::MainCross::ShowMainCross(SDL_Renderer *r)
{
    SDL_SetRenderDrawColor(r, 0xffu, 0, 0, 0xffu);
    SDL_RenderDrawLine(r, verticalUp.x, verticalUp.y, verticalDown.x, 
        verticalDown.y);
    SDL_RenderDrawLine(r, horizontalLeft.x, horizontalLeft.y,
        horizontalRight.x, horizontalRight.y);
}
