cmake_minimum_required(VERSION 3.5)

project(Tichu VERSION 0.1) # your project name

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(sockpp/include)

add_definitions(-DQT_NO_VERSION_TAGGING)

# define a variable CLIENT_SOURCE_FILES that contains the paths to all source files required to compile the client executable
set(CLIENT_SOURCE_FILES
        src/client/main.cpp
        src/client/app/Tichu.cpp src/client/app/Tichu.h
        src/client/GameController.cpp src/client/GameController.h

        #GraphicsViews
        src/client/uiElements/CardCollectionView.cpp src/client/uiElements/CardCollectionView.h
        src/client/uiElements/CardItem.cpp src/client/uiElements/CardItem.h

        # UI
        src/client/widgets/GameView.cpp src/client/widgets/GameView.h src/client/widgets/GameView.ui
        src/client/widgets/ConnectionView.cpp src/client/widgets/ConnectionView.h src/client/widgets/ConnectionView.ui
        src/client/widgets/TeamView.cpp src/client/widgets/TeamView.h src/client/widgets/TeamView.ui
        src/client/widgets/EndRoundView.cpp src/client/widgets/EndRoundView.h src/client/widgets/EndRoundView.ui
        src/client/widgets/CardSwapView.cpp src/client/widgets/CardSwapView.h src/client/widgets/CardSwapView.ui

        # network
        src/client/network/ClientNetworkManager.cpp src/client/network/ClientNetworkManager.h
        src/client/network/ResponseListenerThread.cpp src/client/network/ResponseListenerThread.h
        src/client/network/SignalHandler.cpp src/client/network/SignalHandler.h

        # game state
        src/common/game_state/CardCollection/Card.cpp src/common/game_state/CardCollection/Card.h
        src/common/game_state/CardCollection/CardCollection.cpp src/common/game_state/CardCollection/CardCollection.h
        src/common/game_state/CardCollection/DrawPile.cpp src/common/game_state/CardCollection/DrawPile.h
        src/common/game_state/CardCollection/PlayedCombo.cpp src/common/game_state/CardCollection/PlayedCombo.h
        src/common/game_state/Player.cpp src/common/game_state/Player.h
        src/common/game_state/CurrentStack.cpp src/common/game_state/CurrentStack.h
        src/common/game_state/Team.cpp src/common/game_state/Team.h
        src/common/game_state/GameState.cpp src/common/game_state/GameState.h
        # client requests
        src/common/network/requests/client_request.cpp src/common/network/requests/client_request.h
        src/common/network/requests/award_cards_request.cpp src/common/network/requests/award_cards_request.h
        src/common/network/requests/bomb_request.cpp src/common/network/requests/bomb_request.h
        src/common/network/requests/exit_game_request.cpp src/common/network/requests/exit_game_request.h
        src/common/network/requests/get_cards_request.cpp src/common/network/requests/get_cards_request.h
        src/common/network/requests/join_game_request.cpp src/common/network/requests/join_game_request.h
        src/common/network/requests/join_team_request.cpp src/common/network/requests/join_team_request.h
        src/common/network/requests/new_game_request.cpp src/common/network/requests/new_game_request.h
        src/common/network/requests/pass_request.cpp src/common/network/requests/pass_request.h
        src/common/network/requests/play_cards_request.cpp src/common/network/requests/play_cards_request.h
        src/common/network/requests/play_dragon_request.cpp src/common/network/requests/play_dragon_request.h
        src/common/network/requests/start_game_request.cpp src/common/network/requests/start_game_request.h
        src/common/network/requests/start_round_request.cpp src/common/network/requests/start_round_request.h
        src/common/network/requests/swap_cards_request.cpp src/common/network/requests/swap_cards_request.h
        src/common/network/requests/tichu_request.cpp src/common/network/requests/tichu_request.h
        # server responses
        src/common/network/responses/server_response.cpp src/common/network/responses/server_response.h
        src/common/network/responses/request_response.cpp src/common/network/responses/request_response.h
        src/common/network/responses/game_state_response.cpp src/common/network/responses/game_state_response.h
        # serialization
        src/common/serialization/serializable.h
        src/common/serialization/value_type_helpers.h
        src/common/serialization/vector_utils.h
        src/common/serialization/serializable_value.h
        src/common/serialization/json_utils.h
        src/common/serialization/uuid_generator.h
        src/common/serialization/unique_serializable.cpp src/common/serialization/unique_serializable.h src/client/widgets/EndGameView.cpp src/client/widgets/EndGameView.h src/client/widgets/EndGameView.ui)

