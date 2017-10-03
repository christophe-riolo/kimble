#include "kimble.hpp"

int Player::n_players {0};

Player::Player(Game& game):
    m_pegs{Kimble::pegs_number, Peg(*this)},
    m_number{n_players},
    m_game{game},
    m_finish{}
{
    ++n_players;
}

bool Player::hasWon() const
{
    // Check for win.
    bool won = true;
    for (int i = 0 ; won && i < Kimble::finish_line_size ; i++)
    {
        won = won && m_finish[i];
    }
    return won;
}

ostream& operator<<(ostream& out, const Player& player)
{
    out << "Player " << player.m_number << ":\n";
    for (auto peg = player.m_pegs.begin(); peg < player.m_pegs.end() ; peg++)
        out << "\t" << *peg;
    return out;
}

void Player::getPlayable(int die, vector<Peg*> &playable)
{
    for (vector<Peg>::iterator peg = m_pegs.begin() ; peg < m_pegs.end() ; peg++)
    {
        // The starting point on the board for current player.
        int startSpot = (m_number*(Kimble::board_size/(m_game.m_nplayers)));

        // Testing if peg is out of game.
        if (peg->getPosition() == -1)
        {
            if (
                    die == 6
                && (m_game.getBoard()[startSpot] == nullptr
                ||  m_game.getBoard()[startSpot]->getOwner() != *this)
               ) 
                playable.push_back(&*peg);
        }
        else 
        {
            // destination is where the peg would go on its way from sart (0) to the finish line.
            // boardSpot on the other hand is the actual index of game->board where it goes.
            int destination = peg->getPosition() + die;
            int boardSpot = ( destination + startSpot) % Kimble::board_size;
            
            // We must check that we will not go on our own peg.
            if (
                        destination < Kimble::board_size
                    && (m_game.getBoard()[boardSpot] == nullptr
                    ||  m_game.getBoard()[boardSpot]->getOwner() != *this)
               )
                playable.push_back(&*peg);
            else if (
                    destination < Kimble::board_size + Kimble::finish_line_size
                    && m_finish[destination - Kimble::board_size] == nullptr
                    )
                playable.push_back(&*peg);
            // If the peg would go overboard (pun intended) of on our own other peg,
            // we do not add it.

        }
    }
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fdm=syntax
