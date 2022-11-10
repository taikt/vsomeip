cmake_minimum_required(VERSION 2.8)

set (COMMONAPI_DIR "/data001/tai2.tran/capicxx-core-runtime")
set (COMMONAPI_SOMEIP_DIR "/data001/tai2.tran/capicxx-someip-runtime")
set (VSOMEIP_DIR "/data001/tai2.tran/vsomeip")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++0x -Wl,--as-needed")

# directories for headers
include_directories(
    src-gen
    #$ENV{RUNTIME_PATH}/capicxx-core-runtime/include
    #$ENV{RUNTIME_PATH}/capicxx-someip-runtime/include
    #$ENV{RUNTIME_PATH}/vsomeip/interface
	# header for libCommonAPI.so
	${COMMONAPI_DIR}/include
	# header for libCommonAPI-SomeIP.so
	${COMMONAPI_SOMEIP_DIR}/include
	# header for libvsomeip3.so
	${VSOMEIP_DIR}/interface
)
# directories contain dependency libraries
link_directories(
    #$ENV{RUNTIME_PATH}/capicxx-core-runtime/build
    #$ENV{RUNTIME_PATH}/capicxx-someip-runtime/build
    #$ENV{RUNTIME_PATH}/vsomeip/build
	# locate libCommonAPI.so 
	${COMMONAPI_DIR}/build
	# locate libCommonAPI-SomeIP.so
	${COMMONAPI_SOMEIP_DIR}/build
	# locate libvsomeip3.so
	${VSOMEIP_DIR}/build
)

add_executable(HelloWorldClient
	src/E01HelloWorldClient.cpp
	src-gen/v0/commonapi/examples/E01HelloWorldSomeIPProxy.cpp
	src-gen/v0/commonapi/examples/E01HelloWorldSomeIPDeployment.cpp
)
# link depended libraries to binary 'HelloWorldClient'
# vsomeip3 => means libvsomeip3.so
target_link_libraries(HelloWorldClient CommonAPI CommonAPI-SomeIP vsomeip3)

add_executable(HelloWorldService
	src/E01HelloWorldService.cpp
	src/E01HelloWorldStubImpl.cpp
	src-gen/v0/commonapi/examples/E01HelloWorldSomeIPStubAdapter.cpp
	src-gen/v0/commonapi/examples/E01HelloWorldSomeIPDeployment.cpp
)
target_link_libraries(HelloWorldService CommonAPI CommonAPI-SomeIP vsomeip3)

