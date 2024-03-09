#include "gtest/gtest.h"
#include "../src/common/exceptions/TichuException.h"
#include "../src/common/game_state/CardCollection/Card.h"
#include "../src/common/serialization/json_utils.h"


// Serialization and subsequent deserialization must yield the same object
TEST(CardTest, SerializationEquality) {
    Card card_send(3,3);
    rapidjson::Document* json_send = card_send.to_json();
    std::string message = json_utils::to_string(json_send);
    delete json_send;

    rapidjson::Document json_recv = rapidjson::Document(rapidjson::kObjectType);
    json_recv.Parse(message.c_str());
    Card card_recv = Card::from_json(json_recv);
    EXPECT_EQ(card_send.get_id(), card_recv.get_id());
    EXPECT_EQ(card_send.get_type(), card_recv.get_type());
    EXPECT_EQ(card_send.get_family(), card_recv.get_family());
}

// Deserializing an invalid string must throw a TichuException
TEST(CardTest, SerializationException) {
    rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
    json.Parse("not json");
    EXPECT_THROW(Card::from_json(json), TichuException);
}
