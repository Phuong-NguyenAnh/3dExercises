#pragma warning(disable : 28251)

#include <stdio.h>
#include <Windows.h>
#include <Window.h>
#include <Graphic.h>
#include <Utils.h>
#include "App.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	Utils::showConsole();

	const int WIDTH = 800, HEIGHT = 480;
	const bool RESIZABLE = true;
	Window window(hInst, WIDTH, HEIGHT, RESIZABLE, "Hello Triangle");

	Graphic graphic(window.surface());
	App app(graphic, WIDTH, HEIGHT);

	window.show(30, &app, nCmdShow);
}