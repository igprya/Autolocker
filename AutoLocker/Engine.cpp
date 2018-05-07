#include "stdafx.h"
#include "Engine.h"

using namespace cv;

Engine::Engine(bool useDebug)
{
	capturer = new Processing::Capturer();
	detector = new Processing::Detector();
	recognizer = new Processing::Recognizer();

	this->debugMode = useDebug;
}

Engine::~Engine()
{
	delete securityProvider;
	delete capturer;
	delete detector;
	delete recognizer;
}

int Engine::Start()
{
	int engInitResult = InitEngine();
	int engineActionResult = ECODE_SUCCESS;

	if (engInitResult < ECODE_SUCCESS) {		
		return engInitResult;
	}

	while (nextEngineAction != nullptr)	
	{
		engineActionResult = (this->*nextEngineAction)();

		if (engineActionResult < ECODE_SUCCESS)	{
			securityProvider->ForceLockdown();
			return engineActionResult;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(ENGINE_RPM));
	}

	return ECODE_SUCCESS;
}

int Engine::InitEngine()
{
	securityProvider = new Security::SecurityProvider(ENGINE_DETECTION_FAILURE_THRESHOLD
		, ENGINE_RECOGNITION_FAILURE_THRESHOLD
		, ENGINE_RECOGNITION_INTERVAL
		, this
		, new Security::WinLocker()
		, new Helpers::ConLogger()
	);

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

int Engine::DetectFace()
{
	int faceCount = 0;
	bool detectionSuccess = false;
	Mat frame = capturer->GetFrame();

	if (frame.empty()) {
		return ERROR_CAPTURER_NOFRAME;
	}

	std::vector<Rect> faces = detector->GetFaceRects(frame);

	if (faces.size() > 0) {
		detectionSuccess = true;
		faceCount = faces.size();

		if (debugMode) {
			ShowUI(frame, faces);
		}
	}

	if (detectionSuccess) {
		securityProvider->HandleDetectionSuccess();

		if (faceCount > 1) {
			securityProvider->HandleMultilpleFaces(faceCount);
		}
	}
	else {
		securityProvider->HandleDetectionFailure();
	}

	return ECODE_SUCCESS;
}

int Engine::RecognizeFace()
{
	bool authorizationSuccess = false;
	Mat frame = capturer->GetFrame();

	if (frame.empty()) {
		return ERROR_CAPTURER_NOFRAME;
	}

	std::vector<Mat> faceMats = detector->GetFaces(frame);

	int label = 0;
	double confidence = 0.0;

	if (faceMats.size() > 0) {
		for (int i = 0; i < faceMats.size(); i++) 
		{
			recognizer->RecognizeFace(faceMats[i], label, confidence);
			authorizationSuccess = securityProvider->TryAuthorize(label, confidence);
		}
	}

	if (authorizationSuccess) {
		securityProvider->HandleAuthorizationSuccess();
	}
	else {
		securityProvider->HandleAuthorizaitonFailure();
	}

	return ECODE_SUCCESS;
}

void Engine::SecurityStateChanged(Security::SecurityAction action)
{
	switch (action)
	{
		case Security::SecurityAction::RECOGNIZE: SetNextAction(&Engine::RecognizeFace); break;
		case Security::SecurityAction::DETECT: SetNextAction(&Engine::DetectFace); break;
		default: SetNextAction(&Engine::DetectFace); break;
	}
}

void Engine::SetNextAction(engine_fptr nextAction)
{
	this->nextEngineAction = nextAction;
}

void Engine::DrawFaceFrames(Mat& frame, std::vector<Rect>& detectedFaces)
{
	size_t currentIndex = 0;
	int currentArea = 0;

	for (currentIndex = 0; currentIndex < detectedFaces.size(); currentIndex++)	{
		Point pt1 = Point(detectedFaces[currentIndex].x, detectedFaces[currentIndex].y);
		Point pt2 = Point(detectedFaces[currentIndex].x + detectedFaces[currentIndex].height, detectedFaces[currentIndex].y + detectedFaces[currentIndex].width);

		rectangle(frame, pt1, pt2, 8);
	}
}

void Engine::ShowUI(Mat& frame, std::vector<Rect>& faces)
{
	std::stringstream ss;
	int recognitionLabel = 0;
	double recognitionConfidence = 0;

	recognizer->GetLastRecognitionResults(recognitionLabel, recognitionConfidence);
	ss << recognitionLabel << " (" << recognitionConfidence << ")";

	DrawFaceFrames(frame, faces);
	putText(frame, ss.str(), Point(10, 50), HersheyFonts::FONT_HERSHEY_PLAIN, 2, Scalar::all(255), 2);
	imshow("dbg-enginecycle", frame);
	waitKey(5);
}
