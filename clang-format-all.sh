#!/bin/bash 
find ./src -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' -exec clang-format -i {} \;
