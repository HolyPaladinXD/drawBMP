#include "BmpReader.h"

bool BMP_Reader::isBlackOrWhite(RGBQUAD& pixel) {
 return (pixel.rgbBlue == 255 && pixel.rgbGreen == 255 && pixel.rgbBlue == 255) ||
		(pixel.rgbBlue == 0 && pixel.rgbGreen == 0 && pixel.rgbBlue == 0);
}

int BMP_Reader::openBMP(const std::string& fileName) {
	//Проверка на расширение:
	std::string check = "bmp";
	for (size_t i = 3; i > 0; --i) {
		if (fileName[fileName.size() - i] != check[check.size() - i]) {
			std::cout << "Its not a BMP file!" << std::endl;
			return -1;
		}
	}

	in.open(fileName);
	if (in.is_open()) {
		//BITMAPFILEHEADER
		in.read(reinterpret_cast<char*>(&bmfh), sizeof(bmfh));

		if (bmfh.bfType != 0x4D42) { //"BM"
			std::cout << "Its not a BMP file!" << std::endl;
			closeBMP();
			return -1;
		}

		//BITMAPINFOHEADER
		in.read(reinterpret_cast<char*>(&bmifh), sizeof(bmifh));
		if (bmifh.biBitCount != 32 && bmifh.biBitCount != 24) {
			std::cout << "Its not a 32 or 24 bits format!" << std::endl;
			closeBMP();
			return -1;
		}

		//RGBQUAD
		in.seekg(bmfh.bfOffBits, in.beg);
		int pixelBytes = bmifh.biBitCount / 8; //Количество байт в 1 пикселе
		int countBytesInStr = bmifh.biWidth * pixelBytes; //Количество байт в строке
		int offset = ((bmfh.bfSize - bmfh.bfOffBits) - (bmifh.biWidth * bmifh.biHeight * pixelBytes)) / bmifh.biHeight; //Смещение строки

		for (int i = 0; i < bmifh.biHeight; i++) {

			std::vector<BYTE> vecBytes(bmifh.biWidth * pixelBytes + offset); //Для считывания потока байтов "строки" изображения
			in.read(reinterpret_cast<char*>(vecBytes.data()), vecBytes.size());//Считываем поток байтов в вектор
			std::vector<BYTE>::iterator it = vecBytes.begin();

			std::vector<RGBQUAD> vecRGB;
			for (int j = 0; j < bmifh.biWidth; j++) {
				RGBQUAD tmp;
				tmp.rgbBlue = *it++;

				tmp.rgbGreen = *it++;

				tmp.rgbRed = *it++;

				if (!isBlackOrWhite(tmp)) {
					std::cout << "This image is not a Black-White!" << std::endl;
					return -1;
				}
				if (bmifh.biBitCount == 32) {
					tmp.rgbReserved = *it++;
				}
				vecRGB.push_back(tmp);
			}
			pixels.push_back(vecRGB);
		}
		return 0;
	}
	return -1;
}

void BMP_Reader::displayBMP() {
	if (!in.is_open()) {
		return;
	}
	for (int i = pixels.size() - 1; i >= 0; i--) {
		for (int j = 0; j < pixels[i].size(); j++) {
			if (pixels[i][j].rgbRed) {
				std::cout << "@";
			}
			else {
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
}

void BMP_Reader::closeBMP() {
	if (in.is_open()) {
		in.close();
	}
}

BMP_Reader::~BMP_Reader() {
	closeBMP();
	pixels.clear();
}