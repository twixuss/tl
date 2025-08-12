#define TL_OPENGL_BASE_FUNCS \
D(void, glBindTexture, (GLenum target, GLuint texture), (target, texture))\
D(void, glClearColor, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha), (red, green, blue, alpha))\
D(void, glGetTexImage, (GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels), (target, level, format, type, pixels))\
D(void, glScissor, (GLint x, GLint y, GLsizei width, GLsizei height), (x, y, width, height))\
D(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height), (x, y, width, height))\

#define TL_OPENGL_EXTENSION_FUNCS \
D(void, glActiveTexture, (GLenum texture), (texture))\
D(void, glActiveShaderProgram, (GLuint pipeline, GLuint program), (pipeline, program))\
D(void, glAttachShader, (GLuint program, GLuint id), (program, id))\
D(void, glBindBuffer, (GLenum target, GLuint buffer), (target, buffer))\
D(void, glBindBufferBase, (GLenum target, GLuint index, GLuint buffer), (target, index, buffer))\
D(void, glBindBufferRange, (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size), (target, index, buffer, offset, size))\
D(void, glBindFramebuffer, (GLenum target, GLuint framebuffer), (target, framebuffer))\
D(void, glBindImageTexture, (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format), (unit, texture, level, layered, layer, access, format))\
D(void, glBindRenderbuffer, (GLenum target, GLuint renderbuffer), (target, renderbuffer))\
D(void, glBindSampler, (GLuint unit, GLuint sampler), (unit, sampler))\
D(void, glBindVertexArray, (GLuint array), (array))\
D(void, glBlendEquation, (GLenum mode), (mode))\
D(void, glBlendFuncSeparate, (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha), (srcRGB, dstRGB, srcAlpha, dstAlpha))\
D(void, glBlitFramebuffer, (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter), (srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter))\
D(void, glBufferData, (GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage), (target, size, data, usage))\
D(void, glBufferSubData, (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data), (target, offset, size, data))\
D(GLenum, glCheckFramebufferStatus, (GLenum target), (target))\
D(void, glClearBufferfi, (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil), (buffer, drawbuffer, depth, stencil))\
D(void, glClearBufferfv, (GLenum buffer, GLint drawbuffer, const GLfloat * value), (buffer, drawbuffer, value))\
D(void, glClearBufferiv, (GLenum buffer, GLint drawbuffer, const GLint * value), (buffer, drawbuffer, value))\
D(void, glClearBufferuiv, (GLenum buffer, GLint drawbuffer, const GLuint * value), (buffer, drawbuffer, value))\
D(void, glClearNamedFramebufferfi, (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat depth, GLint stencil), (framebuffer, buffer, drawbuffer, depth, stencil))\
D(void, glClearNamedFramebufferfv, (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat * value), (framebuffer, buffer, drawbuffer, value))\
D(void, glClearNamedFramebufferiv, (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint * value), (framebuffer, buffer, drawbuffer, value))\
D(void, glClearNamedFramebufferuiv, (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint * value), (framebuffer, buffer, drawbuffer, value))\
D(void, glClearTexImage, (GLuint texture, GLint level, GLenum format, GLenum type, const void * data), (texture, level, format, type, data))\
D(void, glCompileShader, (GLuint id), (id))\
D(void, glCopyBufferSubData, (GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size), (readtarget, writetarget, readoffset, writeoffset, size))\
D(void, glCopyNamedBufferSubData, (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size), (readBuffer, writeBuffer, readOffset, writeOffset, size))\
D(void, glCreateBuffers, (GLsizei n, GLuint * buffers), (n, buffers))\
D(GLuint, glCreateProgram, (), ())\
D(GLuint, glCreateShader, (GLenum shaderType), (shaderType))\
D(void, glCreateVertexArrays, (GLsizei n, GLuint * arrays), (n, arrays))\
D(void, glDebugMessageCallback, (DEBUGPROC callback, void * userParam), (callback, userParam))\
D(void, glDebugMessageControl, (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled), (source, type, severity, count, ids, enabled))\
D(void, glDeleteBuffers, (GLsizei n, const GLuint * buffers), (n, buffers))\
D(void, glDeleteFramebuffers, (GLsizei n, GLuint * framebuffers), (n, framebuffers))\
D(void, glDeleteProgram, (GLuint program), (program))\
D(void, glDeleteShader, (GLuint shader), (shader))\
D(void, glDeleteVertexArrays, (GLsizei n, const GLuint * arrays), (n, arrays))\
D(void, glDetachShader, (GLuint program, GLuint shader), (program, shader))\
D(void, glDisableVertexArrayAttrib, (GLuint vaobj, GLuint index), (vaobj, index))\
D(void, glDisableVertexAttribArray, (GLuint index), (index))\
D(void, glDispatchCompute, (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z), (num_groups_x, num_groups_y, num_groups_z))\
D(void, glEnableVertexArrayAttrib, (GLuint vaobj, GLuint index), (vaobj, index))\
D(void, glEnableVertexAttribArray, (GLuint index), (index))\
D(void, glFramebufferRenderbuffer, (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer), (target, attachment, renderbuffertarget, renderbuffer))\
D(void, glFramebufferTexture1D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level), (target, attachment, textarget, texture, level))\
D(void, glFramebufferTexture2D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level), (target, attachment, textarget, texture, level))\
D(void, glFramebufferTexture3D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer), (target, attachment, textarget, texture, level, layer))\
D(void, glFramebufferTexture, (GLenum target, GLenum attachment, GLuint texture, GLint level), (target, attachment, texture, level))\
D(void, glFramebufferTextureLayer, (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer), (target, attachment, texture, level, layer))\
D(void, glGenBuffers, (GLsizei n, GLuint * buffers), (n, buffers))\
D(void, glGenFramebuffers, (GLsizei n, GLuint * ids), (n, ids))\
D(void, glGenRenderbuffers, (GLsizei n, GLuint * renderbuffers), (n, renderbuffers))\
D(void, glGenSamplers, (GLsizei n, GLuint * samplers), (n, samplers))\
D(void, glGenVertexArrays, (GLsizei n, GLuint * arrays), (n, arrays))\
D(void, glGenerateMipmap, (GLenum target), (target))\
D(void, glGenerateTextureMipmap, (GLuint texture), (texture))\
D(void, glGetActiveUniform, (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name), (program, index, bufSize, length, size, type, name))\
D(void, glGetBufferSubData, (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid * data), (target, offset, size, data))\
D(void, glGetProgramInfoLog, (GLuint program, GLsizei maxLength, GLsizei * length, GLchar * infoLog), (program, maxLength, length, infoLog))\
D(void, glGetProgramiv, (GLuint program, GLenum pname, GLint * params), (program, pname, params))\
D(void, glGetShaderInfoLog, (GLuint id, GLsizei maxLength, GLsizei * length, GLchar * infoLog), (id, maxLength, length, infoLog))\
D(void, glGetShaderiv, (GLuint id, GLenum pname, GLint * params), (id, pname, params))\
D(void, glGetTextureImage, (GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid * pixels), (texture, level, format, type, bufSize, pixels))\
D(GLint, glGetUniformLocation, (GLuint program, const GLchar * name), (program, name))\
D(GLuint, glGetUniformBlockIndex, (GLuint program, const GLchar * uniformBlockName), (program, uniformBlockName))\
D(void, glLinkProgram, (GLuint program), (program))\
D(void *, glMapBuffer, (GLenum target, GLenum access), (target, access))\
D(void *, glMapBufferRange, (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access), (target, offset, length, access))\
D(void *, glMapNamedBuffer, (GLuint buffer, GLenum access), (buffer, access))\
D(void, glMemoryBarrier, (GLbitfield barriers), (barriers))\
D(void, glNamedBufferData, (GLuint buffer, GLsizeiptr size, const void * data, GLenum usage), (buffer, size, data, usage))\
D(void, glNamedBufferStorage, (GLuint buffer, GLsizeiptr size, const void * data, GLbitfield flags), (buffer, size, data, flags))\
D(void, glNamedBufferSubData, (GLuint buffer, GLintptr offset, GLsizeiptr size, const void * data), (buffer, offset, size, data))\
D(void, glNamedFramebufferTexture, (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level), (framebuffer, attachment, texture, level))\
D(void, glNamedFramebufferTextureLayer, (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer), (framebuffer, attachment, texture, level, layer))\
D(void, glRenderbufferStorage, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height), (target, internalformat, width, height))\
D(void, glSamplerParameterIiv, (GLuint sampler, GLenum pname, const GLint * params), (sampler, pname, params))\
D(void, glSamplerParameterIuiv, (GLuint sampler, GLenum pname, const GLuint * params), (sampler, pname, params))\
D(void, glSamplerParameterf, (GLuint sampler, GLenum pname, GLfloat param), (sampler, pname, param))\
D(void, glSamplerParameterfv, (GLuint sampler, GLenum pname, const GLfloat * params), (sampler, pname, params))\
D(void, glSamplerParameteri, (GLuint sampler, GLenum pname, GLint param), (sampler, pname, param))\
D(void, glSamplerParameteriv, (GLuint sampler, GLenum pname, const GLint * params), (sampler, pname, params))\
D(void, glShaderSource, (GLuint id, GLsizei count, const GLchar *const * string, const GLint * length), (id, count, string, length))\
D(void, glTexImage3D, (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * data), (target, level, internalFormat, width, height, depth, border, format, type, data))\
D(void, glTexStorage2D, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height), (target, levels, internalformat, width, height))\
D(void, glUniform1f, (GLint location, GLfloat v0), (location, v0))\
D(void, glUniform1fv, (GLint location, GLsizei count, const GLfloat * value), (location, count, value))\
D(void, glUniform1i, (GLint location, GLint v0), (location, v0))\
D(void, glUniform1iv, (GLint location, GLsizei count, const GLint * value), (location, count, value))\
D(void, glUniform1ui, (GLint location, GLuint v0), (location, v0))\
D(void, glUniform1uiv, (GLint location, GLsizei count, const GLuint * value), (location, count, value))\
D(void, glUniform2f, (GLint location, GLfloat v0, GLfloat v1), (location, v0, v1))\
D(void, glUniform2fv, (GLint location, GLsizei count, const GLfloat * value), (location, count, value))\
D(void, glUniform2i, (GLint location, GLint v0, GLint v1), (location, v0, v1))\
D(void, glUniform2iv, (GLint location, GLsizei count, const GLint * value), (location, count, value))\
D(void, glUniform2ui, (GLint location, GLuint v0, GLuint v1), (location, v0, v1))\
D(void, glUniform2uiv, (GLint location, GLsizei count, const GLuint * value), (location, count, value))\
D(void, glUniform3f, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2), (location, v0, v1, v2))\
D(void, glUniform3fv, (GLint location, GLsizei count, const GLfloat * value), (location, count, value))\
D(void, glUniform3i, (GLint location, GLint v0, GLint v1, GLint v2), (location, v0, v1, v2))\
D(void, glUniform3iv, (GLint location, GLsizei count, const GLint * value), (location, count, value))\
D(void, glUniform3ui, (GLint location, GLuint v0, GLuint v1, GLuint v2), (location, v0, v1, v2))\
D(void, glUniform3uiv, (GLint location, GLsizei count, const GLuint * value), (location, count, value))\
D(void, glUniform4f, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3), (location, v0, v1, v2, v3))\
D(void, glUniform4fv, (GLint location, GLsizei count, const GLfloat * value), (location, count, value))\
D(void, glUniform4i, (GLint location, GLint v0, GLint v1, GLint v2, GLint v3), (location, v0, v1, v2, v3))\
D(void, glUniform4iv, (GLint location, GLsizei count, const GLint * value), (location, count, value))\
D(void, glUniform4ui, (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3), (location, v0, v1, v2, v3))\
D(void, glUniform4uiv, (GLint location, GLsizei count, const GLuint * value), (location, count, value))\
D(void, glUniformBlockBinding, (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding), (program, uniformBlockIndex, uniformBlockBinding))\
D(void, glUniformMatrix4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value), (location, count, transpose, value))\
D(GLboolean, glUnmapBuffer, (GLenum target), (target))\
D(GLboolean, glUnmapNamedBuffer, (GLuint buffer), (buffer))\
D(void, glUseProgram, (GLuint program), (program))\
D(void, glVertexArrayAttribBinding, (GLuint vaobj, GLuint attribindex, GLuint bindingindex), (vaobj, attribindex, bindingindex))\
D(void, glVertexArrayAttribFormat, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset), (vaobj, attribindex, size, type, normalized, relativeoffset))\
D(void, glVertexArrayAttribIFormat, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset), (vaobj, attribindex, size, type, relativeoffset))\
D(void, glVertexArrayAttribLFormat, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset), (vaobj, attribindex, size, type, relativeoffset))\
D(void, glVertexArrayElementBuffer, (GLuint vaobj, GLuint buffer), (vaobj, buffer))\
D(void, glVertexArrayVertexBuffer, (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride), (vaobj, bindingindex, buffer, offset, stride))\
D(void, glVertexAttribBinding, (GLuint attribindex, GLuint bindingindex), (attribindex, bindingindex))\
D(void, glVertexAttribFormat, (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset), (attribindex, size, type, normalized, relativeoffset))\
D(void, glVertexAttribIFormat, (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset), (attribindex, size, type, relativeoffset))\
D(void, glVertexAttribIPointer, (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer), (index, size, type, stride, pointer))\
D(void, glVertexAttribLFormat, (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset), (attribindex, size, type, relativeoffset))\
D(void, glVertexAttribPointer, (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer), (index, size, type, normalized, stride, pointer))\

#define TL_OPENGL_WINDOWS_FUNCS \
D(BOOL, wglChoosePixelFormatARB, (HDC hdc, const int * piAttribIList, const FLOAT * pfAttribFList, UINT nMaxFormats, int * piFormats, UINT * nNumFormats), (hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats))\
D(HGLRC, wglCreateContextAttribsARB, (HDC hDC, HGLRC hShareContext, const int * attribList), (hDC, hShareContext, attribList))\
D(BOOL, wglGetPixelFormatAttribivARB, (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int * piAttributes, int * piValues), (hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues))\
D(BOOL, wglSwapIntervalEXT, (int interval), (interval))\
