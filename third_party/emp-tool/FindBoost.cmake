# Dummy CMake script to set the correct paths for Boost libraries.
# This is needed as Boost libraries built using rules_boost have different file
# names and therefore are not detected by the default FindBoost.cmake.

set(Boost_FOUND True)
set(Boost_INCLUDE_DIRS "${EXT_BUILD_DEPS}/include")
set(Boost_LIBRARY_DIRS "${EXT_BUILD_DEPS}")
set(Boost_VERSION "1.71.0")  # Depends on rules_boost version.
