#pragma once
#include "system.h"
#include "console.h"

#pragma warning(push, 0)
#define NOMINMAX
#include <Windows.h>
#include <gl/GL.h>
#pragma warning(pop)

#pragma comment(lib, "opengl32")
#pragma comment(lib, "gdi32")

namespace TL {
namespace OpenGL {

using GLchar = char;
using GLsizeiptr = umm;
typedef void (APIENTRY *DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam);

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
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_DEBUG_SEVERITY_HIGH            0x9146
#define GL_DEBUG_SEVERITY_MEDIUM          0x9147
#define GL_DEBUG_SEVERITY_LOW             0x9148
#define GL_DEBUG_OUTPUT                   0x92E0
#if OS_WINDOWS
#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define ERROR_INVALID_VERSION_ARB               0x2095
#define ERROR_INVALID_PROFILE_ARB               0x2096
#endif

#define D(name, type) using name ## _t = type; extern TL_API name ## _t name;

D(glCreateShader, GLuint (*)(GLenum shaderType))
D(glDeleteShader, void (*)(GLuint shader))
D(glShaderSource, void (*)(GLuint id, GLsizei count, const GLchar * const *string, const GLint *length))
D(glCompileShader, void (*)(GLuint id))
D(glGetShaderiv, void (*)(GLuint id, GLenum pname, GLint *params))
D(glGetShaderInfoLog, void (*)(GLuint id, GLsizei maxLength, GLsizei *length, GLchar *infoLog))
D(glCreateProgram, GLuint (*)())
D(glDeleteProgram, void (*)(GLuint program))
D(glAttachShader, void (*)(GLuint program, GLuint id))
D(glLinkProgram, void (*)(GLuint program))
D(glGetProgramiv, void (*)(GLuint program, GLenum pname, GLint *params))
D(glGetProgramInfoLog, void (*)(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog))
D(glGenVertexArrays, void (*)(GLsizei n, GLuint *arrays))
D(glBindVertexArray, void (*)(GLuint array))
D(glGenBuffers, void (*)(GLsizei n, GLuint * buffers))
D(glBindBuffer, void (*)(GLenum target, GLuint buffer))
D(glDeleteBuffers, void (*)(GLsizei n, const GLuint * buffers))
D(glBufferData, void (*)(GLenum target, GLsizeiptr size, const void * data, GLenum usage))
D(glVertexAttribPointer, void (*)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer))
D(glEnableVertexAttribArray, void (*)(GLuint index))
D(glUseProgram, void (*)(GLuint program))
D(glUniform1f, void (*)(GLint location, GLfloat v0))
D(glUniform2f, void (*)(GLint location, GLfloat v0, GLfloat v1))
D(glUniform3f, void (*)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2))
D(glUniform4f, void (*)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3))
D(glUniform1fv, void (*)(GLint location, GLsizei count, const GLfloat *value))
D(glUniform2fv, void (*)(GLint location, GLsizei count, const GLfloat *value))
D(glUniform3fv, void (*)(GLint location, GLsizei count, const GLfloat *value))
D(glUniform4fv, void (*)(GLint location, GLsizei count, const GLfloat *value))
D(glUniformMatrix4fv, void (*)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))
D(glGetUniformLocation, GLint (*)(GLuint program, const GLchar *name))
D(glDebugMessageCallback, void (*)(DEBUGPROC callback​, void * userParam​))
D(glDebugMessageControl, void (*)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled))
#if OS_WINDOWS
D(wglSwapIntervalEXT, BOOL (*)(int interval))
D(wglCreateContextAttribsARB, HGLRC (*)(HDC hDC, HGLRC hShareContext, const int *attribList))
#endif

#undef D

struct CompiledShader {
	GLuint id;
	char *message;
	operator bool() const { return id; }
};

TL_API void APIENTRY default_debug_proc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam);

TL_API bool init_opengl(HWND window, bool debug = false, DEBUGPROC debug_proc = default_debug_proc);
TL_API void present();
TL_API void viewport(v2u size);
TL_API CompiledShader create_shader(GLenum shaderType, u32 version, bool core, Span<char const> source);
TL_API CompiledShader create_shader(GLenum shaderType, Span<char const> source);
inline CompiledShader create_shader(GLenum shaderType, char const *source) {
	return create_shader(shaderType, Span(source, length(source)));
}
TL_API CompiledShader create_program(GLuint vertexShader, GLuint fragmentShader);
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

