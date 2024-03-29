# Com's CMakeLists

#
# This submodule contains all the stuff necessary to play with messages
#

# Get together all the source files and all the headers
file(GLOB SOURCES src/*.cpp src/*.cc)
file(GLOB EXPORTABLEHEADERS include/*.hpp include/*.h)

# Required libraries
find_package(Boost REQUIRED COMPONENTS system)
find_package(Protobuf REQUIRED)
find_package(ZMQ REQUIRED)

# Helpful debug stuff?
message("-- Protobuf binary found at ${PROTOBUF_PROTOC_EXECUTABLE}")

# Generate the protobuf messages
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ./protobuf_messages/version1.proto)

# The -I flag for the compiler..
include_directories(
		"include"
		${CMAKE_CURRENT_BINARY_DIR} # This is needed for protobuf's messages 
		${ZMQ_INCLUDE_DIRS}
		${PROTOBUF_INCLUDE_DIRS}
		${Boost_INCLUDE_DIRS})

# Library compilation and linking
add_library(rosk_com SHARED ${SOURCES} ${PROTO_SRCS})
target_link_libraries(rosk_com
		${ZMQ_LIBRARIES}
		${PROTOBUF_LIBRARIES}
    ${Boost_LIBRARIES})

# Where to install stuff, protobuf messages generated H included
install(TARGETS rosk_com LIBRARY DESTINATION lib)
install(FILES ${EXPORTABLEHEADERS} DESTINATION include)
install(FILES ${PROTO_HDRS} DESTINATION include)

# Test suite
enable_testing()
add_executable(test_basics "test/test_basics.cpp")
add_executable(test_zmq_socket "test/test_zmq_socket.cpp")
target_link_libraries(test_basics rosk_com)
target_link_libraries(test_zmq_socket rosk_com)

add_test(test_basics ${CMAKE_CURRENT_BINARY_DIR}/test_basics)
add_test(test_zmq_socket ${CMAKE_CURRENT_BINARY_DIR}/test_zmq_socket)
