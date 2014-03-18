cmake_minimum_required( VERSION 2.8 )
project( tetris )

aux_source_directory( . tetris_SOURCES )
list( SORT tetris_SOURCES )

find_package( SDL REQUIRED )
exec_program( sdl-config ARGS "--libs" OUTPUT_VARIABLE SDL_OUTPUT )
message( STATUS "Linking with SDL flags: ${SDL_OUTPUT}" )

add_executable( tetris ${tetris_SOURCES} )

target_link_libraries( tetris ${SDL_OUTPUT} )
