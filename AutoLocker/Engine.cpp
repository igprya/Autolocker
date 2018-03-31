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

	if (IsRecognitionRequired(recognizer->GetOperationTime())) {
		std::vector<Mat> faceMats = detector->GetFaces(currentFrame);

		if (faceMats.size() > 0)
		{
			int recognitionLabel = 0;
			double recognitionConfidence = 0;

			Mat face = faceMats[0];

			recognizer->RecognizeFace(face, recognitionLabel, recognitionConfidence);
			std::cout << "Recognized " << recognitionLabel << " with confidence " << recognitionConfidence << std::endl;
		}
	}

	#ifdef _DEBUG

	std::stringstream ss;
	int recognitionLabel = 0;
	double recognitionConfidence = 0;

	recognizer->GetLastRecognitionResults(recognitionLabel, recognitionConfidence);
	ss << recognitionLabel << " (" << recognitionConfidence << ")";

	DrawFaceFrames(currentFrame, faces);
	putText(currentFrame, ss.str(), Point(10, 50), HersheyFonts::FONT_HERSHEY_PLAIN, 2, Scalar::all(255), 2);
	imshow("dbg-enginecycle", currentFrame);
	waitKey(5);

	#endif // _DEBUG


	return ECODE_SUCCESS;
}

bool Engine::IsRecognitionRequired(time_t lastRecognition)
{
	double timeDifference = std::difftime(std::time(nullptr), lastRecognition);

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
