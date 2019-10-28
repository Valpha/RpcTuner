#ifndef __TEA5767__H__INCLUDED__
#define __TEA5767__H__INCLUDED__
#include "sys.h"
#include "myiic.h"

#define MAX_FREQUENCE		108000
#define MIN_FREQUENCE		87500

typedef union
{
	u8 wrbyte1data;
	struct{
		u8 PLL8:1;
		u8 PLL9:1;
		u8 PLL10:1;
		u8 PLL11:1;
		u8 PLL12:1;
		u8 PLL13:1;
		u8 SM:1;
		u8 MUTE:1;
	}wrbyte1bits;
}tea5767_writebyte1_st;

typedef union
{
	u8 wrbyte2data;
	struct{
		u8 PLL0:1;
		u8 PLL1:1;
		u8 PLL2:1;
		u8 PLL3:1;
		u8 PLL4:1;
		u8 PLL5:1;
		u8 PLL6:1;
		u8 PLL7:1;
	}wrbyte2bits;
}tea5767_writebyte2_st;

typedef union
{
	u8 wrbyte3data;
	struct{
		u8 SWP1:1;
		u8 ML:1;
		u8 MR:1;
		u8 MS:1;
		u8 HLSI:1;
		u8 SSL0:1;
		u8 SSL1:1;
		u8 SUD:1;
	}wrbyte3bits;
}tea5767_writebyte3_st;

typedef union
{
	u8 wrbyte4data;
	struct{
		u8 SI:1;
		u8 SNC:1;
		u8 HCC:1;
		u8 SMUTE:1;
		u8 XTAL:1;
		u8 BL:1;
		u8 STBY:1;
		u8 SWP2:1;
	}wrbyte4bits;
}tea5767_writebyte4_st;

typedef union
{
	u8 wrbyte5data;
	struct{
		u8 RESERVED:6;
		u8 DTC:1;
		u8 PLLREF:1;
	}wrbyte5bits;
}tea5767_writebyte5_st;

typedef struct{
	tea5767_writebyte1_st byte1;
	tea5767_writebyte2_st byte2;
	tea5767_writebyte3_st byte3;
	tea5767_writebyte4_st byte4;
	tea5767_writebyte5_st byte5;
}tea5767_writebytes_st;


typedef union
{
	u8 rdbyte1data;
	struct{
		u8 PLL8:1;
		u8 PLL9:1;
		u8 PLL10:1;
		u8 PLL11:1;
		u8 PLL12:1;
		u8 PLL13:1;
		u8 BLF:1;
		u8 RF:1;
	}rdbyte1bits;
}tea5767_readbyte1_st;

typedef union
{
	u8 rdbyte2data;
	struct{
		u8 PLL0:1;
		u8 PLL1:1;
		u8 PLL2:1;
		u8 PLL3:1;
		u8 PLL4:1;
		u8 PLL5:1;
		u8 PLL6:1;
		u8 PLL7:1;
	}rdbyte2bits;
}tea5767_readbyte2_st;

typedef union
{
	u8 rdbyte3data;
	struct{
		u8 IF0:1;
		u8 IF1:1;
		u8 IF2:1;
		u8 IF3:1;
		u8 IF4:1;
		u8 IF5:1;
		u8 IF6:1;
		u8 STEREO:1;
	}rdbyte3bits;
}tea5767_readbyte3_st;

typedef union
{
	u8 rdbyte4data;
	struct{
		u8 ZERO:1;
		u8 CI1:1;
		u8 CI2:1;
		u8 CI3:1;
		u8 LEV0:1;
		u8 LEV1:1;
		u8 LEV2:1;
		u8 LEV3:1;
	}rdbyte4bits;
}tea5767_readbyte4_st;

typedef union
{
	u8 rdbyte5data;
	struct{
		u8 ZERO:8;
	}rdbyte5bits;
}tea5767_readbyte5_st;

typedef struct{
	tea5767_readbyte1_st byte1;
	tea5767_readbyte2_st byte2;
	tea5767_readbyte3_st byte3;
	tea5767_readbyte4_st byte4;
	tea5767_readbyte5_st byte5;
}tea5767_readbytes_st;

extern void TEA5767_Init(void);
extern void TEA5767_Write(void);
extern u32 TEA5767_GetPLL(u32 frequency, u8 hlsi);
extern u32 TEA5767_GetFrequencybyPll(u32 npll, u8 hlsi);
extern u32 TEA5767_GetFrequency(u8 hlsi);
extern void TEA5767_SetFrequency(u32 fre);
extern void TEA5767_Read(void);
extern void TEA5767_MuteOnSetFrequency(u32 fre, u8 hlsi);
extern void TEA5767_MuteOffSetFrequency(u32 fre, u8 hlsi);
extern void TEA5767_ManualSerachOnSetFrequency(u32 fre, u8 hlsi, u8 sud);
extern void TEA5767_ManualSerachOffSetFrequency(u32 fre, u8 hlsi);
extern u8 TEA5767_isRFOn(void);
extern u8 TEA5767_isBLFOn(void);
extern u8 TEA5767_isHLSIOn(void);
extern u8 TEA5767_isStereoOn(void);
extern u8 TEA5767_getADCLevel(void);
extern u8 TEA5767_GetIF(void);

#endif
