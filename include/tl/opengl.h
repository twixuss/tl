#pragma once
#include "system.h"
#include "console.h"
#include "math.h"
#include "time.h"

#pragma warning(push, 0)
#ifdef TL_IMPL

#define NOMINMAX
#include <Windows.h>
#include <gl/GL.h>

#else

#define WINGDIAPI __declspec(dllimport)
#define APIENTRY __stdcall
using BOOL = int;
using HDC = struct{}*;
using HGLRC = struct{}*;
using UINT = TL::u32;
using INT = TL::s32;
using FLOAT = TL::f32;

#include <gl/GL.h>

#endif
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 4820)

#pragma comment(lib, "opengl32")
#ifdef TL_IMPL
#pragma comment(lib, "gdi32")
#endif

namespace TL {
namespace OpenGL {

using GLchar = char;
using GLsizeiptr = umm;
using GLintptr = smm;
typedef void (APIENTRY *DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam);

#define GL_TEXTURE_MAX_LEVEL              0x813D
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_DEPTH_COMPONENT32              0x81A7
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
#define GL_DEBUG_SOURCE_API               0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM     0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER   0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY       0x8249
#define GL_DEBUG_SOURCE_APPLICATION       0x824A
#define GL_DEBUG_SOURCE_OTHER             0x824B
#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  0x824E
#define GL_DEBUG_TYPE_PORTABILITY         0x824F
#define GL_DEBUG_TYPE_PERFORMANCE         0x8250
#define GL_DEBUG_TYPE_OTHER               0x8251
#define GL_DEBUG_TYPE_MARKER              0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP          0x8269
#define GL_DEBUG_TYPE_POP_GROUP           0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION    0x826B
#define GL_TEXTURE_COMPARE_MODE           0x884C
#define GL_TEXTURE_COMPARE_FUNC           0x884D
#define GL_COMPARE_REF_TO_TEXTURE         0x884E
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STATIC_DRAW                    0x88E4
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_SRC1_COLOR                     0x88F9
#define GL_ONE_MINUS_SRC1_COLOR           0x88FA
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_COPY_READ_BUFFER               0x8F36
#define GL_COPY_WRITE_BUFFER              0x8F37
#define GL_DEBUG_SEVERITY_HIGH            0x9146
#define GL_DEBUG_SEVERITY_MEDIUM          0x9147
#define GL_DEBUG_SEVERITY_LOW             0x9148
#define GL_DEBUG_OUTPUT                   0x92E0
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

#define BASE_FUNCS \
D(glCreateShader,            GLuint, (GLenum shaderType),                                                                                       (shaderType))\
D(glDeleteShader,            void,   (GLuint shader),                                                                                           (shader))\
D(glShaderSource,            void,   (GLuint id, GLsizei count, const GLchar * const *string, const GLint *length),                             (id, count, string, length))\
D(glCompileShader,           void,   (GLuint id),                                                                                               (id))\
D(glGetShaderiv,             void,   (GLuint id, GLenum pname, GLint *params),                                                                  (id, pname, params))\
D(glGetShaderInfoLog,        void,   (GLuint id, GLsizei maxLength, GLsizei *length, GLchar *infoLog),                                          (id, maxLength, length, infoLog))\
D(glCreateProgram,           GLuint, (),                                                                                                        ())\
D(glDeleteProgram,           void,   (GLuint program),                                                                                          (program))\
D(glAttachShader,            void,   (GLuint program, GLuint id),                                                                               (program, id))\
D(glDetachShader,            void,   (GLuint program, GLuint shader),                                                                           (program, shader))\
D(glLinkProgram,             void,   (GLuint program),                                                                                          (program))\
D(glGetProgramiv,            void,   (GLuint program, GLenum pname, GLint *params),                                                             (program, pname, params))\
D(glGetProgramInfoLog,       void,   (GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog),                                     (program, maxLength, length, infoLog))\
D(glGenVertexArrays,         void,   (GLsizei n, GLuint *arrays),                                                                               (n, arrays))\
D(glDeleteVertexArrays,      void,   (GLsizei n, const GLuint *arrays),                                                                         (n, arrays))\
D(glBindVertexArray,         void,   (GLuint array),                                                                                            (array))\
D(glGenBuffers,              void,   (GLsizei n, GLuint * buffers),                                                                             (n, buffers))\
D(glBindBuffer,              void,   (GLenum target, GLuint buffer),                                                                            (target, buffer))\
D(glDeleteBuffers,           void,   (GLsizei n, const GLuint *buffers),                                                                        (n, buffers))\
D(glBufferData,              void,   (GLenum target, GLsizeiptr size, const void *data, GLenum usage),                                          (target, size, data, usage))\
D(glBufferSubData,           void,   (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data),                                     (target, offset, size, data))\
D(glCopyBufferSubData,       void,   (GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size),       (readtarget, writetarget, readoffset, writeoffset, size))\
D(glVertexAttribPointer,     void,   (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer),       (index, size, type, normalized, stride, pointer))\
D(glVertexAttribIPointer,    void,   (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer),                           (index, size, type, stride, pointer))\
D(glEnableVertexAttribArray, void,   (GLuint index),                                                                                            (index))\
D(glUseProgram,              void,   (GLuint program),                                                                                          (program))\
D(glUniform1f,               void,   (GLint location, GLfloat v0),                                                                              (location, v0))\
D(glUniform2f,               void,   (GLint location, GLfloat v0, GLfloat v1),                                                                  (location, v0, v1))\
D(glUniform3f,               void,   (GLint location, GLfloat v0, GLfloat v1, GLfloat v2),                                                      (location, v0, v1, v2))\
D(glUniform4f,               void,   (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3),                                          (location, v0, v1, v2, v3))\
D(glUniform1i,               void,   (GLint location, GLint v0),                                                                                (location, v0))\
D(glUniform2i,               void,   (GLint location, GLint v0, GLint v1),                                                                      (location, v0, v1))\
D(glUniform3i,               void,   (GLint location, GLint v0, GLint v1, GLint v2),                                                            (location, v0, v1, v2))\
D(glUniform4i,               void,   (GLint location, GLint v0, GLint v1, GLint v2, GLint v3),                                                  (location, v0, v1, v2, v3))\
D(glUniform1ui,              void,   (GLint location, GLuint v0),                                                                               (location, v0))\
D(glUniform2ui,              void,   (GLint location, GLuint v0, GLuint v1),                                                                    (location, v0, v1))\
D(glUniform3ui,              void,   (GLint location, GLuint v0, GLuint v1, GLuint v2),                                                         (location, v0, v1, v2))\
D(glUniform4ui,              void,   (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3),                                              (location, v0, v1, v2, v3))\
D(glUniform1fv,              void,   (GLint location, GLsizei count, const GLfloat *value),                                                     (location, count, value))\
D(glUniform2fv,              void,   (GLint location, GLsizei count, const GLfloat *value),                                                     (location, count, value))\
D(glUniform3fv,              void,   (GLint location, GLsizei count, const GLfloat *value),                                                     (location, count, value))\
D(glUniform4fv,              void,   (GLint location, GLsizei count, const GLfloat *value),                                                     (location, count, value))\
D(glUniform1iv,              void,   (GLint location, GLsizei count, const GLint *value),                                                       (location, count, value))\
D(glUniform2iv,              void,   (GLint location, GLsizei count, const GLint *value),                                                       (location, count, value))\
D(glUniform3iv,              void,   (GLint location, GLsizei count, const GLint *value),                                                       (location, count, value))\
D(glUniform4iv,              void,   (GLint location, GLsizei count, const GLint *value),                                                       (location, count, value))\
D(glUniform1uiv,             void,   (GLint location, GLsizei count, const GLuint *value),                                                      (location, count, value))\
D(glUniform2uiv,             void,   (GLint location, GLsizei count, const GLuint *value),                                                      (location, count, value))\
D(glUniform3uiv,             void,   (GLint location, GLsizei count, const GLuint *value),                                                      (location, count, value))\
D(glUniform4uiv,             void,   (GLint location, GLsizei count, const GLuint *value),                                                      (location, count, value))\
D(glUniformMatrix4fv,        void,   (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),                                (location, count, transpose, value))\
D(glGetUniformLocation,      GLint,  (GLuint program, const GLchar *name),                                                                      (program, name))\
D(glDebugMessageCallback,    void,   (DEBUGPROC callback​, void * userParam​),                                                                    (callback​, userParam​))\
D(glDebugMessageControl,     void,   (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled),        (source, type, severity, count, ids, enabled))\
D(glGetActiveUniform,        void,   (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name), (program, index, bufSize, length, size, type, name))\
D(glGenFramebuffers,         void,   (GLsizei n, GLuint *ids),                                                                                  (n, ids))\
D(glBindFramebuffer,         void,   (GLenum target, GLuint framebuffer),                                                                       (target, framebuffer))\
D(glCheckFramebufferStatus,  GLenum, (GLenum target),                                                                                           (target))\
D(glFramebufferTexture2D,    void,   (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level),                         (target, attachment, textarget, texture, level))\
D(glGenRenderbuffers,        void,   (GLsizei n, GLuint *renderbuffers),                                                                        (n, renderbuffers))\
D(glBindRenderbuffer,        void,   (GLenum target, GLuint renderbuffer),                                                                      (target, renderbuffer))\
D(glRenderbufferStorage,     void,   (GLenum target, GLenum internalformat, GLsizei width, GLsizei height),                                     (target, internalformat, width, height))\
D(glFramebufferRenderbuffer, void,   (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer),                        (target, attachment, renderbuffertarget, renderbuffer))\
D(glGenSamplers,             void,   (GLsizei n, GLuint *samplers),                                                                             (n, samplers))\
D(glBindSampler,             void,   (GLuint unit, GLuint sampler),                                                                             (unit, sampler))\
D(glSamplerParameterf,       void,   (GLuint sampler, GLenum pname, GLfloat param),                                                             (sampler, pname, param))\
D(glSamplerParameteri,       void,   (GLuint sampler, GLenum pname, GLint param),                                                               (sampler, pname, param))\
D(glGenerateMipmap,          void,   (GLenum target),                                                                                           (target))\
D(glTexStorage2D,            void,   (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height),                     (target, levels, internalformat, width, height))\
D(glBlendFuncSeparate,       void,   (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha),                                          (srcRGB, dstRGB, srcAlpha, dstAlpha))

#if OS_WINDOWS

#define ALL_FUNCS BASE_FUNCS \
D(wglSwapIntervalEXT,           BOOL,  (int interval),                                                                                                       (interval))\
D(wglCreateContextAttribsARB,   HGLRC, (HDC hDC, HGLRC hShareContext, const int *attribList),                                                                (hDC, hShareContext, attribList))\
D(wglChoosePixelFormatARB,      BOOL,  (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats), (hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats))\
D(wglGetPixelFormatAttribivARB, BOOL,  (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues),               (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues))

#else

#define ALL_FUNCS BASE_FUNCS

#endif

#define D(name, ret, args, params) using name ## _t = ret(*)args; TL_API ret name args;
ALL_FUNCS
#undef D

enum TextureFormat {
	Format_d24_s8,
	Format_d32,
};

struct BackBufferParams {
	TextureFormat depth_stencil_format;
};

TL_API void APIENTRY default_debug_proc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam);

TL_API BackBufferParams get_default_back_buffer_params();

TL_API bool init_opengl(void *window, bool debug, DEBUGPROC debug_proc, BackBufferParams back_buffer_params);
inline bool init_opengl(void *window) { return init_opengl(window, false, default_debug_proc, get_default_back_buffer_params()); }
inline bool init_opengl(void *window, bool debug) { return init_opengl(window, debug, default_debug_proc, get_default_back_buffer_params()); }
inline bool init_opengl(void *window, bool debug, BackBufferParams back_buffer_params) { return init_opengl(window, debug, default_debug_proc, back_buffer_params); }

TL_API void present();
inline void glViewport(v2f size) { ::glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y); }
inline void glViewport(v2u size) { ::glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y); }
inline void glViewport(v2u position, v2u size) { ::glViewport((GLsizei)position.x, (GLsizei)position.y, (GLsizei)size.x, (GLsizei)size.y); }
inline void glScissor(v2f size) { ::glScissor(0, 0, (GLsizei)size.x, (GLsizei)size.y); }
inline void glScissor(v2s size) { ::glScissor(0, 0, (GLsizei)size.x, (GLsizei)size.y); }
inline void glScissor(v2u size) { ::glScissor(0, 0, (GLsizei)size.x, (GLsizei)size.y); }
inline void glScissor(v2f position, v2f size) { ::glScissor((GLsizei)position.x, (GLsizei)position.y, (GLsizei)size.x, (GLsizei)size.y); }
inline void glScissor(v2s position, v2s size) { ::glScissor((GLsizei)position.x, (GLsizei)position.y, (GLsizei)size.x, (GLsizei)size.y); }
inline void glScissor(v2u position, v2u size) { ::glScissor((GLsizei)position.x, (GLsizei)position.y, (GLsizei)size.x, (GLsizei)size.y); }
TL_API GLuint create_shader(GLenum shaderType, u32 version, bool core, Span<char> source);
TL_API GLuint create_shader(GLenum shaderType, Span<char> source);
inline GLuint create_shader(GLenum shaderType, char const *source) {
	return create_shader(shaderType, as_span(source));
}
TL_API GLuint create_program(GLuint vertexShader, GLuint fragmentShader);
#if 0
TL_API void immediate_init();
TL_API void immediate_set_matrix(m4 const &matrix);
TL_API void immediate_line(v3f position_a, v2f uv_a, v4f color_a, v3f position_b, v2f uv_b, v4f color_b);
inline void immediate_line(v3f position_a, v4f color_a, v3f position_b, v4f color_b) {
	immediate_line(position_a, {}, color_a, position_b, {}, color_b);
}
inline void immediate_line(v3f position_a, v3f position_b, v4f color) {
	immediate_line(position_a, {}, color, position_b, {}, color);
}
TL_API void immediate_draw();
#endif
inline void glClearColor(v3f color) {
	::glClearColor(color.x, color.y, color.z, 1);
}
inline void glClearColor(v4f color) {
	::glClearColor(color.x, color.y, color.z, color.w);
}

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
		bounds_check(index < size);
		glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
		glBufferSubData(GL_COPY_WRITE_BUFFER, index * sizeof(T), sizeof(T), &value);
	}
	void set(Index start, Span<T> span) {
		bounds_check(start + span.size - 1 < size);
		glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
		glBufferSubData(GL_COPY_WRITE_BUFFER, start * sizeof(T), span.size * sizeof(T), span.data);
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

namespace TL {
namespace OpenGL {

static union {
	struct {
#define D(name, ret, args, params) name##_t name;
		ALL_FUNCS
#undef D
	};
	void *data[1];
} procedures;

static char const *procedure_names[] {
#define D(name, ret, args, params) #name,
ALL_FUNCS
#undef D
};

#define D(name, ret, args, params) ret name args{return procedures.name params;}
ALL_FUNCS
#undef D

static GLuint compile_shader(GLuint shader) {
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status) {
		GLint maxLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		auto message = ALLOCATE(char, temporary_allocator, maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, message);

		glDeleteShader(shader);
		shader = 0;

		print(message);
	}
	return shader;
}

GLuint create_shader(GLenum shaderType, u32 version, bool core, Span<char> source) {
	StringBuilder version_builder;
	version_builder.allocator = temporary_allocator;
	append(version_builder, "#version "s);
	append(version_builder, version);

	if (core) {
		append(version_builder, " core"s);
	}
	append(version_builder, "\n"s);
	auto version_string = to_string(version_builder);

	StaticList<char, 64> stage_string;
	stage_string += "#define "s;
	switch (shaderType) {
		case GL_VERTEX_SHADER: stage_string += "VERTEX_SHADER"s; break;
		case GL_FRAGMENT_SHADER: stage_string += "FRAGMENT_SHADER"s; break;
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
		(int)version_string.size,
		(int)stage_string.size,
		(int)line_string.size,
		(int)source.size
	};

	auto shader = glCreateShader(shaderType);
	glShaderSource(shader, (GLsizei)count_of(lines), lines, lengths);
	return compile_shader(shader);
}
GLuint create_shader(GLenum shaderType, Span<char> source) {
	auto shader = glCreateShader(shaderType);

	GLint length = (GLint)source.size;
	glShaderSource(shader, 1, &source.data, &length);

	return compile_shader(shader);
}

GLuint create_program(GLuint vertexShader, GLuint fragmentShader) {
	GLuint result = glCreateProgram();
	glAttachShader(result, vertexShader);
	glAttachShader(result, fragmentShader);
	glLinkProgram(result);
	glDetachShader(result, vertexShader);
	glDetachShader(result, fragmentShader);

	GLint status;
	glGetProgramiv(result, GL_LINK_STATUS, &status);

	if (!status) {
		GLint maxLength;
		glGetProgramiv(result, GL_INFO_LOG_LENGTH, &maxLength);

		auto message = ALLOCATE(char, temporary_allocator, maxLength);
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

bool init_opengl(void *_window, bool debug, DEBUGPROC debug_proc, BackBufferParams back_buffer_params) {
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
		print("ChoosePixelFormat failed with code 0x% (%)\n", FormatInt(error, 16), error);
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

	static constexpr u32 procedure_count = sizeof(procedures) / sizeof(void *);

	for (u32 procedure_index = 0; procedure_index < procedure_count; ++procedure_index) {
		char const *name = procedure_names[procedure_index];
		void *procedure = wglGetProcAddress(name);
		if (!procedure) {
			print("Failed to query '%'\n", name);
		}
		procedures.data[procedure_index] = procedure;
	}


	if (procedures.wglCreateContextAttribsARB) {
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

					print("Format #%:\n", format_index);
					for (u32 attrib_index = 0; attrib_index < count_of(attribs); ++attrib_index) {
						print("%: %\n", to_string(attribs[attrib_index]), attrib_values[attrib_index]);
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
	if(id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	bool do_print = false;
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         if constexpr (TL_OPENGL_LOG_LEVEL > 0) do_print = true; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       if constexpr (TL_OPENGL_LOG_LEVEL > 1) do_print = true; break;
		case GL_DEBUG_SEVERITY_LOW:          if constexpr (TL_OPENGL_LOG_LEVEL > 2) do_print = true; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: if constexpr (TL_OPENGL_LOG_LEVEL > 3) do_print = true; break;
	}

	if (do_print) {
		auto time = with(temporary_allocator, get_time_string());
		print("% ---------------\nDebug message (%): %\nSource: ", time, id, message);
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
		print("SwapBuffers failed");
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

#undef IF_OS_WINDOWS
#undef ALL_FUNCS

}
}
#endif

#pragma warning(pop)
