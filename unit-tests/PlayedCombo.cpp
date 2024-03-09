#include "gtest/gtest.h"
#include "../src/common/exceptions/TichuException.h"
#include "../src/common/game_state/CardCollection/PlayedCombo.h"
#include "../src/common/serialization/json_utils.h"

// Ability to construct an object of class PlayedCombo
TEST(PlayedComboTest, RemoveAllCards) {
    Card c_1(2,4);
    Card c_2(10,3);
    Card c_3(6,1);
    Card c_4(2,2);
    std::vector<Card> cards = {c_1, c_2, c_3, c_4};
    PlayedCombo played_combo(cards);

    played_combo.remove_all_cards();
    EXPECT_EQ(played_combo.number_of_cards(), 0);
}

TEST(PlayedComboTest, SetCards) {
    Card c_1(2,4);
    Card c_2(10,3);
    Card c_3(6,1);
    Card c_4(2,2);
    PlayedCombo played_combo;

    CardCollection cards;
    cards.add_card(c_1);
    cards.add_card(c_1);
    cards.add_card(c_1);
    cards.add_card(c_2);
    cards.add_card(c_3);
    cards.add_card(c_2); 
    cards.add_card(c_4);
    
    played_combo.set_cards(cards);

    for (int i = 0; i < played_combo.number_of_cards(); i++)
    {
        EXPECT_EQ(played_combo.get_cards().at(i).get_type(), cards.get_cards().at(i).get_type());
        EXPECT_EQ(played_combo.get_cards().at(i).get_family(), cards.get_cards().at(i).get_family());
    }
}

