
#include "project.h"
#include "GaugeInterface.h"
#include <string>

static float m_DataDelta = 2;
static BOOL m_isDataGoingUp = TRUE;
static uint32_t m_element;
static GaugePtr m_gauge;

static void _Key1Release(void* userData);

void Screen_GaugeDemo_Enter(void)
{
	uint8_t engineIndex = 0;
	DBVAR_T data;
	data.flt = 500;
	Database_Set_Demo(db_GaugeDemo, &data, DBVARTYPE_FLOAT, engineIndex);

	m_gauge = IGauge::create();
	m_gauge->scaleFactor(1.58f);
	m_gauge->minValue(0.0f);
	m_gauge->maxValue(1000.0f);
	m_gauge->startAngle(270.0f);
	m_gauge->endAngle(90.0f);
	m_gauge->centre(IPosition::create(160, 185));
	m_gauge->largeMarkerNumberOf(11);
	m_gauge->smallMarkerNumberOf(1);
	m_gauge->largeMarkerColour(MAKERGB565(128, 255, 128));
	m_gauge->smallMarkerColour(MAKERGB565(128, 255, 128));
	m_gauge->needleColour(MAKERGB565(128, 255, 128));
	m_gauge->isClockwise(true);

	ShapePartPtr needlePart = IShapePart::create("needlePart");
	needlePart->addPosition(-5.0f, -5.0f);
	needlePart->addPosition(0.0f, 0.0f);
	needlePart->addPosition(5.0f, -5.0f);
	needlePart->addPosition(2.0f, -96.0f);
	needlePart->addPosition(0.0f, -100.0f);
	needlePart->addPosition(-2.0f, -96.0f);
	ShapePtr needle = IShape::create("needle");
	needle->addPart(needlePart);

	ShapePartPtr bigMarkerPart = IShapePart::create("bigMarkerPart");
	bigMarkerPart->addPosition(-1.0f, -88.0f);
	bigMarkerPart->addPosition(1.0f, -88.0f);
	bigMarkerPart->addPosition(1.0f, -100.0f);
	bigMarkerPart->addPosition(-1.0f, -100.0f);
	bigMarkerPart->setupColour(1.0f, 1.0f, 1.0f);
	ShapePtr bigMarker = IShape::create("bigMarker");
	bigMarker->addPart(bigMarkerPart);

	ShapePartPtr smallMarkerPart = IShapePart::create("smallMarkerPart");
	smallMarkerPart->addPosition(-0.5f, -92.0f);
	smallMarkerPart->addPosition(0.5f, -92.0f);
	smallMarkerPart->addPosition(0.5f, -100.0f);
	smallMarkerPart->addPosition(-0.5f, -100.0f);
	ShapePtr smallMarker = IShape::create("smallMarker");
	smallMarker->addPart(smallMarkerPart);

	m_gauge->needle(needle);
	m_gauge->largeMarker(bigMarker);
	m_gauge->smallMarker(smallMarker);

	m_gauge->arcOffset(96);
	m_gauge->arcWidth(8);
	m_gauge->arcOpacity(75);
	m_gauge->arcColour(CYAN);

	for (uint32_t i = 0; i <= 10; i++)
	{
		m_gauge->descriptors(std::to_string(i * 100));
	}
	m_gauge->descriptorPosition(IPosition::create(0.0f, -75.0f));
	m_gauge->fontSize(10);
	m_gauge->fontColour(YELLOW);
}

void Screen_GaugeDemo_Create(void)
{
	// Setup default keys
	ScreensSetupDefaultKeys();

	ButtonBarSetKeyImages(KEYINDEX_1, &right, &right_s);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, _Key1Release, nullptr);
	ButtonBarSetKeyImages(KEYINDEX_2, &blank, &blank);
	ButtonBarSetKeyImages(KEYINDEX_3, &blank, &blank);
	ButtonBarSetKeyImages(KEYINDEX_4, &blank, &blank);

	// Clear layers
	vLcdBlankerEx(BLACK, ALPHA_COLOR);

	m_gauge->drawBackground();
}

void Screen_GaugeDemo_Update(void)
{
	DBELEMENT_T gaugeDemoDBElement;
	int InstanceIndex = 0;
	
	// Get the Data from the Database, Clear the previous Needle then updated the data and draw the needle
	DataBase_Get(&gaugeDemoDBElement, db_GaugeDemo, InstanceIndex);
	
	m_gauge->clearForeground();
	m_gauge->data(DataBase_GetDataFromElementAsFloat(&gaugeDemoDBElement));
	m_gauge->drawForeground();

	// Move the data on to the next location, then, put it back in the database.
	if (m_isDataGoingUp)
	{
		gaugeDemoDBElement.Data.flt += m_DataDelta;
		if (gaugeDemoDBElement.Data.flt >= 1000)
		{
			gaugeDemoDBElement.Data.flt = 1000;
			m_isDataGoingUp = FALSE;
		}
	}
	else
	{
		gaugeDemoDBElement.Data.flt -= m_DataDelta;
		if (gaugeDemoDBElement.Data.flt <= 0)
		{
			gaugeDemoDBElement.Data.flt = 0;
			m_isDataGoingUp = TRUE;
		}
	}
	Database_Set_Demo(db_GaugeDemo, &gaugeDemoDBElement.Data, DBVARTYPE_FLOAT, InstanceIndex);
}

void Screen_GaugeDemo_Exit(void)
{
	// get rid of the gauge since it uses memory and we don't need it when we are not on this screen
	m_gauge = nullptr;
}

static void _Key1Release(void* userData)
{
	MMIScreenGoto(SCREENID_1);
}

