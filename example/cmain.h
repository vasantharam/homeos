#define MAX_MALLOC 2000
#define __KERNEL__DS__ 0x18
#define __KERNEL__CS__ 0x10
#define TRUE 1
#define FALSE 0
#define LOGOSIGNATURE "BMWGETZSWIFT"
#define LOGOSTRSIZE 12
/* The main kernel starts here, for the time being.
 */

void interrupt_interface(char *msg);
/* VESA Extension related data structure */
/* Mode information structure function 01 */
typedef struct _mode_info_
{
	short int ModeAttributes;
	char winAAttributes;
	char winBAttributes;
	short int winGranularity;
	short int winSize;
	short int winASegment;
	short int winBSegment;
	char * winFuncPtr;
	short int bytesPerScanLine;

	short int xResolution;
	short int yResolution;
	char xCharSize;
	char yCharSize;
	char numberOfPlanes;
	char bitsPerPixel;
	char numberOfBanks;
	char memoryModel;
	char bankSize;
	char numberOfImagePages;
	char reserved;

	char redMaskSize;
	char redFieldPosition;
	char greenMaskSize;
	char greenFieldPosition;
	char blueMaskSize;
	char blueFieldPosition;
	char rsvdMaskSize;
	char rsvdFieldPosition;
	char directColorModeInfo;

	char *frameBufferPtr;
	int reserved2;
	short reserved3;
} VESAModeInfo;
typedef struct _BMP_FORMAT_
{
	short bfType;
	int bfSize;
	short bfReserved1;
	short bfReserved2;
	int bfStart;
	int biSize;
	int biWidth;
	int biHeight;
	short biPlanes;
	short biBitCount;
	int biCompression;
	int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	int biClrUsed;
	int biClrImportant;
} BMPFormat;
