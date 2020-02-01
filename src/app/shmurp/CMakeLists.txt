project(shmurp)

set(${PROJECT_NAME}_HEADERS
    game.h
)

set(${PROJECT_NAME}_SOURCES
    main.cpp
    game.cpp
)

find_package(2d CONFIG REQUIRED COMPONENTS engine)

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_HEADERS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ad::engine
)
