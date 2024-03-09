#include "gtest/gtest.h"
#include "../src/common/exceptions/TichuException.h"
#include "../src/common/game_state/Team.h"
#include "../src/common/serialization/json_utils.h"


// Serialization and subsequent deserialization must yield the same object
TEST(TeamTest, SerializationEquality) {
    Team team_send("sdfffdsds3242fa");
    team_send.add_points(132,1);
    team_send.add_points(-1234,2);
    team_send.add_points(0,3);
    rapidjson::Document* json_send = team_send.to_json();
    std::string message = json_utils::to_string(json_send);
    delete json_send;

    rapidjson::Document json_recv = rapidjson::Document(rapidjson::kObjectType);
    json_recv.Parse(message.c_str());
    Team team_recv = Team::from_json(json_recv);
    EXPECT_EQ(team_send.get_id(), team_recv.get_id());
    EXPECT_EQ(team_send.get_points(), team_recv.get_points());
    EXPECT_EQ(team_send.get_teamID(), team_recv.get_teamID());
    std::vector<serializable_value<int>> points_recv = team_recv.get_points_vector();
    EXPECT_EQ(points_recv.at(0).get_value(), 132);
    EXPECT_EQ(points_recv.at(1).get_value(), -1234);
    EXPECT_EQ(points_recv.at(2).get_value(), 0);
}

// Deserializing an invalid string must throw a TichuException
TEST(TeamTest, SerializationException) {
    rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
    json.Parse("not json");
    EXPECT_THROW(Team::from_json(json), TichuException);
}
