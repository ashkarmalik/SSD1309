#include <stdint.h>
#ifndef SSD1309_H_
#define SSD1309_H_

/*#############################################################################
########################## select your interface here #########################
#############################################################################*/

//#define SSD1309_68XX		// 68XX 8-Bit interface, not implemented yet
#define SSD1309_80XX		// 80XX 8-Bit interface, not implemented yet
//#define	SSD1309_SPI			// SPI (5-Wire) interface, not tested


/*#############################################################################
######################### config 68XX here, if chosen #########################
#############################################################################*/

#ifdef SSD1309_68XX

// TODO

#endif


/*#############################################################################
######################## config 80XX here, if chosen ##########################
#############################################################################*/

#ifdef SSD1309_80XX

#define DATA_PORT		PORTD
#define DATA_DIR		TRISD
#define DATA_PIN		PORTD

#define CONTROL_PORT	PORTB
#define CONTROL_DIR		TRISB

#define RD				(1 << 0)	//Read
#define WR				(1 << 1)	//Write
#define DC				(1 << 2)	//Data Or Command
#define RES				(1 << 3)	//Reset
#define CS				(1 << 4)	//Chip select


#endif


/*#############################################################################
######################### config SPI here, if chosen ##########################
#############################################################################*/

#ifdef SSD1309_SPI

#define SSD1309_PORT	PORTC_OUT	// The used PORT
#define SSD1309_DIR		PORTC_DIR	// The corresponding DDR Register

#define SSD1309_SPI_I	SPIC		// the used SPI port

#define SSD1309_CS		4			// Serial Data Out (TXD)
#define SSD1309_DC		0			// Serial Clock Out (XCK)
#define SSD1309_RES		1			// Latch Signal (GPIO)
#define SSD1309_SCK		7			// Serial Clock
#define SSD1309_MOSI	5			// Master Out Display In

#endif


/*#############################################################################
############################# select display size #############################
#############################################################################*/

#define SSD1309_COL		128			
#define SSD1309_ROW		64


#define BLACK 0
#define WHITE 1

#define SCREEN_WIDTH	128

#define SETLOWCOLUMN 0x00
#define SETHIGHCOLUMN 0x10
#define SETSTARTCOLUMN 0x21
#define SETSTARTPAGE 0x22
#define MEMORYMODE 0x20
#define SETSTARTLINE 0x40
#define SETCONTRAST 0x81
#define SEGREMAP 0xA0
#define SEGREMAP1 0xA1
#define DISPLAYALLON_RESUME 0xA4
#define DISPLAYALLON 0xA5
#define NORMALDISPLAY 0xA6
#define INVERTDISPLAY 0xA7
#define SETMULTIPLEX 0xA8
#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF
#define SET_PAGE 0xB0
#define COMSCANDEC 0xC8
#define SETDISPLAYOFFSET 0xD3
#define SETDISPLAYCLOCKDIV 0xD5
#define SETPRECHARGE 0xD9
#define SETCOMPINS 0xDA
#define SETVCOMDETECT 0xDB
#define SET_MODIFY 0xE0
#define CLR_MODIFY 0xEE
#define NOPS 0xE3

/*#############################################################################
########################### function prototypes ###############################
#############################################################################*/


/**
  * @brief  initialize the display
  *			This function will configure the SPI-Interface and initialize and clean the display.
  *			Have to be executed before any other display function.
  */
void Oled_Init(void);

/**
  * @brief  clears the whole display by writing 0x00 to memory
  */
void clear(void);

/**
  * @brief  puts a single 5x7 char at specified position
  *
  * @param	aChar		the ASCII char to display (see font for available chars)
  * @param  page		the page (line) where the char is displayed
  * @param  startColumn	the column where the char starts
  */
void putc(unsigned char aChar, uint8_t page, uint8_t startColumn);

/**
  * @brief  puts a single 13x16 digit at specified position
  *
  * Only the following chars and white spaces are allowed: 0123456789,./+-
  *
  * @param	aDigit		the ASCII digit to display (see bigDigit for available chars)
  * @param  page		the upper page (line) where the char is displayed, char needs one more page below
  * @param  startColumn	the column where the char starts
  */
void putBigDigit(unsigned char aDigit, uint8_t page, uint8_t startColumn);

/**
  * @brief  prints a string at specified position
  *
  * @param	*aString	pointer to the string to print (0 terminated)
  * @param  page		the page (line) where the string is displayed
  * @param  startColumn	the column where the string starts
  */
void print(char *aString, uint8_t page, uint8_t startColumn);

/**
  * @brief  prints a string containing a number at specified position
  *
  * Only the following chars and white spaces are allowed: 0123456789,./+-
  *
  * @param	*aString	pointer to the string to print (0 terminated)
  * @param  page		the upper page (line) where the string is displayed, string needs one more page below
  * @param  startColumn	the column where the string starts
  */
void print_bigDigit(char *aString, uint8_t page, uint8_t startColumn);

/**
  * @brief  prints a string from program memory at specified position
  *
  * @param	aString	    pointer to the PROGMEM string to print (0x00 terminated)
  * @param  page		the page (line) where the string is displayed
  * @param  startColumn	the column where the string starts
  */
void print_P(const char* aString, uint8_t page, uint8_t startColumn);

/**
  * @brief  prints a formatted string at specified position
  *
  * @param  page		the page (line) where the string is displayed
  * @param  startColumn	the column where the string starts
  */
void printf(uint8_t page, uint8_t startColumn, const char* __fmt, ...);

/**
  * @brief  prints a formatted string at specified position, format string in program memory
  *
  * @param  page		the page (line) where the string is displayed
  * @param  startColumn	the column where the string starts
  */
void printf_P(uint8_t page, uint8_t startColumn, const char* __fmt, ...);

/**
  * @brief  shows a picture at specified position
  *         Use the Image2GLCD tool from http://www.avrportal.com/?page=image2glcd to generate
  *         pattern from an image. Select setting "LSB to MSB Top to Bottom" and correct size.
  *
  * @param	*pic		pointer to the picture data generated by the Image2GLCD tool, type have to be a "const uint8_t PROGMEM" array
  * @param  startPage	the page where the picture starts, each page is 8 pixels high
  * @param  endPage		the page where the picture ends, height of the picture have to be a multiple of 8 and >= startPage
  * @param  startCol	x offset for display position 
  * @param  totalCol	with of the picture
  */
void showPic(const uint8_t *pic, uint8_t startPage, uint8_t endPage, uint8_t startCol, uint8_t totalCol);

/**
  * @brief  draws a bar graph at specified position
  *
  * @param	percent		the filling of the graph (0-100%)
  * @param  startPage	the page where the bar graph starts, each page is 8 pixels high
  * @param  endPage		the page where the bar graph ends, height of the bar graph have to be a multiple of 8 and >= startPage
  * @param  startCol	x offset for display position 
  * @param  totalCol	with of the bar graph
  */
void drawBargraph(uint8_t percent, uint8_t startPage, uint8_t endPage, uint8_t startCol, uint8_t totalCol);


void GoTo(unsigned char x,unsigned char y);

void SetPixel(unsigned char x, unsigned char y, unsigned char color);

void GLCD_WriteChar(char);

void GLCD_WriteString(char * s);

void ssd1309_showPic(const uint8_t *pic, uint8_t startPage, uint8_t endPage, uint8_t startCol, uint8_t totalCol);


#endif /* SSD1309_H_ */