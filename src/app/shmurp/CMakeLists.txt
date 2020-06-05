project(shmurp VERSION "${CMAKE_PROJECT_VERSION}")

set(${PROJECT_NAME}_HEADERS
    commons.h
    Bloom.h
    BloomShaders.h
    Boss1.h
    DebugUI.h
    Game.h
    Instancing.h
    ScreenQuad.h
    shapes.h
    shapes3D.h

    Components/Boundaries.h
    Components/ControlDevice.h
    Components/CustomCallback.h
    Components/FirePattern.h
    Components/Geometry.h
    Components/SceneGraphComposite.h
    Components/SceneGraphParent.h
    Components/Shape.h
    Components/Speed.h
    Components/TrackerPlayer.h
    Components/Tweening.h

    Entities/Bullets.h
    Entities/Ships.h

    System/BulletSpawn.h
    System/Collision.h
    System/Customizer.h
    System/Displace.h
    System/EnemySpawn.h
    System/Interpolate.h
    System/Interpolate-impl.h
    System/LimitPosition.h
    System/KeyboardControl.h
    System/PruneEntities.h
    System/Rendering.h
    System/Rendering3D.h
    System/RenderingShaders.h
    System/SceneGraph.h
    System/Tracking.h

    utils/interpolation.h
    utils/Periodic.h
    utils/Rythm.h
    utils/TimedSequence.h
)

set(${PROJECT_NAME}_SOURCES
    DebugUI.cpp
    Game.cpp

    System/BulletSpawn.cpp
    System/Collision.cpp
    System/Customizer.cpp
    System/Displace.cpp
    System/EnemySpawn.cpp
    System/LimitPosition.cpp
    System/PruneEntities.cpp
    System/Rendering.cpp
    System/Rendering3D.cpp
    System/KeyboardControl.cpp
    System/SceneGraph.cpp
    System/Tracking.cpp
)

find_package(Aunteater CONFIG REQUIRED COMPONENTS aunteater)
find_package(Graphics CONFIG REQUIRED COMPONENTS engine)
find_package(Math CONFIG REQUIRED COMPONENTS math)
find_package(Websocket CONFIG REQUIRED COMPONENTS websocket)

find_package(glad CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_HEADERS}
)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_options(${PROJECT_NAME}
        PRIVATE "-fno-omit-frame-pointer" "-fsanitize=address" "-fsanitize=undefined")
    target_link_options(${PROJECT_NAME}
        PRIVATE "-fsanitize=address" "-fsanitize=undefined")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ad::aunteater
        ad::engine
        ad::math
        ad::websocket

        glad::glad
)

set_target_properties(${PROJECT_NAME} PROPERTIES
                      VERSION "${${PROJECT_NAME}_VERSION}"
)


##
## Install
##

install(TARGETS ${PROJECT_NAME})
