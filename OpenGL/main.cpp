#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

#include "shader.h"

using namespace std;

Shader *basicShader;

void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
}

float vertices[] = { 0, 0, 0,  0, 0.5, 0,  0.5, 0, 0 };

void render()
{
	if (basicShader == NULL)
		return;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glUseProgram(basicShader->getProgramId());
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	glutSwapBuffers();
	glUseProgram(0);
}

void update()
{
	vertices[0] += 0.001;
}

int main(int argc, char* argv[]) {
	try
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		glutInitContextVersion(3, 1);
		glutInitWindowSize(800, 600);
		glutCreateWindow("Hello, GL");
		glutReshapeFunc(changeViewPort);
		glutDisplayFunc(render);
		glutIdleFunc(update);

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			fprintf(stderr, "GLEW error");
			return 1;
		}
		basicShader = new Shader("Shaders/shader_base.vert", "Shaders/shader_base.frag");
		basicShader->load();
	}
	catch (std::exception& e)
	{
		std::cout << "Exception " << e.what() << std::endl;
	}

	glutMainLoop();

	delete basicShader;
	return 0;
}
