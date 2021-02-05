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

#    ExternalProject_Get_Property(LibSndFile install_dir)
#    include_directories(${install_dir/include})

# Compile cpp files (from Snake variable)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Debug")

find_package(SDL2 REQUIRED)

add_executable(skull_basher_td ${SkullBasherTD})

target_link_libraries(skull_basher_td ${all_libs} ${EXTRA_LIBS} SndFile::sndfile OpenAL)



# copy files to dest
#file(COPY level0.json level0.png powerbar.png assets data DESTINATION Debug)
#file(COPY level0.json level0.png powerbar.png assets data DESTINATION Release)
#file(COPY level0.json level0.png DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
#file(COPY level0.json level0.png powerbar.png maps assets data DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
file(COPY level0.json level0.png powerbar.png maps assets data DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#file(COPY level0.json level0.png powerbar.png maps assets data DESTINATION ${TARGET_FILE_DIR}/Release)

#file(COPY ${DLLFileList} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/)
#file(COPY ${DLLFileList} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/)

#file(COPY level0.json level0.png DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

#Copy Asset files tp dest
#file(COPY ./assets DESTINATION Debug)
#file(COPY ./assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#file(COPY ./assets DESTINATION Release)
#file(COPY ./assets DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

#Copy Map files to dest
#file(COPY ./maps DESTINATION Debug)
#file(COPY ./maps DESTINATION Release)
#file(COPY ./maps DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})


IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
#    file(COPY ${DLLFileList} DESTINATION Debug)
#    file(COPY ${DLLFileList} DESTINATION Release)
#
    #file(COPY ${DLLFileList} DESTINATION .)
    file(COPY ${DLLFileList} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#    file(COPY ${DLLFileList} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY/Debug})

    #This does copy the files there
    file(COPY ${DLLFileList} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
   file(COPY ${DLLFileList} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)

    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    #set_target_properties(skull_basher_td PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Debug)
ENDIF(WIN32)
