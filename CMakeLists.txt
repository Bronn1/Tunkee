cmake_minimum_required(VERSION 3.14)
project(Tunkee)

set(GOOGLETEST_PROJECT_LOCATION D:/gtest/googletest)
set(SFML_ROOT D:/sfml/SFML-2.5.1)
set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")


#add_executable(Tunkee main.cpp)
add_subdirectory(src)
include_directories(.)
source_group("" FILES ${no_group_source_files})

set(Header_Files
        "src/tunkee_game.h"
        "src/core/game_rules.h"
        "src/core/data_types.h"
        "src/core/events.h"
        "src/core/unit_manager.h"
        "src/core/damage_calculator.h"
        "src/core/game_board.h"
        "src/core/game_command.h"
        "src/core/game_tile.h"
        "src/core/game_engine.h"
        "src/core/pathfinding.h"
        "src/core/player.h"
        "src/core/unit.h"
        "src/graphics/scene_node.h"
        "src/graphics/board_view.h"
        "src/graphics/tile_view.h"
        "src/gui/gui.h"
        "src/graphics/interface_unit_view.h"
        "src/graphics/tank_view.h"
        "src/graphics/game_tooltip.h"
        "src/game_builder.h"
        "src/graphics/graphics_data_types.h"
        "src/graphics/game_world_view.h"
        "src/controllers/game_controller.h"
        "src/graphics/unit_setup_view.h"
        "src/units_factory.h"
        "src/graphics/resource_holder.h"
        "src/core/damage_probability_table.h"
        "src/core/unit_damage_system_strategy.h"
        "src/graphics/explosion_effects.h"
        "src/graphics/projectile.h"
        "src/core/game_message.h"
        "src/core/unit_damage_common.h"
        "src/graphics/common.h"
        )
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
        "main.cpp"
        "src/tunkee_game.cpp"
        "src/core/game_rules.cpp"
        "src/core/data_types.cpp"
        "src/core/events.cpp"
        "src/core/unit_manager.cpp"
        "src/core/damage_calculator.cpp"
        "src/core/game_board.cpp"
        "src/core/game_command.cpp"
        "src/core/game_tile.cpp"
        "src/core/game_engine.cpp"
        "src/core/pathfinding.cpp"
        "src/core/player.cpp"
        "src/core/unit.cpp"
        "src/graphics/scene_node.cpp"
        "src/graphics/board_view.cpp"
        "src/graphics/tile_view.cpp"
        "src/gui/gui.cpp"
        "src/graphics/interface_unit_view.cpp"
        "src/graphics/tank_view.cpp"
        "src/graphics/game_tooltip.cpp"
        "src/game_builder.cpp"
        "src/graphics/game_world_view.cpp"
        "src/controllers/game_controller.cpp"
        "src/graphics/unit_setup_view.cpp"
        "src/units_factory.cpp"
        "src/graphics/resource_holder.cpp"
        "src/core/damage_probability_table.cpp"
        "src/core/unit_damage_system_strategy.cpp"
        "src/graphics/explosion_effects.cpp"
        "src/graphics/projectile.cpp"
         "src/core/game_message.cpp" 
        "src/core/unit_damage_common.cpp"
        "src/graphics/common.cpp"
        )
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES  ${Header_Files}  ${Source_Files})

add_executable(Tunkee ${ALL_FILES})
add_library(Tunkee_lib STATIC ${ALL_FILES})
#add_executable(Tunkee  ${ALL_FILES})
#set_target_properties(Tunkee PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
include_directories(headers /usr/include "${SFML_ROOT}/include")


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(Tunkee ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})


# Copy dlls to build
if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()

add_subdirectory(tests)
add_subdirectory( ${GOOGLETEST_PROJECT_LOCATION} build)
