#include "BmpReader.h"



int main(int argc, char* argv[]) {
	BMP_Reader bmp;
	for (size_t i = 1; i < argc; i++) {
		if (bmp.openBMP(argv[i]) != -1) {
			bmp.displayBMP();
			bmp.closeBMP();
		}
	}
	return 0;
}