# define a variable SERVER_SOURCE_FILES that contains the paths to all source files required to compile the server executable
set(SERVER_SOURCE_FILES
        src/server/main.cpp
        src/server/Game.cpp src/server/Game.hpp
        src/server/GameManager.cpp src/server/GameManager.hpp
        src/server/PlayerManager.cpp src/server/PlayerManager.hpp
        src/server/Server.cpp src/server/Server.hpp
        # game state
        src/common/game_state/CardCollection/Card.cpp src/common/game_state/CardCollection/Card.h
        src/common/game_state/CardCollection/CardCollection.cpp src/common/game_state/CardCollection/CardCollection.h
        src/common/game_state/CardCollection/DrawPile.cpp src/common/game_state/CardCollection/DrawPile.h
        src/common/game_state/CardCollection/PlayedCombo.cpp src/common/game_state/CardCollection/PlayedCombo.h
        src/common/game_state/Player.cpp src/common/game_state/Player.h
        src/common/game_state/CurrentStack.cpp src/common/game_state/CurrentStack.h
        src/common/game_state/Team.cpp src/common/game_state/Team.h
        src/common/game_state/GameState.cpp src/common/game_state/GameState.h
        # client requests
        src/common/network/requests/client_request.cpp src/common/network/requests/client_request.h
        src/common/network/requests/award_cards_request.cpp src/common/network/requests/award_cards_request.h
        src/common/network/requests/bomb_request.cpp src/common/network/requests/bomb_request.h
        src/common/network/requests/exit_game_request.cpp src/common/network/requests/exit_game_request.h
        src/common/network/requests/get_cards_request.cpp src/common/network/requests/get_cards_request.h
        src/common/network/requests/join_game_request.cpp src/common/network/requests/join_game_request.h
        src/common/network/requests/join_team_request.cpp src/common/network/requests/join_team_request.h
        src/common/network/requests/new_game_request.cpp src/common/network/requests/new_game_request.h
        src/common/network/requests/pass_request.cpp src/common/network/requests/pass_request.h
        src/common/network/requests/play_cards_request.cpp src/common/network/requests/play_cards_request.h
        src/common/network/requests/play_dragon_request.cpp src/common/network/requests/play_dragon_request.h
        src/common/network/requests/start_game_request.cpp src/common/network/requests/start_game_request.h
        src/common/network/requests/start_round_request.cpp src/common/network/requests/start_round_request.h
        src/common/network/requests/swap_cards_request.cpp src/common/network/requests/swap_cards_request.h
        src/common/network/requests/tichu_request.cpp src/common/network/requests/tichu_request.h
        # server responses
        src/common/network/responses/server_response.cpp src/common/network/responses/server_response.h
        src/common/network/responses/request_response.cpp src/common/network/responses/request_response.h
        src/common/network/responses/game_state_response.cpp src/common/network/responses/game_state_response.h
        # serialization
        src/common/serialization/serializable.h
        src/common/serialization/value_type_helpers.h
        src/common/serialization/vector_utils.h
        src/common/serialization/serializable_value.h
        src/common/serialization/json_utils.h
        src/common/serialization/uuid_generator.h
        src/common/serialization/unique_serializable.cpp src/common/serialization/unique_serializable.h src/server/RequestHandler.hpp src/server/RequestHandler.cpp)


# set source files for client-executable
add_executable(Tichu-client ${CLIENT_SOURCE_FILES})
# set compile directives for client-executable
target_compile_definitions(Tichu-client PRIVATE TICHU_CLIENT=1 RAPIDJSON_HAS_STDSTRING=1)
# link with Qt
target_link_libraries(Tichu-client Qt${QT_VERSION_MAJOR}::Widgets)
# Comment out if you don't want to print network-related messages into the console
target_compile_definitions(Tichu-client PRIVATE PRINT_NETWORK_MESSAGES=1)
#include uiElements for CMake
include_directories(${CMAKE_SOURCE_DIR}/client/uiElements)


# set source files for server-executable
add_executable(Tichu-server ${SERVER_SOURCE_FILES})
# set compile directives for server-executable
target_compile_definitions(Tichu-server PRIVATE TICHU_SERVER=1 RAPIDJSON_HAS_STDSTRING=1)
# Comment out if you don't want to print network-related messages into the console
target_compile_definitions(Tichu-server PRIVATE PRINT_NETWORK_MESSAGES=1)

# linking to sockpp
if(WIN32)
    message("Detected Win32")

    target_link_libraries(Tichu-client ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/sockpp-static.lib)
    target_link_libraries(Tichu-server ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/sockpp-static.lib)

    # Necessary to get sockets working under Windows (with MingW)
    target_link_libraries(Tichu-client wsock32 ws2_32)
elseif(APPLE)
    message("Apple detected")

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    target_link_libraries(Tichu-client ${CMAKE_SOURCE_DIR}/sockpp/build/libsockpp.dylib Threads::Threads)
    target_link_libraries(Tichu-server ${CMAKE_SOURCE_DIR}/sockpp/build/libsockpp.dylib Threads::Threads)
else()
    message("Not Win32 nor an Apple, so probably a Linux") # We assume it's Linux in this case

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    target_link_libraries(Tichu-client ${CMAKE_SOURCE_DIR}/sockpp/build/libsockpp.so Threads::Threads)
    target_link_libraries(Tichu-server ${CMAKE_SOURCE_DIR}/sockpp/build/libsockpp.so Threads::Threads)
endif()

# copy assets (images) to binary directory
file(INSTALL assets DESTINATION ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS "--coverage")
# set source files for server-library
add_library(Tichu-lib ${SERVER_SOURCE_FILES})
# set compile directives for server-library
target_compile_definitions(Tichu-lib PRIVATE TICHU_SERVER=1 RAPIDJSON_HAS_STDSTRING=1)

add_subdirectory(googletest)
add_subdirectory(unit-tests)
