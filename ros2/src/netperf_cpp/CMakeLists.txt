cmake_minimum_required(VERSION 3.8)
project(netperf_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

message("TEST")
# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rcutils)
find_package(std_msgs REQUIRED)

include_directories(include)

#
# references
#  - https://github.com/ros2/demos/blob/galactic/action_tutorials/action_tutorials_cpp/CMakeLists.txt
#

#add_executable(talker_cpp src/talker_cpp.cpp)
add_library(talker_cpp_library SHARED src/talker_cpp.cpp)
target_compile_definitions(talker_cpp_library PRIVATE NETPERF_CPP_BUILDING_DLL)
ament_target_dependencies(talker_cpp_library
    rclcpp
    rcutils
    std_msgs)
target_include_directories(talker_cpp_library PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(talker_cpp_library PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
rclcpp_components_register_node(talker_cpp_library
  PLUGIN "netperf_cpp::Talker"
  EXECUTABLE talker_cpp)

#add_executable(listener_cpp src/listener_cpp.cpp)
add_library(listener_cpp_library SHARED src/listener_cpp.cpp)
target_compile_definitions(listener_cpp_library PRIVATE NETPERF_CPP_BUILDING_DLL)
ament_target_dependencies(listener_cpp_library
    rclcpp
    rcutils
    std_msgs)
target_include_directories(listener_cpp_library PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(listener_cpp_library PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
rclcpp_components_register_node(listener_cpp_library
  PLUGIN "netperf_cpp::Listener"
  EXECUTABLE listener_cpp)

install(TARGETS
	talker_cpp_library
	listener_cpp_library
  DESTINATION lib)

install(TARGETS
	talker_cpp
	listener_cpp
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
