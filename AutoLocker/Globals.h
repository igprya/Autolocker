#pragma once
#include "stdafx.h"
#include <string>

const static int ENGINE_RPM = 50;
const static int ENGINE_DETECTION_FAILURE_THRESHOLD = 10;
const static int ENGINE_RECOGNITION_INTERVAL = 180;
const static int ENGINE_RECOGNITION_FAILURE_THRESHOLD = 10;

const static std::string CASCADE_TEMPLATE_FILE_PATH = "patterns/haarcascade_frontalface_alt.xml";
const static std::string OWNER_FACES_FOLDER = "owner/";
const static int DEFAULT_CAPTURE_DEVICE_INDEX = 0;