#include <array>
#include <iostream>
#include <limits>
#include <sstream>
#include "catch.hpp"
#include "QXO.hpp"
#include "XO.hpp"

TEST_CASE( "default constructor", "[XO]" )
{
  XO game;

  REQUIRE(game.brd == std::array<std::uint8_t, 9>({0,0,0,0,0,0,0,0,0}));
  REQUIRE(game.spaces() == std::vector<std::uint8_t>({ 0,1,2,3,4,5,6,7,8 }));
}

TEST_CASE("parameterized constructor", "[XO]") {


    SECTION("Valid 1 - game is not over(x's turn)") {
        const std::array<uint8_t, 9> board = { 1, 2, 0, 1, 0, 2, 0, 0, 0 };
        std::array<std::uint8_t, 9> turns = { 1, 2, 1, 2,0,0,0,0,0 };

        XO tictac(board, turns);

        REQUIRE(tictac.brd == board);
        REQUIRE(tictac.spaces() == std::vector<std::uint8_t>({ 2, 4, 6, 7, 8 }));
        REQUIRE(tictac.turn() == 1);

    }

    SECTION("Valid 2 - game is over") {

        const std::array<uint8_t, 9> board = { 1, 2, 0, 1, 1, 2, 2, 0, 1 };
        std::array<std::uint8_t, 9> turns = { 1,2,1,2,1,2,1,0, 0 };

        XO tictac(board, turns);

        REQUIRE(tictac.brd == board);
        REQUIRE(tictac.spaces() == std::vector<std::uint8_t>({ 2, 7 }));
        REQUIRE(tictac.turn() == 0);

    }

    SECTION("Valid 3 - game is not over(o's turn)") {

        const std::array<uint8_t, 9> board = { 1, 2, 0, 1, 1, 2, 2, 1, 2 };
        std::array<std::uint8_t, 9> turns = { 1, 2, 1, 2, 1, 2, 1,2,0 };

        XO tictac(board, turns);

        REQUIRE(tictac.brd == board);
        REQUIRE(tictac.spaces() == std::vector<std::uint8_t>({ 2 }));
        REQUIRE(tictac.turn() == 1);

    }
}

TEST_CASE("XO Constructor - Invalid Cases") {
    SECTION("Consecutive Turns by Same Player") {
        std::array<uint8_t, 9> board = { 1, 1, 2, 0, 0, 0, 0, 0, 0 };
        std::array<std::uint8_t, 9> turns = { 1, 1, 2, 0, 0, 0, 0, 0, 0 };
        REQUIRE_THROWS_AS(XO(board, turns), std::invalid_argument);
    }



    SECTION("Invalid Turn Alternation") {
        std::array<uint8_t, 9> board = { 1, 2, 1, 1, 0, 0, 0, 0, 0 };
        std::array<std::uint8_t, 9> turns = { 1, 2, 1, 1, 0, 0, 0, 0, 0 };

        REQUIRE_THROWS_AS(XO(board, turns), std::invalid_argument);
    }

    SECTION("Mismatch Between Turn Count and Board State") {

        std::array<uint8_t, 9> board = { 1, 2, 0, 0, 0, 0, 0, 0, 0 };
        std::array<std::uint8_t, 9> turns = { 1, 2, 1, 0, 0, 0, 0, 0, 0 };
        REQUIRE_THROWS_AS(XO(board, turns), std::invalid_argument);
    }

    SECTION("Invalid Values in Board") {
        std::array<uint8_t, 9> board = { 1, 2, 3, 0, 0, 0, 0, 0, 0 };
        std::array<std::uint8_t, 9> turns = { 1, 2, 0, 0, 0, 0, 0, 0, 0 };
        REQUIRE_THROWS_AS(XO(board, turns), std::invalid_argument);
    }
    SECTION("Invalid Values in Turns") {
        std::array<uint8_t, 9> board = { 1, 2, 0, 0, 0, 0, 0, 0, 0 };
        std::array<std::uint8_t, 9> turns = { 1, 2, 3, 0, 0, 0, 0, 0, 0 };


        REQUIRE_THROWS_AS(XO(board, turns), std::invalid_argument);
    }
    SECTION("Invalid Difference in Player Moves") {
        std::array<uint8_t, 9> board = { 1, 2, 1, 1, 0, 0, 0, 0, 0 };
        std::array<std::uint8_t, 9> turns = { 1, 2, 1, 1, 0, 0, 0, 0, 0 };
        REQUIRE_THROWS_AS(XO(board, turns), std::invalid_argument);
    }
}


