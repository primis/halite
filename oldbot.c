#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hlt.h"

#define BOT_NAME "Primis-V5"

int main(void) {

    GAME game;
    SITE target;

    int x, y, direction, neutral_count;

    srand(time(NULL));


    // Start the game
    game = GetInit();
    SendInit(BOT_NAME);
    int count = 0;
    int sentinel = rand() % 15;
    int current_dir;
    int large_direction=1;
    while (1) {

        GetFrame(game);

        for (x = 0 ; x < game.width ; x++) {
            for (y = 0 ; y < game.height ; y++) {
                if (game.owner[x][y] == game.playertag) {
                    neutral_count = 0;
                    int lowest_strength = 255;
                    current_dir = 0;
                    // Attack a nearby neutral if we can
                    for (direction = 1; direction <= 5; direction++) {
                        target = GetSiteFromMovement(game, x, y, direction);
                        if (target.owner == 0) {
                            neutral_count++;
                            if (target.strength < game.strength[x][y]) {
                                if (target.strength < lowest_strength) {
                                    current_dir = direction;
                                }
                            }
                        }
                    }
                    if (current_dir > 0) {
                        SetMove(game, x, y, current_dir);
                    }

                    // Early game sentinel stuff
                    if (neutral_count == 0 && sentinel > 0) {
                        direction = 1; // Move north
                        sentinel--;
                        SetMove(game, x, y, direction);
                    }

                    // If there are zero neutrals, and we're past sentinel
                    if (neutral_count == 0 && sentinel < 1) {
                        // Are we OP?
                        if(game.strength[x][y] > (rand()%50 + 200)) {
                            if(large_direction==1){
                                direction=2;
                                large_direction=2;
                            } else {
                                direction = 1;
                                large_direction=1;
                            }
                        } else if(rand() % 2) {
                            direction = rand() % 5;
                        } else {
                            direction = 0;
                        }
                        SetMove(game, x, y, direction);

                    }
                }
            }
        }
        SendFrame(game);
    }
    return 0;
}
