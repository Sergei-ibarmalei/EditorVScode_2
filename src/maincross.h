#pragma once
#include "../inc/SDL.h"
#include "const.h"

namespace ed
{
    class MainCross
    {
        private:
        SDL_Point verticalUp{0, 0};
        SDL_Point verticalDown{0, 0};
        SDL_Point horizontalLeft{0, 0};
        SDL_Point horizontalRight{0, 0};

        public:
        MainCross(); 
        MainCross(const MainCross&) = delete;
        MainCross& operator=(const MainCross&) = delete;
        ~MainCross() = default;

        void ShowMainCross(SDL_Renderer* r);
    };
        
}