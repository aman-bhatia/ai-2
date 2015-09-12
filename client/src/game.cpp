#include "game.h"
#include <math.h>
/*
 * Constructor
 */
Game::Game(int _board_size){
	board = new Board(_board_size);
	globalcutoff = 3;
}


/*
 * Returns possible moves from a given position
 */
vector< Coord > Game::getPossibleOrderMoves(Coord position , Board b ){
	vector< Coord > possibleMoves(0);

	int x = position.first;
	int y = position.second;

	for (int i=x-1; i>=0; i--){
		if (b.getColor(i,y) == '-')
			possibleMoves.push_back(make_pair(i,y));
		else
			break;
	}


	for (int i=x+1; i<b.getBoardSize(); i++){
		if (b.getColor(i,y) == '-')
			possibleMoves.push_back(make_pair(i,y));
		else
			break;
	}

	for (int j=y-1; j>=0; j--){
		if (b.getColor(x,j) == '-')
			possibleMoves.push_back(make_pair(x,j));
		else
			break;
	}

	for (int j=y+1; j<b.getBoardSize(); j++){
		if (b.getColor(x,j) == '-')
			possibleMoves.push_back(make_pair(x,j));
		else
			break;
	}
	return possibleMoves;
}



/*
 * Returns random position for chaos to put color
 */
Coord Game::chaosRandomAction(){
	
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


/*
 * Returns random move for order to play
 */
Move Game::orderRandomAction(){
	
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
		vector< Coord > possible_moves = getPossibleOrderMoves(from_position , *board);
		if (possible_moves.size() != 0){
			Coord to_position = possible_moves[(rand() % possible_moves.size())];
			return make_pair(from_position, to_position);
		}
	}
}


void Game::makeChaosMove(Coord position, char c){
	board->setColor(position.first, position.second, c);
}

void Game::makeOrderMove(pair< Coord, Coord> move){
	board->moveColor(move.first.first, move.first.second, move.second.first, move.second.second);
}


pair< Move , double > Game::orderHelper(Board b , int cutoff)
{
	if(cutoff == 0 or b.isGameOver())
		return (make_pair( (  make_pair( make_pair(0,0) , make_pair(0,0) )  ) , b.calculateScore()));
	vector<Board> possible_boards;
	char colors[] = {'A' , 'B' , 'C' , 'D' , 'E'};
	vector<int> colorsused(5,0);
	for(int i = 0 ; i < b.getBoardSize() ; ++i)
	{
		for(int j = 0 ; j < b.getBoardSize() ; ++j)
		{
			char color = b.getColor(i,j);
			for(int c = 0 ; c < 5 ; ++c)
			{
				if(color == colors[c])
				{
					colorsused[c]++;
					break;
				}
			}

			if(color != '-')
			{
				vector< Coord > possible_moves = getPossibleOrderMoves(make_pair(i , j) , b);
				while(!possible_moves.empty())
				{
					Coord final_pos = possible_moves.back();
					Board temp(b);
					temp.moveColor(i,j,final_pos.first,final_pos.second);
					possible_boards.push_back(temp);
					possible_moves.pop_back();
				}
			}
		}
	}
	
	vector<double> expectedscores;
	int totalcolorsremaining = 0;
	for(int i = 0 ; i < 5 ; ++i)
		totalcolorsremaining += (5 - colorsused[i]);
	for(int i = 0 ; i < possible_boards.size() ; ++i)
	{
		double expectedvalue = 0;
		for(int c = 0 ; c < 5 ; ++c)
		{
			if(colorsused[c] != 5)
			{
				pair< Coord , double > temp = chaosHelper(possible_boards.at(i) , cutoff - 1 , colors[c]);
				double probintoscore = temp.second * (double(5 - colorsused[c]) / totalcolorsremaining);
				expectedvalue += probintoscore;
			}
		}
		expectedscores.push_back(expectedvalue);
	}
	double current_max = 0;
	int location = 0;
	for(int i = 0 ; i < expectedscores.size() ; ++i)
	{
		if(current_max <= expectedscores.at(i))
		{
			current_max = expectedscores.at(i);
			location = i;
		}
	}
	Move best_move = getOrderActionFromBoard(b , possible_boards.at(location));
	return (make_pair(best_move , current_max));
}



