set(ACCELERATOR_SOURCE
    ${CMAKE_SOURCE_DIR}/src/accelerator/bvh.cc
    ${CMAKE_SOURCE_DIR}/src/accelerator/bvh.h
    )

set(CORES_SOURCE
    ${CMAKE_SOURCE_DIR}/src/cores/bounds.cc
    ${CMAKE_SOURCE_DIR}/src/cores/bounds.h
    ${CMAKE_SOURCE_DIR}/src/cores/intersection.hpp
    ${CMAKE_SOURCE_DIR}/src/cores/ray.hpp
    ${CMAKE_SOURCE_DIR}/src/cores/render.cc
    ${CMAKE_SOURCE_DIR}/src/cores/render.h
    ${CMAKE_SOURCE_DIR}/src/cores/scene.cc
    ${CMAKE_SOURCE_DIR}/src/cores/scene.h
    ${CMAKE_SOURCE_DIR}/src/cores/primitive.h
    )

set(MATH_SOURCE
    ${CMAKE_SOURCE_DIR}/src/math/geometry.hpp
    ${CMAKE_SOURCE_DIR}/src/math/math_utils.hpp
    ${CMAKE_SOURCE_DIR}/src/math/matrix.hpp
    ${CMAKE_SOURCE_DIR}/src/math/vector.hpp
    ${CMAKE_SOURCE_DIR}/src/math/transform.h
    ${CMAKE_SOURCE_DIR}/src/math/transform.cc
    )

set(SAMPLER_SOURCE
    )

set(CAMERAS_SOURCE
    ${CMAKE_SOURCE_DIR}/src/cameras/camera.h
    ${CMAKE_SOURCE_DIR}/src/cameras/pinholeCamera.cc
    ${CMAKE_SOURCE_DIR}/src/cameras/pinholeCamera.h
    )

set(SHAPES_SOURCE
    ${CMAKE_SOURCE_DIR}/src/shapes/shape.h
    ${CMAKE_SOURCE_DIR}/src/shapes/sphere.cc
    ${CMAKE_SOURCE_DIR}/src/shapes/sphere.h
    )

set(UTILS_SOURCE
    ${CMAKE_SOURCE_DIR}/src/utils/di_global.h
    ${CMAKE_SOURCE_DIR}/src/utils/thread_pool.hpp
    )

set(SOURCE_HEADERS ${ACCELERATOR_SOURCE} ${CORES_SOURCE} ${MATH_SOURCE} ${SAMPLER_SOURCE}
    ${SHAPES_SOURCE} ${CAMERAS_SOURCE} ${UTILS_SOURCE})
