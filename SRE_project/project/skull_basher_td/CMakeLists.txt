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
        )

# Compile cpp files (from Snake variable)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Debug")
add_executable(skull_basher_td ${SkullBasherTD})
target_link_libraries(skull_basher_td ${all_libs})



# copy files to dest
file(COPY level0.json level0.png powerbar.png assets data DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Debug)
file(COPY level0.json level0.png powerbar.png assets data DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Release)
#file(COPY level0.json level0.png DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

#Copy Asset files tp dest
file(COPY ./assets DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Debug)
file(COPY ./assets DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Release)
#file(COPY ./assets DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

#Copy Map files to dest
file(COPY ./maps DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Debug)
file(COPY ./maps DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Release)
#file(COPY ./maps DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})


IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)
    #file(COPY ${DLLFileList} DESTINATION .)

    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    set_target_properties(skull_basher_td PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Debug)
ENDIF(WIN32)
