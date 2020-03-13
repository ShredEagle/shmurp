project(shmurp VERSION "${CMAKE_PROJECT_VERSION}")

set(${PROJECT_NAME}_HEADERS
    commons.h
    Bloom.h
    BloomShaders.h
    DebugUI.h
    Entities.h
    Game.h
    Instancing.h
    ScreenQuad.h
    shapes.h
    shapes3D.h

    Components/Boundaries.h
    Components/FirePattern.h
    Components/ControlDevice.h
    Components/Geometry.h
    Components/Shape.h
    Components/Speed.h

    System/BulletSpawn.h
    System/Collision.h
    System/Displace.h
    System/EnemySpawn.h
    System/LimitPosition.h
    System/KeyboardControl.h
    System/Rendering.h
    System/Rendering3D.h
    System/RenderingShaders.h

    utils/interpolation.h
    utils/Periodic.h
)

set(${PROJECT_NAME}_SOURCES
    DebugUI.cpp
    Game.cpp

    System/BulletSpawn.cpp
    System/Collision.cpp
    System/Displace.cpp
    System/EnemySpawn.cpp
    System/LimitPosition.cpp
    System/Rendering.cpp
    System/Rendering3D.cpp
    System/KeyboardControl.cpp
)

find_package(aunteater CONFIG REQUIRED)
find_package(2d CONFIG REQUIRED COMPONENTS engine)
find_package(math CONFIG REQUIRED)
find_package(websocket CONFIG REQUIRED)

find_package(glad CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_HEADERS}
)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_options(${PROJECT_NAME}
        PRIVATE "-fno-omit-frame-pointer" "-fsanitize=address")
    target_link_options(${PROJECT_NAME}
        PRIVATE "-fsanitize=address")
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
