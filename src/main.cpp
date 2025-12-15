#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

FUSES = {
	.WDTCFG = 0,
	.BODCFG = ACTIVE_ENABLED_gc | LVL_BODLEVEL2_gc | SAMPFREQ_125HZ_gc, // NOLINT(*-suspicious-enum-usage)
	.OSCCFG = FUSE_OSCCFG_DEFAULT | FREQSEL_20MHZ_gc,
	.TCD0CFG = 0,
	.SYSCFG0 = FUSE_SYSCFG0_DEFAULT | FUSE_EESAVE_bm,
	.SYSCFG1 = SUT_32MS_gc,
};

int main() {
	// Configure Clock (20MHz internal)
	// disable prescaler
	CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB &= ~CLKCTRL_PEN_bm;
	// Lock Clock configuration
	CLKCTRL.MCLKLOCK = true;

	// LED outputs
	PORTA.DIRSET = PIN7_bm;

	sei();

	constexpr auto delay_time_s{500};

	// ReSharper disable once CppDFAEndlessLoop
	while (true) {
		_delay_ms(delay_time_s);
		PORTA.OUTSET = PIN7_bm;
		_delay_ms(delay_time_s);
		PORTA.OUTCLR = PIN7_bm;
	}
}