TEST_CASE("XO move function ",  "[XO]" ) {


    SECTION("Valid game") {
        XO game;


        REQUIRE(game.move(1, 0));
        REQUIRE(game.brd[0] == 1);
        REQUIRE(game.move(2, 4));
        REQUIRE(game.brd[4] == 2);

        REQUIRE_FALSE(game.move(1, 0));
        REQUIRE_FALSE(game.move(2, 9));

        // Random move
        REQUIRE(game.move(1));
    }

    SECTION("invalid move") {

        const std::array<uint8_t, 9> board = { 1, 2, 0, 1, 1, 2, 2, 0, 1 };
        std::array<std::uint8_t, 9> turns = { 1,2,1,2,1,2,1,0, 0 };

        XO game(board, turns);

        REQUIRE_FALSE(game.move(1, 2));

    }
}


TEST_CASE("XO random move function ", "[XO]") {

    SECTION("valid checks - empty board and first move") {

        XO game;

        REQUIRE(game.move(1) == true);


    }

    SECTION("invvalid checks - 2 plays in a row") {

        XO game;

        REQUIRE(game.move(1) == true);
        REQUIRE(game.move(1) == false);

        REQUIRE(game.move(2) == true);
        REQUIRE(game.move(2) == false);


    }

    SECTION("invalid checks - make move after board is full") {
        XO game;
        for (int i = 0; i < 9; ++i) {
            if (!game.over())
                REQUIRE((game.move(1) || game.move(2))); // game is not over its a draw
        }
        REQUIRE(game.move(1) == false); 
        REQUIRE(game.move(2) == false); 
    }

    SECTION("invalid checks - make move after game is over") {
        
        const std::array<uint8_t, 9> board = { 1, 2, 0, 1, 1, 2, 2, 0, 1 }; // game is over x won
        std::array<std::uint8_t, 9> turns = { 1,2,1,2,1,2,1,0, 0 };
        
        XO game(board,turns);
      
        REQUIRE(game.move(1) == false); 
        REQUIRE(game.move(2) == false);


        const std::array<uint8_t, 9> board2 = { 2, 1, 0, 2, 2, 1, 1, 0, 2 }; // game is over o won
        std::array<std::uint8_t, 9> turns2 = { 2,1,2,1,2,1,2,0, 0 };

        XO game2(board2, turns2);

        REQUIRE(game2.move(1) == false);
        REQUIRE(game2.move(2) == false);




    }

    SECTION("Random move on an empty board") {
        XO tictac;
        REQUIRE(tictac.move(1));
        auto spaces = tictac.spaces();
        REQUIRE(spaces.size() == 8); 
    }

    SECTION("Random move on a partially filled board") {
        XO tictac;

        // Pre-fill the board with some moves
        tictac.move(1, 0); 
        tictac.move(2, 1);

        REQUIRE(tictac.move(1)); 
        auto spaces = tictac.spaces();
        REQUIRE(spaces.size() == 6); 
    }

    SECTION("Random move on a nearly full board") {
        XO tictac;

        // Fill most of the board
        tictac.move(1, 0);
        tictac.move(2, 1);
        tictac.move(1, 2);
        tictac.move(2, 3);
        tictac.move(1, 4);
        tictac.move(2, 5);
        tictac.move(1, 7);
        tictac.move(2, 6);

        REQUIRE(tictac.move(1)); 
        REQUIRE(tictac.spaces().size() == 0); 
        REQUIRE(tictac.over()); 
    }

    SECTION("invalid checks - random move on a full board") {
        XO tictac;

        // Fill the board completely
        tictac.move(1, 0);
        tictac.move(2, 1);
        tictac.move(1, 2);
        tictac.move(2, 3);
        tictac.move(1, 4);
        tictac.move(2, 5);
        tictac.move(1, 7);
        tictac.move(2, 6);
        tictac.move(1, 8);

        REQUIRE(!tictac.move(1)); 
    }

    SECTION("invalid checks - random move with invalid player ") {
        XO tictac;

        REQUIRE(!tictac.move(3)); 
        REQUIRE(tictac.spaces().size() == 9);
    }

    SECTION("random move with alternating turns") {
        XO tictac;

        // player X starts
        REQUIRE(tictac.move(1));
        REQUIRE(tictac.move(2)); 
        REQUIRE(tictac.move(1)); 
        REQUIRE(tictac.move(2)); 

        auto spaces = tictac.spaces();
        REQUIRE(spaces.size() == 5); 
    }

    SECTION("Random move does not overwrite existing moves") {
        XO tictac;

        
        REQUIRE(tictac.move(1, 0)); 
        REQUIRE(tictac.move(2, 1)); 
       
        REQUIRE(tictac.move(1)); 
        REQUIRE(tictac.move(2)); 

        REQUIRE(tictac.brd[0] == 1);
        REQUIRE(tictac.brd[1] == 2);
    }

    SECTION("valid checks -random move respects turn order") {
        XO tictac;

       
        REQUIRE(tictac.move(1));
        REQUIRE(!tictac.move(1));
        REQUIRE(tictac.move(2)); 

       
        REQUIRE(tictac.turn() == 1); 
    }

    SECTION("valid checks - random move on a board with only one empty space") {
        XO tictac;

        // fill most of the board
        tictac.move(1, 0);
        tictac.move(2, 1);
        tictac.move(1, 2);
        tictac.move(2, 3);
        tictac.move(1, 4);
        tictac.move(2, 5);
        tictac.move(1, 7);
        tictac.move(2, 6);

        // One space left
        REQUIRE(tictac.spaces().size() == 1);

       
        REQUIRE(tictac.move(1)); 
        REQUIRE(tictac.spaces().empty()); 
    }

   


    SECTION("valid -  consecutive random moves") {
        XO tictac;

        // make several random moves
        REQUIRE(tictac.move(1));
        REQUIRE(tictac.move(2));
        REQUIRE(tictac.move(1)); 
        REQUIRE(tictac.move(2)); 
        REQUIRE(tictac.move(1)); 
        
        REQUIRE(tictac.spaces().size() == 4); 
    }

    SECTION("invalid checks - random move does not allow illegal moves") {
        XO tictac;

        // fill all spaces
        tictac.move(1, 0);
        tictac.move(2, 1);
        tictac.move(1, 2);
        tictac.move(2, 3);
        tictac.move(1, 4);
        tictac.move(2, 5);
        tictac.move(1, 7);
        tictac.move(2, 6);
        tictac.move(1, 8);

        
        REQUIRE(!tictac.move(1)); 
        REQUIRE(!tictac.move(2)); 
        REQUIRE(tictac.over()); // should be over
    }

    SECTION("valid - random move preserves turn tracking correctly") {
        XO tictac;

        // alternate random moves
        REQUIRE(tictac.move(1)); 
        REQUIRE(tictac.turn() == 2); 
        REQUIRE(tictac.move(2)); 
        REQUIRE(tictac.turn() == 1); 
        REQUIRE(tictac.move(1));
        REQUIRE(tictac.turn() == 2); 
    }



}

