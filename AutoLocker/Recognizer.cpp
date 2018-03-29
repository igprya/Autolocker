#include "stdafx.h"
#include "Recognizer.h"

namespace Processing
{
	Recognizer::Recognizer()
	{
		model = FisherFaceRecognizer::create();
	}
	
	Recognizer::~Recognizer()
	{
	}

	int Recognizer::InitRecognition(std::string facesDirectoryPath)
	{	
		for (auto& p : std::experimental::filesystem::directory_iterator(facesDirectoryPath))
		{
			std::string path = p.path().string();
			std::string fileName = p.path().filename().string();
			images.push_back(imread(path));

			if (fileName[0] == 'a') {
				labels.push_back(0);
			}
			else {
				labels.push_back(1);
			}
		}

		if (labels.size() > 0 && images.size() > 0)
		{
			model->train(images, labels);
			return ECODE_SUCCESS;
		}
		else
		{
			return ECODE_FAILURE;
		}		
	}

	void Recognizer::RecognizeFace(Mat& face, int& label, double& confidence)
	{
		model->predict(face, label, confidence);	
	}
}