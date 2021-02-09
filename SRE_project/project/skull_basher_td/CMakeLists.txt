# Add as a subdirectory SimpleRenderEngineProject using add_subdirectory
IF (NOT DEFINED all_libs)
    message( FATAL_ERROR "This CMakeLists.txt must be included using the command add_subdirectory(name)" )
ENDIF (NOT DEFINED all_libs)
# Ensure SimpleRenderEngineProject is up-to-date
IF (WIN32)
    IF (NOT DEFINED DLLFileList)
        message( FATAL_ERROR "The SimpleRenderEngineProject has been updated. Clone the project (https://github.com/mortennobel/SimpleRenderEngineProject) again the project again and start over." )
    ENDIF(NOT DEFINED DLLFileList)
ENDIF(WIN32)

# Add hpp and cpp files to Snake variable
file(GLOB SkullBasherTD
        "*.hpp"
        "*.h"
        "*.cpp"
        "./architecture/*.hpp"
        "./architecture/*.h"
        "./architecture/*.cpp"
        "./architecture/scenes/*.hpp"
        "./architecture/scenes/*.h"
        "./architecture/scenes/*.cpp"
        "./architecture/health/*.hpp"
        "./architecture/health/*.h"
        "./architecture/health/*.cpp"
        "./architecture/lifespans/*.hpp"
        "./architecture/lifespans/*.h"
        "./architecture/lifespans/*.cpp"
        "./architecture/physics/*.hpp"
        "./architecture/physics/*.h"
        "./architecture/physics/*.cpp"
        "./architecture/sound/*.hpp"
        "./architecture/sound/*.h"
        "./architecture/sound/*.cpp"
        "./architecture/music/*.hpp"
        "./architecture/music/*.h"
        "./architecture/music/*.cpp"
        "./architecture/collisions/*.hpp"
        "./architecture/collisions/*.h"
        "./architecture/collisions/*.cpp"
        )


# Compile cpp files (from Snake variable)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Debug")

find_package(SDL2 REQUIRED)

add_executable(skull_basher_td ${SkullBasherTD})

target_link_libraries(skull_basher_td ${all_libs} ${EXTRA_LIBS} SndFile::sndfile OpenAL)


# copy files to dest
file(COPY powerbar.png maps assets scripts data DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

IF (WIN32)

    file(COPY ${DLLFileList} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    #This does copy the files there
    file(COPY ${DLLFileList} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
   file(COPY ${DLLFileList} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)

ENDIF(WIN32)
