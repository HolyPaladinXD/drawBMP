#pragma once
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>

class BMP_Reader {
	std::ifstream in;
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmifh;
	std::vector<std::vector<RGBQUAD>>pixels;

public:
	int openBMP(const std::string& fileName);
	void displayBMP();
	void closeBMP();
	bool isBlackOrWhite(RGBQUAD& pixel);
	~BMP_Reader();

};