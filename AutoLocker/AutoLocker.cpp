// AutoLocker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "Engine.h"
#include "ExitCodes.h"

using namespace std;

int main(int argc, char** argv)
{
	stringstream exitMessage;
	Engine* engine = new Engine();
	int engineResult = engine->Run();

	switch (engineResult)
	{
		case ECODE_SUCCESS: exitMessage << "ECODE_SUCCESS"; break;
		case ECODE_FAILURE: exitMessage << "ECODE_FAILURE"; break;
		case ERROR_ENGINE_FAILED_INIT: exitMessage << "ERROR_ENGINE_FAILED_INIT"; break;
		case ERROR_CAPTURER_FAILED_INIT: exitMessage << "ERROR_CAPTURER_FAILED_INIT"; break;
		case ERROR_DETECTOR_FAILED_INIT: exitMessage << "ERROR_DETECTOR_FAILED_INIT"; break;
		case ERROR_RECOGNIZER_FAILED_INIT: exitMessage << "ERROR_RECOGNIZER_FAILED_INIT"; break;
		default: exitMessage << "Exit code " << engineResult << " (unknown)"; break;
	}

	cout << exitMessage.str() << endl;

	getchar();

	return 0;
}

