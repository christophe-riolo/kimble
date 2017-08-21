#ifndef KIMBLE_H
#define KIMBLE_H

#include <vector>
#include <array>

#define BOARD_SIZE 28
#define FINISH_LINE_SIZE 4
#define MAX_PLAYERS 4
#define PEGS_NUMBER 4

class Game 
{
	public:
		Game(int nplayers = MAX_PLAYERS);
		void play();
	private:
		std::array<Peg*,BOARD_SIZE> board;
		std::vector<Player*> players;
		std::vector<Player*> winners;

		// Plays a turn of the given player.
		void turn(Player* player);
}

class Player 
{
	public:
		Player(int number);
	private:
		std::array<Peg*,FINISH_LINE_SIZE> finish; 
		std::array<Peg*,PEGS_NUMBER> pegs;
		int number;
}

class Peg
{
	public:
		Player* owner;
		Peg();
}

#endif
