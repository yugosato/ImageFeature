/*
 * color_histogram.h
 *
 *  Created on: Jun 25, 2017
 *      Author: yugo
 */

#ifndef COLOR_HISTOGRAM_H_
#define COLOR_HISTOGRAM_H_

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


class Histogram
{
public:

	int rgb2bin(int red, int green, int blue)
	{
		int redNo = red / 64;
		int greenNo = green / 64;
		int blueNo = blue / 64;
		return 16 * redNo + 4 * greenNo + blueNo;
	}


	bool calcHistogram(const std::string& filename, float* histogram)
	{
		for (int i = 0; i < 64; i++)
			histogram[i] = 0.0f;

		IplImage *img = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_COLOR);
		if (img == NULL)
		{
			std::cerr << "cannot open image: " << filename << std::endl;
			return false;
		}
		else
		{
			for (int y = 0; y < img->height; y++)
			{
				uchar *pin = (uchar *) (img->imageData + y * img->widthStep);
				for (int x = 0; x < img->width; x++)
				{
					int blue = pin[3 * x + 0];
					int green = pin[3 * x + 1];
					int red = pin[3 * x + 2];

					int bin = rgb2bin(red, green, blue);
					histogram[bin] += 1.0f;
				}
			}

			float max = 0.0f;
			float min = 1000000000.0f;
			for (int i = 0; i < 64; ++i)
			{
				if (max < histogram[i])
					max = histogram[i];

				if (min > histogram[i])
					min = histogram[i];
			}

			for (int i = 0; i < 64; ++i)
				histogram[i] = (histogram[i] - min) / (max - min);

			cvReleaseImage(&img);
			return true;
		}
	}
};

#endif /* COLOR_HISTOGRAM_H_ */
