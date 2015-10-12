#pragma once

#include <GL\glew.h>
#include <GL\GL.h>

#include <string>
#include <stdexcept>

namespace graphics
{
	GLuint createShader(GLenum type, const std::string& source);

	template<typename InputIterator>
	GLuint createProgram(InputIterator first, InputIterator last)
	{
		GLuint program = glCreateProgram();
		GLint status;

		while (first != last) {
			glAttachShader(program, *first);
			++first;
		}
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			GLint infoLogLength;
			GLchar* infoLog;
			std::runtime_error exception( "" );

			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
			infoLog = new GLchar[infoLogLength];
			glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog);

			exception = std::runtime_error(std::string("createProgram: ") + std::string(infoLog));
			delete infoLog;
			throw exception;
		}
		return program;
	}

	template<typename InputIterator>
	GLuint createBuffer(GLenum type, InputIterator first, InputIterator last, GLenum usage)
	{
		GLuint buffer;
		GLsizeiptr bufferSize = (last - first) * sizeof(typename InputIterator::value_type);

		glGenBuffers(1, &buffer);
		glBindBuffer(type, buffer);

		glBufferData(type, bufferSize, &(*first), usage);

		glBindBuffer(type, 0);

		return buffer;
	}
}
