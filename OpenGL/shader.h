#pragma once
#include <string>
class Shader
{
public:
	Shader();
	Shader(std::string const& vertexPath, std::string const& fragmentPath);
	Shader(Shader const &model);
	~Shader();
	GLuint getProgramId() const;
	void load();

protected:
	void compile(GLuint &shader, GLenum type, std::string const &source);


private:
	GLuint m_vertexId;
	GLuint m_fragmentId;
	GLuint m_programId;

	std::string m_vertexPath;
	std::string m_fragmentPath;
};