TEST_CASE("spaces function ", "[XO]") {
    XO game;

    
    REQUIRE(game.spaces() == std::vector<uint8_t>{0, 1, 2, 3, 4, 5, 6, 7, 8});

    
    game.brd = { 1, 2, 0, 2, 1, 0, 0, 2, 1 };

    REQUIRE(game.spaces() == std::vector<uint8_t>{2, 5, 6});
    game.brd = { 1, 2, 1, 2, 1, 2, 2, 1, 2 };
    REQUIRE(game.spaces().empty());
}
TEST_CASE("over", "[XO]") {
    XO game;

    // game not done
    game.brd = { 1, 0, 2, 2, 1, 0, 0, 0, 0 };
    REQUIRE_FALSE(game.over());

   
    game.brd = { 1, 1, 1, 0, 2, 2, 0, 0, 0 }; 
    REQUIRE(game.over());
    
    game.brd = { 1, 2, 1, 2, 1, 2, 2, 1, 2 };
    REQUIRE(game.over());


    game.brd = { 1, 2, 0, 1, 1, 2, 2, 0, 1 };
    REQUIRE(game.over());

    game.brd = { 1, 2, 1, 1, 1, 2, 2, 2, 1 };
    REQUIRE(game.over());


    game.brd = { 0,0,0,0,0,0,0,0,0 };
    REQUIRE(!game.over());
    // full baord and x winner
    XO tictac;
    tictac.move(1, 0);
    tictac.move(2, 1);
    tictac.move(1, 2);
    tictac.move(2, 3);
    tictac.move(1, 4);
    tictac.move(2, 5);
    tictac.move(1, 7);
    tictac.move(2, 6);
    tictac.move(1, 8);
    REQUIRE(tictac.over()); 
    REQUIRE(tictac.winner() == 1); 

    XO tictac1;
    tictac1.move(1, 0);
    tictac1.move(2, 1);
    tictac1.move(1, 2);
    REQUIRE(!tictac1.over()); // game is still in progress

}

