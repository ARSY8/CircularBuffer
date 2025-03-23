cmake_minimum_required(VERSION 3.10)

project(CircularBuffer)

# Задаем стандарт C++
set(CMAKE_CXX_STANDARD 23)

# Подключаем googletest
add_subdirectory(googletest)

# Создаем библиотеку CircularBuffer
add_library(
    CircularBuffer
    src/CircularBuffer.cpp
    src/CircularBuffer.hpp
)

# Создаем исполняемый файл с тестами
add_executable(
    CircularBufferTests 
    test/CircularBufferTests.cpp
)

# Подключаем заголовочные файлы
target_include_directories(
    CircularBufferTests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Линкуем тесты с библиотеками gtest и CircularBuffer
target_link_libraries(
    CircularBufferTests PRIVATE 
    gtest 
    gtest_main
    CircularBuffer
)


# Опции компиляции

target_compile_options(
    CircularBuffer PRIVATE
    -Wall -Wextra -pedantic -Werror
)

target_compile_options(
    CircularBufferTests PRIVATE
    -Wall -Wextra -pedantic -Werror
)

# if(MSVC){
#     set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

#     target_compile_options(
#         CircularBuffer PRIVATE
#         /W4
#     )

#     target_compile_options(
#         CircularBufferTests PRIVATE
#         /W4
#     )
# }
