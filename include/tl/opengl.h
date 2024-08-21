#pragma once
#include "system.h"
#include "console.h"
#include "math.h"
#include "time.h"

#pragma warning(push, 0)
#define NOMINMAX
#include <Windows.h>
#include <gl/GL.h>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 4820)

#pragma comment(lib, "opengl32")
#ifdef TL_IMPL
#pragma comment(lib, "gdi32")
#endif

using GLchar = char;
using GLsizeiptr = tl::umm;
using GLintptr = tl::smm;
typedef void (APIENTRY *DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam);

#define GL_CONSTANT_COLOR                        0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR              0x8002
#define GL_CONSTANT_ALPHA                        0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA              0x8004
#define GL_BLEND_COLOR                           0x8005
#define GL_FUNC_ADD                              0x8006
#define GL_MIN                                   0x8007
#define GL_MAX                                   0x8008
#define GL_BLEND_EQUATION                        0x8009
#define GL_FUNC_SUBTRACT                         0x800A
#define GL_FUNC_REVERSE_SUBTRACT                 0x800B
#define GL_CLAMP_TO_BORDER                       0x812D
#define GL_CLAMP_TO_EDGE                         0x812F
#define GL_TEXTURE_BASE_LEVEL                    0x813C
#define GL_TEXTURE_MAX_LEVEL                     0x813D
#define GL_DEPTH_COMPONENT16                     0x81A5
#define GL_DEPTH_COMPONENT32                     0x81A7
#define GL_FRAMEBUFFER_UNDEFINED                 0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT              0x821A
#define GL_RG                                    0x8227
#define GL_RG_INTEGER                            0x8228
#define GL_R8                                    0x8229
#define GL_R16                                   0x822A
#define GL_RG8                                   0x822B
#define GL_RG16                                  0x822C
#define GL_R16F                                  0x822D
#define GL_R32F                                  0x822E
#define GL_RG16F                                 0x822F
#define GL_RG32F                                 0x8230
#define GL_R8I                                   0x8231
#define GL_R8UI                                  0x8232
#define GL_R16I                                  0x8233
#define GL_R16UI                                 0x8234
#define GL_R32I                                  0x8235
#define GL_R32UI                                 0x8236
#define GL_RG8I                                  0x8237
#define GL_RG8UI                                 0x8238
#define GL_RG16I                                 0x8239
#define GL_RG16UI                                0x823A
#define GL_RG32I                                 0x823B
#define GL_RG32UI                                0x823C
#define GL_DEBUG_OUTPUT_SYNCHRONOUS              0x8242
#define GL_DEBUG_SOURCE_API                      0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM            0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER          0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY              0x8249
#define GL_DEBUG_SOURCE_APPLICATION              0x824A
#define GL_DEBUG_SOURCE_OTHER                    0x824B
#define GL_DEBUG_TYPE_ERROR                      0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR        0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR         0x824E
#define GL_DEBUG_TYPE_PORTABILITY                0x824F
#define GL_DEBUG_TYPE_PERFORMANCE                0x8250
#define GL_DEBUG_TYPE_OTHER                      0x8251
#define GL_DEBUG_TYPE_MARKER                     0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP                 0x8269
#define GL_DEBUG_TYPE_POP_GROUP                  0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION           0x826B
#define GL_TEXTURE0                              0x84C0
#define GL_TEXTURE1                              0x84C1
#define GL_TEXTURE2                              0x84C2
#define GL_TEXTURE3                              0x84C3
#define GL_TEXTURE4                              0x84C4
#define GL_TEXTURE5                              0x84C5
#define GL_TEXTURE6                              0x84C6
#define GL_TEXTURE7                              0x84C7
#define GL_TEXTURE8                              0x84C8
#define GL_TEXTURE9                              0x84C9
#define GL_TEXTURE10                             0x84CA
#define GL_TEXTURE11                             0x84CB
#define GL_TEXTURE12                             0x84CC
#define GL_TEXTURE13                             0x84CD
#define GL_TEXTURE14                             0x84CE
#define GL_TEXTURE15                             0x84CF
#define GL_TEXTURE16                             0x84D0
#define GL_TEXTURE17                             0x84D1
#define GL_TEXTURE18                             0x84D2
#define GL_TEXTURE19                             0x84D3
#define GL_TEXTURE20                             0x84D4
#define GL_TEXTURE21                             0x84D5
#define GL_TEXTURE22                             0x84D6
#define GL_TEXTURE23                             0x84D7
#define GL_TEXTURE24                             0x84D8
#define GL_TEXTURE25                             0x84D9
#define GL_TEXTURE26                             0x84DA
#define GL_TEXTURE27                             0x84DB
#define GL_TEXTURE28                             0x84DC
#define GL_TEXTURE29                             0x84DD
#define GL_TEXTURE30                             0x84DE
#define GL_TEXTURE31                             0x84DF
#define GL_TEXTURE_CUBE_MAP                      0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP              0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X           0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X           0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y           0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y           0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z           0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z           0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP                0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE             0x851C
#define GL_DEPTH_CLAMP                           0x864F
#define GL_RGBA32F                               0x8814
#define GL_RGB32F                                0x8815
#define GL_RGBA16F                               0x881A
#define GL_RGB16F                                0x881B
#define GL_TEXTURE_COMPARE_MODE                  0x884C
#define GL_TEXTURE_COMPARE_FUNC                  0x884D
#define GL_COMPARE_REF_TO_TEXTURE                0x884E
#define GL_ARRAY_BUFFER                          0x8892
#define GL_ELEMENT_ARRAY_BUFFER                  0x8893
#define GL_ARRAY_BUFFER_BINDING                  0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING          0x8895
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING    0x889F
#define GL_READ_ONLY                             0x88B8
#define GL_WRITE_ONLY                            0x88B9
#define GL_READ_WRITE                            0x88BA
#define GL_BUFFER_ACCESS                         0x88BB
#define GL_BUFFER_MAPPED                         0x88BC
#define GL_BUFFER_MAP_POINTER                    0x88BD
#define GL_STREAM_DRAW                           0x88E0
#define GL_STREAM_READ                           0x88E1
#define GL_STREAM_COPY                           0x88E2
#define GL_STATIC_DRAW                           0x88E4
#define GL_STATIC_READ                           0x88E5
#define GL_STATIC_COPY                           0x88E6
#define GL_DYNAMIC_DRAW                          0x88E8
#define GL_DYNAMIC_READ                          0x88E9
#define GL_DYNAMIC_COPY                          0x88EA
#define GL_DEPTH24_STENCIL8                      0x88F0
#define GL_SRC1_COLOR                            0x88F9
#define GL_ONE_MINUS_SRC1_COLOR                  0x88FA
#define GL_UNIFORM_BUFFER                        0x8A11
#define GL_VERTEX_SHADER                         0x8B31
#define GL_FRAGMENT_SHADER                       0x8B30
#define GL_COMPILE_STATUS                        0x8B81
#define GL_LINK_STATUS                           0x8B82
#define GL_INFO_LOG_LENGTH                       0x8B84
#define GL_ACTIVE_UNIFORMS                       0x8B86
#define GL_FRAMEBUFFER_COMPLETE                  0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT     0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER    0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER    0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED               0x8CDD
#define GL_COLOR_ATTACHMENT0                     0x8CE0
#define GL_DEPTH_ATTACHMENT                      0x8D00
#define GL_FRAMEBUFFER                           0x8D40
#define GL_RENDERBUFFER                          0x8D41
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE    0x8D56
#define GL_COPY_READ_BUFFER                      0x8F36
#define GL_COPY_WRITE_BUFFER                     0x8F37
#define GL_SHADER_STORAGE_BUFFER                 0x90D2
#define GL_DEBUG_SEVERITY_HIGH                   0x9146
#define GL_DEBUG_SEVERITY_MEDIUM                 0x9147
#define GL_DEBUG_SEVERITY_LOW                    0x9148
#define GL_COMPUTE_SHADER                        0x91B9
#define GL_DEBUG_OUTPUT                          0x92E0

