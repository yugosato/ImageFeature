
#include <iostream>
#include <vector>
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

int main()
{
	std::string imageListtxt = "/home/yugo/Desktop/dataset/cfd-cropped/images_selected.txt";
	std::ofstream ofs("cfd-gabor.tsv", std::ios::trunc);
	int dim = 72; // histogram:64, gabor:72

	std::ifstream ifs(imageListtxt.c_str());
	if (ifs.fail())
		std::cerr << "Can not open the specific file: " << imageListtxt << std::endl;

	std::string line;
	std::vector<std::string> splited;
//	Histogram hist;
	Gabor gabor;
	float result[dim];

	while (getline(ifs, line))
	{
		splitline(line, " ", splited);
		std::cout << splited[0] << std::endl;
//		hist.calcHistogram(splited[0], result);
		gabor.calcGaborFeature(splited[0], result);
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
