#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utils.h"
#include "inputHandlers.h"
#include "graphicsBuffer.h"
#include "shaders.h"
#include "bricks.h"
#include "player.h"

/*********************
* -- GLOBAL VARS  -- *
**********************/
bool isRunning = false;
int main() {
	// GLFW Setup
	glfwSetErrorCallback(errorCallback);

	if(!glfwInit()) return -1;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "BOC", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}

	// Provide a pointer to the player instance to the keyboard handler
	Player player = {
		0, 0, // starting x and y
		28, 6, // width and height
		0, // direction
		2, // speed
		rgbToUint32(187, 187, 187) // color
	};
	glfwSetWindowUserPointer(window, &player);

	glfwSetKeyCallback(window, handleKeyboardEvents);
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	// GLEW SETUP
	GLenum err = glewInit();
	if(err != GLEW_OK) {
		fprintf(stderr, "Error initializing GLEW.\n");
		glfwTerminate();
		return -1;
	}

	int glVersion[2] = {-1, 1};
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);


	// GENERAL SETUP
	printf("Using OpenGL: %d.%d\n", glVersion[0], glVersion[1]);
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));


	// Graphics Buffer
	const size_t bufferWidth = 240;
	const size_t bufferHeight = 256;
	Buffer buffer;
	buffer.width = bufferWidth;
	buffer.height = bufferHeight;
	buffer.data = new uint32_t[buffer.width * buffer.height];

	bufferClear(&buffer, 0);

	// Create texture for presenting buffer to OpenGL
	GLuint bufferTexture;
	glGenTextures(1, &bufferTexture);
	glBindTexture(GL_TEXTURE_2D, bufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, buffer.width, buffer.height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Create vao for generating fullscreen triangle VAO
	// VAO is a Vertex Array Object, using a triange is more efficient than using a quad or another shape
	// VAOs are necessary in modern OpenGL for defining vertex attribute configurations
	GLuint fullscreenTriangeVAO;
	glGenVertexArrays(1, &fullscreenTriangeVAO);

	// Set up shaders
	GLuint shaderId = glCreateProgram();
	initShaders(shaderId);

	glLinkProgram(shaderId);

	if(!validateProgram(shaderId)) {
		fprintf(stderr, "Error validating shader.\n");
		glfwTerminate();
		glDeleteVertexArrays(1, &fullscreenTriangeVAO);
		delete buffer.data;
		return -1;
	}

	glUseProgram(shaderId);

	GLint location = glGetUniformLocation(shaderId, "buffer");
	glUniform1i(location, 0);

	/**********************
	 * -- OpenGL Setup -- *
	 **********************/
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(fullscreenTriangeVAO);

	// Prepare game
	isRunning = true;
	uint32_t clearColor = rgbToUint32(0, 0, 0);
	player.x = buffer.width / 2;
	player.y = 40;
	/******************
	* -- MAIN LOOP -- *
	*******************/
	while(!glfwWindowShouldClose(window) && isRunning) {
		bufferClear(&buffer, clearColor);

		/**************/
		/* -- Draw -- */
		drawBricks(&buffer, buffer.width, buffer.height, 10, 10);

		// Draw Player
		bufferDrawRect(&buffer, player.width, player.height, player.x, player.y, player.color);
		/**************/

		// RENDERING
        glTexSubImage2D(
            GL_TEXTURE_2D, 0, 0, 0,
            buffer.width, buffer.height,
            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
            buffer.data
        );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);		

		glfwSwapBuffers(window);

		/* -- SIMULATION -- */
		// Player
		if(player.direction != 0) {
			player.x += player.direction * player.speed;
			// Keep player within screen bounds
			if(player.x < 0) {
				player.x = 0;
			} else if(player.x + player.width > buffer.width) {
				player.x = buffer.width - player.width;
			}
		}

		// Poll
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	glDeleteVertexArrays(1, &fullscreenTriangeVAO);

	return 0;
}
