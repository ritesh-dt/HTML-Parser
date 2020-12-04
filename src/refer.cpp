#include <stdio.h>
#include <SDL.h>
#include <iostream>
using namespace std;
// #include <SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurfaces {
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL	
};

bool init();
bool loadMedia();
void close();

SDL_Surface* loadSurface(string);
SDL_Texture* loadTexture(string);

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
SDL_Surface* gCurrentSurface = NULL;

int main(int argc, char* args[]) {
	

	if (!init()) {
		printf("SDL could not initialize!");
	} else {
		if (!loadMedia()) {
			printf("Media could not be created!");
		} else {
			bool quit = false;
			SDL_Event e;
			while (!quit) {
				
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) 	quit = true;
					
					else if (e.type == SDL_KEYDOWN) {
						switch(e.key.keysym.sym) {
							case SDLK_UP:
								
								break;

							default:
								break;
						}
					}
				}
				
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				SDL_Rect fillRect = {SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
				SDL_RenderFillRect(gRenderer, &fillRect);
				// SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				// SDL_Rect viewport = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
				// SDL_RenderSetViewport(gRenderer, &viewport);

				// for (int i=0; i<SCREEN_HEIGHT; i+=3) {
				// 	SDL_SetRenderDrawColor(gRenderer, i, 0x00, 0xFF, 0xFF);
				// 	SDL_RenderDrawPoint(gRenderer, 0, i);
				// }
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	close();

	return 0;
}

// Create a new window
bool init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		} else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL){
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			} else {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

			}
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

bool loadMedia() {
	bool success = true;

	// gTexture = loadTexture("hello.bmp");
	// if (gTexture == NULL) {
	// 	printf("Could not load texture image %s! SDL Error: %s\n", "hello.bmp", SDL_GetError());
	// 	success = false;
	// }
	return success;
}

void close() {
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	SDL_Quit();
}

SDL_Surface* loadSurface(string path) {
	SDL_Surface* optimizedSurface = NULL;

	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL) {
		printf("Surface could not be loaded! SDL Error: %s\n", SDL_GetError());
	} else {
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if (optimizedSurface == NULL) {
			printf("Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	return optimizedSurface;
}

SDL_Texture* loadTexture(string path) {
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL) {
		printf("Surface could not be loaded! SDL Error: %s\n", SDL_GetError());
	} else {
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Texture could not be created! SDL Error: %s\n", SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}