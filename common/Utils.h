#pragma once

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <GLES2/gl2.h>

class Utils
{
public:

	static ULONGLONG currentTime()
	{
		//Warning	C28159	Consider using 'GetTickCount64' instead of 'GetTickCount'.Reason: GetTickCount overflows roughly every 49 days.Code that does not take that into account can loop indefinitely.GetTickCount64 operates on 64 bit valuesand does not have that problem
		return GetTickCount64();
	}

	static void showConsole()
	{
		AllocConsole();
		auto conOut = freopen("con", "w", stdout);
		auto contErr = freopen("con", "w", stderr);
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
			delete[] infoLog;
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
			delete[] infoLog;
		}
		glDeleteProgram(program);
		return 0;
	}

	static std::string readFile(const char* filePath)
	{
		std::ifstream t(filePath);
		std::stringstream buffer;
		buffer << t.rdbuf();
		return buffer.str();
	}

};