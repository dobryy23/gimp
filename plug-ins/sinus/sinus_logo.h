/*  GIMP header image file format (INDEXED): \tmp\logo.h  */

static unsigned int width = 100;
static unsigned int height = 100;

/*  Call this macro repeatedly.  After each use, the pixel data can be extracted  */

#define HEADER_PIXEL(data,pixel) {\
  pixel[0] = header_data_cmap[(unsigned char)data[0]][0]; \
  pixel[1] = header_data_cmap[(unsigned char)data[0]][1]; \
  pixel[2] = header_data_cmap[(unsigned char)data[0]][2]; \
  data ++; }

static char header_data_cmap[256][3] = {
	{210,214,212},
	{187,178,183},
	{184,185,180},
	{172,172,165},
	{172,164,165},
	{122, 96, 93},
	{ 88, 58, 46},
	{109, 71, 58},
	{108, 72, 71},
	{109, 65, 60},
	{127, 65, 63},
	{ 98, 66, 43},
	{122, 71, 71},
	{122,104, 99},
	{ 98, 81, 75},
	{108, 81, 72},
	{ 89, 89, 81},
	{122,113,111},
	{136,140,132},
	{158,149,150},
	{159,157,153},
	{122,118,116},
	{112,105,103},
	{ 42, 44, 38},
	{ 58, 54, 44},
	{ 71, 58, 57},
	{ 52, 36, 37},
	{ 40, 21, 22},
	{ 37,  4,  4},
	{ 39, 12,  5},
	{ 56,  5,  4},
	{ 53, 27, 19},
	{ 57, 11,  4},
	{ 53, 20, 21},
	{ 53, 44, 41},
	{ 56, 53, 56},
	{198,190,196},
	{ 26,  3,  4},
	{ 50, 62, 52},
	{ 42, 36, 34},
	{ 69, 12,  5},
	{ 25, 13,  4},
	{ 40, 28, 20},
	{ 82, 13,  6},
	{ 87, 20, 11},
	{111, 63, 13},
	{114, 72,  8},
	{ 72, 20,  8},
	{ 82,  6,  4},
	{ 73,  4,  4},
	{113,110,104},
	{ 71, 44, 35},
	{ 89, 27, 11},
	{ 71, 27,  7},
	{162,104, 12},
	{159,110, 12},
	{ 66, 74, 68},
	{ 71, 28, 22},
	{ 98, 28, 12},
	{170,116, 12},
	{178,124, 12},
	{194,150, 12},
	{ 98, 19, 12},
	{ 67, 37, 44},
	{ 70, 64, 66},
	{147, 89, 12},
	{169,124, 12},
	{125, 64, 12},
	{206,156, 12},
	{161, 97, 12},
	{190,132, 12},
	{190,140, 12},
	{181,116, 12},
	{ 87, 80, 75},
	{ 85, 72, 65},
	{130,132,104},
	{146,148,143},
	{ 68, 53, 55},
	{102, 44, 11},
	{210,166, 12},
	{116, 58, 12},
	{129, 71, 12},
	{ 87, 44, 10},
	{118, 52, 13},
	{101, 37, 19},
	{205,148, 12},
	{ 70, 43,  6},
	{ 74, 70, 73},
	{130, 79, 12},
	{146,140,139},
	{220,164, 12},
	{108, 28, 12},
	{109, 21, 12},
	{111, 94, 79},
	{122,124,116},
	{ 87, 53, 48},
	{ 86, 36, 10},
	{116, 37, 13},
	{124, 81, 77},
	{ 98, 72, 63},
	{ 89, 63, 39},
	{123, 89, 84},
	{ 87, 65, 64},
	{136,132,130},
	{140, 70, 12},
	{100, 52, 48},
	{130,106, 95},
	{130,118,120},
	{ 99, 44, 33},
	{130, 54, 12},
	{ 95, 78, 57},
	{132,113,115},
	{131,124,119},
	{ 73, 20, 21},
	{109, 87, 76},
	{143, 80, 12},
	{115, 44, 19},
	{102, 58, 50},
	{103, 53,  9},
	{113, 97, 97},
	{ 58, 34,  4},
	{ 72, 79, 79},
	{ 66, 51, 44},
	{ 66, 62, 52},
	{146, 95, 12},
	{ 52, 58, 53},
	{ 33,103,159},
	{ 18,142,235},
	{ 18,137,228},
	{ 34, 46,  4},
	{ 41, 89,139},
	{ 98, 90, 75},
	{ 34, 98,156},
	{ 98, 90, 60},
	{ 98, 94, 60},
	{ 43,110,156},
	{ 26,118,198},
	{ 30,112,190},
	{ 74, 88, 98},
	{ 25,122,196},
	{ 42,105,156},
	{ 50, 86,132},
	{ 20,130,212},
	{ 97, 94, 71},
	{ 36, 82,142},
	{ 69, 35, 15},
	{ 70,112,128},
	{ 66,126,132},
	{ 86, 28, 21},
	{ 74,102,124},
	{ 71, 97,116},
	{217,172, 12},
	{ 86, 36, 31},
	{ 34, 98,172},
	{ 42, 94,132},
	{ 87, 44, 33},
	{ 71, 51,  5},
	{ 82, 64,  9},
	{ 42, 78,108},
	{ 86, 88, 24},
	{ 50, 70,108},
	{ 50, 74,  4},
	{ 62,104,  4},
	{ 64, 65,  6},
	{ 50,155,  4},
	{ 49,115,  4},
	{ 27,213,  4},
	{ 38,184,  4},
	{ 61, 97,  4},
	{ 74, 72,  4},
	{ 34,192,  4},
	{ 59,124,  4},
	{ 33,203,  4},
	{ 50,105,  4},
	{ 98, 64, 59},
	{ 98,110,100},
	{ 56, 51,  4},
	{154,122,116},
	{194,154, 12},
	{230,174, 12},
	{158,132,140},
	{138, 90, 76},
	{118, 53, 44},
	{154, 74, 12},
	{146,132,134},
	{ 34,126,188},
	{ 82, 55,  7},
	{ 43,172,  4},
	{ 50, 97,  4},
	{ 58, 44,  4},
	{ 47,148,  5},
	{119,149, 97},
	{ 65, 80,  4},
	{ 33,178,  4},
	{ 63, 88,  4},
	{ 42, 58,  4},
	{ 74, 63,  4},
	{ 42,210,  4},
	{ 98,122,  4},
	{ 42,200,  8},
	{ 27,220,  4},
	{ 40,164,  4},
	{ 50,220, 16},
	{ 34,231,  4},
	{110,202,  4},
	{114,182,  4},
	{106,166, 92},
	{ 86,112, 52},
	{ 34,162, 76},
	{ 42,131,  4},
	{ 53,132,  4},
	{ 74, 96,  8},
	{ 98,198, 76},
	{ 98,110, 56},
	{ 58,146,  8},
	{ 60, 72,  6},
	{ 34,146,  4},
	{ 60,112,  5},
	{ 69, 58,  4},
	{ 82,122, 52},
	{ 74,126,  4},
	{ 41,156,  4},
	{ 50, 63,  4},
	{ 52,139,  4},
	{ 49, 83,  4},
	{162,116,116},
	{ 52,164, 15},
	{ 20,144,  4},
	{ 18,124,  4},
	{ 58, 58,  4},
	{ 58,162, 44},
	{ 24, 42,  4},
	{ 13, 70,  4},
	{ 26, 66,  4},
	{ 10, 88,  4},
	{ 12,103,  4},
	{ 26,110,  4},
	{183,191,180},
	{ 14,114,  4},
	{ 21, 51,  4},
	{ 90,118, 84},
	{114, 58, 56},
	{170,150,144},
	{162,139,140},
	{146,116,118},
	{131,100, 99},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255}
	};
