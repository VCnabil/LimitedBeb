//------------------------------------------------------------------------------
//  TITLE :          canviewer.c
//  DESCRIPTION :    CAN viewer.
//------------------------------------------------------------------------------

#include "project.h"

#define TITLE_HEIGHT					MenuTitleBarGetHeight()
#define FONT_SIZE						9
#define HEADER_DATA_Y					(TITLE_HEIGHT - 2 - FONT_SIZE)
#define ROW_HEIGHT						(FONT_SIZE + 2)
#define DATA_START_Y					(TITLE_HEIGHT + 3)

#define DATA_XDelta						18
#define DATA_1_X						(lcd_get_width() - (DATA_XDelta * 8))

#define TIME_X							75
#define DIRECTION_X						85
#define CANID_X							155

#define MAX_MESSAGES_PER_SCREEN			((lcd_get_height() - DATA_START_Y - 48) / ROW_HEIGHT)

#define MAX_MESSAGE_PAGES				20
#define MAX_LOGGED_MESSAGES_PER_PORT	300

#define STATS_POSITION_Y				35
#define STATS_FONT_SIZE					10
#define STATS_ROW_HEIGHT				(STATS_FONT_SIZE + 2)

#define STATS_TEXT_X					110
#define STATS_BLOCK_WIDTH				((lcd_get_width() - STATS_TEXT_X) / 2)
#define STATS_PORT_1X					(STATS_TEXT_X + (STATS_BLOCK_WIDTH / 2))
#define STATS_PORT_2X					(STATS_PORT_1X + STATS_BLOCK_WIDTH)

// Logger CAN port selection
typedef enum
{
	CANVIEWERLOGGER_PORT_1 = 0,
	CANVIEWERLOGGER_PORT_2,
	CANVIEWERLOGGER_PORT_BOTH,
	// Must be last!
	CANVIEWERLOGGER_PORT_MAX
} CANVIEWERLOGGER_PORT_T;

// Display mode
typedef enum
{
	DISPLAYMODE_VIEWER = 0,
	DISPLAYMODE_VIEWER_PAUSED,
	DISPLAYMODE_LOGGER,
	DISPLAYMODE_STATS
} DISPLAYMODE_T;

static void _CANViewerCreateScreen(void);
static void _CANViewerCreateExtraButtons(void);

static void _CANViewerTogglePause(void* userData);
static void _CANViewerExit(void* userData);
static void _CANViewerPortSelect(void* userData);
static void _CANViewerPageDown(void* userData);
static void _CANViewerPageUp(void* userData);
static void _CANViewerBackToViewer(void* userData);

static void _CANViewerLoggerEnter(void* userData);
static void _CANViewerLoggerToggleRecord(void* userData);
static void _CANViewerLoggerPortSelect(void* userData);

static void _CANViewerStatsEnter(void* userData);

static void _CANViewerLogWrite(CTBINLOG_FILERECORD_T* pRecord);
static void _CANViewerLogDelete(void* userData);

static void _CANViewerUpdateStatisticsValues(void* pUserData = nullptr);

// Current CAN port
static CAN_PORTS_T m_curCANPort = CAN_PORT1;

// Current display mode
static DISPLAYMODE_T m_curDisplayMode = DISPLAYMODE_VIEWER;

// Current page being viewed
static int32_t m_curPage = 0;

// Circular array of logged CAN messages per port
static CTBINLOG_FILERECORD_T m_frames[CAN_MAX_PORTS][MAX_LOGGED_MESSAGES_PER_PORT];
// Index of next write slot per port
static uint32_t m_frameWriteIndex[CAN_MAX_PORTS];

// Name of the log file
static const char* m_pLogFileName = "canlog.log";
// Name of the rotated log file (ready to be copied to USB stick)
static const char* m_pRotatedLogFileName = "canlog.ctb";

