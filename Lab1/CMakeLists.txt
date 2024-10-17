cmake_minimum_required(VERSION 3.10)

# Название проекта
project(OS)

# Установка стандарта C
set(CMAKE_C_STANDARD 99)

# Включаем GoogleTest через FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
  TLS_VERIFY false  # Не проверяем TLS сертификат (если нужно)
)
# Загружаем GoogleTest
FetchContent_MakeAvailable(googletest)

# Включаем тестирование в проекте
enable_testing()

# Добавление родительского процесса
add_executable(ParentProcess src/ParentProcess.c)

# Добавление дочернего процесса
add_executable(ChildProcess src/ChildProcess.c)


# Добавляем тестовый файл
add_executable(Tests test/test_example.cpp)

# Линкуем GoogleTest к тестам
target_link_libraries(Tests gtest gtest_main)

# Добавляем тесты
add_test(NAME MyTests COMMAND Tests)