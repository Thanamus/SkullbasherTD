
cmake_minimum_required(VERSION 3.8)
project(SimpleRenderEngineProject CXX)

set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

IF (UNIX)
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
ELSE (UNIX)
    SET(SDL2_INCLUDE "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2-2.0.9/include" CACHE STRING "Where the SDL2 headers can be found")
    SET(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2-2.0.9/include" CACHE STRING "Where the SDL2 headers can be found")
    include_directories( ${SDL2_INCLUDE})
	
    SET(SDL2_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2-2.0.9/lib/x86/SDL2.lib" )
ENDIF (UNIX)

#########################################################
# FIND OPENGL
#########################################################
IF(APPLE)
    INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
    FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
    MARK_AS_ADVANCED (OpenGL_LIBRARY)
    SET(EXTRA_LIBS ${OpenGL_LIBRARY})
ELSE (APPLE)
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})
    IF(UNIX)
        find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIR})
    ELSE(UNIX)
		SET(GLEW_include "${CMAKE_SOURCE_DIR}/thirdparty/Win/glew-2.1.0/include" CACHE STRING "Where the glew.h can be found")
        SET(GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/Win/glew-2.1.0/include" CACHE STRING "Where the glew.h can be found")
        include_directories(${GLEW_include})
		SET(GLEW_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/Win/glew-2.1.0/lib/Release/Win32/glew32s.lib" CACHE STRING "Where the glew lib(static) can be found")  
    ENDIF(UNIX)
    
    SET(EXTRA_LIBS ${OPENGL_LIBRARY} ${GLEW_LIBRARY})
ENDIF (APPLE)

IF (UNIX)
    find_package(SDL2_IMAGE REQUIRED)
    include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

    find_package(SDL2_mixer REQUIRED)
    include_directories(${SDL2MIXER_INCLUDE_DIR})
ELSE (UNIX)
    SET(SDL2_image_include "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_image-2.0.4/include" CACHE STRING "Where the SDL_image.h can be found")
    SET(SDL2_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_image-2.0.4/include" CACHE STRING "Where the SDL_image.h can be found")
	
	include_directories(${SDL2_image_include})
	SET(SDL2_IMAGE_LIBRARIES "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_image-2.0.4/lib/x86/SDL2_image.lib" CACHE STRING "Location of SDL2_image.lib")
	SET(SDL2_image_lib_path "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_image-2.0.4/lib/x86" CACHE STRING "Location of SDL2_image.lib")
	
	SET(SDL2_mixer_include "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_mixer-2.0.4/include" CACHE STRING "Location of SDL_mixer.h")
    include_directories(${SDL2_mixer_include})
	SET(SDL2MIXER_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_mixer-2.0.4/lib/x86/SDL2_mixer.lib" CACHE STRING "Location of SDL2_mixer.lib")  
ENDIF(UNIX)

#### Box2D #####
IF(APPLE)
    INCLUDE_DIRECTORIES ( thirdparty/macOS/Box2D/include )
    SET(Box2D_LIBRARY_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/macOS/Box2D/lib/libBox2D.a )
	SET(Box2D_LIBRARY_REL ${CMAKE_SOURCE_DIR}/thirdparty/macOS/Box2D/lib/libBox2D.a )
ELSE (APPLE)
    IF (UNIX)
        INCLUDE_DIRECTORIES ( thirdparty/Linux/Box2D/include )
        SET(Box2D_LIBRARY_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/Linux/Box2D/lib/libBox2D.a )
        SET(Box2D_LIBRARY_REL ${CMAKE_SOURCE_DIR}/thirdparty/Linux/Box2D/lib/libBox2D.a )
    ELSE (UNIX)
	    SET(Box2D_include "${CMAKE_SOURCE_DIR}/thirdparty/Win/Box2D/include" CACHE STRING "Location of Box2D/Box2D.h")
        INCLUDE_DIRECTORIES (${Box2D_include})
	    SET(Box2D_LIBRARY_DEBUG "${CMAKE_SOURCE_DIR}/thirdparty/Win/Box2D/lib/Box2D_d.lib" CACHE STRING "Location of Box2D_d.lib")
	    SET(Box2D_LIBRARY_REL "${CMAKE_SOURCE_DIR}/thirdparty/Win/Box2D/lib/Box2D.lib" CACHE STRING "Location of Box2D.lib")
    ENDIF(UNIX)
ENDIF (APPLE)

#### Lua #####
IF(APPLE)
    SET(LUA_INCLUDE_DIR thirdparty/macOS/Lua/include)
    INCLUDE_DIRECTORIES (${LUA_INCLUDE_DIR})
    SET(LUA_LIBRARIES ${CMAKE_SOURCE_DIR}/thirdparty/macOS/Lua/lib/liblua.a )
ELSE (APPLE)
    IF (UNIX)
        find_package(Lua REQUIRED)
        if(LUA_FOUND)
            INCLUDE_DIRECTORIES (${LUA_INCLUDE_DIR})
        ENDIF(LUA_FOUND)
    ELSE (UNIX)
        SET(LUA_INCLUDE_DIR thirdparty/Win/Lua/include)
        INCLUDE_DIRECTORIES (${LUA_INCLUDE_DIR})
        SET(LUA_LIBRARIES ${CMAKE_SOURCE_DIR}/thirdparty/Win/Lua/lib/lua5.3.0-static.lib )
    ENDIF(UNIX)
ENDIF (APPLE)


# add bullet
#   BULLET_FOUND - Was bullet found
#   BULLET_INCLUDE_DIRS - the Bullet include directories
#   BULLET_LIBRARIES - Link to this, by default it includes
#                      all bullet components (Dynamics,
#                      Collision, LinearMath, & SoftBody)
#
#  For MacOS copy Frameworks in ./thirdparty/macOS/Bullet/ to /Library/Frameworks/
#  And set
#    BULLET_INCLUDE_DIR:PATH=/Library/Frameworks/BulletCollision.framework/Headers;/Library/Frameworks/BulletDynamics.framework/Headers;/Library/Frameworks/LinearMath.framework/Headers
#    BULLET_COLLISION_LIBRARY:FILEPATH=/Library/Frameworks/BulletCollision.framework
#    etc
#
#  If you're using Homebrew on MacOS
#    Use the following instead:
#    BULLET_INCLUDE_DIR:PATH=/usr/local/Cellar/bullet/2.87_1/include
#    BULLET_DYNAMICS_LIBRARY:PATH=/usr/local/Cellar/bullet/2.87_1/Frameworks/BulletDynamics.framework
#    BULLET_COLLISION_LIBRARY:PATH=/usr/local/Cellar/bullet/2.87_1/Frameworks/BulletCollision.framework
#    BULLET_MATH_LIBRARY:PATH=/usr/local/Cellar/bullet/2.87_1/Frameworks/LinearMath.framework
#    BULLET_DYNAMICS_LIBRARY_DEBUG:PATH=/usr/local/Cellar/bullet/2.87_1/Frameworks/BulletDynamics.framework
#    BULLET_COLLISION_LIBRARY_DEBUG:PATH=/usr/local/Cellar/bullet/2.87_1/Frameworks/BulletCollision.framework
#    BULLET_MATH_LIBRARY_DEBUG:PATH=/usr/local/Cellar/bullet/2.87_1/Frameworks/LinearMath.framework
#

option(USE_BULLET "Include bullet example" ON)
IF (USE_BULLET)
IF (WIN32)
    SET(BULLET_DYNAMICS_LIBRARY  ${CMAKE_SOURCE_DIR}/thirdparty/Win/Bullet/lib/BulletDynamics.lib CACHE PATH "")
    SET(BULLET_COLLISION_LIBRARY ${CMAKE_SOURCE_DIR}/thirdparty/Win/Bullet/lib/BulletCollision.lib CACHE PATH  "")
    SET(BULLET_MATH_LIBRARY      ${CMAKE_SOURCE_DIR}/thirdparty/Win/Bullet/lib/LinearMath.lib CACHE PATH  "") 
    SET(BULLET_DYNAMICS_LIBRARY_DEBUG  ${CMAKE_SOURCE_DIR}/thirdparty/Win/Bullet/lib/BulletDynamics_d.lib CACHE PATH  "")
    SET(BULLET_COLLISION_LIBRARY_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/Win/Bullet/lib/BulletCollision_d.lib CACHE PATH  "")
    SET(BULLET_MATH_LIBRARY_DEBUG      ${CMAKE_SOURCE_DIR}/thirdparty/Win/Bullet/lib/LinearMath_d.lib CACHE PATH  "") 
    SET(BULLET_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/thirdparty/Win/Bullet/include)
ELSE (WIN32) 
    find_package(Bullet REQUIRED)
ENDIF(WIN32)
    include_directories(${BULLET_INCLUDE_DIRS})
ENDIF(USE_BULLET)

#### Sol #####
INCLUDE_DIRECTORIES (thirdparty/All/include)

include(ExternalProject)

#### OpenAL #####
#find_package(OpenAL)
#if($OPENAL_FOUND)
#else($OPENAL_Found)
#        ExternalProject_Add(
#        OpenAL
#        URL http://kcat.strangesoft.net/openal-releases/openal-soft-1.14.tar.bz2
#    )
#endif($OPENAL_FOUND)

#SET(OpenAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/All/OpenAL-soft/include" CACHE STRING "Where the openAL headers can be found")
#include_directories(${OpenAL_include})
SET(OpenAL_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/All/OpenAL-soft_build/Debug/OpenAL32.lib" CACHE STRING "Where the OpenAL lib(static) can be found")
SET(OpenAL_COMMON_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/All/OpenAL-soft_build/Debug/common.lib" CACHE STRING "Where the OpenAL Common lib(static) can be found")
SET(OpenAL_Ex-COMMON_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/All/OpenAL-soft_build/Debug/ex-common.lib" CACHE STRING "Where the OpenAL ex-Common lib(static) can be found")
#SET(OpenAL_DLL_Path "${CMAKE_SOURCE_DIR}/thirdparty/All/OpenAL-soft_build/Debug/" CACHE STRING "Base DLL File Path")
#list(APPEND OpenAL_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/All/OpenAL-soft_build/Debug/OpenAL32.dll") #BAD idea, don't link a .dll this way

list(APPEND EXTRA_LIBS ${OpenAL_LIBRARY} ${OpenAL_COMMON_LIBRARY} ${Ex-COMMON_LIBRARY})


add_subdirectory(thirdparty/All/OpenAL-soft)
#list(APPEND all_libs OpenAL-soft)
SET(OpenAL_include "${CMAKE_SOURCE_DIR}/thirdparty/All/OpenAL-soft/include" CACHE STRING "Where the openAL headers can be found")
INCLUDE_DIRECTORIES (${OpenAL_include})
list(APPEND EXTRA_INCLUDES ${OpenAL_include})

#file(COPY thirdparty/All/OpenAL-soft DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Debug)
#file(COPY thirdparty/All/OpenAL-soft DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Release)
#file(COPY thirdparty/All/OpenAL-soft_build DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Debug)
#file(COPY thirdparty/All/OpenAL-soft_build DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Release)



#### end openAL ###########

#### libsndfile ### 
# used by OpenAL coding in Skullbasher TD
find_package(SndFile)
#add_subdirectory(thirdparty/All/libsndfile)

#INCLUDE_DIRECTORIES (thirdparty/All/libsndfile)
#INCLUDE_DIRECTORIES (thirdparty/All/libsndfile_build/include)

find_package(FLAC)
if(${FLAC_FOUND})
else(${FLAC_FOUND})

    ExternalProject_Add(
        FLAC
        URL "http://downloads.xiph.org/releases/flac/flac-1.3.0.tar.xz"
    )

    ExternalProject_Get_Property(FLAC install_dir)
    include_directories(${install_dir/include})
    SET(FLAC_DIR install_dir)
endif(${FLAC_FOUND})

find_package(Ogg)
if(${Ogg_FOUND})
else(${Ogg_FOUND})
    include(ExternalProject)

    ExternalProject_Add(
        libogg
        URL "https://downloads.xiph.org/releases/ogg/libogg-1.3.4.tar.xz"
        CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/flac/configure --prefix=<INSTALL_DIR>
        BUILD_COMMAND ${MAKE}
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/ogg
        UPDATE_COMMAND ""
        INSTALL_COMMAND 
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON
    )
    ExternalProject_Get_Property(libogg install_dir)
    include_directories(${install_dir/include})
endif(${Ogg_FOUND})

find_package(Vorbis)
if(${Vorbis_FOUND})
else(${Vorbis_FOUND})
    include(ExternalProject)

    ExternalProject_Add(
        libvorbis
        URL "https://downloads.xiph.org/releases/vorbis/libvorbis-1.3.7.tar.gz"
        CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/flac/configure --prefix=<INSTALL_DIR>
        BUILD_COMMAND ${MAKE}
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/vorbis
        UPDATE_COMMAND ""
        INSTALL_COMMAND 
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON
    )
    ExternalProject_Get_Property(libvorbis install_dir)
    include_directories(${install_dir/include})
endif(${Vorbis_FOUND})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


find_package(SndFile)
ExternalProject_Add(
    SndFile
    DEPENDS FLAC libogg libvorbis
    URL "http://www.mega-nerd.com/libsndfile/files/libsndfile-1.0.25.tar.gz"

    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
)
    ExternalProject_Get_Property(SndFile install_dir)
    include_directories(${install_dir/include})

SET(SndFile_Library "${CMAKE_SOURCE_DIR}/thirdparty/All/libsndfile_build/Debug/sndfile.lib" CACHE STRING "Where the snd file lib (static) can be found")
list(APPEND EXTRA_LIBS ${SndFile_Library})


#dd_subdirectory(thirdparty/All/libsndfile_build)
INCLUDE_DIRECTORIES (thirdparty/All/libsndfile_build/include)
    
######### end libsndfile ##############

#file(COPY thirdparty/All/libsndfile_build DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Debug)
#file(COPY thirdparty/All/libsndfile_build DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Release)


set(OPENVR_LIB)
IF (USE_OPENVR)
    find_package(OpenVR REQUIRED)
    include_directories(${OPENVR_INCLUDE_DIRS})
    IF(UNIX)
        add_definitions(-DSRE_OPENVR)
    ELSE(UNIX)
        add_definitions(/DSRE_OPENVR)
    ENDIF(UNIX)
    set(OPENVR_LIB "${OPENVR_ROOT_DIR}/lib/${OPENVR_PLATFORM}/openvr_api.lib")
ENDIF(USE_OPENVR)


# all_libs contains a list of all libraries that should be linked
# for bullet support add ${BULLET_LIBRARIES}
set(all_libs ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2MIXER_LIBRARY} ${LUA_LIBRARIES} SRE debug ${Box2D_LIBRARY_DEBUG} optimized ${Box2D_LIBRARY_REL} ${OPENVR_LIB})
IF (USE_BULLET)
	set(all_libs optimized  ${all_libs} optimized  ${BULLET_DYNAMICS_LIBRARY} optimized  ${BULLET_COLLISION_LIBRARY} optimized  ${BULLET_MATH_LIBRARY} debug ${BULLET_DYNAMICS_LIBRARY_DEBUG} debug ${BULLET_COLLISION_LIBRARY_DEBUG} debug ${BULLET_MATH_LIBRARY_DEBUG} )
ENDIF(USE_BULLET)



# Create DllFileList on Windows
IF (WIN32)
    SET(SDL2_dll "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2-2.0.9/lib/x86/SDL2.dll" CACHE STRING "Where the SDL2.dll can be found")
    SET(SDL2_image_dll_path "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_image-2.0.4/lib/x86" CACHE PATH "Where the SDL2_image.dll can be found")
    SET(SDL2_mixer_dll_path "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_mixer-2.0.4/lib/x86" CACHE PATH "Where the SDL2_mixer.dll can be found")
    #SET(DLLFileList "${SDL2_image_dll_path}/SDL2_image.dll" "${SDL2_image_dll_path}/libjpeg-9.dll" "${SDL2_image_dll_path}/libpng16-16.dll" "${SDL2_image_dll_path}/libtiff-5.dll" "${SDL2_image_dll_path}/libwebp-7.dll" "${SDL2_image_dll_path}/zlib1.dll" "${SDL2_mixer_dll_path}/SDL2_mixer.dll" "${SDL2_mixer_dll_path}/libvorbisfile-3.dll"  "${SDL2_mixer_dll_path}/libvorbis-0.dll"  "${SDL2_mixer_dll_path}/libogg-0.dll"  "${SDL2_mixer_dll_path}/libmodplug-1.dll"  "${SDL2_mixer_dll_path}/libFLAC-8.dll"   "${SDL2_mixer_dll_path}/libmpg123-0.dll" ${SDL2_dll})
    SET(DLLFileList "${OpenAL_DLL_Path}/OpenAL32.dll" "${SDL2_image_dll_path}/SDL2_image.dll" "${SDL2_image_dll_path}/libjpeg-9.dll" "${SDL2_image_dll_path}/libpng16-16.dll" "${SDL2_image_dll_path}/libtiff-5.dll" "${SDL2_image_dll_path}/libwebp-7.dll" "${SDL2_image_dll_path}/zlib1.dll" "${SDL2_mixer_dll_path}/SDL2_mixer.dll" "${SDL2_mixer_dll_path}/libvorbisfile-3.dll"  "${SDL2_mixer_dll_path}/libvorbis-0.dll"  "${SDL2_mixer_dll_path}/libogg-0.dll"  "${SDL2_mixer_dll_path}/libmodplug-1.dll"  "${SDL2_mixer_dll_path}/libFLAC-8.dll"   "${SDL2_mixer_dll_path}/libmpg123-0.dll" ${SDL2_dll})
ENDIF(WIN32)

# build sre library
add_subdirectory(submodules/SimpleRenderEngine)
include_directories(${SRE_INCLUDE})

# build targets using sre
SET(USE_SRE_PROJECT_EXAMPLES ON CACHE BOOL "Enable project examples for SRE")
if (USE_SRE_PROJECT_EXAMPLES)
add_subdirectory(project)
endif(USE_SRE_PROJECT_EXAMPLES)
