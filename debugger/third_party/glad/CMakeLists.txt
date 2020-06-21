cmake_minimum_required(VERSION 3.0)
project(Glad)

add_library(glad glad.h glad.c khrplatform.h)
target_include_directories(glad PUBLIC .)
