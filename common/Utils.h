#pragma once

#include <stdio.h>
#include <string>
#include <Windows.h>
#include <GLES2/gl2.h>

class Utils {
public:

	static unsigned int currentTime()
	{
		return GetTickCount();
	}

	static void showConsole()
	{
		AllocConsole();
		freopen("con", "w", stdout);
		freopen("con", "w", stderr);
	}

	static GLuint compileShader(const std::string& source, GLenum type)
	{
		auto shader = glCreateShader(type);
		auto src = source.c_str();
		glShaderSource(shader, 1, &src, NULL);
		glCompileShader(shader);
		
		GLint compiledOkay;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledOkay);

		if (compiledOkay) return shader;

		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 0)
		{
			auto infoLog = new char[infoLen];
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			printf("Error compiling %s shader:\n%s\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment", infoLog);
			delete [] infoLog;
		}
		glDeleteShader(shader);
		return 0;
	}

	static GLuint linkProgram(GLuint vs, GLuint fs)
	{
		auto program = glCreateProgram();
		
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		
		glLinkProgram(program);
		
		GLint linkedOkay;
		glGetProgramiv(program, GL_LINK_STATUS, &linkedOkay);
		if (linkedOkay) return program;
		
		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 0)
		{
			auto infoLog = new char[infoLen];
			glGetProgramInfoLog(program, infoLen, NULL, infoLog);
			printf("Error linking program:\n%s\n", infoLog);
			delete [] infoLog;
		}
		glDeleteProgram(program);
		return 0;
	}


	static GLfloat* rotationMatrix(float x, float y, float z, float angle) {
		float c = cos(angle);
		float s = sin(angle);
		GLfloat matrix[16] =
		{
			x * x * (1 - c) + c, x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, 0,
			x * y * (1 - c) + z * s, y * y * (1 - c) + c, y * z * (1 - c) - x * s, 0,
			x * z * (1 - c) - y * s, y * z * (1 - c) + x * s, z * z * (1 - c) + c, 0,
			0, 0, 0, 1
		};

		GLfloat* result = new GLfloat[16];

		for (int i = 0; i < 16; i++) {
			result[i] = matrix[i];
		}

		return result;
	}
};
