#include "Rendering.h"

#include <vector>

namespace graphics
{
	GLuint createShader(GLenum type, const std::string& source)
	{
		GLuint shader = glCreateShader(type);
		const char* sourceData = source.c_str();
		GLint status;

		glShaderSource(shader, 1, &sourceData, nullptr);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			GLint infoLogLength;
			std::vector<GLchar> infoLog;
			std::string typeName;

			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			infoLog.resize(infoLogLength);
			glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog.data());

			switch (type) {
			case GL_FRAGMENT_SHADER:
				typeName = "Fragment";
				break;
			case GL_GEOMETRY_SHADER:
				typeName = "Geometry";
				break;
			case GL_VERTEX_SHADER:
				typeName = "Vertex";
				break;
			}

			throw std::runtime_error( "createShader: " + typeName + " shader: " + std::string( infoLog.begin(), infoLog.end() ) );
		}

		return shader;
	}
}