#include "bsp_usb_com.h"
//#include "usbd_cdc_if.h"
 
//void usb_printf(const char *format, ...)
//{
//    va_list args;
//    uint32_t length;
//    va_start(args, format);
//    length = vsnprintf((char *)UserTxBufferFS, APP_TX_DATA_SIZE, (char *)format, args);
//    va_end(args);
//    CDC_Transmit_FS(UserTxBufferFS,length);
//}