cmake_minimum_required(VERSION 3.16)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.15.0
)

FetchContent_MakeAvailable(Catch2)

set_target_properties(Catch2 PROPERTIES FOLDER "deps")
set_target_properties(Catch2WithMain PROPERTIES FOLDER "deps")

