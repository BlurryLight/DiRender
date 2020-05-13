#!/bin/bash 
find ./src ./test -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' -exec clang-format -i {} \;
