#include <iostream>
#include <string>

#include "project.h"

// Globals
extern int passcodeScreen = 0;

//UART Related
extern char pvci_arr[MAX_RAWINPUTSTRING_LENGTH][MAX_RAWINPUTSTRING_DIGITS];

//Button Bar
static void _Key1Release(void* userData);
static void _Key2Release(void* userData);
static void _Key3Release(void* userData);
static void _Key4Release(void* userData);
static void _Key5Release(void* userData);

static uint32_t m_eidAlarm_ack = ELEMENTID_INVALID;
static uint32_t m_eidFault_screen = ELEMENTID_INVALID;
static uint32_t m_eidCalib_screen = ELEMENTID_INVALID;
static uint32_t m_eidSystem_screen = ELEMENTID_INVALID;
static uint32_t m_eidExit = ELEMENTID_INVALID;

bool buttonBarVisible = false;

//Config
int temp_config;

static void setButtonBarImagesScreens() {
	ButtonBarSetKeyImages(KEYINDEX_1, &alarm_ack, &alarm_ack);
	ButtonBarSetKeyImages(KEYINDEX_2, &fault_screen, &fault_screen);
	ButtonBarSetKeyImages(KEYINDEX_3, &calib_screen, &calib_screen);
	ButtonBarSetKeyImages(KEYINDEX_4, &system_screen, &system_screen);
	ButtonBarSetKeyImages(KEYINDEX_5, &exit_menu, &exit_menu);
}

// element ids
static uint32_t m_eidBar = ELEMENTID_INVALID;

//Alarm graphic drawing booleans
bool genericAlarm;
bool serialAlarm;

bool genericAlarmFlash = false;
bool serialAlarmFlash = false;

//Alarm graphic timer


void flashAlarms() {
	if (genericAlarm) {
		
	}
	if (serialAlarm) {

	}
}

//Graphs
DoubleSidedVerticalBarGraph bucket1;
DoubleSidedVerticalBarGraph bucket2;

DoubleSidedHorizontalBarGraph nozzle1;
DoubleSidedHorizontalBarGraph nozzle2;

SingleSidedVerticalBarGraph interceptor1;
SingleSidedVerticalBarGraph interceptor2;

void graphSetup(int indication_config) 
{
	switch (indication_config) {
	case 1:
		bucket1 = DoubleSidedVerticalBarGraph("BUCKET", "", 148, 90, false);
		nozzle1 = DoubleSidedHorizontalBarGraph("NOZZLE", "", 160, 215, 0);
		break;

	case 2:
		bucket1 = DoubleSidedVerticalBarGraph("Port", "BKT", 80, 90, true);
		bucket2 = DoubleSidedVerticalBarGraph("Stbd", "BKT", 228, 90, false);

		nozzle1 = DoubleSidedHorizontalBarGraph("Port", "NOZ", 80, 215, 0);
		nozzle2 = DoubleSidedHorizontalBarGraph("Stbd", "NOZ", 240, 215, 0);
		break;

	case 3:
		bucket1 = DoubleSidedVerticalBarGraph("Port", "BKT", 80, 90, true);
		bucket2 = DoubleSidedVerticalBarGraph("Stbd", "BKT", 228, 90, false);

		nozzle1 = DoubleSidedHorizontalBarGraph("NOZZLE", "", 160, 215, 0);
		break;

	case 4:
		bucket1 = DoubleSidedVerticalBarGraph("Port", "BKT", 140, 90, true);
		bucket2 = DoubleSidedVerticalBarGraph("Stbd", "BKT", 174, 90, false);

		nozzle1 = DoubleSidedHorizontalBarGraph("Port", "NOZ", 80, 215, 0);
		nozzle2 = DoubleSidedHorizontalBarGraph("Stbd", "NOZ", 240, 215, 0);

		interceptor1 = SingleSidedVerticalBarGraph("Port", "INT", 15, 20, true);
		interceptor2 = SingleSidedVerticalBarGraph("Stbd", "INT", 300, 20, false);
		break;

	case 5:
		bucket1 = DoubleSidedVerticalBarGraph("Port", "BKT", 140, 90, true);
		bucket2 = DoubleSidedVerticalBarGraph("Stbd", "BKT", 174, 90, false);

		nozzle1 = DoubleSidedHorizontalBarGraph("NOZZLE", "", 160, 215, 0);

		interceptor1 = SingleSidedVerticalBarGraph("Port", "INT", 15, 20, true);
		interceptor2 = SingleSidedVerticalBarGraph("Stbd", "INT", 300, 20, false);
		break;
	}
}

