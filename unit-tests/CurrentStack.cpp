#include "gtest/gtest.h"
#include "../src/common/exceptions/TichuException.h"
#include "../src/common/game_state/CurrentStack.h"
#include "../src/common/game_state/CardCollection/CardCollection.h"
#include "../src/common/serialization/json_utils.h"

// Ability to construct an object of class CurrentStack
TEST(CurrentStackTest, Play) {
    
}

// Be able to reset the currentstack, when a round ends.
TEST(CurrentStackTest, Reset) {
    CurrentStack stack;
    Card c_1(2,4);
    Card c_2(2,3);
    Card c_3(2,1);
    CardCollection cards;
    cards.add_card(c_1);
    cards.add_card(c_2);
    cards.add_card(c_3);

    bool check = stack.play(cards);
    stack.reset();
    EXPECT_EQ(stack.get_points(), 0);
    EXPECT_EQ(stack.get_cards().size(), 0);
}

 // Serialization and subsequent deserialization must yield the same object
TEST(CurrentStackTest, SerializationEquality) {
    CurrentStack stack_send;
    Card c_1(2,4);
    Card c_2(2,3);
    Card c_3(2,1);
    CardCollection cards;
    cards.add_card(c_1);
    cards.add_card(c_2);
    cards.add_card(c_3);

    bool check = stack_send.play(cards);

    rapidjson::Document* json_send = stack_send.to_json();
    std::string message = json_utils::to_string(json_send);
    delete json_send;

    rapidjson::Document json_recv = rapidjson::Document(rapidjson::kObjectType);
    json_recv.Parse(message.c_str());
    CurrentStack stack_recv = CurrentStack::from_json(json_recv);
    EXPECT_EQ(stack_send.get_id(), stack_recv.get_id());
    EXPECT_EQ(stack_send.get_points(), stack_recv.get_points());
    for (int i = 0; i < stack_recv.get_cards().size(); i++)
    {
        EXPECT_EQ(stack_send.get_cards().at(i).get_type(), stack_recv.get_cards().at(i).get_type());
        EXPECT_EQ(stack_send.get_cards().at(i).get_family(), stack_recv.get_cards().at(i).get_family());
    }
}

// Deserializing an invalid string must throw a TichuException
TEST(CurrentStackTest, SerializationException) {
    rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
    json.Parse("not json");
    EXPECT_THROW(CurrentStack::from_json(json), TichuException);
} 
