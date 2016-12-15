#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hlt.h"

#define BOT_NAME "Primis-V7"

int main(void) {

    GAME game;
    SITE target;

    int x, y, direction, neutral_count;

    srand(time(NULL));


    // Start the game
    game = GetInit();
    SendInit(BOT_NAME);
    int delay = 0;
    int count = 0;
    int sentinel = rand() % 20;
    int current_dir;
    int large_direction = 1;
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
                        } else if ((target.owner != 0) && (target.owner != game.playertag)) {
                            if (delay > 1) {
                                large_direction = direction;
                                delay = 27;
                            }
                            SetMove(game, x, y, large_direction);
                            break;
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
                        if(game.strength[x][y] > (rand() % 30 + 100)) {
                            direction = large_direction;
                            if (delay > 0) { // Not in attack phase
                                if (large_direction == 1) {
                                    large_direction = 2;
                                } else {
                                    large_direction = 1;
                                }
                            }
                        } else if(rand() % 2) {
                            direction = rand() % 5;
                        } else {
                            direction = 0;
                        }
                        target = GetSiteFromMovement(game, x, y, direction);
                        if (((target.strength + game.strength[x][y]) > 270) && (direction > 0)) {
                            // Wasting too much, go somewhere else
                            direction += 1;
                            if (direction > 4) {
                                direction = 1;
                            }
                        }
                        SetMove(game, x, y, direction);

                    }
                }
            }
        }
        delay--;
        SendFrame(game);
    }
    return 0;
}
