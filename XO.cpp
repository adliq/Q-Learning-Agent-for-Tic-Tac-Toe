#include <algorithm>
#include <cstdlib>

#include "XO.hpp"



XO::XO() {
	brd = { 0,0,0,0,0,0,0,0,0 };
	trns = { 0,0,0,0,0,0,0,0,0 };

	N_moves = 0;

	
}

XO::XO(const std::array<uint8_t, 9> board, const std::array<std::uint8_t, 9> turns) {
	
	// Invalid player or turns values
	for (std::size_t i = 0; i < board.size(); i++) {
		if (board[i] > 2 || board[i] < 0 || turns[i] > 2 || turns[i] < 0) {
			throw std::invalid_argument("invalid value found");
		}
	}

	int count = 0;
	
	// check for consecutive moves
	for (std::size_t i = 0; i < board.size(); i++) {
		if (i != 0 && turns[i] == turns[i - 1] && turns[i] != 0) {
			throw std::invalid_argument("illegal move");
		}


	}
	// Count all plays made on board
	for (std::size_t i = 0; i < board.size(); i++) {
		if (board[i] != 0) {
			count++;
		}
	
	}
	// count the turns
	for (std::size_t i = 0; i < turns.size(); i++) {
		if (turns[i] != 0) {
			N_moves++;
		}
	}
	// Check if number moves corresponds with board state
	if (count != N_moves) {
		throw std::invalid_argument("invalid number of moves");
	}
	


	// Check if number of moves is greater than 9 
	if (N_moves > 9) {
		throw std::invalid_argument("invalid number of moves");
	}

	int countx = 0;
	int counto = 0;

	
	// Count all Xs and Os and make sure no illegal moves were made
	for (std::size_t i = 0; i < board.size(); i++) {
		if (board[i] == 1) {
			countx++;
		}
		else if (board[i] == 2) {
			counto++;
		}
	}

	if (!(std::abs(countx - counto) <= 1)) {
		throw std::invalid_argument("illegal move made");
	}

	if (turns[0] == 1 && (countx < counto)) {
		throw std::invalid_argument("illegal move made");
	}
	if (turns[0] == 2 && (countx > counto)) {
		throw std::invalid_argument("illegal move made");
	}









	
	brd = board;
	trns = turns;
	



}


 /** 
   * Place player marker at specified board position
   * @param player marker (1 [X] or 2 [O])
   * @param position to place marker at (0--8)
   * @return true if legal move could be made, board unchanged otherwise
   */
bool XO::move(const std::uint8_t player, const std::uint8_t position) {

	if (position < 0 || position > 8 || player > 2 || player < 1 ) {
		return false;
	}
	if (brd[position] == 0) {
		 
		if (N_moves == 0 && trns[0] == 0 && !spaces().empty()) { // if this is first move of game
			brd[position] = player;

			trns[N_moves] = player;
			N_moves++;

			return true;
		}
		else if (turn() == player) { // if its player turn 
			brd[position] = player;

			trns[N_moves] = player;
			N_moves++;

			return true;
		}


	}
	
		return false;

}


/**
 * Place player marker at random, unoccupied board position (a very
 * crude artificial intelligence agent to play against)
 * @param player marker (1 [X] or 2 [O])
 * @return true if legal move could be made, board unchanged otherwise
 */
bool XO::move(const std::uint8_t player) {
	//|| over()
	if ( player > 2 || player < 1 || spaces().empty() || over()) {
		return false;
	}

	

	std::vector<std::uint8_t> empty = spaces();



	std::uniform_int_distribution<std::size_t> distr(0, empty.size() - 1);
	std::uint8_t randoms = empty[distr(g)];

	return move(player, randoms);


}

/**
 * Provide a list of occupied game spaces
 * @return indices of unoccupied spaces (0--8), in ascending order
 */
std::vector<std::uint8_t> XO::spaces() const {

	std::vector<std::uint8_t> spaces;

	for (std::uint8_t i = 0; i < brd.size(); i++) {
		
		if (brd[i] == 0) {
			spaces.push_back(i);
		}


	}

	return spaces;
}

/**
 * Check whether the game has reached a terminal state
 * @return true if the game has been won or there are zero legal
 * moves available (a draw)
 */
bool XO::over() const {


	//check for win or draw using helper function
	
	//check rows

	if (winningcombos(0, 1, 2) ||
		winningcombos(3, 4, 5) ||
		winningcombos(6, 7, 8) ||
		winningcombos(0, 3, 6) ||
		winningcombos(1, 4, 7) ||
		winningcombos(2, 5, 8) ||
		winningcombos(0, 4, 8) ||
		winningcombos(2, 4, 6)
		) {
		return true;
	}
	//check for draw
	else if ((spaces().empty())) {
		return true;
	}
	return false;




}

bool XO::winningcombos(int a, int b, int c) const {

	return (brd[a] != 0 && brd[a] == brd[b] && brd[b] == brd[c] );

}

/**
   * Provide the player marker to take the next turn
   * @return the player marker for the next turn (1, 2) or 0 for no turns available/taken
   */
std::uint8_t XO::turn() const {

	if (spaces().empty()) {// if full board
		return 0;
	}

	int countx = 0;
	int counto = 0;






	//}
	for (std::size_t i = 0; i < trns.size(); i++) { // Count X's and O's
		if (trns[i] == 1) {
			countx++;
		}
		if (trns[i] == 2) {
			counto++;
		}
	}
	// Check if illegal moves were made
	if (trns[0] == 1 && (countx == counto)) {
		return 1;

	}
	else if (trns[0] == 2 && countx == counto) {
		return 2;
	}
	if (countx > counto && !over()) {
		return 2;
	}
	else if (counto > countx && !over()) {
		return 1;
	}


	return 0;




	
}


/**
 * Outputs the current game state as a 3-by-3 grid
 * e.g., board = {012210201} would appear as
 * _ X O
 * O X _
 * O _ X
 * @remark First row should *not* start with a new line (std::endl)
 * @remark Last row should end with a new line (std::endl)
 * @param os - the ostream object to use to output.
 * @param game - the XO game we want to `display' or `print'
 * @return - Game board in two-dimensional form
 */


std::ostream& operator<<(std::ostream& os, const XO& game) {
	for (int i = 0; i < 9; i++) {
		if (i % 3 == 0 && i != 0) {
			os << "\n";
		}
		if (game.brd[i] == 0) {
			os << "_";
		}
		else if (game.brd[i] == 1) {
			os << "X";
		}
		else if (game.brd[i] == 2) {
			os << "O";
		}

	}
	os << "\n";
	return os;
}

/**
 * Provide the winner of the game
 * @return the player marker of the winner (1, 2) or 0 for no winner
 */
std::uint8_t XO::winner() const {

	if (winningcombos(0, 1, 2)) return brd[0]; // Row 1
	if (winningcombos(3, 4, 5)) return brd[3]; // Row 2
	if (winningcombos(6, 7, 8)) return brd[6]; // Row 3

	if (winningcombos(0, 3, 6)) return brd[0]; // Column 1
	if (winningcombos(1, 4, 7)) return brd[1]; // Column 2
	if (winningcombos(2, 5, 8)) return brd[2]; // Column 3


	if (winningcombos(0, 4, 8)) return brd[0]; // Diagonal 1
	if (winningcombos(2, 4, 6)) return brd[2]; // Diagonal 2

	// No winner
	return 0;
}