#define GL_SHADER_STORAGE_BARRIER_BIT 0x00002000

#define GL_MAP_READ_BIT              0x0001
#define GL_MAP_WRITE_BIT             0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT  0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT    0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT    0x0020

#if OS_WINDOWS
#define WGL_SAMPLE_BUFFERS_3DFX 0x2060
#define WGL_SAMPLES_3DFX 0x2061
#define WGL_STEREO_EMITTER_ENABLE_3DL 0x2055
#define WGL_STEREO_EMITTER_DISABLE_3DL 0x2056
#define WGL_STEREO_POLARITY_NORMAL_3DL 0x2057
#define WGL_STEREO_POLARITY_INVERT_3DL 0x2058
#define WGL_GPU_VENDOR_AMD 0x1F00
#define WGL_GPU_RENDERER_STRING_AMD 0x1F01
#define WGL_GPU_OPENGL_VERSION_STRING_AMD 0x1F02
#define WGL_GPU_FASTEST_TARGET_GPUS_AMD 0x21A2
#define WGL_GPU_RAM_AMD 0x21A3
#define WGL_GPU_CLOCK_AMD 0x21A4
#define WGL_GPU_NUM_PIPES_AMD 0x21A5
#define WGL_GPU_NUM_SIMD_AMD 0x21A6
#define WGL_GPU_NUM_RB_AMD 0x21A7
#define WGL_GPU_NUM_SPI_AMD 0x21A8
#define WGL_FRONT_COLOR_BUFFER_BIT_ARB 0x00000001
#define WGL_BACK_COLOR_BUFFER_BIT_ARB 0x00000002
#define WGL_DEPTH_BUFFER_BIT_ARB 0x00000004
#define WGL_STENCIL_BUFFER_BIT_ARB 0x00000008
#define WGL_CONTEXT_RELEASE_BEHAVIOR_ARB 0x2097
#define WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB 0
#define WGL_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB 0x2098
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB 0x2093
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define ERROR_INVALID_VERSION_ARB 0x2095
#define WGL_CONTEXT_OPENGL_NO_ERROR_ARB 0x31B3
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define ERROR_INVALID_PROFILE_ARB 0x2096
#define WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB 0x00000004
#define WGL_LOSE_CONTEXT_ON_RESET_ARB 0x8252
#define WGL_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB 0x8256
#define WGL_NO_RESET_NOTIFICATION_ARB 0x8261
#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB 0x20A9
#define ERROR_INVALID_PIXEL_TYPE_ARB 0x2043
#define ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB 0x2054
#define WGL_SAMPLE_BUFFERS_ARB 0x2041
#define WGL_SAMPLES_ARB 0x2042
#define WGL_DRAW_TO_PBUFFER_ARB 0x202D
#define WGL_MAX_PBUFFER_PIXELS_ARB 0x202E
#define WGL_MAX_PBUFFER_WIDTH_ARB 0x202F
#define WGL_MAX_PBUFFER_HEIGHT_ARB 0x2030
#define WGL_PBUFFER_LARGEST_ARB 0x2033
#define WGL_PBUFFER_WIDTH_ARB 0x2034
#define WGL_PBUFFER_HEIGHT_ARB 0x2035
#define WGL_PBUFFER_LOST_ARB 0x2036
#define WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_DRAW_TO_BITMAP_ARB 0x2002
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_NEED_PALETTE_ARB 0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB 0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB 0x2006
#define WGL_SWAP_METHOD_ARB 0x2007
#define WGL_NUMBER_OVERLAYS_ARB 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB 0x2009
#define WGL_TRANSPARENT_ARB 0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB 0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
#define WGL_SHARE_DEPTH_ARB 0x200C
#define WGL_SHARE_STENCIL_ARB 0x200D
#define WGL_SHARE_ACCUM_ARB 0x200E
#define WGL_SUPPORT_GDI_ARB 0x200F
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_STEREO_ARB 0x2012
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_RED_BITS_ARB 0x2015
#define WGL_RED_SHIFT_ARB 0x2016
#define WGL_GREEN_BITS_ARB 0x2017
#define WGL_GREEN_SHIFT_ARB 0x2018
#define WGL_BLUE_BITS_ARB 0x2019
#define WGL_BLUE_SHIFT_ARB 0x201A
#define WGL_ALPHA_BITS_ARB 0x201B
#define WGL_ALPHA_SHIFT_ARB 0x201C
#define WGL_ACCUM_BITS_ARB 0x201D
#define WGL_ACCUM_RED_BITS_ARB 0x201E
#define WGL_ACCUM_GREEN_BITS_ARB 0x201F
#define WGL_ACCUM_BLUE_BITS_ARB 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB 0x2021
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_AUX_BUFFERS_ARB 0x2024
#define WGL_NO_ACCELERATION_ARB 0x2025
#define WGL_GENERIC_ACCELERATION_ARB 0x2026
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_SWAP_EXCHANGE_ARB 0x2028
#define WGL_SWAP_COPY_ARB 0x2029
#define WGL_SWAP_UNDEFINED_ARB 0x202A
#define WGL_TYPE_RGBA_ARB 0x202B
#define WGL_TYPE_COLORINDEX_ARB 0x202C
#define WGL_TYPE_RGBA_FLOAT_ARB 0x21A0
#define WGL_BIND_TO_TEXTURE_RGB_ARB 0x2070
#define WGL_BIND_TO_TEXTURE_RGBA_ARB 0x2071
#define WGL_TEXTURE_FORMAT_ARB 0x2072
#define WGL_TEXTURE_TARGET_ARB 0x2073
#define WGL_MIPMAP_TEXTURE_ARB 0x2074
#define WGL_TEXTURE_RGB_ARB 0x2075
#define WGL_TEXTURE_RGBA_ARB 0x2076
#define WGL_NO_TEXTURE_ARB 0x2077
#define WGL_TEXTURE_CUBE_MAP_ARB 0x2078
#define WGL_TEXTURE_1D_ARB 0x2079
#define WGL_TEXTURE_2D_ARB 0x207A
#define WGL_MIPMAP_LEVEL_ARB 0x207B
#define WGL_CUBE_MAP_FACE_ARB 0x207C
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x207D
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x207E
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x207F
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x2080
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x2081
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x2082
#define WGL_FRONT_LEFT_ARB 0x2083
#define WGL_FRONT_RIGHT_ARB 0x2084
#define WGL_BACK_LEFT_ARB 0x2085
#define WGL_BACK_RIGHT_ARB 0x2086
#define WGL_AUX0_ARB 0x2087
#define WGL_AUX1_ARB 0x2088
#define WGL_AUX2_ARB 0x2089
#define WGL_AUX3_ARB 0x208A
#define WGL_AUX4_ARB 0x208B
#define WGL_AUX5_ARB 0x208C
#define WGL_AUX6_ARB 0x208D
#define WGL_AUX7_ARB 0x208E
#define WGL_AUX8_ARB 0x208F
#define WGL_AUX9_ARB 0x2090
#define WGL_CONTEXT_RESET_ISOLATION_BIT_ARB 0x00000008
#define WGL_TYPE_RGBA_FLOAT_ATI 0x21A0
#define WGL_TEXTURE_RECTANGLE_ATI 0x21A5
#define WGL_COLORSPACE_EXT 0x309D
#define WGL_COLORSPACE_SRGB_EXT 0x3089
#define WGL_COLORSPACE_LINEAR_EXT 0x308A
#define WGL_CONTEXT_ES2_PROFILE_BIT_EXT 0x00000004
#define WGL_CONTEXT_ES_PROFILE_BIT_EXT 0x00000004
#define WGL_DEPTH_FLOAT_EXT 0x2040
#define WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT 0x20A9
#define ERROR_INVALID_PIXEL_TYPE_EXT 0x2043
#define WGL_SAMPLE_BUFFERS_EXT 0x2041
#define WGL_SAMPLES_EXT 0x2042
#define WGL_DRAW_TO_PBUFFER_EXT 0x202D
#define WGL_MAX_PBUFFER_PIXELS_EXT 0x202E
#define WGL_MAX_PBUFFER_WIDTH_EXT 0x202F
#define WGL_MAX_PBUFFER_HEIGHT_EXT 0x2030
#define WGL_OPTIMAL_PBUFFER_WIDTH_EXT 0x2031
#define WGL_OPTIMAL_PBUFFER_HEIGHT_EXT 0x2032
#define WGL_PBUFFER_LARGEST_EXT 0x2033
#define WGL_PBUFFER_WIDTH_EXT 0x2034
#define WGL_PBUFFER_HEIGHT_EXT 0x2035
#define WGL_NUMBER_PIXEL_FORMATS_EXT 0x2000
#define WGL_DRAW_TO_WINDOW_EXT 0x2001
#define WGL_DRAW_TO_BITMAP_EXT 0x2002
#define WGL_ACCELERATION_EXT 0x2003
#define WGL_NEED_PALETTE_EXT 0x2004
#define WGL_NEED_SYSTEM_PALETTE_EXT 0x2005
#define WGL_SWAP_LAYER_BUFFERS_EXT 0x2006
#define WGL_SWAP_METHOD_EXT 0x2007
#define WGL_NUMBER_OVERLAYS_EXT 0x2008
#define WGL_NUMBER_UNDERLAYS_EXT 0x2009
#define WGL_TRANSPARENT_EXT 0x200A
#define WGL_TRANSPARENT_VALUE_EXT 0x200B
#define WGL_SHARE_DEPTH_EXT 0x200C
#define WGL_SHARE_STENCIL_EXT 0x200D
#define WGL_SHARE_ACCUM_EXT 0x200E
#define WGL_SUPPORT_GDI_EXT 0x200F
#define WGL_SUPPORT_OPENGL_EXT 0x2010
#define WGL_DOUBLE_BUFFER_EXT 0x2011
#define WGL_STEREO_EXT 0x2012
#define WGL_PIXEL_TYPE_EXT 0x2013
#define WGL_COLOR_BITS_EXT 0x2014
#define WGL_RED_BITS_EXT 0x2015
#define WGL_RED_SHIFT_EXT 0x2016
#define WGL_GREEN_BITS_EXT 0x2017
#define WGL_GREEN_SHIFT_EXT 0x2018
#define WGL_BLUE_BITS_EXT 0x2019
#define WGL_BLUE_SHIFT_EXT 0x201A
#define WGL_ALPHA_BITS_EXT 0x201B
#define WGL_ALPHA_SHIFT_EXT 0x201C
#define WGL_ACCUM_BITS_EXT 0x201D
#define WGL_ACCUM_RED_BITS_EXT 0x201E
#define WGL_ACCUM_GREEN_BITS_EXT 0x201F
#define WGL_ACCUM_BLUE_BITS_EXT 0x2020
#define WGL_ACCUM_ALPHA_BITS_EXT 0x2021
#define WGL_DEPTH_BITS_EXT 0x2022
#define WGL_STENCIL_BITS_EXT 0x2023
#define WGL_AUX_BUFFERS_EXT 0x2024
#define WGL_NO_ACCELERATION_EXT 0x2025
#define WGL_GENERIC_ACCELERATION_EXT 0x2026
#define WGL_FULL_ACCELERATION_EXT 0x2027
#define WGL_SWAP_EXCHANGE_EXT 0x2028
#define WGL_SWAP_COPY_EXT 0x2029
#define WGL_SWAP_UNDEFINED_EXT 0x202A
#define WGL_TYPE_RGBA_EXT 0x202B
#define WGL_TYPE_COLORINDEX_EXT 0x202C
#define WGL_TYPE_RGBA_UNSIGNED_FLOAT_EXT 0x20A8
#define WGL_DIGITAL_VIDEO_CURSOR_ALPHA_FRAMEBUFFER_I3D 0x2050
#define WGL_DIGITAL_VIDEO_CURSOR_ALPHA_VALUE_I3D 0x2051
#define WGL_DIGITAL_VIDEO_CURSOR_INCLUDED_I3D 0x2052
#define WGL_DIGITAL_VIDEO_GAMMA_CORRECTED_I3D 0x2053
#define WGL_GAMMA_TABLE_SIZE_I3D 0x204E
#define WGL_GAMMA_EXCLUDE_DESKTOP_I3D 0x204F
#define WGL_GENLOCK_SOURCE_MULTIVIEW_I3D 0x2044
#define WGL_GENLOCK_SOURCE_EXTERNAL_SYNC_I3D 0x2045
#define WGL_GENLOCK_SOURCE_EXTERNAL_FIELD_I3D 0x2046
#define WGL_GENLOCK_SOURCE_EXTERNAL_TTL_I3D 0x2047
#define WGL_GENLOCK_SOURCE_DIGITAL_SYNC_I3D 0x2048
#define WGL_GENLOCK_SOURCE_DIGITAL_FIELD_I3D 0x2049
#define WGL_GENLOCK_SOURCE_EDGE_FALLING_I3D 0x204A
#define WGL_GENLOCK_SOURCE_EDGE_RISING_I3D 0x204B
#define WGL_GENLOCK_SOURCE_EDGE_BOTH_I3D 0x204C
#define WGL_IMAGE_BUFFER_MIN_ACCESS_I3D 0x00000001
#define WGL_IMAGE_BUFFER_LOCK_I3D 0x00000002
#define WGL_ACCESS_READ_ONLY_NV 0x00000000
#define WGL_ACCESS_READ_WRITE_NV 0x00000001
#define WGL_ACCESS_WRITE_DISCARD_NV 0x00000002
#define WGL_FLOAT_COMPONENTS_NV 0x20B0
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_R_NV 0x20B1
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RG_NV 0x20B2
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGB_NV 0x20B3
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGBA_NV 0x20B4
#define WGL_TEXTURE_FLOAT_R_NV 0x20B5
#define WGL_TEXTURE_FLOAT_RG_NV 0x20B6
#define WGL_TEXTURE_FLOAT_RGB_NV 0x20B7
#define WGL_TEXTURE_FLOAT_RGBA_NV 0x20B8
#define ERROR_INCOMPATIBLE_AFFINITY_MASKS_NV 0x20D0
#define ERROR_MISSING_AFFINITY_MASK_NV 0x20D1
#define WGL_CONTEXT_MULTIGPU_ATTRIB_NV 0x20AA
#define WGL_CONTEXT_MULTIGPU_ATTRIB_SINGLE_NV 0x20AB
#define WGL_CONTEXT_MULTIGPU_ATTRIB_AFR_NV 0x20AC
#define WGL_CONTEXT_MULTIGPU_ATTRIB_MULTICAST_NV 0x20AD
#define WGL_CONTEXT_MULTIGPU_ATTRIB_MULTI_DISPLAY_MULTICAST_NV 0x20AE
#define WGL_COVERAGE_SAMPLES_NV 0x2042
#define WGL_COLOR_SAMPLES_NV 0x20B9
#define WGL_NUM_VIDEO_SLOTS_NV 0x20F0
#define WGL_BIND_TO_TEXTURE_DEPTH_NV 0x20A3
#define WGL_BIND_TO_TEXTURE_RECTANGLE_DEPTH_NV 0x20A4
#define WGL_DEPTH_TEXTURE_FORMAT_NV 0x20A5
#define WGL_TEXTURE_DEPTH_COMPONENT_NV 0x20A6
#define WGL_DEPTH_COMPONENT_NV 0x20A7
#define WGL_BIND_TO_TEXTURE_RECTANGLE_RGB_NV 0x20A0
#define WGL_BIND_TO_TEXTURE_RECTANGLE_RGBA_NV 0x20A1
#define WGL_TEXTURE_RECTANGLE_NV 0x20A2
#define WGL_UNIQUE_ID_NV 0x20CE
#define WGL_NUM_VIDEO_CAPTURE_SLOTS_NV 0x20CF
#define WGL_BIND_TO_VIDEO_RGB_NV 0x20C0
#define WGL_BIND_TO_VIDEO_RGBA_NV 0x20C1
#define WGL_BIND_TO_VIDEO_RGB_AND_DEPTH_NV 0x20C2
#define WGL_VIDEO_OUT_COLOR_NV 0x20C3
#define WGL_VIDEO_OUT_ALPHA_NV 0x20C4
#define WGL_VIDEO_OUT_DEPTH_NV 0x20C5
#define WGL_VIDEO_OUT_COLOR_AND_ALPHA_NV 0x20C6
#define WGL_VIDEO_OUT_COLOR_AND_DEPTH_NV 0x20C7
#define WGL_VIDEO_OUT_FRAME 0x20C8
#define WGL_VIDEO_OUT_FIELD_1 0x20C9
#define WGL_VIDEO_OUT_FIELD_2 0x20CA
#define WGL_VIDEO_OUT_STACKED_FIELDS_1_2 0x20CB
#define WGL_VIDEO_OUT_STACKED_FIELDS_2_1 0x20CC

