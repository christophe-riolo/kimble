#include <iostream>
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


Game::Game(int nplayers)
{
    this->nplayers = nplayers;

    // Initializing board places to null (no peg)
    for (int i = 0 ; i < BOARD_SIZE ; i++)
        board[i] = NULL;

    // Creating the players
    for (int i = 0 ; i < nplayers ; i++) 
        players.push_back(new Player(i, this));
}

Game::~Game()
{
    vector<Player*>::iterator player;

    for ( player = winners.begin() ; player < winners.end() ; player++)
        delete (*player);

    for ( player = players.begin() ; player < players.end() ; player++)
        delete (*player);
}
// Runs the whole game.
void Game::play()
{
    vector<Player*>::iterator currentPlayer = players.begin();
    // The game is finished when all players but one have won.
    while(players.size() > 1)
    {
        // Tells if we advance the iterator or not. We can
        // stay at the same place if we popped a 6 or 
        // if the player has finished.

        cout << endl << "Player " << (*currentPlayer)->number << " playing ..." << endl;
        bool replay = turn(currentPlayer);

        if(!replay)
            currentPlayer++;

        // We replay also when a player has won.
        // But if he was the last player in the vector,
        // we still need to go back to the first.
        currentPlayer =
            (currentPlayer >= players.end()) ?
            players.begin():
            currentPlayer;
    }

    displayWinners();

}

// Display the winners at the end of game.
void Game::displayWinners()
{
    for (unsigned int i = 0 ; i < winners.size() ; i++)
        cout << "#" << i+1 << ": Player " << winners[i]->number << endl;
}

// "Pops" (roll) the die.
int Game::pop()
{
    uniform_int_distribution<int> dist(1, 6);
    random_device rd;
    return dist(rd);
}

// Plays a turn of the given player. Returns true if player popped a 6 or won.
bool Game::turn(vector<Player*>::iterator player)
{
    int die = pop();
    cout << "Rolled a " << die << "." << endl;

    vector<Peg*> playable;
    (*player)->getPlayable(die, playable);

    if (playable.begin() == playable.end()) // nothing can be played
    {
        cout << "No peg to play." << endl;
        return false;
    }

    unsigned int count;
    for ( count = 0 ; count < playable.size() ;count++)
        cout << count + 1 << "> Peg at " << playable[count]->position << endl;
    
    // Read user answer
    unsigned int answer = 0;
    do
    {
        std::string sAnswer;
        getline(cin, sAnswer);
        try 
        {
            answer = stoi(sAnswer);
        }
        catch(std::invalid_argument) {continue;}
    }
    while (answer < 1 || answer > count);

    // Play said peg.
    Peg* played = playable.at(answer-1);
    
    // I took the liberty of changing the shape of the board depending
    // on the number of players (evenly distributed).
    int start = (*player)->number*(BOARD_SIZE/nplayers);
    int destination = played->position + die;
    int boardSpotDest = ( destination + start) % BOARD_SIZE;
    int boardSpotInit = ( played->position + start) % BOARD_SIZE;
    
    if (played->position == -1)
    {
        played->position = 0;

        if (board[start] != NULL)
            board[start]->position = -1;
        board[start] = played;
    }
    else if (destination < BOARD_SIZE)
    {
        if (board[boardSpotDest] != NULL)
            board[boardSpotDest]->position = -1;
        board[boardSpotDest] = played;
        board[boardSpotInit] = NULL;
        played->position += die;
    }
    else if (destination < BOARD_SIZE + FINISH_LINE_SIZE)
    {
        (*player)->finish[destination - BOARD_SIZE] = played;
        board[boardSpotInit] = NULL;
        played->position += die;
        
        // Check for win.
        bool won = true;
        for (int i = 0 ; won && i < FINISH_LINE_SIZE ; i++)
        {
            won = won && (*player)->finish[i];
        }
        if (won)
        {
            winners.push_back(*player);
            players.erase(player);
            return true;
        }
    }
    else
    {
        cout << "How did you select this peg ? Try again." << endl;
        return true;
    }
    return die == 6;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fdm=syntax
