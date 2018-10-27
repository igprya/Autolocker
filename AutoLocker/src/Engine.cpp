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
	engineLogger->Log("Deinitializing engine...");

	cv::destroyAllWindows();
	delete capturer;
	delete detector;
	delete recognizer;
}

int Engine::Start()
{
	engineLogger->Log("Starting eninge...");

	int engineActionResult = ECODE_SUCCESS;

	while (nextEngineAction != nullptr)	
	{
		engineActionResult = (this->*nextEngineAction)();

		if (engineActionResult < ECODE_SUCCESS)	{
			securityProvider->ForceLockdown();
			return engineActionResult;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(settings.EngineRpm()));
	}

	engineLogger->Log("Stopping engine...");

	return ECODE_SUCCESS;
}

int Engine::InitEngine(Helpers::Settings settings
	, Security::IBaseLocker* locker
	, Helpers::ILogger* logger)
{
	int initResult = ECODE_SUCCESS;
	
	this->settings = settings;
	this->engineLogger = logger;
	
	engineLogger->Log("Initializing engine...");
	
	securityProvider = new Security::SecurityProvider(settings
		, this
		, locker
		, engineLogger
	);

	bool capturerRunning = false;
	bool detectorRunning = false;
	bool recognizerRunning = false;

	capturerRunning = this->capturer->InitCapture(settings.DefaultCaptureDeviceIndex());
	detectorRunning = this->detector->InitDetection(settings.CascadeTemplateFilePath());
	recognizerRunning = this->recognizer->InitRecognition(settings.AuthorizedFacesPath());	

	if (!capturerRunning) {
		initResult = ERROR_CAPTURER_FAILED_INIT;
	}

	if (!detectorRunning) {
		initResult = ERROR_DETECTOR_FAILED_INIT;
	}

	if (!recognizerRunning) {
		initResult = ERROR_RECOGNIZER_FAILED_INIT;
	}

	if (initResult != ECODE_SUCCESS) {
		engineLogger->Error("Engine initialization failure!");
	} else {
		engineLogger->Log("Initialization complete");
	}

	return initResult;
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

	if (!faces.empty()) {
		detectionSuccess = true;
		faceCount = faces.size();

		if (settings.FeedWindow()) {
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
	bool recognitionSuccess = false;
	Mat frame = capturer->GetFrame();

	if (frame.empty()) {
		return ERROR_CAPTURER_NOFRAME;
	}

	std::vector<Mat> faceMats = detector->GetFaces(frame);

	int label = 0;
	double confidence = 0.0;

	if (!faceMats.empty()) {
		for (int i = 0; i < faceMats.size(); i++) 
		{
			recognizer->RecognizeFace(faceMats[i], label, confidence);
			recognitionSuccess = securityProvider->TryRecognize(label, confidence);
		}

		if (settings.FeedWindow()) {
			std::vector<Rect> empty;
			ShowUI(frame, empty);
		}
	}

	if (recognitionSuccess) {
		securityProvider->HandleRecognitionSuccess();
	}
	else {
		securityProvider->HandleRecognitionFailure();
	}

	return ECODE_SUCCESS;
}

void Engine::OnSecurityStateChange(Security::SecurityAction requiredAction)
{
	switch (requiredAction)
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
	cv::putText(frame, ss.str(), Point(10, 50), HersheyFonts::FONT_HERSHEY_PLAIN, 2, Scalar::all(255), 2);
	cv::imshow("Camera feed", frame);
	waitKey(1);
}
