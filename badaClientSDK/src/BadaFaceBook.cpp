/**
 * Name        : BadaFaceBook
 * Version     : 
 * Vendor      : 
 * Description : 
 */


#include "BadaFaceBook.h"
#include "FormManager.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::System;


BadaFaceBook::BadaFaceBook()
{
}

BadaFaceBook::~BadaFaceBook()
{
}

Application*
BadaFaceBook::CreateInstance(void)
{
	// Create the instance through the constructor.
	return new BadaFaceBook();
}

bool
BadaFaceBook::OnAppInitializing(AppRegistry& appRegistry)
{
	// TODO:
	// Initialize UI resources and application specific data.
	// The application's permanent data and context can be obtained from the appRegistry.
	//
	// If this method is successful, return true; otherwise, return false.
	// If this method returns false, the application will be terminated.

	// Uncomment the following statement to listen to the screen on/off events.
	//PowerManager::SetScreenEventListener(*this);

	FormManager *pFormManager = new FormManager();
	pFormManager->Initialize();
	GetAppFrame()->GetFrame()->AddControl(*pFormManager);

	pFormManager->SetStarterForm(FormManager::REQUEST_MAINFORM, null);


	return true;
}

bool
BadaFaceBook::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	// TODO:
	// Deallocate resources allocated by this application for termination.
	// The application's permanent data and context can be saved via appRegistry.
	return true;
}

void
BadaFaceBook::OnForeground(void)
{
	// TODO:
	// Start or resume drawing when the application is moved to the foreground.
}

void
BadaFaceBook::OnBackground(void)
{
	// TODO:
	// Stop drawing when the application is moved to the background.
}

void
BadaFaceBook::OnLowMemory(void)
{
	// TODO:
	// Free unused resources or close the application.
}

void
BadaFaceBook::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
	// TODO:
	// Handle any changes in battery level here.
	// Stop using multimedia features(camera, mp3 etc.) if the battery level is CRITICAL.
}

void
BadaFaceBook::OnScreenOn (void)
{
	// TODO:
	// Get the released resources or resume the operations that were paused or stopped in OnScreenOff().
}

void
BadaFaceBook::OnScreenOff (void)
{
	// TODO:
	//  Unless there is a strong reason to do otherwise, release resources (such as 3D, media, and sensors) to allow the device to enter the sleep mode to save the battery.
	// Invoking a lengthy asynchronous method within this listener method can be risky, because it is not guaranteed to invoke a callback before the device enters the sleep mode.
	// Similarly, do not perform lengthy operations in this listener method. Any operation must be a quick one.
}
