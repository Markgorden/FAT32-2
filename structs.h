#include <stdint.h>

#define EOC 0x0FFFFFF8

#ifndef B
#define B 1
struct BS_BPB
{
	uint8_t jmpBoot[3];
	uint8_t OEMName[8];
	uint16_t BytsPerSec;
	uint8_t SecPerClus;
	uint16_t RsvdSecCnt;
	uint8_t NumFATs;
	uint16_t RootEntCnt;
	uint16_t TotSec16;
	uint8_t Media;
	uint16_t FATSz16;
	uint16_t SecPerTrk;
	uint16_t NumHeads;
	uint32_t HiddSec;
	uint32_t TotSec32;
	uint32_t FATSz32;
	uint16_t ExtFlags;
	uint16_t FSVer;
	uint32_t RootClus;
	uint16_t FSInfo;
	uint16_t BkBootSec;
	uint8_t Reserved[12];
	uint8_t DrvNum;
	uint8_t Reserved1;
	uint8_t BootSig;
	uint32_t VolID;
	uint8_t VolLab[11];
	uint8_t FilSysType[8];
} __attribute__((packed));
#endif

#ifndef D
#define D 1
struct DIR
{
	uint8_t Name[11];
	uint8_t Attr;
	uint8_t NTRes;
	uint8_t CrtTimeTenth;
	uint16_t CrtTime;
	uint16_t CrtDate;
	uint16_t LstAccDate;
	uint16_t FstClusHI;
	uint16_t WrtTime;
	uint16_t WrtDate;
	uint16_t FstClusLO;
	uint32_t FileSize;
} __attribute__((packed));
#endif

#ifndef F
#define F 1
struct FSI
{
	uint32_t LeadSig;
	uint8_t Reserved1[480];
	uint32_t StrucSig;
	uint32_t Free_Count;
	uint32_t Nxt_free;
	uint8_t Reserved2[12];
	uint32_t TrailSig;
} __attribute__((packed));
#endif
