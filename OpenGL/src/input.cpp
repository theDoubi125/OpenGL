
#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "input.h"

Input Input::instance;

Input::Input() : m_time(0)
{
	for (int i = 0; i < 3; i++)
	{
		m_buttons[i] = 0;
	}
}

Input::~Input()
{

}

void Input::onMouseEvent(int button, int state, int x, int y)
{
	if (state == 0)
		m_buttons[button] = m_time;
	else
		m_buttons[button] = -m_time;
	m_mousePos.x = x;
	m_mousePos.y = y;
}

void Input::onSpecialKeyDown(int key, int x, int y)
{
	m_keys[key] = m_time;
}

void Input::onSpecialKeyUp(int key, int x, int y)
{
	m_keys[key] = -m_time;
}

void Input::update()
{
	m_time++;
}

bool Input::isButtonPressed(int button) const
{
	return m_buttons[button] == m_time;
}

bool Input::isButtonDown(int button) const
{
	return m_buttons[button] > 0;
}

bool Input::isButtonReleased(int button) const
{
	return m_buttons[button] == -m_time;
}

bool Input::isKeyPressed(int key) const
{
	return m_keys[key] == m_time;
}

bool Input::isKeyDown(int key) const
{
	return m_keys[key] > 0;
}

bool Input::isKeyReleased(int key) const
{
	return m_keys[key] == -m_time;
}

glm::ivec2 Input::mousePos() const
{
	return m_mousePos;
}