// Are we logging to file?
static BOOL m_isLoggingToFile = FALSE;
// CAN ports we are logging to file
static CANVIEWERLOGGER_PORT_T m_loggerCANPort = CANVIEWERLOGGER_PORT_BOTH;

// Element Id for data
static uint32_t m_eidData = ELEMENTID_INVALID;

// Mutex Handle
static MUTEXHANDLE_T m_canViewerMutex = 0;

// CAN Stats
static CAN_STATS_T m_port1Stats;
static CAN_STATS_T m_port2Stats;

void CANViewerInit(void)
{
	//Create our Mutex
	if (m_canViewerMutex == 0)
	{
		MutexCreate(&m_canViewerMutex);
	}
}

void CANViewerEnter(void)
{
	// Show the viewer page
	m_curDisplayMode = DISPLAYMODE_VIEWER;
}

void CANViewerCreate(void)
{
	// Get our element ids
	m_eidData = ElementGetNewId();

	// Clear layers
	vLcdBlankerEx(BLACK, ALPHA_COLOR);

	// Create the header
	_CANViewerCreateScreen();

	// Create any buttons we need
	_CANViewerCreateExtraButtons();
}

static void _UpdateNormal(void)
{
	// Viewer and Logger have the same main display
	char str[255];
	uint32_t dataPos;
	uint32_t screenPos;
	uint32_t msgIndex;

	msgIndex = m_frameWriteIndex[m_curCANPort];

	// Are we paused?
	if (m_curDisplayMode == DISPLAYMODE_VIEWER_PAUSED)
	{
		// Yes. Work out start index of the current page
		uint32_t pageOffset = m_curPage * MAX_MESSAGES_PER_SCREEN;
		if (pageOffset > msgIndex)
		{
			msgIndex = (MAX_LOGGED_MESSAGES_PER_PORT)-(pageOffset - msgIndex);
		}
		else
		{
			msgIndex -= pageOffset;
		}
	}

	// We can show a max of 14 messages
	for (screenPos = 0; screenPos < MAX_MESSAGES_PER_SCREEN; screenPos++)
	{
		CTBINLOG_FILERECORD_T* pRec;

		// Get next message to display
		if (msgIndex > 0)
		{
			--msgIndex;
		}
		else
		{
			msgIndex = MAX_LOGGED_MESSAGES_PER_PORT - 1;
		}
		pRec = &m_frames[m_curCANPort][msgIndex];

		
		SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, FONT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
		
		// Time
		sprintf(str, "%7llu.%03llu", pRec->timeOffsetMicrosecs / 1000000, (pRec->timeOffsetMicrosecs % 1000000) / 1000);
		SimpleTextDrawEle(m_eidData, TIME_X, DATA_START_Y + (ROW_HEIGHT * screenPos), str, WHITE_TEXT, 100, LAYER_FRONT);

		SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, FONT_SIZE, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 1);

		// Direction
		sprintf(str, "%c", (pRec->flags & CTLOGBIN_FLAGS_TRANSMIT) != 0 ? 'T' : 'R');
		SimpleTextDrawEle(m_eidData, DIRECTION_X, DATA_START_Y + (ROW_HEIGHT * screenPos), str, WHITE_TEXT, 100, LAYER_FRONT);

		// CAN ID
		SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, FONT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
		sprintf(str, "%X", pRec->id);
		SimpleTextDrawEle(m_eidData, CANID_X, DATA_START_Y + (ROW_HEIGHT * screenPos), str, WHITE_TEXT, 100, LAYER_FRONT);


		// Message Data
		SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, FONT_SIZE, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 1);
		for (dataPos = 0; dataPos < pRec->dataLen; dataPos++)
		{
			sprintf(str, "%02X", pRec->data[dataPos]);
			SimpleTextDrawEle(m_eidData, DATA_1_X + (dataPos * DATA_XDelta), DATA_START_Y + (ROW_HEIGHT * screenPos), str, WHITE_TEXT, 100, LAYER_FRONT);
		}
	}
}

