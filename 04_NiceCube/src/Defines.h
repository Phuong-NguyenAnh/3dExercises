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

	-1.f,  1.f, -1.f,
	 1.f,  1.f, -1.f,

	-1.f, -1.f, -1.f,
	 1.f, -1.f, -1.f,
};

static const float texCoords[] = {
	0.0, 1.0,
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,

	1.0, 1.0,
	1.0, 0.0,
	0.0, 0.0,
	0.0, 1.0,

	0.0, 0.0,
	1.0, 0.0,	

	0.0, 1.0,
	1.0, 1.0,
};

static const byte indicesSize = 36;
static const GLubyte indices[indicesSize] = {
	0,1,2, 0,2,3,
	4,6,5, 4,7,6,
	0,5,1, 0,4,5,
	3,2,6, 3,6,7,

	0,3,8, 3,8,9,
	1,2,10, 2,10,11
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
