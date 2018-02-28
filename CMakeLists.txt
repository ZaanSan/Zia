cmake_minimum_required(VERSION 3.9)
project(cpp_zia)

if(WIN32)
    set(CMAKE_CXX_STANDARD 17)
    if (MSVC_VERSION GREATER_EQUAL "1900")
        include(CheckCXXCompilerFlag)
        CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
        if (_cpp_latest_flag_supported)
            add_compile_options("/std:c++latest")
        endif()
    endif()
else()
    set(CMAKE_CXX_COMPILER  "/usr/bin/g++-7")
    set(CMAKE_CXX_FLAGS "-std=c++1z")
endif()


if (NOT DEFINED BOOST_ROOT AND
        NOT DEFINED ENV{BOOST_ROOT})
    if (WIN32)
        set(BOOST_ROOT "C:/dev/libs/BOOST")
        set(Boost_USE_MULTITHREADED ON)
        set(Boost_USE_STATIC_RUNTIME OFF)
        set(Boost_USE_STATIC_LIBS OFF)
        add_definitions(-DBOOST_ALL_NO_LIB)
        add_definitions(-DBOOST_ALL_DYN_LINK)
    endif ()
endif ()
find_package(Boost 1.57.0 COMPONENTS system filesystem REQUIRED)
if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# Module Net
file(GLOB_RECURSE MODULE_NET Modules/Net/*)

add_library(Net SHARED
        ${MODULE_NET})

add_custom_command(
        TARGET Net
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Net> ../Modules/.)

# Module RequestParser
file(GLOB_RECURSE MODULE_REQUEST_PARSER Modules/RequestParser/*)

add_library(RequestParser SHARED
        ${MODULE_REQUEST_PARSER})

add_custom_command(
        TARGET RequestParser
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RequestParser> ../Modules/.)

# Module TreatRequest
file(GLOB_RECURSE MODULE_TREAT_REQUEST Modules/TreatRequest/*)

add_library(TreatRequest SHARED
        ${MODULE_TREAT_REQUEST})

add_custom_command(
        TARGET TreatRequest
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:TreatRequest> ../Modules/.)

# Module GenerateResponse
file(GLOB_RECURSE MODULE_GENERATE_RESPONSE Modules/GenerateResponse/*)

add_library(GenerateResponse SHARED
        ${MODULE_GENERATE_RESPONSE})

add_custom_command(
        TARGET GenerateResponse
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:GenerateResponse> ../Modules/.)

#Module Php
file(GLOB_RECURSE MODULE_GENERATE_RESPONSE Modules/Php/*)

add_library(Php SHARED
        ${MODULE_GENERATE_RESPONSE})

add_custom_command(
        TARGET Php
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Php> ../Modules/.)

# Zia executable
add_executable(cpp_zia
        api/conf.h
        api/http.h
        api/module.h
        api/net.h
        main.cpp
        Core/Core.cpp
        Core/Core.hpp
        Core/ILibrary.hpp
        Core/LibLoader.cpp
        Core/LibLoader.hpp
        Core/LibLoaderWindows.cpp
        Core/LibLoaderWindows.hpp
        Parser/json.hpp
        Core/ZiaException.cpp
        Core/ZiaException.hpp
        Core/ConfigHandler.cpp
        Core/ConfigHandler.hpp Core/ModuleManager.cpp Core/ModuleManager.hpp)

# Copy dependencies (Windows only)
if (WIN32)
    add_custom_command(
            TARGET cpp_zia
            POST_BUILD
            COMMAND $(CMAKE_COMMAND) -E copy ../Dependencies/boost_system-vc140-mt-1_58.dll ${CMAKE_CURRENT_BINARY_DIR}/boost_system-vc140-mt-1_58.dll)
endif()

if(Boost_FOUND)
    if (WIN32)
        target_link_libraries(cpp_zia ${Boost_LIBRARIES})
        target_link_libraries(Net ${Boost_LIBRARIES})
    else()
        target_link_libraries(cpp_zia ${Boost_LIBRARIES} -lpthread -ldl)
        target_link_libraries(Net ${Boost_LIBRARIES} -lpthread)
    endif()
endif()