static void _UpdateStatsViewer(void)
{
	char str[255];

	blockfillEx(0, 0, lcd_get_width() - 1, lcd_get_height() - 1, ALPHA_COLOR, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, STATS_FONT_SIZE, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 1);

	{
		sprintf(str, "%llu", m_port1Stats.rx_packets);
		SimpleTextDraw(STATS_PORT_1X, STATS_POSITION_Y + (1 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
		sprintf(str, "%llu", m_port2Stats.rx_packets);
		SimpleTextDraw(STATS_PORT_2X, STATS_POSITION_Y + (1 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
	}

	{
		sprintf(str, "%llu", m_port1Stats.rx_bytes);
		SimpleTextDraw(STATS_PORT_1X, STATS_POSITION_Y + (2 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
		sprintf(str, "%llu", m_port2Stats.rx_bytes);
		SimpleTextDraw(STATS_PORT_2X, STATS_POSITION_Y + (2 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
	}

	{
		sprintf(str, "%llu", m_port1Stats.tx_packets);
		SimpleTextDraw(STATS_PORT_1X, STATS_POSITION_Y + (3 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
		sprintf(str, "%llu", m_port2Stats.tx_packets);
		SimpleTextDraw(STATS_PORT_2X, STATS_POSITION_Y + (3 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
	}

	{
		sprintf(str, "%llu", m_port1Stats.tx_bytes);
		SimpleTextDraw(STATS_PORT_1X, STATS_POSITION_Y + (4 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
		sprintf(str, "%llu", m_port2Stats.tx_bytes);
		SimpleTextDraw(STATS_PORT_2X, STATS_POSITION_Y + (4 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
	}

	{
		sprintf(str, "%llu", m_port1Stats.rx_berr_count);
		SimpleTextDraw(STATS_PORT_1X, STATS_POSITION_Y + (6 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
		sprintf(str, "%llu", m_port2Stats.rx_berr_count);
		SimpleTextDraw(STATS_PORT_2X, STATS_POSITION_Y + (6 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
	}

	{
		sprintf(str, "%llu", m_port1Stats.tx_berr_count);
		SimpleTextDraw(STATS_PORT_1X, STATS_POSITION_Y + (7 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
		sprintf(str, "%llu", m_port2Stats.tx_berr_count);
		SimpleTextDraw(STATS_PORT_2X, STATS_POSITION_Y + (7 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
	}

	{
		sprintf(str, "%llu", m_port1Stats.bus_error_count);
		SimpleTextDraw(STATS_PORT_1X, STATS_POSITION_Y + (8 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
		sprintf(str, "%llu", m_port2Stats.bus_error_count);
		SimpleTextDraw(STATS_PORT_2X, STATS_POSITION_Y + (8 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
	}

	{
		sprintf(str, "%llu", m_port1Stats.error_warning_count);
		SimpleTextDraw(STATS_PORT_1X, STATS_POSITION_Y + (9 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
		sprintf(str, "%llu", m_port2Stats.error_warning_count);
		SimpleTextDraw(STATS_PORT_2X, STATS_POSITION_Y + (9 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
	}

	{
		sprintf(str, "%llu", m_port1Stats.error_passive_count);
		SimpleTextDraw(STATS_PORT_1X, STATS_POSITION_Y + (10 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
		sprintf(str, "%llu", m_port2Stats.error_passive_count);
		SimpleTextDraw(STATS_PORT_2X, STATS_POSITION_Y + (10 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
	}

	{
		sprintf(str, "%llu", m_port1Stats.bus_off_count);
		SimpleTextDraw(STATS_PORT_1X, STATS_POSITION_Y + (11 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
		sprintf(str, "%llu", m_port2Stats.bus_off_count);
		SimpleTextDraw(STATS_PORT_2X, STATS_POSITION_Y + (11 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
	}

	{
		sprintf(str, "%llu", m_port1Stats.bus_restart_count);
		SimpleTextDraw(STATS_PORT_1X, STATS_POSITION_Y + (12 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
		sprintf(str, "%llu", m_port2Stats.bus_restart_count);
		SimpleTextDraw(STATS_PORT_2X, STATS_POSITION_Y + (12 * STATS_ROW_HEIGHT), str, WHITE_TEXT, 100, LAYER_FRONT);
	}
}

void CANViewerUpdate(void)
{
	ElementFillPrevRect(m_eidData, ALPHA_COLOR, LAYER_FRONT);

	MutexLock(&m_canViewerMutex);
	if (m_curDisplayMode != DISPLAYMODE_STATS)
	{
		_UpdateNormal();
	}
	else
	{
		_UpdateStatsViewer();
	}
	MutexUnlock(&m_canViewerMutex);
}

static void _CreateNormal(void)
{
	char str[255];
	int32_t dataByteIndex;

	// We want the buttons only to be visible in the main CAN Viewer menu and not the CAN Statistics Menu.
	ButtonBarSetKeyImages(KEYINDEX_1, &blank, &blank_s);
	ButtonBarSetKeyImages(KEYINDEX_2, &blank, &blank_s);
	ButtonBarSetKeyImages(KEYINDEX_3, &blank, &blank_s);
	ButtonBarSetKeyImages(KEYINDEX_4, &blank, &blank_s);

	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 9, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	// Are we paused?
	if (m_curDisplayMode != DISPLAYMODE_VIEWER_PAUSED)
	{
		// No
		sprintf(str, "CAN PORT %d", (uint32_t)m_curCANPort + 1);
	}
	else
	{
		// Yes
		sprintf(str, "CAN Port %d - Paused - Page %d/%d", (uint32_t)m_curCANPort + 1, m_curPage + 1, MAX_MESSAGE_PAGES);
	}
	SimpleTextDraw(5, 5, str, WHITE_TEXT, 100, LAYER_FRONT);

	// Are we recording a log file?
	if (m_isLoggingToFile != FALSE)
	{
		// Yes
		SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, FONT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
		sprintf(str, "RECORDING...");
		SimpleTextDraw(lcd_get_width() - 61, 5, str, RED, 100, LAYER_FRONT);
	}

	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, FONT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(TIME_X, HEADER_DATA_Y, "TIME", WHITE_TEXT, 100, LAYER_FRONT);

	// Receive or Transmit?
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, FONT_SIZE, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 1);
	
	SimpleTextDraw(DIRECTION_X, HEADER_DATA_Y, "D", WHITE_TEXT, 100, LAYER_FRONT);

	// CAN ID
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, FONT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(CANID_X, HEADER_DATA_Y, "CAN ID", WHITE_TEXT, 100, LAYER_FRONT);

	// Message data
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, FONT_SIZE, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 1);
	for (dataByteIndex = 0; dataByteIndex < CAN_CONTENT_MAX_LENGTH; dataByteIndex++)
	{
		sprintf(str, "D%d", dataByteIndex + 1);
		SimpleTextDraw(DATA_1_X + (dataByteIndex * DATA_XDelta), HEADER_DATA_Y, str, WHITE_TEXT, 100, LAYER_FRONT);
	}
}

static void _CreateStats(void)
{
	// Title
	MenuDrawTitleBar("CAN STATISTICS");

	// Headers
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, STATS_FONT_SIZE, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(STATS_PORT_1X, STATS_POSITION_Y + (0 * STATS_ROW_HEIGHT), "CAN 1", WHITE_TEXT, 100, LAYER_BACK);
	SimpleTextDraw(STATS_PORT_2X, STATS_POSITION_Y + (0 * STATS_ROW_HEIGHT), "CAN 2", WHITE_TEXT, 100, LAYER_BACK);

	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, STATS_FONT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(STATS_TEXT_X, STATS_POSITION_Y + (1 * STATS_ROW_HEIGHT), "Rx Packets:", WHITE_TEXT, 100, LAYER_BACK);
	SimpleTextDraw(STATS_TEXT_X, STATS_POSITION_Y + (2 * STATS_ROW_HEIGHT), "Rx Bytes:", WHITE_TEXT, 100, LAYER_BACK);
	SimpleTextDraw(STATS_TEXT_X, STATS_POSITION_Y + (3 * STATS_ROW_HEIGHT), "Tx Packets:", WHITE_TEXT, 100, LAYER_BACK);
	SimpleTextDraw(STATS_TEXT_X, STATS_POSITION_Y + (4 * STATS_ROW_HEIGHT), "Tx Bytes:", WHITE_TEXT, 100, LAYER_BACK);

	SimpleTextDraw(STATS_TEXT_X, STATS_POSITION_Y + (6 * STATS_ROW_HEIGHT), "Bit Error Count Rx:", WHITE_TEXT, 100, LAYER_BACK);
	SimpleTextDraw(STATS_TEXT_X, STATS_POSITION_Y + (7 * STATS_ROW_HEIGHT), "Bit Error Count Tx:", WHITE_TEXT, 100, LAYER_BACK);
	SimpleTextDraw(STATS_TEXT_X, STATS_POSITION_Y + (8 * STATS_ROW_HEIGHT), "Bus Error Count:", WHITE_TEXT, 100, LAYER_BACK);
	SimpleTextDraw(STATS_TEXT_X, STATS_POSITION_Y + (9 * STATS_ROW_HEIGHT), "Error Warning:", WHITE_TEXT, 100, LAYER_BACK);
	SimpleTextDraw(STATS_TEXT_X, STATS_POSITION_Y + (10 * STATS_ROW_HEIGHT), "Error Passive:", WHITE_TEXT, 100, LAYER_BACK);
	SimpleTextDraw(STATS_TEXT_X, STATS_POSITION_Y + (11 * STATS_ROW_HEIGHT), "Bus Off Count:", WHITE_TEXT, 100, LAYER_BACK);
	SimpleTextDraw(STATS_TEXT_X, STATS_POSITION_Y + (12 * STATS_ROW_HEIGHT), "Restart Count:", WHITE_TEXT, 100, LAYER_BACK);
}

static void _CANViewerCreateScreen(void)
{
	//Setup Keys
	ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
	ButtonBarSetHeight(48);

	if (m_curDisplayMode != DISPLAYMODE_VIEWER)
	{
		MenuDrawTitleBar("");
		ButtonBarSetKeyImages(KEYINDEX_5, &back, &back_s);
		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_5, _CANViewerBackToViewer, nullptr);
	}
	else
	{
		MenuDrawTitleBar("");
		ButtonBarSetKeyImages(KEYINDEX_5, &back, &back_s);
		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_5, _CANViewerExit, nullptr);
	}

	if (m_curDisplayMode != DISPLAYMODE_STATS)
	{
		_CreateNormal();
	}
	else
	{
		_CreateStats();
	}
}

static void _CANViewerCreateExtraButtons(void)
{
	switch (m_curDisplayMode)
	{
	default:
	case DISPLAYMODE_VIEWER:
		// Buttons for the viewer
		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, _CANViewerTogglePause, nullptr);
		ButtonBarSetKeyText(KEYINDEX_1, FONT_INDEX_TTMAIN, 15, BLACK, "PAUSE", 0);


		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_2, _CANViewerPortSelect, nullptr);
		switch (m_curCANPort)
		{
		default:
		case CAN_PORT1:
			ButtonBarSetKeyText(KEYINDEX_2, FONT_INDEX_TTMAIN, 15, BLACK, "PORT", "2");
			break;

		case CAN_PORT2:
			ButtonBarSetKeyText(KEYINDEX_2, FONT_INDEX_TTMAIN, 15, BLACK, "PORT", "1");
			break;
		}
		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_3, _CANViewerLoggerEnter, nullptr);
		ButtonBarSetKeyText(KEYINDEX_3, FONT_INDEX_TTMAIN, 15, BLACK, "LOG", 0);

		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_4, _CANViewerStatsEnter, nullptr);
		ButtonBarSetKeyText(KEYINDEX_4, FONT_INDEX_TTMAIN, 15, BLACK, "STATS", 0);
		break;

	case DISPLAYMODE_VIEWER_PAUSED:
		// Buttons for paused viewer
		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, _CANViewerPageUp, nullptr);
		ButtonBarSetKeyText(KEYINDEX_1, FONT_INDEX_TTMAIN, 15, BLACK, "PG UP", 0);


		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_2, _CANViewerPageDown, nullptr);
		ButtonBarSetKeyText(KEYINDEX_2, FONT_INDEX_TTMAIN, 15, BLACK, "PG DN", 0);
		break;

	case DISPLAYMODE_LOGGER:
		// Button for the logger
		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, _CANViewerLoggerToggleRecord, nullptr);
		if (m_isLoggingToFile)
		{
			ButtonBarSetKeyText(KEYINDEX_1, FONT_INDEX_TTMAIN, 15, BLACK, "STOP", 0);
		}
		else
		{
			ButtonBarSetKeyText(KEYINDEX_1, FONT_INDEX_TTMAIN, 15, BLACK, "START", 0);
		}

		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_2, _CANViewerLoggerPortSelect, nullptr);
		switch (m_loggerCANPort)
		{
		default:
		case CANVIEWERLOGGER_PORT_1:
			ButtonBarSetKeyText(KEYINDEX_2, FONT_INDEX_TTMAIN, 15, BLACK, "PORT", "1");
			break;

		case CANVIEWERLOGGER_PORT_2:
			ButtonBarSetKeyText(KEYINDEX_2, FONT_INDEX_TTMAIN, 15, BLACK, "PORT", "2");
			break;

		case CANVIEWERLOGGER_PORT_BOTH:
			ButtonBarSetKeyText(KEYINDEX_2, FONT_INDEX_TTMAIN, 15, BLACK, "PORT", "BOTH");
			break;
		}
		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_3, _CANViewerLogDelete, nullptr);
		ButtonBarSetKeyText(KEYINDEX_3, FONT_INDEX_TTMAIN, 15, BLACK, "CLEAR", "LOG");
		break;

	case DISPLAYMODE_STATS:
		// No buttons on stats
		break;
	}
}

void CANViewerLogMessageReceive(CAN_PORTS_T canPort, CAN_MSG_T* pMsg)
{
	CTBINLOG_FILERECORD_T fileRec;
	CTBINLOG_FILERECORD_T* pRec;

	// This is needed as this is called from a separate THREAD.
	// Needed due to Callbacks
	MutexLock(&m_canViewerMutex);

	// Are we paused?
	if (m_curDisplayMode != DISPLAYMODE_VIEWER_PAUSED)
	{
		// No. We can save this record in our buffers then
		pRec = &m_frames[canPort][m_frameWriteIndex[canPort]];
		// and increment the write index
		++m_frameWriteIndex[canPort];
		// Wrap the write index if we have gone past the end of the buffer
		if (m_frameWriteIndex[canPort] >= MAX_LOGGED_MESSAGES_PER_PORT)
		{
			m_frameWriteIndex[canPort] = 0;
		}
	}
	else
	{
		// Yep, paused. We will use our local record instead then.
		pRec = &fileRec;
	}

	// Copy message info to our record
	pRec->busNo = (uint8_t)canPort;
	pRec->timeOffsetMicrosecs = pMsg->timestampMicrosecs;
	pRec->id = pMsg->id;
	pRec->flags = ((pMsg->extend != 0) ? CTLOGBIN_FLAGS_EXTENDED : 0);
	pRec->dataLen = pMsg->msg_length;
	memcpy(pRec->data, pMsg->msg_content, CAN_CONTENT_MAX_LENGTH);

	//Unlock.
	MutexUnlock(&m_canViewerMutex);

	// Log record to file
	_CANViewerLogWrite(pRec);
}

void CANViewerLogMessageTransmit(CAN_PORTS_T canPort, CAN_MSG_T* pMsg)
{
	CTBINLOG_FILERECORD_T fileRec;
	CTBINLOG_FILERECORD_T* pRec;

	// This is needed as this is called from a separate THREAD.
	// Needed due to Callbacks
	MutexLock(&m_canViewerMutex);

	// Are we paused?
	if (m_curDisplayMode != DISPLAYMODE_VIEWER_PAUSED)
	{
		// No. We can save this record in our buffers then
		pRec = &m_frames[canPort][m_frameWriteIndex[canPort]];
		// and increment the write index
		++m_frameWriteIndex[canPort];
		// Wrap the write index if we have gone past the end of the buffer
		if (m_frameWriteIndex[canPort] >= MAX_LOGGED_MESSAGES_PER_PORT)
		{
			m_frameWriteIndex[canPort] = 0;
		}
	}
	else
	{
		// Yep. We will use our local record then
		pRec = &fileRec;
	}

	// Copy message info to our record
	pRec->busNo = (uint8_t)canPort;
	// Use current time as it is not filled on a transmit
	pRec->timeOffsetMicrosecs = get_uptime_microseconds();
	pRec->id = pMsg->id;
	pRec->flags = ((pMsg->extend != 0) ? CTLOGBIN_FLAGS_EXTENDED : 0) | CTLOGBIN_FLAGS_TRANSMIT;
	pRec->dataLen = pMsg->msg_length;
	memcpy(pRec->data, pMsg->msg_content, CAN_CONTENT_MAX_LENGTH);

	//Unlock.
	MutexUnlock(&m_canViewerMutex);

	// Log record to file
	_CANViewerLogWrite(pRec);
}

static void _CANViewerTogglePause(void* userData)
{
	// Toggle pause mode
	if (m_curDisplayMode != DISPLAYMODE_VIEWER_PAUSED)
	{
		m_curDisplayMode = DISPLAYMODE_VIEWER_PAUSED;
	}
	else
	{
		m_curDisplayMode = DISPLAYMODE_VIEWER;
	}

	// Reset page
	m_curPage = 0;

	// Force a full redraw
	MMIInvalidate();
}

static void _CANViewerExit(void* userData)
{
	// Reset to the default viewer page
	m_curDisplayMode = DISPLAYMODE_VIEWER;

	// Exit out of the CAN viewer to the previous screen
	MMIScreenGotoPrev();
}

static void _CANViewerPortSelect(void* userData)
{
	// Switch the ports
	if (m_curCANPort == CAN_PORT1)
	{
		m_curCANPort = CAN_PORT2;
	}
	else
	{
		m_curCANPort = CAN_PORT1;
	}

	// Force a full redraw
	MMIInvalidate();
}

static void _CANViewerPageDown(void* userData)
{
	// Increment the page (allow wrap)
	++m_curPage;
	if (m_curPage >= MAX_MESSAGE_PAGES)
	{
		m_curPage = 0;
	}

	// Force a full redraw
	MMIInvalidate();
}

static void _CANViewerPageUp(void* userData)
{
	// Decrement the page (allow wrap)
	--m_curPage;
	if (m_curPage < 0)
	{
		m_curPage = MAX_MESSAGE_PAGES - 1;
	}

	// Force a full redraw
	MMIInvalidate();
}

static void _CANViewerBackToViewer(void* userData)
{
	// Go back to the main viewer page
	m_curDisplayMode = DISPLAYMODE_VIEWER;

	timer_deregister(_CANViewerUpdateStatisticsValues, nullptr);

	// Force a full redraw
	MMIInvalidate();
}

static void _CANViewerLoggerEnter(void* userData)
{
	// Change to logger page
	m_curDisplayMode = DISPLAYMODE_LOGGER;

	// Force a full redraw
	MMIInvalidate();
}

static void _CANViewerLoggerToggleRecord(void* userData)
{
	// Toggle logging to file
	if (m_isLoggingToFile == FALSE)
	{
		m_isLoggingToFile = TRUE;
	}
	else
	{
		m_isLoggingToFile = FALSE;
	}

	// Force a full redraw
	MMIInvalidate();
}

static void _CANViewerLoggerPortSelect(void* userData)
{
	// Increment logging port selection
	switch (m_loggerCANPort)
	{
	case CANVIEWERLOGGER_PORT_1:
		m_loggerCANPort = CANVIEWERLOGGER_PORT_2;
		break;

	case CANVIEWERLOGGER_PORT_2:
		m_loggerCANPort = CANVIEWERLOGGER_PORT_BOTH;
		break;

	default:
	case CANVIEWERLOGGER_PORT_BOTH:
		m_loggerCANPort = CANVIEWERLOGGER_PORT_1;
		break;
	}

	// Force a full redraw
	MMIInvalidate();
}

static void _CANViewerStatsEnter(void* userData)
{
	// Display the stats page
	m_curDisplayMode = DISPLAYMODE_STATS;
	
	timer_register(_CANViewerUpdateStatisticsValues, nullptr, 0, 1000);

	// Force a full redraw
	MMIInvalidate();
}

// Write a record to the log file
static void _CANViewerLogWrite(CTBINLOG_FILERECORD_T* pRecord)
{
	FILE_T* pFile = 0;

	// We only write a log record if we are supposed to be
	if ((m_isLoggingToFile == FALSE) ||
		((m_loggerCANPort == CANVIEWERLOGGER_PORT_1) && (pRecord->busNo != 0)) ||
		((m_loggerCANPort == CANVIEWERLOGGER_PORT_2) && (pRecord->busNo != 1)))
	{
		// Don't need to log this record.
		return;
	}

	// Does the log file exist?
	if (FileExists(FOLDERLOC_LOG, m_pLogFileName) != FALSE)
	{
		// Yep. Open it
		pFile = FileOpen(FOLDERLOC_LOG, m_pLogFileName, "r+");
		// and seek to the end of the file
		FileSeek(pFile, 0, FILESEEK_END);
	}
	else
	{
		// Nope. Create the log file and write the header
		pFile = FileOpen(FOLDERLOC_LOG, m_pLogFileName, "w+");
		if (pFile != 0)
		{
			CTBINLOG_FILEHEADER_T logHeader;
			// Setup header
			strcpy(logHeader.identifier, CTBINLOG_FILEHEADER_IDENTIFIER);
			logHeader.version = CTBINLOG_FILEHEADER_VERSION;
			// and write it to the new log file
			FileWrite(pFile, &logHeader, sizeof(CTBINLOG_FILEHEADER_T));
		}
	}

	if (pFile != 0)
	{
		// Write the record to the file
		FileWrite(pFile, pRecord, sizeof(CTBINLOG_FILERECORD_T));
		// Finished with the file
		FileClose(pFile);
	}
}

void CANViewerLogRotate(void)
{
	// Rename current log file to rotated log file for USB retrieval
	FileRename(FOLDERLOC_LOG, m_pLogFileName, FOLDERLOC_LOG, m_pRotatedLogFileName);
}

static void _CANViewerLogDelete(void* userData)
{
	// Delete current log file
	FileRemove(FOLDERLOC_LOG, m_pLogFileName);
}

static void _CANViewerUpdateStatisticsValues(void* pUserData)
{
	(void)pUserData;

	can_statistics_get(CAN_PORT1, &m_port1Stats);
	can_statistics_get(CAN_PORT2, &m_port2Stats);
}