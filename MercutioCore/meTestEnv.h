#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>

class TestEnv
{
	/*
	Tests whether the project has been set up correctly.
	Major library dependencies:
	glfw: done
	glew: done
	freetype: not started
	sfml audio: not started
	*/
public:
	TestEnv()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}
	}
};