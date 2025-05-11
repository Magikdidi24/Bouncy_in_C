#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#define HEIGHT 900
#define WIDTH 1200
#define g 0.3
#define max_tab 1000


struct Circle{
    double x;
    double y;
    double radius;
    double v_x;
    double v_y;
};


void next(struct Circle* cercle){
    cercle->x += cercle->v_x;
    cercle->y += cercle->v_y;
    cercle->v_y += g;

    printf("a %f  , %f\n", cercle->y+cercle->radius, cercle->v_y);
    if (cercle->y+cercle->radius >= HEIGHT){
        cercle->y = HEIGHT - cercle->radius;
        cercle->v_y = -cercle->v_y;
    }else if (cercle->x+cercle->radius >= WIDTH){
        cercle->x = WIDTH - cercle->radius;
        cercle->v_x = -cercle->v_x;
    }else if (cercle->y-cercle->radius < 0){
        cercle->y = cercle->radius;
        cercle->v_y = -cercle->v_y;
    }else if (cercle->x-cercle->radius < 0){
        cercle->x = cercle->radius;
        cercle->v_x = -cercle->v_x;
    }
}

int color_random()
{
    return rand() % 0xFFFFFF;
}

void fill_circle(SDL_Surface *surface, struct Circle circle, int is_rainbow){
    int color = 0xfffffff;
    for(double i = -circle.radius; i < circle.radius; i++){
        for(double j = -circle.radius; j < circle.radius; j++){
            if((j*j+i*i) <= circle.radius*circle.radius){
                color = (is_rainbow == 1) ? color_random() : color;
                SDL_FillRect(surface, &((SDL_Rect) {.x=j + circle.x, .y=i+circle.y, .h=1, .w=1}), color);
            }    
        }
    }
}

void drawn_point(int trajectoire_x[max_tab], int trajectoire_y[max_tab], SDL_Surface *surface, int is_rainbow){
    int color = 0xff0000;
    for(int i = 0; i< max_tab; i++){
        if (trajectoire_x[i] == -1 || trajectoire_y[i] == -1){
            continue;
        }
        color = (is_rainbow ) ? color_random() : color;
        SDL_FillRect(surface, &((SDL_Rect) {.x = trajectoire_x[i], .y = trajectoire_y[i], .w = 5, .h=5}), color);
    }
}

int main(void){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "SDL_error: %s\n", SDL_GetError());
    }
    SDL_Window *window = SDL_CreateWindow("Bouncing Ball", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH,HEIGHT, 0);
    if (!window) {
        fprintf(stderr, "Window Creation Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *surface = SDL_GetWindowSurface(window);
    if (!surface) {
        fprintf(stderr, "Surface Creation Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    srand( time( NULL ) );
    int searchedValue = rand() % 5;

    struct Circle cercle = {.x = 200, .y = 200, .radius = 50, .v_x = 6, .v_y = 0};
    SDL_Rect erase_rect = {0,0,WIDTH, HEIGHT};
    int trajectoire_x[max_tab] = {-1};
    int trajectoire_y[max_tab] = {-1};
    



    int k = 0;
    int running = 1;
    int is_rainbow = 0;
    while(running){
        SDL_Event ev;
        while(SDL_PollEvent(&ev)){
            switch (ev.type)
            {
                case (SDL_QUIT):
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return 0;
                    break;
                
                case(SDL_KEYDOWN):
                    switch (ev.key.keysym.sym)
                    {
                        case SDLK_SPACE:
                            is_rainbow = !is_rainbow;
                            break;
                        
                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        k %=max_tab;
        trajectoire_x[k] = cercle.x;
        trajectoire_y[k] = cercle.y;
        k++;
        SDL_FillRect(surface, &erase_rect, 0x00000);
        drawn_point(trajectoire_x, trajectoire_y, surface, is_rainbow);
        fill_circle(surface, cercle,is_rainbow);
        next(&cercle);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(20);
    }
}  