#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <fstream>

#include "json/json.h"
#include <json.hpp>

#include "shader.h"
#include "scene.h"
#include "entity.h"
#include "input.h"

using namespace std;
using json = nlohmann::json;

Shader *basicShader;
Scene* scene;

void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
	scene->onResize(vec2(w, h));
}


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
	Input::instance.update();
	glutPostRedisplay();
}

void onMouseEvent(int button, int state, int x, int y)
{
	Input::instance.onMouseEvent(button, state, x, y);
}

void onSpecialKeyDown(int button, int x, int y)
{
	Input::instance.onSpecialKeyDown(button, x, y);
}

void onSpecialKeyUp(int button, int x, int y)
{
	Input::instance.onSpecialKeyUp(button, x, y);
}

int main(int argc, char* argv[])
{
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
		glutMouseFunc(onMouseEvent);
		glutSpecialFunc(onSpecialKeyDown);
		glutSpecialUpFunc(onSpecialKeyUp);

		scene = new Scene(vec2(800, 600));

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			fprintf(stderr, "GLEW error");
			return 1;
		}
		scene->init(Json::Value());
	}
	catch (std::exception& e)
	{
		std::cout << "Exception " << e.what() << std::endl;
	}
	std::ifstream myfile("resources/configs/scenes/mainScene.json");
	json test;
	myfile >> test;

	std::cout << test << std::endl;

	glutMainLoop();

	delete basicShader;
	return 0;
}
