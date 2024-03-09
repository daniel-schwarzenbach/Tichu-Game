#include "gtest/gtest.h"
#include "../src/common/exceptions/TichuException.h"
#include "../src/common/game_state/CardCollection/DrawPile.h"
#include "../src/common/game_state/CardCollection/PlayedCombo.h"
#include "../src/common/serialization/json_utils.h"



TEST(PhoenixTests, Single){
    // mah jong
    CardCollection mah_jongColl;
    mah_jongColl.add_card(mah_jong);
    // phoenix
    CardCollection phoenixColl;
    phoenixColl.add_card(phoenix);
    // 2
    CardCollection twoColl;
    twoColl.add_card(Card(2,1));
    // 3
    CardCollection threeColl;
    threeColl.add_card(Card(3,2));
    // dragon
    CardCollection dragonColl;
    dragonColl.add_card(dragon);
    // Ace
    CardCollection aceColl;
    aceColl.add_card(Card(14,1));
    //              Check low cases
    {
    PlayedCombo playedCombo1;
    playedCombo1.set_cards(mah_jongColl);
    EXPECT_TRUE(playedCombo1.can_be_played(phoenixColl));
    playedCombo1.set_cards(phoenixColl);
    EXPECT_TRUE(playedCombo1.can_be_played(twoColl));
    playedCombo1.set_cards(twoColl);
    EXPECT_TRUE(playedCombo1.can_be_played(phoenixColl));
    playedCombo1.set_cards(phoenixColl);
    EXPECT_TRUE(playedCombo1.can_be_played(threeColl));
    EXPECT_FALSE(playedCombo1.can_be_played(twoColl));
    }
    //              Check low cases2
    {
    PlayedCombo playedCombo2;
    EXPECT_TRUE(playedCombo2.can_be_played(phoenixColl));
    playedCombo2.set_cards(phoenixColl.get_cards());
    EXPECT_FALSE(playedCombo2.can_be_played(mah_jongColl));
    EXPECT_TRUE(playedCombo2.can_be_played(twoColl));
    playedCombo2.set_cards(twoColl);
    EXPECT_TRUE(playedCombo2.can_be_played(phoenixColl));
    }
    //              Check high cases
    {
    PlayedCombo pC3;
    pC3.set_cards(aceColl);
    EXPECT_TRUE(pC3.can_be_played(phoenixColl));
    pC3.set_cards(phoenixColl);
    EXPECT_TRUE(pC3.can_be_played(dragonColl));
    EXPECT_FALSE(pC3.can_be_played(aceColl));
    }
}

TEST(PhoenixTests, Doubles){
    // Mah Jong
    {
    CardCollection doubles1;
    doubles1.add_card(phoenix); doubles1.add_card(mah_jong);
    EXPECT_FALSE(doubles1.is_valid());
    }
    // Dragon
    {
    CardCollection doubles2;
    doubles2.add_card(phoenix); doubles2.add_card(dragon);
    EXPECT_FALSE(doubles2.is_valid());
    }
    // Any other
    {
    CardCollection doubles3;
    doubles3.add_card(phoenix); doubles3.add_card(Card(10,2));
    EXPECT_TRUE(doubles3.is_valid());
    PlayedCombo pc;
    EXPECT_TRUE(pc.can_be_played(doubles3));
    pc.set_cards(doubles3);
    CardCollection doubles4;
    doubles4.add_card(Card(11,1)); doubles4.add_card(Card(11,2));
    EXPECT_TRUE(pc.can_be_played(doubles4));
    }
}

