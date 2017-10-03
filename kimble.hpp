#ifndef KIMBLE_H
#define KIMBLE_H

#include <vector>
#include <array>
#include <list>
#include <iostream>

using std::vector;
using std::array;
using std::list;
using std::ostream;

namespace Kimble {
    const int board_size {28};
    const int finish_line_size {4};
    const int max_players {4};
    const int pegs_number {4};
}

class Player;
class Game;

class Peg
{
    public:
        unsigned int boardPos() const;
        int& getPosition() 
        {
            return m_position;
        }
        const Player& getOwner() const;
        Peg(const Player& owner);
        friend ostream& operator<<(ostream& out, const Peg& peg);

    private:
        const Player& m_owner;
        int m_position = -1;
};

class Player 
{
    public:
        Player(Game& game);
        void getPlayable(int die, vector<Peg*> &playable);
        bool hasWon() const;
        int getNumber() const
        {
            return m_number;
        }
        const Game& getGame() const
        {
            return m_game;
        }
        array<Peg*,Kimble::finish_line_size>& getFinish()
        {
            return m_finish;
        }
        vector<Peg>& getPegs()
        {
            return m_pegs;
        }
        friend ostream& operator<<(ostream& out, const Player& player);

        // Comparison of players is made by number
        bool operator!=(const Player p) const
        {
            return m_number != p.m_number;
        }
    private:
        static int n_players;
        vector<Peg> m_pegs;
        int m_number;
        Game& m_game;
        array<Peg*,Kimble::finish_line_size> m_finish; 
};


class Game 
{
    public:
        Game(int nplayers);
        Game();
        void play();
        void displayAll();
        void displayWinners();
        const int m_nplayers;
        const array<Peg*,Kimble::board_size>& getBoard() const;
    private:
        array<Peg*,Kimble::board_size> m_board;
        list<Player> m_players;
        vector<Player> m_winners;

        // Plays a turn of the given player. Returns true if player popped a 6 or won.
        bool turn(list<Player>::iterator player);
        int pop();
};
#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fdm=syntax
