
#include <iostream>
#include <fstream>
#include "color_histogram.h"
#include "gabor_filtering.h"


void splitline(const std::string& s, const std::string& delim, std::vector<std::string>& result)
{
	result.clear();

	std::string::size_type pos = 0;

	while (pos != std::string::npos)
	{
		std::string::size_type p = s.find(delim, pos);

		if (p == std::string::npos)
		{
			result.push_back(s.substr(pos));
			break;
		}
		else
		{
			result.push_back(s.substr(pos, p - pos));
		}

		pos = p + delim.size();
	}
}

void listcat(float* v, const float* a, const float* b, const int size_a, const int size_b)
{
	for (int i = 0; i < size_a; ++i)
	{
		v[i] = a[i];
	}

	for (int i = 0; i < size_b; ++i)
	{
		v[size_a + i] = b[i];
	}
}

int main()
{
	std::string imageListtxt = "/home/yugo/Desktop/dataset/cfd-cropped/images_selected.txt";
	std::ofstream ofs("cfd-histogram-gabor.tsv", std::ios::trunc);
	const int hist_dim = 64;
	const int gabor_dim = 72;
	const int dim = hist_dim + gabor_dim;

	std::ifstream ifs(imageListtxt.c_str());
	if (ifs.fail())
		std::cerr << "Can not open the specific file: " << imageListtxt << std::endl;

	std::string line;
	std::vector<std::string> splited;
	Histogram hist;
	Gabor gabor;
	float result_hist[hist_dim];
	float result_gabor[gabor_dim];
	float result[dim];

	while (getline(ifs, line))
	{
		splitline(line, " ", splited);
		std::cout << splited[0] << std::endl;

		hist.calcHistogram(splited[0], result_hist);
		gabor.calcGaborFeature(splited[0], result_gabor);
		listcat(result, result_hist, result_gabor, hist_dim, gabor_dim);

		for (int i = 0; i < dim; ++i)
		{
			ofs << result[i];
			if (i < dim - 1)
				ofs << "\t";
			else
				ofs << std::endl;
		}
	}

	return 0;
}
