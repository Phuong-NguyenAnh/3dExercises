#pragma once

static const float positions[] = {
	-1.f,  1.f, 1.f,
	-1.f, -1.f, 1.f,
	 1.f, -1.f, 1.f,
	 1.f,  1.f, 1.f,

	-1.f,  1.f, -1.f,
	-1.f, -1.f, -1.f,
	 1.f, -1.f, -1.f,
	 1.f,  1.f, -1.f,
};

static const GLubyte colors[] = {
	 0,	0, 0,
	 0, 0, 255,
	 0, 255, 0,
	 0, 255, 255,

	 255,0,0,
	 255,0,255,
	 255,255,0,
	 255,255,255
};

#define CREATE_SHADER(name, fromFile, type)\
auto name = Utils::compileShader(Utils::readFile(fromFile), type);\
assert(name > 0);

#define CREATE_ATRIBUTE_LOCATION(name,program, attribName)\
GLint name = glGetAttribLocation(program, attribName);\
assert(name >= 0);

#define ATTRIBUTE_POINTER(position, size, type, pointer)\
glVertexAttribPointer(position, size, type, GL_TRUE, 0, pointer);\
glEnableVertexAttribArray(position);
