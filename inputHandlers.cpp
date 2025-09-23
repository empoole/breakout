#include "player.h"
#include "inputHandlers.h"

bool keys[1024];

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	} else if (action == GLFW_PRESS) {
		keys[key] = true;
	} else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

void handleKeyboardEvents(GLFWwindow* window) {
	Player* player = static_cast<Player*>(glfwGetWindowUserPointer(window));
	if (keys[GLFW_KEY_LEFT]) {
		player->direction = -1;
	} else if (keys[GLFW_KEY_RIGHT]) {
		player->direction = 1;
	} else {
		player->direction = 0;
	}
}