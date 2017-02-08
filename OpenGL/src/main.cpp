#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <cmath>

#include "shader.h"
#include "scene.h"

using namespace std;

Shader *basicShader;

void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
}

Scene* scene;

void render()
{
	scene->render();
}

int lastFrameTime = 0;
void update()
{
	int time = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (time - lastFrameTime) / 1000.0;
	scene->update(deltaTime);
	lastFrameTime = time;
	glutPostRedisplay();
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

		scene = new Scene();


		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			fprintf(stderr, "GLEW error");
			return 1;
		}
		scene->init();
	}
	catch (std::exception& e)
	{
		std::cout << "Exception " << e.what() << std::endl;
	}

	glutMainLoop();

	delete basicShader;
	return 0;
}
