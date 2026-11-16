cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(gaps_likelihood VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(ROOT REQUIRED COMPONENTS MathCore Minuit2 Hist)

add_library(gaps_likelihood INTERFACE)
add_library(gaps_likelihood::gaps_likelihood ALIAS gaps_likelihood)

target_include_directories(gaps_likelihood INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(gaps_likelihood INTERFACE
  ROOT::MathCore
  ROOT::Minuit2
  ROOT::Hist
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS gaps_likelihood EXPORT gaps_likelihoodTargets)

set(package_dir "${CMAKE_INSTALL_LIBDIR}/cmake/gaps_likelihood")

install(EXPORT gaps_likelihoodTargets
    FILE gaps_likelihoodTargets.cmake
    NAMESPACE gaps_likelihood::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/gaps_likelihood
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/gaps_likelihoodConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/gaps_likelihoodConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/gaps_likelihood
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/gaps_likelihoodConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/gaps_likelihoodConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/gaps_likelihoodConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/gaps_likelihood
)

add_executable(beta_ml examples/beta_ml.cpp)
target_link_libraries(beta_ml PRIVATE gaps_likelihood::gaps_likelihood)
