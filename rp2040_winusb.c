#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"

#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

uint8_t buf[1024];

bool transmit = false;

void tud_vendor_rx_cb(uint8_t itf)
{
    tud_vendor_read(buf, tud_vendor_available());

    // transmit = !transmit;
   //  if(!transmit) return;

    tud_vendor_write(buf, CFG_TUD_VENDOR_TX_BUFSIZE);
     tud_vendor_flush();
}


void tud_vendor_tx_cb(uint8_t itf, uint32_t sent_bytes)
{
    //if(!transmit) return;
    uint32_t canSend = MIN(tud_vendor_write_available(), 1024);

    //if(canSend > 128)
        tud_vendor_write(buf, canSend);
}


int main()
{
    stdio_init_all();

  tusb_init();      // Initialize TinyUSB stack
  tud_disconnect(); // force reset usb without power off
  sleep_ms(350);    // should be enof so OS will detect inactivity and reset bus and start reunumeration
  tud_connect();  
  
   printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
   printf("USB Clock Frequency is %d Hz\n", clock_get_hz(clk_usb));  

    while (true) {
        tud_task();
    }
}





///////////////////////////////












