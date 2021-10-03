cmake_minimum_required (VERSION 3.10)

project ("evilsocket" CXX)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(EVILSOCK_IMPL evilsocket.cc)
else()
    set(EVILSOCK_IMPL unix_evilsocket.cc)
endif()

set(EVILSOCK_SRC "evilsocket.h" "tcpclient.h" "tcpserver.h" ${EVILSOCK_IMPL})

add_library(${PROJECT_NAME} STATIC ${EVILSOCK_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_executable("evilsocket_client" "main_client.cc")
target_link_libraries(evilsocket_client PUBLIC ${PROJECT_NAME})

add_executable("evilsocket_server" "main_server.cc")
target_link_libraries(evilsocket_server PUBLIC ${PROJECT_NAME})