#define ERROR_INVALID_VERSION_ARB               0x2095
#define ERROR_INVALID_PROFILE_ARB               0x2096
#endif


namespace tl {
namespace gl {

#include "generated/opengl_all_funcs.h"

#define D(ret, name, args, params) using name##_t=ret(__stdcall*)args;
EXT_AND_OS_FUNCS
#undef D

#ifdef TL_GL_VALIDATE_EACH_CALL
#define D(ret, name, args, params) extern TL_API ret _##name args;
BASE_FUNCS
#undef D
#define D(ret, name, args, params) extern TL_API ret _##name args;
EXT_AND_OS_FUNCS
#undef D
#endif

union Functions {
	struct {
#define D(ret, name, args, params) name##_t _##name;
		EXT_AND_OS_FUNCS
#undef D
	};
	void *data[1];
};

extern TL_API Functions functions;

#ifdef TL_IMPL

#ifdef TL_GL_VALIDATE_EACH_CALL
#define D(ret, name, args, params) ret _##name args{defer{auto error=glGetError();assert(error==GL_NO_ERROR);};return ::name params;}
BASE_FUNCS
#undef D
#define D(ret, name, args, params) ret _##name args{defer{auto error=glGetError();assert(error==GL_NO_ERROR);};return functions._##name params;}
EXT_AND_OS_FUNCS
#undef D
#endif

Functions functions;

static char const *function_names[] {
#define D(ret, name, args, params) #name,
EXT_AND_OS_FUNCS
#undef D
};

#endif

#include "generated/opengl_redefine.h"

enum TextureFormat {
	Format_d24_s8,
	Format_d32,
};

struct BackBufferParams {
	TextureFormat depth_stencil_format;
};

TL_API void APIENTRY default_debug_proc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam);

TL_API BackBufferParams get_default_back_buffer_params();

using InitFlags = u32;
enum : InitFlags {
	Init_debug = 0x1,
};

TL_API bool init_opengl(NativeWindowHandle window, InitFlags flags, DEBUGPROC debug_proc, BackBufferParams back_buffer_params);
inline bool init_opengl(NativeWindowHandle window) { return init_opengl(window, false, default_debug_proc, get_default_back_buffer_params()); }
inline bool init_opengl(NativeWindowHandle window, InitFlags flags) { return init_opengl(window, flags, default_debug_proc, get_default_back_buffer_params()); }
inline bool init_opengl(NativeWindowHandle window, InitFlags flags, BackBufferParams back_buffer_params) { return init_opengl(window, flags, default_debug_proc, back_buffer_params); }

TL_API void present();
TL_API GLuint create_shader(GLenum shaderType, u32 version, bool core, Span<char> source);
TL_API GLuint create_shader(GLenum shaderType, Span<char> source);
inline GLuint create_shader(GLenum shaderType, char const *source) {
	return create_shader(shaderType, as_span(source));
}

struct ProgramStages {
	GLuint vertex;
	GLuint fragment;
	GLuint compute;
};

TL_API GLuint create_program(ProgramStages stages);

#ifdef TL_IMPL

#ifndef TL_OPENGL_DEBUG_BREAK_LEVEL
// 0 - don't break
// 1 - break only on high severerity messages
// 2 - break high+medium severerity messages
// 3 - break high+medium+low severerity messages
// 4 - break high+medium+low+notification messages
#define TL_OPENGL_DEBUG_BREAK_LEVEL 1
#endif

#ifndef TL_OPENGL_LOG_LEVEL
// 0 - don't log
// 1 - log only high severerity messages
// 2 - log high+medium severerity messages
// 3 - log high+medium+low severerity messages
// 4 - log high+medium+low+notification messages
#define TL_OPENGL_LOG_LEVEL 3
#endif

static GLuint compile_shader(GLuint shader) {
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status) {
		GLint maxLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		auto message = current_temporary_allocator.allocate<char>(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, message);

		glDeleteShader(shader);
		shader = 0;

		print(message);
	}
	return shader;
}

