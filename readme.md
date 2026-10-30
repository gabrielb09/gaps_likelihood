# Installation Instructions
1. Navigate to the gaps_likelihood directory
2. mkdir build
3. cd build
4. cmake ..
5. make

# usage
To use just add 

'add_subdirectory(
    /path/to/gaps_likelihood
    ${CMAKE_CURRENT_BINARY_DIR}/gaps_likelihood
)'

to your CMakeLists.txt
