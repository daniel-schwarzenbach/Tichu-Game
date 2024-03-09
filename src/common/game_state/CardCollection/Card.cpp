#include "Card.h"
#include "../../exceptions/TichuException.h"

// Deserialization Constructor
Card::Card(std::string id, serializable_value<int> *type, serializable_value<int> *family)
        : unique_serializable(id), _type(type), _family(family)
{ 
    if((type->get_value() < 2 || type->get_value() > 15 || family->get_value() < 1 || family->get_value() > 4) && !(type->get_value() == 1 && family->get_value() == 1) && !(type == 0 && family == 0)){
        throw TichuException("Type of Card must be within 2 - 15. Family of Card must be within 1 - 4. Exception only for MahJong which is of type and family 1.");
    }
}

// Public Constructors
Card::Card(int type, int family) :
        unique_serializable(),
        _type(new serializable_value<int>(type)),
        _family(new serializable_value<int>(family))
{ 
    if((type < 2 || type > 15 || family < 1 || family > 4) && !(type == 1 && family == 1) && !(type == 0 && family == 0)){
        throw TichuException("Type of Card must be within 2 - 15. Family of Card must be within 1 - 4. Exception only for MahJong which is of type and family 1.");
    }
}

Card::Card() :
        unique_serializable(),
        _type(new serializable_value<int>(0)),
        _family(new serializable_value<int>(0))
{ }

// Deconstructor
Card::~Card() { }

// Accessors
int Card::get_type() const noexcept {
    return _type->get_value();
}

int Card::get_family() const noexcept {
    return _family->get_value();
}

bool Card::isEmptyCard(){
    if (_family->get_value() == 0 && _type->get_value()== 0){ // We defined that a card with type and family 0 to be a placeholder card
        return true;
    }

    return false;
}

// Serialization & Deserialization
Card Card::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id") && json.HasMember("type") && json.HasMember("family")) {
        return Card(json["id"].GetString(), serializable_value<int>::from_json(json["type"].GetObject()), serializable_value<int>::from_json(json["family"].GetObject()));
    }
    
    throw TichuException("Could not parse json of Card. Was missing 'id' or 'type' or 'family'.");
}

void Card::write_into_json(rapidjson::Value &json, rapidjson::Document::AllocatorType& allocator) const {
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value value_type(rapidjson::kObjectType);
    _type->write_into_json(value_type, allocator);
    json.AddMember("type", value_type, allocator);

    rapidjson::Value value_family(rapidjson::kObjectType);
    _family->write_into_json(value_family, allocator);
    json.AddMember("family", value_family, allocator);
}

bool Card::operator==(Card const& other) const{
    if(this->get_type() == other.get_type() && this->get_family() == other.get_family()){
        return true;
    } else{
        return false;
    }
}

bool Card::operator!=(Card const& other) const{
    return !(*this == other);
}