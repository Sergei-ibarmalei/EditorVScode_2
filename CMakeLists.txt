cmake_minimum_required(VERSION 3.27.4)

project(Editor)
set(SOURCES
    src/_coloredtable.cpp 
    src/_table.cpp 
    src/area.cpp 
    src/editor.cpp  
    src/loads.cpp 
    src/pixel.cpp 
    src/rect.cpp 
    src/sdltype.cpp
    src/maincross.cpp
    src/filedata.cpp
    main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    SDL2)
    #SDL2_image)

target_compile_options(${PROJECT_NAME} PRIVATE
$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>)
#$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-O3 -Wall -Wextra -Wpedantic >)



