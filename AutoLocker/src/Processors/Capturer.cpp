#include "stdafx.h"
#include "Capturer.h"

namespace Processing
{
	int Capturer::InitCapture(int deviceIndex)
	{
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
		Mat currentFrame;

		if (!capture.isOpened()) {
			return Mat();
		}
		
		capture.read(currentFrame);

		if (currentFrame.empty()) {
			return Mat();
		}

		return currentFrame;
	}
}