TEST(PhoenixTests, Fullhouse){
    // Mah Jong
    {
    CardCollection fh;
    fh.add_card(phoenix); fh.add_card(mah_jong); 
    fh.add_card(Card(2,2)); fh.add_card(Card(2,1)); 
    fh.add_card(Card(2,3));
    EXPECT_FALSE(fh.is_valid());
    }
    // Dragon
    {
    CardCollection fh;
    fh.add_card(phoenix); fh.add_card(dragon); 
    fh.add_card(Card(2,2)); fh.add_card(Card(2,1)); 
    fh.add_card(Card(2,3));
    EXPECT_FALSE(fh.is_valid());
    }
    // Any other
    {
    PlayedCombo pc;
    CardCollection fh1;
    fh1.add_card(phoenix); fh1.add_card(Card(4,2)); 
    fh1.add_card(Card(2,2)); fh1.add_card(Card(2,1)); 
    fh1.add_card(Card(2,3));
    EXPECT_EQ(fh1.get_type(), "Fullhouse");
    pc.set_cards(fh1);
    CardCollection fh2;
    fh2.add_card(phoenix); fh2.add_card(Card(4,2)); 
    fh2.add_card(Card(3,2)); fh2.add_card(Card(3,1)); 
    fh2.add_card(Card(3,3));
    EXPECT_EQ(fh2.get_type(), "Fullhouse");
    EXPECT_TRUE(pc.can_be_played(fh2));
    pc.set_cards(fh2);
    CardCollection fh3;
    fh3.add_card(phoenix); fh3.add_card(Card(2,2)); 
    fh3.add_card(Card(4,2)); fh3.add_card(Card(4,1)); 
    fh3.add_card(Card(4,3));
    EXPECT_EQ(fh3.get_type(), "Fullhouse");
    EXPECT_TRUE(pc.can_be_played(fh3));
    pc.set_cards(fh3);
    CardCollection lower_fh;
    lower_fh.add_card(phoenix); lower_fh.add_card(Card(3,2)); 
    lower_fh.add_card(Card(3,2)); lower_fh.add_card(Card(4,1)); 
    lower_fh.add_card(Card(4,3));
    EXPECT_EQ(lower_fh.get_type(), "Fullhouse");
    EXPECT_FALSE(pc.can_be_played(lower_fh));
    }
    //              NOT FH
    {
        CardCollection not_fh;
        not_fh.add_card(phoenix);not_fh.add_card(Card(2,3));
        not_fh.add_card(Card(3,2));not_fh.add_card(Card(3,3));
        not_fh.add_card(Card(4,2));
        EXPECT_EQ(not_fh.get_type(), "");
        EXPECT_FALSE(not_fh.is_valid());
    }
}

TEST(PhoenixTests, Street){
    Card c_1_1(1,1); // Mah Jong
    Card c_2_2(2,2);
    Card c_3_1(3,1);
    Card c_4_1(4,1);
    Card c_5_1(5,1);
    Card c_6_1(6,1);
    Card c_7_1(7,1);
    Card c_8_1(8,1);
    Card c_9_1(9,1);
    Card c_10_1(10,1);
    Card c_11_3(11,3);
    Card c_12_1(12,1);
    Card c_13_1(13,1);
    Card c_14_1(14,1);

    //              CHECK STREET
    CardCollection low_street;
    low_street.add_card(c_1_1); low_street.add_card(c_2_2);
    low_street.add_card(c_5_1); low_street.add_card(c_4_1);
    low_street.add_card(phoenix);
    EXPECT_TRUE(low_street.is_valid());
    EXPECT_EQ(low_street.get_type(), "Street");

    CardCollection low_non_street;
    low_non_street.add_card(c_1_1); low_non_street.add_card(c_2_2);
    low_non_street.add_card(c_5_1); low_non_street.add_card(c_6_1);
    low_non_street.add_card(phoenix);
    EXPECT_FALSE(low_non_street.is_valid());
    EXPECT_EQ(low_non_street.get_type(), "");

    //              CHECK THAT PHOENIX STREET IS NOT TOO BIG!
    CardCollection high_street;
    high_street.add_card(c_14_1); high_street.add_card(c_13_1);
    high_street.add_card(c_12_1); high_street.add_card(c_11_3);
    high_street.add_card(c_10_1);
    PlayedCombo played;
    EXPECT_TRUE(played.can_be_played(high_street));
    played.set_cards(high_street);
    CardCollection not_higher_street;
    not_higher_street.add_card(c_14_1); not_higher_street.add_card(c_13_1);
    not_higher_street.add_card(phoenix); not_higher_street.add_card(c_11_3);
    not_higher_street.add_card(c_10_1);
    EXPECT_EQ(not_higher_street.get_type(), "Street");
    EXPECT_FALSE(played.can_be_played(not_higher_street));

    //              CHECK FOR TO LONG STREETS
    {
    CardCollection long_street;
    long_street.add_card(Card(2,2)); long_street.add_card(Card(3,2));
    long_street.add_card(Card(4,1)); long_street.add_card(Card(5,1));
    long_street.add_card(Card(6,1)); long_street.add_card(Card(7,1));
    long_street.add_card(Card(8,1)); long_street.add_card(Card(9,1));
    long_street.add_card(Card(10,1));long_street.add_card(Card(11,2));
    long_street.add_card(Card(12,1));long_street.add_card(Card(13,2));
    long_street.add_card(Card(14,1));
    EXPECT_EQ(long_street.get_type(), "Street");
    long_street.add_card(phoenix);
    EXPECT_FALSE(long_street.is_valid());
    }
    //              CHECK MAH Jong
    {
    CardCollection long_street2;
    long_street2.add_card(Card(1,1));long_street2.add_card(Card(3,2));
    long_street2.add_card(Card(4,1));long_street2.add_card(Card(5,1));
    long_street2.add_card(Card(6,1));long_street2.add_card(Card(7,1));
    long_street2.add_card(Card(8,1));long_street2.add_card(Card(9,1));
    long_street2.add_card(Card(10,1));
    long_street2.add_card(Card(11,2));
    long_street2.add_card(Card(12,1));
    long_street2.add_card(Card(13,2));
    long_street2.add_card(Card(14,1));
    long_street2.add_card(phoenix);
    EXPECT_EQ(long_street2.get_type(), "Street");
    long_street2.add_card(Card(2,1));
    EXPECT_FALSE(long_street2.is_valid());
    }
}

