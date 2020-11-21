#IF (WIN32)
#	## Copy dll's to output path
#	file(COPY ${DLLFileList} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
#	file(COPY ${DLLFileList} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
#ENDIF(WIN32)

add_subdirectory(assignment_1_asteroids)
add_subdirectory(assignment_2_wolf)
add_subdirectory(assignment_3_bird)
add_subdirectory(assignment_4_platformer)
add_subdirectory(skull_basher_td)
