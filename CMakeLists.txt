cmake_minimum_required(VERSION 3.10)

# Nama proyek
project(run)

# Mencari OpenCV yang terinstal di sistem
find_package(OpenCV REQUIRED)

# Menambahkan executable dari file main.cpp
add_executable(run tugas2.cpp)

# Menautkan library OpenCV
target_link_libraries(run ${OpenCV_LIBS})