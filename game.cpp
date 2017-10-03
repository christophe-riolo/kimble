#include <string>
#include <random>
#include "kimble.hpp"

using std::cout;
using std::cin;
using std::getline;
using std::endl;
using std::stoi;
using std::uniform_int_distribution;
using std::random_device;


Game::Game(int nplayers):
    m_nplayers{nplayers},
    m_board{}
{
    // Creating the players
    for (int i = 0 ; i < nplayers ; i++) 
        m_players.emplace_back(*this);
}

Game::Game():
    Game(4)
{
}

// Runs the whole game.
void Game::play()
{
    list<Player>::iterator currentPlayer = m_players.begin();
    // The game is finished when all players but one have won.
    while(m_players.size() > 1)
    {
        // Tells if we advance the iterator or not. We can
        // stay at the same place if we popped a 6 or 
        // if the player has finished.

        cout << endl << "Player " << currentPlayer->getNumber() << " playing ..." << endl;
        bool replay = turn(currentPlayer);

        if (currentPlayer->hasWon())
        {
            cout << "Player " << currentPlayer->getNumber() << " finished !" << endl;
            m_winners.push_back(*currentPlayer);
            currentPlayer = m_players.erase(currentPlayer);
            displayAll();
            replay = true;
        }

        if(!replay)
            currentPlayer++;


        // We replay also when a player has won.
        // But if he was the last player in the vector,
        // we still need to go back to the first.
        currentPlayer =
            (currentPlayer == m_players.end()) ?
            m_players.begin():
            currentPlayer;
    }

    cout << "Game over !";
    displayWinners();

}

// Display the winners at the end of game.
void Game::displayWinners()
{
    cout << "\nWinners :" << endl;
    for (unsigned int i = 0 ; i < m_winners.size() ; i++)
        cout  << "#" << i+1 << ": Player " << m_winners[i].getNumber() << endl;
}

// Display situation
void Game::displayAll()
{
    displayWinners();
    cout << "\nPlayers :" << endl;
    for (auto player = m_players.begin() ; player != m_players.end() ; player++)
        cout << *player << "\n";
    cout << endl;
}

// "Pops" (roll) the die.
int Game::pop()
{
    uniform_int_distribution<int> dist(1, 6);
    random_device rd;
    return dist(rd);
}

// Plays a turn of the given player. Returns true if player popped a 6 or won.
bool Game::turn(list<Player>::iterator player)
{
    int die = pop();
    cout << "Rolled a " << die << "." << endl;

    vector<Peg*> playable;
    player->getPlayable(die, playable);

    if (playable.begin() == playable.end()) // nothing can be played
    {
        cout << "No peg to play." << endl;
        return false;
    }

    unsigned int count;

    unsigned int answer = 0;

    for (count = 0 ; count < playable.size() ;count++)
    {
        cout << count + 1 << "> Peg at " << playable[count]->getPosition() << endl;
        if (answer == 0 || playable[answer]->getPosition() < playable[count]->getPosition())
            answer = count + 1;
    }

    cout << "Choosing peg " << answer << endl;
    
    // Read user answer
    /*
     * DISCLAIMER : I had not read properly that the peg
     * has to be chosen by the program, I leave this piece
     * of code for further reference (and possible use
     * in a "strategy object".
     * 
     *do
     *{
     *    std::string sAnswer;
     *    getline(cin, sAnswer);
     *    try 
     *    {
     *        answer = stoi(sAnswer);
     *    }
     *    catch(std::invalid_argument) {continue;}
     *}
     *while (answer < 1 || answer > count);
     */
     
     // Play said peg.
     auto played = playable.at(answer-1);
     
    
    // I took the liberty of changing the shape of the board depending
    // on the number of players (evenly distributed).
    int start = player->getNumber()*(Kimble::board_size/m_nplayers);
    int destination = played->getPosition() + die;
    int boardSpotDest = ( destination + start) % Kimble::board_size;
    int boardSpotInit = played->boardPos();
    
    if (played->getPosition() == -1)
    {
        played->getPosition() = 0;

        if (m_board[start] != nullptr)
            m_board[start]->getPosition() = -1;
        m_board[start] = played;
    }
    else if (destination < Kimble::board_size)
    {
        if (m_board[boardSpotDest] != nullptr)
            m_board[boardSpotDest]->getPosition() = -1;
        m_board[boardSpotDest] = played;
        m_board[boardSpotInit] = nullptr;
        played->getPosition() += die;
    }
    else if (destination < Kimble::board_size + Kimble::finish_line_size)
    {
        player->getFinish()[destination - Kimble::board_size] = played;
        if (played->getPosition() < Kimble::board_size)
            m_board[boardSpotInit] = nullptr;
        else
            player->getFinish()[played->getPosition() - Kimble::board_size] = nullptr;
        played->getPosition() += die;
        
        return false;
    }
    else
    {
        cout << "How did you select this peg ? Try again." << endl;
        displayAll();
        return true;
    }
    displayAll();
    return die == 6;
}

const array<Peg*,Kimble::board_size>& Game::getBoard() const
{
    return m_board;
}


// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fdm=syntax
