cmake_minimum_required(VERSION 3.5)
project(omega_client)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  visualization_msgs
  tf
)

catkin_package()

###########
## Build ##
###########

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(omega7_client src/omega_client.cpp)
target_link_libraries(omega7_client   ${catkin_LIBRARIES})
add_executable(testComm src/TestComm.cpp)
target_link_libraries(testComm ${catkin_LIBRARIES})
add_executable(testDiff src/TestDiff.cpp)
target_link_libraries(testDiff ${catkin_LIBRARIES})