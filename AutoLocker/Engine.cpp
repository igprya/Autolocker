#include "stdafx.h"
#include "Engine.h"

using namespace cv;

Engine::Engine()
{
	capturer = new Processing::Capturer();
	detector = new Processing::Detector();
	recognizer = new Processing::Recognizer();
}

Engine::~Engine()
{
	delete keykeeper;
	delete capturer;
	delete detector;
	delete recognizer;
}

int Engine::Start()
{
	int engInitResult = InitEngine();

	if (engInitResult < ECODE_SUCCESS) {
		keykeeper->Lock();
		return engInitResult;
	}

	while (true)
	{
		int cycleResult = EngineCycle();		

		if (cycleResult < ECODE_SUCCESS) 
			return cycleResult;

		std::this_thread::sleep_for(std::chrono::milliseconds(ENGINE_RPM));
	}

	return ECODE_SUCCESS;
}

int Engine::InitEngine()
{
	keykeeper = new Helpers::WinLocker();

	bool capturerRunning = false;
	bool detectorRunning = false;
	bool recognizerRunning = false;

	capturerRunning = this->capturer->InitCapture(DEFAULT_CAPTURE_DEVICE_INDEX);
	detectorRunning = this->detector->InitDetection(CASCADE_TEMPLATE_FILE_PATH);
	recognizerRunning = this->recognizer->InitRecognition(OWNER_FACES_FOLDER);

	if (!capturerRunning) {
		return ERROR_CAPTURER_FAILED_INIT;
	}

	if (!detectorRunning) {
		return ERROR_DETECTOR_FAILED_INIT;
	}

	if (!recognizerRunning) {
		return ERROR_RECOGNIZER_FAILED_INIT;
	}

	return ECODE_SUCCESS;
}

int Engine::EngineCycle()
{
	int handleResult = ECODE_SUCCESS;

	currentFrame = capturer->GetFrame();

	if (currentFrame.empty()) {
		return ERROR_CAPTURER_NOFRAME;
	}

	std::vector<Rect> faces = detector->GetFaceRects(currentFrame);

	if (faces.size() == 0) 
	{
		handleResult = HandleDetectionFailure();
		if (handleResult < ECODE_SUCCESS) {
			return handleResult;
		}
	}
	else {
		failedDetectionCount = 0;
	}

	DrawFaceFrames(currentFrame, faces);
	imshow("dbg-enginecycle", currentFrame);
	waitKey(5);

	//if (IsRecognitionRequired(recognizer->GetOperationTime())) {
	if (true) {
		std::vector<Mat> faceMats = detector->GetFaces(currentFrame);

		if (faceMats.size() > 0)
		{
			int label = 0;
			double confidence = 0.0;

			Mat face = faceMats[0];
			Mat faceGray;

			resize(face, face, Size(150, 150), 0, 0, INTER_LINEAR);

			recognizer->RecognizeFace(face, label, confidence);
			std::cout << "Recognized " << label << " with confidence " << confidence << std::endl;
		}
	}

	return ECODE_SUCCESS;
}

bool Engine::IsRecognitionRequired(time_t lastRecognition)
{
	double timeDifference = std::difftime(lastRecognition, std::time(nullptr));

	if (timeDifference > recognitionInterval) {
		return true;
	}
	else {
		return false;
	}
}

int Engine::HandleDetectionFailure()
{
	failedDetectionCount++;

	if (failedDetectionCount > failedDetectionsThreshold) {
		failedDetectionCount = 0;

		return keykeeper->Lock();
	}

	return ECODE_SUCCESS;
}

int Engine::HandleRecognitionFailure()
{
	failedRecognitionCount++;

	if (failedRecognitionCount > failedRecognitionsThreshold) {
		failedRecognitionCount = 0;

		return keykeeper->Lock();
	}

	return ECODE_SUCCESS;
}

int Engine::DrawFaceFrames(Mat& frame, std::vector<Rect>& detectedFaces)
{
	size_t currentIndex = 0;
	int currentArea = 0;

	for (currentIndex = 0; currentIndex < detectedFaces.size(); currentIndex++) 
	{
		Point pt1 = Point(detectedFaces[currentIndex].x, detectedFaces[currentIndex].y);
		Point pt2 = Point(detectedFaces[currentIndex].x + detectedFaces[currentIndex].height, detectedFaces[currentIndex].y + detectedFaces[currentIndex].width);

		rectangle(frame, pt1, pt2, 8);
	}

	return ECODE_SUCCESS;
}
