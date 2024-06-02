
#include <iostream>
#include <dlfcn.h>
#include <cstdlib>
#include <linux/input.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <list>
#include <cstring>
#include "structs.h"

enum DrumID {
	HiHat,
	Snare,
	Tom1,
	Tom2,
	Cymbal,
	Bass
};

uint8_t IOState[6] = { 0 };

struct DLSBind {
    const char* Key;
	DrumID Drum;
};

const std::list<DLSBind> Binds = {
    { "d", HiHat },
    { "f", Snare },
    { "g", Bass },
    { "h", Tom1 },
    { "j", Tom2 },
    { "k", Cymbal },
};

struct __attribute__((packed)) __attribute__((aligned(1))) CUSBIO
{
	uint32_t vftable;
	uint8_t state[12];
	uint32_t USBDeviceHandle;
	char HiHat;
	char Snare;
	char Tom1;
	char Tom2;
	char Cymbal;
	char Bass;
	char Unk0;
	char Unk1;
	uint32_t dword1C;
	uint8_t byte20;
	uint8_t byte21;
	uint8_t byte22;
	uint8_t gap23[2];
	uint8_t byte25;
	uint8_t gap26[2];
	uint16_t word28;
	uint8_t HasIOBoard;
};

extern "C" int usb_control_msg(usb_dev_handle *dev, int requesttype, int request, int value, int index, char *bytes, int size, int timeout) {
    static auto o_usb_control_msg = reinterpret_cast<decltype(&usb_control_msg)>(dlsym(RTLD_NEXT, "usb_control_msg"));

    if (!o_usb_control_msg) {
        std::cerr << "o_usb_control_msg was NULL" << std::endl;
        std::exit(1);
    }

	if (reinterpret_cast<uint32_t>(dev) != 0xCAFEBABE || size != 8) {
		return o_usb_control_msg(dev, requesttype, request, value, index, bytes, size, timeout);
	}
	
	memcpy(bytes, IOState, 6);

    return 1;
}

extern "C" int XNextEvent(Display* display, XEvent* event_return) {
    static auto oXNextEvent = reinterpret_cast<decltype(&XNextEvent)>(dlsym(RTLD_NEXT, "XNextEvent"));

    if (!oXNextEvent) {
        std::cerr << "o_x_next_event was NULL" << std::endl;
        std::exit(1);
    }

	CUSBIO* USBIO = *reinterpret_cast<CUSBIO**>(0x81A5B54);
	
	if (!USBIO->HasIOBoard) {
		USBIO->HasIOBoard = true;
		USBIO->USBDeviceHandle = 0xCAFEBABE;

		printf("Faked IO board\n");
	}

    int ret = oXNextEvent(display, event_return);

	for (const auto& bind : Binds) {
		if (event_return->xkey.keycode == XKeysymToKeycode(display, XStringToKeysym(bind.Key))) {
			switch (event_return->type) {
				case KeyPress:
					IOState[bind.Drum] = 255;
					break;
				case KeyRelease:
					IOState[bind.Drum] = 0;
					break;
			}
		}
	}


    return ret;
}