TEST(PlayedComboTest, CanBePlayed) {
    Card c1_1(1,1); // Mah Jong
    Card c2_1(2,1);
    Card c3_1(3,1);
    Card c4_1(4,1);
    Card c5_1(5,1);
    Card c6_1(6,1);
    Card c7_1(7,1);
    Card c8_1(8,1);
    Card c9_1(9,1);
    Card c10_1(10,1);
    Card c11_1(11,1);
    Card c12_1(12,1);
    Card c13_1(13,1);
    Card c14_1(14,1);

    Card c2_2(2,2);
    Card c3_2(3,2);
    Card c4_2(4,2);
    Card c5_2(5,2);
    Card c6_2(6,2);
    Card c7_2(7,2);
    Card c8_2(8,2);
    Card c9_2(9,2);
    Card c10_2(10,2);
    Card c11_2(11,2);
    Card c12_2(12,2);
    Card c13_2(13,2);
    Card c14_2(14,2);

    Card c2_3(2,3);
    Card c3_3(3,3);
    Card c4_3(4,3);
    Card c5_3(5,3);

    Card c2_4(2,4);
    Card c3_4(3,4);
   
    CardCollection nocard;

    CardCollection single1;
    single1.add_card(c2_1);

    CardCollection single2;
    single2.add_card(c8_2);

    CardCollection doubles1;
    doubles1.add_card(c2_1);
    doubles1.add_card(c2_2);

    CardCollection doubles2;
    doubles2.add_card(c3_1);
    doubles2.add_card(c3_2);

    CardCollection doubles3;
    doubles3.add_card(c2_1);
    doubles3.add_card(c2_2);
    doubles3.add_card(c3_1);
    doubles3.add_card(c3_2);

    CardCollection doubles4;
    doubles4.add_card(c4_1);
    doubles4.add_card(c4_2);
    doubles4.add_card(c3_1);
    doubles4.add_card(c3_2);

    CardCollection doubles5;
    doubles5.add_card(c6_1);
    doubles5.add_card(c6_2);
    doubles5.add_card(c5_1);
    doubles5.add_card(c5_2);
    doubles5.add_card(c4_1);
    doubles5.add_card(c4_2);
    doubles5.add_card(c3_1);
    doubles5.add_card(c3_2);

    CardCollection doubles6;
    doubles6.add_card(c7_1);
    doubles6.add_card(c7_2);
    doubles6.add_card(c6_1);
    doubles6.add_card(c6_2);
    doubles6.add_card(c5_1);
    doubles6.add_card(c5_2);
    doubles6.add_card(c4_1);
    doubles6.add_card(c4_2);

    CardCollection tripple1;
    tripple1.add_card(c2_1);
    tripple1.add_card(c2_2);
    tripple1.add_card(c2_3);

    CardCollection tripple2;
    tripple2.add_card(c3_1);
    tripple2.add_card(c3_2);
    tripple2.add_card(c3_3);

    CardCollection bomb1;
    bomb1.add_card(c2_1);
    bomb1.add_card(c2_2);
    bomb1.add_card(c2_3);
    bomb1.add_card(c2_4);

    CardCollection bomb2;
    bomb2.add_card(c3_1);
    bomb2.add_card(c3_2);
    bomb2.add_card(c3_3);
    bomb2.add_card(c3_4);

    CardCollection fullhouse1;
    fullhouse1.add_card(c2_1);
    fullhouse1.add_card(c2_2);
    fullhouse1.add_card(c2_3);
    fullhouse1.add_card(c3_1);
    fullhouse1.add_card(c3_2);
    
    CardCollection fullhouse2;
    fullhouse2.add_card(c2_1);
    fullhouse2.add_card(c2_2);
    fullhouse2.add_card(c3_1);
    fullhouse2.add_card(c3_2);
    fullhouse2.add_card(c3_3);

    CardCollection fullhouse3;
    fullhouse3.add_card(c4_1);
    fullhouse3.add_card(c4_2);
    fullhouse3.add_card(c4_3);
    fullhouse3.add_card(c3_1);
    fullhouse3.add_card(c3_2);
    
    CardCollection fullhouse4;
    fullhouse4.add_card(c6_1);
    fullhouse4.add_card(c6_2);
    fullhouse4.add_card(c5_1);
    fullhouse4.add_card(c5_2);
    fullhouse4.add_card(c5_3);

    CardCollection street1;
    street1.add_card(c2_1);
    street1.add_card(c3_2);
    street1.add_card(c4_1);
    street1.add_card(c5_2);
    street1.add_card(c6_1);

    CardCollection street2;
    street2.add_card(c3_1);
    street2.add_card(c4_2);
    street2.add_card(c5_1);
    street2.add_card(c6_2);
    street2.add_card(c7_1);

    CardCollection street3;
    street3.add_card(c2_1);
    street3.add_card(c3_1);
    street3.add_card(c4_1);
    street3.add_card(c5_2);
    street3.add_card(c6_1);
    street3.add_card(c7_1);

    CardCollection street4;
    street4.add_card(c1_1);
    street4.add_card(c2_1);
    street4.add_card(c3_1);
    street4.add_card(c4_1);
    street4.add_card(c5_2);
    street4.add_card(c6_1);

    PlayedCombo combo;

    // Single
    combo.set_cards(nocard);
    EXPECT_TRUE(combo.can_be_played(single2));
    combo.set_cards(single2);
    EXPECT_FALSE(combo.can_be_played(single1)); // Card must be higher than the one on the table
    combo.set_cards(single1);
    EXPECT_TRUE(combo.can_be_played(single2));
    EXPECT_TRUE(combo.can_be_played(bomb1));
    
    
    // Double
    combo.set_cards(nocard);
    EXPECT_TRUE(combo.can_be_played(doubles2));
    combo.set_cards(doubles2);
    EXPECT_FALSE(combo.can_be_played(doubles1)); 
    combo.set_cards(doubles1);
    EXPECT_TRUE(combo.can_be_played(doubles2));
    EXPECT_TRUE(combo.can_be_played(bomb1));

    // Multiple Doubles
    combo.set_cards(doubles1);
    EXPECT_FALSE(combo.can_be_played(doubles4)); 
    combo.set_cards(doubles4);
    EXPECT_FALSE(combo.can_be_played(doubles3)); 
    combo.set_cards(doubles3);
    EXPECT_TRUE(combo.can_be_played(doubles4));
    EXPECT_TRUE(combo.can_be_played(bomb1));
    EXPECT_FALSE(combo.can_be_played(doubles6)); // must be same length
    combo.set_cards(doubles6);
    EXPECT_FALSE(combo.can_be_played(doubles5)); 
    combo.set_cards(doubles5);
    EXPECT_TRUE(combo.can_be_played(doubles6));
    EXPECT_TRUE(combo.can_be_played(bomb1));

    // Tripples
    combo.set_cards(tripple2);
    EXPECT_FALSE(combo.can_be_played(tripple1)); 
    combo.set_cards(tripple1);
    EXPECT_TRUE(combo.can_be_played(tripple2));
    EXPECT_TRUE(combo.can_be_played(bomb1));

    // Fullhouse
    combo.set_cards(fullhouse1);
    EXPECT_TRUE(combo.can_be_played(fullhouse2));
    EXPECT_TRUE(combo.can_be_played(fullhouse3));
    EXPECT_TRUE(combo.can_be_played(fullhouse4));
    combo.set_cards(fullhouse2);
    EXPECT_FALSE(combo.can_be_played(fullhouse1));
    EXPECT_TRUE(combo.can_be_played(fullhouse3));
    EXPECT_TRUE(combo.can_be_played(fullhouse4));
    combo.set_cards(fullhouse3);
    EXPECT_FALSE(combo.can_be_played(fullhouse1));
    EXPECT_FALSE(combo.can_be_played(fullhouse2));
    EXPECT_TRUE(combo.can_be_played(fullhouse4));
    combo.set_cards(fullhouse4);
    EXPECT_FALSE(combo.can_be_played(fullhouse1));
    EXPECT_FALSE(combo.can_be_played(fullhouse2));
    EXPECT_FALSE(combo.can_be_played(fullhouse3));
    EXPECT_TRUE(combo.can_be_played(bomb1));

    // Street
    combo.set_cards(street1);
    EXPECT_TRUE(combo.can_be_played(street2));
    EXPECT_FALSE(combo.can_be_played(street3)); // Must be same length
    combo.set_cards(street2);
    EXPECT_FALSE(combo.can_be_played(street1));
    combo.set_cards(nocard);
    EXPECT_TRUE(combo.can_be_played(street4));
    combo.set_cards(street4);
    EXPECT_TRUE(combo.can_be_played(street3));
    EXPECT_TRUE(combo.can_be_played(bomb1));

    // Bomb
    combo.set_cards(bomb1);
    EXPECT_TRUE(combo.can_be_played(bomb2));
    combo.set_cards(bomb2);
    EXPECT_FALSE(combo.can_be_played(bomb1));

}


 // Serialization and subsequent deserialization must yield the same object
