#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#define CFG_TUSB_MCU             OPT_MCU_RP2040
#define CFG_TUSB_RHPORT0_MODE    (OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)

#define TUD_OPT_RP2040_USB_DEVICE_UFRAME_FIX 0

#define CFG_TUD_MAX_SPEED 1

#define CFG_TUD_VENDOR            1

// Adjust buffer size based on your transfer speed needs
#define CFG_TUD_VENDOR_RX_BUFSIZE 64
#define CFG_TUD_VENDOR_TX_BUFSIZE 512


#define CFG_TUD_INTERRUPT 1


#endif