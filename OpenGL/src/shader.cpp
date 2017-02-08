#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <fstream>

#include "shader.h"

Shader::Shader() : m_vertexId(0), m_fragmentId(0), m_programId(0), m_vertexPath(), m_fragmentPath()
{
}

Shader::Shader(std::string const& vertexPath, std::string const& fragmentPath) : m_vertexId(0), m_fragmentId(0), m_programId(0), m_vertexPath(vertexPath), m_fragmentPath(fragmentPath)
{
}

Shader::Shader(Shader const& model)
{
	m_vertexPath = model.m_vertexPath;
	m_fragmentPath = model.m_fragmentPath;
	load();
}

Shader::~Shader()
{
	glDeleteShader(m_fragmentId);
	glDeleteShader(m_vertexId);
	glDeleteProgram(m_programId);
}

void Shader::load()
{
	if (glIsShader(m_fragmentId) == GL_TRUE)
		glDeleteShader(m_fragmentId);
	if (glIsShader(m_vertexId) == GL_TRUE)
		glDeleteShader(m_vertexId);
	if (glIsProgram(m_programId) == GL_TRUE)
		glDeleteProgram(m_programId);

	compile(m_fragmentId, GL_FRAGMENT_SHADER, m_fragmentPath);
	compile(m_vertexId, GL_VERTEX_SHADER, m_vertexPath);

	m_programId = glCreateProgram();

	glAttachShader(m_programId, m_vertexId);
	glAttachShader(m_programId, m_fragmentId);

	glBindAttribLocation(m_programId, 0, "in_vertex");
	glBindAttribLocation(m_programId, 1, "in_color");
	glBindAttribLocation(m_programId, 2, "in_texCoord");

	glLinkProgram(m_programId);

	GLint linkError(0);
	glGetProgramiv(m_programId, GL_LINK_STATUS, &linkError);

	if (linkError != GL_TRUE)
	{
		GLint errorSize(0);
		glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &errorSize);

		char *error = new char[errorSize + 1];
		glGetShaderInfoLog(m_programId, errorSize, &errorSize, error);
		error[errorSize] = '\0';

		throw std::exception(error);
	}
}

void Shader::compile(GLuint &shader, GLenum type, std::string const &path)
{
	shader = glCreateShader(type);

	if (shader == 0)
	{
		throw std::exception("Error : shader type not recognised ");
	}

	std::ifstream file(path.c_str());

	if (!file)
	{
		std::string message = "Error : could not open shader file " + path;
		throw std::exception(message.c_str());
	}

	std::string line;
	std::string source;

	while (getline(file, line))
		source += line + '\n';

	file.close();

	const char* sourceStr = source.c_str();

	GLint length;
	glShaderSource(shader, 1, &sourceStr, NULL);
	glCompileShader(shader);

	GLint compileError(0);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileError);

	if (compileError != GL_TRUE)
	{
		GLint errorSize(0);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorSize);

		char *error = new char[errorSize + 1];

		glGetShaderInfoLog(shader, errorSize, &errorSize, error);
		error[errorSize] = '\0';
		std::string errorMessage(error);
		throw std::exception(error);
	}
}

GLuint Shader::getProgramId() const
{
	return m_programId;
}
