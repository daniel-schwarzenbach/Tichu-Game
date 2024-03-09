#include "GameState.h"

#include "../exceptions/TichuException.h"
#include "../serialization/vector_utils.h"

// Private Constructor
GameState::GameState(std::string id) : unique_serializable(id) {
    this->_stack = CurrentStack();
    this->_draw = DrawPile();
    this->_cards = std::vector<CardCollection>(4);
    this->_players = std::vector<Player>();
    this->_teams = std::vector<Team>();
    Team dragon("dragon");
    Team phoenix("phoenix");
    _teams.push_back(dragon);
    _teams.push_back(phoenix);
    this->_previousPlayerIdx = new serializable_value<int>(0);
    this->_currentPlayerIdx = new serializable_value<int>(0);
    this->_winningPlayerId = new serializable_value<std::string>("");
    this->_turnCounter = new serializable_value<int>(0);
    this->_roundCounter = new serializable_value<int>(1);
    this->_gamephase = new serializable_value<std::string>("team_view");
}

// Private Constructor for Deserialization
GameState::GameState(std::string id, CurrentStack stack, DrawPile draw, std::vector<CardCollection> cards,
                       std::vector<Player> players, std::vector<Team> teams, 
                       serializable_value<int> *previousPlayerIdx, serializable_value<int> *currentPlayerIdx, 
                       serializable_value<std::string>* winningPlayerId, serializable_value<std::string>* dragonTargetPlayerId,
                       serializable_value<int> *turnCounter, serializable_value<int> *roundCounter,
                       serializable_value<std::string> *gamephase)
        : unique_serializable(id),
        _stack(stack),
        _draw(draw),
        _cards(cards),
        _players(players),
        _teams(teams),
        _previousPlayerIdx(previousPlayerIdx),
        _currentPlayerIdx(currentPlayerIdx),
        _winningPlayerId(winningPlayerId),
        _dragonTargetPlayerId(dragonTargetPlayerId),
        _turnCounter(turnCounter),
        _roundCounter(roundCounter),
        _gamephase(gamephase)
{ }

// Public Constructor
GameState::GameState() : unique_serializable() {
    this->_stack = CurrentStack();
    this->_draw = DrawPile();
    this->_cards = std::vector<CardCollection>(4);
    this->_players = std::vector<Player>();
    this->_teams = std::vector<Team>();
    Team dragon("dragon");
    Team phoenix("phoenix");
    _teams.push_back(dragon);
    _teams.push_back(phoenix);
    this->_previousPlayerIdx = new serializable_value<int>(0);
    this->_currentPlayerIdx = new serializable_value<int>(0);
    this->_winningPlayerId = new serializable_value<std::string>("");
    this->_dragonTargetPlayerId = new serializable_value<std::string>("");
    this->_turnCounter = new serializable_value<int>(0);
    this->_roundCounter = new serializable_value<int>(1);
    this->_gamephase = new serializable_value<std::string>("team_view");
}

// Deconstructor
GameState::~GameState() {
        _previousPlayerIdx = nullptr;
        _currentPlayerIdx = nullptr;
        _winningPlayerId = nullptr;
        _dragonTargetPlayerId = nullptr;
        _turnCounter = nullptr;
        _roundCounter = nullptr;
        _gamephase = nullptr;
}

// Private State Change
void GameState::turn() {
    // Check if the round is finished
    if(round_is_finished() != "") {
        _players.at(_currentPlayerIdx->get_value()).add_points(_stack.get_points()); // Award the current stack to the 2nd last player.
        wrap_up_round();
        return;
    }

    do {
        // Increase turnCounter
        _turnCounter->set_value(_turnCounter->get_value() + 1);

        // Increase currentPlayerIdx
        if(_currentPlayerIdx->get_value() == 3){
            _currentPlayerIdx->set_value(0);
        } else {
            _currentPlayerIdx->set_value(_currentPlayerIdx->get_value() + 1);
        }

    } while (_players.at(_currentPlayerIdx->get_value()).get_handcards().size() == 0); // Skip players without handcards

    // Check if the player that played the last card won the current play
    if(_turnCounter->get_value() >= 4){
        wrap_up_play();
        _turnCounter->set_value(0);
    }    
}

bool GameState::is_current_player(int index) {
    if(_currentPlayerIdx->get_value() == index){ 
        return true;
    }
    return false;
}


bool GameState::is_current_player(Player player) {
    if(_players.at(_currentPlayerIdx->get_value()).get_id() == player.get_id()) {
        return true;
    }
    return false;
}

