#define BASE_FUNCS \
D(void, glClearColor, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha), (red, green, blue, alpha))\
D(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height), (x, y, width, height))\
D(void, glScissor, (GLint x, GLint y, GLsizei width, GLsizei height), (x, y, width, height))\
D(void, glGetTexImage, (GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels), (target, level, format, type, pixels))\
D(void, glBindTexture, (GLenum target, GLuint texture), (target, texture))\

#define EXTENSION_FUNCS \
D(void, glBlendEquation, (GLenum mode), (mode))\
D(GLuint, glCreateShader, (GLenum shaderType), (shaderType))\
D(void, glDeleteShader, (GLuint shader), (shader))\
D(void, glShaderSource, (GLuint id, GLsizei count, const GLchar *const * string, const GLint * length), (id, count, string, length))\
D(void, glCompileShader, (GLuint id), (id))\
D(void, glGetShaderiv, (GLuint id, GLenum pname, GLint * params), (id, pname, params))\
D(void, glGetShaderInfoLog, (GLuint id, GLsizei maxLength, GLsizei * length, GLchar * infoLog), (id, maxLength, length, infoLog))\
D(GLuint, glCreateProgram, (), ())\
D(void, glDeleteProgram, (GLuint program), (program))\
D(void, glAttachShader, (GLuint program, GLuint id), (program, id))\
D(void, glDetachShader, (GLuint program, GLuint shader), (program, shader))\
D(void, glLinkProgram, (GLuint program), (program))\
D(void, glGetProgramiv, (GLuint program, GLenum pname, GLint * params), (program, pname, params))\
D(void, glGetProgramInfoLog, (GLuint program, GLsizei maxLength, GLsizei * length, GLchar * infoLog), (program, maxLength, length, infoLog))\
D(void, glGenVertexArrays, (GLsizei n, GLuint * arrays), (n, arrays))\
D(void, glDeleteVertexArrays, (GLsizei n, const GLuint * arrays), (n, arrays))\
D(void, glBindVertexArray, (GLuint array), (array))\
D(void, glGenBuffers, (GLsizei n, GLuint * buffers), (n, buffers))\
D(void, glBindBuffer, (GLenum target, GLuint buffer), (target, buffer))\
D(void, glDeleteBuffers, (GLsizei n, const GLuint * buffers), (n, buffers))\
D(void, glBufferData, (GLenum target, GLsizeiptr size, const void * data, GLenum usage), (target, size, data, usage))\
D(void, glBufferSubData, (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data), (target, offset, size, data))\
D(void, glCopyBufferSubData, (GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size), (readtarget, writetarget, readoffset, writeoffset, size))\
D(void, glVertexAttribPointer, (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer), (index, size, type, normalized, stride, pointer))\
D(void, glVertexAttribIPointer, (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer), (index, size, type, stride, pointer))\
D(void, glEnableVertexAttribArray, (GLuint index), (index))\
D(void, glUseProgram, (GLuint program), (program))\
D(void, glUniform1f, (GLint location, GLfloat v0), (location, v0))\
D(void, glUniform2f, (GLint location, GLfloat v0, GLfloat v1), (location, v0, v1))\
D(void, glUniform3f, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2), (location, v0, v1, v2))\
D(void, glUniform4f, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3), (location, v0, v1, v2, v3))\
D(void, glUniform1i, (GLint location, GLint v0), (location, v0))\
D(void, glUniform2i, (GLint location, GLint v0, GLint v1), (location, v0, v1))\
D(void, glUniform3i, (GLint location, GLint v0, GLint v1, GLint v2), (location, v0, v1, v2))\
D(void, glUniform4i, (GLint location, GLint v0, GLint v1, GLint v2, GLint v3), (location, v0, v1, v2, v3))\
D(void, glUniform1ui, (GLint location, GLuint v0), (location, v0))\
D(void, glUniform2ui, (GLint location, GLuint v0, GLuint v1), (location, v0, v1))\
D(void, glUniform3ui, (GLint location, GLuint v0, GLuint v1, GLuint v2), (location, v0, v1, v2))\
D(void, glUniform4ui, (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3), (location, v0, v1, v2, v3))\
D(void, glUniform1fv, (GLint location, GLsizei count, const GLfloat * value), (location, count, value))\
D(void, glUniform2fv, (GLint location, GLsizei count, const GLfloat * value), (location, count, value))\
D(void, glUniform3fv, (GLint location, GLsizei count, const GLfloat * value), (location, count, value))\
D(void, glUniform4fv, (GLint location, GLsizei count, const GLfloat * value), (location, count, value))\
D(void, glUniform1iv, (GLint location, GLsizei count, const GLint * value), (location, count, value))\
D(void, glUniform2iv, (GLint location, GLsizei count, const GLint * value), (location, count, value))\
D(void, glUniform3iv, (GLint location, GLsizei count, const GLint * value), (location, count, value))\
D(void, glUniform4iv, (GLint location, GLsizei count, const GLint * value), (location, count, value))\
D(void, glUniform1uiv, (GLint location, GLsizei count, const GLuint * value), (location, count, value))\
D(void, glUniform2uiv, (GLint location, GLsizei count, const GLuint * value), (location, count, value))\
D(void, glUniform3uiv, (GLint location, GLsizei count, const GLuint * value), (location, count, value))\
D(void, glUniform4uiv, (GLint location, GLsizei count, const GLuint * value), (location, count, value))\
D(void, glUniformMatrix4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value), (location, count, transpose, value))\
D(GLint, glGetUniformLocation, (GLuint program, const GLchar * name), (program, name))\
D(void, glDebugMessageCallback, (DEBUGPROC callback, void * userParam), (callback, userParam))\
D(void, glDebugMessageControl, (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled), (source, type, severity, count, ids, enabled))\
D(void, glGetActiveUniform, (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name), (program, index, bufSize, length, size, type, name))\
D(void, glGenFramebuffers, (GLsizei n, GLuint * ids), (n, ids))\
D(void, glBindFramebuffer, (GLenum target, GLuint framebuffer), (target, framebuffer))\
D(GLenum, glCheckFramebufferStatus, (GLenum target), (target))\
D(void, glFramebufferTexture2D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level), (target, attachment, textarget, texture, level))\
D(void, glGenRenderbuffers, (GLsizei n, GLuint * renderbuffers), (n, renderbuffers))\
D(void, glBindRenderbuffer, (GLenum target, GLuint renderbuffer), (target, renderbuffer))\
D(void, glRenderbufferStorage, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height), (target, internalformat, width, height))\
D(void, glFramebufferRenderbuffer, (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer), (target, attachment, renderbuffertarget, renderbuffer))\
D(void, glGenSamplers, (GLsizei n, GLuint * samplers), (n, samplers))\
D(void, glBindSampler, (GLuint unit, GLuint sampler), (unit, sampler))\
D(void, glSamplerParameterf, (GLuint sampler, GLenum pname, GLfloat param), (sampler, pname, param))\
D(void, glSamplerParameteri, (GLuint sampler, GLenum pname, GLint param), (sampler, pname, param))\
D(void, glGenerateMipmap, (GLenum target), (target))\
D(void, glTexStorage2D, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height), (target, levels, internalformat, width, height))\
D(void, glBlendFuncSeparate, (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha), (srcRGB, dstRGB, srcAlpha, dstAlpha))\
D(void, glActiveTexture, (GLenum texture), (texture))\
D(GLuint, glGetUniformBlockIndex, (GLuint program, const GLchar * uniformBlockName), (program, uniformBlockName))\
D(void, glUniformBlockBinding, (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding), (program, uniformBlockIndex, uniformBlockBinding))\
D(void, glBindBufferBase, (GLenum target, GLuint index, GLuint buffer), (target, index, buffer))\
D(void, glBindBufferRange, (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size), (target, index, buffer, offset, size))\
D(void, glDispatchCompute, (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z), (num_groups_x, num_groups_y, num_groups_z))\
D(void, glMemoryBarrier, (GLbitfield barriers), (barriers))\
D(void, glGetBufferSubData, (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid * data), (target, offset, size, data))\
D(void, glBindImageTexture, (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format), (unit, texture, level, layered, layer, access, format))\
D(void *, glMapBufferRange, (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access), (target, offset, length, access))\
D(GLboolean, glUnmapBuffer, (GLenum target), (target))\
D(void, glGetTextureImage, (GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid * pixels), (texture, level, format, type, bufSize, pixels))\

#define WINDOWS_FUNCS \
D(BOOL, wglSwapIntervalEXT, (int interval), (interval))\
D(HGLRC, wglCreateContextAttribsARB, (HDC hDC, HGLRC hShareContext, const int * attribList), (hDC, hShareContext, attribList))\
D(BOOL, wglChoosePixelFormatARB, (HDC hdc, const int * piAttribIList, const FLOAT * pfAttribFList, UINT nMaxFormats, int * piFormats, UINT * nNumFormats), (hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats))\
D(BOOL, wglGetPixelFormatAttribivARB, (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int * piAttributes, int * piValues), (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues))\

#if OS_WINDOWS
#define EXT_AND_OS_FUNCS EXTENSION_FUNCS WINDOWS_FUNCS
#endif