TEST(PhoenixTests, Tripple){
    {
    CardCollection tripple1;
    tripple1.add_card(Card(14,1));
    tripple1.add_card(Card(14,2));
    tripple1.add_card(phoenix);
    EXPECT_EQ(tripple1.get_type(), "Tripple");
    }
    {
    CardCollection tripple2;
    tripple2.add_card(phoenix);
    tripple2.add_card(Card(10,3));
    tripple2.add_card(Card(10,2));
    EXPECT_EQ(tripple2.get_type(), "Tripple");
    }
    {
    CardCollection tripple3;
    tripple3.add_card(Card(5,3));
    tripple3.add_card(phoenix);
    tripple3.add_card(Card(5,1));
    EXPECT_EQ(tripple3.get_type(), "Tripple");
    }
    {
    CardCollection not_tripple;
    not_tripple.add_card(Card(5,3));
    not_tripple.add_card(phoenix);
    not_tripple.add_card(Card(6,1));
    EXPECT_FALSE(not_tripple.is_valid());
    }
}

TEST(PhoenixTests, Following_Following_Doubles){
    //      Check Following Doubles
    {
    CardCollection follow1;
    follow1.add_card(Card(5,3));
    follow1.add_card(phoenix);
    follow1.add_card(Card(6,1));
    follow1.add_card(Card(6,2));
    EXPECT_EQ(follow1.get_type(), "Following_Doubles");
    PlayedCombo played;
    played.set_cards(follow1);
    CardCollection follow2;
    follow2.add_card(Card(8,3));
    follow2.add_card(phoenix);
    follow2.add_card(Card(7,1));
    follow2.add_card(Card(7,2));
    EXPECT_EQ(follow2.get_type(), "Following_Doubles");
    EXPECT_TRUE(played.can_be_played(follow2));
    CardCollection follow3;
    follow3.add_card(Card(8,3));
    follow3.add_card(phoenix);
    follow3.add_card(Card(9,3));
    follow3.add_card(Card(9,2));
    follow3.add_card(Card(10,3));
    follow3.add_card(Card(10,2));
    EXPECT_EQ(follow3.get_type(), "Following_Doubles");
    EXPECT_FALSE(played.can_be_played(follow3));
    }
    //      Check long Case
    {
    CardCollection long_follow;
    long_follow.add_card(Card(7,3));
    long_follow.add_card(phoenix);
    long_follow.add_card(Card(6,1));
    long_follow.add_card(Card(6,2));
    long_follow.add_card(Card(8,1));
    long_follow.add_card(Card(8,2));
    long_follow.add_card(Card(9,1));
    long_follow.add_card(Card(9,2));
    EXPECT_EQ(long_follow.get_type(), "Following_Doubles");
    }
    //      CHECK False Case
    {
    CardCollection non_follow;
    non_follow.add_card(Card(5,3));
    non_follow.add_card(Card(6,1));
    non_follow.add_card(Card(6,2));
    non_follow.add_card(Card(7,3));
    non_follow.add_card(phoenix);
    EXPECT_EQ(non_follow.get_type(), "");
    }
}