void graphDraw(int indication_config) {
	switch (indication_config) {
	case 1:
		bucket1.draw(portBucketPosition);

		nozzle1.draw(portNozzlePosition);
		break;

	case 2:
		bucket1.draw(portBucketPosition);
		bucket2.draw(stbdBucketPosition);

		nozzle1.draw(portNozzlePosition);
		nozzle2.draw(stbdNozzlePosition);
		break;

	case 3:
		bucket1.draw(portBucketPosition);
		bucket2.draw(stbdBucketPosition);

		nozzle1.draw(portNozzlePosition);
		break;

	case 4:
		bucket1.draw(portBucketPosition);
		bucket2.draw(stbdBucketPosition);

		nozzle1.draw(portNozzlePosition);
		nozzle2.draw(stbdNozzlePosition);

		interceptor1.draw(portInterceptorPosition);
		interceptor2.draw(stbdInterceptorPosition);
		break;

	case 5:
		bucket1.draw(portBucketPosition);
		bucket2.draw(stbdBucketPosition);

		nozzle1.draw(portNozzlePosition);

		interceptor1.draw(portInterceptorPosition);
		interceptor2.draw(stbdBucketPosition);
		break;
	}
}

void Screen1Enter(void)
{
	vLcdBlankerEx(WHITE, WHITE);

	temp_config = lcd_config;
	graphSetup(lcd_config);
}

void Screen1Create(void)
{
	vLcdBlankerEx(WHITE, ALPHA_COLOR);
	// Setup default keys
	ButtonBarSetAnimation(BUTTONBARANIMATION_SLIDE);
	ButtonBarSetHeight(48);

	ButtonBarSetMode(BUTTONBARMODE_INVISIBLE);

	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, _Key1Release, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_2, _Key2Release, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_3, _Key3Release, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_4, _Key4Release, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_5, _Key5Release, nullptr);

	setButtonBarImagesScreens();

	// Get new element Ids
	m_eidBar = ElementGetNewId();

	temp_config = lcd_config;
	graphSetup(lcd_config);
}

void Screen1Update(void)
{

	fill_lcd_screen(WHITE, LAYER_FRONT);
	
	processSerial();
	
	if (temp_config != lcd_config) {
		temp_config = lcd_config;
		graphSetup(temp_config);
	}

	graphDraw(lcd_config);

}

void Screen1Exit(void)
{

}

static void _Key1Release(void* userData)
{	
	if (!buttonBarVisible) {
		setButtonBarImagesScreens();
		ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
		buttonBarVisible = true;
	}
	else {

	}
	
}

static void _Key2Release(void* userData) 
{
	if (!buttonBarVisible) {
		setButtonBarImagesScreens();
		ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
		buttonBarVisible = true;
	}
	else {
		MMIScreenGoto(SCREENID_2);
		buttonBarVisible = false;

	}
}

static void _Key3Release(void* userData) 
{
	if (!buttonBarVisible) {
		setButtonBarImagesScreens();
		ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
		buttonBarVisible = true;
	}
	else {
		passcodeScreen = 0;
		MMIScreenGoto(SCREENID_PASSCODE);
		buttonBarVisible = false;
	}
}

static void _Key4Release(void* userData) 
{
	if (!buttonBarVisible) {
		setButtonBarImagesScreens();
		ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
		buttonBarVisible = true;
	}
	else {
		passcodeScreen = 1;
		MMIScreenGoto(SCREENID_PASSCODE);
		buttonBarVisible = false;
	}
}

static void _Key5Release(void* userData) 
{
	if (!buttonBarVisible) {
		/*
		LightingEnter();
		ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
		*/
		MMIScreenGoto(SCREENID_DEBUG);
	}
	else {
		ButtonBarSetMode(BUTTONBARMODE_INVISIBLE);
		buttonBarVisible = false;
	}
}