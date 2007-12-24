/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */


#include "common/endian.h"
#include "common/stream.h"

#include "cine/cine.h"
#include "cine/anim.h"
#include "cine/gfx.h"
#include "cine/pal.h"
#include "cine/part.h"
#include "cine/various.h"

namespace Cine {

struct AnimHeader2Struct {
	uint32 field_0;
	uint16 width;
	uint16 height;
	uint16 type;
	uint16 field_A;
	uint16 field_C;
	uint16 field_E;
};

static uint16 animDataCount = 0;

AnimHeaderStruct animHeader;

static const AnimDataEntry animData[] = {
	{"ALPHA", 0xF},
	{"TITRE2", 0xF},
	{"ET", 0xC},
	{"L311", 0x3},
	{"L405", 0x1},
	{"L515", 0xC},
	{"L009", 0xE},
	{"L010", 0xE},
	{"FUTUR", 0x6},
	{"PAYSAN3", 0xB},
	{"L801", 0xC},
	{"L802", 0xC},
	{"L803", 0xC},
	{"L901", 0xD},
	{"L902", 0x8},
	{"L903", 0xD},
	{"L904", 0xD},
	{"L905", 0xD},
	{"L906", 0xD},
	{"L907", 0xD},
	{"LA03", 0x4},
	{"MOINE", 0xB},
	{"L908", 0x8},
	{"L909", 0x8},
	{"L807", 0xC},
	{"L808", 0xC},
	{"LA01", 0xB},
	{"L1201", 0xC},
	{"L1202", 0xC},
	{"L1203", 0xC},
	{"L1210", 0x5},
	{"L1211", 0xC},
	{"L1214", 0xC},
	{"L1215", 0xC},
	{"L1216", 0xC},
	{"L1217", 0xC},
	{"L1218", 0xC},
	{"L1219", 0xC},
	{"L1220", 0xC},
	{"SEIGNEUR", 0x6},
	{"PERE0", 0xD},
	{"L1302", 0x4},
	{"L1303", 0x4},
	{"L1304", 0x4},
	{"L1401", 0xF},
	{"L1402", 0xF},
	{"L1501", 0x8},
	{"L1503", 0x8},
	{"L1504", 0x4},
	{"L1505", 0x8},
	{"L1506", 0x8},
	{"L1601", 0xB},
	{"L1602", 0xB},
	{"L1603", 0xB},
	{"L1604", 0x4},
	{"L1605", 0x4},
	{"L1701", 0x4},
	{"L1702", 0x4},
	{"L1801", 0x6},
	{"L1904", 0x8},
	{"L2002", 0x8},
	{"L2003", 0x8},
	{"L2101", 0x4},
	{"L2102", 0x4},
	{"L2201", 0x7},
	{"L2202", 0x7},
	{"L2203", 0xE},
	{"L2305", 0x9},
	{"L2306", 0x9},
	{"GARDE1", 0x7},
	{"L2402", 0x7},
	{"L2407", 0x7},
	{"L2408", 0x7},
	{"GARDE2", 0x6},
	{"L2601", 0x6},
	{"L2602", 0x6},
	{"L2603", 0x6},
	{"L2604", 0x6},
	{"L2605", 0x8},
	{"L2606", 0x8},
	{"L2607", 0x8},
	{"L2610", 0x6},
	{"L2611", 0x6},
	{"L2612", 0x6},
	{"L2613", 0x8},
	{"L2614", 0x6},
	{"VOYAGEUR", 0x6},
	{"L2701", 0xD},
	{"L2702", 0xD},
	{"L2703", 0x6},
	{"L2801", 0xD},
	{"L2802", 0xD},
	{"L2803", 0xD},
	{"L2804", 0xD},
	{"L2807", 0xD},
	{"L2902", 0x8},
	{"L2903", 0x8},
	{"L3101", 0xA},
	{"L3102", 0xA},
	{"L3103", 0xA},
	{"L3203", 0xF},
	{"L3204", 0xF},
	{"L3001", 0x7},
	{"L3002", 0x7},
	{"L3416", 0xC},
	{"L3601", 0x5},
	{"L3602", 0x5},
	{"L3603", 0x5},
	{"L3607", 0x5},
	{"L3701", 0x8},
	{"L3702", 0x8},
	{"L3703", 0x8},
	{"L4001", 0xD},
	{"L4002", 0xD},
	{"L4103", 0xF},
	{"L4106", 0xF},
	{"CRUGHON1", 0xC},
	{"L4203", 0xC},
	{"L4301", 0xC},
	{"L4302", 0xC},
	{"L4303", 0xC},
	{"FUTUR2", 0x6},
	{"L4601", 0xE},
	{"L4603", 0x1},
	{"L4106", 0xF},
	{"L4801", 0xD},
	{"L4802", 0xD},
	{"FIN01", 0xB},
	{"FIN02", 0xB},
	{"FIN03", 0xB},
	{"FIN", 0x9},
};

static void freeAnimData(byte idx) {
	assert(idx < NUM_MAX_ANIMDATA);
	if (animDataTable[idx].ptr1) {
		free(animDataTable[idx].ptr1);
		free(animDataTable[idx].ptr2);
		memset(&animDataTable[idx], 0, sizeof(AnimData));
		animDataTable[idx].fileIdx = -1;
		animDataTable[idx].frameIdx = -1;
		if (animDataCount > 0)
			animDataCount--;
	}
}

void freeAnimDataRange(byte startIdx, byte numIdx) {
	for (byte i = 0; i < numIdx; i++) {
		freeAnimData(i + startIdx);
	}
}

void freeAnimDataTable() {
	freeAnimDataRange(0, NUM_MAX_ANIMDATA);
}

static byte getAnimTransparentColor(const char *animName) {
	char name[15];

	removeExtention(name, animName);

	for (int i = 0; i < ARRAYSIZE(animData); i++) {
		if (!strcmp(name, animData[i].name)) {
			return animData[i].color;
		}
	}
	return 0;
}

int16 allocFrame(uint16 width, uint16 height, int8 isMask) {
	uint16 i;
	uint32 frameSize;

	for (i = 0; i < NUM_MAX_ANIMDATA; i++) {
		if (!animDataTable[i].ptr1)
			break;
	}

	if (i == NUM_MAX_ANIMDATA)
		return -1;

	if (!isMask) {		// sprite + generated mask
		frameSize = width * height;

		animDataTable[i].ptr1 = (byte *)malloc(frameSize);
		animDataTable[i].ptr2 = (byte *)malloc(frameSize);
	} else {
		// mask
		frameSize = width * height * 8;

		animDataTable[i].ptr1 = (byte *)malloc(frameSize);
		animDataTable[i].ptr2 = NULL;
	}

	animDataTable[i].width = width;
	animDataTable[i].var1 = width >> 3;
	animDataTable[i].bpp = 4;
	animDataTable[i].height = height;

	animDataTable[i].fileIdx = -1;
	animDataTable[i].frameIdx = -1;

	animDataCount++;

	return i;
}

int16 reserveFrame(uint16 width, uint16 height, uint16 type, int16 idx) {
	uint16 i;
	uint32 frameSize;

	if (idx >= 0) {
		i = (uint16) idx;
	} else {
		for (i = 0; i < NUM_MAX_ANIMDATA; i++) {
			if (!animDataTable[i].ptr1)
				break;
		}

		if (i == NUM_MAX_ANIMDATA)
			return -1;
	}

	frameSize = width * height;

	if (type == 4) {		// 256 color sprites
		frameSize *= 2;
		type = 8;
		width *= 2;
	}

	if (type == 5) {
		frameSize += 16;
	}

	frameSize *= 2;

	animDataTable[i].ptr1 = (byte *)malloc(frameSize);

	assert(animDataTable[i].ptr1);

	animDataTable[i].width = width;

	if (type == 5) {
		animDataTable[i].var1 = width / 8;
	} else {
		animDataTable[i].var1 = width / 16;
	}

	animDataTable[i].bpp = type;

	animDataTable[i].height = height;

	animDataTable[i].fileIdx = -1;
	animDataTable[i].frameIdx = -1;

	animDataCount++;

	return i;
}

void generateMask(byte * sprite, byte * mask, uint16 size, byte transparency) {
	for (uint16 i = 0; i < size; i++) {
		if (*(sprite++) != transparency) {
			*(mask++) = 0;
		} else {
			*(mask++) = 1;
		}
	}
}

void convertMask(byte * dest, byte * source, int16 width, int16 height) {
	int16 i, j;
	byte maskEntry;

	for (i = 0; i < width * height; i++) {
		maskEntry = *(source++);
		for (j = 0; j < 8; j++) {
			*(dest++) = (maskEntry & 0x80) ? 0 : 1;
			maskEntry <<= 1;
		}
	}
}

void convert4BBP(byte * dest, byte * source, int16 width, int16 height) {
	byte maskEntry;

	for (int16 i = 0; i < width * height; i++) {
		maskEntry = *(source++);
		*(dest++) = (maskEntry & 0xF0) >> 4;
		*(dest++) = (maskEntry & 0xF);
	}
}

void loadAnimHeader(Common::MemoryReadStream readS) {
	animHeader.field_0 = readS.readByte();
	animHeader.field_1 = readS.readByte();
	animHeader.field_2 = readS.readByte();
	animHeader.field_3 = readS.readByte();
	animHeader.frameWidth = readS.readUint16BE();
	animHeader.frameHeight = readS.readUint16BE();
	animHeader.field_8 = readS.readByte();
	animHeader.field_9 = readS.readByte();
	animHeader.field_A = readS.readByte();
	animHeader.field_B = readS.readByte();
	animHeader.field_C = readS.readByte();
	animHeader.field_D = readS.readByte();
	animHeader.numFrames = readS.readUint16BE();
	animHeader.field_10 = readS.readByte();
	animHeader.field_11 = readS.readByte();
	animHeader.field_12 = readS.readByte();
	animHeader.field_13 = readS.readByte();
	animHeader.field_14 = readS.readUint16BE();
}

void loadSpl(const char *resourceName, int16 idx) {
	int16 foundFileIdx = findFileInBundle(resourceName);
	int16 entry;
	byte *dataPtr = readBundleFile(foundFileIdx);

	if (idx >= 0) {
		entry = reserveFrame((uint16) partBuffer[foundFileIdx].unpackedSize, 1, 0, idx);
		memcpy(animDataTable[entry].ptr1, dataPtr, partBuffer[foundFileIdx].unpackedSize);
	} else {
		entry = allocFrame((uint16) partBuffer[foundFileIdx].unpackedSize, 1, -1);
		assert(entry != -1);
		memcpy(animDataTable[entry].ptr1, dataPtr, (uint16) partBuffer[foundFileIdx].unpackedSize);

		animDataTable[entry].fileIdx = foundFileIdx;
		animDataTable[entry].frameIdx = 0;
		strcpy(animDataTable[entry].name, currentPartName);
	}

	free(dataPtr);
}

void loadMsk(const char *resourceName) {
	int16 foundFileIdx = findFileInBundle(resourceName);
	int16 entry;
	byte *dataPtr = readBundleFile(foundFileIdx);
	byte *ptr;

	Common::MemoryReadStream readS(dataPtr, 0x16);
	loadAnimHeader(readS);
	ptr = dataPtr + 0x16;

	for (int16 i = 0; i < animHeader.numFrames; i++) {
		entry = allocFrame(animHeader.frameWidth * 2, animHeader.frameHeight, 1);

		assert(entry != -1);

		convertMask(animDataTable[entry].ptr1, ptr, animHeader.frameWidth, animHeader.frameHeight);
		ptr += animHeader.frameWidth * animHeader.frameHeight;

		animDataTable[entry].fileIdx = foundFileIdx;
		animDataTable[entry].frameIdx = i;
		strcpy(animDataTable[entry].name, currentPartName);
	}

	free(dataPtr);
}

void loadAni(const char *resourceName) {
	int16 foundFileIdx = findFileInBundle(resourceName);
	int16 entry;
	byte *dataPtr = readBundleFile(foundFileIdx);
	byte *ptr, *animPtr;
	byte transparentColor;
	uint32 fullSize;

	Common::MemoryReadStream readS(dataPtr, 0x16);
	loadAnimHeader(readS);
	ptr = dataPtr + 0x16;

	transparentColor = getAnimTransparentColor(resourceName);

	fullSize = animHeader.frameWidth * animHeader.frameHeight;

	for (int16 i = 0; i < animHeader.numFrames; i++) {
		entry = allocFrame(animHeader.frameWidth * 2, animHeader.frameHeight, 0);

		assert(entry != -1);

		// special case transparency handling
		if (!strcmp(resourceName, "L2202.ANI")) {
			if (i < 2) {
				transparentColor = 0;
			} else {
				transparentColor = 7;
			}
		}

		if (!strcmp(resourceName, "L4601.ANI")) {
			if (i < 1) {
				transparentColor = 0xE;
			} else {
				transparentColor = 0;
			}
		}

		animPtr = (byte *)malloc(fullSize);

		memcpy(animPtr, ptr, fullSize);
		ptr += fullSize;

		gfxConvertSpriteToRaw(animDataTable[entry].ptr1, animPtr, animHeader.frameWidth, animHeader.frameHeight);

		generateMask(animDataTable[entry].ptr1, animDataTable[entry].ptr2, animHeader.frameWidth * 2 * animHeader.frameHeight, transparentColor);

		free(animPtr);

		animDataTable[entry].fileIdx = foundFileIdx;
		animDataTable[entry].frameIdx = i;
		strcpy(animDataTable[entry].name, currentPartName);
	}

	free(dataPtr);
}

void convert8BBP(byte * dest, byte * source, int16 width, int16 height) {
	byte table[16];
	byte color;

	memcpy(table, source, 16);
	source += 16;

	for (uint16 i = 0; i < width * height; i++) {
		color = *(source++);

		*(dest++) = table[color >> 4];
		*(dest++) = table[color & 0xF];
	}
}

void convert8BBP2(byte * dest, byte * source, int16 width, int16 height) {
	uint16 i, j, k, m;
	byte color;

	for (j = 0; j < (width * height) / 16; j++) {
		// m = 0: even bits, m = 1: odd bits
		for (m = 0; m == 1; m++) {
			for (i = 0; i < 8; i++) {
				color = 0;
				for (k = 14 + m; k == 0 + m; k = k - 2) {
					color |= ((*(source + k) & 0x080) >> 7);
					*(source + k) <<= 1;
					if (k > 0 + m)
						color <<= 1;
				}	// end k
				*(dest++) = color;
			}	// end i
		}	// end m

		source += 0x10;
	}	// end j
}

void loadSet(const char *resourceName, int16 idx) {
	AnimHeader2Struct header2;
	uint32 fullSize;
	uint16 numSpriteInAnim;
	int16 foundFileIdx = findFileInBundle(resourceName);
	int16 entry, typeParam;
	byte *ptr, *startOfDataPtr, *dataPtr, *origDataPtr;
	byte table[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	origDataPtr = dataPtr = readBundleFile(foundFileIdx);
	assert(!memcmp(dataPtr, "SET", 3));
	ptr = dataPtr + 4;

	numSpriteInAnim = READ_BE_UINT16(ptr);
	ptr += 2;

	startOfDataPtr = ptr + numSpriteInAnim * 0x10;

	for (int16 i = 0; i < numSpriteInAnim; i++) {
		typeParam = 0;

		Common::MemoryReadStream readS(ptr, 0x10);

		header2.field_0 = readS.readUint32BE();
		header2.width = readS.readUint16BE();
		header2.height = readS.readUint16BE();
		header2.type = readS.readUint16BE();
		header2.field_A = readS.readUint16BE();
		header2.field_C = readS.readUint16BE();
		header2.field_E = readS.readUint16BE();

		ptr += 0x10;

		fullSize = header2.width * header2.height;

		if (header2.type == 5) {
			fullSize += 16;
		}

		if (header2.type == 4) {
			header2.type = 5;
			typeParam = 1;
		}

		if (idx >= 0)
			entry = reserveFrame(header2.width * 2, header2.height, header2.type, idx + i);
		else
			entry = reserveFrame(header2.width * 2, header2.height, header2.type, -1);

		assert(entry != -1);

		dataPtr = startOfDataPtr + header2.field_0;

		if (typeParam) {
			memcpy(animDataTable[entry].ptr1, table, 0x10);
			gfxConvertSpriteToRaw(animDataTable[entry].ptr1, dataPtr, header2.width, header2.height);
			//memcpy(animDataTable[entry].ptr1 + 0x10, dataPtr, fullSize);
		} else {
			if (header2.type == 1) {
				convert4BBP(animDataTable[entry].ptr1, dataPtr, header2.width, header2.height);
			} else if (header2.type == 5) {
				convert8BBP(animDataTable[entry].ptr1, dataPtr, header2.width, header2.height);
			} else if (header2.type == 4) {
				error("loadSet: header2.type == 4");
			} else {
				convert8BBP2(animDataTable[entry].ptr1, dataPtr, header2.width, header2.height);
			}
		}

		animDataTable[entry].fileIdx = foundFileIdx;
		animDataTable[entry].frameIdx = i;
		strcpy(animDataTable[entry].name, currentPartName);
	}

	free(origDataPtr);
}

void loadSeq(const char *resourceName, int16 idx) {
	int16 foundFileIdx = findFileInBundle(resourceName);
	byte *dataPtr = readBundleFile(foundFileIdx);
	int16 entry = reserveFrame((uint16) partBuffer[foundFileIdx].unpackedSize, 1, 0, idx);

	memcpy(animDataTable[entry].ptr1, dataPtr + 0x16, (uint16) partBuffer[foundFileIdx].unpackedSize - 0x16);
	free(dataPtr);
}

void loadResource(const char *resourceName) {
	/* byte isMask = 0; */
	/* byte isSpl = 0; */

	if (strstr(resourceName, ".SPL")) {
		loadSpl(resourceName, -1);
		return;
	} else if (strstr(resourceName, ".MSK")) {
		loadMsk(resourceName);
		return;
	} else if (strstr(resourceName, ".ANI")) {
		loadAni(resourceName);
		return;
	} else if (strstr(resourceName, ".ANM")) {
		loadAni(resourceName);
		return;
	} else if (strstr(resourceName, ".SET")) {
		loadSet(resourceName, -1);
		return;
	} else if (strstr(resourceName, ".SEQ")) {
		loadSeq(resourceName, -1);
		return;
	} else if (strstr(resourceName, "ECHEC")) {
		exitEngine = 1;
		return;
	}

	error("loadResource: Cannot determine type for '%s'", resourceName);
}

void loadAbs(const char *resourceName, uint16 idx) {
	/* byte isMask = 0; */
	/* byte isSpl = 0; */

	if (strstr(resourceName, ".SET")) {
		loadSet(resourceName, idx);
		return;
	} else if (strstr(resourceName, ".H32")) {
		return;
	} else if (strstr(resourceName, ".SEQ")) {
		loadSeq(resourceName, idx);
		return;
	} else if (strstr(resourceName, ".SPL")) {
		loadSpl(resourceName, idx);
		return;
	} else if (strstr(resourceName, ".AMI")) {
		return;
	} else if (strstr(resourceName, ".ANI")) {
		return;
	}

	error("loadAbs: Cannot determine type for '%s'", resourceName);
}

void loadResourcesFromSave() {
	int16 currentAnim, foundFileIdx, fullSize, entry, i;
	int8 isMask = 0, isSpl = 0;
	byte *dataPtr, *ptr, *animPtr;
	char animName[256], part[256];
	byte transparentColor;
	AnimData *currentPtr;

	strcpy(part, currentPartName);

	for (currentAnim = 0; currentAnim < NUM_MAX_ANIMDATA; currentAnim++) {
		currentPtr = &animDataTable[currentAnim];
		if (currentPtr->refresh && currentPtr->fileIdx != -1) {
			if (strcmp(currentPartName, currentPtr->name)) {
				closePart();
				loadPart(currentPtr->name);
			}

			foundFileIdx = currentPtr->fileIdx;

			strcpy(animName, partBuffer[foundFileIdx].partName);
			ptr = dataPtr = readBundleFile(foundFileIdx);

			isSpl  = (strstr(animName, ".SPL")) ? 1 : 0;
			isMask = (strstr(animName, ".MSK")) ? 1 : 0;

			if (isSpl) {
				animHeader.frameWidth = (uint16) partBuffer[foundFileIdx].unpackedSize;
				animHeader.frameHeight = 1;
				animHeader.numFrames = 1;
				isMask = -1;
			} else {
				Common::MemoryReadStream readS(ptr, 0x22);

				loadAnimHeader(readS);

				ptr += 0x16;
			}

			{
				fullSize = animHeader.frameWidth * animHeader.frameHeight;

				loadRelatedPalette(animName);

				transparentColor = getAnimTransparentColor(animName);

				for (i = 0; i < animHeader.numFrames; i++) { // load all the frames
					// special case transparency handling
					if (!strcmp(animName, "L2202.ANI")) {
						if (i < 2) {
							transparentColor = 0;
						} else {
							transparentColor = 7;
						}
					}

					if (!strcmp(animName, "L4601.ANI")) {
						if (i < 1) {
							transparentColor = 0xE;
						} else {
							transparentColor = 0;
						}
					}

					currentPtr[i].ptr1 = NULL;
					entry = allocFrame(animHeader.frameWidth * 2, animHeader.frameHeight, isMask);

					currentPtr->fileIdx = foundFileIdx;

					assert(entry != -1);

					if (isSpl) {
						memcpy(animDataTable[entry].ptr1, ptr, fullSize);
						ptr += fullSize;
					} else {
						if (!isMask) {
							animPtr = (byte *)malloc(fullSize);
							memcpy(animPtr, ptr, fullSize);
							ptr += fullSize;

							gfxConvertSpriteToRaw(animDataTable[entry].ptr1, animPtr,
										animHeader.frameWidth, animHeader.frameHeight);
							generateMask(animDataTable[entry].ptr1, animDataTable[entry].ptr2,
										animHeader.frameWidth * 2 *animHeader.frameHeight, transparentColor);

							free(animPtr);
						} else {
							convertMask(animDataTable[entry].ptr1, ptr, animHeader.frameWidth,
										animHeader.frameHeight);
							ptr += fullSize;
						}
					}

					//animDataTable[entry].fileIdx = foundFileIdx; // Only when reading from bundles

					animDataTable[entry].frameIdx = i;
					strcpy(animDataTable[entry].name, currentPartName);
				}
			}

			free(dataPtr);
		}
	}

	loadPart(part);
}

} // End of namespace Cine
