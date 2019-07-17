#pragma once

class WindowListener {
public:
	virtual bool tick() = 0;
	virtual void onResized(int newWidth, int newHeight) { }
	virtual void onKeyDown(int keycode) { }

	virtual void onMouseDown(int mouseButtonCode, int x, int y) { }
	virtual void onMouseUp(int mouseButtonCode, int x, int y) { }
	virtual void onMouseMove(int mouseButtonCode, int x, int y) { }

	virtual void onMouseScroll(int w, int l) { }
};
