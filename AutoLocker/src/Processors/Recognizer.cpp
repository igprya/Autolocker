#include "stdafx.h"
#include "Recognizer.h"

namespace Processing
{
	Recognizer::Recognizer()
	{
		model = LBPHFaceRecognizer::create(2, 16);
	}


	int Recognizer::InitRecognition(std::string facesDirectoryPath)
	{	
		for (auto& p : std::experimental::filesystem::directory_iterator(facesDirectoryPath))
		{
			std::string path = p.path().string();
			std::string fileName = p.path().filename().string();
			images.push_back(imread(path, IMREAD_GRAYSCALE));


			if (fileName[0] == 'a') {
				labels.push_back(0);
			}
			else {
				labels.push_back(1);
			}
		}

		if (labels.size() > 0 && images.size() > 0) {
			TrainModel();			
			return ECODE_SUCCESS;
		}

		return ECODE_FAILURE;
	}


	int Recognizer::RecognizeFace(Mat& face, int& label, double& confidence)
	{
		try	{
			model->predict(face, label, confidence);
			SetLastRecognitionResults(label, confidence);

			return ECODE_SUCCESS;
		}
		catch (const std::exception& ex) {
			return ERROR_RECOGNIZER_FAILED_RECOGNITION;
		}
	}
	

	void Recognizer::GetLastRecognitionResults(int& lastLabel, double& lastConfidence)
	{
		lastLabel = this->lastRecognitionLablel;
		lastConfidence = this->lastRecognitionConfidence;
	}


	void Recognizer::SetLastRecognitionResults(int& label, double& confidence)
	{
		this->lastRecognitionLablel = label;
		this->lastRecognitionConfidence = confidence;
	}


	int Recognizer::TrainModel()
	{
		try	{
			model->train(images, labels);
			return ECODE_SUCCESS;
		}
		catch (std::exception& ex)	{
			return ECODE_FAILURE;
		}
	}


	int Recognizer::TrainModel(std::vector<Mat>& cImages, std::vector<int> cLabels)
	{
		try	{
			model->train(cImages, cLabels);
			return ECODE_SUCCESS;
		}
		catch (std::exception& ex)	{
			return ECODE_FAILURE;
		}
	}
}