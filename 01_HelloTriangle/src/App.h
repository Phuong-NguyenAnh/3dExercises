#pragma once

#include <WindowListener.h>
#include <Graphic.h>
#include <Utils.h>
#include <string>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr auto M_PI = 3.14159265358979323846;

class App : public WindowListener
{
private:
	Graphic& mGraphic;
	int mWidth, mHeight;
	GLuint mTransformLocation;
	float mAngle;
public:
	App(Graphic& graphic, int width, int height) : mGraphic(graphic), mWidth(width), mHeight(height)
	{
		const std::string vsSource = "\
			uniform mat4 uTransform;\
			attribute vec2 a_position;\
			attribute vec4 a_color;\
			varying vec4 v_color;\
			void main()\
			{\
				v_color = a_color;\
				gl_Position = uTransform * vec4(a_position, 0.0, 1.0);\
			}";
		auto vs = Utils::compileShader(vsSource, GL_VERTEX_SHADER);
		assert(vs > 0);

		const std::string fsSource = "\
			precision mediump float;\
			varying vec4 v_color;\
			void main()\
			{\
				gl_FragColor = v_color;\
			}";
		auto fs = Utils::compileShader(fsSource, GL_FRAGMENT_SHADER);
		assert(fs > 0);

		auto program = Utils::linkProgram(vs, fs);
		assert(program >= 0);
		glUseProgram(program);
		static float positions[] = {
			0.f, .5f,
			-.5f, -.5f,
			.5f, -.5f
		};
		auto positionLocation = glGetAttribLocation(program, "a_position");
		assert(positionLocation >= 0);
		glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, positions);
		glEnableVertexAttribArray(positionLocation);

		static float colors[] = {
			1.f, 0.f, 0.f, 1.f,
			0.f, 1.f, 0.f, 1.f,
			0.f, 0.f, 1.f, 1.f,
		};
		auto colorLocation = glGetAttribLocation(program, "a_color");
		assert(colorLocation >= 0);
		glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, colors);
		glEnableVertexAttribArray(colorLocation);

		mTransformLocation = glGetUniformLocation(program, "uTransform");
		assert(mTransformLocation >= 0);
		mAngle = 0;

		glClearColor(0.f, 0.f, 0.f, 1.f);
	}
	
	bool tick()
	{
		render();
		return update();
	}

	void onResized(int newWidth, int newHeight)
	{
		mWidth = newWidth;
		mHeight = newHeight;
		glViewport(0, 0, mWidth, mHeight);
	}

private:
	bool update() {
		mAngle += 1;
		if (mAngle >= 360) mAngle -= 360;

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.f), (float)(mAngle * M_PI / 180), glm::vec3(0.f, 0.f, 1.f));
		glUniformMatrix4fv(mTransformLocation, 1, GL_FALSE, glm::value_ptr(rotationMatrix));


		return true;
	}

	void render()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		mGraphic.swapBuffers();
	}
};
