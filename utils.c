#include <stdio.h>
#include <stdint.h>
#include "constants.h"
#include "utils.h"

extern uint8_t map[MAP_H * MAP_W];

void parse_map(char * filename) {
	FILE *f;
    char c;
    f = fopen(filename,"rt");

	int cpt = 0;
    while((c=fgetc(f)) != EOF){
		if (c >= 48) { // i.e. un caractère ASCII
			map[cpt] = c;
			cpt++;
		}
    }

    fclose(f);

	for (int i = 0; i < MAP_W * MAP_H; i++) {
		map[i] -= 48;
	}
}

void print_map() {
	for (int i = 0; i < MAP_H * MAP_W; i++) {
		if (i % MAP_W == 0) {
			printf("\n");
		}
		printf("%d", map[i]);
	}
}