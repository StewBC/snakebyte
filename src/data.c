/*
 *  data.c
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#include "globals.h"
#include "data.h"

const t_LevelLine ct_level1[] =
{
	{11, 17, 28, 17, gCHAR_Wall1},
};

const t_LevelLine ct_level2[] =
{
	{20,  9, 20, 25, gCHAR_Wall1},
	{12, 17, 28, 17, gCHAR_Wall1},
};

const t_LevelLine ct_level3[] =
{
	{11,  9, 28,  9, gCHAR_Wall6},
	{11, 10, 11, 25, gCHAR_Wall6},
	{12, 17, 28, 17, gCHAR_Wall6},
	{12, 25, 28, 25, gCHAR_Wall6},
};

const t_LevelLine ct_level4[] =
{
	{11,  9, 11, 25, gCHAR_Wall8},
	{20,  9, 20, 25, gCHAR_Wall8},
	{28,  9, 28, 25, gCHAR_Wall8},
	{12, 17, 19, 17, gCHAR_Wall4},
	{21, 17, 27, 17, gCHAR_Wall4},
};

const t_LevelLine ct_level5[] =
{
	{11,  9, 18,  9, gCHAR_Wall7},
	{21,  9, 28,  9, gCHAR_Wall7},
	{11, 10, 11, 25, gCHAR_Wall7},
	{28, 10, 28, 25, gCHAR_Wall7},
	{12, 25, 28, 25, gCHAR_Wall7},
};

const t_LevelLine ct_level6[] =
{
	{11,  9, 11, 25, gCHAR_Wall7},
	{16,  9, 28,  9, gCHAR_Wall7},
	{28, 10, 28, 25, gCHAR_Wall7},
	{12, 17, 23, 17, gCHAR_Wall7},
	{16, 25, 28, 25, gCHAR_Wall7},
};

const t_LevelLine ct_level7[] =
{
	{ 1, 16, 18, 16, gCHAR_Wall1},
	{21, 17, 39, 17, gCHAR_Wall1},
};

const t_LevelLine ct_level8[] =
{
	{ 5,  9,  5, 25, gCHAR_Wall1},
	{10,  9, 10, 25, gCHAR_Wall2},
	{15,  9, 15, 25, gCHAR_Wall3},
	{20,  9, 20, 25, gCHAR_Wall6},
	{25,  9, 25, 25, gCHAR_Wall3},
	{30,  9, 30, 25, gCHAR_Wall2},
	{35,  9, 35, 25, gCHAR_Wall1},
};

const t_LevelLine ct_level9[] =
{
	{11,  9, 24,  9, gCHAR_Wall8},
	{28,  9, 28, 25, gCHAR_Wall6},
	{11, 10, 11, 25, gCHAR_Wall8},
	{15, 13, 28, 13, gCHAR_Wall6},
	{12, 17, 24, 17, gCHAR_Wall8},
	{15, 21, 28, 21, gCHAR_Wall6},
	{12, 25, 24, 25, gCHAR_Wall8},
};

const t_LevelLine ct_level10[] =
{
	{11,  9, 28,  9, gCHAR_Wall7},
	{11, 10, 11, 15, gCHAR_Wall7},
	{20, 10, 20, 25, gCHAR_Wall7},
	{28, 10, 28, 15, gCHAR_Wall7},
	{11, 18, 11, 25, gCHAR_Wall7},
	{28, 18, 28, 25, gCHAR_Wall7},
	{12, 25, 28, 25, gCHAR_Wall7},
};

const t_LevelLine ct_level11[] =
{
	{11,  9, 28,  9, gCHAR_Wall6},
	{11, 10, 11, 16, gCHAR_Wall6},
	{28, 10, 28, 16, gCHAR_Wall6},
	{20, 15, 20, 19, gCHAR_Wall7},
	{11, 18, 11, 25, gCHAR_Wall6},
	{28, 18, 28, 25, gCHAR_Wall6},
	{12, 25, 28, 25, gCHAR_Wall6},
};

const t_LevelLine ct_level12[] =
{
	{ 6,  5,  6, 11, gCHAR_Wall1},
	{13,  5, 19,  5, gCHAR_Wall2},
	{26,  5, 33,  5, gCHAR_Wall3},
	{19,  6, 19, 11, gCHAR_Wall2},
	{33,  6, 33, 18, gCHAR_Wall3},
	{ 7, 11, 12, 11, gCHAR_Wall1},
	{20, 11, 25, 11, gCHAR_Wall2},
	{26, 18, 33, 18, gCHAR_Wall3},
	{ 6, 19, 12, 19, gCHAR_Wall1},
	{19, 19, 19, 24, gCHAR_Wall2},
	{20, 24, 33, 24, gCHAR_Wall2},
	{ 6, 25,  6, 29, gCHAR_Wall3},
	{33, 25, 33, 29, gCHAR_Wall2},
	{12, 29, 28, 29, gCHAR_Wall1},
};

const t_LevelLine ct_level13[] =
{
	{17,  1, 17,  6, gCHAR_Wall6},
	{17,  9, 17, 16, gCHAR_Wall6},
	{ 1, 16, 17, 16, gCHAR_Wall6},
	{21, 19, 39, 19, gCHAR_Wall8},
	{21, 20, 21, 25, gCHAR_Wall8},
	{21, 28, 21, 34, gCHAR_Wall8},
};

const t_LevelLine ct_level14[] =
{
	{19,  1, 19, 14, gCHAR_Wall7},
	{ 1, 17, 16, 17, gCHAR_Wall7},
	{22, 17, 39, 17, gCHAR_Wall7},
	{19, 20, 19, 34, gCHAR_Wall7},
};

const t_LevelLine ct_level15[] =
{
	{21,  1, 21, 16, gCHAR_Wall9},
	{ 1, 16, 18, 16, gCHAR_Wall9},
	{18, 19, 18, 34, gCHAR_Wall9},
	{21, 19, 39, 19, gCHAR_Wall9},
};

const t_LevelLine ct_level16[] =
{
	{19,  1, 19,  7, gCHAR_Wall2},
	{19,  9, 19, 25, gCHAR_Wall2},
	{ 1, 17,  9, 17, gCHAR_Wall2},
	{11, 17, 28, 17, gCHAR_Wall2},
	{30, 17, 39, 17, gCHAR_Wall2},
	{19, 27, 19, 34, gCHAR_Wall2},
};

const t_LevelLine ct_level17[] =
{
	{ 6,  5, 11,  5, gCHAR_Wall7},
	{18,  5, 18,  7, gCHAR_Wall6},
	{21,  5, 21,  7, gCHAR_Wall6},
	{28,  5, 33,  5, gCHAR_Wall7},
	{11,  6, 11, 10, gCHAR_Wall7},
	{28,  6, 28, 10, gCHAR_Wall7},
	{15,  7, 18,  7, gCHAR_Wall6},
	{22,  7, 24,  7, gCHAR_Wall6},
	{ 8,  8,  8, 17, gCHAR_Wall7},
	{15,  8, 15, 10, gCHAR_Wall6},
	{24,  8, 24, 10, gCHAR_Wall6},
	{31,  8, 31, 17, gCHAR_Wall7},
	{18, 10, 21, 10, gCHAR_Wall8},
	{20, 13, 20, 26, gCHAR_Wall8},
	{11, 14, 15, 14, gCHAR_Wall6},
	{24, 14, 28, 14, gCHAR_Wall6},
	{11, 15, 11, 24, gCHAR_Wall6},
	{28, 15, 28, 24, gCHAR_Wall6},
	{ 6, 17,  8, 17, gCHAR_Wall7},
	{32, 17, 33, 17, gCHAR_Wall7},
	{ 6, 18,  6, 21, gCHAR_Wall7},
	{33, 18, 33, 21, gCHAR_Wall7},
	{15, 19, 24, 19, gCHAR_Wall8},
	{12, 24, 15, 24, gCHAR_Wall6},
	{24, 24, 28, 24, gCHAR_Wall6},
	{ 6, 25,  6, 29, gCHAR_Wall4},
	{33, 25, 33, 29, gCHAR_Wall4},
	{15, 27, 15, 29, gCHAR_Wall9},
	{24, 27, 24, 29, gCHAR_Wall9},
	{11, 28, 11, 29, gCHAR_Wall4},
	{28, 28, 28, 29, gCHAR_Wall4},
	{ 7, 29, 11, 29, gCHAR_Wall4},
	{16, 29, 24, 29, gCHAR_Wall9},
	{29, 29, 33, 29, gCHAR_Wall4},
};

const t_LevelLine ct_level18[] =
{
	{ 6,  5, 18,  5, gCHAR_Wall8},
	{21,  5, 33,  5, gCHAR_Wall8},
	{ 6,  6,  6, 29, gCHAR_Wall8},
	{33,  6, 33, 29, gCHAR_Wall8},
	{11, 10, 28, 10, gCHAR_Wall6},
	{11, 11, 11, 24, gCHAR_Wall6},
	{28, 11, 28, 16, gCHAR_Wall6},
	{28, 19, 28, 24, gCHAR_Wall6},
	{12, 24, 28, 24, gCHAR_Wall6},
	{ 7, 29, 18, 29, gCHAR_Wall8},
	{21, 29, 33, 29, gCHAR_Wall8},
};

const t_LevelLine ct_level19[] =
{
	{11,  8, 28,  8, gCHAR_Wall9},
	{20,  9, 20, 15, gCHAR_Wall9},
	{ 9, 10,  9, 24, gCHAR_Wall9},
	{30, 10, 30, 24, gCHAR_Wall9},
	{10, 17, 18, 17, gCHAR_Wall9},
	{22, 17, 30, 17, gCHAR_Wall9},
	{20, 19, 20, 26, gCHAR_Wall9},
	{11, 26, 28, 26, gCHAR_Wall9},
};

const t_LevelLine ct_level20[] =
{
	{18,  1, 18,  5, gCHAR_Wall1},
	{21,  1, 21, 26, gCHAR_Wall1},
	{18,  8, 18, 34, gCHAR_Wall1},
	{21, 29, 21, 34, gCHAR_Wall1},
};

const t_LevelLine ct_level21[] =
{
	{ 5,  4, 34,  4, gCHAR_Wall1},
	{ 8,  5,  8, 29, gCHAR_Wall1},
	{ 5,  7,  5, 29, gCHAR_Wall1},
	{11,  7, 34,  7, gCHAR_Wall2},
	{11,  8, 11, 29, gCHAR_Wall2},
	{14, 10, 34, 10, gCHAR_Wall3},
	{14, 11, 14, 29, gCHAR_Wall3},
	{17, 13, 34, 13, gCHAR_Wall7},
	{17, 14, 17, 29, gCHAR_Wall7},
	{20, 16, 34, 16, gCHAR_Wall6},
	{20, 17, 20, 29, gCHAR_Wall6},
	{23, 19, 34, 19, gCHAR_Wall8},
	{23, 20, 23, 29, gCHAR_Wall8},
	{26, 22, 34, 22, gCHAR_Wall4},
	{26, 23, 26, 29, gCHAR_Wall4},
	{29, 25, 34, 25, gCHAR_Wall5},
	{29, 26, 29, 29, gCHAR_Wall5},
	{32, 29, 34, 29, gCHAR_Wall9},
};

const t_LevelLine ct_level22[] =
{
	{28,  5, 28,  7, gCHAR_Wall6},
	{31,  5, 31,  7, gCHAR_Wall6},
	{ 2,  7,  9,  7, gCHAR_Wall7},
	{12,  7, 19,  7, gCHAR_Wall7},
	{21,  7, 28,  7, gCHAR_Wall6},
	{32,  7, 37,  7, gCHAR_Wall6},
	{ 2,  8,  2, 16, gCHAR_Wall7},
	{ 9,  8,  9,  9, gCHAR_Wall7},
	{12,  8, 12,  9, gCHAR_Wall7},
	{19,  8, 19, 16, gCHAR_Wall7},
	{21,  8, 21, 16, gCHAR_Wall6},
	{37,  8, 37, 16, gCHAR_Wall6},
	{ 3, 16, 19, 16, gCHAR_Wall7},
	{22, 16, 37, 16, gCHAR_Wall6},
	{ 2, 18, 19, 18, gCHAR_Wall8},
	{21, 18, 37, 18, gCHAR_Wall4},
	{ 2, 19,  2, 27, gCHAR_Wall8},
	{19, 19, 19, 27, gCHAR_Wall8},
	{21, 19, 21, 27, gCHAR_Wall4},
	{37, 19, 37, 27, gCHAR_Wall4},
	{28, 25, 28, 27, gCHAR_Wall4},
	{31, 25, 31, 27, gCHAR_Wall4},
	{ 3, 27,  9, 27, gCHAR_Wall8},
	{12, 27, 19, 27, gCHAR_Wall8},
	{22, 27, 28, 27, gCHAR_Wall4},
	{32, 27, 37, 27, gCHAR_Wall4},
	{ 9, 28,  9, 29, gCHAR_Wall8},
	{12, 28, 12, 29, gCHAR_Wall8},
};

const t_LevelLine ct_level23[] =
{
	{15,  1, 15, 14, gCHAR_Wall8},
	{19,  1, 19, 14, gCHAR_Wall8},
	{23,  1, 23, 14, gCHAR_Wall8},
	{17,  2, 17, 16, gCHAR_Wall5},
	{21,  2, 21, 16, gCHAR_Wall5},
	{15, 16, 23, 16, gCHAR_Wall5},
	{15, 18, 23, 18, gCHAR_Wall5},
	{17, 19, 17, 33, gCHAR_Wall5},
	{21, 19, 21, 33, gCHAR_Wall5},
	{15, 20, 15, 34, gCHAR_Wall8},
	{19, 20, 19, 34, gCHAR_Wall8},
	{23, 20, 23, 34, gCHAR_Wall8},
};

const t_LevelLine ct_level24[] =
{
	{ 5,  5, 10,  5, gCHAR_Wall7},
	{12,  5, 12,  7, gCHAR_Wall6},
	{27,  5, 27,  7, gCHAR_Wall6},
	{29,  5, 34,  5, gCHAR_Wall7},
	{ 5,  6,  5, 10, gCHAR_Wall7},
	{34,  6, 34, 10, gCHAR_Wall7},
	{ 7,  7, 12,  7, gCHAR_Wall6},
	{14,  7, 25,  7, gCHAR_Wall8},
	{28,  7, 32,  7, gCHAR_Wall6},
	{ 7,  8,  7, 12, gCHAR_Wall6},
	{32,  8, 32, 12, gCHAR_Wall6},
	{ 9,  9, 18,  9, gCHAR_Wall4},
	{21,  9, 30,  9, gCHAR_Wall4},
	{ 9, 10,  9, 16, gCHAR_Wall4},
	{30, 10, 30, 16, gCHAR_Wall4},
	{ 5, 12,  7, 12, gCHAR_Wall6},
	{33, 12, 34, 12, gCHAR_Wall6},
	{ 7, 14,  7, 21, gCHAR_Wall8},
	{32, 14, 32, 21, gCHAR_Wall8},
	{ 9, 19,  9, 26, gCHAR_Wall4},
	{30, 19, 30, 26, gCHAR_Wall4},
	{ 5, 23,  7, 23, gCHAR_Wall6},
	{32, 23, 34, 23, gCHAR_Wall6},
	{ 7, 24,  7, 28, gCHAR_Wall6},
	{32, 24, 32, 28, gCHAR_Wall6},
	{ 5, 25,  5, 30, gCHAR_Wall7},
	{34, 25, 34, 30, gCHAR_Wall7},
	{10, 26, 18, 26, gCHAR_Wall4},
	{21, 26, 30, 26, gCHAR_Wall4},
	{ 8, 28, 12, 28, gCHAR_Wall6},
	{14, 28, 25, 28, gCHAR_Wall8},
	{27, 28, 32, 28, gCHAR_Wall6},
	{12, 29, 12, 30, gCHAR_Wall6},
	{27, 29, 27, 30, gCHAR_Wall6},
	{ 6, 30, 10, 30, gCHAR_Wall7},
	{29, 30, 34, 30, gCHAR_Wall7},
};

const t_LevelLine ct_level25[] =
{
	{27,  1, 27,  7, gCHAR_Wall3},
	{34,  4, 34,  9, gCHAR_Wall3},
	{28,  7, 32,  7, gCHAR_Wall3},
	{36,  7, 39,  7, gCHAR_Wall3},
	{27,  9, 37,  9, gCHAR_Wall3},
	{27, 10, 27, 23, gCHAR_Wall3},
	{12, 12, 24, 12, gCHAR_Wall2},
	{12, 13, 12, 26, gCHAR_Wall2},
	{24, 13, 24, 20, gCHAR_Wall2},
	{15, 15, 21, 15, gCHAR_Wall3},
	{15, 16, 15, 23, gCHAR_Wall3},
	{21, 16, 21, 17, gCHAR_Wall3},
	{18, 18, 18, 20, gCHAR_Wall2},
	{19, 20, 24, 20, gCHAR_Wall2},
	{16, 23, 27, 23, gCHAR_Wall3},
	{ 2, 26, 12, 26, gCHAR_Wall2},
	{ 5, 27,  5, 31, gCHAR_Wall2},
	{ 1, 28,  3, 28, gCHAR_Wall2},
	{ 7, 28, 12, 28, gCHAR_Wall2},
	{12, 29, 12, 34, gCHAR_Wall2},
};

const t_LevelLine ct_level26[] =
{
	{17,  2, 17,  5, gCHAR_Wall7},
	{23,  2, 23,  5, gCHAR_Wall7},
	{18,  5, 23,  5, gCHAR_Wall7},
	{10,  7, 19,  7, gCHAR_Wall6},
	{21,  7, 29,  7, gCHAR_Wall6},
	{10,  8, 10, 13, gCHAR_Wall6},
	{29,  8, 29, 13, gCHAR_Wall6},
	{ 1, 15,  8, 15, gCHAR_Wall6},
	{10, 15, 10, 26, gCHAR_Wall6},
	{29, 15, 29, 26, gCHAR_Wall6},
	{31, 15, 39, 15, gCHAR_Wall6},
	{11, 19, 15, 19, gCHAR_Wall6},
	{17, 19, 22, 19, gCHAR_Wall6},
	{24, 19, 29, 19, gCHAR_Wall6},
	{ 2, 24,  8, 24, gCHAR_Wall8},
	{31, 24, 37, 24, gCHAR_Wall8},
	{ 8, 25,  8, 33, gCHAR_Wall8},
	{31, 25, 31, 33, gCHAR_Wall8},
	{16, 29, 19, 29, gCHAR_Wall8},
	{21, 29, 24, 29, gCHAR_Wall8},
	{16, 30, 16, 33, gCHAR_Wall8},
	{24, 30, 24, 33, gCHAR_Wall8},
	{ 9, 33, 16, 33, gCHAR_Wall8},
	{25, 33, 31, 33, gCHAR_Wall8},
};

const t_LevelLine ct_level27[] =
{
	{ 2, 10,  9, 10, gCHAR_Wall7},
	{ 2, 11,  2, 16, gCHAR_Wall7},
	{ 3, 16,  9, 16, gCHAR_Wall7},
	{12, 16, 12, 23, gCHAR_Wall3},
	{15, 16, 18, 16, gCHAR_Wall6},
	{21, 16, 21, 23, gCHAR_Wall8},
	{24, 16, 24, 23, gCHAR_Wall4},
	{29, 16, 29, 23, gCHAR_Wall4},
	{32, 16, 37, 16, gCHAR_Wall5},
	{ 9, 17,  9, 23, gCHAR_Wall7},
	{15, 17, 15, 23, gCHAR_Wall6},
	{32, 17, 32, 19, gCHAR_Wall5},
	{33, 19, 37, 19, gCHAR_Wall5},
	{37, 20, 37, 23, gCHAR_Wall5},
	{ 2, 23,  9, 23, gCHAR_Wall7},
	{25, 23, 29, 23, gCHAR_Wall4},
	{32, 23, 37, 23, gCHAR_Wall5},
};

const t_LevelLine ct_rip[] =
{
	{ 6,  4, 33,  4, gCHAR_Wall6},
	{ 6,  5,  6, 25, gCHAR_Wall6},
	{33,  5, 33, 25, gCHAR_Wall6},
	{10,  8, 10, 17, gCHAR_Wall6},
	{23,  8, 23, 23, gCHAR_Wall6},
	{11,  9, 14,  9, gCHAR_Wall6},
	{18,  9, 18, 17, gCHAR_Wall6},
	{24,  9, 27,  9, gCHAR_Wall6},
	{27, 10, 27, 17, gCHAR_Wall6},
	{13, 17, 13, 17, gCHAR_Wall6},
	{20, 17, 20, 17, gCHAR_Wall6},
	{24, 17, 27, 17, gCHAR_Wall6},
	{30, 17, 30, 17, gCHAR_Wall6},
	{10, 19, 21, 19, gCHAR_Wall6},
	{25, 19, 27, 19, gCHAR_Wall6},
	{ 7, 25, 33, 25, gCHAR_Wall6},
};
const char cc_numRIPLines = sizeof(ct_rip) / sizeof(ct_rip[0]);

const t_LevelLine *ct_levels[] =
{
	0,
	ct_level1,
	ct_level2,
	ct_level3,
	ct_level4,
	ct_level5,
	ct_level6,
	ct_level7,
	ct_level8,
	ct_level9,
	ct_level10,
	ct_level11,
	ct_level12,
	ct_level13,
	ct_level14,
	ct_level15,
	ct_level16,
	ct_level17,
	ct_level18,
	ct_level19,
	ct_level20,
	ct_level21,
	ct_level22,
	ct_level23,
	ct_level24,
	ct_level25,
	ct_level26,
	ct_level27,
};
const char cc_numLevels = sizeof(ct_levels) / sizeof(ct_levels[0]);

const char cc_levelLines[] =
{
	0,
	sizeof(ct_level1) / sizeof(ct_level1[0]),
	sizeof(ct_level2) / sizeof(ct_level2[0]),
	sizeof(ct_level3) / sizeof(ct_level3[0]),
	sizeof(ct_level4) / sizeof(ct_level4[0]),
	sizeof(ct_level5) / sizeof(ct_level5[0]),
	sizeof(ct_level6) / sizeof(ct_level6[0]),
	sizeof(ct_level7) / sizeof(ct_level7[0]),
	sizeof(ct_level8) / sizeof(ct_level8[0]),
	sizeof(ct_level9) / sizeof(ct_level9[0]),
	sizeof(ct_level10) / sizeof(ct_level10[0]),
	sizeof(ct_level11) / sizeof(ct_level11[0]),
	sizeof(ct_level12) / sizeof(ct_level12[0]),
	sizeof(ct_level13) / sizeof(ct_level13[0]),
	sizeof(ct_level14) / sizeof(ct_level14[0]),
	sizeof(ct_level15) / sizeof(ct_level15[0]),
	sizeof(ct_level16) / sizeof(ct_level16[0]),
	sizeof(ct_level17) / sizeof(ct_level17[0]),
	sizeof(ct_level18) / sizeof(ct_level18[0]),
	sizeof(ct_level19) / sizeof(ct_level19[0]),
	sizeof(ct_level20) / sizeof(ct_level20[0]),
	sizeof(ct_level21) / sizeof(ct_level21[0]),
	sizeof(ct_level22) / sizeof(ct_level22[0]),
	sizeof(ct_level23) / sizeof(ct_level23[0]),
	sizeof(ct_level24) / sizeof(ct_level24[0]),
	sizeof(ct_level25) / sizeof(ct_level25[0]),
	sizeof(ct_level26) / sizeof(ct_level26[0]),
	sizeof(ct_level27) / sizeof(ct_level27[0]),
};

const char cc_bonusText[] = 		"       * *  bonus =  %-4d  * *          ";
const char cc_scoreText[] = 		"score            snakes      apples";
const char cc_hiScoreText[] = 	" hi         (c) 1982 sirius    level";
const char cc_numPlumsText[] =	"         number of plums (0-2)?         ";
const char cc_crashText[] = 		"    crash!  press space to continue.    ";
const char cc_gameOvertext[]= 	"               game over                ";
const char *cc_welcomeText[] =
{
	"            sirius presents             ",
	"             game design by             ",
	"           chuck summerville            ",
	"      programmed by dan stansfield      ",
	"  this version by stefan wessels, 2011  ",
	"  music and sound effects by ray elzey  ",
	"         (c) 1982, 1983 sirius          "
};
const char cc_numWelcomeTextLines = sizeof(cc_welcomeText) / sizeof(cc_welcomeText[0]);

const char *cc_scrollText[] =
{
	"        the object:     guide your snake",
	"through the  various  mazes,  eating all",
	"the apples that appear.                 ",
	"                                        ",
	"you begin each level with  10  apples to",
	"devour.   for  every  apple  eaten,  the",
	"snake  gets a little  longer and travels",
	"a  little faster.   if the snake's  head",
	"runs into the walls or itself,  it dies.",
	"                                        ",
	"      should you take too long to eat an",
	"apple,  you will be given  3 more apples",
	"as a penalty.   when all your apples are",
	"gone,  a small door will open at the top",
	"of the screen.   go through this door to",
	"enter the next level.                   ",
	"                                        ",
	"    you have the choice of playing with ",
	"zero, one, or two perilous purple plums.",
	"these  seemingly  innocent fruits bounce",
	"randomly about the screen,  intending no",
	"harm to anything.   the snake,  however,",
	"is terribly allergic to plums.  if it is",
	"hit in the  head  with a  plum,  it will",
	"shrivel up and die  (not good).         ",
	"the game is  easiest to play without any",
	"plums,  but more  sophisticated  players",
	"will want the  challenge and the  higher",
	"scores that plums offer.                ",
	"                                        ",
	"  there are twenty-eight different mazes",
	"to conquer.   good luck!!!              ",
	"                                        ",
	"       * * keyboard  controls * *       ",
	"press:                                  ",
	"    a  or  i    to go   up              ",
	"    z  or  m    to go   down            ",
	"    :  or  j    to go   left            ",
	"    ;  or  k    to go   right           ",
	"    <           to go  counter-clockwise",
	"    >           to go  clockwise        ",
};
const char cc_numScrollTextLines = sizeof(cc_scrollText) / sizeof(cc_scrollText[0]);