int GameState::get_player_index(Player player) {
    return get_player_index(player.get_id());
}

int GameState::get_player_index(std::string player_id) {
    // Search for specified player_id
    for(int i = 0; i < this->_players.size(); i++){
        if(this->_players.at(i).get_id() == player_id){
            return i;
        }
    } 

    TichuException("GameState::get_player_index reached end of function, which shouldn't happen. Check for error");
    return 0;
}

void GameState::order_player_vector() {
    std::string team;
    for(int i = 0; i < _players.size(); i++){
        // Change the teamname each iteration
        if(i%2 == 0) {
            team = "phoenix";
        } else {
            team = "dragon";
        }
        // Order the players so that a player with the specified teamname is at the next position
        for(int counter = i; counter < _players.size(); counter++){
            if(_players.at(counter).get_teamID() == team){
                std::swap(_players.at(i), _players.at(counter));
                break;
            }
        }
    }
}

std::string GameState::round_is_finished() {
    // Check how many players from which team finished already
    int players_finished = 0, dragon_finished = 0, phoenix_finished = 0;
    for(int i = 0; i < _players.size(); i++) {
        if(_players.at(i).get_handcards().size() == 0) {
            players_finished++;
            if(_players.at(i).get_teamID() == "phoenix"){
                phoenix_finished++;
            }
            if(_players.at(i).get_teamID() == "dragon"){
                dragon_finished++;
            }
        }
    }

    if(players_finished == 3) {
        // in that case a team didn't win completely, points need to be calculated
        return "shared";
    }

    if(dragon_finished == 2){
        return "dragon"; // In this case the team dragon won this round
    }

    if(phoenix_finished == 2){
        return "phoenix"; // In this case the team phoenix won this round
    }   

    return ""; // This means that round isn't finished yet. 
}

bool GameState::wrap_up_round(){
    std::string result = round_is_finished();
    if(result == ""){
        // In this case the round isn't finished yet
        return false;
    }

    // If a teams players finish first and second, points don't need to be calculated. The team just gets 100 points.
    if(result == "dragon") {
        _teams.at(0).add_points(200, _roundCounter->get_value());
    }

    if(result == "phoenix") {
        _teams.at(1).add_points(200, _roundCounter->get_value());
    }

    if(result == "shared") {
        for(int i = 0; i < _players.size(); i++){
            // Check if this player lost
            if(_players.at(i).get_hand().number_of_cards() != 0) {
                // Last player gives handcards to opponent team
                int opponent_team = 0;
                if(_teams.at(0).get_teamID() == _players.at(i).get_teamID()) {
                    opponent_team = 1;
                }
                _teams.at(opponent_team).add_points(_players.at(i).get_hand().get_points(), _roundCounter->get_value());

                // Last player gives won points to opponent team
                int winners_team = 1;
                if(_teams.at(0).get_teamID() == _players.at(get_player_index(_winningPlayerId->get_value())).get_teamID()) {
                    winners_team = 0;
                }
                _teams.at(winners_team).add_points(_players.at(i).get_points(), _roundCounter->get_value());

                // Reset this players points to 0
                _players.at(i).add_points(-_players.at(i).get_points());
            }  

            // Summarize points
            _teams.at((i+1)%2).add_points(_players.at(i).get_points(), _roundCounter->get_value());  
        }
    }

    // Hand out bonus/penalty points to players for their Tichus.
    // Small Tichu is worth 100 points
    // Big Tichu is worth 200 points
    for(int i = 0; i < _players.size(); i++) {
        if(_players.at(i).get_id() == _winningPlayerId->get_value()) { // If the player won give him bonus points
            if(_players.at(i).get_small_tichu()) {
                _players.at(i).add_points(100); 
                _teams.at((i+1)%2).add_points(100, _roundCounter->get_value());
            }
            if(_players.at(i).get_big_tichu()) {
                _players.at(i).add_points(200); 
                _teams.at((i+1)%2).add_points(200, _roundCounter->get_value());
            }
        } else{                                                         // If the player lost give him penalty points
            if(_players.at(i).get_small_tichu()) {
                _players.at(i).add_points(-100); 
                _teams.at((i+1)%2).add_points(-100, _roundCounter->get_value());
            }
            if(_players.at(i).get_big_tichu()) {
                _players.at(i).add_points(-200); 
                _teams.at((i+1)%2).add_points(-200, _roundCounter->get_value());
            }
        }
    }

    // Game is finished if a team has more than 1000 points
    if(_teams.at(0).get_points() >= 1000 || _teams.at(1).get_points() >= 1000) {
        _gamephase->set_value("end_game_view");
    } else {
        _gamephase->set_value("end_round_view"); 
    }

    // Ensure that both team point vectors hold a value
    _teams.at(0).add_points(0, _roundCounter->get_value());
    _teams.at(1).add_points(0, _roundCounter->get_value());

    // Increment round Counter
    _roundCounter->set_value(_roundCounter->get_value() + 1);

    return true;
}

