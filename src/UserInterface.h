#pragma once

#include <GLFW/glfw3.h>


namespace cgbv
{
	class UserInterface
	{
	public:
		UserInterface(GLFWwindow *window);
		~UserInterface();

		void render();

		void newFrame();

		void keyInput(int key, int scancode, int action, int modifier);
		void cursorPosition(double x, double y);
		void scrollInput(double x, double y);
		void charInput(unsigned int c);
		void mouseButton(int button, int action, int mods);
	};
}

