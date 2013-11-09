#include "framebuffer.h"
#include "mailbox.h"

/* Framebuffer initialisation failure codes
 * If the FB can't be initialised, one of the following numbers will be
 * flashed on the OK LED
 */

#define FBFAIL_GET_RESOLUTION		        1  /* Mailbox call to get screen resolution failed */
#define FBFAIL_GOT_INVALID_RESOLUTION	  2  /* Mailbox call returned bad resolution */
#define FBFAIL_SETUP_FRAMEBUFFER	      3  /* Mailbox call to setup FB failed */
#define FBFAIL_INVALID_TAGS		          4  /* Setup call FB returned an invalid list of response tags */
#define FBFAIL_INVALID_TAG_RESPONSE	    5  /* Setup FB call returned an invalid response for the framebuffer tag */
#define FBFAIL_INVALID_TAG_DATA		      6  /* Setup FB call returned an invalid address/size */
#define FBFAIL_INVALID_PITCH_RESPONSE	  7  /* Read FB pitch call returned an invalid response */
#define FBFAIL_INVALID_PITCH_DATA	      8  /* Read FB pitch call returned an invalid pitch value */

/* Character cells are 8x16 */
#define CHARSIZE_X	8
#define CHARSIZE_Y	16

extern unsigned char font[][CHARSIZE_Y];
extern uint32_t fontsize;  // contains the size of 'font.bin'

/* Screen parameters set in fb_init() */
static uint32_t screenbase, screensize;
static uint32_t fb_x, fb_y, pitch;
/* Max x/y character cell */
static uint32_t max_x, max_y;

/* Framebuffer initialisation failed. Can't display an error, so flashing
 * the OK LED will have to do
 */
static void fbFail(uint32_t num)  // get in her if an error occurs
{
	while(1);
		//output(num);
}

/* Initialise the framebuffer with nativ resolution */
void fbInitNativ(void)
{
  fbInit(0xFFFFFFFF, 0xFFFFFFFF);
}

