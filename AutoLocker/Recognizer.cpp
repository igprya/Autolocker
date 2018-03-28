#include "stdafx.h"
#include "Recognizer.h"

namespace Processing
{
	Recognizer::Recognizer()
	{
	}
	
	Recognizer::~Recognizer()
	{
	}

	int Recognizer::InitRecognition()
	{
		labels.push_back(1);

		string path = "owner/";

		for (auto& p : std::experimental::filesystem::directory_iterator(path))
		{
			std::string path = p.path().string();
			images.push_back(imread(path));
		}

		return ECODE_SUCCESS;
	}

	int Recognizer::RecognizeFace(Mat& face)
	{
		return ECODE_FAILURE;
	}
}