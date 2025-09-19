#include "player.h"
#include "inputHandlers.h"

void handleKeyboardEvents(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Player* player = static_cast<Player*>(glfwGetWindowUserPointer(window));
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	} else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		player->direction = -1;
	} else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		player->direction = 1;
	} else if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT and action == GLFW_RELEASE) {
		player->direction = 0;
	}
}
