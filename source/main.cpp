//------------------------------------------------------------------------------
//  main.c
//------------------------------------------------------------------------------

#include "project.h"

//------------------------------------------------------------------------------
// Prototypes
//------------------------------------------------------------------------------
static void CANvuMainExit(void);

//------------------------------------------------------------------------------
// GLOBAL FUNCTIONS
//------------------------------------------------------------------------------

// Starts the SDK when running on the target
int main(int argc, char* argv[])
{
	int exitCode;
	exitCode = InitialiseSDKCFramework(argc, argv);
	return exitCode;
}

// Main program initialisation - This is called first and only once
extern "C" void CANvuMainInit(void)
{
	// Register the Exit function called on App shutdown
	PlatformShutdownRegisterCallback(CANvuMainExit);

	// Initialise the SDK
	sdk_init();

	// Load the fonts
	FontInit();

	// Init the settings
	SettingsInit();

	// CAN port initialisation
	CANInit();

	// Initialise the UART port
	UARTInit();

	// Initialise the MMI
	MMIInit(100, 100, 30);

	// Initialise the event handler
	EventHandlerInit();

	// Make sure the Backlight levels are sorted out.
	LightingInit();

	// Init the Database
	DataBase_Init();

	// Set up user configuration of I/O Board
	if (IOUserInit() == FALSE )
	{
		printf("I/O Board Failed!!\n");
	}
}

// Start of the main program. Only called once after CANvuMainInit()
extern "C" void CANvuMain(void)
{
	// Info message
	printf("%s %s - %s\n", __DATE__, __TIME__, SOFTWARE);

	// refresh the watchdog
	watchdog_refresh();

	// Initialise our keypad handler
	ButtonBarInit();

	// Init the Menu system
	MenuInit();

	// Setup the screens
	ScreensInit();
}

// The main program loop. Called repetitively!
extern "C" void CANvuMainLoop(void)
{
	// Kick the watchdog to avoid a reset
	watchdog_refresh();

	// UART decode
	UARTDecode();

	// Update the screen buffers if we need to (this calls the Create/Update functions for the active screen)
	MMIProcess(); 

	// Update the LCD from our screen buffers
	merge_layers();

	// Process any events
	EventHandlerProcess();
}

static void CANvuMainExit(void)
{
	//  De-initialise the CAN ports
	CANDeInit();
}
