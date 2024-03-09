#include "gtest/gtest.h"
#include "../src/common/exceptions/TichuException.h"
#include "../src/common/game_state/Player.h"
#include "../src/common/serialization/json_utils.h"

TEST(PlayerTest, AddRemoveCards) {
    Player player("Max Muster");
    Card card1(3,4);
    Card card2(7,2);
    Card card3(5,1);
    Card card4(14,2);
    std::vector<Card> cards = {card1, card2, card3, card4};
    CardCollection card_collection(cards);
    player.add_cards(card_collection);

    EXPECT_TRUE(player.remove_cards(cards));
    EXPECT_FALSE(player.remove_cards(cards)); // You can't remove a card that isn't part of the player's hand
}

TEST(PlayerTest, Tichu){
    Player player("Karl Kübel");
    EXPECT_FALSE(player.get_big_tichu());
    EXPECT_FALSE(player.get_small_tichu());
    std::string msg;
    std::string& err = msg;
    EXPECT_TRUE(player.tichu("big tichu", err));
    EXPECT_FALSE(player.tichu("small tichu", err)); // You can't announce two differnet Tichus

}
// Serialization and subsequent deserialization must yield the same object
TEST(PlayerTest, SerializationEquality) {
    Player player_send("Max Muster");
    player_send.set_gameID("fdsalkjfdsj");
    player_send.set_teamID("3324lh34");
    player_send.add_points(100);
    rapidjson::Document* json_send = player_send.to_json();
    std::string message = json_utils::to_string(json_send);
    delete json_send;

    rapidjson::Document json_recv = rapidjson::Document(rapidjson::kObjectType);
    json_recv.Parse(message.c_str());
    Player player_recv = Player::from_json(json_recv);
    EXPECT_EQ(player_send.get_id(), player_recv.get_id());
    EXPECT_EQ(player_send.get_points(), player_recv.get_points());
    EXPECT_EQ(player_send.get_small_tichu(), player_recv.get_small_tichu());
    EXPECT_EQ(player_send.get_big_tichu(), player_recv.get_big_tichu());
    EXPECT_EQ(player_send.get_name(), player_recv.get_name());
    EXPECT_EQ(player_send.get_teamID(), player_recv.get_teamID());
    EXPECT_EQ(player_send.get_gameID(), player_recv.get_gameID());
}

// Deserializing an invalid string must throw a TichuException
TEST(PlayerTest, SerializationException) {
    rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
    json.Parse("not json");
    EXPECT_THROW(Player::from_json(json), TichuException);
}