cmake_minimum_required(VERSION 3.13)

project(Platform.Collections.Methods CXX)


add_library(${PROJECT_NAME}.Library INTERFACE)
target_include_directories(${PROJECT_NAME}.Library INTERFACE
	${PROJECT_NAME}
	"${CMAKE_CURRENT_SOURCE_DIR}/../../Interfaces/cpp/Platform.Interfaces/" )

add_executable(${PROJECT_NAME}.Tests.Console ${PROJECT_NAME}.Tests.Console/App.cpp)
set_target_properties(${PROJECT_NAME}.Tests.Console PROPERTIES CXX_STANDARD 20)
target_link_libraries(${PROJECT_NAME}.Tests.Console PRIVATE ${PROJECT_NAME}.Library)