static char logo_data[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,2,3,4,4,5,6,
	7,8,9,9,9,10,11,12,13,14,15,16,17,18,19,20,
	3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,20,21,22,23,24,25,25,26,27,27,27,27,28,29,30,
	29,28,28,29,29,28,29,28,31,28,32,33,33,33,34,35,
	64,24,16,21,20,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,36,0,20,21,37,
	27,26,38,34,34,39,34,29,31,32,28,29,37,29,30,28,
	32,32,30,29,32,32,40,32,30,32,32,28,29,30,31,32,
	120,122,74,121,64,74,4,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,17,41,28,42,23,
	24,35,26,26,32,31,30,28,28,29,29,32,32,32,30,30,
	30,30,30,43,44,45,46,30,32,47,47,48,49,49,49,44,
	32,32,47,57,74,121,64,13,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,50,41,29,29,31,34,51,
	34,31,33,32,32,30,32,49,44,44,49,48,30,30,30,49,
	40,49,52,53,52,54,55,44,43,40,44,53,49,49,30,49,
	30,44,47,40,52,105,74,121,73,50,76,4,0,0,0,4,
	18,18,112,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,26,28,29,42,35,56,35,57,
	31,30,30,30,30,30,49,47,49,30,49,40,30,43,47,43,
	30,40,58,59,60,61,59,52,43,44,49,49,48,44,44,62,
	48,49,44,30,40,30,44,95,174,175,102,176,37,28,28,29,
	28,28,37,37,28,33,74,177,3,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,63,37,41,39,64,56,63,53,32,
	30,40,52,49,43,49,47,40,43,44,47,46,65,52,49,43,
	44,52,66,60,67,68,69,52,49,49,62,49,49,44,44,43,
	43,58,62,43,62,52,44,32,53,95,16,73,74,33,28,28,
	28,29,29,30,28,28,28,29,28,29,93,20,0,36,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,18,28,26,23,35,26,53,28,30,49,
	49,43,32,49,32,49,30,49,49,40,60,70,71,71,44,43,
	30,44,72,67,40,72,54,62,43,43,43,52,52,49,49,47,
	43,44,32,49,54,66,80,47,49,30,152,95,16,73,102,33,
	28,30,28,30,30,49,40,49,29,30,30,29,145,93,3,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,20,50,73,74,75,
	18,2,0,0,0,76,37,27,77,42,42,32,30,49,44,49,
	58,55,78,40,49,30,49,43,49,78,79,69,59,68,44,30,
	43,47,72,60,80,70,54,49,49,62,44,49,62,47,62,43,
	62,40,49,82,79,71,178,59,43,43,49,53,155,114,119,110,
	53,30,49,43,49,32,32,30,32,148,30,30,32,32,86,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,31,37,37,37,37,37,29,
	29,28,37,25,6,29,39,23,42,33,30,32,30,32,30,49,
	49,65,81,44,49,43,43,48,47,62,61,72,67,55,82,44,
	47,43,83,68,71,71,59,62,47,49,40,62,49,49,30,43,
	32,49,44,54,85,78,80,151,81,49,40,40,49,51,74,50,
	174,155,48,32,91,52,40,44,40,40,30,47,30,30,28,0,
	0,0,0,0,
	0,0,0,0,0,0,0,2,37,41,37,37,28,28,29,28,
	37,28,28,29,31,77,64,63,28,30,30,49,49,40,43,40,
	44,84,85,45,40,47,44,43,43,52,69,61,70,59,62,32,
	40,49,32,84,58,47,62,49,44,44,44,47,43,43,47,49,
	44,43,78,90,70,97,83,179,124,44,49,40,48,48,148,15,
	111,50,148,44,43,30,44,49,43,43,44,43,49,49,155,0,
	0,0,0,0,
	0,0,0,0,0,0,0,37,37,41,37,29,37,29,29,40,
	49,32,48,86,74,87,51,30,28,30,30,49,32,43,49,62,
	49,62,55,60,58,30,43,43,49,30,58,88,65,53,49,62,
	44,48,58,48,49,62,49,49,49,49,44,40,30,43,49,62,
	49,62,54,68,60,178,151,85,84,30,43,43,32,40,49,62,
	114,107,13,96,49,62,49,49,49,49,43,40,32,43,180,0,
	0,0,0,0,
	0,0,0,0,0,0,89,41,37,29,28,37,29,49,32,30,
	30,49,51,22,14,57,53,49,49,43,52,62,44,40,62,30,
	43,48,82,90,80,43,44,32,44,52,49,40,49,62,49,49,
	32,43,48,49,44,49,49,44,47,52,44,91,44,49,92,30,
	40,84,68,115,49,84,83,47,44,44,49,40,49,44,43,49,
	47,155,5,107,105,49,49,44,44,52,44,91,47,49,0,0,
	0,0,0,0,
	0,0,0,0,0,0,37,41,37,37,29,29,32,29,49,62,
	44,44,93,94,95,30,30,43,47,96,97,32,48,44,30,62,
	43,49,44,59,60,44,32,44,43,30,48,32,44,30,43,92,
	62,43,43,43,32,49,40,44,52,43,62,49,43,44,30,62,
	43,47,67,58,43,48,32,44,43,30,43,32,44,96,60,80,
	62,43,99,21,114,113,32,44,44,47,62,49,43,12,0,0,
	0,0,0,0,
	0,0,0,0,0,20,37,28,41,29,28,30,30,49,58,49,
	47,98,18,99,44,43,58,48,32,59,88,44,49,49,58,32,
	32,44,44,84,65,43,43,49,30,44,40,43,49,43,62,49,
	49,43,49,32,44,43,62,48,30,43,49,49,49,49,62,40,
	40,43,43,49,49,43,44,48,32,43,40,43,53,70,72,43,
	49,62,53,99,111,99,58,48,30,43,49,49,49,106,0,0,
	0,0,0,0,
	0,0,0,0,0,100,37,37,28,29,30,32,49,49,30,44,
	101,94,11,62,49,49,43,49,58,54,70,65,43,49,49,48,
	44,48,32,52,49,49,49,40,62,49,48,30,43,49,32,44,
	62,32,44,62,32,49,43,40,91,44,44,49,43,49,49,49,
	52,43,30,44,49,49,49,32,62,48,48,40,59,70,96,44,
	62,30,44,62,15,21,9,40,91,44,62,49,44,3,0,0,
	0,0,0,0,
	0,0,0,0,0,102,41,29,28,28,29,32,32,49,62,101,
	103,98,53,30,44,58,62,92,43,104,90,72,52,43,52,44,
	40,43,44,49,44,43,52,62,49,49,40,48,49,48,44,49,
	40,52,49,30,62,58,62,92,43,44,49,44,44,43,52,62,
	40,43,44,49,44,43,52,62,49,49,30,54,68,83,44,48,
	32,52,49,30,52,12,94,181,43,44,49,44,43,0,0,0,
	0,0,0,0,
	0,0,0,0,0,37,37,37,28,30,30,40,43,30,105,17,
	106,57,43,62,40,49,58,43,44,52,88,70,81,40,30,44,
	47,49,43,44,52,49,43,40,49,48,32,49,44,32,48,40,
	44,40,43,62,40,49,62,43,44,49,43,32,43,49,30,44,
	44,49,43,44,44,49,43,40,49,48,67,85,81,30,48,40,
	44,40,43,62,32,155,15,13,108,49,43,40,43,0,0,0,
	0,0,0,0,
	0,0,0,0,0,37,28,29,28,28,30,49,49,52,13,107,
	108,49,43,32,44,44,48,58,92,43,52,109,70,80,62,49,
	49,49,49,49,49,43,43,44,43,30,44,43,49,43,32,49,
	40,49,48,32,44,44,43,58,92,43,52,62,32,30,43,49,
	49,49,49,49,49,43,43,44,43,49,115,81,40,43,32,49,
	40,49,43,40,44,44,117,21,101,44,52,44,30,0,0,0,
	0,0,0,0,
	0,0,0,0,3,37,37,28,28,30,30,49,52,110,18,14,
	40,43,40,49,30,40,48,43,40,52,62,40,81,65,40,49,
	49,47,43,52,44,47,62,43,43,43,43,43,62,40,49,43,
	32,43,40,49,32,49,43,43,49,52,62,30,43,48,30,49,
	49,47,43,52,44,47,62,43,43,43,43,44,62,40,49,40,
	32,43,40,49,30,40,43,114,17,102,62,30,48,0,0,0,
	0,0,0,0,
	0,0,0,0,89,28,37,37,28,30,30,40,51,22,111,108,
	43,43,40,49,44,44,43,62,52,43,43,43,52,47,52,43,
	44,58,53,44,43,48,32,49,32,49,43,49,32,44,43,44,
	43,43,40,49,43,44,43,43,58,43,43,43,43,43,52,43,
	52,58,47,44,43,48,32,30,32,49,43,49,32,44,43,44,
	43,43,47,44,44,44,43,118,22,112,155,43,49,0,0,0,
	0,0,0,0,
	0,0,0,0,18,41,37,29,28,30,30,84,13,93,95,49,
	49,49,40,49,43,30,44,43,49,49,44,62,52,44,43,40,
	49,30,49,30,43,40,43,43,43,49,43,62,62,43,32,49,
	49,49,40,49,43,30,44,44,49,49,44,62,52,43,43,32,
	49,30,49,30,43,40,43,43,43,49,43,62,62,43,40,49,
	49,84,71,115,44,30,44,43,108,22,15,84,44,0,0,0,
	0,0,0,0,
	0,0,0,0,19,37,28,37,30,32,62,105,112,5,52,62,
	40,30,104,84,40,62,49,49,58,62,44,49,44,49,47,62,
	49,62,44,44,49,44,47,49,44,43,92,52,52,62,44,62,
	40,30,44,48,32,62,49,49,52,62,43,49,62,49,47,62,
	49,62,44,44,49,44,43,49,44,44,92,58,52,62,44,62,
	67,85,59,84,30,62,49,49,84,93,94,95,148,0,0,0,
	0,0,0,0,
	0,0,0,0,19,37,28,37,30,30,113,114,112,95,48,43,
	43,91,115,69,65,62,44,58,40,43,43,44,44,58,43,44,
	44,49,49,49,92,48,49,49,44,48,48,44,48,49,48,43,
	43,92,40,43,62,62,43,58,40,43,43,44,47,91,43,44,
	44,49,49,49,92,48,49,49,43,48,48,44,48,49,48,118,
	151,54,53,58,54,97,44,58,32,117,112,98,44,4,0,0,
	0,0,0,0,
	0,0,0,0,19,37,37,29,28,30,63,22,93,58,62,43,
	62,40,52,116,79,66,84,43,62,32,62,43,32,30,49,49,
	43,49,52,49,32,49,49,43,43,43,49,44,49,52,62,43,
	92,40,62,43,40,43,43,48,62,32,62,43,40,30,49,49,
	43,40,52,49,32,49,49,47,43,43,49,44,49,52,62,84,
	90,81,83,70,71,97,48,43,62,47,98,50,105,112,0,0,
	0,0,0,0,
	0,0,0,0,19,37,29,28,30,53,110,93,117,49,43,49,
	49,49,92,49,84,71,71,118,44,58,49,48,44,44,44,44,
	49,44,40,44,52,47,52,43,43,44,62,40,52,48,43,30,
	48,49,62,49,43,62,44,49,43,58,49,48,44,47,62,44,
	49,44,43,44,52,44,52,43,43,44,62,32,58,48,43,49,
	67,68,90,59,82,62,44,49,43,58,9,50,99,106,0,0,
	0,0,0,0,
	0,0,0,0,20,37,29,37,28,34,119,114,113,49,49,62,
	43,62,30,43,43,91,54,68,84,40,40,49,43,52,49,43,
	44,49,43,44,40,48,44,30,49,30,49,43,48,40,49,92,
	40,92,30,40,43,62,43,62,44,40,40,49,43,52,49,43,
	44,49,43,44,40,48,43,30,49,30,49,43,49,40,49,62,
	148,60,81,52,47,62,43,62,44,32,44,114,103,13,0,0,
	0,0,0,0,
	0,0,0,0,4,37,37,28,29,74,14,95,49,48,40,40,
	44,49,43,52,43,49,62,78,43,43,43,62,32,49,40,40,
	47,44,44,44,52,44,49,52,43,43,44,49,49,43,49,40,
	47,49,43,52,43,30,44,52,49,43,43,62,32,30,43,40,
	47,47,44,47,58,44,49,52,43,43,52,49,40,43,49,32,
	44,47,44,52,43,30,44,52,40,44,44,182,50,114,0,0,
	0,0,0,0,
	0,0,0,0,3,29,28,28,120,121,102,113,32,30,43,49,
	30,43,43,44,43,52,49,43,62,58,40,40,47,40,47,48,
	49,40,43,52,62,43,62,49,40,43,43,40,32,32,43,49,
	30,43,43,44,47,58,48,40,62,58,40,32,44,40,47,49,
	48,40,43,52,62,43,62,49,43,43,43,49,40,32,43,40,
	30,44,43,44,47,58,49,40,92,62,49,47,15,17,20,0,
	0,0,0,0,
	0,0,0,0,4,37,41,28,42,16,6,47,43,49,30,43,
	43,49,30,43,30,49,92,44,40,48,43,40,49,44,52,43,
	47,40,47,44,40,49,40,44,40,49,43,40,43,49,32,43,
	43,49,30,48,30,49,91,52,40,48,43,40,49,47,44,43,
	44,40,47,44,40,49,40,47,40,49,43,40,43,49,30,43,
	43,49,30,48,30,40,92,52,32,43,44,57,174,22,19,0,
	0,0,0,0,
	0,0,0,0,0,28,28,31,122,87,86,30,49,48,44,30,
	32,44,44,43,91,44,49,43,62,92,52,62,47,43,49,44,
	62,92,92,58,62,92,62,62,43,52,32,40,49,43,43,30,
	40,44,62,43,91,43,49,43,62,92,52,62,47,43,40,44,
	44,92,92,58,62,92,43,92,43,52,32,40,49,43,44,30,
	32,44,62,43,97,104,66,70,115,58,44,62,152,5,103,0,
	0,0,0,0,
	0,0,0,0,0,37,28,27,35,24,43,44,52,40,62,91,
	67,30,40,49,32,43,62,44,43,49,53,44,62,43,40,62,
	43,48,43,92,43,43,48,40,40,48,44,52,44,49,44,44,
	91,32,40,49,40,43,62,44,43,49,52,44,62,43,40,62,
	43,48,43,92,43,43,48,40,30,48,43,52,43,49,44,62,
	58,32,47,78,115,68,69,54,70,148,53,44,84,114,94,0,
	0,0,0,0,
	0,0,0,0,0,28,28,42,25,95,57,30,49,43,54,85,
	54,84,44,62,62,91,49,44,52,52,62,30,43,40,62,49,
	43,43,58,43,52,62,92,58,91,62,43,40,49,48,49,40,
	43,44,43,43,58,92,49,44,62,52,62,30,43,49,62,49,
	43,43,58,43,58,62,91,91,91,92,43,40,40,48,49,40,
	43,44,183,59,70,72,49,78,68,67,62,30,148,99,21,2,
	0,0,0,0,
	0,0,0,0,0,28,29,23,123,26,30,47,43,49,78,71,
	71,60,124,60,78,49,62,43,44,49,62,62,52,62,52,52,
	52,49,52,52,92,91,62,44,48,30,49,52,47,49,44,92,
	43,40,32,43,43,49,62,47,44,49,62,62,58,62,52,52,
	52,49,52,52,92,92,62,44,48,30,49,52,47,49,44,92,
	43,32,96,54,115,71,65,70,151,69,62,62,58,182,175,20,
	0,0,0,0,
	0,0,0,0,0,37,29,125,26,40,49,40,30,49,44,44,
	58,81,72,66,52,52,62,44,43,52,44,40,40,49,49,49,
	44,58,52,44,49,49,43,43,44,91,44,49,30,49,44,40,
	44,52,62,49,49,52,62,44,43,53,44,40,32,49,49,49,
	44,58,52,44,49,49,43,43,44,91,44,49,30,49,44,43,
	47,58,62,124,60,151,90,54,109,52,44,40,32,148,114,20,
	0,0,0,0,
	0,0,0,0,0,37,42,35,34,32,28,48,43,48,43,49,
	43,49,53,52,91,49,32,48,49,44,44,44,44,44,44,62,
	43,49,40,49,43,44,44,43,48,32,40,44,47,52,11,82,
	48,49,49,62,92,49,32,48,49,44,62,44,44,44,44,62,
	43,49,40,49,43,44,105,43,48,40,40,44,47,48,43,49,
	48,49,49,116,72,65,96,47,40,44,44,52,52,44,101,184,
	0,0,0,0,
	0,0,0,0,0,29,27,125,33,40,49,43,49,30,43,47,
	44,43,62,43,49,44,62,44,44,43,52,44,126,127,127,128,
	14,52,52,52,44,44,49,44,91,52,44,44,49,95,128,114,
	44,43,62,43,49,44,62,44,44,43,52,44,43,40,30,49,
	40,52,52,52,44,84,127,114,58,52,44,44,49,32,43,47,
	44,43,62,44,44,52,58,44,44,44,52,43,43,49,95,18,
	0,0,0,0,
	0,0,0,0,0,37,129,39,33,28,32,40,32,43,62,43,
	40,49,49,43,52,52,62,62,44,44,49,49,126,128,127,128,
	110,40,44,40,49,32,44,52,48,30,130,74,32,11,128,110,
	32,49,49,43,52,52,62,62,44,52,49,49,43,44,44,58,
	43,40,11,130,49,53,128,110,43,30,48,49,32,44,44,62,
	32,49,49,43,52,52,62,62,44,44,49,49,43,44,108,119,
	0,0,0,0,
	0,0,0,0,0,131,39,26,42,30,30,49,43,43,32,49,
	52,43,48,43,43,32,40,43,49,43,44,49,132,127,127,127,
	15,62,92,47,44,43,44,30,44,58,132,133,44,6,127,134,
	58,40,48,43,44,32,43,43,49,48,62,49,48,52,52,43,
	43,62,15,126,44,48,84,47,62,58,49,49,43,49,40,49,
	52,43,49,44,43,32,43,43,49,43,62,49,48,52,152,5,
	0,0,0,0,
	0,0,0,0,0,17,23,39,28,28,32,43,32,40,48,43,
	30,49,49,43,48,92,44,62,43,43,43,52,135,128,127,127,
	110,44,47,136,130,40,100,136,99,96,137,136,130,49,132,138,
	130,126,6,48,49,116,132,139,138,43,47,140,141,25,126,121,
	43,62,126,136,132,53,132,160,49,30,121,185,126,95,48,43,
	30,49,49,43,49,62,44,52,84,52,43,148,52,49,44,114,
	0,0,0,0,
	0,0,0,0,0,111,26,26,29,32,30,49,43,52,49,30,
	43,49,43,58,44,48,49,49,40,62,52,49,100,127,128,142,
	83,40,86,128,132,30,134,128,134,100,127,127,137,30,132,128,
	127,127,137,58,44,136,128,128,127,143,44,127,128,127,127,128,
	118,145,128,128,128,145,128,132,49,11,127,128,128,128,47,30,
	43,49,43,58,44,48,49,44,66,116,52,49,44,44,84,174,
	0,0,0,0,
	0,0,0,0,0,94,39,39,28,29,40,32,96,59,82,44,
	43,47,43,32,92,62,43,62,43,49,40,44,96,127,144,128,
	32,49,108,128,132,43,14,127,133,95,128,127,137,32,132,128,
	128,128,127,120,116,127,127,128,128,139,40,127,128,127,127,127,
	16,155,128,127,128,152,128,132,43,126,127,127,127,128,95,44,
	47,43,43,40,92,92,43,92,71,118,40,62,49,49,47,93,
	0,0,0,0,
	0,0,0,0,0,103,42,27,29,30,30,30,116,70,81,47,
	40,49,32,52,43,30,44,49,44,44,52,47,100,128,145,127,
	105,43,47,146,132,47,78,126,15,78,136,147,11,49,132,126,
	77,87,128,108,148,128,64,100,149,136,53,150,127,138,110,142,
	114,43,147,136,14,44,146,132,44,126,132,145,118,127,117,40,
	40,49,32,52,43,30,44,47,60,183,53,44,40,43,113,155,
	0,0,0,0,
	0,0,0,0,0,18,39,42,37,28,32,49,67,151,151,71,
	69,97,52,43,49,62,49,49,49,49,47,43,121,136,49,136,
	99,58,49,133,126,30,43,126,134,32,132,133,32,40,132,133,
	44,49,128,82,152,127,49,49,6,137,47,100,136,43,49,126,
	110,58,133,132,43,32,15,132,49,132,110,44,32,136,186,62,
	44,43,52,48,49,62,49,49,55,115,53,43,30,43,43,105,
	0,0,0,0,
	0,0,0,0,0,94,23,27,28,30,30,30,53,97,118,65,
	59,97,48,49,44,49,49,44,47,52,44,52,153,136,135,142,
	126,48,40,133,132,43,44,154,114,44,132,133,49,52,132,131,
	32,43,127,152,96,128,49,44,11,139,44,7,136,49,62,132,
	15,40,133,132,32,43,133,132,44,126,131,32,49,174,82,49,
	32,43,48,49,44,49,49,44,84,97,52,58,44,186,159,95,
	0,0,0,0,
	0,0,0,0,0,18,23,27,29,37,32,30,43,30,43,62,
	62,43,43,43,32,49,32,44,52,43,62,49,128,128,128,128,
	137,49,47,15,126,43,32,140,14,30,132,133,43,30,126,15,
	62,40,128,155,145,127,132,126,137,136,62,95,136,44,30,135,
	15,49,14,126,43,48,133,140,43,154,15,40,44,30,43,92,
	62,43,43,43,32,49,32,44,44,43,62,49,171,166,187,8,
	0,0,0,0,
	0,0,0,0,0,18,35,42,28,28,32,49,30,43,62,49,
	49,62,30,32,44,43,62,48,49,43,49,145,127,95,52,156,
	128,52,43,134,132,40,44,132,133,44,132,110,49,43,135,133,
	49,62,127,145,96,128,114,15,133,100,49,157,136,49,44,132,
	133,43,114,132,49,43,14,132,30,140,16,43,49,43,44,49,
	49,62,30,32,44,43,62,43,30,43,49,49,120,170,170,94,
	0,0,0,0,
	0,0,0,0,0,94,125,42,28,30,30,30,49,49,30,44,
	62,40,44,62,40,49,43,40,58,44,62,11,127,47,49,113,
	127,100,30,133,132,49,49,132,114,49,126,16,43,49,132,110,
	62,32,128,84,51,127,43,40,117,158,62,95,139,49,49,132,
	133,48,159,126,49,49,134,132,62,132,110,30,43,130,86,44,
	62,32,62,62,32,49,43,40,91,44,62,49,43,188,170,94,
	0,0,0,0,
	0,0,0,0,0,103,34,31,28,28,28,49,32,49,44,49,
	32,52,49,30,62,58,44,92,43,52,110,142,127,132,52,126,
	128,142,7,133,140,43,52,126,134,49,154,110,49,48,126,134,
	32,52,127,51,82,127,44,92,11,139,49,11,139,43,52,126,
	16,43,15,132,44,43,114,126,49,132,133,48,49,137,9,48,
	32,52,49,30,62,58,62,92,43,52,189,145,44,40,190,191,
	0,0,0,0,
	0,0,0,0,0,39,35,29,28,28,30,49,43,30,48,40,
	44,40,43,62,40,49,62,43,44,49,133,128,127,137,30,136,
	128,128,110,114,137,74,99,137,150,44,132,136,11,40,135,150,
	44,57,128,74,148,127,16,95,150,136,44,138,136,148,47,126,
	150,47,15,137,11,44,150,126,47,132,136,133,138,128,6,43,
	44,32,43,62,40,49,44,145,192,192,170,193,194,40,195,191,
	0,0,0,0,
	0,0,0,0,0,39,125,27,29,29,30,40,78,115,118,96,
	49,49,48,32,44,44,43,91,62,43,133,127,128,136,43,136,
	127,127,14,159,127,128,127,128,128,96,144,128,127,11,128,127,
	96,132,127,136,96,128,127,128,127,126,47,127,128,143,96,127,
	127,86,133,127,127,34,128,128,11,144,128,127,127,127,95,30,
	40,49,48,30,52,44,196,197,172,198,199,200,200,156,44,94,
	0,0,0,0,
	0,0,0,0,0,42,24,42,28,30,49,96,70,85,54,71,
	118,43,40,49,49,49,48,43,49,52,133,128,127,136,30,137,
	128,127,14,108,127,127,127,127,127,51,105,128,127,95,128,128,
	57,132,128,136,32,136,127,128,127,73,44,127,128,73,96,127,
	128,152,155,127,127,35,127,128,100,11,128,142,128,139,47,43,
	40,40,53,173,201,188,52,202,203,204,204,203,203,205,145,206,
	0,0,0,0,
	0,0,0,0,0,39,125,27,28,29,32,80,70,69,115,72,
	124,43,32,49,47,44,43,62,52,43,6,126,126,130,52,144,
	126,140,99,44,56,126,123,158,132,47,40,95,144,95,126,140,
	57,160,132,130,43,52,144,135,87,43,43,126,126,6,82,126,
	140,78,47,95,130,63,132,132,86,207,170,208,130,82,44,43,
	161,201,209,165,166,172,196,210,166,187,211,166,200,187,82,212,
	0,0,0,0,
	0,0,0,0,0,41,125,39,29,30,30,118,85,71,60,68,
	67,48,40,49,44,30,44,43,49,48,44,44,52,43,43,40,
	49,30,49,30,44,32,40,43,43,49,43,62,62,43,32,40,
	49,49,40,49,43,30,44,43,49,49,44,62,52,43,43,32,
	49,30,49,30,43,33,213,214,145,190,166,201,62,215,216,209,
	173,166,167,161,172,166,217,218,172,166,162,201,172,190,82,219,
	0,0,0,0,
	0,0,0,0,0,42,125,23,28,32,52,52,67,60,72,115,
	53,30,44,48,30,62,49,49,58,62,44,49,62,40,47,44,
	49,62,44,44,49,44,43,49,44,44,92,52,62,62,44,62,
	40,30,44,48,30,92,49,49,58,62,44,49,62,40,47,62,
	49,62,44,82,163,171,172,200,192,186,220,171,194,171,197,172,
	161,170,200,173,201,166,193,189,201,166,190,173,201,189,47,15,
	0,0,0,0,
	0,0,0,0,0,29,39,23,30,30,40,43,48,49,52,44,
	43,92,40,43,62,43,62,52,40,43,43,44,47,91,43,44,
	44,49,49,49,92,48,49,49,43,48,48,44,48,49,48,43,
	43,92,40,44,62,43,44,58,40,43,43,44,47,91,43,44,
	44,49,145,193,187,47,189,216,221,47,187,200,187,222,170,166,
	165,223,166,187,162,166,166,157,224,166,172,86,47,91,58,131,
	0,0,0,0,
	0,0,0,0,0,21,39,39,31,28,30,43,32,44,62,43,
	92,32,62,43,40,43,48,48,62,32,62,43,40,30,49,49,
	43,40,53,49,32,49,49,47,43,48,49,44,49,52,62,48,
	92,40,44,43,32,43,43,40,62,32,62,43,40,30,49,49,
	43,43,209,172,200,176,49,43,156,44,165,166,172,194,190,172,
	170,188,172,172,224,170,172,173,96,172,166,173,40,30,174,94,
	2,0,0,0,
	0,0,0,0,0,21,39,39,32,32,49,30,52,49,43,30,
	48,49,92,49,43,62,52,49,43,58,49,49,44,44,62,44,
	49,44,43,44,52,44,52,43,43,44,58,32,58,43,43,30,
	48,49,92,49,43,62,44,49,43,58,49,49,44,47,62,44,
	49,43,187,166,166,210,52,44,86,192,157,170,166,209,173,166,
	166,224,167,172,173,192,169,40,43,210,166,216,44,44,9,22,
	4,0,0,0,
	0,0,0,0,0,4,42,23,27,37,30,49,49,49,49,62,
	44,62,30,43,40,62,43,62,44,40,49,49,43,52,49,43,
	44,49,43,44,40,48,43,30,49,30,49,43,49,40,49,92,
	43,92,30,43,43,62,43,62,44,40,49,49,43,44,49,43,
	44,47,187,172,166,167,86,188,193,172,161,201,166,167,215,193,
	166,171,224,194,96,58,88,109,52,176,217,86,43,44,6,22,
	75,0,0,0,
	0,0,0,0,0,3,27,23,26,33,30,32,47,43,78,60,
	65,47,43,52,43,30,44,52,49,44,43,62,32,30,43,40,
	44,47,44,47,58,44,49,52,43,43,44,49,49,43,49,32,
	47,49,43,52,43,49,44,52,49,44,43,62,32,49,40,40,
	47,44,216,172,166,166,224,187,166,166,168,161,166,166,168,173,
	165,120,43,52,40,96,70,68,70,54,80,62,32,47,15,17,
	5,0,0,0,
	0,0,0,0,0,0,27,42,34,27,30,82,59,96,62,54,
	85,55,47,52,43,58,48,40,62,62,40,32,44,40,47,49,
	49,40,43,52,62,43,62,49,43,43,43,49,40,32,43,49,
	30,43,43,44,47,52,49,40,62,62,120,161,162,145,47,49,
	49,40,222,172,166,166,201,173,166,166,173,120,201,223,86,47,
	30,43,47,44,43,62,148,116,69,90,65,30,44,155,13,119,
	96,0,0,0,
	0,0,0,0,0,0,122,42,125,26,32,124,60,43,30,47,
	115,151,32,43,30,49,92,52,32,48,43,40,49,47,44,44,
	44,40,47,44,40,49,40,47,40,49,43,40,43,49,32,43,
	43,49,30,48,30,49,91,52,40,163,164,165,166,167,96,43,
	44,40,52,209,172,166,172,224,172,170,163,40,53,32,32,43,
	43,49,30,43,32,55,183,58,52,70,67,40,30,8,22,114,
	44,3,0,0,
	0,0,0,0,0,0,17,31,23,34,27,54,81,40,43,49,
	45,85,84,43,92,44,49,43,92,92,52,62,47,43,40,43,
	44,92,92,58,62,92,43,92,43,148,40,32,49,43,43,49,
	32,44,44,43,91,96,168,169,82,167,170,171,170,172,173,44,
	44,92,92,52,210,172,200,217,165,192,40,32,49,43,40,49,
	32,62,44,43,91,59,178,68,71,178,84,62,44,114,111,105,
	40,225,0,0,
	0,0,0,0,0,0,2,27,24,26,47,88,59,96,116,65,
	68,54,40,48,32,43,62,44,47,49,53,62,62,43,40,62,
	43,48,43,92,43,43,43,40,30,43,44,52,43,49,44,44,
	58,40,47,86,165,209,200,193,96,167,200,190,162,163,47,44,
	43,49,43,62,47,86,163,120,49,48,43,52,43,49,44,44,
	58,49,40,49,40,44,116,81,65,118,44,44,84,15,94,9,
	52,9,0,0,
	0,0,0,0,0,0,0,29,23,35,42,47,66,71,79,85,
	69,78,43,43,58,92,49,44,62,52,62,30,43,49,62,49,
	43,43,58,43,58,62,92,91,58,118,44,40,40,48,49,40,
	196,194,218,172,200,164,221,165,96,187,166,193,86,49,186,176,
	47,44,58,43,58,58,91,91,58,92,43,40,43,48,49,40,
	43,53,43,43,58,91,49,43,58,58,43,49,95,119,106,148,
	43,58,0,0,
	0,0,0,0,0,0,0,37,27,125,24,31,47,82,80,116,
	52,32,40,43,43,49,62,44,44,49,62,62,58,62,52,52,
	52,49,52,52,92,91,58,159,167,170,216,82,53,189,192,171,
	199,172,176,221,200,193,157,53,47,173,166,166,162,82,223,186,
	52,49,52,52,92,92,62,44,48,30,49,52,47,49,44,92,
	48,40,32,43,48,49,62,43,44,49,62,108,5,103,9,52,
	52,49,0,0,
	0,0,0,0,0,0,0,50,27,39,122,33,32,43,43,43,
	47,58,62,49,49,148,62,44,43,53,44,40,32,49,49,49,
	44,58,52,52,215,226,193,224,172,166,200,209,161,172,172,173,
	166,200,162,173,200,172,159,52,43,53,209,170,193,209,215,49,
	43,58,52,44,49,49,43,43,44,91,44,49,30,49,62,40,
	52,58,62,49,49,52,62,52,43,53,44,105,119,5,152,49,
	44,62,243,0,
	0,0,0,0,0,0,0,3,37,42,35,34,47,49,49,40,
	48,49,49,62,92,49,32,48,49,62,44,52,44,44,44,62,
	43,49,40,53,167,200,200,173,221,200,200,167,169,172,172,165,
	193,200,201,218,170,200,227,53,48,44,44,156,156,52,44,62,
	43,49,40,49,43,44,44,47,48,40,32,44,47,48,43,49,
	43,49,49,62,92,49,30,48,49,44,52,105,75,98,44,62,
	40,40,180,0,
	0,0,0,0,0,0,0,0,28,42,24,87,57,30,40,44,
	44,43,62,43,49,44,62,44,44,43,52,43,43,40,30,49,
	40,52,52,44,216,200,200,187,168,200,200,201,53,221,200,193,
	171,166,172,192,209,166,199,82,44,43,52,44,43,49,49,49,
	40,52,52,52,44,52,49,44,58,58,44,44,49,32,43,47,
	44,43,44,43,49,44,52,44,44,44,155,114,119,108,30,49,
	40,62,177,0,
	0,0,0,0,0,0,0,0,37,37,42,123,25,47,43,43,
	30,49,49,40,52,52,62,62,44,44,49,49,6,96,44,52,
	43,40,47,40,215,172,200,172,224,167,170,224,32,224,166,170,
	173,172,166,188,169,190,162,96,44,52,49,49,43,44,44,58,
	43,40,44,40,49,40,44,44,48,49,48,49,32,44,44,43,
	32,49,49,43,52,52,62,62,44,52,174,119,174,52,44,58,
	43,49,8,0,
	0,0,0,0,0,0,0,0,64,29,42,125,73,57,30,43,
	44,43,48,43,44,32,43,43,40,43,62,47,210,156,52,43,
	43,62,92,43,53,193,200,166,165,169,120,49,44,120,167,166,
	209,216,167,86,43,47,43,43,49,48,62,49,43,52,52,43,
	43,62,62,40,44,48,44,30,44,52,49,49,43,49,40,49,
	52,43,48,43,43,32,43,43,49,108,13,119,96,44,52,43,
	47,62,181,0,
	0,0,0,0,0,0,0,0,94,41,28,145,16,51,53,43,
	30,49,49,48,49,62,44,11,162,210,145,82,190,47,49,43,
	43,44,32,49,49,228,200,200,193,189,43,44,40,40,168,201,
	221,229,145,43,49,62,44,62,47,43,43,52,52,49,49,47,
	43,44,32,49,30,40,40,49,49,30,48,44,40,49,48,43,
	30,49,49,43,49,92,44,62,40,174,21,8,58,49,49,43,
	43,44,5,0,
	0,0,0,0,0,0,0,0,19,28,37,27,102,16,152,30,
	48,49,43,58,44,48,188,193,165,166,164,222,223,47,62,47,
	62,32,49,30,49,229,170,200,166,194,49,43,43,49,49,31,
	47,49,43,52,44,48,49,49,49,62,52,49,44,47,62,43,
	62,40,49,49,49,30,49,43,49,44,49,43,43,49,49,30,
	43,49,43,58,44,48,49,49,155,119,22,152,44,47,62,44,
	62,32,180,0,
	0,0,0,0,0,0,0,0,0,37,28,37,63,73,102,52,
	43,43,53,49,92,62,216,172,171,193,172,223,165,120,222,221,
	224,47,44,43,49,47,221,200,166,216,47,49,49,32,49,44,
	44,47,43,32,92,62,43,62,47,49,40,62,49,49,30,43,
	32,49,44,43,49,43,43,43,43,49,43,49,49,32,49,44,
	47,44,43,32,92,62,43,62,9,112,7,62,49,49,30,40,
	40,49,243,0,
	0,0,0,0,0,0,0,0,0,37,37,37,42,102,22,95,
	31,86,209,190,188,31,171,200,221,209,199,221,165,193,162,172,
	172,192,49,49,40,47,194,167,221,168,47,32,43,48,49,40,
	40,49,40,52,43,30,44,49,44,44,47,44,43,43,47,49,
	52,43,49,30,40,43,44,43,44,44,49,32,43,48,48,40,
	40,49,32,52,43,30,44,6,17,93,96,113,43,43,47,49,
	44,43,0,0,
	0,0,0,0,0,0,0,0,0,16,28,29,37,32,73,207,
	193,209,210,172,166,210,229,201,170,224,165,221,165,200,187,223,
	166,223,49,49,43,32,53,218,47,30,43,43,40,40,49,62,
	44,48,58,43,49,62,49,49,49,49,44,40,30,43,49,62,
	49,58,49,49,44,30,43,43,49,30,43,43,40,32,49,62,
	44,48,58,43,49,62,145,14,14,152,43,43,49,43,49,62,
	49,98,0,0,
	0,0,0,0,0,0,0,0,0,111,41,37,41,47,51,230,
	200,167,192,166,166,172,163,86,192,96,96,187,215,172,172,161,
	166,209,40,49,32,43,44,30,52,44,49,40,49,44,49,49,
	32,43,48,49,44,49,49,44,44,52,44,91,44,49,92,30,
	43,43,40,49,30,43,44,30,44,44,49,40,49,62,49,49,
	32,43,48,49,62,113,9,94,174,58,44,58,47,49,92,30,
	43,4,0,0,
	0,0,0,0,0,0,0,0,0,76,231,232,233,42,32,226,
	166,166,168,187,166,200,209,44,52,43,196,190,47,210,172,171,
	209,218,43,43,43,48,40,44,43,30,48,32,44,30,43,92,
	62,40,43,43,32,49,32,44,52,43,62,49,43,44,30,62,
	48,49,43,47,43,48,32,44,43,30,48,32,44,30,43,92,
	62,40,43,43,40,105,17,13,152,47,62,49,43,44,30,62,
	106,0,0,0,
	0,0,0,0,0,0,0,0,0,1,231,234,235,235,145,216,
	166,200,201,188,166,166,201,47,30,43,215,223,49,53,163,176,
	47,43,44,49,49,43,43,48,30,44,40,43,49,43,44,49,
	49,62,49,32,44,43,62,48,49,40,49,49,49,49,62,32,
	32,43,43,49,49,43,44,48,32,44,40,43,49,43,62,49,
	49,62,30,32,117,13,13,108,49,43,49,49,49,49,62,7,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,2,231,232,235,228,236,173,
	170,200,166,159,170,166,167,47,58,44,168,224,43,49,49,49,
	52,48,32,44,49,49,49,32,62,49,48,30,43,49,30,44,
	62,32,44,62,32,49,43,40,91,44,62,49,40,49,49,48,
	52,43,30,44,49,49,49,32,62,49,48,30,43,49,30,44,
	62,40,44,182,143,119,108,40,58,44,62,49,43,82,180,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,237,231,231,235,228,227,195,
	193,200,166,230,164,166,223,91,44,44,53,44,44,43,52,44,
	40,43,44,49,44,43,52,58,49,49,40,48,49,48,52,49,
	32,52,49,30,62,58,44,92,43,44,49,44,44,43,52,44,
	40,43,44,49,44,43,58,62,49,49,40,48,49,48,44,48,
	32,52,152,22,13,9,62,92,43,44,49,43,181,1,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,2,232,231,232,238,227,129,
	224,172,200,202,219,167,157,40,44,49,43,40,43,49,30,44,
	47,49,43,44,52,49,48,40,49,48,40,49,44,30,48,40,
	62,40,43,62,40,49,62,43,44,49,43,40,43,40,30,44,
	47,49,43,44,44,49,43,40,49,48,32,49,44,32,48,40,
	44,47,101,18,114,47,62,48,43,117,180,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,237,232,239,232,238,227,195,
	30,161,223,226,191,213,117,91,92,43,52,44,32,49,43,49,
	49,49,49,49,49,43,43,44,43,30,44,43,49,43,32,49,
	32,49,43,32,44,44,43,58,62,43,52,44,32,49,62,49,
	49,49,49,49,49,43,43,44,43,49,44,43,49,43,32,49,
	95,110,17,114,58,43,82,225,36,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,206,234,232,231,238,227,53,
	30,49,40,53,174,17,13,105,49,52,62,49,40,49,30,49,
	49,47,43,52,47,44,44,43,43,43,43,43,62,40,49,43,
	32,43,40,49,49,49,43,43,49,52,62,49,40,49,30,49,
	49,47,43,52,47,43,44,43,40,43,43,43,62,40,49,11,
	18,18,8,53,105,180,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,240,234,234,231,235,161,49,
	40,43,49,49,44,9,5,13,108,53,40,43,43,43,52,43,
	44,58,47,44,43,48,32,49,32,49,43,49,40,44,43,44,
	43,43,40,49,47,44,43,62,58,43,40,43,43,48,52,43,
	44,58,47,44,43,43,40,30,40,49,40,49,40,96,9,76,
	184,5,117,3,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,37,231,239,231,42,28,30,
	40,49,40,49,43,53,108,13,107,98,58,62,52,43,43,40,
	49,30,49,30,43,40,43,43,43,49,43,62,62,43,32,49,
	49,49,40,49,43,30,44,44,49,49,44,62,52,43,43,40,
	49,30,49,30,43,40,43,43,43,49,43,62,182,244,20,103,
	101,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,18,37,28,28,32,30,49,44,
	30,30,44,48,32,62,49,155,17,184,106,155,62,40,47,62,
	49,62,44,44,49,44,43,49,47,43,92,58,52,62,44,62,
	49,30,44,48,30,62,49,49,52,92,44,49,62,40,47,62,
	49,62,44,44,49,44,43,49,44,44,116,9,18,76,4,4,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,176,37,37,29,29,30,49,52,
	43,92,40,43,62,43,62,58,51,9,103,184,15,84,43,44,
	44,49,49,49,62,48,49,49,44,48,48,44,48,49,48,44,
	43,92,40,44,62,43,44,58,32,43,43,44,47,91,43,44,
	44,49,49,49,62,48,49,49,43,96,17,21,20,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,2,37,28,37,28,28,30,47,49,
	62,49,44,43,40,43,43,48,62,53,241,22,103,114,9,113,
	43,40,52,49,40,49,49,43,43,43,49,44,49,52,62,48,
	92,32,52,43,40,43,48,48,62,32,62,43,40,30,49,40,
	43,40,53,49,40,49,49,108,114,184,103,20,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,17,37,28,28,28,32,40,49,30,
	49,49,92,49,43,62,44,49,40,91,49,105,5,112,112,5,
	9,108,40,44,52,47,52,43,43,44,62,32,58,43,40,40,
	48,49,92,49,43,62,44,49,43,58,49,49,44,47,62,44,
	49,44,43,44,52,108,101,17,103,89,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,41,37,41,28,28,28,30,49,62,
	48,62,30,40,43,44,43,92,44,32,49,49,44,117,15,21,
	112,5,105,84,47,48,44,30,49,30,49,43,49,40,49,62,
	48,62,30,40,43,62,43,92,44,40,40,49,43,52,49,48,
	44,49,43,82,114,22,94,76,76,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,37,37,28,28,29,30,30,49,30,
	43,49,43,52,43,32,52,44,49,43,40,52,30,32,113,6,
	114,18,184,114,8,117,47,62,43,43,52,49,49,43,49,40,
	44,49,48,52,43,30,43,52,49,43,43,62,32,49,47,43,
	105,131,15,111,112,13,50,4,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,237,28,41,29,28,29,30,49,49,
	30,43,43,44,43,52,7,106,242,243,3,0,3,180,244,95,
	57,6,110,103,21,103,98,99,108,52,43,47,40,32,43,40,
	30,43,44,52,47,58,40,40,92,62,43,40,96,152,15,93,
	22,103,89,184,245,9,242,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,106,77,28,30,30,28,49,
	49,145,7,111,4,0,0,0,0,0,0,0,0,0,0,0,
	3,13,53,95,87,134,94,94,50,143,119,174,174,155,95,96,
	155,43,113,44,43,148,83,108,73,95,110,13,14,18,18,17,
	17,119,99,174,47,13,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,4,89,177,180,177,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,180,106,105,100,8,99,119,111,22,22,112,17,13,
	119,245,5,13,5,93,112,111,240,50,175,111,112,94,131,174,
	174,116,97,58,98,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,4,111,114,95,6,8,114,14,13,103,
	112,111,21,103,17,112,19,89,94,119,22,15,8,95,57,58,
	43,40,48,116,36,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,3,89,15,99,113,95,
	117,105,105,6,9,12,15,98,105,108,84,47,43,49,62,49,
	43,43,10,1,36,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,
	180,99,99,155,49,30,44,49,43,49,148,43,52,52,44,44,
	116,111,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,180,243,180,244,98,9,7,180,180,1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0
	};
