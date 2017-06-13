#include "ImageInterpolation.h"
#include "ColorSpaces.h"
#include <math.h>


void sampleAndHold(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	// allocate

	uchar* Y_old = new uchar[xSize * ySize]();
	char* U_old = new char[xSize * ySize / 4]();
	char* V_old = new char[xSize * ySize / 4]();

	uchar* Y_new = new uchar[newXSize * newYSize]();
	char* U_new = new char[newXSize * newYSize / 4]();
	char* V_new = new char[newXSize * newYSize / 4]();

	// convert from RGB to YUV
	RGBtoYUV420(input, xSize, ySize, Y_old, U_old, V_old);

	// scale
	const double s_horizontal = (double)newXSize / xSize;
	const double s_vertical = (double)newYSize / ySize;

	// going through  matrix and calculating formula
	int m = 0;
	int k = 0;

	for (int i = 0; i < newYSize; i++) {
		for (int j = 0; j < newXSize; j++) {

			m = (i - 1) / s_vertical;
			k = (j - 1) / s_horizontal;

			if (m < ySize - 1)
				m = (i - 1) / s_vertical + 1;

			if (k < xSize - 1)
				k = (j - 1) / s_horizontal + 1;

			Y_new[i * newXSize + j] = Y_old[m * xSize + k];
		}
	}

	// for U and V components 
	for (int i = 0; i < newYSize / 2; i++) {
		for (int j = 0; j < newXSize / 2; j++) {

			m = (i - 1) / s_vertical;
			k = (j - 1) / s_horizontal;

			if (m < ySize / 2 - 1)
				m = (i - 1) / s_vertical + 1;

			if (k < xSize / 2 - 1)
				k = (j - 1) / s_horizontal + 1;

			U_new[i * newXSize / 2 + j] = U_old[m * xSize / 2 + k];
			V_new[i * newXSize / 2 + j] = V_old[m * xSize / 2 + k];
		}
	}

	
	// revert from YUV to RGB
	YUV420toRGB(Y_new, U_new, V_new, newXSize, newYSize, output);

	delete[] Y_old;
	delete[] U_old;
	delete[] V_old;

	delete[] Y_new;
	delete[] U_new;
	delete[] V_new;
}

void bilinearInterpolate(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
}

void bicubicInterpolate(uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
}

void imageRotate(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	/* TO DO */
}

void imageRotateBilinear(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	/* TO DO */
}