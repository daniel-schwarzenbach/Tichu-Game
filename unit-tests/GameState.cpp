#include "gtest/gtest.h"
#include "../src/common/exceptions/TichuException.h"
#include "../src/common/game_state/GameState.h"
#include "../src/common/serialization/json_utils.h"

class GameStateTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        
    }

    /* Any object and subroutine declared here can be accessed in the tests */
    GameState gamestate;
    std::string gameID = "fd32lkj23lj";
    
};

// Ability to construct an object of class GameState
TEST_F(GameStateTest, Add_RemovePlayers) {
    Player player1("Max Muster");
    Player player2("Becka Bauer");
    Player player3("Lorenz Löffel");
    Player player4("Florian Fallobst");
    Player player5("Karin Kümmerer");

    EXPECT_TRUE(gamestate.add_player_to_game(player1));
    EXPECT_EQ(gamestate.get_players().at(0).get_name(), "Max Muster");
    EXPECT_TRUE(gamestate.add_player_to_team(player1, "phoenix"));
    EXPECT_EQ(gamestate.get_players().at(0).get_teamID(), "phoenix");
    EXPECT_TRUE(gamestate.add_player_to_team(player1, ""));     // Check if you can also unselect a team
    EXPECT_EQ(gamestate.get_players().at(0).get_teamID(), "");
    EXPECT_TRUE(gamestate.add_player_to_team(player1, "dragon"));
    EXPECT_EQ(gamestate.get_players().at(0).get_teamID(), "dragon");

    EXPECT_TRUE(gamestate.add_player_to_game(player2));
    EXPECT_TRUE(gamestate.add_player_to_team(player2, "dragon"));

    EXPECT_TRUE(gamestate.add_player_to_game(player3));
    EXPECT_FALSE(gamestate.add_player_to_team(player3, "dragon")); // Only two players per team allowed
    EXPECT_TRUE(gamestate.add_player_to_team(player3, "phoenix"));
  
    EXPECT_TRUE(gamestate.add_player_to_game(player4));
    EXPECT_TRUE(gamestate.add_player_to_team(player4, "phoenix"));
  
    EXPECT_FALSE(gamestate.add_player_to_game(player5)); // Only players allowed in the game

    EXPECT_TRUE(gamestate.get_players().at(0).get_teamID() == "phoenix");
    EXPECT_TRUE(gamestate.get_players().at(1).get_teamID() == "dragon");
    EXPECT_TRUE(gamestate.get_players().at(2).get_teamID() == "phoenix");
    EXPECT_TRUE(gamestate.get_players().at(3).get_teamID() == "dragon");

    gamestate.remove_player_from_game(player2);
    EXPECT_TRUE(gamestate.add_player_to_game(player5));
    EXPECT_TRUE(gamestate.add_player_to_team(player5, "dragon"));

    EXPECT_TRUE(gamestate.get_players().at(0).get_teamID() == "phoenix");
    EXPECT_TRUE(gamestate.get_players().at(1).get_teamID() == "dragon");
    EXPECT_TRUE(gamestate.get_players().at(2).get_teamID() == "phoenix");
    EXPECT_TRUE(gamestate.get_players().at(3).get_teamID() == "dragon");

    std::vector<Player> list = gamestate.get_players_ordered(player1);
    EXPECT_EQ(list.at(0).get_name(), "Max Muster");
    EXPECT_EQ(list.at(1).get_name(), "Florian Fallobst");
    EXPECT_EQ(list.at(2).get_name(), "Karin Kümmerer");
    EXPECT_EQ(list.at(3).get_name(), "Lorenz Löffel");

    list = gamestate.get_players_ordered(player3);
    EXPECT_EQ(list.at(0).get_name(), "Lorenz Löffel");
    EXPECT_EQ(list.at(1).get_name(), "Max Muster");
    EXPECT_EQ(list.at(2).get_name(), "Florian Fallobst");
    EXPECT_EQ(list.at(3).get_name(), "Karin Kümmerer");

    list = gamestate.get_players_ordered(player4);
    EXPECT_EQ(list.at(0).get_name(), "Florian Fallobst");
    EXPECT_EQ(list.at(1).get_name(), "Karin Kümmerer");
    EXPECT_EQ(list.at(2).get_name(), "Lorenz Löffel");
    EXPECT_EQ(list.at(3).get_name(), "Max Muster");


    list = gamestate.get_players_ordered(player5);
    EXPECT_EQ(list.at(0).get_name(), "Karin Kümmerer");
    EXPECT_EQ(list.at(1).get_name(), "Lorenz Löffel");
    EXPECT_EQ(list.at(2).get_name(), "Max Muster");
    EXPECT_EQ(list.at(3).get_name(), "Florian Fallobst");
    
}

