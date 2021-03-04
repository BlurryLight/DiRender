set(ACCELERATOR_SOURCE
        ${CMAKE_SOURCE_DIR}/src/accelerator/bvh.cc
        ${CMAKE_SOURCE_DIR}/src/accelerator/bvh.h
        ${CMAKE_SOURCE_DIR}/src/accelerator/linear_list.cc
        ${CMAKE_SOURCE_DIR}/src/accelerator/linear_list.h
        )

set(CORES_SOURCE
        ${CMAKE_SOURCE_DIR}/src/cores/bounds.cc
        ${CMAKE_SOURCE_DIR}/src/cores/bounds.h
        ${CMAKE_SOURCE_DIR}/src/cores/intersection.hpp
        ${CMAKE_SOURCE_DIR}/src/cores/ray.hpp
        ${CMAKE_SOURCE_DIR}/src/cores/scene.cc
        ${CMAKE_SOURCE_DIR}/src/cores/scene.h
        ${CMAKE_SOURCE_DIR}/src/cores/primitive.h
        )

set(MATH_SOURCE
        ${CMAKE_SOURCE_DIR}/src/math/geometry.hpp
        ${CMAKE_SOURCE_DIR}/src/math/math_utils.hpp
        ${CMAKE_SOURCE_DIR}/src/math/matrix.hpp
        ${CMAKE_SOURCE_DIR}/src/math/matrix.cc
        ${CMAKE_SOURCE_DIR}/src/math/vector.hpp
        ${CMAKE_SOURCE_DIR}/src/math/transform.h
        ${CMAKE_SOURCE_DIR}/src/math/transform.cc
        )

set(SAMPLER_SOURCE
        )

set(CAMERAS_SOURCE
        ${CMAKE_SOURCE_DIR}/src/cameras/camera.h
        ${CMAKE_SOURCE_DIR}/src/cameras/camera.cc
        ${CMAKE_SOURCE_DIR}/src/cameras/pinhole_camera.cc
        ${CMAKE_SOURCE_DIR}/src/cameras/pinhole_camera.h
        )

set(SHAPES_SOURCE
        ${CMAKE_SOURCE_DIR}/src/shapes/shape.h
        ${CMAKE_SOURCE_DIR}/src/shapes/sphere.cc
        ${CMAKE_SOURCE_DIR}/src/shapes/sphere.h
        ${CMAKE_SOURCE_DIR}/src/shapes/triangle.h
        ${CMAKE_SOURCE_DIR}/src/shapes/triangle.cc
        ${CMAKE_SOURCE_DIR}/src/shapes/quad.h
        ${CMAKE_SOURCE_DIR}/src/shapes/quad.cc
        )

set(UTILS_SOURCE
        ${CMAKE_SOURCE_DIR}/src/utils/di_global.h
        ${CMAKE_SOURCE_DIR}/src/utils/thread_pool.hpp
        ${CMAKE_SOURCE_DIR}/src/utils/parse_scene.hh
        ${CMAKE_SOURCE_DIR}/src/utils/parse_scene.cc
        ${CMAKE_SOURCE_DIR}/src/utils/parse_scene_txt.hh
        ${CMAKE_SOURCE_DIR}/src/utils/parse_scene_txt.cc
        ${CMAKE_SOURCE_DIR}/src/utils/high_resolution_timer.h
        ${CMAKE_SOURCE_DIR}/src/utils/high_resolution_timer.cc
        ${CMAKE_SOURCE_DIR}/src/utils/resource_path_searcher.h
        ${CMAKE_SOURCE_DIR}/src/utils/resource_path_searcher.cc
        )
set(SPECIAL_SOURCE #Include twice may cause link error

        ${CMAKE_SOURCE_DIR}/src/utils/stb_image_wrapper.h
        ${CMAKE_SOURCE_DIR}/src/utils/stb_image_wrapper.cc
        ${CMAKE_SOURCE_DIR}/src/utils/OBJ_Loader_wrapper.h
        ${CMAKE_SOURCE_DIR}/src/utils/OBJ_Loader_wrapper.cc
        )
set(MATERIAL_SOURCE
        ${CMAKE_SOURCE_DIR}/src/material/material.h
        ${CMAKE_SOURCE_DIR}/src/material/matte_material.h
        ${CMAKE_SOURCE_DIR}/src/material/matte_material.cc
        ${CMAKE_SOURCE_DIR}/src/material/metal_material.h
        ${CMAKE_SOURCE_DIR}/src/material/metal_material.cc
        ${CMAKE_SOURCE_DIR}/src/material/glass_material.h
        ${CMAKE_SOURCE_DIR}/src/material/glass_material.cc
        ${CMAKE_SOURCE_DIR}/src/material/dielectric_material.h
        ${CMAKE_SOURCE_DIR}/src/material/dielectric_material.cc
        ${CMAKE_SOURCE_DIR}/src/material/phong_material.h
        ${CMAKE_SOURCE_DIR}/src/material/phong_material.cc
        )

set(THIRD_PARTY_SOURCE
        ${CMAKE_SOURCE_DIR}/third_party/stb_image_write.h
        ${CMAKE_SOURCE_DIR}/third_party/stb_image.h
        ${CMAKE_SOURCE_DIR}/third_party/OBJ_Loader.h
        )

set(TEXTURE_SOURCE
        ${CMAKE_SOURCE_DIR}/src/texture/texture.h
        ${CMAKE_SOURCE_DIR}/src/texture/constant_texture.h
        ${CMAKE_SOURCE_DIR}/src/texture/checker_texture.h
        )

set(RENDERER_SOURCE
        ${CMAKE_SOURCE_DIR}/src/renderer/renderer.h
        ${CMAKE_SOURCE_DIR}/src/renderer/path_tracing_renderer.h
        ${CMAKE_SOURCE_DIR}/src/renderer/path_tracing_renderer.cc
        ${CMAKE_SOURCE_DIR}/src/renderer/direct_light_renderer.h
        ${CMAKE_SOURCE_DIR}/src/renderer/direct_light_renderer.cc
        ${CMAKE_SOURCE_DIR}/src/renderer/bling_phong_renderer.h
        ${CMAKE_SOURCE_DIR}/src/renderer/bling_phong_renderer.cc
        )

set(SOURCE_HEADERS ${THIRD_PARTY_SOURCE} ${ACCELERATOR_SOURCE} ${CORES_SOURCE} ${MATH_SOURCE} ${SAMPLER_SOURCE}
        ${SHAPES_SOURCE} ${CAMERAS_SOURCE} ${UTILS_SOURCE} ${MATERIAL_SOURCE} ${TEXTURE_SOURCE} ${RENDERER_SOURCE} ${SPECIAL_SOURCE})
