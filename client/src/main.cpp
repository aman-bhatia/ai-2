#include "game.h"
#include <time.h>       /* time */
using namespace std;

int main() {
	srand(time(NULL));
	int board_size;
	cin >> board_size;
	string role;
	cin >> role;
	
	Game game(board_size);
	if (role == "ORDER")
		game.playAsOrder();
	else if (role == "CHAOS")
		game.playAsChaos();
	else
		cerr << "I am not intelligent for this " << role << endl;
	return 0;
}