TEST_F(GameStateTest, StartGame){
    Player player1("Max Muster");
    Player player2("Becka Bauer");
    Player player3("Lorenz Löffel");
    Player player4("Florian Fallobst");

    gamestate.add_player_to_game(player1);
    gamestate.add_player_to_game(player2);
    gamestate.add_player_to_game(player3);

    EXPECT_FALSE(gamestate.start_game());

    gamestate.add_player_to_game(player2);

    EXPECT_TRUE(gamestate.start_game());
}

TEST_F(GameStateTest, StartRound){
    Player player1("Max Muster");
    Player player2("Becka Bauer");
    Player player3("Lorenz Löffel");
    Player player4("Florian Fallobst");

    gamestate.add_player_to_game(player1);
    gamestate.add_player_to_game(player2);
    gamestate.add_player_to_game(player3);
    gamestate.add_player_to_game(player4);
    gamestate.start_game();

    for(int i = 0; i < 4; i++) {
        EXPECT_EQ(gamestate.get_players().at(i).get_handcards().size(), 8);
    }
    EXPECT_EQ(gamestate.get_gamephase()->get_value(), "switch_view");
}

TEST_F(GameStateTest, Tichu) {
    Player player1("Max Muster");
    Player player2("Becka Bauer");
    Player player3("Lorenz Löffel");
    Player player4("Florian Fallobst");

    gamestate.add_player_to_game(player1);
    gamestate.add_player_to_game(player2);
    gamestate.add_player_to_game(player3);
    gamestate.add_player_to_game(player4);

    gamestate.start_game();
    
    std::string msg = "small tichu";
    std::string& err = msg;
    EXPECT_TRUE(gamestate.tichu(player1, msg, err));
    msg = "big tichu";
    EXPECT_FALSE(gamestate.tichu(player1, msg, err)); // A player can only announce one Tichu 

}

 // Serialization and subsequent deserialization must yield the same object
TEST_F(GameStateTest, SerializationEquality) {
    GameState gamestate_send;

    rapidjson::Document* json_send = gamestate_send.to_json();
    std::string message = json_utils::to_string(json_send);
    delete json_send;

    rapidjson::Document json_recv = rapidjson::Document(rapidjson::kObjectType);
    json_recv.Parse(message.c_str());
    GameState gamestate_recv = GameState::from_json(json_recv);
    EXPECT_EQ(gamestate_send.get_id(), gamestate_recv.get_id());
    for (int i = 0; i < gamestate_recv.get_number_of_players(); i++)
    {
        EXPECT_EQ(gamestate_send.get_players().at(i).get_id(), gamestate_recv.get_players().at(i).get_id());
    }
    EXPECT_EQ(gamestate_send.get_teams().at(0).get_id(), gamestate_recv.get_teams().at(0).get_id());
    EXPECT_EQ(gamestate_send.get_teams().at(1).get_id(), gamestate_recv.get_teams().at(1).get_id());
    EXPECT_EQ(gamestate_send.get_gamephase()->get_value(), gamestate_recv.get_gamephase()->get_value());
    EXPECT_EQ(gamestate_send.get_current_player_index()->get_value(), gamestate_recv.get_current_player_index()->get_value());
    EXPECT_EQ(gamestate_send.get_round_counter()->get_value(), gamestate_recv.get_round_counter()->get_value());

}

// Deserializing an invalid string must throw a TichuException
TEST_F(GameStateTest, SerializationException) {
    rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
    json.Parse("not json");
    EXPECT_THROW(GameState::from_json(json), TichuException);
} 
