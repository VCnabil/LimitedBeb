
#include "project.h"

static void _MenuLanguageCreate(void* userData);
static void _MenuLanguageChangeGB(void* userData);
static void _MenuLanguageChangeES(void* userData); 
static void _MenuLanguageChangeSE(void* userData); 
static void _MenuLanguageChangeFR(void* userData); 
static void _MenuLanguageChangeDE(void* userData); 
static void _MenuLanguageChangeIT(void* userData); 
static void _MenuLanguageChangeNL(void* userData); 
static void _MenuLanguageChangePT(void* userData); 

void MenuLanguageEnter(void* userData)
{
	// Creating our menu
	MenuCreateNew(_MenuLanguageCreate, nullptr);
}
// --------------------------------------------------------------------------->

static void _MenuLanguageCreate(void* userData)
{
	uint32_t currentItem = 0;

	MenuSetTitle(cucSystemWords[wLANGUAGE][SettingsGetLanguageIndex()]);

	MenuSetItem(currentItem++, cucSystemWords[wLANG][0], 0, _MenuLanguageChangeGB, nullptr);
	MenuSetItem(currentItem++, cucSystemWords[wLANG][1], 0, _MenuLanguageChangeES, nullptr);
	MenuSetItem(currentItem++, cucSystemWords[wLANG][2], 0, _MenuLanguageChangeSE, nullptr);
	MenuSetItem(currentItem++, cucSystemWords[wLANG][3], 0, _MenuLanguageChangeFR, nullptr);
	MenuSetItem(currentItem++, cucSystemWords[wLANG][4], 0, _MenuLanguageChangeDE, nullptr);
	MenuSetItem(currentItem++, cucSystemWords[wLANG][5], 0, _MenuLanguageChangeIT, nullptr);
	MenuSetItem(currentItem++, cucSystemWords[wLANG][6], 0, _MenuLanguageChangeNL, nullptr);
	MenuSetItem(currentItem++, cucSystemWords[wLANG][7], 0, _MenuLanguageChangePT, nullptr);
}
// --------------------------------------------------------------------------->

static void _MenuLanguageChangeGB(void* userData) 
{ 
	SettingsSetLanguageIndex(0); 
	MenuBack(); 
}
// --------------------------------------------------------------------------->

static void _MenuLanguageChangeES(void* userData) 
{ 
	SettingsSetLanguageIndex(1); 
	MenuBack(); 
}
// --------------------------------------------------------------------------->

static void _MenuLanguageChangeSE(void* userData) 
{ 
	SettingsSetLanguageIndex(2); 
	MenuBack(); 
}
// --------------------------------------------------------------------------->

static void _MenuLanguageChangeFR(void* userData) 
{ 
	SettingsSetLanguageIndex(3); 
	MenuBack(); 
}
// --------------------------------------------------------------------------->

static void _MenuLanguageChangeDE(void* userData) 
{ 
	SettingsSetLanguageIndex(4); 
	MenuBack(); 
}
// --------------------------------------------------------------------------->

static void _MenuLanguageChangeIT(void* userData) 
{ 
	SettingsSetLanguageIndex(5); 
	MenuBack(); 
}
// --------------------------------------------------------------------------->

static void _MenuLanguageChangeNL(void* userData) 
{ 
	SettingsSetLanguageIndex(6); 
	MenuBack(); 
}
// --------------------------------------------------------------------------->

static void _MenuLanguageChangePT(void* userData) 
{ 
	SettingsSetLanguageIndex(7); 
	MenuBack(); 
}
// --------------------------------------------------------------------------->