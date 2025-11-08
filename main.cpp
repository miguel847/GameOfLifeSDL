#include <iostream>
#include <unordered_map>
#include "logic.h"
#include <list>
#include <utility>
#include <SDL.h>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std; 

// Globals
const int secondsBetweenTicks = 1;
const int pixelsPerMovement = 3;
atomic<bool> modifyTextureIsNeeded = true;

void printStade(unordered_map<pair<int,int>, bool, hash_pair> gS){
	for (auto i = gS.begin(); i != gS.end(); ++i){
		cout << "(" << (*i).first.first << "," << (*i).first.second << ") \n";
	}
	cout << "\n";
}

void RunSimulation(GridClass gridClass, unordered_map<pair<int,int>, bool, hash_pair>* renderGrid){
	while (!gridClass.grid.empty()){
		this_thread::sleep_for(chrono::seconds(secondsBetweenTicks));
		gridClass.handleTick();
		*renderGrid = gridClass.grid;
		modifyTextureIsNeeded = true;
	}
}

int main(int argv, char** args){
	//SDL stuff
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_CreateWindowAndRenderer(600, 600, 0, &window, &renderer);
	SDL_SetWindowTitle(window, "Conway's Game of life");
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
	SDL_Event event;
	bool running = true;
	
	SDL_Rect source{600,600, 600/32, 600/32};
	SDL_Rect display {20,20, 600-20, 600-20};
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRX8888, SDL_TEXTUREACCESS_TARGET, 1200, 1200);
	
	
	// Program begins
	list<pair<int,int>> input{{0,0},{1,0},{2,0},{1,1},{2,1}};
	GridClass gridClass (input);
	auto renderGrid = gridClass.grid;
	thread simulationThread(RunSimulation, ref(gridClass), &renderGrid);
	
	while (running == true){
		if (modifyTextureIsNeeded == true){
			SDL_SetRenderTarget(renderer, texture);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			for (auto i = renderGrid.begin(); i != renderGrid.end(); ++i){
				SDL_RenderDrawPoint(renderer, (*i).first.first+603, (*i).first.second+603);
			}
			SDL_SetRenderTarget(renderer, NULL);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			modifyTextureIsNeeded = false;
		}
		SDL_RenderCopy(renderer, texture, &source, &display);
		while (SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_KEYDOWN:
					switch( event.key.keysym.sym){
						case SDLK_RIGHT:
							source.x += pixelsPerMovement;
							break;
						case SDLK_LEFT:
							source.x -= pixelsPerMovement;
							break;
						case SDLK_UP:
							source.y -= pixelsPerMovement;
							break;
						case SDLK_DOWN:
							source.y += 3;
							break;
						default:
							break;
					}
					break;
				case SDL_QUIT:
					running = false;
					break;
				default:
					break;
			}
		}
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}
	
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	return 0;
}