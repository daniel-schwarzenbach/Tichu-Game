#include "gtest/gtest.h"
#include "../src/common/exceptions/TichuException.h"
#include "../src/common/game_state/CardCollection/CardCollection.h"
#include "../src/common/serialization/json_utils.h"

// Ability to construct an object of class CardCollection
TEST(CardCollectionTest, Constructor) {
    Card c_1(2,4);
    Card c_2(10,3);
    Card c_3(6,1);
    std::vector<Card> cards = {c_1, c_2, c_3};
    CardCollection col(cards);
}

TEST(CardCollectionTest, Add_RemoveCards) {
    Card c_1(2,4);
    Card c_2(10,3);
    Card c_3(6,1);
    Card c_4(2,2);
    CardCollection col;
    col.add_card(c_1);
    col.add_card(c_1);
    col.add_card(c_1);
    col.add_card(c_2);
    col.add_card(c_3);
    col.add_card(c_2); 
    col.add_card(c_4);

    std::vector<Card> expected_cards = {c_4, c_1, c_1, c_1, c_3, c_2, c_2};
   
    for (int i = 0; i < col.number_of_cards(); i++)
    {   
        EXPECT_EQ(col.get_cards().at(i).get_type(), expected_cards.at(i).get_type());
        EXPECT_EQ(col.get_cards().at(i).get_family(), expected_cards.at(i).get_family());
    }

    col.remove_card(c_2);
    col.remove_card(c_2);
    col.remove_card(c_4);
    expected_cards = {c_1, c_1, c_1, c_3};
   
    for (int i = 0; i < col.number_of_cards(); i++)
    {   
        EXPECT_EQ(col.get_cards().at(i).get_type(), expected_cards.at(i).get_type());
        EXPECT_EQ(col.get_cards().at(i).get_family(), expected_cards.at(i).get_family());
    }

    EXPECT_FALSE(col.remove_card(c_2)); // Can't remove a card, that isn't part of the CardCollection

    col.remove_all_cards();
    EXPECT_TRUE(col.get_cards().size() == 0);
}

