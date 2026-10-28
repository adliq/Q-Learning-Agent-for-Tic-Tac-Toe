#ifndef QXO_HPP
#define QXO_HPP

#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <unordered_map>
#include <vector>

#include "Qopt.hpp"
#include "XO.hpp"

/**
 * A model-free reinforcement learning approach, using Q-learning, for
 * `Tic-tac-toe, noughts and crosses, or Xs and Os'.
 */
class QXO
{
public:
  /**
   * Default constructor
   * @return a Q-table initialized with the 765 base states of the
   * game and neutral rewards (0) for possible actions (moves)
   */
  QXO();

  /** 
   * Use the current Q-table to play against the XO class `AI'
   * @param player marker (1 [X] or 2 [O]) for the RL agent
   * @remark play cannot commence if specified player different than
   * trained player
   * @param N number of games to play
   * @remark There are only 26,830 legal games in tic-tac-toe so we limit the
   * number of games we can play accordingly by using std::uint16_t
   * @return a list of the number of wins, losses, and draws on the
   * part of the RL agent encountered during play
   */
  std::array<std::uint16_t, 3> play(const std::uint8_t player, const std::uint16_t N) const;

  /** 
   * Create/update the Q-table
   * @remark the Q-table is updated so long as model and game
   * parameters have not changed, otherwise it must be reinitialized
   * (changing epsilon for mini-batches would not constitute a change
   * in model parameters so long as epsilon is reset after training)
   * @param player marker (1 [X] or 2 [O]) for the RL agent
   * @param N number of games to play
   * @param N_batch number of games to play per mini-batch
   * @return a list of the number of wins, losses, and draws on the
   * part of the RL agent encountered during play
   */
  std::array<std::uint16_t, 3> train(const std::uint8_t player, const std::uint16_t N, const std::uint16_t N_batch);

  /**
   * Set the model parameters of the reinforcement learning agent
   * @param options values for alpha, epsilon, and gamma
   * @remark to ensure consistency in training, updating model
   * parameters should reset the Q-table
   * @return true if the provided model parameters were valid and set
   */
  bool model(const Qopt & options);
  
private:
  /*
   * C++11 lacks a hash function for std::array. The following
   * function assigns a unique number to each possible game board. 
   */
  struct StdArrayHash
  {
    std::size_t operator()(const std::array<std::uint8_t, 9> & a) const
    {
      //hash = sum_{i=0}^{8}(brd(i) * 3^i) from 
      std::size_t hash = (a[0]) + (a[1] * 3) + (a[2] * 9) + (a[3] * 27) + (a[4] * 81) + (a[5] * 243) + (a[6] * 729) + (a[7] * 2187) + (a[8] * 6561);
      
      return hash;
    }
  };
    
  /**
   * A `table' to store the state-action pairs used by the RL agent to
   * determine the optimal move (action) for a given game board (state)
   */
  std::unordered_map<std::array<std::uint8_t, 9>, std::array<float, 9>, StdArrayHash> Q_table;
  //the player marker (1 [X] or 2 [O]) of the RL agent
  std::uint8_t Q_mrk;
  //the player marker (1 [X] or 2 [O]) of the opponent/adversary (use
  //A instead of O because it's easier to distinguish between A/Q than
  //O/Q)
  std::uint8_t A_mrk;
  
  /*
   * Reinforcement learning agent model parameters, with appropriate defaults
   */
  //learning rate
  float alpha = 0.1;
  //the proportion of actions in training that the RL agent randomly selects
  float epsilon = 0.9;
  //discount factor
  float gamma = 1;

  /*
   * Random number generation
   */
  std::random_device rd;  //seed for the random number engine
  std::mt19937 g; //mersenne_twister_engine for random number generation
  std::uniform_real_distribution<float> dis; //generate random reals on interval [a,b)


  /**
   * Update the Q-table based on the play and outcome of a single game
   * @param an instance of a tic-tac-toe game
   * @return the outcome of the game (0 := draw, 1 := player 1 [X]
   * win, 2 := player 2 [O] win)
   */
  uint8_t update(XO & game);
  


  //perform a 90 degree, clockwise rotation on the game board
  std::array<std::uint8_t, 9> rotate(const std::array<std::uint8_t, 9>& board, uint8_t& move) const;
  //perform a reflection about the horizontal axis of the game board
  std::array<std::uint8_t, 9> reflect(const std::array<std::uint8_t, 9> & board,  std::uint8_t& move) const;
  //find the index of the action with the maximum value
  std::uint8_t argmax(const std::array<float, 9> & actions) const;
  

  std::array<std::uint8_t, 9> reflect1(const std::array<std::uint8_t, 9>& board) const;
  std::array<std::uint8_t, 9> rotate1(const std::array<std::uint8_t, 9>& board) const;


  std::vector<std::array < std::uint8_t, 9>> nextboard;

  std::uint8_t argmax2(const std::array<float, 9>& actions, const std::vector<std::uint8_t>& valid_moves) const;

 
};
#endif
