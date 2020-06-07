# from https://zhuanlan.zhihu.com/p/146434531
# experimental
# Unity Build 函数，将输入的文件列表打包输出为一个 unit_build.cpp文件
# input_src：文件列表，调用时通过字符串传递，如 "${PROJECT_SRC}"。若在.cmake文件中调用，则需要提供绝对路径
# output_file：输出文件，存放在 ${CMAKE_BINARY_DIR}/${output_file}，即build目录
function(UNITY_BUILD output_file input_src)
    file(WRITE ${CMAKE_BINARY_DIR}/${output_file} "//  ${output_file}\n")
    foreach(filename ${input_src})
        file(APPEND ${CMAKE_BINARY_DIR}/${output_file} "#include \"${filename}\"\n")
    endforeach()
    message("Generate unity build file: ${CMAKE_BINARY_DIR}/${output_file}")
endfunction(UNITY_BUILD)