TEST(CardCollectionTest, IsValid) {
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

    Card c2_4(2,4);
    Card c3_4(3,4);
   
    CardCollection nocard;
    EXPECT_FALSE(nocard.is_valid());
    EXPECT_EQ(nocard.get_type(), "");

    CardCollection single1;
    single1.add_card(c2_1);
    EXPECT_TRUE(single1.is_valid());
    EXPECT_FALSE(single1.is_valid_bomb());
    EXPECT_EQ(single1.get_type(), "Single");

    CardCollection doubles1;
    doubles1.add_card(c2_1);
    doubles1.add_card(c2_2);
    EXPECT_TRUE(doubles1.is_valid());
    EXPECT_EQ(doubles1.get_type(), "Doubles");

    CardCollection wrong1;
    wrong1.add_card(c3_1);
    wrong1.add_card(c2_2);
    EXPECT_FALSE(wrong1.is_valid());
    EXPECT_EQ(wrong1.get_type(), "");

    CardCollection doubles2;
    doubles2.add_card(c2_1);
    doubles2.add_card(c2_2);
    doubles2.add_card(c3_1);
    doubles2.add_card(c3_2);
    EXPECT_TRUE(doubles2.is_valid());
    EXPECT_EQ(doubles2.get_type(), "Following_Doubles");

    CardCollection wrong2;
    wrong2.add_card(c2_1);
    wrong2.add_card(c2_2);
    wrong2.add_card(c3_1);
    wrong2.add_card(c4_2);
    EXPECT_FALSE(wrong2.is_valid());
    EXPECT_EQ(wrong2.get_type(), "");

    CardCollection doubles3;
    doubles3.add_card(c2_1);
    doubles3.add_card(c2_2);
    doubles3.add_card(c3_1);
    doubles3.add_card(c3_2);
    doubles3.add_card(c4_1);
    doubles3.add_card(c4_2);
    EXPECT_TRUE(doubles3.is_valid());
    EXPECT_EQ(doubles3.get_type(), "Following_Doubles");

    CardCollection tripple1;
    tripple1.add_card(c2_1);
    tripple1.add_card(c2_2);
    tripple1.add_card(c2_3);
    EXPECT_TRUE(tripple1.is_valid());
    EXPECT_EQ(tripple1.get_type(), "Tripple");

    CardCollection wrong3;
    wrong3.add_card(c2_1);
    wrong3.add_card(c2_2);
    wrong3.add_card(c3_3);
    EXPECT_FALSE(wrong3.is_valid());
    EXPECT_EQ(wrong3.get_type(), "");

    CardCollection bomb1;
    bomb1.add_card(c3_1);
    bomb1.add_card(c3_2);
    bomb1.add_card(c3_3);
    bomb1.add_card(c3_4);
    EXPECT_TRUE(bomb1.is_valid());
    EXPECT_TRUE(bomb1.is_valid_bomb());
    EXPECT_EQ(bomb1.get_type(), "Bomb_Quad");

    CardCollection wrong4;
    wrong4.add_card(c3_1);
    wrong4.add_card(c2_2);
    wrong4.add_card(c2_3);
    wrong4.add_card(c2_4);
    EXPECT_FALSE(wrong4.is_valid());
    EXPECT_FALSE(wrong4.is_valid_bomb());
    EXPECT_EQ(wrong4.get_type(), "");

    CardCollection fullhouse1;
    fullhouse1.add_card(c2_1);
    fullhouse1.add_card(c2_2);
    fullhouse1.add_card(c2_3);
    fullhouse1.add_card(c3_1);
    fullhouse1.add_card(c3_2);
    EXPECT_TRUE(fullhouse1.is_valid());
    EXPECT_EQ(fullhouse1.get_type(), "Fullhouse");
    
    CardCollection fullhouse2;
    fullhouse2.add_card(c2_1);
    fullhouse2.add_card(c2_2);
    fullhouse2.add_card(c3_1);
    fullhouse2.add_card(c3_2);
    fullhouse2.add_card(c3_3);
    EXPECT_TRUE(fullhouse2.is_valid());
    EXPECT_EQ(fullhouse2.get_type(), "Fullhouse");

    CardCollection wrong5;
    wrong5.add_card(c3_1);
    wrong5.add_card(c3_2);
    wrong5.add_card(c4_1);
    wrong5.add_card(c2_2);
    wrong5.add_card(c2_3);
    EXPECT_FALSE(wrong5.is_valid());
    EXPECT_EQ(wrong5.get_type(), "");

    CardCollection street1;
    street1.add_card(c2_1);
    street1.add_card(c3_1);
    street1.add_card(c4_2);
    street1.add_card(c5_1);
    street1.add_card(c6_1);
    EXPECT_TRUE(street1.is_valid());
    EXPECT_EQ(street1.get_type(), "Street");

    CardCollection street2;
    street2.add_card(c5_1);
    street2.add_card(c6_1);
    street2.add_card(c7_1);
    street2.add_card(c8_1);
    street2.add_card(c9_1);
    street2.add_card(c10_1);
    EXPECT_TRUE(street2.is_valid());
    EXPECT_EQ(street2.get_type(), "Bomb_Street");

    CardCollection street3;
    street3.add_card(c1_1);
    street3.add_card(c2_1);
    street3.add_card(c3_1);
    street3.add_card(c4_2);
    street3.add_card(c5_1);
    EXPECT_TRUE(street3.is_valid());
    EXPECT_EQ(street3.get_type(), "Street");

    CardCollection wrong6;
    wrong6.add_card(c2_1);
    wrong6.add_card(c3_1);
    wrong6.add_card(c4_1);
    wrong6.add_card(c5_1);
    wrong6.add_card(c7_1);
    EXPECT_FALSE(wrong6.is_valid());
    EXPECT_EQ(wrong6.get_type(), "");

    CardCollection wrong7;
    wrong7.add_card(c2_1);
    wrong7.add_card(c3_1);
    wrong7.add_card(c4_1);
    wrong7.add_card(c5_1);
    EXPECT_FALSE(wrong7.is_valid());
    EXPECT_EQ(wrong7.get_type(), "");
}
 // Serialization and subsequent deserialization must yield the same object
TEST(CardCollectionTest, SerializationEquality) {
    Card c_1(2,4);
    Card c_2(10,3);
    Card c_3(6,1);
    std::vector<Card> cards = {c_1, c_2, c_3};
    CardCollection card_col_send(cards);
    rapidjson::Document* json_send = card_col_send.to_json();
    std::string message = json_utils::to_string(json_send);
    delete json_send;

    rapidjson::Document json_recv = rapidjson::Document(rapidjson::kObjectType);
    json_recv.Parse(message.c_str());
    CardCollection card_col_recv = CardCollection::from_json(json_recv);
    EXPECT_EQ(card_col_send.get_id(), card_col_recv.get_id());
    EXPECT_EQ(card_col_send.number_of_cards(), card_col_recv.number_of_cards());
    for (int i = 0; i < card_col_recv.number_of_cards(); i++)
    {
        EXPECT_EQ(card_col_send.get_cards().at(i).get_type(), card_col_recv.get_cards().at(i).get_type());
        EXPECT_EQ(card_col_send.get_cards().at(i).get_family(), card_col_recv.get_cards().at(i).get_family());
    }
}

// Deserializing an invalid string must throw a TichuException
TEST(CardCollectionTest, SerializationException) {
    rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
    json.Parse("not json");
    EXPECT_THROW(CardCollection::from_json(json), TichuException);
} 
