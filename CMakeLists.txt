cmake_minimum_required(VERSION 3.15)

project(iyoite)

set(CMAKE_CXX_STANDARD 20)

add_executable(iyo
	src/main.cpp
	src/ArenaAlloc.h
	src/ArgParser.cpp
	src/ArgParser.h
	src/Generation.cpp
	src/Generation.h
	src/Parser.cpp
	src/Parser.h
	src/Tokenizer.cpp
	src/Tokenizer.h
)

target_compile_definitions(iyo
PUBLIC
    $<$<CONFIG:Debug>:IYO_DEBUG>
    $<$<CONFIG:Release>:IYO_RELEASE>

    $<$<CONFIG:RelWithDebInfo>:IYO_DEBUG>
    $<$<CONFIG:MinSizeRel>:IYO_RELEASE>
)
