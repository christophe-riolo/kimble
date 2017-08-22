#include "kimble.hpp"

Player::Player(int number, Game* game)
{
    this->number = number;
    this->game = game;

    for (int i = 0 ; i < FINISH_LINE_SIZE ; i++)
    {
        finish[i] = NULL; 
    }

    for (int i = 0 ; i < PEGS_NUMBER; i++)
    {
        pegs[i] = new Peg(this); 
    }
}

Player::~Player()
{
    for (int i = 0 ; i < PEGS_NUMBER ; i++)
        delete pegs[i];
}

void Player::getPlayable(int die, vector<Peg*>& playable)
{
    for (array<Peg*,PEGS_NUMBER>::iterator peg = pegs.begin() ; peg < pegs.end() ; peg++)
    {
        // The starting point on the board for current player.
        int startSpot = (number*(BOARD_SIZE/(game->nplayers)));

        // Testing if peg is out of game.
        if ((*peg)->position == -1)
        {
            if (
                    die == 6
                && (game->board[startSpot] == NULL
                ||  game->board[startSpot]->owner != this)
               ) 
                playable.push_back(*peg);
        }
        else 
        {
            // destination is where the peg would go on its way from sart (0) to the finish line.
            // boardSpot on the other hand is the actual index of game->board where it goes.
            int destination = (*peg)->position + die;
            int boardSpot = ( destination + startSpot) % BOARD_SIZE;
            
            // We must check that we will not go on our own peg.
            if (
                        destination < BOARD_SIZE
                    && (game->board[boardSpot] == NULL
                    ||  game->board[boardSpot]->owner != this)
               )
                playable.push_back(*peg);
            else if (
                    destination < BOARD_SIZE + FINISH_LINE_SIZE
                    && finish[destination - BOARD_SIZE] == NULL
                    )
                playable.push_back(*peg);
            // If the peg would go overboard (pun intended) of on our own other peg,
            // we do not add it.

        }
    }
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fdm=syntax
