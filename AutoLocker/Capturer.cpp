#include "stdafx.h"
#include "Capturer.h"

namespace Processing
{
	Capturer::Capturer()
	{
	}
	
	Capturer::~Capturer()
	{
		capture.~VideoCapture();
	}

	int Capturer::InitCapture(int deviceIndex)
	{
		SetOperationTime();

		if (capture.isOpened()) {
			return ECODE_SUCCESS;
		}

		capture.open(deviceIndex);

		if (!capture.isOpened()) {
			return ECODE_FAILURE;
		}

		return ECODE_SUCCESS;
	}
	cv::Mat* Capturer::GetFrame()
	{
		static cv::Mat currentFrame;

		if (!capture.isOpened()) {
			return NULL;
		}
		
		capture.read(currentFrame);

		if (currentFrame.empty()) {
			return NULL;
		}

		SetOperationTime();

		return &currentFrame;
	}
}
