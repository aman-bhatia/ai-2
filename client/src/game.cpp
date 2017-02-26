#include "game.h"
#include <math.h>
/*
 * Constructor
 */
Game::Game(int _board_size){
	board = new Board(_board_size);
	globalcutoff_order = 3;
	globalcutoff_chaos = 3;
}

Game::~Game()
{
	board->~Board();
}
/*
 * Returns possible moves from a given position
 */
vector< Coord > Game::getPossibleOrderMoves(Coord position , Board& b ){
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
			possibleMoves.push_back( make_pair(x,j));
		else
			break;
	}
	return possibleMoves;
}

void Game::makeChaosMove(Coord position, char c){
	board->setColor(position.first, position.second, c);
}

void Game::makeOrderMove(pair< Coord, Coord> move){
	board->moveColor(move.first.first, move.first.second, move.second.first, move.second.second);
}


pair< Move , double > Game::orderHelper(Board& b , int cutoff, double alpha , double beta)
{
	//The maximum possible score for any board is 150
	if(cutoff == 0 or b.isGameOver())
		return (make_pair( (  make_pair( make_pair(0,0) , make_pair(0,0) )  ) , b.evalFunction()));
	vector<Board> possible_boards;
	vector<Move> moves_used;
	char colors[] = {'A' , 'B' , 'C' , 'D' , 'E'};
	vector<int> colorsused(5,0);
	for(int i = 0 ; i < b.getBoardSize() ; ++i)
	{
		for(int j = 0 ; j < b.getBoardSize() ; ++j)
		{
			char color = b.getColor(i,j);

			if(color != '-')
			{
				++colorsused[color - 'A'];
				vector< Coord > possible_moves = getPossibleOrderMoves(make_pair(i , j) , b);
				while(!possible_moves.empty())
				{
					Coord final_pos = possible_moves.back();
					b.moveColor(i,j,final_pos.first,final_pos.second);
					possible_boards.push_back(b);
					b.moveColor(final_pos.first,final_pos.second,i,j);
					moves_used.push_back(make_pair(make_pair(i,j) , possible_moves.back()));
					possible_moves.pop_back();
				}
			}
		}
	}
	int totalcolorsremaining = 0;
	for(int i = 0 ; i < 5 ; ++i)
		totalcolorsremaining += (5 - colorsused[i]);
	double current_max = -1000000;
	int location = 0;
	for(int i = 0 ; i < possible_boards.size() ; ++i)
	{
		double expectedvalue = 0;
		//int over_colors = 0;
		for(int c = 0 ; c < 5 ; ++c)
		{
			if(colorsused[c] != 5)
			{
				pair< Coord , double > temp = chaosHelper(possible_boards.at(i) , cutoff - 1 , colors[c] , alpha , beta);
				double probintoscore = 0;
				if (temp.second != -1.0)
					probintoscore = (temp.second * (double(5 - colorsused[c]) / totalcolorsremaining));
				//over_colors += 5 - colorsused[c];
				expectedvalue += probintoscore;
				//if((expectedvalue + (150 * (totalcolorsremaining - over_colors) / totalcolorsremaining)) <= alpha)
				//	break;
			}
		}
		alpha = max(alpha , expectedvalue);
		if(alpha >= beta)
			return (make_pair( moves_used.at(i), -1));
		if(current_max < expectedvalue)
		{
			current_max = expectedvalue;
			location = i;
		}
	}
	
	Move best_move = moves_used.at(location);
	if(current_max > b.evalFunction())
		return (make_pair(best_move , current_max));
	else
		return (make_pair(make_pair(best_move.first , best_move.first) , b.evalFunction()));
}



pair < Coord , double > Game::chaosHelper(Board& b , int cutoff , char col , double alpha , double  beta)
{
	if(cutoff == 0 or b.isGameOver())
		return make_pair(make_pair(0,0) , b.evalFunction1());
	double minscore = 10000000;
	int mini = -1 , minj = -1;
	for(int i = 0 ; i < b.getBoardSize() ; ++i)
	{
		for(int j = 0 ; j < b.getBoardSize() ; ++j)
		{
			if(b.getColor(i,j) == '-')
			{
				b.setColor(i,j,col);
				pair< Move , double > temp = orderHelper(b , cutoff - 1 , alpha , beta);
				if(temp.second != -1.0)
				{
					beta = min(beta , temp.second);
					if(alpha >= beta)
						return (make_pair(make_pair(i,j) , -1));
					if(temp.second < minscore)
					{
						minscore = temp.second;
						mini = i;
						minj = j;
					}
				}
				b.setColor(i,j,'-');
			}
		}
	}
	Coord toret = make_pair(mini , minj);
	return (make_pair( toret , minscore));
}


Move Game::orderAIAction()
{
	  pair < Move , double > result = orderHelper(*board , this->globalcutoff_order , -(pow(2 , 40)) , (pow(2 , 40)));
	  return result.first;
}

Coord Game::chaosAIAction(char color)
{
	pair < Coord , double > result;
	if (board->getNumCellsFree() < 10)
		result = chaosHelper(*board , this->globalcutoff_chaos + 1, color , -(pow(2 , 40)) , (pow(2 , 40)));
	else
		result = chaosHelper(*board , this->globalcutoff_chaos, color , -(pow(2 , 40)) , (pow(2 , 40)));
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
		Coord move = chaosAIAction(color);
		makeChaosMove(move, color);
		cout << move.first << " " << move.second << endl;
	}
}