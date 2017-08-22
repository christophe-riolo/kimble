#ifndef KIMBLE_H
#define KIMBLE_H

#include <vector>
#include <array>

using std::vector;
using std::array;

#define BOARD_SIZE 28
#define FINISH_LINE_SIZE 4
#define MAX_PLAYERS 4
#define PEGS_NUMBER 4

class Player;
class Game;

class Peg
{
    public:
        Player* owner;
        int position = -1;
        Peg(Player* owner);
};

class Player 
{
    public:
        Player(int number, Game* game);
        ~Player();
        int number;
        Game* game;
        void getPlayable(int die, vector<Peg*>& playable);
        array<Peg*,FINISH_LINE_SIZE> finish; 
        array<Peg*,PEGS_NUMBER> pegs;
};


class Game 
{
    public:
        Game(int nplayers = MAX_PLAYERS);
        ~Game();
        void play();
        array<Peg*,BOARD_SIZE> board;
        int nplayers;
    private:
        vector<Player*> players;
        vector<Player*> winners;

        // Plays a turn of the given player. Returns true if player popped a 6 or won.
        bool turn(vector<Player*>::iterator player);
        void displayWinners();
        int pop();
};
#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fdm=syntax