bool GameState::wrap_up_play(){
    if(_turnCounter->get_value() >= 4){ // Check if current play is finished
        if(_dragonTargetPlayerId->get_value() == "") { // If this play was won with a dragon card, award the points to the specified player
            _players.at(_currentPlayerIdx->get_value()).add_points(_stack.get_points());
        } else {                                        // Otherwise give the points to the player that won the current play
            _players.at(get_player_index(_dragonTargetPlayerId->get_value())).add_points(_stack.get_points());
            _dragonTargetPlayerId->set_value("");
        }
        
        _stack.reset();

        return true;
    } else{
        return false;
    }
}

// Accessors
serializable_value<std::string>* GameState::get_gamephase(){
    return _gamephase;
}

serializable_value<int>* GameState::get_current_player_index(){
    return _currentPlayerIdx;
}

int GameState::get_number_of_players(){
    return _players.size();
}

std::vector<Player> GameState::get_players() {
    return _players;
}

std::vector<Player> GameState::get_players_ordered(Player player) {
    // Order the player list, so that the specified player is comes first and the other players are behind him in play order
    std::vector<Player> list;
    int index = get_player_index(player);

    for(int i = index; i < _players.size(); i++){
       list.push_back(this->_players.at(i));
    }

    for(int i = 0; i < index; i++){
       list.push_back(this->_players.at(i));
    }

    return list;
}

std::vector<Team> GameState::get_teams() {
    return _teams;
}

serializable_value<int>* GameState::get_round_counter(){
    return _roundCounter;
}

std::vector<Card> GameState::get_cards() {
    return _stack.get_cards();
}

int GameState::get_stack_points() {
    return _stack.get_points();
}

// State Changes
bool GameState::play(Player player, CardCollection cards, std::string& err){
    int player_index = get_player_index(player);

    if(is_current_player(player_index)){  // Established that this player is currently the player that's allowed to play.
        if(this->_stack.play(cards)){
            this->_players.at(player_index).remove_cards(cards); // Remove cards that the player played from his hand

            _turnCounter->set_value(0); // Reset counter, if counter exceeds max_nof_players this means, that someone won the stack.

            // Check if the player finished first
            if(_winningPlayerId->get_value() == "" && _players.at(_currentPlayerIdx->get_value()).get_handcards().size() == 0) { 
                _winningPlayerId->set_value(_players.at(_currentPlayerIdx->get_value()).get_id());
            }

            // If you played the dog, then skip the next player
            if(cards.try_get_card(Card(15,4))) {
                if(_currentPlayerIdx->get_value() == 3){
                    _currentPlayerIdx->set_value(0);
                } else {
                    _currentPlayerIdx->set_value(_currentPlayerIdx->get_value() + 1);
                }
                _turnCounter->set_value(_turnCounter->get_value() + 1);
            }

            turn();
            return true;
        } else {
            err = "You can't play these cards.";
            return false;
        }
    } else {
        err = "You are not the current player.";
        return false;
    }
}

bool GameState::play_dragon(Player player, CardCollection cards, Player other_player, std::string& err){
    _dragonTargetPlayerId->set_value(other_player.get_id()); // Save which other player is destined to eventually receive points from the current play
    return play(player, cards, err);    
}

bool GameState::pass(Player player, std::string& err){
     if(is_current_player(get_player_index(player))){ 
        turn();
        return true;
    } else {
        err = "You are not the current player.";
        return false;
    }
}

bool GameState::bomb(Player player, CardCollection cards, std::string& err){
    _previousPlayerIdx->set_value(_currentPlayerIdx->get_value()); // Save previous Player
    _currentPlayerIdx->set_value(get_player_index(player)); // Update current Player

    if(this->play(player, cards, err)){ // Let player try to play the bomb
        _dragonTargetPlayerId->set_value(""); // Overwrite if a dragon way played before
        return true;
    }
    
    _currentPlayerIdx->set_value(_previousPlayerIdx->get_value()); // Change current player back to last player
    return false;
}

