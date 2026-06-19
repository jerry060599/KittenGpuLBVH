#pragma once
// Copyright (c) 2026 Advanced Micro Devices, Inc.
// \author Jeff Daily <jeff.daily@amd.com>
//
// CUDA-to-HIP compatibility shim for the ROCm build. Force-included (compiler
// -include flag) on every HIP translation unit so it is parsed before any
// project or GLM header, then again pulled in by sources that name CUDA symbols.
//
// Aliases the cuda* runtime/stream surface the sources use to hip*. The HIP
// runtime is included first so its __device__ memcpy/memset overloads are in
// scope for any header the sources pull in afterwards.
//
// GLM device-qualifier note: the kernels call glm:: math from __device__ code.
// GLM only decorates those __host__ __device__ when it detects a device compiler.
// GLM >= 1.0 recognizes hipcc via __HIP__ directly (glm/simd/platform.h ->
// GLM_COMPILER_HIP), so the ROCm build pins GLM 1.0.x (FetchContent in
// CMakeLists.txt) and needs no compiler-identity spoofing here.

#if defined(USE_HIP)

#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <hip/hip_runtime.h>

// --- runtime API -----------------------------------------------------------
#define cudaError_t                         hipError_t
#define cudaSuccess                         hipSuccess
#define cudaGetErrorString                  hipGetErrorString
#define cudaGetLastError                    hipGetLastError
#define cudaPeekAtLastError                 hipPeekAtLastError
#define cudaDeviceSynchronize               hipDeviceSynchronize

#define cudaMalloc                          hipMalloc
#define cudaFree                            hipFree
#define cudaMemcpy                          hipMemcpy
#define cudaMemcpyAsync                     hipMemcpyAsync
#define cudaMemset                          hipMemset
#define cudaMemsetAsync                     hipMemsetAsync
#define cudaMemcpyHostToDevice              hipMemcpyHostToDevice
#define cudaMemcpyDeviceToHost              hipMemcpyDeviceToHost
#define cudaMemcpyDeviceToDevice            hipMemcpyDeviceToDevice
#define cudaMemcpyKind                      hipMemcpyKind

// --- streams ---------------------------------------------------------------
#define cudaStream_t                        hipStream_t
#define cudaStreamCreate                    hipStreamCreate
#define cudaStreamDestroy                   hipStreamDestroy
#define cudaStreamSynchronize               hipStreamSynchronize

#endif // USE_HIP
