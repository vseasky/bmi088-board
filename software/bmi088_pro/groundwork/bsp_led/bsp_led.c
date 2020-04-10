#include "cmsis_os.h"
#include "bsp_led.h"
#include "tim.h"
/******************************************************************************
aRGB 为一种色彩模式，aRGB 分别代表了 alpha（透明度）Red（红色）Green（绿色）和
Blue（蓝色）四个要素,一般我们给每个要素设置十进制下 0-255 的取值范围，通过 16 进
制表示就是 0x00-0xFF，因此一个 aRGB 值可以通过八位十六进制数来描述，从前到后每
两位依次对应 a，R，G，B。 在 aRGB 中，alpha 值越大色彩越不透明，RGB 中哪个值越大，对应的色彩就越强。比如
纯红色可以用 8 位 16 进制表示为 0xFFFF0000，纯绿色可以表示为 0xFF00FF00，纯蓝色
可以表示为 0xFF0000FF，黄色由蓝色和绿色合成，所以可以表示为 0xFF00FFFF。
******************************************************************************/
//uint32_t RGB_flow_color[RGB_FLOW_COLOR_LENGHT + 1] = {0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFF0000FF};
void aRGB_led_show(uint32_t aRGB)
{
    static uint8_t alpha;
    static uint16_t red,green,blue;

    alpha = (aRGB & 0xFF000000) >> 24;
    red = ((aRGB & 0x00FF0000) >> 16) * alpha;
    green = ((aRGB & 0x0000FF00) >> 8) * alpha;
    blue = ((aRGB & 0x000000FF) >> 0) * alpha;

    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, blue);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, green);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, red);
}
//void homework_flow_led(void)
//{
//	    uint16_t i, j;
//        fp32 delta_alpha, delta_red, delta_green, delta_blue;
//        fp32 alpha,red,green,blue;
//        uint32_t aRGB;
//        for(i = 0; i < RGB_FLOW_COLOR_LENGHT; i++)
//        {
//            alpha = (RGB_flow_color[i] & 0xFF000000) >> 24;
//            red = ((RGB_flow_color[i] & 0x00FF0000) >> 16);
//            green = ((RGB_flow_color[i] & 0x0000FF00) >> 8);
//            blue = ((RGB_flow_color[i] & 0x000000FF) >> 0);

//            delta_alpha = (fp32)((RGB_flow_color[i + 1] & 0xFF000000) >> 24) - (fp32)((RGB_flow_color[i] & 0xFF000000) >> 24);
//            delta_red = (fp32)((RGB_flow_color[i + 1] & 0x00FF0000) >> 16) - (fp32)((RGB_flow_color[i] & 0x00FF0000) >> 16);
//            delta_green = (fp32)((RGB_flow_color[i + 1] & 0x0000FF00) >> 8) - (fp32)((RGB_flow_color[i] & 0x0000FF00) >> 8);
//            delta_blue = (fp32)((RGB_flow_color[i + 1] & 0x000000FF) >> 0) - (fp32)((RGB_flow_color[i] & 0x000000FF) >> 0);

//            delta_alpha /= RGB_FLOW_COLOR_CHANGE_TIME;
//            delta_red /= RGB_FLOW_COLOR_CHANGE_TIME;
//            delta_green /= RGB_FLOW_COLOR_CHANGE_TIME;
//            delta_blue /= RGB_FLOW_COLOR_CHANGE_TIME;
//            for(j = 0; j < RGB_FLOW_COLOR_CHANGE_TIME; j++)
//            {
//                alpha += delta_alpha;
//                red += delta_red;
//                green += delta_green;
//                blue += delta_blue;

//                aRGB = ((uint32_t)(alpha)) << 24 | ((uint32_t)(red)) << 16 | ((uint32_t)(green)) << 8 | ((uint32_t)(blue)) << 0;
//                aRGB_led_show(aRGB);
//                osDelay(1);
//            }
//		}
//}
