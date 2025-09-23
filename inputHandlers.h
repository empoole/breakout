#include <GLFW/glfw3.h>

#ifndef INPUT_HANDLERS
#define INPUT_HANDLERS

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void handleKeyboardEvents(GLFWwindow* window);

#endif // INPUT_HANDLERS
