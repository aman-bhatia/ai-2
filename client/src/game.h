#ifndef GAME_H
#define GAME_H


#define Move pair< Coord , Coord >
#define Coord pair< int , int >

#include <iostream>
#include <vector>
#include <utility>		// pair
#include <stdlib.h>     // srand, rand
#include "board.h"
using namespace std;

class Game{
	private:
		Board* board;
		int globalcutoff_order;
		int globalcutoff_chaos;
	public:
		Game(int _board_size);
		~Game();
		vector< Coord > getPossibleOrderMoves(Coord position , Board& b);
		Coord chaosRandomAction();
		Move orderRandomAction();
		Move getOrderActionFromBoard(Board &b1, Board &b2);
		Coord getChaosActionFromBoard(Board &b1, Board &b2);
		Coord chaosAIAction(char color);
		Move orderAIAction();
		pair< Move , double > orderHelper(Board& b , int cutoff , double alpha , double beta);
		pair < Coord , double > chaosHelper(Board& b , int cutoff , char color , double alpha , double beta);
		void makeChaosMove(Coord position, char c);
		void makeOrderMove(pair< Coord, Coord> move);
		void playAsOrder();
		void playAsChaos();
};


#endif