GLuint create_shader(GLenum shaderType, u32 version, bool core, Span<char> source) {
	StringBuilder version_builder;
	version_builder.allocator = current_temporary_allocator;
	append(version_builder, "#version "s);
	append(version_builder, version);

	if (core) {
		append(version_builder, " core"s);
	}
	append(version_builder, "\n"s);
	auto version_string = (List<char>)to_string(version_builder);

	StaticList<char, 64> stage_string;
	stage_string += "#define "s;
	switch (shaderType) {
		case GL_VERTEX_SHADER:   stage_string += "VERTEX_SHADER"s; break;
		case GL_FRAGMENT_SHADER: stage_string += "FRAGMENT_SHADER"s; break;
		case GL_COMPUTE_SHADER:  stage_string += "COMPUTE_SHADER"s; break;
		default: return 0;
	}
	stage_string += '\n';

	auto line_string = "#line 1\n"s;

	char const *lines[] {
		version_string.data,
		stage_string.data,
		line_string.data,
		source.data
	};
	int const lengths[] {
		(int)version_string.count,
		(int)stage_string.count,
		(int)line_string.count,
		(int)source.count
	};

	auto shader = glCreateShader(shaderType);
	glShaderSource(shader, (GLsizei)count_of(lines), lines, lengths);
	return compile_shader(shader);
}
GLuint create_shader(GLenum shaderType, Span<char> source) {
	auto shader = glCreateShader(shaderType);

	GLint length = (GLint)source.count;
	glShaderSource(shader, 1, &source.data, &length);

	return compile_shader(shader);
}

