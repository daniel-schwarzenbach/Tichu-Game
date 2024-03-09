#include "CurrentStack.h"
#include "../exceptions/TichuException.h"

// Private Constructor for Deserialization
CurrentStack::CurrentStack( std::string id,
                serializable_value<int>* points, PlayedCombo on_display) :
        unique_serializable(id),
        _points(points),
        _cards(on_display)
{ }

// Public Constructor
CurrentStack::CurrentStack() : unique_serializable() {
    this->_points = new serializable_value<int>(0);
    this->_cards = PlayedCombo();
}

// Destructor
CurrentStack::~CurrentStack() { }

// Getters
int CurrentStack::get_points() {
    return _points->get_value();
}

std::vector<Card> CurrentStack::get_cards() {
    return this->_cards.get_cards();
}

// State Change
void CurrentStack::reset() {
    _points->set_value(0);
    _cards.remove_all_cards();
}

bool CurrentStack::play(CardCollection card_collection){
    if(this->_cards.can_be_played(card_collection)){ // Check if specified CardCollection can be played
        _cards.set_cards(card_collection); // Update the stack
        _points->set_value(_points->get_value() + card_collection.get_points()); // Update the points
        return true;
    } else{
        return false;
    }
}

// Serialization & Deserialization
void CurrentStack::write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const {
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value id_val(_id.c_str(), allocator);
    json.AddMember("id", id_val, allocator);

    rapidjson::Value points_val(rapidjson::kObjectType);
    _points->write_into_json(points_val, allocator);
    json.AddMember("points", points_val, allocator);

    rapidjson::Value on_display_val(rapidjson::kObjectType);
    _cards.write_into_json(on_display_val, allocator);
    json.AddMember("on_display", on_display_val, allocator);
}

CurrentStack CurrentStack::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id")
        && json.HasMember("on_display")
        && json.HasMember("points"))
    {
        return CurrentStack(
                json["id"].GetString(),
                serializable_value<int>::from_json(json["points"].GetObject()),
                PlayedCombo::from_json(json["on_display"].GetObject()));
    } else {
        throw TichuException("Failed to deserialize CurrentStack from json. Required json entries were missing.");
    }
}
