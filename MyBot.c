#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hlt.h"

#define BOT_NAME "Primis-V3"

int main(void) {

    GAME game;
    SITE target;

    int x, y, direction, neutral_count;

    srand(time(NULL));


    // Start the game
    game = GetInit();
    SendInit(BOT_NAME);
    int startX = 0;
    int startY = 0;
    int current_dir;
    int count = 0;
    int large_direction = 1;

    while (1) {
        GetFrame(game);
        count++;
        for (x = 0 ; x < game.width ; x++) {
            for (y = 0 ; y < game.height ; y++) {
                if (game.owner[x][y] == game.playertag) {
                    if(count == 0 ) {
                        startX = x;
                        startY = y;
                    }
                    neutral_count = 0;
                    int lowest_strength = 0;
                    current_dir=0;
                    // Attack a nearby neutral if we can
                    for (direction = 1; direction <= 5; direction++) {
                        target = GetSiteFromMovement(game, x, y, direction);
                        if (target.owner == 0) {
                            neutral_count++;
                            if (target.strength < game.strength[x][y]) {
                                SetMove(game, x, y, direction);
                                break;
                            } else {
                                if(target.strength < lowest_strength) {
                                    current_dir = direction;
                                    lowest_strength = target.strength;
                                }
                            }
                        }
                    }
                    // If there are zero neutrals, and we're past sentinel
                    if (neutral_count == 0) {
                        // Are we OP?
                        if (game.strength[x][y] > ( rand() % 50 + 200 ) && count > 40 ) {
                            if (y > startY) {
                                if (x > startX) {
                                    direction = 1 + (3 * large_direction);
                                } else {
                                    direction = 1 + large_direction;
                                }
                            } else {
                                if (x > startX) {
                                    direction = 3 + large_direction;
                                } else {
                                    direction = 3 - large_direction;
                                }
                            }
                            if (large_direction == 1) {
                                large_direction = 0;
                            } else {
                                large_direction = 1;
                            }
                        } else {
                            direction = rand() % 5 ; // stay still
                        }
                    }
                    SetMove(game, x, y, direction);
                }
            }
        }
        SendFrame(game);
    }
    return 0;
}