bool GameState::tichu(Player player, std::string type, std::string& err){
    int player_idx = get_player_index(player);
    if(_players.at(player_idx).get_handcards().size() < 9){
        _players.at(player_idx).add_cards(_draw.get_rnd_card(6)); // Add cards to the players hand
        if(type == ""){ // We use this empty string to signify that the player declined to announce a big Tichu during swap_view
            return true;
        }
    }
    int player_index = get_player_index(player);
    return _players.at(player_index).tichu(type, err);
}

bool GameState::add_player_to_game(Player player){
    if(this->_players.size() < 4){ // No more than 4 players can join the game
        this->_players.push_back(player);
        return true;
    }
    return false;
}

bool GameState::swap_players(Player to_add, Player to_remove, 
        std::string& err
){
    for(int i = 0; i < _players.size(); ++i){
        if(_players.at(i).get_id() == to_remove.get_id()){
            _players.at(i) = to_add;
            return true;
        }
    }
    err = "couln't find player to remove";
    return false;
}

bool GameState::remove_player_from_game(Player player){
   for(int i = 0; i < this->_players.size(); i++){
        if(this->_players.at(i).get_id() == player.get_id()){
            this->_players.erase(_players.begin()+i);
            return true;
        }
    } 

    // You can't remove a player that isn't in the game
    return false;
}

bool GameState::add_player_to_team(Player player, std::string team){
    if(team != "dragon" && team != "phoenix" && team != ""){ // "" empty string means that the player didn't choose a team yet or deselected his previous team
        // specified team doesn't exist 
        return false;
    }

    // Count how many players are already in the specified team
    int counter = 0;
    for(int i = 0; i < this->_players.size(); i++){
        if(this->_players.at(i).get_teamID() == team){
            counter ++;
        }
    }

    if(counter < 2 || team == ""){ // Check if the team is already full
        for(int i = 0; i < this->_players.size(); i++){
            if(this->_players.at(i).get_id() == player.get_id()){
                
                this->_players.at(i).set_teamID(team);
                order_player_vector();
                return true;
            }
        }
    } else{
        // You can't join a team that's already full
        return false;
    }

    TichuException("Undefined behaviour in Gamestate::add_player_to_team()"); 
    return false;
}

bool GameState::start_game(){
    if (_players.size() >= 4 && (_gamephase->get_value() == "team_view" || _gamephase->get_value() == "end_round_view")) {
        _gamephase->set_value("switch_view");

        // Add 8 cards to each player
        _draw.initialize();
        for(int i = 0; i < 4; i++){
            _players.at(i).reset();
            _players.at(i).add_cards(_draw.get_rnd_card(8));
        }
        return true;
    } else {
        // Either there aren't enough players currently in the game or you are in the wrong gamephase
        return false;  
    }  
}

bool GameState::start_round(){
    if (_players.size() >= 4 && _gamephase->get_value() == "switch_view") {
        _gamephase->set_value("game_view");

        // distribute switched cards to the players
        for(int i = 0; i < _players.size() && _cards.size() != 0; i++) {
            _players.at(i).add_cards(_cards.at(i));
            _cards.at(i).remove_all_cards();
        }
        
        _stack.reset();
        // Player with Mah Jong can play first
        for(int i = 0; i < _players.size(); i++) {
            if(_players.at(i).get_hand().try_get_card(Card(1,1))) {
                _currentPlayerIdx->set_value(get_player_index(_players.at(i).get_id()));
            }
        }

        return true;
    } else {
        // Either there aren't enough players currently in the game or you are in the wrong gamephase
        return false;  
    }  
}

bool GameState::swap_cards(Player player, CardCollection cards, std::string& err) {
    int index = get_player_index(player);

    // Check if the player has these cards
    if(!_players.at(index).remove_cards(cards)) {
        err = "Player doesn't have these cards";
        return false;
    }
    
    // Check if the player already swapped cards
    if(_players.at(index).has_swapped()) {
        err = "Player has already swapped before";
        return false;
    }
    
    // Save the cards in the placeholder CardCollection until the round starts
    for(int i = 1; i < _players.size(); i++) {
        int cardIndex;
        if(index+i < _players.size()) {      
            cardIndex = index+i;
        } else{
            cardIndex = index+i-4;
        }
        _cards.at(cardIndex).add_card(cards.get_cards().at(i-1));
    }

    // Check if some players didn't exchange their cards
    for(int i = 0; i < _players.size(); i++) {
        if(_cards.at(i).get_cards().size() != 3){
            return true; // In this case there are some players that didnt exchange their cards yet
        }
    }

    // Because the for loop didn't return already, we can assume that all players exchanged their cards
    return start_round();
}


