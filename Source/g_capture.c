#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include <png.h>
/* Mac OS X */
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#endif

/* Linux */
#ifdef __linux
#include <GL/glut.h>
#endif

/* Windows */
#ifdef WIN32
#include <windows.h>
#include <GL/glut.h>
#endif

//g_normaly, bit_depth = 8, color_type = PNG_COLOR_TYPE_RGB_ALPHA.
//if you don't reserve buffer for alpha, color_type = PNG_COLOR_TYPE_RGB.
int PNGWrite(const char *file_name, unsigned char **image, size_t width, size_t height, int bit_depth, int color_type);

//g_normaly, x = 0, y = 0, pixel_width and pixel_height are size of client area.
//If you wish clip from window to PNG file, you must set valid value.
int GLCapture(const char *file_name, int x, int y, unsigned int pixel_width, unsigned int pixel_height);

#define DirectoryPathLength 256
#define FilePathLength (DirectoryPathLength + 12)
#define CommandLength (DirectoryPathLength + 7)

static char DirectoryPath[DirectoryPathLength];
static char FilePath[FilePathLength];
static char Command[CommandLength];

static int isInitialized = 0;
static int width, height;
static int count = 0;

static unsigned char *buf = 0;
static unsigned char **pbuf = 0;

static png_structp p_ps = 0;
static png_infop p_pi = 0;

#ifdef NSECURE
#define SNPRINTF(dest, limit, ...) snprintf(dest, limit, __VA_ARGS__)
#else
#define SNPRINTF(dest, limit, ...) \
	{\
		int n = snprintf(dest, limit, __VA_ARGS__);\
		if(n < 0)\
		{\
			fprintf(stderr, __FILE__ ":%s:%s", __func__, strerror(errno));\
			assert(0);\
		}\
	}
#endif

#define STRNCPY(dest, src, limit) {strncpy(dest, src, limit - 1); dest[limit - 1] = '\0';}
#define STRERROR {fprintf(stderr, __FILE__ " : %s : %d : %s\n", __func__, __LINE__, strerror(errno));}

int //error-code:if error occured then return non-0
	//           else return 0
g_capture()
{
	if(!isInitialized)
	{
		fprintf(stderr, "void g_capture(void) should be called after void g_capture_set(directory)\n");
		return 1;
	}

	//file_name = DirectoryPath/%06d.png , count
	sprintf(FilePath, "%s/%06d.png", DirectoryPath, count);

	FILE *fp = fopen(FilePath, "wb");

	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buf);

	p_ps = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	p_pi = png_create_info_struct(p_ps);
	png_init_io(p_ps, fp);
	png_set_IHDR(p_ps, p_pi, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(p_ps, p_pi);
	png_write_image(p_ps, pbuf);
	png_write_end(p_ps, p_pi);
	png_destroy_write_struct(&p_ps, &p_pi);

	count++;

	fclose(fp);
	return 0;
}

int	//error_code:if error occured then return non-0
	//          :else return 0
g_capture_set(name)
	const char *name;//null-terminated string represent directory name
{
	size_t len = name ? strlen(name) : 0;
	int empty_flag = len ? 0 : 1;

	free(buf);
	free(pbuf);

	//for finalize
	if(name == (void*)-1)
		return 0;

	//if len == 0 then DirectoryPath is "Frames.YYYY.MM.DD.hh.mm.ss"
	//else             DirectoryPath is name;
	if(empty_flag)
	{
		time_t sec_time;
		errno = 0;
		if(time(&sec_time) == -1)
		{
			fprintf(stderr, "failed retrieving calender time\n");
			STRERROR;
			return 1;
		}
		const struct tm *date = localtime(&sec_time);
		if(date == NULL)
		{
			fprintf(stderr, "failed to convert calender time to broken-down time\n");
			STRERROR;
			return 2;
		}
		SNPRINTF(DirectoryPath, DirectoryPathLength, "Frames.%04d.%02d.%02d.%02d.%02d.%02d", 
						date->tm_year + 1900, 
						date->tm_mon + 1, 
						date->tm_mday, 
						date->tm_hour,
						date->tm_min,
						date->tm_sec);
	}
	else
		STRNCPY(DirectoryPath, name, DirectoryPathLength);

	SNPRINTF(Command, CommandLength, "mkdir %s", DirectoryPath);
	if(system(Command))
		STRERROR;

	//width = 640;
	//height = 480;
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	buf = malloc(sizeof(char)*width*height*3);
	if(buf == 0)
	{
		STRERROR;
		return 4;
	}

	pbuf = malloc(sizeof(char*)*height);
	if(pbuf == 0)
	{
		STRERROR;
		return 5;
	}

	//Is is necessary to reverse pbuf for writing true order image.
	for(int i = 0; i < height; ++i)
		pbuf[height - 1 - i] = buf + i*width*3;

	isInitialized = 1;
	count = 0;

	return 0;
}
