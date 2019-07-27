#pragma once

#include <WindowListener.h>
#include <Graphic.h>
#include <Utils.h>
#include <string>
#include <cassert>

#include <glmath.h>

#include "Defines.h"


class App : public WindowListener
{
private:
	Graphic& mGraphic;
	int mWidth, mHeight;
	GLuint mTransformLocation;
	Matrix mTransformMatrix;
	float mDistance;
	bool mExit;
	float mAngle;
	int preMouseX, preMouseY;
public:
	App(Graphic& graphic, int width, int height) : mGraphic(graphic), mWidth(width), mHeight(height), mTransformMatrix(Matrix::identity())
	{
		CREATE_SHADER(vs, "data/vs.glsl", GL_VERTEX_SHADER)

		CREATE_SHADER(fs, "data/fs.glsl", GL_FRAGMENT_SHADER)

		auto program = Utils::linkProgram(vs, fs);
		assert(program > 0);
		glUseProgram(program);
		
		CREATE_ATRIBUTE_LOCATION(positionLocation, program, "aPosition")
		ATTRIBUTE_POINTER(positionLocation, 3, GL_FLOAT, positions)

		CREATE_ATRIBUTE_LOCATION(colorLocation, program, "aColor")
		ATTRIBUTE_POINTER(colorLocation, 3, GL_UNSIGNED_BYTE, colors)


		mTransformLocation = glGetUniformLocation(program, "uTransform");
		assert(mTransformLocation >= 0);
		mAngle = 0;
		mDistance = 0;
		mExit = false;

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glEnable(GL_DEPTH_TEST);
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

	void onKeyDown(int keyCode) {
		const float rotationStep = 2.f;
		const float distanceStep = 0.1f;

		switch (keyCode)
		{
		case VK_DOWN:
			mTransformMatrix = Matrix::rotation(rotationStep, 1.f, 0.f, 0.f) * mTransformMatrix;
			break;
		case VK_UP:
			mTransformMatrix = Matrix::rotation(-rotationStep, 1.f, 0.f, 0.f) * mTransformMatrix;
			break;
		case VK_LEFT:
			mTransformMatrix = Matrix::rotation(-rotationStep, 0.f, 1.f, 0.f) * mTransformMatrix;
			break;
		case VK_RIGHT:
			mTransformMatrix = Matrix::rotation(rotationStep, 0.f, 1.f, 0.f) * mTransformMatrix;
			break;
		case VK_SPACE:
			mTransformMatrix = Matrix::identity();
			mDistance = 0.f;
			break;
		case VK_RETURN:
			mDistance += distanceStep;
			break;
		case VK_BACK:
			mDistance -= distanceStep;
			break;
		default:
			mExit = true;
			break;
		}
	}

	void onMouseMove(int mouseButtonCode, int x, int y) {
		int tx = x - preMouseX;
		int ty = y - preMouseY;
		switch (mouseButtonCode)
		{
		case MK_LBUTTON:
			mTransformMatrix = Matrix::rotation((float)tx, 0.f, 1.f, 0.f) * Matrix::rotation((float)ty, 1.f, 0.f, 0.f) * mTransformMatrix;
			break;
		case MK_RBUTTON:
			mDistance += (float)ty / 100;
			break;
		}
		preMouseX = x;
		preMouseY = y;
	}

private:
	bool update() {
		return !mExit;
	}

	void render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const auto h = 1.f;
		const auto w = h * mWidth / mHeight;

		const auto matrix = Matrix::frustum(-w / 2, w / 2, -h / 2, h / 2, 1.f, 50.f)
			* Matrix::translate(0.f, 0.f, -4.f + mDistance)
			* mTransformMatrix;

		glUniformMatrix4fv(mTransformLocation, 1, GL_FALSE, matrix.data());

		static const GLubyte indices[] = {
			0,1,2,0,2,3,
			4,6,5,4,7,6,
			0,5,1,0,4,5,
			3,2,6,3,6,7,
			0,3,4,4,3,7,
			1,6,2,1,5,6
		};

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

		mGraphic.swapBuffers();
	}
};