/* Initialise the framebuffer */
void fbInit(uint32_t set_fb_x, uint32_t set_fb_y)
{
  uint32_t var;
  uint32_t count;

  /* min resolution */
  if(set_fb_x < 640)
    set_fb_x = 640;
  if(set_fb_y < 480)
    set_fb_y = 480;

  volatile uint32_t mailbuffer[256] __attribute__((aligned (0x10)));

  /* Get the display size */
  mailbuffer[0] = 8 * 4;		// Total size to send via Mailbox
  mailbuffer[1] = 0;		    // Type: Request
  mailbuffer[2] = 0x40003;	// Tag ID: physical display size
  mailbuffer[3] = 8;		    // Buffer size
  mailbuffer[4] = 0;		    // Request size
  mailbuffer[5] = 0;		    // Space for horizontal resolution
  mailbuffer[6] = 0;		    // Space for vertical resolution
  mailbuffer[7] = 0;		    // End tag

  writeMailbox(GPU_MAILBOX_CHANNEL, (uint32_t)mailbuffer);
  var = readMailbox(GPU_MAILBOX_CHANNEL);

  /* Valid response in data structure */
  if(mailbuffer[1] != 0x80000000)
    fbFail(FBFAIL_GET_RESOLUTION);	

  fb_x = mailbuffer[5];
  fb_y = mailbuffer[6];

  /* If both fb_x and fb_y are both zero, assume we're running on the
   * qemu Raspberry Pi emulation (which doesn't return a screen size
   * at this point), and request a 640x480 screen
   */
  if(fb_x==0 && fb_y==0)
  {
    fb_x = 640;
    fb_y = 480;
  }

  if(fb_x==0 || fb_y==0)
    fbFail(FBFAIL_GOT_INVALID_RESOLUTION);

  /* check if wished resolution larger then nativ */
  if(set_fb_x < fb_x)
    fb_x = set_fb_x;
  if(set_fb_y < fb_y)
    fb_y = set_fb_y;

  /* Set up screen */
  uint32_t c = 1;
  mailbuffer[c++] = 0;		// Request

  mailbuffer[c++] = 0x00048003;	// Tag id (set physical size)
  mailbuffer[c++] = 8;		// Value buffer size (bytes)
  mailbuffer[c++] = 8;		// Req. + value length (bytes)
  mailbuffer[c++] = fb_x;		// Horizontal resolution
  mailbuffer[c++] = fb_y;		// Vertical resolution

  mailbuffer[c++] = 0x00048004;	// Tag id (set virtual size)
  mailbuffer[c++] = 8;		// Value buffer size (bytes)
  mailbuffer[c++] = 8;		// Req. + value length (bytes)
  mailbuffer[c++] = fb_x;		// Horizontal resolution
  mailbuffer[c++] = fb_y;		// Vertical resolution

  mailbuffer[c++] = 0x00048005;	// Tag id (set depth)
  mailbuffer[c++] = 4;		// Value buffer size (bytes)
  mailbuffer[c++] = 4;		// Req. + value length (bytes)
  mailbuffer[c++] = 16;		// 16 bpp

  mailbuffer[c++] = 0x00040001;	// Tag id (allocate framebuffer)
  mailbuffer[c++] = 8;		// Value buffer size (bytes)
  mailbuffer[c++] = 4;		// Req. + value length (bytes)
  mailbuffer[c++] = 16;		// Alignment = 16
  mailbuffer[c++] = 0;		// Space for response

  mailbuffer[c++] = 0;		// Terminating tag

  mailbuffer[0] = c*4;		// Buffer size

  writeMailbox(GPU_MAILBOX_CHANNEL, (uint32_t)mailbuffer);
  var = readMailbox(GPU_MAILBOX_CHANNEL);

  /* Valid response in data structure */
  if(mailbuffer[1] != 0x80000000)
    fbFail(FBFAIL_SETUP_FRAMEBUFFER);	

  count=2;	/* First tag */
  while((var = mailbuffer[count]))
  {
    if(var == 0x40001)
      break;

    /* Skip to next tag
     * Advance count by 1 (tag) + 2 (buffer size/value size)
     *                          + specified buffer size
    */
    count += 3+(mailbuffer[count+1]>>2);

    if(count>c)
      fbFail(FBFAIL_INVALID_TAGS);
  }

  /* 8 bytes, plus MSB set to indicate a response */
  if(mailbuffer[count+2] != 0x80000008)
    fbFail(FBFAIL_INVALID_TAG_RESPONSE);

  /* Framebuffer address/size in response */
  screenbase = mailbuffer[count+3];
  screensize = mailbuffer[count+4];

  if(screenbase == 0 || screensize == 0)
    fbFail(FBFAIL_INVALID_TAG_DATA);

  /* Get the framebuffer pitch (bytes per line) */
  mailbuffer[0] = 7 * 4;		// Total size
  mailbuffer[1] = 0;		// Request
  mailbuffer[2] = 0x40008;	// Display size
  mailbuffer[3] = 4;		// Buffer size
  mailbuffer[4] = 0;		// Request size
  mailbuffer[5] = 0;		// Space for pitch
  mailbuffer[6] = 0;		// End tag

  writeMailbox(GPU_MAILBOX_CHANNEL, (uint32_t)mailbuffer);
  var = readMailbox(GPU_MAILBOX_CHANNEL);

  /* 4 bytes, plus MSB set to indicate a response */
  if(mailbuffer[4] != 0x80000004)
    fbFail(FBFAIL_INVALID_PITCH_RESPONSE);

  pitch = mailbuffer[5];
  if(pitch == 0)
    fbFail(FBFAIL_INVALID_PITCH_DATA);

  /* Need to set up max_x/max_y before using console_write */
  max_x = fb_x / CHARSIZE_X;
  max_y = fb_y / CHARSIZE_Y;

  //console_write(COLOUR_PUSH BG_BLUE BG_HALF FG_CYAN
      //"Framebuffer initialised. Address = 0x");
  //console_write(tohex(screenbase, sizeof(screenbase)));
  //console_write(" (physical), 0x");
  //console_write(tohex(screenbase, sizeof(screenbase)));
  //console_write(" (virtual), size = 0x");
  //console_write(tohex(screensize, sizeof(screensize)));
  //console_write(", resolution = ");
  //console_write(todec(fb_x, 0));
  //console_write("x");
  //console_write(todec(fb_y, 0));
  //console_write(COLOUR_POP "\n");
}

/* Current console text cursor position (ie. where the next character will
 * be written
*/
static int32_t consx = 0;
static int32_t consy = 0;

/* Current fg/bg colour */
static uint16_t fgcolour = 0xffff;
static uint16_t bgcolour = 0;

