#include "gtest/gtest.h"
#include "../src/common/exceptions/TichuException.h"
#include "../src/common/game_state/CardCollection/DrawPile.h"
#include "../src/common/serialization/json_utils.h"

// Ability to construct an object of class DrawPile
TEST(DrawPileTest, get_rnd_card) {
    Card c_1(2,4);
    Card c_2(10,3);
    Card c_3(6,1);
    Card c_4(2,2);
    std::vector<Card> cards = {c_4, c_1, c_3, c_2};
    DrawPile draw(cards);

    CardCollection rnd_cards = draw.get_rnd_card(1);
    EXPECT_FALSE(draw.try_get_card(rnd_cards.get_cards().at(0)));
}

 // Serialization and subsequent deserialization must yield the same object
TEST(DrawPileTest, SerializationEquality) {
    Card c_1(2,4);
    Card c_2(10,3);
    Card c_3(6,1);
    Card c_4(2,2);
    std::vector<Card> cards = {c_4, c_1, c_3, c_2};
    DrawPile draw_send(cards);

    rapidjson::Document* json_send = draw_send.to_json();
    std::string message = json_utils::to_string(json_send);
    delete json_send;

    rapidjson::Document json_recv = rapidjson::Document(rapidjson::kObjectType);
    json_recv.Parse(message.c_str());
    DrawPile draw_recv = DrawPile::from_json(json_recv);
    EXPECT_EQ(draw_send.get_id(), draw_recv.get_id());
    EXPECT_EQ(draw_send.number_of_cards(), draw_recv.number_of_cards());
    for (int i = 0; i < draw_recv.number_of_cards(); i++)
    {
        EXPECT_EQ(draw_send.get_cards().at(i).get_type(), draw_recv.get_cards().at(i).get_type());
        EXPECT_EQ(draw_send.get_cards().at(i).get_family(), draw_recv.get_cards().at(i).get_family());
    }
}

// Deserializing an invalid string must throw a TichuException
TEST(DrawPileTest, SerializationException) {
    rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
    json.Parse("not json");
    EXPECT_THROW(DrawPile::from_json(json), TichuException);
} 