GLuint create_program(ProgramStages stages) {
	GLuint result = glCreateProgram();
	if (stages.vertex)   glAttachShader(result, stages.vertex);
	if (stages.fragment) glAttachShader(result, stages.fragment);
	if (stages.compute)  glAttachShader(result, stages.compute);
	glLinkProgram(result);
	if (stages.vertex)   glDetachShader(result, stages.vertex);
	if (stages.fragment) glDetachShader(result, stages.fragment);
	if (stages.compute)  glDetachShader(result, stages.compute);

	GLint status;
	glGetProgramiv(result, GL_LINK_STATUS, &status);

	if (!status) {
		GLint maxLength;
		glGetProgramiv(result, GL_INFO_LOG_LENGTH, &maxLength);

		auto message = current_temporary_allocator.allocate<char>(maxLength);
		glGetProgramInfoLog(result, maxLength, &maxLength, message);

		glDeleteProgram(result);
		result = 0;

		print(message);
	}
	return result;
}

BackBufferParams get_default_back_buffer_params() {
	BackBufferParams p;
	p.depth_stencil_format = Format_d24_s8;
	return p;
}

#if OS_WINDOWS

struct ImmediateVertex {
	v3f position;
	v2f uv;
	v4f color;
};

#if 0
static GLenum immediate_type = ~0;
static List<ImmediateVertex> immediate_vertices;
static GLuint immediate_shader;
static GLuint immediate_vertex_buffer;
static GLuint immediate_vertex_array;
#endif

static HDC client_dc;
static HGLRC context;

static StaticList<int, 16> context_attribs;

