//[*]--------------------------------------------------------------------------------------------------------------[*]
//	ENGLISH FONT
//[*]--------------------------------------------------------------------------------------------------------------[*]
const unsigned char ENGFONT[][16] =
{
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,126,129,165,129,129,129,189,153,129,126,  0,  0,  0,  0},
	{  0,  0,126,255,219,255,255,195,231,231,255,126,  0,  0,  0,  0},
	{  0,  0,  0, 54,127,127,127,127, 62, 28,  8,  0,  0,  0,  0,  0},
	{  0,  0,  0,  8, 28, 62,127, 62, 28,  8,  0,  0,  0,  0,  0,  0},
	{  0,  0, 24, 60, 60,231,231,231, 24, 24, 60,  0,  0,  0,  0,  0},
	{  0,  0, 24, 60,126,255,255,126, 24, 24, 60,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0, 24, 60, 60, 24,  0,  0,  0,  0,  0,  0,  0,  0},
	{127,127,127,127, 99, 65, 65, 65, 99,127,127,127,127,127,127,127},
	{  0,  0,  0,  0, 28, 34, 34, 34, 28,  0,  0,  0,  0,  0,  0,  0},
	{127,127,127,127, 99, 93, 93, 93, 99,127,127,127,127,127,127,127},
	{  0,  0,  8, 28, 42,  8, 28, 34, 65, 65, 34, 28,  0,  0,  0,  0},
	{  0,  0, 28, 34, 65, 65, 34, 28,  8,127,  8,  8,  0,  0,  0,  0},
	{  0,  0, 24, 20, 18, 18, 18, 16, 48,112,112, 32,  0,  0,  0,  0},
	{  0,  0, 31, 17, 31, 17, 17, 17, 19, 55,119,114, 32,  0,  0,  0},
	{  0,  0, 24, 24,219, 60,231, 60,219, 24, 24,  0,  0,  0,  0,  0},
	{  0,  0, 32, 48, 56, 60, 62, 60, 56, 48, 32,  0,  0,  0,  0,  0},
	{  0,  0,  2,  6, 14, 30, 62, 30, 14,  6,  2,  0,  0,  0,  0,  0},
	{  0,  0, 24, 60,126, 24, 24, 24,126, 60, 24,  0,  0,  0,  0,  0},
	{  0,  0, 51, 51, 51, 51, 51, 51,  0,  0, 51,  0,  0,  0,  0,  0},
	{  0,  0,127,219,219,219,123, 27, 27, 27, 27,  0,  0,  0,  0,  0},
	{  0, 62, 99, 48, 28, 54, 99, 99, 54, 28,  6, 99, 62,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,126,126,126,  0,  0,  0,  0},
	{  0,  0, 24, 60,126, 24, 24, 24,126, 60, 24,255,  0,  0,  0,  0},
	{  0,  0, 24, 60,126, 24, 24, 24, 24, 24, 24,  0,  0,  0,  0,  0},
	{  0,  0, 24, 24, 24, 24, 24, 24,126, 60, 24,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0, 12,  6,127,  6, 12,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0, 24, 48,127, 48, 24,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,128,128,128,128,128,128,255,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0, 36, 66,255, 66, 36,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  8,  8, 28, 28, 62, 62,127,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,127, 62, 62, 28, 28,  8,  8,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0, 24, 60, 60, 60, 24, 24, 24,  0, 24, 24,  0,  0,  0,  0},
	{  0,102,102,102, 36,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,108,108,254,108,108,108,254,108,108,  0,  0,  0,  0},
	{ 24, 24,124,198,194,192,124,  6,  6,134,198,124, 24, 24,  0,  0},
	{  0,  0,  0,  0,194,198, 12, 24, 48, 96,198,134,  0,  0,  0,  0},
	{  0,  0, 56,108,108, 56,118,220,204,204,204,118,  0,  0,  0,  0},
	{  0, 48, 48, 48, 96,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0, 12, 24, 48, 48, 48, 48, 48, 48, 24, 12,  0,  0,  0,  0},
	{  0,  0, 48, 24, 12, 12, 12, 12, 12, 12, 24, 48,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,102, 60,255, 60,102,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0, 24, 24,126, 24, 24,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 24, 24, 48,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,  0,254,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 24,  0,  0,  0,  0},
	{  0,  0,  0,  0,  2,  6, 12, 24, 48, 96,192,128,  0,  0,  0,  0},
	{  0,  0, 56,108,198,198,214,214,198,198,108, 56,  0,  0,  0,  0},
	{  0,  0, 24, 56,120, 24, 24, 24, 24, 24, 24,126,  0,  0,  0,  0},
	{  0,  0,124,198,  6, 12, 24, 48, 96,192,198,254,  0,  0,  0,  0},
	{  0,  0,124,198,  6,  6, 60,  6,  6,  6,198,124,  0,  0,  0,  0},
	{  0,  0, 12, 28, 60,108,204,254, 12, 12, 12, 30,  0,  0,  0,  0},
	{  0,  0,254,192,192,192,252,  6,  6,  6,198,124,  0,  0,  0,  0},
	{  0,  0, 56, 96,192,192,252,198,198,198,198,124,  0,  0,  0,  0},
	{  0,  0,254,198,  6,  6, 12, 24, 48, 48, 48, 48,  0,  0,  0,  0},
	{  0,  0,124,198,198,198,124,198,198,198,198,124,  0,  0,  0,  0},
	{  0,  0,124,198,198,198,126,  6,  6,  6, 12,120,  0,  0,  0,  0},
	{  0,  0,  0,  0, 24, 24,  0,  0,  0, 24, 24,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0, 24, 24,  0,  0,  0, 24, 24, 48,  0,  0,  0,  0},
	{  0,  0,  0,  6, 12, 24, 48, 96, 48, 24, 12,  6,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,126,  0,  0,126,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0, 96, 48, 24, 12,  6, 12, 24, 48, 96,  0,  0,  0,  0},
	{  0,  0,124,198,198, 12, 24, 24, 24,  0, 24, 24,  0,  0,  0,  0},
	{  0,  0,  0,124,198,198,222,222,222,220,192,124,  0,  0,  0,  0},
	{  0,  0, 16, 56,108,198,198,254,198,198,198,198,  0,  0,  0,  0},
	{  0,  0,252,102,102,102,124,102,102,102,102,252,  0,  0,  0,  0},
	{  0,  0, 60,102,194,192,192,192,192,194,102, 60,  0,  0,  0,  0},
	{  0,  0,248,108,102,102,102,102,102,102,108,248,  0,  0,  0,  0},
	{  0,  0,254,102, 98,104,120,104, 96, 98,102,254,  0,  0,  0,  0},
	{  0,  0,254,102, 98,104,120,104, 96, 96, 96,240,  0,  0,  0,  0},
	{  0,  0, 60,102,194,192,192,222,198,198,102, 58,  0,  0,  0,  0},
	{  0,  0,198,198,198,198,254,198,198,198,198,198,  0,  0,  0,  0},
	{  0,  0, 60, 24, 24, 24, 24, 24, 24, 24, 24, 60,  0,  0,  0,  0},
	{  0,  0, 30, 12, 12, 12, 12, 12,204,204,204,120,  0,  0,  0,  0},
	{  0,  0,230,102,102,108,120,120,108,102,102,230,  0,  0,  0,  0},
	{  0,  0,240, 96, 96, 96, 96, 96, 96, 98,102,254,  0,  0,  0,  0},
	{  0,  0,198,238,254,254,214,198,198,198,198,198,  0,  0,  0,  0},
	{  0,  0,198,230,246,254,222,206,198,198,198,198,  0,  0,  0,  0},
	{  0,  0,124,198,198,198,198,198,198,198,198,124,  0,  0,  0,  0},
	{  0,  0,252,102,102,102,124, 96, 96, 96, 96,240,  0,  0,  0,  0},
	{  0,  0,124,198,198,198,198,198,198,214,222,124, 12, 14,  0,  0},
	{  0,  0,252,102,102,102,124,108,102,102,102,230,  0,  0,  0,  0},
	{  0,  0,124,198,198, 96, 56, 12,  6,198,198,124,  0,  0,  0,  0},
	{  0,  0,126,126, 90, 24, 24, 24, 24, 24, 24, 60,  0,  0,  0,  0},
	{  0,  0,198,198,198,198,198,198,198,198,198,124,  0,  0,  0,  0},
	{  0,  0,198,198,198,198,198,198,198,108, 56, 16,  0,  0,  0,  0},
	{  0,  0,198,198,198,198,214,214,214,254,238,108,  0,  0,  0,  0},
	{  0,  0,198,198,108,124, 56, 56,124,108,198,198,  0,  0,  0,  0},
	{  0,  0,102,102,102,102, 60, 24, 24, 24, 24, 60,  0,  0,  0,  0},
	{  0,  0,254,198,134, 12, 24, 48, 96,194,198,254,  0,  0,  0,  0},
	{  0,  0, 60, 48, 48, 48, 48, 48, 48, 48, 48, 60,  0,  0,  0,  0},
	{  0,  0,  0,128,192,224,112, 56, 28, 14,  6,  2,  0,  0,  0,  0},
	{  0,  0, 60, 12, 12, 12, 12, 12, 12, 12, 12, 60,  0,  0,  0,  0},
	{ 16, 56,108,198,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,  0,  0},
	{ 48, 48, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,120, 12,124,204,204,204,118,  0,  0,  0,  0},
	{  0,  0,224, 96, 96,120,108,102,102,102,102,124,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,124,198,192,192,192,198,124,  0,  0,  0,  0},
	{  0,  0, 28, 12, 12, 60,108,204,204,204,204,118,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,124,198,254,192,192,198,124,  0,  0,  0,  0},
	{  0,  0, 56,108,100, 96,240, 96, 96, 96, 96,240,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,118,204,204,204,204,204,124, 12,204,120,  0},
	{  0,  0,224, 96, 96,108,118,102,102,102,102,230,  0,  0,  0,  0},
	{  0,  0, 24, 24,  0, 56, 24, 24, 24, 24, 24, 60,  0,  0,  0,  0},
	{  0,  0,  6,  6,  0, 14,  6,  6,  6,  6,  6,  6,102,102, 60,  0},
	{  0,  0,224, 96, 96,102,108,120,120,108,102,230,  0,  0,  0,  0},
	{  0,  0, 56, 24, 24, 24, 24, 24, 24, 24, 24, 60,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,236,254,214,214,214,214,198,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,220,102,102,102,102,102,102,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,124,198,198,198,198,198,124,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,220,102,102,102,102,102,124, 96, 96,240,  0},
	{  0,  0,  0,  0,  0,118,204,204,204,204,204,124, 12, 12, 30,  0},
	{  0,  0,  0,  0,  0,220,118,102, 96, 96, 96,240,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,124,198, 96, 56, 12,198,124,  0,  0,  0,  0},
	{  0,  0, 16, 48, 48,252, 48, 48, 48, 48, 54, 28,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,204,204,204,204,204,204,118,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,102,102,102,102,102, 60, 24,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,198,198,214,214,214,254,108,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,198,108, 56, 56, 56,108,198,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,198,198,198,198,198,198,126,  6, 12,248,  0},
	{  0,  0,  0,  0,  0,254,204, 24, 48, 96,198,254,  0,  0,  0,  0},
	{  0,  0, 14, 24, 24, 24,112, 24, 24, 24, 24, 14,  0,  0,  0,  0},
	{  0,  0, 24, 24, 24, 24,  0, 24, 24, 24, 24, 24,  0,  0,  0,  0},
	{  0,  0,112, 24, 24, 24, 14, 24, 24, 24, 24,112,  0,  0,  0,  0},
	{  0,  0,118,220,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0, 16, 56,108,198,198,198,254,  0,  0,  0,  0,  0},
	{  0,  0, 62, 99,192,192,192,192,193, 99, 62, 24, 12,  4, 28,  0},
	{  0,  0,  0,102,102,  0,102,102,102,102,102, 59,  0,  0,  0,  0},
	{  0,  0,  6, 12, 24,  0, 62, 99,127, 96, 99, 62,  0,  0,  0,  0},
	{  0,  0,  8, 28, 54,  0, 60,  6, 62,102,102, 59,  0,  0,  0,  0},
	{  0,  0,  0,102,102,  0, 60,  6, 62,102,102, 59,  0,  0,  0,  0},
	{  0,  0, 48, 24, 12,  0, 60,  6, 62,102,102, 59,  0,  0,  0,  0},
	{  0,  0, 28, 54, 28,  0, 60,  6, 62,102,102, 59,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0, 60,102, 96,102, 60, 12,  6, 60,  0,  0},
	{  0,  0,  8, 28, 54,  0, 62, 99,127, 96, 99, 62,  0,  0,  0,  0},
	{  0,  0,  0,102,102,  0, 62, 99,127, 96, 99, 62,  0,  0,  0,  0},
	{  0,  0, 48, 24, 12,  0, 62, 99,127, 96, 99, 62,  0,  0,  0,  0},
	{  0,  0,  0,102,102,  0, 56, 24, 24, 24, 24, 60,  0,  0,  0,  0},
	{  0,  0, 24, 60,102,  0, 56, 24, 24, 24, 24, 60,  0,  0,  0,  0},
	{  0,  0, 48, 24, 12,  0, 28, 12, 12, 12, 12, 30,  0,  0,  0,  0},
	{  0, 99, 99,  0,  8, 28, 54, 99, 99,127, 99, 99,  0,  0,  0,  0},
	{  0, 28, 54, 28,  8, 28, 54, 99, 99,127, 99, 99,  0,  0,  0,  0},
	{  0, 12, 24, 48,  0,127, 51, 48, 62, 48, 51,127,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,110,187, 27,255,216,217,118,  0,  0,  0,  0},
	{  0,  0,  0, 63,108,204,204,255,204,204,204,207,  0,  0,  0,  0},
	{  0,  0,  8, 28, 54,  0, 62, 99, 99, 99, 99, 62,  0,  0,  0,  0},
	{  0,  0,  0, 99, 99,  0, 62, 99, 99, 99, 99, 62,  0,  0,  0,  0},
	{  0,  0, 48, 24, 12,  0, 62, 99, 99, 99, 99, 62,  0,  0,  0,  0},
	{  0,  0, 48,120,204,  0,204,204,204,204,204,118,  0,  0,  0,  0},
	{  0,  0, 96, 48, 24,  0,204,204,204,204,204,118,  0,  0,  0,  0},
	{  0,  0,  0,198,198,  0,198,198,198,198,126,  6, 12,120,  0,  0},
	{  0,198,198,  0, 56,108,198,198,198,198,108, 56,  0,  0,  0,  0},
	{  0,  0,198,198,  0,198,198,198,198,198,198,124,  0,  0,  0,  0},
	{  0,  0, 24, 24,126,218,216,216,218,126, 24, 24,  0,  0,  0,  0},
	{  0,  0, 56,108,100, 96,248, 96, 96, 96,230,252,  0,  0,  0,  0},
	{  0,  0,195,102, 60, 24,126, 24,126, 24, 24,  0,  0,  0,  0,  0},
	{  0,  0,252,102,102,124, 96,100,111,100,100,247,  0,  0,  0,  0},
	{  0,  0, 14, 26, 24, 24, 24,126, 24, 24, 24, 24, 88,112,  0,  0},
	{  0,  0, 24, 48, 96,  0,120, 12,124,204,204,118,  0,  0,  0,  0},
	{  0,  0, 12, 24, 48,  0, 56, 24, 24, 24, 24, 60,  0,  0,  0,  0},
	{  0,  0, 24, 48, 96,  0,124,198,198,198,198,124,  0,  0,  0,  0},
	{  0,  0, 24, 48, 96,  0,204,204,204,204,204,118,  0,  0,  0,  0},
	{  0,  0,  0,118,220,  0,220,102,102,102,102,102,  0,  0,  0,  0},
	{  0,118,220,  0,198,230,246,254,222,206,198,198,  0,  0,  0,  0},
	{  0,  0,  0,112,144,144,120,  0,248,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0, 96,144,144, 96,  0,240,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0, 48, 48,  0, 48, 48, 96,198,198,124,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,254,192,192,192,192,192,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,127,  3,  3,  3,  3,  3,  0,  0,  0,  0},
	{  0,  0, 64, 64, 64, 68, 72, 16, 44, 82,130,  4,  8, 30,  0,  0},
	{  0,  0, 64, 64, 64, 68, 72, 16, 36, 76,148, 36, 62,  4,  0,  0},
	{  0,  0,  0, 24, 24,  0, 24, 24, 60, 60, 60, 24,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0, 51,102,204,102, 51,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,204,102, 51,102,204,  0,  0,  0,  0,  0,  0},
	{ 17, 68, 17, 68, 17, 68, 17, 68, 17, 68, 17, 68, 17, 68, 17, 68},
	{ 42, 85, 42, 85, 42, 85, 42, 85, 42, 85, 42, 85, 42, 85, 42, 85},
	{ 85,127, 42,127, 85,127, 42,127, 85,127, 42,127, 85,127, 42,127},
	{ 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24},
	{ 24, 24, 24, 24, 24, 24, 24,248, 24, 24, 24, 24, 24, 24, 24, 24},
	{ 24, 24, 24, 24, 24, 24,248, 24,248, 24, 24, 24, 24, 24, 24, 24},
	{ 54, 54, 54, 54, 54, 54, 54,246, 54, 54, 54, 54, 54, 54, 54, 54},
	{  0,  0,  0,  0,  0,  0,  0,254, 54, 54, 54, 54, 54, 54, 54, 54},
	{  0,  0,  0,  0,  0,  0,248, 24,248, 24, 24, 24, 24, 24, 24, 24},
	{ 54, 54, 54, 54, 54, 54,246, 22,246, 54, 54, 54, 54, 54, 54, 54},
	{ 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54},
	{  0,  0,  0,  0,  0,  0,254,  6,246, 54, 54, 54, 54, 54, 54, 54},
	{ 54, 54, 54, 54, 54, 54,246,  6,254,  0,  0,  0,  0,  0,  0,  0},
	{ 54, 54, 54, 54, 54, 54, 54,254,  0,  0,  0,  0,  0,  0,  0,  0},
	{ 24, 24, 24, 24, 24, 24,248, 24,248,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,  0,248, 24, 24, 24, 24, 24, 24, 24, 24},
	{ 24, 24, 24, 24, 24, 24, 24, 31,  0,  0,  0,  0,  0,  0,  0,  0},
	{ 24, 24, 24, 24, 24, 24, 24,255,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,  0,255, 24, 24, 24, 24, 24, 24, 24, 24},
	{ 24, 24, 24, 24, 24, 24, 24, 31, 24, 24, 24, 24, 24, 24, 24, 24},
	{  0,  0,  0,  0,  0,  0,  0,255,  0,  0,  0,  0,  0,  0,  0,  0},
	{ 24, 24, 24, 24, 24, 24, 24,255, 24, 24, 24, 24, 24, 24, 24, 24},
	{ 24, 24, 24, 24, 24, 24, 31, 24, 31, 24, 24, 24, 24, 24, 24, 24},
	{ 54, 54, 54, 54, 54, 54, 54, 55, 54, 54, 54, 54, 54, 54, 54, 54},
	{ 54, 54, 54, 54, 54, 54, 55, 48, 63,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0, 63, 48, 55, 54, 54, 54, 54, 54, 54, 54},
	{ 54, 54, 54, 54, 54, 54,247,  0,255,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,255,  0,247, 54, 54, 54, 54, 54, 54, 54},
	{ 54, 54, 54, 54, 54, 54, 55, 48, 55, 54, 54, 54, 54, 54, 54, 54},
	{  0,  0,  0,  0,  0,  0,255,  0,255,  0,  0,  0,  0,  0,  0,  0},
	{ 54, 54, 54, 54, 54, 54,247,  0,247, 54, 54, 54, 54, 54, 54, 54},
	{ 24, 24, 24, 24, 24, 24,255,  0,255,  0,  0,  0,  0,  0,  0,  0},
	{ 54, 54, 54, 54, 54, 54, 54,255,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,255,  0,255, 24, 24, 24, 24, 24, 24, 24},
	{  0,  0,  0,  0,  0,  0,  0,255, 54, 54, 54, 54, 54, 54, 54, 54},
	{ 54, 54, 54, 54, 54, 54, 54, 63,  0,  0,  0,  0,  0,  0,  0,  0},
	{ 24, 24, 24, 24, 24, 24, 31, 24, 31,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0, 31, 24, 31, 24, 24, 24, 24, 24, 24, 24},
	{  0,  0,  0,  0,  0,  0,  0, 63, 54, 54, 54, 54, 54, 54, 54, 54},
	{ 54, 54, 54, 54, 54, 54, 54,255, 54, 54, 54, 54, 54, 54, 54, 54},
	{ 24, 24, 24, 24, 24, 24,255, 24,255, 24, 24, 24, 24, 24, 24, 24},
	{ 24, 24, 24, 24, 24, 24, 24,248,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,  0, 31, 24, 24, 24, 24, 24, 24, 24, 24},
	{255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255},
	{  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255,255,255,255,255},
	{240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240},
	{ 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
	{255,255,255,255,255,255,255,255,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,121,207,198,198,207,121,  0,  0,  0,  0,  0},
	{  0,  0, 28, 54,102,124,102,102,102,124, 96, 96,  0,  0,  0,  0},
	{  0,  0,126, 50, 48, 48, 48, 48, 48, 48,120,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,254,108,108,108,108,108,  0,  0,  0,  0,  0,  0},
	{  0,  0,254,194, 96, 48, 24, 48, 96,194,254,  0,  0,  0,  0,  0},
	{  0,  0, 30, 56,104,204,204,204,204,204,120,  0,  0,  0,  0,  0},
	{  0,  0,  0,102,102,102,102,102,102,124, 96, 96,192,128,  0,  0},
	{  0,  0,  0,110, 58, 48, 48, 48, 48, 48,  0,  0,  0,  0,  0,  0},
	{  0,  0,124, 16,124,214,214,214,124, 16,124,  0,  0,  0,  0,  0},
	{  0,  0, 60,102,102,102,126,102,102,102, 60,  0,  0,  0,  0,  0},
	{  0,  0, 60, 66,195,195,195, 66, 36,165,231,  0,  0,  0,  0,  0},
	{  0,  0, 28, 38, 32, 16, 28, 38,102,102,102, 60,  0,  0,  0,  0},
	{  0,  0,  0,  0,108,146,146,146,108,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0, 24, 24,126,219,219,219,219,126, 24, 24,  0,  0,  0,  0},
	{  0,  0, 62, 96,192,192,254,192,192, 96, 62,  0,  0,  0,  0,  0},
	{  0,  0, 60,102,195,195,195,195,195,195,195,  0,  0,  0,  0,  0},
	{  0,  0,126,  0,  0,  0,126,  0,  0,  0,126,  0,  0,  0,  0,  0},
	{  0,  0, 16, 16, 16,254, 16, 16, 16,  0,254,  0,  0,  0,  0,  0},
	{  0,  0,128, 96, 24,  6, 24, 96,128,  0,254,  0,  0,  0,  0,  0},
	{  0,  0,  6, 24, 96,128, 96, 24,  6,  0,254,  0,  0,  0,  0,  0},
	{  0,  0,  0, 14, 27, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24},
	{ 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,216,112,  0,  0,  0},
	{  0,  0,  0,  0, 24, 24,  0,126,  0, 24, 24,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0, 96,146, 12, 96,146, 12,  0,  0,  0,  0,  0},
	{  0, 56, 68, 68, 68, 56,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,  0, 24, 24,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0, 30, 24, 24, 24, 24, 24, 24, 88, 56, 24,  8,  0,  0,  0},
	{  0,  0,176, 72, 72, 72, 72,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0, 48, 72,  8, 16, 32,120,  0,  0,  0,  0,  0,  0,  0,  0},
	{  0,  0,  0,126,126,126,126,126,126,126,126,  0,  0,  0,  0,  0},
	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}
};
//[*]--------------------------------------------------------------------------------------------------------------[*]
