
#include "project.h"

#include <vector>

static void _Key1Release(void* userData);

std::vector<std::string> faults;


void Screen2Enter(void)
{

}

void Screen2Create(void)
{
	// Setup default keys
	ScreensSetupDefaultKeys();

	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, _Key1Release, nullptr);

	ButtonBarSetMode(BUTTONBARMODE_INVISIBLE);

	// Clear layers
	vLcdBlankerEx(WHITE, ALPHA_COLOR);

	// Text output (to layer 1)
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 25, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 0);
	SimpleTextDraw(lcd_get_width() / 2, 5, "Control System Faults", BLACK, 100, LAYER_BACK);

	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 15, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 0);
	SimpleTextDraw(180, 35, "Active Faults = ", BLACK, 100, LAYER_BACK);
}

void Screen2Update(void)
{

    fill_lcd_screen(WHITE, LAYER_FRONT);
    faults.clear();

    processSerial();

    int faultCount = 0;
    for (int i = 0; i < 6; i++) { // 6 faults per station
        // STA1 Faults
        if (sta1 & (1 << i)) {
            faults.push_back(m_FAULTS_DB[i].Description); //   0-5
            faultCount++;
        }
        // STA2 Faults
        if (sta2 & (1 << i)) {
            faults.push_back(m_FAULTS_DB[i + 6].Description); //   6-11
            faultCount++;
        }
        // STA3 Faults
        if (sta3 & (1 << i)) {
            faults.push_back(m_FAULTS_DB[i + 12].Description); //   12-17
            faultCount++;
        }
        // NFE Faults
        if (nfe & (1 << i)) {
            faults.push_back(m_FAULTS_DB[i + 18].Description); //   18-23
            faultCount++;
        }
        // SFE Faults
        if (sfe & (1 << i)) {
            faults.push_back(m_FAULTS_DB[i + 24].Description); //   24-29
            faultCount++;
        }


    }

    // CFE Fault
    if (cfe & (1 << 0)) {
        faults.push_back(m_FAULTS_DB[30].Description); // Index 30
        faultCount++;
    }
    // GAL Fault
    //if (gal & (1 << 0)) {
    //    faults.push_back(m_FAULTS_DB[31].Description); //   31
    //    faultCount++;
    //}

    SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 15, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 0);
    SimpleTextDraw(282, 35, std::to_string(faultCount).c_str(), BLACK, 100, LAYER_FRONT);

    SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 20, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 0);
    for (int i = 0; i < faults.size(); i++)
    {
        if (i > 7) break;
        SimpleTextDraw(5, 50 + (i * 20), faults[i].c_str(), BLACK, 100, LAYER_FRONT);
    }
}
 

 
void Screen2Exit(void)
{
}

static void _Key1Release(void* userData)
{
	MMIScreenGoto(SCREENID_1);
}

