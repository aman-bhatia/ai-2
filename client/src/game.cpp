#include "game.h"

/*
 * Constructor
 */
Game::Game(int _board_size){
	board = new Board(_board_size);
}


/*
 * returns true if the game is over else false
 */
bool Game::isGameOver(){
	for (int i=0; i<board->getBoardSize(); i++){
		for (int j=0; j<board->getBoardSize(); j++){
			if (board->getColor(i,j) == '-')
				return false;
		}
	}
	return true;
}



vector< Coord > Game::getPossibleOrderMoves(Coord position){
	vector< Coord > possibleMoves(0);

	int x = position.first;
	int y = position.second;

	for (int i=x-1; i>=0; i--){
		if (board->getColor(i,y) == '-')
			possibleMoves.push_back(make_pair(i,y));
		else
			break;
	}


	for (int i=x+1; i<board->getBoardSize(); i++){
		if (board->getColor(i,y) == '-')
			possibleMoves.push_back(make_pair(i,y));
		else
			break;
	}

	for (int j=y-1; j>=0; j--){
		if (board->getColor(x,j) == '-')
			possibleMoves.push_back(make_pair(x,j));
		else
			break;
	}

	for (int j=y+1; j<board->getBoardSize(); j++){
		if (board->getColor(x,j) == '-')
			possibleMoves.push_back(make_pair(x,j));
		else
			break;
	}

	return possibleMoves;
}




Coord Game::chaosRandom(){
	
	vector< Coord > open_cells(0);

	// store all open cells
	for (int i=0; i<board->getBoardSize(); i++){
		for (int j=0; j<board->getBoardSize(); j++){
			if (board->getColor(i,j) == '-')
				open_cells.push_back(make_pair(i,j));
		}
	}
	
	// return position of a open random cell
	return open_cells[(rand() % open_cells.size())];
}


pair< Coord, Coord > Game::orderRandom(){
	
	vector< Coord > captured_cells(0);
	
	// store all captures cells
	for (int i=0; i<board->getBoardSize(); i++){
		for (int j=0; j<board->getBoardSize(); j++){
			if (board->getColor(i,j) != '-')
				captured_cells.push_back(make_pair(i,j));
		}
	}

	while (true){
		// pick any random captured cell
		pair<int, int> from_position = captured_cells[(rand() % captured_cells.size())];
		
		// check if picked cell has any order-moves or not
		vector< Coord > possible_moves = getPossibleOrderMoves(from_position);
		if (possible_moves.size() != 0){
			Coord to_position = possible_moves[(rand() % possible_moves.size())];
			return make_pair(from_position, to_position);
		}
	}
}


bool Game::makeChaosMove(Coord position, char c){
	return board->setColor(position.first, position.second, c);
}

bool Game::makeOrderMove(pair< Coord, Coord> move){
	return board->moveColor(move.first.first, move.first.second, move.second.first, move.second.second);
}


void Game::playAsOrder(){
	cerr << "ORDER" << endl;
	int x,y;
	char c;
	while(true){
		//board->printBoard();
		cin >> x >> y >> c;
		if (! makeChaosMove(make_pair(x,y),c)){
			cerr << "Error in chaos move (" << x << "," << y << "," << c << ")" << endl;
			exit(0);
		};
		if (isGameOver())
			return;
		pair< Coord, Coord > move = orderRandom();
		if (! makeOrderMove(move)){
			cerr << "Error in order move (" << move.first.first << "," << move.first.second << "," << move.second.first << "," << move.second.second << ")" << endl;
			exit(0);
		};
		//board->printBoard();
		cout << move.first.first << " " << move.first.second << " " << move.second.first << " " << move.second.second << endl;
	}
}

void Game::playAsChaos(){
	cerr << "CHAOS" << endl;
	char color;
	cin >> color;
	Coord move = chaosRandom();
	if (! makeChaosMove(move, color)){
		cerr << "Error in chaos move (" << move.first << "," << move.second << "," << color << ")" << endl;
		exit(0);
	};
	cout << move.first << " " << move.second << endl;
	//board->printBoard();

	while (true){
		if (isGameOver())
			return;

		int a,b,c,d;
		cin >> a >> b >> c >> d;
		if (! makeOrderMove( make_pair( make_pair(a,b), make_pair(c,d)) )){
			cerr << "Error in order move (" << a << "," << b << "," << c << "," << d << ")" << endl;
			exit(0);
		};
		cin >> color;
		Coord move = chaosRandom();
		if (! makeChaosMove(move, color)){
			cerr << "Error in chaos move (" << move.first << "," << move.second << "," << color << ")" << endl;
			exit(0);
		};
		//board->printBoard();
		cout << move.first << " " << move.second << endl;
	}
}