TEST(PlayedComboTest, SerializationEquality) {
    Card c_1(2,4);
    Card c_2(10,3);
    Card c_3(6,1);
    std::vector<Card> cards = {c_1, c_2, c_3};
    PlayedCombo card_col_send(cards);
    rapidjson::Document* json_send = card_col_send.to_json();
    std::string message = json_utils::to_string(json_send);
    delete json_send;

    rapidjson::Document json_recv = rapidjson::Document(rapidjson::kObjectType);
    json_recv.Parse(message.c_str());
    PlayedCombo card_col_recv = PlayedCombo::from_json(json_recv);
    EXPECT_EQ(card_col_send.get_id(), card_col_recv.get_id());
    EXPECT_EQ(card_col_send.number_of_cards(), card_col_recv.number_of_cards());
    for (int i = 0; i < card_col_recv.number_of_cards(); i++)
    {
        EXPECT_EQ(card_col_send.get_cards().at(i).get_type(), card_col_recv.get_cards().at(i).get_type());
        EXPECT_EQ(card_col_send.get_cards().at(i).get_family(), card_col_recv.get_cards().at(i).get_family());
    }
}

// Deserializing an invalid string must throw a TichuException
TEST(PlayedComboTest, SerializationException) {
    rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
    json.Parse("not json");
    EXPECT_THROW(PlayedCombo::from_json(json), TichuException);
} 