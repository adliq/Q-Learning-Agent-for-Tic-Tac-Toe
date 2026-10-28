#ifndef XO_HPP
#define XO_HPP

#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

/**
 * A `Tic-tac-toe, noughts and crosses, or Xs and Os' class designed for
 * experimentation with machine learning algorithms.  For ease of use
 * and speed, safe guards to prevent corruption of the game state are
 * almost non-existent.
 */
class XO
{
public:
  /**
   * Default constructor
   * @return blank game state (i.e., all spaces unoccupied and no turns taken)
   */
  XO();

  /**
   * Instantiate a game where moves have already been played
   * @param board the desired board state, a combination of unoccupied
   * spaces and player markers
   * @param turns ordered list of which player made
   * @throw std::invalid_argument if specified game state (board and
   * turns) is illegal, board remains uninitialized
   * @return game created that reflects moves specified by user
   */
  XO(const std::array<uint8_t, 9> board, const std::array<std::uint8_t, 9> turns);

  /** 
   * Place player marker at specified board position
   * @param player marker (1 [X] or 2 [O])
   * @param position to place marker at (0--8)
   * @return true if legal move could be made, board unchanged otherwise
   */
  bool move(const std::uint8_t player, const std::uint8_t position);

  /** 
   * Place player marker at random, unoccupied board position (a very
   * crude artificial intelligence agent to play against)
   * @param player marker (1 [X] or 2 [O])
   * @return true if legal move could be made, board unchanged otherwise
   */
  bool move(const std::uint8_t player);

  /**
   * Check whether the game has reached a terminal state
   * @return true if the game has been won or there are zero legal
   * moves available (a draw)
   */
  bool over() const;

  /**
   * Provide the winner of the game
   * @return the player marker of the winner (1, 2) or 0 for no winner
   */
  std::uint8_t winner() const;

  /**
   * Provide the player marker to take the next turn
   * @return the player marker for the next turn (1, 2) or 0 for no turns available/taken
   */
  std::uint8_t turn() const;
  
  /**
   * Provide a list of occupied game spaces
   * @return indices of unoccupied spaces (0--8), in ascending order
   */
  std::vector<std::uint8_t> spaces() const;

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
  friend std::ostream& operator<<(std::ostream& os, const XO & game);
  
  /**
   * The current board state
   * 0 := unoccupied space
   * 1 := space occupied by X
   * 2 := spaced occupied by O
   * @remark Exposed as public to allow fast, in-place updates that *only*
   * re-arrange the existing board configuration; i.e., no new
   * markers should be placed.  This is dangerous and a bad idea for
   * production code but makes our reinforcement learning task
   * slightly easier and quicker.
   */
  std::array<std::uint8_t, 9> brd;
  
private:
  /**
   * List for the order of play; i.e., ordered list of player
   * turns. Useful for determining legal moves, etc. 0 indicates no
   * turn taken.
   */
  std::array<std::uint8_t, 9> trns;
  
  /**
   * Number of moves made in current game
   */
  std::uint8_t N_moves = 0;

  /*
   * Random number generation
   */
  std::random_device rd;  //seed for the random number engine
  std::mt19937 g; //mersenne_twister_engine for random number generation


  bool winningcombos(int a, int b, int c) const;
};
#endif
