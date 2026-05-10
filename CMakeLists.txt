
cmake_minimum_required(VERSION 3.15)
project(Ships)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    #��� /utf-8 � ���� ��������
    add_compile_options(/utf-8)
endif()
###############
set(PDCURSES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/PDcurses)


# ��� .cpp ����� �� src
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp" "src/**/**/*.cpp")
file(GLOB HEADERS "include/*.h" "include/**/*.h" "include/**/**/*.h")
add_executable(${PROJECT_NAME} ${SOURCES} )
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    ${PDCURSES_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${PDCURSES_DIR}/lib/pdcurses.lib
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PDCURSES_DIR}/lib/pdcurses.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

#if(WIN32)
#    # Находим PDCurses
#    find_package(PDCurses REQUIRED)
#    
#    # Подключаем библиотеку
#    target_link_libraries(${PROJECT_NAME} PRIVATE PDCurses::PDCurses)
#    
#    # Добавляем опции Windows
#    target_compile_definitions(${PROJECT_NAME} PRIVATE
#        NOMINMAX
#        WIN32_LEAN_AND_MEAN
#    )
#endif()
#
## Linux-specific настройки
#if(UNIX AND NOT APPLE)
#    find_package(Curses REQUIRED)
#    target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARIES})
#    target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIRS})
#endif()