bool init_opengl(NativeWindowHandle _window, InitFlags flags, DEBUGPROC debug_proc, BackBufferParams back_buffer_params) {
	bool debug = flags & Init_debug;

	auto window = (HWND)_window;
	u32 depth_bits = 0;
	u32 stencil_bits = 0;
	switch (back_buffer_params.depth_stencil_format) {
		case Format_d32:
			depth_bits = 32;
			stencil_bits = 8;
			break;
		case Format_d24_s8:
			depth_bits = 24;
			stencil_bits = 8;
			break;
	}

	client_dc = GetDC(window);
	if (!client_dc) {
		print("GetDC failed\n");
		return false;
	}

	PIXELFORMATDESCRIPTOR dp = {};
	dp.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	dp.nVersion = 1;
	dp.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	dp.cColorBits = 32;
	dp.cAlphaBits = 8;
	dp.cDepthBits = depth_bits;
	dp.cStencilBits = stencil_bits;
	dp.iPixelType = PFD_TYPE_RGBA;
	dp.iLayerType = PFD_MAIN_PLANE;

	int index = ChoosePixelFormat(client_dc, &dp);
	if (!index) {
		auto error = GetLastError();
		print("ChoosePixelFormat failed with code 0x{} ({})\n", FormatInt{.value = error, .radix = 16}, error);
		return false;
	}

	PIXELFORMATDESCRIPTOR sp = {};
	DescribePixelFormat(client_dc, index, sizeof(sp), &sp);

	SetPixelFormat(client_dc, index, &sp);

	context = wglCreateContext(client_dc);
	if (!wglMakeCurrent(client_dc, context)) {
		print("wglMakeCurrent failed");
		return false;
	}

	assert(wglGetCurrentContext());

	static constexpr u32 function_count = sizeof(functions) / sizeof(void *);

	for (u32 function_index = 0; function_index < function_count; ++function_index) {
		char const *name = function_names[function_index];
		void *function = wglGetProcAddress(name);
		if (!function) {
			print("Failed to query '{}'\n", name);
		}
		functions.data[function_index] = function;
	}


	if (functions._wglCreateContextAttribsARB) {
#if 0
		float required_attribs_f[] = {
			0, 0
		};

#define REQUIRED_ATTRIBS \
	/*A(WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB)*/ \
	A(WGL_COLOR_BITS_ARB,     32) \
	A(WGL_ALPHA_BITS_ARB,     8) \
	A(WGL_DEPTH_BITS_ARB,     depth_bits) \
	A(WGL_STENCIL_BITS_ARB,   stencil_bits) \
	A(WGL_DOUBLE_BUFFER_ARB,  GL_TRUE)

#define A(attr, value) attr, value,
		int required_attribs_i[] = {
			REQUIRED_ATTRIBS
			0, 0
		};
#undef A

		int pixel_formats[32];
		u32 num_formats;
		bool success = wglChoosePixelFormatARB(client_dc, required_attribs_i, required_attribs_f, 1, pixel_formats, &num_formats);
		if (success) {
			if (num_formats != 0) {
				int selected_format_index = pixel_formats[0];

				PIXELFORMATDESCRIPTOR selected_format = {};
				DescribePixelFormat(client_dc, selected_format_index, sizeof(selected_format), &selected_format);
				SetPixelFormat(client_dc, selected_format_index, &selected_format);
			} else {
#define ATTRIBS \
	/*A(WGL_NUMBER_PIXEL_FORMATS_ARB)*/ \
	A(WGL_DRAW_TO_WINDOW_ARB) \
	A(WGL_DRAW_TO_BITMAP_ARB) \
	A(WGL_ACCELERATION_ARB) \
	A(WGL_NEED_PALETTE_ARB) \
	A(WGL_NEED_SYSTEM_PALETTE_ARB) \
	A(WGL_SWAP_LAYER_BUFFERS_ARB) \
	A(WGL_SWAP_METHOD_ARB) \
	A(WGL_NUMBER_OVERLAYS_ARB) \
	A(WGL_NUMBER_UNDERLAYS_ARB) \
	A(WGL_TRANSPARENT_ARB) \
	A(WGL_TRANSPARENT_RED_VALUE_ARB) \
	A(WGL_TRANSPARENT_GREEN_VALUE_ARB) \
	A(WGL_TRANSPARENT_BLUE_VALUE_ARB) \
	A(WGL_TRANSPARENT_ALPHA_VALUE_ARB) \
	A(WGL_TRANSPARENT_INDEX_VALUE_ARB) \
	A(WGL_SHARE_DEPTH_ARB) \
	A(WGL_SHARE_STENCIL_ARB) \
	A(WGL_SHARE_ACCUM_ARB) \
	A(WGL_SUPPORT_GDI_ARB) \
	/*A(WGL_SUPPORT_OPENGL_ARB)*/ \
	A(WGL_DOUBLE_BUFFER_ARB) \
	A(WGL_STEREO_ARB) \
	A(WGL_PIXEL_TYPE_ARB) \
	A(WGL_COLOR_BITS_ARB) \
	A(WGL_RED_BITS_ARB) \
	A(WGL_RED_SHIFT_ARB) \
	A(WGL_GREEN_BITS_ARB) \
	A(WGL_GREEN_SHIFT_ARB) \
	A(WGL_BLUE_BITS_ARB) \
	A(WGL_BLUE_SHIFT_ARB) \
	A(WGL_ALPHA_BITS_ARB) \
	A(WGL_ALPHA_SHIFT_ARB) \
	A(WGL_ACCUM_BITS_ARB) \
	A(WGL_ACCUM_RED_BITS_ARB) \
	A(WGL_ACCUM_GREEN_BITS_ARB) \
	A(WGL_ACCUM_BLUE_BITS_ARB) \
	A(WGL_ACCUM_ALPHA_BITS_ARB) \
	A(WGL_DEPTH_BITS_ARB) \
	A(WGL_STENCIL_BITS_ARB) \
	A(WGL_AUX_BUFFERS_ARB)

#define A(x) x,
				int attribs[] = { ATTRIBS };
#undef A
				int attrib_values[count_of(attribs)];

				bool satisfies = true;
				for (umm format_index = 1;; ++format_index) {
					if (!wglGetPixelFormatAttribivARB(client_dc, format_index, 0, count_of(attribs), attribs, attrib_values))
						break;

					satisfies = true;
					for (u32 required_attrib_index = 0; required_attrib_index < count_of(required_attribs_i); required_attrib_index += 2) {
						auto required_attrib       = required_attribs_i[required_attrib_index];
						auto required_attrib_value = required_attribs_i[required_attrib_index + 1];

						auto found_index = find(attribs, required_attrib);
						assert(found_index);
						auto index = found_index - attribs;
						if (attrib_values[index] != required_attrib_value) {
							satisfies = false;
							break;
						}
					}
					if (satisfies) {
						PIXELFORMATDESCRIPTOR selected_format = {};
						DescribePixelFormat(client_dc, format_index, sizeof(selected_format), &selected_format);
						SetPixelFormat(client_dc, format_index, &selected_format);
						break;
					}
				}
				if (!satisfies) {
					print("No mathing format found\n");
				}

#if 1
				print("wglChoosePixelFormatARB no matching format found\nAvailable formats:\n");


#define A(x) case x: return #x ## s;
				auto to_string = [](int attr) {
					switch (attr) {
						ATTRIBS
					}
				};
#undef A

				for (u32 format_index = 1;;++format_index) {
					if (!wglGetPixelFormatAttribivARB(client_dc, format_index, 0, count_of(attribs), attribs, attrib_values))
						break;

					print("Format #{}:\n", format_index);
					for (u32 attrib_index = 0; attrib_index < count_of(attribs); ++attrib_index) {
						print("{}: {}\n", to_string(attribs[attrib_index]), attrib_values[attrib_index]);
					}
					print('\n');
				}
#endif
			}
		} else {
			print("wglChoosePixelFormatARB failed\n");
		}
#endif

		HGLRC share = {};
		context_attribs = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 0,
			WGL_CONTEXT_FLAGS_ARB, debug ? WGL_CONTEXT_DEBUG_BIT_ARB : 0,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0,
		};
		HGLRC new_context = wglCreateContextAttribsARB(client_dc, share, context_attribs.data);
		if (new_context) {
			wglMakeCurrent(client_dc, new_context);
			wglDeleteContext(context);
			context = new_context;

			if (debug) {
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(debug_proc, NULL);

				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
			}
		}
	}

	//immediate_init();

	return true;
}