TEST_CASE("turn", "[XO]") {

    
    SECTION("(x's turn)") {
        const std::array<uint8_t, 9> board = { 1, 2, 0, 1, 0, 2, 0, 0, 0 };
        std::array<std::uint8_t, 9> turns = { 1, 2, 1, 2, 0, 0, 0, 0, 0 };

        XO tictac(board, turns);

   
        REQUIRE(tictac.turn() == 1);

    }

    SECTION("game is over 1") {

        const std::array<uint8_t, 9> board = { 1, 2, 0, 1, 1, 2, 2, 0, 1 };
        std::array<std::uint8_t, 9> turns = { 1, 2, 1, 2, 1, 2, 1, 0, 0 };

        XO tictac(board, turns);

     
        REQUIRE(tictac.turn() == 0);

    }

    SECTION("game is over 2") {

        const std::array<uint8_t, 9> board = { 2, 1, 0, 2, 2, 1, 1, 0, 2 };
        std::array<std::uint8_t, 9> turns = { 2, 1, 2, 1, 2, 1, 2, 0, 0 };

        XO tictac(board, turns);


        REQUIRE(tictac.turn() == 0);

    }

    SECTION("O's turn") {

        const std::array<uint8_t, 9> board = { 2, 1, 0, 2, 2, 1, 1, 2, 1 };
        std::array<std::uint8_t, 9> turns = { 2, 1, 2, 1, 2, 1, 2, 1, 0 };

        XO tictac(board, turns);


        REQUIRE(tictac.turn() == 2);

    }



    SECTION(" o's turn") {

        const std::array<uint8_t, 9> board = { 1, 2, 0, 1, 1, 2, 2, 1, 0 };
        std::array<std::uint8_t, 9> turns = { 1, 2, 1, 2, 1, 2, 1, 0, 0 };

        XO tictac(board, turns);

        REQUIRE(tictac.turn() == 2);

    }
    

}

TEST_CASE("operator<<", "[XO]") {
    XO game;

   
    game.brd = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::ostringstream output1;
    output1 << game;
    REQUIRE(output1.str() == "___\n___\n___\n");
  
    game.brd = { 1, 0, 2, 2, 1, 0, 2, 0, 1 };
    std::ostringstream output2;
    output2 << game;
    REQUIRE(output2.str() == "X_O\nOX_\nO_X\n");
    game.brd = { 1, 2, 1, 2, 1, 2, 2, 1, 2 };
    std::ostringstream output3;
    output3 << game;
    REQUIRE(output3.str() == "XOX\nOXO\nOXO\n");
}


TEST_CASE("Test Model Parameters") {
    QXO agent;
    Qopt valid_options = { 0.1f, 0.9f, 0.95f };
    Qopt invalid_options = { 1.5f, -0.2f, 1.1f };

    REQUIRE(agent.model(valid_options) == true);
    REQUIRE(agent.model(invalid_options) == false);
}

TEST_CASE("Test Training") {
    QXO qxo;
    Qopt options = { 0.1f, 0.9f, 1.0f };
    qxo.model(options);

    SECTION("Invalid player") {
        REQUIRE_THROWS_AS(qxo.train(3, 1000, 100), std::invalid_argument);
    }

    SECTION("Invalid batch size") {
        REQUIRE_THROWS_AS(qxo.train(1, 1050, 100), std::invalid_argument);
    }

   
    std::cout << "Initial training..." << std::endl;
    auto results =  qxo.train(1, 1000, 100);

    
    std::cout << "Intermediate training..." << std::endl;
    results = qxo.train(1, 5000, 500);

     
    std::cout << "Final training..." << std::endl;
    results = qxo.train(1, 10000, 1000);

    results = qxo.train(1, 20000, 2000);


    REQUIRE(results[0] + results[1] + results[2] == 20000);
  

    std::cout << results[0] << std::endl;

}


TEST_CASE("Test Playing - agent is X") {
    QXO agent;
    Qopt options = { 0.1f, 0.9f, 0.95f };
    agent.model(options);
    std::cout << "training if agent is x" << std::endl;
    agent.train(1, 7000, 700);
    agent.train(1, 7000, 700);


 

    std::cout << "start play" << std::endl;
    auto results = agent.play(1, 1000);
    std::cout << results[1] << std::endl;
    REQUIRE(results[0] > results[1]); 
    std::cout << results[0] << std::endl;

    SECTION("Invalid player") {
        REQUIRE(agent.play(3, 10) == std::array<std::uint16_t, 3>{0, 0, 0});
    }

    SECTION("Too many games") {
        REQUIRE(agent.play(1, 30000) == std::array<std::uint16_t, 3>{0, 0, 0});
    }

   

}


