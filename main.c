#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include "constants.h"
#include "utils.h"

uint8_t map[MAP_H * MAP_W];

int main(int argc, char** argv)
{
	parse_map(MAP_FILE);
	print_map();

    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Erreur lors de l'initialisation de la SDL : %s", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if (!window) {
        SDL_Log("Erreur lors de la création de la fenêtre : %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du rendu
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Erreur lors de la création du rendu : %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (int x = 0; x < MAP_W; x++) {
		for (int y = 0; y < MAP_H; y++) {
			int loc = MAP_W * y + x;
			if (map[loc] != 0) {
				SDL_Rect carre = { x * TILE_LENGTH, y * TILE_LENGTH, TILE_LENGTH, TILE_LENGTH }; // Position (x, y) et taille (largeur, hauteur) du carré
				SDL_RenderFillRect(renderer, &carre);
			}
		}
	}

    // Mettre à jour l'affichage
    SDL_RenderPresent(renderer);

    // Boucle principale pour attendre la fermeture de la fenêtre
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    // Libérer les ressources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}