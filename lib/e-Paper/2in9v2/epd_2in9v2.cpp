/*****************************************************************************
* | File      	:  	EPD_2in9_V2.c
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper V2
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-10-20
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "epd_2in9v2.hpp"
extern "C" {
#include "../../Config/Debug.h"
}

static uint8_t _WF_PARTIAL_2IN9[159] =
{
0x0,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x80,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x40,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0A,0x0,0x0,0x0,0x0,0x0,0x2,  
0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x22,0x22,0x22,0x22,0x22,0x22,0x0,0x0,0x0,
0x22,0x17,0x41,0xB0,0x32,0x36,
};

static uint8_t WS_20_30[159] =
{											
0x80,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x40,	0x0,	0x0,	0x0,
0x10,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x20,	0x0,	0x0,	0x0,
0x80,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x40,	0x0,	0x0,	0x0,
0x10,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x20,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x14,	0x8,	0x0,	0x0,	0x0,	0x0,	0x1,					
0xA,	0xA,	0x0,	0xA,	0xA,	0x0,	0x1,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x14,	0x8,	0x0,	0x1,	0x0,	0x0,	0x1,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x1,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x44,	0x44,	0x44,	0x44,	0x44,	0x44,	0x0,	0x0,	0x0,			
0x22,	0x17,	0x41,	0x0,	0x32,	0x36
};

EPaper_2in9v2::EPaper_2in9v2(std::unique_ptr<HardwareAPI> api) : api_(std::move(api)) {}

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
void EPaper_2in9v2::Reset()
{
	api_->DigitalWrite(RST, 1);
	api_->Delay(10);
	api_->DigitalWrite(RST, 0);
	api_->Delay(2);
	api_->DigitalWrite(RST, 1);
	api_->Delay(10);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
void EPaper_2in9v2::SendCommand(uint8_t reg)
{
	api_->DigitalWrite(DC, 0);
	api_->DigitalWrite(CS, 0);
	api_->SPIWriteByte(reg);
	api_->DigitalWrite(CS, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
void EPaper_2in9v2::SendData(uint8_t data)
{
	api_->DigitalWrite(DC, 1);
	api_->DigitalWrite(CS, 0);
	api_->SPIWriteByte(data);
	api_->DigitalWrite(CS, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void EPaper_2in9v2::ReadBusy()
{
    //Debug("e-Paper busy\r\n");
	int busy = 1;
	while(1)
	{	 //=1 BUSY
		if(api_->DigitalRead(BUSY)==0) 
			break;
		if (busy == 1) {
			Debug("e-Paper busy\r\n");
			busy = 0;
		}
		api_->Delay(50);
	}
	api_->Delay(50);
    Debug("e-Paper busy release\r\n");
}

void EPaper_2in9v2::Lut(uint8_t *lut)
{
	uint8_t count;
	SendCommand(0x32);
	for(count=0; count<153; count++) 
		SendData(lut[count]); 
	ReadBusy();
}

void EPaper_2in9v2::LutByHost(uint8_t *lut)
{
	Lut((uint8_t *)lut);			//lut
	SendCommand(0x3f);
	SendData(*(lut+153));
	SendCommand(0x03);	// gate voltage
	SendData(*(lut+154));
	SendCommand(0x04);	// source voltage
	SendData(*(lut+155));	// VSH
	SendData(*(lut+156));	// VSH2
	SendData(*(lut+157));	// VSL
	SendCommand(0x2c);		// VCOM
	SendData(*(lut+158));
	
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
void EPaper_2in9v2::TurnOnDisplay()
{
	SendCommand(0x22); //Display Update Control
	SendData(0xc7);
	SendCommand(0x20); //Activate Display Update Sequence
	ReadBusy();
}

void EPaper_2in9v2::TurnOnDisplayPartial()
{
	SendCommand(0x22); //Display Update Control
	SendData(0x0F);   
	SendCommand(0x20); //Activate Display Update Sequence
	ReadBusy();
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
void EPaper_2in9v2::SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    SendData((Xstart>>3) & 0xFF);
    SendData((Xend>>3) & 0xFF);
	
    SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    SendData(Ystart & 0xFF);
    SendData((Ystart >> 8) & 0xFF);
    SendData(Yend & 0xFF);
    SendData((Yend >> 8) & 0xFF);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
void EPaper_2in9v2::SetCursor(uint16_t Xstart, uint16_t Ystart)
{
    SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    SendData(Xstart & 0xFF);

    SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    SendData(Ystart & 0xFF);
    SendData((Ystart >> 8) & 0xFF);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPaper_2in9v2::Init()
{
	Reset();
	api_->Delay(100);

	ReadBusy();   
	SendCommand(0x12); // soft reset
	ReadBusy();
	
	SendCommand(0x01); //Driver output control      
	SendData(0x27);
	SendData(0x01);
	SendData(0x00);
	
	SendCommand(0x11); //data entry mode       
	SendData(0x03);
 
	SetWindows(0, 0, EPD_2IN9_V2_WIDTH-1, EPD_2IN9_V2_HEIGHT-1);
 
	SendCommand(0x21); //  Display update control
	SendData(0x00);
	SendData(0x80);	
	
	SetCursor(0, 0);
	ReadBusy();	
	
	LutByHost(WS_20_30);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPaper_2in9v2::Clear()
{
	uint16_t i;
	
	SendCommand(0x24);   //write RAM for black(0)/white (1)
	for(i=0;i<4736;i++)
	{
		SendData(0xff);
	}
	TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPaper_2in9v2::Display(uint8_t *image)
{
	uint16_t i;	
	SendCommand(0x24);   //write RAM for black(0)/white (1)
	for(i=0;i<4736;i++)
	{
		SendData(image[i]);
	}
	TurnOnDisplay();	
}

void EPaper_2in9v2::DisplayBase(uint8_t *image)
{
	uint16_t i;   

	SendCommand(0x24);   //Write Black and White image to RAM
	for(i=0;i<4736;i++)
	{               
		SendData(image[i]);
	}
	SendCommand(0x26);   //Write Black and White image to RAM
	for(i=0;i<4736;i++)
	{               
		SendData(image[i]);
	}
	TurnOnDisplay();	
}

void EPaper_2in9v2::DisplayPartial(uint8_t *image)
{
	uint16_t i;

//Reset
	api_->DigitalWrite(RST, 0);
	api_->Delay(1);
	api_->DigitalWrite(RST, 1);
	api_->Delay(2);

	Lut(_WF_PARTIAL_2IN9);
	SendCommand(0x37); 
	SendData(0x00);  
	SendData(0x00);  
	SendData(0x00);  
	SendData(0x00); 
	SendData(0x00);  
	SendData(0x40);  
	SendData(0x00);  
	SendData(0x00);   
	SendData(0x00);  
	SendData(0x00);

	SendCommand(0x3C); //BorderWavefrom
	SendData(0x80);	

	SendCommand(0x22); 
	SendData(0xC0);   
	SendCommand(0x20); 
	ReadBusy();  
	
	SetWindows(0, 0, EPD_2IN9_V2_WIDTH-1, EPD_2IN9_V2_HEIGHT-1);
	SetCursor(0, 0);

	SendCommand(0x24);   //Write Black and White image to RAM
	for(i=0;i<4736;i++)
	{
		SendData(image[i]);
	} 
	TurnOnDisplayPartial();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPaper_2in9v2::Sleep()
{
	SendCommand(0x10); //enter deep sleep
	SendData(0x01); 
	api_->Delay(100);
}
