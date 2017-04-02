#include <SDL.h>
#include <iostream>
#include<string>
using std::string;
#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

void randomNextRectangle(SDL_Renderer* renderer);
void createRectangle(int posX, int posY, int largura, int altura, int vermelho, int verde, int azul, SDL_Renderer* renderer);

int gameSequence[50]; //Array que armazena a ordem do jogo
int currentPostion = 0; //Posição atual que o jogador está no jogo

int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 768;

/**
 * Função que cria uma janela SDL com o título da String passada como parâmetro
 */
SDL_Window* createSDLWindow(string windowName){
    return SDL_CreateWindow(
                 windowName.c_str(), SDL_WINDOWPOS_UNDEFINED,
                 SDL_WINDOWPOS_UNDEFINED,
                 SCREEN_WIDTH,
                 SCREEN_HEIGHT,
                 SDL_WINDOW_SHOWN
             );
}



/**
 * Cria os retângulos para o jogo
 */
void createRectangle(int posX, int posY, int largura, int altura, int vermelho, int verde, int azul, SDL_Renderer* renderer){

    SDL_Rect r;
    r.x = posX;
    r.y = posY;
    r.w = largura;
    r.h = altura;

    SDL_SetRenderDrawColor( renderer, vermelho, verde, azul, 0 );

    SDL_RenderFillRect( renderer, &r );

    SDL_RenderPresent(renderer);
}

/**
 * Pisca o retangulo
 */
void blinkRectangle(int rectangle, SDL_Renderer* renderer, bool isClick){
    int x, y, largura = SCREEN_WIDTH/2, altura = SCREEN_HEIGHT/2;
    int originalRed, originalGreen, originalBlue;
    int newRed, newGreen, newBlue;



    switch(rectangle){
        case 0:
            x = 0;
            y = 0;
            originalRed = 255;
            originalGreen = 0;
            originalBlue = 0;
            newRed = 248;
            newGreen = 154;
            newBlue = 158;
            break;
        case 1:
            x = (SCREEN_WIDTH/2)+1;
            y = 0;
            originalRed = 0;
            originalGreen = 255;
            originalBlue = 0;
            newRed = 202;
            newGreen = 255;
            newBlue = 202;
            break;
        case 2:
            x = 0;
            y = (SCREEN_HEIGHT/2)+1;
            originalRed = 0;
            originalGreen = 0;
            originalBlue = 255;
            newRed = 151;
            newGreen = 151;
            newBlue = 255;
            break;
        case 3:
            x = (SCREEN_WIDTH/2)+1;
            y = (SCREEN_HEIGHT/2)+1;
            originalRed = 255;
            originalGreen = 255;
            originalBlue = 0;
            newRed = 255;
            newGreen = 255;
            newBlue = 202;
            break;
    }

    createRectangle(x,y,largura,altura,newRed,newGreen,newBlue, renderer);
    if(isClick){
        sleep_for(nanoseconds(100000000));
    } else {
        sleep_for(seconds(1));
    }

    createRectangle(x,y,largura,altura,originalRed,originalGreen,originalBlue, renderer);
    if(!isClick){
        sleep_for(seconds(1));
    }
}


/**
 * Calcula qual retangulo foi clicado a partir das coordenadas do mouse
 */
int calculateClikedRectangle(int x, int y){
    if(x <= SCREEN_WIDTH/2 && y <= SCREEN_HEIGHT/2){
        return 0;
    }
    if(x > SCREEN_WIDTH/2 && y < SCREEN_HEIGHT/2){
        return 1;
    }
    if(x <= SCREEN_WIDTH/2 && y > SCREEN_HEIGHT/2){
        return 2;
    }
    if(x > SCREEN_WIDTH/2 && y > SCREEN_HEIGHT/2){
        return 3;
    }
    return 0;
}

/**
 *  Verifica ações do mouse
 */
void startMouseListener(SDL_Renderer* renderer){

    SDL_Event event;
    int quit = 0;
    int currentClick = 0;
    bool mistake = false;

    while (!quit){
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_MOUSEBUTTONDOWN){
                if (event.button.button == SDL_BUTTON_LEFT){
                    int clickedRectangle = calculateClikedRectangle(event.button.x, event.button.y);
                    if(clickedRectangle != gameSequence[currentClick]){
                        quit = 1;
                        mistake = true;
                    } else {
                        mistake = false;
                        currentClick++;
                        blinkRectangle(clickedRectangle,renderer,true);
                        if(currentClick == currentPostion){
                            quit = 1;
                        }
                    }
                }
            }
        }
    }

    if(!mistake){
        sleep_for(seconds(1));
        randomNextRectangle(renderer);
    }

}

/**
 * Sorteia o próximo retangulo da seguencia
 */
void randomNextRectangle(SDL_Renderer* renderer){
     /* initialize random seed: */
     srand (time(NULL));
     int rand_val = (rand() % 4);
     gameSequence[currentPostion] = rand_val;
     currentPostion++;

     int i = 0;
     for(i = 0; i < currentPostion; i++){
        blinkRectangle(gameSequence[i],renderer, false);
     }

     startMouseListener(renderer);

}

/**
 * Função principal para criação do jogo
 */
int main(int argc, char **argv){
    SDL_Window* window = createSDLWindow("SDL Genius - Douglas Campolongo");

    int i = 0;
    for(i = 0; i < 50; i++){
        gameSequence[i] = -1;
    }

    //Configura a renderização
    SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

    // Altera a cor de fundo da janela para cinza claro (195,195,195)
    SDL_SetRenderDrawColor( renderer, 195, 195, 195, 255 );

    // Limpa a janela
    SDL_RenderClear( renderer );

    createRectangle(0,0,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,255,0,0, renderer);
    createRectangle((SCREEN_WIDTH/2)+1,0,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0,255,0, renderer);
    createRectangle(0,(SCREEN_HEIGHT/2)+1,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0,0,255, renderer);
    createRectangle((SCREEN_WIDTH/2)+1,(SCREEN_HEIGHT/2)+1,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,255,255,0, renderer);

    randomNextRectangle(renderer);

    return 0;
}




