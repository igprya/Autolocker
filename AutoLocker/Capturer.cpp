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
	Mat Capturer::GetFrame()
	{
		static Mat currentFrame;

		if (!capture.isOpened()) {
			return Mat();
		}
		
		capture.read(currentFrame);

		if (currentFrame.empty()) {
			return Mat();
		}

		SetOperationTime();

		return currentFrame;
	}
}
