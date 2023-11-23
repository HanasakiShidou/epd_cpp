#include <csignal>
#include <iostream>

extern "C" {
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include "ImageData.h"
#include "Debug.h"
#include "DEV_Config.h"
}
#include <stdlib.h>
#include "../lib/Hardware/npi_neo3/npi_neo3.hpp"
#include "../lib/e-Paper/2in9v2/epd_2in9v2.hpp"

static constexpr int EPD_PWR_PIN     = 101;
static constexpr int EPD_DIN_PIN     = 81;
static constexpr int EPD_CLK_PIN     = 79;
static constexpr int EPD_CS_PIN      = 104;
static constexpr int EPD_DC_PIN      = 83;
static constexpr int EPD_RST_PIN     = 102;
static constexpr int EPD_BUSY_PIN    = 100;

void Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:exit\r\n");
    //DEV_Module_Exit();

    exit(0);
}

int main()
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);

    // hardware api
    auto hardwareapi = std::unique_ptr<HardwareAPI>(new NPiNeo3_Soft_SPI(EPD_PWR_PIN, EPD_DIN_PIN, EPD_CLK_PIN, EPD_CS_PIN, EPD_DC_PIN, EPD_RST_PIN, EPD_BUSY_PIN));
    if (!hardwareapi->Available()) {
        std::cout<<"Init failed"<<std::endl;
        return -1;
    }
    EPaper_2in9v2 epd(std::move(hardwareapi));

#ifdef epd2in9V2

    printf("EPD_2IN9_V2_test Demo\r\n");
    epd.Init();

	struct timespec start={0,0}, finish={0,0}; 
    clock_gettime(CLOCK_REALTIME,&start);
    epd.Clear();
	clock_gettime(CLOCK_REALTIME,&finish);
    printf("Init and clear takes %ld Second(s)\r\n",finish.tv_sec-start.tv_sec);	

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
	Paint_Clear(WHITE);
	
#if 0   // show bmp
	Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 0, WHITE);
    printf("show window BMP-----------------\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/100x100.bmp", 10, 10);
    epd.Display(BlackImage);
    DEV_Delay_ms(3000);

    printf("show bmp------------------------\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/2in9.bmp", 0, 0);
    epd.Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 0  //show image for array  
    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);  
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_2in9);

    epd.Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 0  // Drawing on the image
	Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);  	
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    // 2.Drawing on the image
/*
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
*/
    //Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "Peace & Love", &Font12, WHITE, BLACK);

    //Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    //Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

    //Paint_DrawString_CN(130, 0,"你好abc", &Font12CN, BLACK, WHITE);
    //Paint_DrawString_CN(130, 20, "微雪电子", &Font24CN, WHITE, BLACK);

    epd.DisplayBase(BlackImage);
    //DEV_Delay_ms(10000);
    sleep(10);
#endif

#if 1   //Partial refresh, example shows time    		
	Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);  
    printf("Partial refresh\r\n");
    Paint_SelectImage(BlackImage);
	
    PAINT_TIME sPaint_time;
    sPaint_time.Hour = 0;
    sPaint_time.Min = 0;
    sPaint_time.Sec = 0;
    UBYTE num = 600;
    std::cout<<"print time"<<std::endl;
    bool first_run = true;
    for (;;) {
        time_t now = time(nullptr);

        struct tm * currtime = localtime(&now);
        sPaint_time.Hour = currtime->tm_hour;
        sPaint_time.Min = currtime->tm_min;
        sPaint_time.Sec = currtime->tm_sec;

        Paint_ClearWindows(0, 80, 0 + Font20.Width * 18, 80 + Font20.Height, WHITE);
        Paint_DrawString_EN(0, 80, "Currtime:", &Font20, WHITE, BLACK);
        Paint_DrawTime(0 + Font20.Width * 11, 80, &sPaint_time, &Font20, WHITE, BLACK);

        num = num - 1;
        if(num == 0) {
            break;
        }
        if (first_run) {
		    epd.Display(BlackImage);
            first_run = false;
        } else {
            epd.DisplayPartial(BlackImage);
        }
        usleep(500);

    }
#endif

	printf("Clear...\r\n");
	epd.Init();
    epd.Clear();
	
    printf("Goto Sleep...\r\n");
    epd.Sleep();
    free(BlackImage);
    BlackImage = NULL;
    sleep(3);
    //DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    //DEV_Module_Exit();
    return 0;

#else
    printf("Please specify the EPD model when making. \r\n");
    printf("Example: When you run the EPD_7in5_V2_test() program, input: sudo make clean && make EPD=epd7in5V2 \r\n");
    printf("Don't know which program you need to run? Refer to the user manual (Wiki) and main.c \r\n");
#endif
    
    return 0;
}
