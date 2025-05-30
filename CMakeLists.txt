cmake_minimum_required(VERSION 3.22)
include(FetchContent)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project("Eidolon" CXX)
option(EIDOLON_TESTS "Build tests for this->Eidolon" OFF)
if (EIDOLON_TESTS)
    include(CTest)
endif()

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG c1ab645ca298a2801097931d1079b10ff7eb9df8 # v5.5
)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG 55f93686c01528224f448c19128836e7df245f72 # v3.12.0
)

FetchContent_Declare(
    perlinnoise
    GIT_REPOSITORY https://github.com/Reputeless/PerlinNoise.git
    GIT_TAG 39ca74069057407a04c46e2ec06baf57c6fc4b3f # v3.0.0
)

FetchContent_MakeAvailable(raylib json perlinnoise)

add_executable(Eidolon
    "src/main.cpp"
    "src/Client.cpp"
    "src/GameState.cpp"
    "src/WorldData.cpp"
    "src/MenuState.cpp"
    "src/Interp.cpp"
    "src/SettingsState.cpp"
    "src/CreatureObject.cpp"
    "src/ItemObject.cpp"
    "src/GameObject.cpp"
    "src/PlayState.cpp"
    "src/World.cpp"
    "src/Player.cpp"
    "src/Events.h"
    "src/Inventory.cpp"
    "src/Inventory.h"
    "src/creatures/Creatures.h"
    "src/items/Items.h"
    "src/ui/Slider.cpp"
    "src/ui/ListMenu.cpp"
    "src/ui/HealthBar.cpp"
    "src/ui/TextInput.cpp"
    "src/ui/Button.cpp"
    "src/ui/MessageList.cpp"
    "src/ui/GameObjectList.cpp"
    "src/ui/SelectionList.cpp"
    "src/ui/StatScreen.cpp"
    "${json_SOURCE_DIR}/single_include/nlohmann/json.hpp"
    "${perlinnoise_SOURCE_DIR}/PerlinNoise.hpp"
)

set_property(TARGET Eidolon PROPERTY CXX_STANDARD 20)
target_link_libraries(Eidolon PUBLIC raylib)
target_include_directories(Eidolon PUBLIC
    "${raylib_SOURCE_DIR}/src/"
    "${json_SOURCE_DIR}/single_include/"
    "${perlinnoise_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/ui"
    "${PROJECT_SOURCE_DIR}/src/creatures"
    "${PROJECT_SOURCE_DIR}/src/items"
)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY settings.json DESTINATION ${CMAKE_BINARY_DIR})
if (APPLE)
    add_definitions(-DAPPLE)
endif()


if (EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
    message("[Eidolon] Copied compile_commands.json to project root")
    file(COPY
        "${CMAKE_BINARY_DIR}/compile_commands.json"
        DESTINATION
        ${PROJECT_SOURCE_DIR}
    )
endif()

# ?? Test setup (cleaned and unified)

if (EIDOLON_TESTS)
    add_executable(tests
        tests/Tests.cpp
        "src/Client.cpp"
        "src/GameState.cpp"
        "src/WorldData.cpp"
        "src/MenuState.cpp"
        "src/Interp.cpp"
        "src/SettingsState.cpp"
        "src/CreatureObject.cpp"
        "src/ItemObject.cpp"
        "src/GameObject.cpp"
        "src/PlayState.cpp"
        "src/World.cpp"
        "src/Player.cpp"
        "src/Events.h"
        "src/Inventory.cpp"
        "src/Inventory.h"
        "src/creatures/Creatures.h"
        "src/items/Items.h"
        "src/ui/Slider.cpp"
        "src/ui/ListMenu.cpp"
        "src/ui/HealthBar.cpp"
        "src/ui/TextInput.cpp"
        "src/ui/Button.cpp"
        "src/ui/MessageList.cpp"
        "src/ui/GameObjectList.cpp"
        "src/ui/SelectionList.cpp"
        "src/ui/StatScreen.cpp")
    target_include_directories(tests PUBLIC
        "${raylib_SOURCE_DIR}/src/"
        "${json_SOURCE_DIR}/single_include/"
        "${perlinnoise_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/src"
        "${PROJECT_SOURCE_DIR}/tests"
        "${PROJECT_SOURCE_DIR}/src/ui"
        "${PROJECT_SOURCE_DIR}/src/creatures"
        "${PROJECT_SOURCE_DIR}/src/items"
    )
    target_link_libraries(tests PUBLIC raylib)
    set_property(TARGET tests
        PROPERTY CXX_STANDARD 20
    )
    add_test(NAME Events COMMAND tests events)
    set_tests_properties(Events PROPERTIES
    PASS_REGULAR_EXPRESSION "${passRegex}"
    FAIL_REGULAR_EXPRESSION "${failRegex}"
    )
    add_test(NAME goblin_shout_setup
        COMMAND tests goblin_shout_setup
    )
    set_tests_properties(goblin_shout_setup PROPERTIES
        PASS_REGULAR_EXPRESSION "${passRegex}"
        FAIL_REGULAR_EXPRESSION "${failRegex}"
    )
endif()
