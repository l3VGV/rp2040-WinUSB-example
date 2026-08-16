#include "tusb.h"
#include "usb_descriptors.h"

#define USBD_VID 0xCafe
#define USBD_PID 0x4017
#define USBD_BCD 0x0210

//--------------------------------------------------------------------+
// Device Descriptor
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = USBD_BCD,
    
    .bDeviceClass       = TUSB_CLASS_VENDOR_SPECIFIC, 
    .bDeviceSubClass    = 0,
    .bDeviceProtocol    = 0,
    
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = USBD_VID,
    .idProduct          = USBD_PID,
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};

uint8_t const * tud_descriptor_device_cb(void) {
  return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+
enum {
  ITF_NUM_VENDOR = 0,
  ITF_NUM_TOTAL
};

#define EPNUM_VENDOR_OUT 0x01
#define EPNUM_VENDOR_IN  0x81

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_VENDOR_DESC_LEN) 

uint8_t const desc_configuration[] = {
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

  // Interface number, string index, EP Out & EP In address, EP size
  TUD_VENDOR_DESCRIPTOR(ITF_NUM_VENDOR, 0, EPNUM_VENDOR_OUT, EPNUM_VENDOR_IN, 64)
};

uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
  (void) index;
  return desc_configuration;
}

//--------------------------------------------------------------------+
// BOS & Microsoft OS 2.0 Descriptors 
//--------------------------------------------------------------------+
#define BOS_TOTAL_LEN      (TUD_BOS_DESC_LEN + TUD_BOS_MICROSOFT_OS_DESC_LEN)
#define MS_OS_20_DESC_LEN  0x00A2 // 162 bytes


// BOS Descriptor
uint8_t const desc_bos[] = {
  // Total length calculation, 1 device capability entry
  TUD_BOS_DESCRIPTOR(BOS_TOTAL_LEN, 1),

  // Microsoft OS 2.0 descriptor platform capability. Vendor Code = 2
   TUD_BOS_MS_OS_20_DESCRIPTOR(MS_OS_20_DESC_LEN, VENDOR_REQUEST_MICROSOFT)
};

TU_VERIFY_STATIC(sizeof(desc_bos) == BOS_TOTAL_LEN, "Incorrect size");

// uint8_t const ms_os_bos[] = {TUD_BOS_MS_OS_20_DESCRIPTOR(MS_OS_20_DESC_LEN, VENDOR_REQUEST_MICROSOFT)};
// TU_VERIFY_STATIC(sizeof(ms_os_bos) == TUD_BOS_MICROSOFT_OS_DESC_LEN, "Incorrect size");

uint8_t const * tud_descriptor_bos_cb(void) {
  return desc_bos;
}


uint8_t const desc_ms_os_20[] =
{
  // 1. Set header: length (10), type, windows version, total length (162)
  U16_TO_U8S_LE(0x000A), U16_TO_U8S_LE(MS_OS_20_SET_HEADER_DESCRIPTOR), U32_TO_U8S_LE(0x06030000), U16_TO_U8S_LE(MS_OS_20_DESC_LEN),

  // 2. MS OS 2.0 Compatible ID descriptor: length (20), type, compatible ID, sub compatible ID
  U16_TO_U8S_LE(0x0014), U16_TO_U8S_LE(MS_OS_20_FEATURE_COMPATBLE_ID), 'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // 3. MS OS 2.0 Registry property descriptor: length (132 / 0x0084), type
  U16_TO_U8S_LE(0x0084), U16_TO_U8S_LE(MS_OS_20_FEATURE_REG_PROPERTY),
  U16_TO_U8S_LE(0x0007), U16_TO_U8S_LE(0x002A), // wPropertyDataType, wPropertyNameLength and PropertyName "DeviceInterfaceGUIDs\0" in UTF-16
  'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00, 't', 0x00, 'e', 0x00,
  'r', 0x00, 'f', 0x00, 'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, 0x00, 0x00,
  
  U16_TO_U8S_LE(0x0050), // wPropertyDataLength (80 bytes)
  // bPropertyData: "{8F8B3C2E-4A1D-4E6B-8F3C-1A2B3C4D5E6F}"
  '{', 0, '8', 0, 'F', 0, '8', 0, 'B', 0, '3', 0, 'C', 0, '2', 0, 'E', 0, '-', 0, '4', 0, 'A', 0, '1', 0, 'D', 0, '-', 0, '4', 0, 'E', 0, '6', 0, 'B', 0, '-', 0, '8', 0, 'F', 0, '3', 0, 'C', 0, '-', 0, '1', 0, 'A', 0, '2', 0, 'B', 0, '3', 0, 'C', 0, '4', 0, 'D', 0, '5', 0, 'E', 0, '6', 0, 'F', 0, '}', 0x00, 0x00, 0x00, 0x00, 0x00
};


uint8_t const * tud_descriptor_ms_os_20_cb(void) {
  return desc_ms_os_20;
}

TU_VERIFY_STATIC(sizeof(desc_ms_os_20) == MS_OS_20_DESC_LEN, "Incorrect size");

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// array of pointer to string descriptors
char const* string_desc_arr [] =
{
  (const char[]) { 0x09, 0x04 }, // 0: is supported language is English (0x0409)
  "TinyUSB",                     // 1: Manufacturer
  "TinyUSB Device",              // 2: Product
  "123456",                      // 3: Serials, should use chip ID
  "TinyUSB CDC",                 // 4: CDC Interface
  "TinyUSB WebUSB"               // 5: Vendor Interface
};

static uint16_t _desc_str[32] = {0};

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  (void) langid;

  uint8_t chr_count;

  if ( index == 0)
  {
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
  }else
  {
    // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
    // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

    if ( !(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) ) return NULL;

    const char* str = string_desc_arr[index];

    // Cap at max char
    chr_count = (uint8_t) strlen(str);
    if ( chr_count > 31 ) chr_count = 31;

    // Convert ASCII string into UTF-16
    for(uint8_t i=0; i<chr_count; i++)
    {
      _desc_str[1+i] = str[i];
    }
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8 ) | (2*chr_count + 2));

  return _desc_str;
}




bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const * request)
{
  // nothing to with DATA & ACK stage
  if (stage != CONTROL_STAGE_SETUP) return true;

  switch (request->bmRequestType_bit.type)
  {
    case TUSB_REQ_TYPE_VENDOR:
      switch (request->bRequest)
      {
          case VENDOR_REQUEST_MICROSOFT:
          if ( request->wIndex == 7 )
          {
            return tud_control_xfer(rhport, request, (uint8_t*)desc_ms_os_20, MS_OS_20_DESC_LEN);
          }else
          {
            return false;
          }

        default: break;
      }
    break;

    case TUSB_REQ_TYPE_CLASS://
      if (request->bRequest == 0x22)
      {

        // response with status OK
        return tud_control_status(rhport, request);
      }
    break;

    default: break;
  }

  // stall unknown request
  return false;
}



