cmake_minimum_required(VERSION 3.5)
project(rclcpp_mt_bug)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(subscribers subscribers.cpp)
ament_target_dependencies(subscribers rclcpp std_msgs)

add_executable(talker talker.cpp)
ament_target_dependencies(talker rclcpp std_msgs)

install(TARGETS
  subscribers
  talker
  DESTINATION lib/${PROJECT_NAME})

ament_package()
