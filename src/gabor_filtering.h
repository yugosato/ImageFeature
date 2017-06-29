/*
 * gabor_filtering.h
 *
 *  Created on: Jun 27, 2017
 *      Author: yugo
 */

#ifndef GABOR_FILTERING_H_
#define GABOR_FILTERING_H_

#include <iostream>
#include <math.h>
#include <sstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Gabor
{
public:

	float average(const cv::Mat& mat)
	{
		float sum = 0.0f;
		for (int y = 0; y < mat.rows; ++y)
		{
			for (int x = 0; x < mat.cols; ++x)
			{
				sum += mat.at<uchar>(y, x);
			}
		}
		float average = sum / (float) (mat.rows * mat.cols);
		return average;
	}

	float standard_deviation(const cv::Mat& mat, float average)
	{
		float sum = 0.0f;
		for (int y = 0; y < mat.rows; ++y)
		{
			for (int x = 0; x < mat.cols; ++x)
			{
				sum += std::pow(((float) mat.at<uchar>(y, x) - average), 2.0);
			}
		}
		float standard_deviation = std::sqrt(sum / (float) (mat.rows * mat.cols));
		return standard_deviation;
	}

	void gabor_filtering(const cv::Mat& img, float* result)
	{
		const int GABOR_R = 9;
		const cv::Size ksize = cv::Size(GABOR_R + 1 + GABOR_R, GABOR_R + 1 + GABOR_R);
		const double sigma = (GABOR_R / CV_PI) + 2.0;
		const double lambda = GABOR_R + 1;
		const double deg30 = CV_PI / 6.0;
		const int scale = 6;

		for (int i = 0; i < scale; ++i)
		{
			cv::Mat gabor000 = cv::getGaborKernel(ksize, sigma + i, deg30 * 0, lambda, 1.0, 0.0, CV_32F);
			cv::Mat gabor030 = cv::getGaborKernel(ksize, sigma + i, deg30 * 1, lambda, 1.0, 0.0, CV_32F);
			cv::Mat gabor060 = cv::getGaborKernel(ksize, sigma + i, deg30 * 2, lambda, 1.0, 0.0, CV_32F);
			cv::Mat gabor090 = cv::getGaborKernel(ksize, sigma + i, deg30 * 3, lambda, 1.0, 0.0, CV_32F);
			cv::Mat gabor120 = cv::getGaborKernel(ksize, sigma + i, deg30 * 4, lambda, 1.0, 0.0, CV_32F);
			cv::Mat gabor150 = cv::getGaborKernel(ksize, sigma + i, deg30 * 5, lambda, 1.0, 0.0, CV_32F);

			cv::Mat dst000, dst030, dst060, dst090, dst120, dst150;
			cv::filter2D(img, dst000, CV_32F, gabor000);
			cv::filter2D(img, dst030, CV_32F, gabor030);
			cv::filter2D(img, dst060, CV_32F, gabor060);
			cv::filter2D(img, dst090, CV_32F, gabor090);
			cv::filter2D(img, dst120, CV_32F, gabor120);
			cv::filter2D(img, dst150, CV_32F, gabor150);

			float ave000 = average(dst000);
			float ave030 = average(dst030);
			float ave060 = average(dst060);
			float ave090 = average(dst090);;
			float ave120 = average(dst120);;
			float ave150 = average(dst150);

			float sdev000 = standard_deviation(dst000, ave000);
			float sdev030 = standard_deviation(dst030, ave030);
			float sdev060 = standard_deviation(dst060, ave060);
			float sdev090 = standard_deviation(dst090, ave090);
			float sdev120 = standard_deviation(dst120, ave120);
			float sdev150 = standard_deviation(dst150, ave150);

			result[i * 12 + 0] = ave000;
			result[i * 12 + 1] = sdev000;
			result[i * 12 + 2] = ave030;
			result[i * 12 + 3] = sdev030;
			result[i * 12 + 4] = ave060;
			result[i * 12 + 5] = sdev060;
			result[i * 12 + 6] = ave090;
			result[i * 12 + 7] = sdev090;
			result[i * 12 + 8] = ave120;
			result[i * 12 + 9] = sdev120;
			result[i * 12 + 10] = ave150;
			result[i * 12 + 11] = sdev150;
		}
	}

	bool calcGaborFeature(const std::string& filename, float* gaborFeature)
	{
		for (int i = 0; i < 72; i++)
			gaborFeature[i] = 0.0f;

		cv::Mat img = cv::imread(filename, 1);
		if (img.empty())
		{
			std::cerr << "cannot open image: " << filename << std::endl;
			return false;
		}
		else
		{
			cv::Mat img_gray;
			cv::cvtColor(img, img_gray, CV_BGR2GRAY);
			gabor_filtering(img_gray, gaborFeature);
			return true;
		}
	}

};

#endif /* GABOR_FILTERING_H_ */
