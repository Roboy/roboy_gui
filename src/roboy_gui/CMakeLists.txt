cmake_minimum_required(VERSION 2.8.3)
project(roboy_gui)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devel/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devel/${PROJECT_NAME})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -fPIC")
SET(CMAKE_BUILD_TYPE Debug)


#==============================================================================
# Dependencies
#==============================================================================
## Find CATKIN/ROS
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  common_utilities
)


# includes in corresponding build directories
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
#set(CMAKE_AUTOMOC ON)

## Find Qt5
find_package(Qt5 CONFIG REQUIRED COMPONENTS
             Core
	         Widgets	
	         Quick)


add_subdirectory(src)

message(Include Dirs:\n${ROBOY_INCLUDE_DIRS})
message(Headers Found:\n${ROBOY_HEADERS})
message(Sources Found:\n${ROBOY_SOURCES})

## Dummy target to add non-source files to Qt-Project Viewer
file(GLOB ETC ${CMAKE_CURRENT_SOURCE_DIR}/etc/*)
file(GLOB DB  ${CMAKE_CURRENT_SOURCE_DIR}/database/*)
file(GLOB DB  ${CMAKE_CURRENT_SOURCE_DIR}/msg/*)
add_custom_target(dummy_target SOURCES ${ROBOY_HEADERS} ${ETC} ${DB})

## Generate Qt-Files (Mocs for Headers)
qt5_wrap_cpp(MOC_HDRS ${ROBOY_HEADERS})

if(NOT DEFINED ENV{MATERIAL_DIR)
    message(STATUS "env variable MATERIAL_DIR not set, falling back to default")
    set(ENV{MATERIAL_DIR} "~/Downloads/qml-material" )
endif(NOT DEFINED ENV{MATERIAL_DIR)

## Adding Material resources
qt5_add_resources(MOC_RESOURCES ${ROBOY_RESOURCES}
  								$ENV{MATERIAL_DIR}/modules/FontAwesome.qrc
  								$ENV{MATERIAL_DIR}/modules/FontRoboto.qrc
  								$ENV{MATERIAL_DIR}/modules/Icons.qrc
  								$ENV{MATERIAL_DIR}/modules/Material.qrc
 						 		$ENV{MATERIAL_DIR}/modules/MaterialQtQuick.qrc)

catkin_package(
   CATKIN_DEPENDS message_runtime common_utilities
)

## Build Target
include_directories(
        include
        ${ROBOY_INCLUDE_DIRS}
        ${catkin_INCLUDE_DIRS}
        ${Qt5Widgets_INCLUDE_DIRS}
		${Qt5Core_INCLUDE_DIRS}
        ${Qt5Quick_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
)

#==============================================================================
# Application
#==============================================================================
add_executable(roboy_control_node ${ROBOY_SOURCES} ${MOC_HDRS} ${MOC_RESOURCES})
qt5_use_modules(roboy_control_node Core Widgets Quick)
target_link_libraries(roboy_control_node ${catkin_LIBRARIES} )

#==============================================================================
# Installation
#==============================================================================
install(TARGETS
        roboy_control_node
	    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(FILES
        package.xml
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