inline void glClearColor(v3f color) {
	::glClearColor(color.x, color.y, color.z, 1);
}
inline void glClearColor(v4f color) {
	::glClearColor(color.x, color.y, color.z, color.w);
}

}
}

#ifdef TL_IMPL

namespace TL {
namespace OpenGL {

#define D(name) name ## _t name;

D(glCreateShader)
D(glDeleteShader)
D(glShaderSource)
D(glCompileShader)
D(glGetShaderiv)
D(glGetShaderInfoLog)
D(glCreateProgram)
D(glDeleteProgram)
D(glAttachShader)
D(glLinkProgram)
D(glGetProgramiv)
D(glGetProgramInfoLog)
D(glGenVertexArrays)
D(glBindVertexArray)
D(glGenBuffers)
D(glDeleteBuffers)
D(glBindBuffer)
D(glBufferData)
D(glVertexAttribPointer)
D(glEnableVertexAttribArray)
D(glUseProgram)
D(glUniform1f)
D(glUniform2f)
D(glUniform3f)
D(glUniform4f)
D(glUniform1fv)
D(glUniform2fv)
D(glUniform3fv)
D(glUniform4fv)
D(glUniformMatrix4fv)
D(glGetUniformLocation)
D(glDebugMessageCallback)
D(glDebugMessageControl)
#if OS_WINDOWS
D(wglSwapIntervalEXT)
D(wglCreateContextAttribsARB)
#endif

#undef D

void viewport(v2u size) {
	glViewport(0, 0, size.x, size.y);
}

CompiledShader compile_shader(GLuint shader) {
	CompiledShader result = {};
	result.id = shader;
    glCompileShader(result.id);

    GLint status;
	glGetShaderiv(result.id, GL_COMPILE_STATUS, &status);

    if (!status) {
		GLint maxLength;
		glGetShaderiv(result.id, GL_INFO_LOG_LENGTH, &maxLength);

		result.message = (char *)malloc(maxLength);
        glGetShaderInfoLog(result.id, maxLength, &maxLength, result.message);

		glDeleteShader(result.id);
		result.id = 0;

        print(result.message);
	}
    return result;
}

CompiledShader create_shader(GLenum shaderType, u32 version, bool core, Span<char const> source) {
	char version_string[64] = "#version ";

	toString<char>(version, [&](char const *str, umm len) {
		strncat(version_string, str, len);
	});
	if (core) {
		strcat(version_string, " core");
	}
	strcat(version_string, "\n");


	char stage_string[64] = "#define ";
	switch (shaderType) {
		case GL_VERTEX_SHADER: strcat(stage_string, "VERTEX_SHADER"); break;
		case GL_FRAGMENT_SHADER: strcat(stage_string, "FRAGMENT_SHADER"); break;
	}
	strcat(stage_string, "\n");

	char const *lines[3] {
		version_string,
		stage_string,
		source.data()
	};
	int const lengths[3] {
		(int)length(version_string),
		(int)length(stage_string),
		(int)source.size()
	};
	
	auto shader = glCreateShader(shaderType);
    glShaderSource(shader, 3, lines, lengths);
	return compile_shader(shader);
}
CompiledShader create_shader(GLenum shaderType, Span<char const> source) {
	auto shader = glCreateShader(shaderType);

	GLint length = source.size();
    glShaderSource(shader, 1, &source._begin, &length);
	
	return compile_shader(shader);
}

CompiledShader create_program(GLuint vertexShader, GLuint fragmentShader) {
	CompiledShader result = {};

    result.id = glCreateProgram();
    glAttachShader(result.id, vertexShader);
    glAttachShader(result.id, fragmentShader);
    glLinkProgram(result.id);
    
	GLint status;
	glGetProgramiv(result.id, GL_LINK_STATUS, &status);

    if (!status) {
		GLint maxLength;
		glGetProgramiv(result.id, GL_INFO_LOG_LENGTH, &maxLength);

		result.message = (char *)malloc(maxLength);
        glGetProgramInfoLog(result.id, maxLength, &maxLength, result.message);
		
		glDeleteProgram(result.id);
		result.id = 0;

        print(result.message);
	}
    return result;
}

#if OS_WINDOWS

struct ImmediateVertex {
	v3f position;
	v2f uv;
	v4f color;
};

GLenum immediate_type = ~0;
List<ImmediateVertex> immediate_vertices;
GLuint immediate_shader;
GLuint immediate_vertex_buffer;
GLuint immediate_vertex_array;

HDC client_dc;
HGLRC context;

bool init_opengl(HWND window, bool debug, DEBUGPROC debug_proc) {
	client_dc = GetDC(window);

	PIXELFORMATDESCRIPTOR dp = {};
	dp.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	dp.nVersion = 1;
	dp.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	dp.cColorBits = 32;
	dp.cAlphaBits = 8;
	dp.iPixelType = PFD_TYPE_RGBA;
	dp.iLayerType = PFD_MAIN_PLANE;

	int index = ChoosePixelFormat(client_dc, &dp);

	PIXELFORMATDESCRIPTOR sp = {};
	DescribePixelFormat(client_dc, index, sizeof(sp), &sp);

	SetPixelFormat(client_dc, index, &sp);

	context = wglCreateContext(client_dc);
	if (!wglMakeCurrent(client_dc, context)) {
		print("wglMakeCurrent failed");
		return false;
	}
	
#define D(name) \
	name = (name ## _t)wglGetProcAddress(# name); \
	if (!name) { \
		print("Failed to query '" # name "'\n"); \
	}

	D(glCreateShader)
	D(glDeleteShader)
	D(glShaderSource)
	D(glCompileShader)
	D(glGetShaderiv)
	D(glGetShaderInfoLog)
	D(glCreateProgram)
	D(glDeleteProgram)
	D(glAttachShader)
	D(glLinkProgram)
	D(glGetProgramiv)
	D(glGetProgramInfoLog)
	D(glGenVertexArrays)
	D(glBindVertexArray)
	D(glGenBuffers)
	D(glDeleteBuffers)
	D(glBindBuffer)
	D(glBufferData)
	D(glVertexAttribPointer)
	D(glEnableVertexAttribArray)
	D(glUseProgram)
	D(glUniform1f)
	D(glUniform2f)
	D(glUniform3f)
	D(glUniform4f)
	D(glUniform1fv)
	D(glUniform2fv)
	D(glUniform3fv)
	D(glUniform4fv)
	D(glUniformMatrix4fv)
	D(glGetUniformLocation)
	D(glDebugMessageCallback)
	D(glDebugMessageControl)
#if OS_WINDOWS
	D(wglSwapIntervalEXT)
	D(wglCreateContextAttribsARB)
#endif

#undef D

	if (wglCreateContextAttribsARB) {
		HGLRC share = {};
		int attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 0,
			WGL_CONTEXT_FLAGS_ARB, debug ? WGL_CONTEXT_DEBUG_BIT_ARB : 0,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0,
		};
		HGLRC new_context = wglCreateContextAttribsARB(client_dc, share, attribs);
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

	immediate_init();

	return true;
}

void APIENTRY default_debug_proc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam) {
	// ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return; 

    print("---------------\nDebug message (%): %\nSource: ", id, message);

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
	print('\n');
	debug_break();
}

void present() {
	if (!SwapBuffers(client_dc))
		print("SwapBuffers failed");
}

void immediate_init() {
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

)").id;

	auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, R"(
#version 330 core

in vec2 vertex_uv;
in vec4 vertex_color;

out vec4 color;

void main() {
	color = vertex_color;
}

)").id;

	immediate_shader = create_program(vertex_shader, fragment_shader).id;

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
	if (~immediate_type && immediate_type != GL_LINES) {
		immediate_draw();
	}

	immediate_type = GL_LINES;
	immediate_vertices += {
		{ position_a, uv_a, color_a }, 
		{ position_b, uv_b, color_b },
	};
}
void immediate_draw() {
	glUseProgram(immediate_shader);
	glBindVertexArray(immediate_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, immediate_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, immediate_vertices.size() * sizeof(immediate_vertices[0]), immediate_vertices.data(), GL_STATIC_DRAW);

	glDrawArrays(immediate_type, 0, immediate_vertices.size());
	
	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	immediate_vertices.clear();
	immediate_type = ~0;
}

#endif

}
}
#endif
