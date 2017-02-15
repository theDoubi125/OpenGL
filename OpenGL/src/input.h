#ifndef INPUT_INCLUDED
#define INPUT_INCLUDED

#include <map>
#include <glm/vec2.hpp>

class Input
{
public:
	Input();
	~Input();

	void onMouseEvent(int button, int state, int x, int y);
	void onSpecialKeyDown(int key, int x, int y);
	void onSpecialKeyUp(int key, int x, int y);

	void update();

	bool isButtonPressed(int button) const;
	bool isButtonDown(int button) const;
	bool isButtonReleased(int button) const;

	bool isKeyPressed(int key) const;
	bool isKeyDown(int key) const;
	bool isKeyReleased(int key) const;

	glm::ivec2 mousePos() const;

	static Input instance;

private:
	glm::ivec2 m_mousePos;
	int m_buttons[3];
	int m_keys[256];
	int m_time;
};

#endif