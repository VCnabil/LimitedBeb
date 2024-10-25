
#include "project.h"

void
platform_init(void)
{
    // Initialise the watchdog
    watchdog_init();

    // Initialise the timers
    timer_init(20);

    // Initialise GPIOs
	gpio_init();

	// Configures keypad
	keypad_init();

	// Initialise buzzer
	buzzer_init();
	// and turn turn buzzer off
	buzzer_out(FALSE);

    // Initialise the LCD hardware
	lcd_init(16);

	// Initialise the dirty rectangle system
	DirtyRectInit();
}

void
portable_init(void)
{
    // Clear LCD
    vLcdBlankerEx(BLACK, ALPHA_COLOR);
}

void
sdk_init(void)
{
    platform_init();

    portable_init();
}