bool init_opengl_thread() {

	HGLRC thread_context = wglCreateContextAttribsARB(client_dc, context, context_attribs.data);
	if (!thread_context)
		return false;

	return true;
}

void APIENTRY default_debug_proc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam) {
	// ignore non-significant error/warning codes
	//if(id == 131169 || id == 131185 || id == 131218 || id == 131204)
	//	return;

	bool do_print = false;
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         if constexpr (TL_OPENGL_LOG_LEVEL > 0) do_print = true; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       if constexpr (TL_OPENGL_LOG_LEVEL > 1) do_print = true; break;
		case GL_DEBUG_SEVERITY_LOW:          if constexpr (TL_OPENGL_LOG_LEVEL > 2) do_print = true; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: if constexpr (TL_OPENGL_LOG_LEVEL > 3) do_print = true; break;
	}

	if (do_print) {
		auto time = TL_TMP(get_time_string());
		print("{} ---------------\nDebug message ({}): {}\nSource: ", time, id, message);
		switch (source) {
			case GL_DEBUG_SOURCE_API:             print("API");             break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   print("Window System");   break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: print("Shader Compiler"); break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:     print("Third Party");     break;
			case GL_DEBUG_SOURCE_APPLICATION:     print("Application");     break;
			case GL_DEBUG_SOURCE_OTHER:           print("Other");           break;
		}
		print("\nType: ");
		switch (type) {
			case GL_DEBUG_TYPE_ERROR:               print("Error");                break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: print("Deprecated Behaviour"); break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  print("Undefined Behaviour");  break;
			case GL_DEBUG_TYPE_PORTABILITY:         print("Portability");          break;
			case GL_DEBUG_TYPE_PERFORMANCE:         print("Performance");          break;
			case GL_DEBUG_TYPE_MARKER:              print("Marker");               break;
			case GL_DEBUG_TYPE_PUSH_GROUP:          print("Push Group");           break;
			case GL_DEBUG_TYPE_POP_GROUP:           print("Pop Group");            break;
			case GL_DEBUG_TYPE_OTHER:               print("Other");                break;
		}
		print("\nSeverity: ");

		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:         print("Severity: high");         break;
			case GL_DEBUG_SEVERITY_MEDIUM:       print("Severity: medium");       break;
			case GL_DEBUG_SEVERITY_LOW:          print("Severity: low");          break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: print("Severity: notification"); break;
		}
		print("\n");
	}

	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         if constexpr (TL_OPENGL_DEBUG_BREAK_LEVEL > 0) debug_break(); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       if constexpr (TL_OPENGL_DEBUG_BREAK_LEVEL > 1) debug_break(); break;
		case GL_DEBUG_SEVERITY_LOW:          if constexpr (TL_OPENGL_DEBUG_BREAK_LEVEL > 2) debug_break(); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: if constexpr (TL_OPENGL_DEBUG_BREAK_LEVEL > 3) debug_break(); break;
	}
}

void present() {
	if (!SwapBuffers(client_dc))
		print("SwapBuffers failed\n");
}

#if 0
void immediate_init() {
	immediate_vertices.allocator = current_allocator;

	auto vertex_shader = create_shader(GL_VERTEX_SHADER, R"(
#version 330 core

uniform mat4 mvp_matrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 color;

out vec2 vertex_uv;
out vec4 vertex_color;

void main() {
	gl_Position = mvp_matrix * vec4(position, 1);
	vertex_uv = uv;
	vertex_color = color;
}

)");

	auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, R"(
#version 330 core

in vec2 vertex_uv;
in vec4 vertex_color;

out vec4 color;

void main() {
	color = vertex_color;
}

)");

	immediate_shader = create_program(vertex_shader, fragment_shader);

	glGenVertexArrays(1, &immediate_vertex_array);
	glBindVertexArray(immediate_vertex_array);

	glGenBuffers(1, &immediate_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, immediate_vertex_buffer);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(ImmediateVertex), (void *)offsetof(ImmediateVertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(ImmediateVertex), (void *)offsetof(ImmediateVertex, uv));
	glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(ImmediateVertex), (void *)offsetof(ImmediateVertex, color));

	glBindVertexArray(0);

	immediate_set_matrix(m4::identity());
}
void immediate_set_matrix(m4 const &matrix) {
	glUseProgram(immediate_shader);
	glUniformMatrix4fv(glGetUniformLocation(immediate_shader, "mvp_matrix"), 1, false, matrix.s);
}

