#include <iostream>
#include <random>
#include "kimble.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::uniform_int_distribution;
using std::random_device;


Game::Game(int nplayers)
{
    // Initializing board places to null (no peg)
    for (int i = 0 ; i < BOARD_SIZE ; i++)
        board[i] = NULL;

    // Creating the players
    for (int i = 0 ; i < nplayers ; i++) 
        players.push_back(new Player(i));
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

        cout << "Player " << (*currentPlayer)->number << " playing ..." << endl;
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
    for (int i = 0 ; i < winners.size() ; i++)
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

    vector<Peg*> playable;
    (*player)->getPlayable(die, playable);

    int count;
    for ( count = 0 ; count < playable.size() ;count++)
        cout << count + 1 << "> Peg at " << playable[count]->position << endl;
    
    // Read user answer
    int answer = 0;
    do
        cin >> answer;
    while (answer < 1 || answer >= count);

    // Play said peg.
    Peg* played = playable.at(answer);
    if (played->position == -1)
    {
        played->position = 0;

        // I took the liberty of changing the shape of the board depending
        // on the number of players (evenly distributed).
        int start = (*player)->number*(BOARD_SIZE/players.size());
        if (board[start] != NULL)
            board[start]->position = -1;
        board[start] = played;
    }
    else if (played->position + die < BOARD_SIZE)
    {
        int destination = played->position + die;
        if (board[destination] != NULL)
            board[destination]->position = -1;
        board[destination] = played;
    }
    else if (played->position + die < BOARD_SIZE + FINISH_LINE_SIZE)
    {
        (*player)->finish[played->position + die - BOARD_SIZE] = played;
        
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
