# Top-Level CMakeList.txt

cmake_minimum_required (VERSION 2.6)
project ( RUN )

file(GLOB SOURCES
	src/*.h
    src/*.cpp
)

add_executable(RUN ${SOURCES})
add_definitions(-std=c++11)
target_compile_options(RUN PUBLIC -O3)

find_package (SDL)

if ( NOT SDL_FOUND )
   message ( FATAL_ERROR "SDL not found!" )
endif ( NOT SDL_FOUND )

if(SDL_FOUND)
	include_directories(
		${SDL_INCLUDE_DIR}
	)
endif(SDL_FOUND)

find_package(ASSIMP)

if(NOT ASSIMP_FOUND)
	message (FATAL_ERROR "ASSIMP not found!" )
endif(NOT ASSIMP_FOUND)

if(ASSIMP_FOUND)
	include_directories(
		${ASSIMP_INCLUDE_DIR}
	)
endif(ASSIMP_FOUND)

include_directories(
	${PROJECT_SOURCE_DIR}/include/
)
target_link_libraries(RUN ${SDL_LIBRARY})
target_link_libraries(RUN ${ASSIMP_LIBRARIES})


