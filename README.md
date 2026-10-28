# Tic-Tac-Toe Q-Learning Agent

A C++ reinforcement learning project that trains an agent to play Tic-Tac-Toe against a random opponent using tabular Q-learning.

The project combines a game engine, configurable learning parameters, board symmetry transformations, and automated tests.

## Features

- Tic-Tac-Toe engine with move validation, turn tracking, and win detection.
- Q-table containing board states and action values.
- Epsilon-greedy exploration during training.
- Batch training with decreasing exploration.
- Rotation and reflection helpers for sharing learned action values.
- Evaluation that reports wins, losses, and draws.
- Automated tests using Catch2 and CTest.

## Technologies

- C++11
- CMake
- Catch2 2.13.10
- C++ Standard Library

No GPU or external machine learning framework is required.

## Project Structure

| File | Purpose |
| --- | --- |
| `XO.hpp`, `XO.cpp` | Game engine and board operations |
| `QXO.hpp`, `QXO.cpp` | Learning agent, training, and evaluation |
| `Qopt.hpp` | Learning parameter configuration |
| `student_tests.cpp` | Automated tests |
| `student_catch.cpp` | Test runner entry point |
| `catch.hpp` | Bundled Catch2 library |
| `CMakeLists.txt` | Build configuration and test registration |

## Build and Test

### Requirements

- A compiler supporting C++11, such as GCC, Clang, or MSVC
- CMake 3.5 or newer
- A compatible build tool

Catch2 is included in the repository.

From the project directory:

```sh
mkdir build
cd build
cmake ..
cmake --build . --config Release
ctest -C Release --output-on-failure
```

If the `build` directory already exists, skip the first command.

On Windows with MSVC, use a Visual Studio developer terminal so CMake can locate the compiler.

The supplied executable is `student-tests`. The project currently exposes a C++ API rather than an interactive game interface.

## Example Usage

Create an `example.cpp` file in the project directory:

```cpp
#include "QXO.hpp"
#include <iostream>

int main() {
    QXO agent;

    // Train as X for 14,000 games in batches of 700.
    const auto training = agent.train(1, 14000, 700);

    // Evaluate against a random opponent.
    const auto results = agent.play(1, 1000);

    std::cout << "Training games: "
              << training[0] + training[1] + training[2]
              << '\n';

    std::cout << "Wins: " << results[0] << '\n';
    std::cout << "Losses: " << results[1] << '\n';
    std::cout << "Draws: " << results[2] << '\n';

    return 0;
}
```

Compile with GCC:

```sh
g++ -std=c++11 example.cpp XO.cpp QXO.cpp -o xo-demo
```

Run on Linux or macOS:

```sh
./xo-demo
```

Run in Windows PowerShell:

```powershell
.\xo-demo.exe
```

Do not include `student_catch.cpp` when compiling this example because it defines the test runner’s separate entry point.

## Learning Parameters

| Parameter | Default | Description |
| --- | --- | --- |
| `alpha` | `0.1` | Learning rate |
| `epsilon` | `0.9` | Initial probability of choosing a random training action |
| `gamma` | `1.0` | Discount factor used in value updates |

Configure parameters before training:

```cpp
QXO agent;

Qopt options{0.1f, 0.9f, 0.95f};

if (agent.model(options)) {
    agent.train(1, 14000, 700);
}
```

All parameters must be between `0` and `1`, inclusive.

Changing parameters through `model()` clears the Q-table, so the agent must be trained again afterward.

During a training call, exploration decreases across batches. The configured epsilon value is restored when that call finishes.

## API

### Train

```cpp
agent.train(player, numberOfGames, batchSize);
```

- `1` represents X.
- `2` represents O.
- Use positive game counts and batch sizes.
- The number of games must be evenly divisible by the batch size.
- Never pass a batch size of zero.

### Evaluate

```cpp
agent.play(player, numberOfGames);
```

Evaluation runs games without updating the learned Q-table.

Train before evaluating and use the same player marker for both. Use separate agent instances when experimenting with different markers.

The current implementation returns zero counts if the evaluation player does not match the training player or if the requested evaluation count exceeds 26,380 games.

Both methods return:

```text
{ wins, losses, draws }
```

Game-count arguments use `std::uint16_t`.

## Board Representation

The board contains nine cells:

- `0`: empty
- `1`: X
- `2`: O

Action indices follow this layout:

```text
0 | 1 | 2
---------
3 | 4 | 5
---------
6 | 7 | 8
```

Each Q-table entry associates a board with nine action values.

## Learning Approach

During training, the agent chooses between a random move and an action selected from its current Q-values. The opponent selects random available moves.

The implementation plays out each game recursively and updates values using the outcome:

| Outcome | Reward |
| --- | --- |
| Agent wins | +1 |
| Agent loses | -1 |
| Draw | 0 |

Rotation and reflection helpers propagate learned values to transformed board-action pairs.

During evaluation, the agent uses its learned table and falls back to random moves for unseen states.

Training and evaluation are randomized, so results vary between runs. Performance against a random opponent does not establish performance against an optimal opponent.

## Tests

The test suite covers:

- Board construction and validation
- Legal and illegal moves
- Turn order and available spaces
- Win detection and completed games
- Text-based board output
- Learning parameter validation
- Training input validation
- Training and evaluation result counts

Some tests involve randomized gameplay.

## Potential Improvements

- Save and load trained Q-tables
- Add reproducible random seeds
- Create a human-versus-agent interface
- Compare performance against a minimax opponent
- Plot learning progress across training batches
- Add systematic experiments for learning parameters

## Acknowledgments

The source credits [Saman Khamesian’s Tic-Tac-Toe base states](https://github.com/SamanKhamesian/Tic-Tac-Toe/blob/master/Source/base_matrix.txt) for the initial board-state table.

The bundled Catch2 header contains its copyright and license information.