// Serialization
void GameState::write_into_json(rapidjson::Value &json,
                                 rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value stack_val(rapidjson::kObjectType);
    _stack.write_into_json(stack_val, allocator);
    json.AddMember("stack", stack_val, allocator);

    rapidjson::Value draw_val(rapidjson::kObjectType);
    _draw.write_into_json(draw_val, allocator);
    json.AddMember("draw", draw_val, allocator);

    json.AddMember("cards", vector_utils::serialize_vector(_cards, allocator), allocator);
    
    json.AddMember("players", vector_utils::serialize_vector(_players, allocator), allocator);

    json.AddMember("teams", vector_utils::serialize_vector(_teams, allocator), allocator);

    rapidjson::Value currentPlayerIdx_val(rapidjson::kObjectType);
    _currentPlayerIdx->write_into_json(currentPlayerIdx_val, allocator);
    json.AddMember("currentPlayerIdx", currentPlayerIdx_val, allocator);

    rapidjson::Value previousPlayerIdx_val(rapidjson::kObjectType);
    _previousPlayerIdx->write_into_json(previousPlayerIdx_val, allocator);
    json.AddMember("previousPlayerIdx", previousPlayerIdx_val, allocator);

    rapidjson::Value winningPlayerId_val(rapidjson::kObjectType);
    _winningPlayerId->write_into_json(winningPlayerId_val, allocator);
    json.AddMember("winningPlayerId", winningPlayerId_val, allocator);

    rapidjson::Value dragonTargetPlayerId_val(rapidjson::kObjectType);
    _dragonTargetPlayerId->write_into_json(dragonTargetPlayerId_val, allocator);
    json.AddMember("dragonTargetPlayerId", dragonTargetPlayerId_val, allocator);

    rapidjson::Value turnCounter_val(rapidjson::kObjectType);
    _turnCounter->write_into_json(turnCounter_val, allocator);
    json.AddMember("turnCounter", turnCounter_val, allocator);

    rapidjson::Value roundCounter_val(rapidjson::kObjectType);
    _roundCounter->write_into_json(roundCounter_val, allocator);
    json.AddMember("roundCounter", roundCounter_val, allocator);

    rapidjson::Value gamephase_val(rapidjson::kObjectType);
    _gamephase->write_into_json(gamephase_val, allocator);
    json.AddMember("gamephase", gamephase_val, allocator);
}

// Deserialization
GameState GameState::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id")
        && json.HasMember("stack")
        && json.HasMember("draw")
        && json.HasMember("cards")
        && json.HasMember("teams")
        && json.HasMember("players")
        && json.HasMember("previousPlayerIdx")
        && json.HasMember("currentPlayerIdx")
        && json.HasMember("winningPlayerId")
        && json.HasMember("dragonTargetPlayerId")
        && json.HasMember("turnCounter")
        && json.HasMember("roundCounter")
        && json.HasMember("gamephase"))
    {
        std::vector<CardCollection> deserialized_cards;
        for (auto &serialized_cards : json["cards"].GetArray()) {
            deserialized_cards.push_back(CardCollection::from_json(serialized_cards.GetObject()));
        }

        std::vector<Player> deserialized_players;
        for (auto &serialized_player : json["players"].GetArray()) {
            deserialized_players.push_back(Player::from_json(serialized_player.GetObject()));
        }
        std::vector<Team> deserialized_teams;
        for (auto &serialized_team : json["teams"].GetArray()) {
            deserialized_teams.push_back(Team::from_json(serialized_team.GetObject()));
        }
        return GameState(json["id"].GetString(),
                            CurrentStack::from_json(json["stack"].GetObject()),
                            DrawPile::from_json(json["draw"].GetObject()),
                            deserialized_cards,
                            deserialized_players,
                            deserialized_teams,
                            serializable_value<int>::from_json(json["previousPlayerIdx"].GetObject()),
                            serializable_value<int>::from_json(json["currentPlayerIdx"].GetObject()),
                            serializable_value<std::string>::from_json(json["winningPlayerId"].GetObject()),
                            serializable_value<std::string>::from_json(json["dragonTargetPlayerId"].GetObject()),
                            serializable_value<int>::from_json(json["turnCounter"].GetObject()),
                            serializable_value<int>::from_json(json["roundCounter"].GetObject()),
                            serializable_value<std::string>::from_json(json["gamephase"].GetObject()));


    } else {
        throw TichuException("Failed to deserialize GameState. Required entries were missing.");
    }
}

bool GameState::reset_game(std::string& err){
    _gamephase->set_value("team_view");
    return true;
}

