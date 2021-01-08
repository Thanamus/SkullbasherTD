# Install script for directory: C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/libsndfile")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Debug/sndfile-info.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Release/sndfile-info.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/MinSizeRel/sndfile-info.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/RelWithDebInfo/sndfile-info.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Debug/sndfile-play.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Release/sndfile-play.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/MinSizeRel/sndfile-play.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/RelWithDebInfo/sndfile-play.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Debug/sndfile-convert.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Release/sndfile-convert.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/MinSizeRel/sndfile-convert.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/RelWithDebInfo/sndfile-convert.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Debug/sndfile-cmp.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Release/sndfile-cmp.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/MinSizeRel/sndfile-cmp.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/RelWithDebInfo/sndfile-cmp.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Debug/sndfile-metadata-set.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Release/sndfile-metadata-set.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/MinSizeRel/sndfile-metadata-set.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/RelWithDebInfo/sndfile-metadata-set.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Debug/sndfile-metadata-get.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Release/sndfile-metadata-get.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/MinSizeRel/sndfile-metadata-get.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/RelWithDebInfo/sndfile-metadata-get.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Debug/sndfile-interleave.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Release/sndfile-interleave.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/MinSizeRel/sndfile-interleave.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/RelWithDebInfo/sndfile-interleave.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Debug/sndfile-deinterleave.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Release/sndfile-deinterleave.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/MinSizeRel/sndfile-deinterleave.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/RelWithDebInfo/sndfile-deinterleave.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Debug/sndfile-concat.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Release/sndfile-concat.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/MinSizeRel/sndfile-concat.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/RelWithDebInfo/sndfile-concat.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Debug/sndfile-salvage.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Release/sndfile-salvage.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/MinSizeRel/sndfile-salvage.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/RelWithDebInfo/sndfile-salvage.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Debug/sndfile.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/Release/sndfile.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/MinSizeRel/sndfile.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/RelWithDebInfo/sndfile.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/include/sndfile.hh"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/include/sndfile.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SndFileTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SndFileTargets.cmake"
         "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/CMakeFiles/Export/cmake/SndFileTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SndFileTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SndFileTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/CMakeFiles/Export/cmake/SndFileTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/CMakeFiles/Export/cmake/SndFileTargets-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/CMakeFiles/Export/cmake/SndFileTargets-minsizerel.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/CMakeFiles/Export/cmake/SndFileTargets-relwithdebinfo.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/CMakeFiles/Export/cmake/SndFileTargets-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/SndFileConfig.cmake"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/SndFileConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/man/sndfile-info.1"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/man/sndfile-play.1"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/man/sndfile-convert.1"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/man/sndfile-cmp.1"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/man/sndfile-metadata-get.1"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/man/sndfile-concat.1"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/man/sndfile-interleave.1"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/man/sndfile-salvage.1"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE RENAME "sndfile-metadata-set.1" FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/man/sndfile-metadata-get.1")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE RENAME "sndfile-deinterleave.1" FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/man/sndfile-interleave.1")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libsndfile" TYPE FILE FILES
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/index.md"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/libsndfile.jpg"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/libsndfile.css"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/print.css"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/api.md"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/command.md"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/bugs.md"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/formats.md"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/sndfile_info.md"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/new_file_type_howto.md"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/win32.md"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/FAQ.md"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/lists.md"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/embedded_files.md"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/octave.md"
    "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile/docs/tutorial.md"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/sndfile.pc")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/nfgol/ITU_GProg/SkullBasherTD/SRE_project/thirdparty/All/libsndfile_build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