void immediate_line(v3f position_a, v2f uv_a, v4f color_a, v3f position_b, v2f uv_b, v4f color_b) {
	if (immediate_type != GL_LINES) {
		immediate_draw();
	}

	immediate_type = GL_LINES;
	immediate_vertices += {
		{ position_a, uv_a, color_a },
		{ position_b, uv_b, color_b },
	};
}
void immediate_quad(
	v3f position_a, v2f uv_a, v4f color_a,
	v3f position_b, v2f uv_b, v4f color_b,
	v3f position_c, v2f uv_c, v4f color_c,
	v3f position_d, v2f uv_d, v4f color_d
) {
	if (immediate_type != GL_QUADS) {
		immediate_draw();
	}

	immediate_type = GL_QUADS;
	immediate_vertices += {
		{ position_a, uv_a, color_a },
		{ position_b, uv_b, color_b },
		{ position_c, uv_c, color_c },
		{ position_d, uv_d, color_d },
	};
}
void immediate_quad(
	v3f position_a, v4f color_a,
	v3f position_b, v4f color_b,
	v3f position_c, v4f color_c,
	v3f position_d, v4f color_d
) {
	if (immediate_type != GL_QUADS) {
		immediate_draw();
	}

	immediate_type = GL_QUADS;
	immediate_vertices += {
		{ position_a, {0,0}, color_a },
		{ position_b, {0,1}, color_b },
		{ position_c, {1,1}, color_c },
		{ position_d, {1,0}, color_d },
	};
}
void immediate_quad(
	v4f color_a,
	v4f color_b,
	v4f color_c,
	v4f color_d
) {
	if (immediate_type != GL_QUADS) {
		immediate_draw();
	}

	immediate_type = GL_QUADS;
	immediate_vertices += {
		{ {-1,-1}, {0,0}, color_a },
		{ {-1, 1}, {0,1}, color_b },
		{ { 1, 1}, {1,1}, color_c },
		{ { 1,-1}, {1,0}, color_d },
	};
}
void immediate_draw() {
	if (immediate_vertices.size == 0) return;

	glUseProgram(immediate_shader);
	glBindVertexArray(immediate_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, immediate_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, immediate_vertices.size * sizeof(immediate_vertices[0]), immediate_vertices.data, GL_STATIC_DRAW);

	glDrawArrays(immediate_type, 0, (GLsizei)immediate_vertices.size);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	immediate_vertices.clear();
	immediate_type = ~0;
}
#endif
#endif

#endif

inline void clear_color(f32 r, f32 g, f32 b, f32 a) { ::glClearColor(r, g, b, a); }
inline void clear_color(f32 r, f32 g, f32 b) { ::glClearColor(r, g, b, 1); }
inline void clear_color(v3f color) { ::glClearColor(color.x, color.y, color.z, 1); }
inline void clear_color(v4f color) { ::glClearColor(color.x, color.y, color.z, color.w); }

inline void viewport(v2f size) { ::glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y); }
inline void viewport(v2u size) { ::glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y); }
inline void viewport(v2u position, v2u size) { ::glViewport((GLsizei)position.x, (GLsizei)position.y, (GLsizei)size.x, (GLsizei)size.y); }

inline void scissor(v2f size) { ::glScissor(0, 0, (GLsizei)size.x, (GLsizei)size.y); }
inline void scissor(v2s size) { ::glScissor(0, 0, (GLsizei)size.x, (GLsizei)size.y); }
inline void scissor(v2u size) { ::glScissor(0, 0, (GLsizei)size.x, (GLsizei)size.y); }
inline void scissor(v2f position, v2f size) { ::glScissor((GLsizei)position.x, (GLsizei)position.y, (GLsizei)size.x, (GLsizei)size.y); }
inline void scissor(v2s position, v2s size) { ::glScissor((GLsizei)position.x, (GLsizei)position.y, (GLsizei)size.x, (GLsizei)size.y); }
inline void scissor(v2u position, v2u size) { ::glScissor((GLsizei)position.x, (GLsizei)position.y, (GLsizei)size.x, (GLsizei)size.y); }

inline void set_uniform(GLuint shader, char const *name, u32 value) { glUniform1i(glGetUniformLocation(shader, name), value); }
inline void set_uniform(GLuint shader, char const *name, f32 value) { glUniform1f(glGetUniformLocation(shader, name), value); }
inline void set_uniform(GLuint shader, char const *name, v2f value) { glUniform2fv(glGetUniformLocation(shader, name), 1, value.s); }
inline void set_uniform(GLuint shader, char const *name, v3f value) { glUniform3fv(glGetUniformLocation(shader, name), 1, value.s); }
inline void set_uniform(GLuint shader, char const *name, v4f value) { glUniform4fv(glGetUniformLocation(shader, name), 1, value.s); }
inline void set_uniform(GLuint shader, char const *name, m4  value) { glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, false, value.s); }

inline void set_sampler(GLuint shader, char const *name, GLuint texture, u32 index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader, name), index);
}

inline void use_shader(GLuint shader) { glUseProgram(shader); }

template <class T, class Index = umm>
struct GrowingBuffer {
	void add(T value) {
		reserve(size + 1);
		glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
		glBufferSubData(GL_COPY_WRITE_BUFFER, size * sizeof(T), sizeof(T), &value);
		++size;
	}

	void add(Span<T> span) {
		reserve(size + span.size);
		glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
		glBufferSubData(GL_COPY_WRITE_BUFFER, size * sizeof(T), span.size * sizeof(T), span.data);
		size += span.size;
	}
	void add(std::initializer_list<T> list) { add(as_span(list)); }
	void set(Index index, T value) {
		bounds_check(assert(index < size));
		glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
		glBufferSubData(GL_COPY_WRITE_BUFFER, index * sizeof(T), sizeof(T), &value);
	}
	void set(Index start, Span<T> span) {
		bounds_check(assert(start + span.size - 1 < size));
		glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
		glBufferSubData(GL_COPY_WRITE_BUFFER, start * sizeof(T), span.size * sizeof(T), span.data);
	}
	void reset(Span<T> span) {
		reserve(span.size);
		glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
		glBufferSubData(GL_COPY_WRITE_BUFFER, 0, span.size * sizeof(T), span.data);
		size = span.size;
	}
	void init() {
		if (!buffer) {
			glGenBuffers(1, &buffer);
		}
	}
	void clear() {
		size = 0;
	}
	void reserve(Index amount) {
		if (amount <= capacity)
			return;

		Index new_capacity = max(capacity, 1);
		while (new_capacity < amount) new_capacity *= 2;

		GLuint new_buffer;
		glGenBuffers(1, &new_buffer);

		glBindBuffer(GL_COPY_WRITE_BUFFER, new_buffer);
		glBufferData(GL_COPY_WRITE_BUFFER, new_capacity * sizeof(T), 0, usage);

		glBindBuffer(GL_COPY_READ_BUFFER, buffer);

		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, capacity * sizeof(T));

		glDeleteBuffers(1, &buffer);

		buffer = new_buffer;
		capacity = new_capacity;
	}

	GrowingBuffer &operator=(Span<T> span) {
		reserve(span.size);
		size = span.size;
		set(0, span);
		return *this;
	}

	Index size = 0;
	Index capacity = 0;
	GLuint buffer = 0;
	GLenum usage = 0;
};

template <class T>
void free(GrowingBuffer<T> &buffer) {
	glDeleteBuffers(1, &buffer.buffer);
	buffer.buffer = 0;
	buffer.size = 0;
	buffer.capacity = 0;
}

}
}

#undef BASE_FUNCS
#undef EXTENSION_FUNCS
#undef WINDOWS_FUNCS
#undef EXT_AND_OS_FUNCS
#undef ALL_FUNCS

#pragma warning(pop)
