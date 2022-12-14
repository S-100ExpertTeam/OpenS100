# Install script for directory: C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/out/install/x64-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/GeographicLib" TYPE FILE FILES
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/Accumulator.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/AlbersEqualArea.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/AzimuthalEquidistant.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/CassiniSoldner.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/CircularEngine.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/Constants.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/DMS.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/Ellipsoid.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/EllipticFunction.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/GARS.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/GeoCoords.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/Geocentric.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/Geodesic.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/GeodesicExact.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/GeodesicLine.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/GeodesicLineExact.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/Geohash.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/Geoid.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/Georef.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/Gnomonic.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/GravityCircle.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/GravityModel.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/LambertConformalConic.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/LocalCartesian.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/MGRS.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/MagneticCircle.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/MagneticModel.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/Math.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/NearestNeighbor.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/NormalGravity.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/OSGB.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/PolarStereographic.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/PolygonArea.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/Rhumb.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/SphericalEngine.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/SphericalHarmonic.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/SphericalHarmonic1.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/SphericalHarmonic2.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/TransverseMercator.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/TransverseMercatorExact.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/UTMUPS.hpp"
    "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/Utility.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/GeographicLib" TYPE FILE FILES "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/out/build/x64-Debug/Config.h")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/PROST/newS100/extlibs/GeographicLib/include/GeographicLib/out/build/x64-Debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
