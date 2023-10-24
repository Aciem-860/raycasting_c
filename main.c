#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "constants.h"
#include "utils.h"

struct Point {
    int x;
    int y;
};
typedef struct Point Point;

uint8_t map[MAP_H * MAP_W];
Point player_pos;

int sgn(double x);
void show_map(SDL_Renderer*);
void drawCircle(SDL_Renderer*, Point*, int);
double norm(Point*);
Point getIntersect(Point*, Point*);

int sgn(double x) {
    return (x >= 0) ? 1 : -1;
}

double norm(Point * v) {
    return sqrt(pow(v->x, 2) + pow(v->y, 2));
}

Point getIntersect(Point * src, Point * dir) {
    double dx = TILE_LENGTH;
    double dy = TILE_LENGTH;
    double slope = dir->x;
    if (dir->y != 0) {
        slope = (double)dir->x/(double)dir->y;
    }

    double lx = dx * sqrt(1 + 1 / pow(slope, 2));
    double ly = dy * sqrt(1 + pow(slope, 2));

    dx = dx * ((dir->x < 0) ? -1 : 1);
    dy = dy * ((dir->y < 0) ? -1 : 1);

    Point intersect;

    if (lx < ly) {
        intersect.x = src->x + dx;
        intersect.y = src->y + dx / slope;
    }
    else {
        intersect.x = src->x + dy * slope;
        intersect.y = src->y + dy;
    }

    return intersect;
}

void drawCircle(SDL_Renderer* renderer, Point* pos, int radius) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    for (int i = 0; i < 360; i++) {
        double angle = i * M_PI / 180; // ° -> rad
        int x = pos->x + (int)(radius * cos(angle));
        int y = pos->y + (int)(radius * sin(angle));    
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

void show_map(SDL_Renderer* renderer) {
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
}

void draw_rays(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    double angle;
    for (int i = 0; i < 360; i+=10) {
        angle = i * M_PI/180; // angle en radians
        Point p = {player_pos.x + LENGTH_RAY * cos(angle), player_pos.y + LENGTH_RAY * sin(angle)}; // point d'arrivée du rayon
        Point dir = {p.x - player_pos.x, p.y - player_pos.y};
        Point dst = getIntersect(&player_pos, &dir);
        SDL_RenderDrawLine(renderer, player_pos.x, player_pos.y, dst.x, dst.y);
    }
}

void show_player(SDL_Renderer* renderer) {
    SDL_GetMouseState(&player_pos.x, &player_pos.y);
    drawCircle(renderer, &player_pos, 10);
}

int main(int argc, char** argv)
{
	parse_map(MAP_FILE);

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

    SDL_RenderPresent(renderer);
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Couleur de fond (noir)
        SDL_RenderClear(renderer);

        show_map(renderer);
        show_player(renderer);
        draw_rays(renderer);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_RenderPresent(renderer);
    }

    // Libérer les ressources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}