#ifndef GAME_H
#define GAME_H


#define Coord pair<int,int>

#include <iostream>
#include <vector>
#include <utility>		// pair
#include <stdlib.h>     // srand, rand
#include "board.h"
using namespace std;

class Game{
	private:
		Board* board;
	public:
		Game(int _board_size);
		bool isGameOver();
		vector< Coord > getPossibleOrderMoves(Coord position);
		Coord chaosRandom();
		pair< Coord, Coord > orderRandom();
		bool makeChaosMove(Coord position, char c);
		bool makeOrderMove(pair< Coord, Coord> move);
		void playAsOrder();
		void playAsChaos();
};


#endif