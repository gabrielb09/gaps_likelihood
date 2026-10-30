cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(gaps_likelihood LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ROOT REQUIRED COMPONENTS MathCore Minuit2 Hist)

add_library(likelihood INTERFACE)
target_include_directories(likelihood INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(likelihood INTERFACE
  ROOT::MathCore
  ROOT::Minuit2
  ROOT::Hist
)

add_executable(beta_ml examples/beta_ml.cpp)
target_link_libraries(beta_ml PRIVATE likelihood)
