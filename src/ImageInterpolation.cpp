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

	// Y component
	for (int i = 0; i < newYSize; i++) 
	{
		for (int j = 0; j < newXSize; j++) 
		{

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
	for (int i = 0; i < newYSize / 2; i++) 
	{
		for (int j = 0; j < newXSize / 2; j++) 
		{

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

	int m = 0;
	int k = 0;

	int f = 0;
	int d = 0;

	for (int i = 0; i < newYSize; i++) 
	{
		for (int j = 0; j < newXSize; j++) 
		{
			double a = i / s_vertical - floor(i / s_vertical);
			double b = j / s_horizontal - floor(j / s_horizontal);

			m = i / s_vertical;
			k = j / s_horizontal;

			f = m;
			d = k;

			//out of range protection
			if (m < ySize - 1)
				f = m + 1;
			if (k < xSize - 1)
				d = k + 1;

			Y_new[i * newXSize + j] =
				(1 - a) * (1 - b) * Y_old[m * xSize + k] +
				(1 - a) * b * Y_old[m * xSize + d] +
				a * (1 - b) * Y_old[f * xSize + k] +
				a * b * Y_old[f * xSize + d];

		}
	}
	
	for (int i = 0; i < newYSize / 2; i++) 
	{
		for (int j = 0; j < newXSize / 2; j++) 
		{
			double a = i / s_vertical - floor(i / s_vertical);
			double b = j / s_horizontal - floor(j / s_horizontal);

			m = i / s_vertical;
			k = j / s_horizontal;

			f = m;
			d = k;

			//out of range protection
			if (m < ySize / 2 - 1)
				f = m + 1;
			if (k < xSize / 2 - 1)
				d = k + 1;

			U_new[i * newXSize / 2 + j] =
				(1 - a) * (1 - b) * U_old[m * xSize / 2 + k] +
				(1 - a) * b * U_old[m * xSize / 2 + d] +
				a * (1 - b) * U_old[f * xSize / 2 + k] +
				a * b * U_old[f * xSize / 2 + d];

			V_new[i * newXSize / 2 + j] =
				(1 - a) * (1 - b) * V_old[m * xSize / 2 + k] +
				(1 - a) * b * V_old[m * xSize / 2 + d] +
				a * (1 - b) * V_old[f * xSize / 2 + k] +
				a * b * V_old[f * xSize / 2 + d];
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

void bicubicInterpolate(uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
}

void imageRotate(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	// allocate
	uchar* Y_old = new uchar[xSize * ySize]();
	char* U_old = new char[xSize * ySize / 4]();
	char* V_old = new char[xSize * ySize / 4]();

	uchar* Y_new = new uchar[xSize * ySize]();
	char* U_new = new char[xSize * ySize / 4]();
	char* V_new = new char[xSize * ySize / 4]();

	// convert from RGB to YUV
	RGBtoYUV420(input, xSize, ySize, Y_old, U_old, V_old);

	// theta calculation
	double t = 3.14 * angle / 180;

	int f = 0;
	int d = 0;

	for (int i = 0; i < ySize; i++) 
	{
		for (int j = 0; j < xSize; j++) 
		{
			f = (int)(i * cos(t) + j * sin(t) - m * sin(t) - n * cos(t) + n);
			d = (int)(j * cos(t) - i * sin(t) - m * cos(t) + n * sin(t) + m);

			if (d >= xSize || d < 0 || f >= ySize || f < 0) 
			{
				Y_new[i * xSize + j] = 0;
			}
			else 
			{
				Y_new[i * xSize + j] = Y_old[f * xSize + d];
			}
		}
	}

	for (int i = 0; i < ySize / 2; i++) 
	{
		for (int j = 0; j < xSize / 2; j++) 
		{
			f = (int)(i * cos(t) + j * sin(t) - m / 2 * sin(t) - n / 2 * cos(t) + n / 2);
			d = (int)(j * cos(t) - i * sin(t) - m / 2 * cos(t) + n / 2 * sin(t) + m / 2);

			if (d >= xSize / 2 || d < 0 || f >= ySize / 2 || f < 0) 
			{
				U_new[i * xSize / 2 + j] = 0;
				V_new[i * xSize / 2 + j] = 0;
			}
			else 
			{
				U_new[i * xSize / 2 + j] = U_old[f * xSize / 2 + d];
				V_new[i * xSize / 2 + j] = V_old[f * xSize / 2 + d];
			}
		}
	}

	// revert from YUV to RGB
	YUV420toRGB(Y_new, U_new, V_new, xSize, ySize, output);

	delete[] Y_old;
	delete[] U_old;
	delete[] V_old;

	delete[] Y_new;
	delete[] U_new;
	delete[] V_new;
}

void imageRotateBilinear(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	// allocate
	uchar* Y_old = new uchar[xSize * ySize]();
	char* U_old = new char[xSize * ySize / 4]();
	char* V_old = new char[xSize * ySize / 4]();

	uchar* Y_new = new uchar[xSize * ySize]();
	char* U_new = new char[xSize * ySize / 4]();
	char* V_new = new char[xSize * ySize / 4]();

	// convert from RGB to YUV
	RGBtoYUV420(input, xSize, ySize, Y_old, U_old, V_old);

	// theta calculation
	double t = 3.1415 * angle / 180;

	int f = 0;
	int d = 0;

	int xbase = 0;
	int ybase = 0;

	double a = 0;
	double b = 0;

	for (int i = 0; i < ySize; i++) //y
	{
		for (int j = 0; j < xSize; j++) //x
		{
			f = (int)(i * cos(t) + j * sin(t) - m * sin(t) - n * cos(t) + n);
			d = (int)(j * cos(t) - i * sin(t) - m * cos(t) + n * sin(t) + m);
			
			xbase = (int)floor(d),
			ybase = (int)floor(f);

			if (d >= xSize || d < 0 || f >= ySize || f < 0)
			{
				Y_new[i * xSize + j] = 0;
			}
			else if (d != xbase || f != ybase)
			{
				a = f - ybase,
				b = d - xbase;

				Y_new[i * xSize + j] =
					(1 - a) * (1 - b) * Y_old[ybase * xSize + xbase] +
					(1 - a) * b * Y_old[(ybase + 1) * xSize + xbase] +
					a * (1 - b) * Y_old[ybase * xSize + (xbase + 1)] +
					a * b * Y_old[(ybase + 1) * xSize + (xbase + 1)];
			}
			else
			{
				Y_new[i * xSize + j] = Y_old[(int)(f * xSize + d)];
			}
		}
	}

	for (int i = 0; i < ySize / 2; i++)
	{
		for (int j = 0; j < xSize / 2; j++)
		{
			f = (int)(i * cos(t) + j * sin(t) - m / 2 * sin(t) - n / 2 * cos(t) + n / 2);
			d = (int)(j * cos(t) - i * sin(t) - m / 2 * cos(t) + n / 2 * sin(t) + m / 2);

			if (d >= xSize / 2 || d < 0 || f >= ySize / 2 || f < 0)
			{
				U_new[i * xSize / 2 + j] = 0;
				V_new[i * xSize / 2 + j] = 0;
			}
			else if (d != floor(d) || f != floor(f))
			{
				xbase = floor(d),
				ybase = floor(f);

				a = f - ybase;
				b = d - xbase;

					U_new[i * xSize / 2 + j] =
					(1 - a) * (1 - b) * U_old[ybase * xSize / 2 + xbase] +
					(1 - a) * b * U_old[(ybase + 1) * xSize / 2 + xbase] +
					a * (1 - b) * U_old[ybase * xSize / 2 + (xbase + 1)] +
					a * b * U_old[(ybase + 1) * xSize / 2 + (xbase + 1)];

					V_new[i * xSize / 2 + j] =
					(1 - a) * (1 - b) * V_old[ybase * xSize / 2 + xbase] +
					(1 - a) * b * V_old[(ybase + 1) * xSize / 2 + xbase] +
					a * (1 - a) * V_old[ybase * xSize / 2 + (xbase + 1)] +
					a * b * V_old[(ybase + 1) * xSize / 2 + (xbase + 1)];
			}
			else
			{
				U_new[i * xSize / 2 + j] = U_old[(int)(f * xSize / 2 + d)];
				V_new[i * xSize / 2 + j] = V_old[(int)(f * xSize / 2 + d)];
			}
		}
	}
	
	// revert from YUV to RGB
	YUV420toRGB(Y_new, U_new, V_new, xSize, ySize, output);

	delete[] Y_old;
	delete[] U_old;
	delete[] V_old;

	delete[] Y_new;
	delete[] U_new;
	delete[] V_new;
}