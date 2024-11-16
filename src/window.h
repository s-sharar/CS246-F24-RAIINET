#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>

class Xwindow {
	Display *d;
	Window w;
	int s;
	GC gc;
	unsigned long colours[10];

public:
	Xwindow(int width=580, int height=880);  // Constructor; displays the window.
	~Xwindow();                              // Destructor; destroys the window.
	Xwindow(const Xwindow&) = delete;
	Xwindow &operator=(const Xwindow&) = delete;

	enum {White=0, Black, Red, Green, Blue, Yellow, Pink, Orange}; // Available colours.

	// Draws a rectangle
	void fillRectangle(int x, int y, int width, int height, int colour=Black);

	// Draws a string
	void drawString(int x, int y,  std::string msg);

	// Draws a Circle
	void fillCircle (int x, int y, int r, int colour);
	void drawCircle (int x, int y, int r, int colour);
};

#endif
