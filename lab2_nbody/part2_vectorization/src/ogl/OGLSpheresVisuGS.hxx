/*
 * Do not remove.
 * Optimization training courses 2014 (CINES)
 * Adrien Cassagne, adrien.cassagne@cines.fr
 * This file is under CC BY-NC-ND license (http://creativecommons.org/licenses/by-nc-nd/4.0/legalcode)
 */

#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <cassert>
#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "OGLTools.h"
#include "OGLSpheresVisuGS.h"

template <typename T>
OGLSpheresVisuGS<T>::OGLSpheresVisuGS(const string winName,
                                      const int winWidth,
                                      const int winHeight,
                                      const vec_t<T> *positionsX,
                                      const vec_t<T> *positionsY,
                                      const vec_t<T> *positionsZ,
                                      const vec_t<T> *radius,
                                      const unsigned long nSpheres)
	: OGLSpheresVisu<T>(winName, winWidth, winHeight, positionsX, positionsY, positionsZ, radius, nSpheres)
{
	if(this->window)
	{
		// specify shaders path and compile them
		vector<GLenum> shadersType(3);
		vector<string> shadersFiles(3);
		shadersType[0] = GL_VERTEX_SHADER;   shadersFiles[0] = "src/ogl/shaders/vertex150.glsl";
		shadersType[1] = GL_GEOMETRY_SHADER; shadersFiles[1] = "src/ogl/shaders/geometry150.glsl";
		shadersType[2] = GL_FRAGMENT_SHADER; shadersFiles[2] = "src/ogl/shaders/fragment150.glsl";

		this->compileShaders(shadersType, shadersFiles);
	}
}

template <typename T>
OGLSpheresVisuGS<T>::~OGLSpheresVisuGS()
{
}

template <typename T>
void OGLSpheresVisuGS<T>::refreshDisplay()
{
	if(this->window)
	{
		this->updatePositions();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// use our shader program
		if(this->shaderProgramRef != 0)
			glUseProgram(this->shaderProgramRef);

		// 1rst attribute buffer : vertex positions
		int iBufferIndex;
		for(iBufferIndex = 0; iBufferIndex < 3; iBufferIndex++)
		{
			glEnableVertexAttribArray(iBufferIndex);
			glBindBuffer(GL_ARRAY_BUFFER, this->positionBufferRef[iBufferIndex]);
			glVertexAttribPointer(
					iBufferIndex, // attribute. No particular reason for 0, but must match the layout in the shader.
					1,            // size
					GL_FLOAT,     // type
					GL_FALSE,     // normalized?
					0,            // stride
					(void*)0      // array buffer offset
			);
		}

		// 2nd attribute buffer : radius
		glEnableVertexAttribArray(iBufferIndex);
		glBindBuffer(GL_ARRAY_BUFFER, this->radiusBufferRef);
		glVertexAttribPointer(
				iBufferIndex++, // attribute. No particular reason for 1, but must match the layout in the shader.
				1,              // size
				GL_FLOAT,       // type
				GL_FALSE,       // normalized?
				0,              // stride
				(void*)0        // array buffer offset
		);

		// Compute the MVP matrix from keyboard and mouse input
		this->mvp = this->control->computeViewAndProjectionMatricesFromInputs();

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(this->mvpRef, 1, GL_FALSE, &this->mvp[0][0]);

		// Draw the triangle !
		glDrawArrays(GL_POINTS, 0, this->nSpheres);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);

		// Swap front and back buffers
		glfwSwapBuffers(this->window);

		// Poll for and process events
		glfwPollEvents();

		// Sleep if necessary
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
