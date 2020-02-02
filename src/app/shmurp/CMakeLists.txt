project(shmurp)

set(${PROJECT_NAME}_HEADERS
    commons.h
    Game.h
    shapes.h

    Components/Position.h

    System/KeyboardControl.h
    System/Rendering.h
    System/RenderingShaders.h
)

set(${PROJECT_NAME}_SOURCES
    Game.cpp

    System/Rendering.cpp
    System/KeyboardControl.cpp
)

find_package(aunteater CONFIG REQUIRED)
find_package(2d CONFIG REQUIRED COMPONENTS engine)
find_package(math CONFIG REQUIRED)

find_package(glad CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ad::aunteater
        ad::engine
        ad::math

        glad::glad
)