/* A small stack to allow temporary colour changes in text */
static uint32_t colour_stack[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static uint32_t colour_sp = 8;

/* Move to a new line, and, if at the bottom of the screen, scroll the
 * framebuffer 1 character row upwards, discarding the top row
 */
static void newline()
{
  //uint32_t source;
  /* Number of bytes in a character row */
  //uint32_t rowbytes = CHARSIZE_Y * pitch;

  consx = 0;
  if(consy<(max_y-1))
  {
    consy++;
    return;
  }

  /* Copy a screen's worth of data (minus 1 character row) from the
   * second row to the first
   */

  /* Calculate the address to copy the screen data from */
  //source = screenbase + rowbytes;
  //memmove((void *)screenbase, (void *)source, (max_y-1)*rowbytes);

  /* Clear last line on screen */
  //memclr((void *)(screenbase + (max_y-1)*rowbytes), rowbytes);
}

/* Write null-terminated text to the console
 * Supports control characters (see framebuffer.h) for colour and newline
 */
void consoleWrite(char *text)
{
  //volatile unsigned short int *ptr;
  volatile uint16_t* ptr;

  uint32_t row, addr;
  int32_t  col;
  unsigned char ch;

  /* Double parentheses to silence compiler warnings about
   * assignments as boolean values
   */
  while((ch = (unsigned char)*text))
  {
    text++;

    /* Deal with control codes */
    switch(ch)
    {
      case 1: fgcolour = 0b1111100000000000; continue;
      case 2: fgcolour = 0b0000011111100000; continue;
      case 3: fgcolour = 0b0000000000011111; continue;
      case 4: fgcolour = 0b1111111111100000; continue;
      case 5: fgcolour = 0b1111100000011111; continue;
      case 6: fgcolour = 0b0000011111111111; continue;
      case 7: fgcolour = 0b1111111111111111; continue;
      case 8: fgcolour = 0b0000000000000000; continue;
        /* Half brightness */
      case 9: fgcolour = (fgcolour >> 1) & 0b0111101111101111; continue;
      case 10: newline(); continue;
      case 11: /* Colour stack push */
        if(colour_sp)
          colour_sp--;
        colour_stack[colour_sp] =
          fgcolour | (bgcolour<<16);
        continue;
      case 12: /* Colour stack pop */
        fgcolour = colour_stack[colour_sp] & 0xffff;
        bgcolour = colour_stack[colour_sp] >> 16;
        if(colour_sp<8)
          colour_sp++;
        continue;
      case 17: bgcolour = 0b1111100000000000; continue;
      case 18: bgcolour = 0b0000011111100000; continue;
      case 19: bgcolour = 0b0000000000011111; continue;
      case 20: bgcolour = 0b1111111111100000; continue;
      case 21: bgcolour = 0b1111100000011111; continue;
      case 22: bgcolour = 0b0000011111111111; continue;
      case 23: bgcolour = 0b1111111111111111; continue;
      case 24: bgcolour = 0b0000000000000000; continue;
        /* Half brightness */
      case 25: bgcolour = (bgcolour >> 1) & 0b0111101111101111; continue;
    }

    /* Unknown control codes, and anything >127, get turned into
     * spaces. Anything >=32 <=127 gets 32 subtracted from it to
     * turn it into a value between 0 and 95, to index into the
     * character definitions table
     */
    if(ch<32)
    {
      ch=0;
    }
    else
    {
      if(ch>127)
        ch=0;
    }

    /* Plot character onto screen
     *
     * CHARSIZE_Y and CHARSIZE_X are the size of the block the
     * character occupies. The character itself is one pixel
     * smaller in each direction, and is located in the upper left
     * of the block
     */
    for(row=0; row<CHARSIZE_Y; row++)
    {
      addr = (row+consy*CHARSIZE_Y)*pitch + consx*CHARSIZE_X*2;

      for(col=0; col<CHARSIZE_X; ++col)
      {
        ptr = (unsigned short int *)(screenbase+addr);

        addr+=2;

        if(row<(CHARSIZE_Y-1) && (font[ch][row] & (1<<col)))
          *ptr = fgcolour;
        else
          *ptr = bgcolour;
      }

      ptr = (unsigned short int *)(screenbase+addr);
      *ptr = bgcolour;
    }

    if(++consx >=max_x)
    {
      newline();
    }
  }
}
