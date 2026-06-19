# Copyright (c) 2026 Advanced Micro Devices, Inc.
# Author: Jeff Daily <jeff.daily@amd.com>
#
# Standalone build for the testLBVH() sample. The default configuration targets
# NVIDIA via CUDA (the .cu source is compiled by the CUDA toolchain, matching
# the Visual Studio solution). Configuring with -DUSE_HIP=ON instead targets
# AMD via ROCm/HIP: the sources are compiled by hipcc with the in-repo
# cuda_to_hip.h force-included. The CUDA path is unchanged by the HIP option.

cmake_minimum_required(VERSION 3.21)

option(USE_HIP "Build with ROCm/HIP" OFF)

if(USE_HIP)
    project(KittenLBVH C CXX HIP)
else()
    project(KittenLBVH C CXX CUDA)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(USE_HIP)
    find_package(hip REQUIRED)
endif()

include(FetchContent)

# GLM 1.0.1: the system GLM 0.9.9.8 lacks __HIP__ detection; 1.0 has it
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(glm)

find_package(OpenMP)

set(SOURCES main.cpp lbvh.cu)

if(USE_HIP)
    set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE HIP)
    add_executable(lbvh_test ${SOURCES})
    set_target_properties(lbvh_test PROPERTIES
        HIP_ARCHITECTURES "${CMAKE_HIP_ARCHITECTURES}"
    )
    target_compile_definitions(lbvh_test PRIVATE USE_HIP)
    target_include_directories(lbvh_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/KittenEngine/includes
    )
    target_compile_options(lbvh_test PRIVATE
        -include ${CMAKE_CURRENT_SOURCE_DIR}/cuda_to_hip.h
    )
    target_link_libraries(lbvh_test PRIVATE glm::glm hip::host)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(lbvh_test PRIVATE OpenMP::OpenMP_CXX)
    endif()
else()
    add_executable(lbvh_test ${SOURCES})
    target_include_directories(lbvh_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/KittenEngine/includes
    )
    target_link_libraries(lbvh_test PRIVATE glm::glm)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(lbvh_test PRIVATE OpenMP::OpenMP_CXX)
    endif()
endif()
