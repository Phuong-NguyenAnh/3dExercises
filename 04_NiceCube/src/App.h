#pragma once

#include <WindowListener.h>
#include <Graphic.h>
#include <Utils.h>
#include <string>
#include <cassert>

#include <glmath.h>

#include "Defines.h"

#include <Tga.h>

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
	Tga * tgaObject;
public:
	App(Graphic& graphic, int width, int height) : mGraphic(graphic), mWidth(width), mHeight(height), mTransformMatrix(Matrix::identity())
	{
		CREATE_SHADER(vs, "data/vs.glsl", GL_VERTEX_SHADER);
		CREATE_SHADER(fs, "data/fs.glsl", GL_FRAGMENT_SHADER);

		auto program = Utils::linkProgram(vs, fs);
		assert(program > 0);
		glUseProgram(program);

		CREATE_ATRIBUTE_LOCATION(positionLocation, program, "aPosition");
		ATTRIBUTE_POINTER(positionLocation, 3, GL_FLOAT, positions);

		// texture
		tgaObject = new Tga("data/ngoctrinh.tga");
		auto textureWidth = tgaObject->width();
		auto textureHeight = tgaObject->height();
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		auto internalFormat = tgaObject->hasAlpha() ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, textureWidth, textureHeight, 0, internalFormat, GL_UNSIGNED_BYTE, tgaObject->data());
		glActiveTexture(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		CREATE_ATRIBUTE_LOCATION(texCoordLocation, program, "aTexCoord");
		ATTRIBUTE_POINTER(texCoordLocation, 2, GL_FLOAT, texCoords);

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

		glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_BYTE, indices);

		mGraphic.swapBuffers();
	}
};
