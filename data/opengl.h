void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
void glGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
void glBindTexture(GLenum target, GLuint texture);

EXTENSIONS

void glBlendEquation(GLenum mode);
GLuint glCreateShader(GLenum shaderType);
void glDeleteShader(GLuint shader);
void glShaderSource(GLuint id, GLsizei count, const GLchar *const *string, const GLint *length);
void glCompileShader(GLuint id);
void glGetShaderiv(GLuint id, GLenum pname, GLint *params);
void glGetShaderInfoLog(GLuint id, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
GLuint glCreateProgram();
void glDeleteProgram(GLuint program);
void glAttachShader(GLuint program, GLuint id);
void glDetachShader(GLuint program, GLuint shader);
void glLinkProgram(GLuint program);
void glGetProgramiv(GLuint program, GLenum pname, GLint *params);
void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
void glGenVertexArrays(GLsizei n, GLuint *arrays);
void glCreateVertexArrays(GLsizei n, GLuint *arrays);
void glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
void glBindVertexArray(GLuint array);
void glVertexArrayVertexBuffer(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
void glVertexArrayElementBuffer(GLuint vaobj, GLuint buffer);
void glGenBuffers(GLsizei n, GLuint * buffers);
void glCreateBuffers(GLsizei n, GLuint *buffers);
void glBindBuffer(GLenum target, GLuint buffer);
void glDeleteBuffers(GLsizei n, const GLuint *buffers);
void glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
void glNamedBufferStorage(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
void glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);
void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
void glEnableVertexAttribArray(GLuint index);
void glDisableVertexAttribArray(GLuint index);
void glEnableVertexArrayAttrib(GLuint vaobj, GLuint index);
void glDisableVertexArrayAttrib(GLuint vaobj, GLuint index);
void glVertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
void glVertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void glVertexAttribLFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void glVertexArrayAttribFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
void glVertexArrayAttribIFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void glVertexArrayAttribLFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void glVertexAttribBinding(GLuint attribindex, GLuint bindingindex);
void glVertexArrayAttribBinding(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
void glUseProgram(GLuint program);
void glUniform1f(GLint location, GLfloat v0);
void glUniform2f(GLint location, GLfloat v0, GLfloat v1);
void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void glUniform1i(GLint location, GLint v0);
void glUniform2i(GLint location, GLint v0, GLint v1);
void glUniform3i(GLint location, GLint v0, GLint v1, GLint v2);
void glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void glUniform1ui(GLint location, GLuint v0);
void glUniform2ui(GLint location, GLuint v0, GLuint v1);
void glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2);
void glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void glUniform1fv(GLint location, GLsizei count, const GLfloat *value);
void glUniform2fv(GLint location, GLsizei count, const GLfloat *value);
void glUniform3fv(GLint location, GLsizei count, const GLfloat *value);
void glUniform4fv(GLint location, GLsizei count, const GLfloat *value);
void glUniform1iv(GLint location, GLsizei count, const GLint *value);
void glUniform2iv(GLint location, GLsizei count, const GLint *value);
void glUniform3iv(GLint location, GLsizei count, const GLint *value);
void glUniform4iv(GLint location, GLsizei count, const GLint *value);
void glUniform1uiv(GLint location, GLsizei count, const GLuint *value);
void glUniform2uiv(GLint location, GLsizei count, const GLuint *value);
void glUniform3uiv(GLint location, GLsizei count, const GLuint *value);
void glUniform4uiv(GLint location, GLsizei count, const GLuint *value);
void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLint glGetUniformLocation(GLuint program, const GLchar *name);
void glDebugMessageCallback(DEBUGPROC callback, void * userParam);
void glDebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
void glGenFramebuffers(GLsizei n, GLuint *ids);
void glBindFramebuffer(GLenum target, GLuint framebuffer);
GLenum glCheckFramebufferStatus(GLenum target);
void glFramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level);
void glFramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void glFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer);
void glNamedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
void glGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
void glBindRenderbuffer(GLenum target, GLuint renderbuffer);
void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void glGenSamplers(GLsizei n, GLuint *samplers);
void glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
void glNamedFramebufferTextureLayer(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
void glBindSampler(GLuint unit, GLuint sampler);
void glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param);
void glSamplerParameteri(GLuint sampler, GLenum pname, GLint param);
void glSamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat *params);
void glSamplerParameteriv(GLuint sampler, GLenum pname, const GLint *params);
void glSamplerParameterIiv(GLuint sampler, GLenum pname, const GLint *params);
void glSamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint *params);
void glGenerateMipmap(GLenum target);
void glGenerateTextureMipmap(GLuint texture);
void glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
void glTexImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * data);
void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
void glActiveTexture(GLenum texture);
GLuint glGetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName);
void glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
void glBindBufferBase(GLenum target, GLuint index, GLuint buffer);
void glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
void glMemoryBarrier(GLbitfield barriers);
void glGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
void glBindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
void * glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
void glGetTextureImage(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid *pixels);
void *glMapBuffer(GLenum target, GLenum access);
void *glMapNamedBuffer(GLuint buffer, GLenum access);
GLboolean glUnmapBuffer(GLenum target);
GLboolean glUnmapNamedBuffer(GLuint buffer);
void glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

WINDOWS

BOOL wglSwapIntervalEXT(int interval);
HGLRC wglCreateContextAttribsARB(HDC hDC, HGLRC hShareContext, const int *attribList);
BOOL wglChoosePixelFormatARB(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
BOOL wglGetPixelFormatAttribivARB(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
