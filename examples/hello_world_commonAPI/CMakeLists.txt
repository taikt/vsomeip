cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++0x")
include_directories(
    src-gen
    $ENV{RUNTIME_PATH}/capicxx-core-runtime/include
    $ENV{RUNTIME_PATH}/capicxx-someip-runtime/include
    $ENV{RUNTIME_PATH}/vsomeip/interface
	/usr/local/include/CommonAPI-3.2
)
link_directories(
    $ENV{RUNTIME_PATH}/capicxx-core-runtime/build
    $ENV{RUNTIME_PATH}/capicxx-someip-runtime/build
    $ENV{RUNTIME_PATH}/vsomeip/build
)
add_executable(HelloWorldClient
	src/E01HelloWorldClient.cpp
	src-gen/v0/commonapi/examples/E01HelloWorldSomeIPProxy.cpp
	src-gen/v0/commonapi/examples/E01HelloWorldSomeIPDeployment.cpp
)
target_link_libraries(HelloWorldClient CommonAPI CommonAPI-SomeIP vsomeip3)
add_executable(HelloWorldService
	src/E01HelloWorldService.cpp
	src/E01HelloWorldStubImpl.cpp
	src-gen/v0/commonapi/examples/E01HelloWorldSomeIPStubAdapter.cpp
	src-gen/v0/commonapi/examples/E01HelloWorldSomeIPDeployment.cpp
)
target_link_libraries(HelloWorldService CommonAPI CommonAPI-SomeIP vsomeip3)
