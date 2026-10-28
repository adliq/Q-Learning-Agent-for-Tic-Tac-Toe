#include <iostream>
#include <limits>

/**
 * A simple struct for Q-learning model parameters
 */
struct Qopt
{
  float alpha; //learning rate
  float epsilon; //the proportion of actions in training that the RL agent randomly selects
  float gamma; //discount factor
};
