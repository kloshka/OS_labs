cmake_minimum_required(VERSION 3.10)

# Название проекта
project(DistributedSystem)

# Включаем поддержку C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавляем флаги компиляции 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

# Указываем исходные файлы
set(SOURCES
    client.cpp
    worker.cpp
)

# Заголовочные файлы
set(HEADERS
    node.h
    net_func.h
)

# Добавляем библиотеку ZeroMQ
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

# Создаем исполняемые файлы
add_executable(client client.cpp)
add_executable(worker worker.cpp)

# Подключаем заголовочные файлы
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(worker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Линкуем ZeroMQ
target_link_libraries(client PRIVATE ${ZMQ_LIBRARIES})
target_link_libraries(worker PRIVATE ${ZMQ_LIBRARIES})

# Указываем, что ZMQ необходим для сборки
target_include_directories(client PRIVATE ${ZMQ_INCLUDE_DIRS})
target_include_directories(worker PRIVATE ${ZMQ_INCLUDE_DIRS})
