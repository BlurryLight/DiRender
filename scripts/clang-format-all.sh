#!/bin/bash 
find ./src ./test -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' -exec clang-format -i {} \;
find ./debugger -maxdepth 1 -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' -exec clang-format -i {} \;