pair < Coord , double > Game::chaosHelper(Board b , int cutoff , char col)
{
	if(cutoff == 0 or b.isGameOver())
		return make_pair(make_pair(0,0) , b.calculateScore());
	double minscore = 10000000;		 		// b.calculateScore() no need of this put in the evaluation function here instead of 2^31
	int mini = -1 , minj = -1;
	for(int i = 0 ; i < b.getBoardSize() ; ++i)
	{
		for(int j = 0 ; j < b.getBoardSize() ; ++j)
		{
			if(b.getColor(i,j) == '-')
			{
				b.setColor(i,j,col);
				pair< Move , double > temp = orderHelper(b , cutoff - 1);
				if(temp.second < minscore)
				{
					minscore = temp.second;
					mini = i;
					minj = j;
				}
				b.setColor(i,j,'-');
			}
		}
	}
	Coord toret = make_pair(mini , minj);
	return (make_pair( toret , minscore));
}



// b1 is the initial board
// b2 is the resulting board
Move Game::getOrderActionFromBoard(Board &b1, Board &b2){
	int count = 0;
	Coord c1,c2;
	for (int i=0; i<b1.getBoardSize(); i++){
		for (int j=0; j<b1.getBoardSize(); j++){
			if (b1.getColor(i,j) == b2.getColor(i,j))
				continue;
			else {
				if (count == 0)
					c1 = make_pair(i,j);
				else if (count == 1)
					c2 = make_pair(i,j);
				count++;
			}
		}
	}

	if (count != 2){
		cerr << "Error in getOrderActionFromBoard : More than two no. of cells differ" << endl;
		
		cerr << "b1 is : " << endl;
		b1.printBoard();
		cerr << endl;
		cerr << "b2 is : " << endl;
		b2.printBoard();

		exit(0);
	}

	if ((b1.getColor(c1.first, c1.second) != '-') && \
		(b2.getColor(c1.first, c1.second) == '-') && \
		(b1.getColor(c1.first, c1.second) == b2.getColor(c2.first, c2.second)))
		return (make_pair(c1 , c2));
	else if ((b1.getColor(c2.first, c2.second) != '-') && \
		(b2.getColor(c2.first, c2.second) == '-') && \
		(b1.getColor(c2.first, c2.second) == b2.getColor(c1.first, c1.second)))
		return (make_pair(c2 , c1));
	else {
		cerr << "Something unknown just happened!!!" << endl;
		exit(0);
	}
}


// b1 is the initial board
// b2 is the resulting board
Coord Game::getChaosActionFromBoard(Board &b1, Board &b2){
	int count = 0;
	Coord c1,c2;
	for (int i=0; i<b1.getBoardSize(); i++){
		for (int j=0; j<b1.getBoardSize(); j++){
			if (b1.getColor(i,j) == b2.getColor(i,j))
				continue;
			else {
				if (count == 0)
					c1 = make_pair(i,j);
				count++;
			}
		}
	}

	if (count != 1){
		cerr << "Error in getChaosActionFromBoard : More than one no. of cells differ" << endl;
		
		cerr << "Board b1 is : " << endl;
		b1.printBoard();
		
		cerr << "Board b2 is : " << endl;
		b2.printBoard();
		
		exit(0);
	}

	if ((b1.getColor(c1.first, c1.second) == '-') && 
		(b2.getColor(c1.first, c1.second) != '-'))
		return c1;
	else {
		cerr << "Error in getChaosActionFromBoard : Something unknown just happened!!!" << endl;
		exit(0);
	}
}



Move Game::orderAIAction()
{
	  pair < Move , double > result = orderHelper(*board , this->globalcutoff);
	  return result.first;
}

Coord Game::chaosAIAction(char &color)
{
	pair < Coord , double > result = chaosHelper(*board , this->globalcutoff, color);
	return result.first;
}

void Game::playAsOrder(){
	cerr << "ORDER" << endl;
	int x,y;
	char c;
	while(true){
		cin >> x >> y >> c;
		makeChaosMove(make_pair(x,y),c);
		if (board->isGameOver())
			return;
		pair< Coord, Coord > move = orderAIAction();
		makeOrderMove(move);
		cout << move.first.first << " " << move.first.second << " " << move.second.first << " " << move.second.second << endl;
	}
}

void Game::playAsChaos(){
	cerr << "CHAOS" << endl;
	char color;
	cin >> color;

	Coord move = chaosAIAction(color);
	makeChaosMove(move, color);
	cout << move.first << " " << move.second << endl;

	while (true){
		if (board->isGameOver())
			return;

		int a,b,c,d;
		cin >> a >> b >> c >> d;
		makeOrderMove( make_pair( make_pair(a,b), make_pair(c,d)) );

		cin >> color;

/*
		if (color == '1'){
			board->printBoard();
			cin >> color;
		}

		if (color == '2'){
			cerr << "Score is : " << this->board->calculateScore() << endl;
			cin >> color;
		}
*/

		Coord move = chaosAIAction(color);
		makeChaosMove(move, color);
		cout << move.first << " " << move.second << endl;
	}
}