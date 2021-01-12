#pragma once
struct RECT {
	RECT() = default;
	RECT(const int Left, const int Top, const int Right, const int Bottom)
		: left(Left), top(Top), right(Right), bottom(Bottom) {}
	int left;
	int top;
	int right;
	int bottom;
};
