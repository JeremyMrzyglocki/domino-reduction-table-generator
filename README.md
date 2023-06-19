# domino-reduction-table-generator

In Fewest Moves Rubik's Cube solving an optional step after reducing the possible states of the cube to the set of solved edge orientation (EO), one can go further and reduce into a set called "domino-reduction" (DR) which visually means that on two opposite sides of the Rubik's Cube, there are only two kind of sticker colors.

In October 2022, I programmed this table-generator that brute-forces all possible 7-move-algorithms and saves the algorithm together with a string of state-describing numbers, and the inverse solution into a big array that is printed out into the console. I might work on this further or merge it with my halfturn-reduction-alg-generator and a post-HTR solver and a scramble->DR solver to build a very efficient full scramble Rubik's Cube solver.

Some parts of the code and the comments are in German
