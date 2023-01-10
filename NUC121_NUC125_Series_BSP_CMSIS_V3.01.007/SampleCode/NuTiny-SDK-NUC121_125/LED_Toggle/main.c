#include "stdio.h"
#include "NuMicro.h"
#include "string.h"
#include "i2c.h"
#include "timer.h"

#define COK1 PB4
#define COK2 PC3
#define COK3 PC1
#define COK4 PC13
#define COK5 PC11
#define COK6 PC9

#define CRG1 PB5
#define CRG2 PC2
#define CRG3 PC0
#define CRG4 PC12
#define CRG5 PC10
#define CRG6 PC8

#define SMBRST PD3
#define IOExpanderInterrupt PB14

#define LEDON 0
#define LEDOFF 1
#define LEDCHANGE 1

#define I2CSwitchAddress 0X70
#define ChargeBoardAddress 0X09
#define BMSAddress 0X0b
#define IOExpanderAddress 0x21

volatile uint32_t *COK[6] = { &COK1, &COK2, &COK3, &COK4, &COK5, &COK6 };
volatile uint32_t *CRG[6] = { &CRG1, &CRG2, &CRG3, &CRG4, &CRG5, &CRG6 };

char ChargeBoardID[2];
char BMSManufacturerAccessCommad[2] = { 0X00, 0X01 };
char BMSID[2];
char TempPresentVoltage_mv[2];
char TempPresentCurrent_ma[2];
char TempPresentTemperature_dK[2];
char TempDeviceName[8];
char TempBatteryStatus[2];
char TempBatteryDesiredChargeCurrent[2];
char TempBatteryDesiredChargeVoltage[2];
char IOExpanderState[2] = { 0XFF, 0XFF };

char DesingChargeVoltage[2] = { 0XA0, 0X41 };
char DesingChargeCurrent[2] = { 0XB0, 0X4 };
char InputCurrent[2] = { 0Xc4, 0X9 };
char ChargeOption0[2];
char ChargeOption1[2];
char ChargeOption2[2];
char ChargeOption3[2];
char ProchotOption0[2];
char ProchotOption1[2];
char ProchotStatus[2];

uint16_t PresentVoltage_mv[6];
uint16_t PresentCurrent_ma[6];
uint16_t PresentTemperature_dK[6];
char PresentDevuceName[48];
uint16_t PresentBatteryStatus[6];
uint16_t BatteryDesiredChargeCurrent[6];
uint16_t BatteryDesiredChargeVoltage[6];

uint8_t ChargeBoardState;
uint8_t ChargeBatteryState;
uint8_t ChargeNonErroState = 0XFF;
uint8_t ChargeOngoingState;
uint8_t ChargeDoneState;
uint8_t ChargeState[6];

uint8_t TimesReadData = 0;

void SYS_Init(void) {
	CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
	CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

	CLK_EnableModuleClock(USBD_MODULE);
	CLK_EnableModuleClock(I2C1_MODULE);
	CLK_EnableModuleClock(TMR3_MODULE);

	CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
	CLK_SetModuleClock(USBD_MODULE, CLK_CLKSEL3_USBDSEL_HIRC, CLK_CLKDIV0_USB(1));
	CLK_SetModuleClock(TMR3_MODULE, CLK_CLKSEL1_TMR3SEL_HIRC_DIV2, 0);
	SystemCoreClockUpdate();

	SYS->GPA_MFPH &= ~(SYS_GPA_MFPH_PA10MFP_Msk | SYS_GPA_MFPH_PA11MFP_Msk);
	SYS->GPA_MFPH |= (SYS_GPA_MFPH_PA10MFP_I2C1_SDA | SYS_GPA_MFPH_PA11MFP_I2C1_SCL);
	PA->SMTEN |= GPIO_SMTEN_SMTEN10_Msk | GPIO_SMTEN_SMTEN11_Msk;
}

void I2C1_Close(void) {
	/* Disable I2C0 interrupt and clear corresponding NVIC bit */
	I2C_DisableInt(I2C1);
	NVIC_DisableIRQ(I2C1_IRQn);

	/* Disable I2C0 and close I2C0 clock */
	I2C_Close(I2C1);
	CLK_DisableModuleClock(I2C1_MODULE);
}

void ChargeStateUpdate() {
	for (int i = 0; i < 6; i++) {
		ChargeState[i] = (((ChargeBoardState >> i) & 1) << 4) + (((ChargeBatteryState >> i) & 1) << 3) + (((ChargeNonErroState >> i) & 1) << 2)
				+ (((ChargeOngoingState >> i) & 1) << 1) + ((ChargeDoneState >> i) & 1);
	}
}
void LEDUpdate(void) {
	for (int i = 0; i < 6; i++) {
		if (ChargeState[i] <= 0b10000) { // Charge Board None Exist
			*COK[i] ^= 1;
			*CRG[i] = LEDOFF;
		} else if (ChargeState[i] <= 0b11000 && ChargeState[i] > 0b10000) { //Battery None Exist
			*COK[i] = LEDOFF;
			*CRG[i] = LEDOFF;

		} else if (ChargeState[i] <= 0b11100 && ChargeState[i] > 0b11000) { //Charge Erro
			*COK[i] = LEDOFF;
			*CRG[i] ^= 1;
		} else if (ChargeState[i] <= 0b11110 && ChargeState[i] > 0b11100) { //Charge in process
			*COK[i] = LEDOFF;
			*CRG[i] = LEDON;
		} else if (ChargeState[i] == 0b11111) { //Charge Done
			*COK[i] = LEDON;
			*CRG[i] = LEDOFF;
		}
	}
}

//void ChargeBoardRegisterRead() {
//	for (int i = 0; i < 6; i++) {
//		if ((0b1 << i) & ChargeBoardState) {
//			I2C_WriteByte(I2C1, I2CSwitchAddress, 0x01 << i);
//			I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X15, DesingChargeVoltage, sizeof(DesingChargeVoltage));
//			I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X14, DesingChargeCurrent, sizeof(DesingChargeCurrent));
//			I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X3F, InputCurrent, sizeof(InputCurrent));
//			I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X12, ChargeOption0, sizeof(ChargeOption0));
//			I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X3b, ChargeOption1, sizeof(ChargeOption1));
//			I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X38, ChargeOption2, sizeof(ChargeOption2));
//			I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X37, ChargeOption3, sizeof(ChargeOption3));
//			I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X3c, ProchotOption0, sizeof(ProchotOption0));
//			I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X3d, ProchotOption1, sizeof(ProchotOption1));
//			I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X3a, ProchotStatus, sizeof(ProchotStatus));
//		}
//	}
//}
void ReadBatteryInformation() {
	for (int i = 0; i < 6; i++) {
		if (((0b1 << i) & ChargeBoardState) && ((0b1 << i) & ChargeBatteryState)) {
			I2C_WriteByte(I2C1, I2CSwitchAddress, 0x01 << i);

			I2C_ReadMultiBytesOneReg(I2C1, BMSAddress, 0X09, TempPresentVoltage_mv, sizeof(TempPresentVoltage_mv));
//			if (!TempPresentVoltage_mv[0] == 0x17)
			PresentVoltage_mv[i] = (TempPresentVoltage_mv[0] + (TempPresentVoltage_mv[1] << 8));
			memset(TempPresentVoltage_mv, 0, sizeof(TempPresentVoltage_mv));

			I2C_ReadMultiBytesOneReg(I2C1, BMSAddress, 0X0a, TempPresentCurrent_ma, sizeof(TempPresentCurrent_ma));
//			if (!TempPresentCurrent_ma[0] == 0x17)
			PresentCurrent_ma[i] = (TempPresentCurrent_ma[0] + (TempPresentCurrent_ma[1] << 8));
			memset(TempPresentCurrent_ma, 0, sizeof(TempPresentCurrent_ma));

			I2C_ReadMultiBytesOneReg(I2C1, BMSAddress, 0X08, TempPresentTemperature_dK, sizeof(TempPresentTemperature_dK));
//			if (!TempPresentTemperature_dK[0] == 0x1717)
			PresentTemperature_dK[i] = (TempPresentTemperature_dK[0] + (TempPresentTemperature_dK[1] << 8));
			memset(TempPresentTemperature_dK, 0, sizeof(TempPresentTemperature_dK));

//			I2C_ReadMultiBytesOneReg(I2C1, BMSAddress, 0X21, TempDeviceName, sizeof(TempDeviceName));
//			for (int a = 0; a < 8; a++) {
//				PresentDevuceName[a + i * 8] = TempDeviceName[a];
//			}
//			 {

			I2C_ReadMultiBytesOneReg(I2C1, BMSAddress, 0X14, TempBatteryStatus, sizeof(TempBatteryStatus));
//			if (!TempBatteryStatus[0] == 0x1717)
			PresentBatteryStatus[i] = (TempBatteryStatus[0] + (TempBatteryStatus[1] << 8));
			memset(TempBatteryStatus, 0, sizeof(TempBatteryStatus));

//			I2C_ReadMultiBytesOneReg(I2C1, BMSAddress, 0X16, TempBatteryDesiredChargeCurrent, sizeof(TempBatteryDesiredChargeCurrent));
//			BatteryDesiredChargeCurrent[i] = (TempBatteryDesiredChargeCurrent[0] + (TempBatteryDesiredChargeCurrent[1] << 8));
//			memset(TempBatteryDesiredChargeCurrent, 0, sizeof(TempBatteryDesiredChargeCurrent));
//
//			I2C_ReadMultiBytesOneReg(I2C1, BMSAddress, 0X15, TempBatteryDesiredChargeVoltage, sizeof(TempBatteryDesiredChargeVoltage));
//			BatteryDesiredChargeVoltage[i] = (TempBatteryDesiredChargeVoltage[0] + (TempBatteryDesiredChargeVoltage[1] << 8));
//			memset(TempBatteryDesiredChargeVoltage, 0, sizeof(TempBatteryDesiredChargeVoltage));

			if (PresentVoltage_mv[i] > 16700) {
				ChargeDoneState |= 0b1 << i;
			} else {
				ChargeDoneState &= 0b0 << i;
			}

			if (PresentCurrent_ma[i] > 500 || PresentVoltage_mv[i] > 16400) {
				ChargeOngoingState |= 0b1 << i;
			} else {
				ChargeOngoingState &= 0b0 << i;
			}

		}
	}
}

void TMR3_IRQHandler(void) {

	if (TIMER_GetIntFlag(TIMER3) == 1) {
		/* Clear Timer3 time-out interrupt flag */
		TIMER_ClearIntFlag(TIMER3);
//		COKLEDState ^= ~ChargeBoardState;
		ChargeStateUpdate();
		LEDUpdate();
//		ChargeBoardRegisterRead();
		TimesReadData++;
		if (TimesReadData >= 2) {
			TimesReadData = 0;
			ReadBatteryInformation();
		}

	}
}
void GPAB_IRQHandler(void) {
	/* To check if PB.14 interrupt occurred */
	if (GPIO_GET_INT_FLAG(PB, BIT14)) {
		GPIO_CLR_INT_FLAG(PB, BIT14);
		I2C_ReadMultiBytes(I2C1, IOExpanderAddress, IOExpanderState, sizeof(IOExpanderState));
		ChargeBatteryState = IOExpanderState[0] & 0b111111;
	} else {
		uint32_t u32Status;

		/* Un-expected interrupt. Just clear all PORTA, PORTB interrupts */
		u32Status = PA->INTSRC;
		PA->INTSRC = u32Status;
		u32Status = PB->INTSRC;
		PB->INTSRC = u32Status;
	}
}

void delay_loop(void) {
	__IO uint32_t j;

	for (j = 0; j < 480000; j++)
		;

}

void ChargeBoardScan() {
	for (int i = 0; i < 6; i++) {
		I2C_WriteByte(I2C1, I2CSwitchAddress, 0x01 << i);
		I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0XFF, ChargeBoardID, sizeof(ChargeBoardID));
		ChargeBoardState |= ((ChargeBoardID[0] == 0X30 && ChargeBoardID[1] == 0X00) ? 1 : 0) << i;
		memset(ChargeBoardID, 0, sizeof(ChargeBoardID));
	}
}
void ChargeBoardWriteVoltageAndCurrent() {
	for (int i = 0; i < 6; i++) {
		if ((0b1 << i) & ChargeBoardState) {
			I2C_WriteByte(I2C1, I2CSwitchAddress, 0x01 << i);
			I2C_WriteMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X15, DesingChargeVoltage, sizeof(DesingChargeVoltage));
			I2C_WriteMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X14, DesingChargeCurrent, sizeof(DesingChargeCurrent));
			I2C_WriteMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X3F, InputCurrent, sizeof(InputCurrent));
		}
	}
}
//void ChargeBoardReadVoltageAndCurrent() {
//	for (int i = 0; i < 6; i++) {
//		if ((0b1 << i) & ChargeBoardState) {
//			I2C_WriteByte(I2C1, I2CSwitchAddress, 0x01 << i);
//			I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X15, DesingChargeVoltage, sizeof(DesingChargeVoltage));
//			I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X14, DesingChargeCurrent, sizeof(DesingChargeCurrent));
//		}
//	}
//}
//void ChargeBoardEnableBatteryPresent() {
//	for (int i = 0; i < 6; i++) {
//		I2C_WriteByte(I2C1, I2CSwitchAddress, 0x01 << i);
//		I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X3D, ProchotOption1, sizeof(ProchotOption1));
//		ProchotOption1[0] |= 0b10;
//		I2C_WriteMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X3D, ProchotOption1, sizeof(ProchotOption1));
//		I2C_ReadMultiBytesOneReg(I2C1, ChargeBoardAddress, 0X3D, ProchotOption1, sizeof(ProchotOption1));
//	}
//}

void LEDFlash() {
	for (int i = 0; i < 2; i++) {
		for (int i = 0; i < 6; i++) {
			*COK[i] ^= LEDCHANGE;
			TIMER_Delay(TIMER3, 250000);
			*COK[i] ^= LEDCHANGE;
		}
		for (int i = 0; i < 6; i++) {
			*CRG[i] ^= LEDCHANGE;
			TIMER_Delay(TIMER3, 250000);
			*CRG[i] ^= LEDCHANGE;
		}
	}
	for (int i = 0; i < 6; i++) {
		*COK[i] = LEDOFF;
		*CRG[i] = LEDOFF;
	}
}

int32_t main(void) {
	SYS_UnlockReg();
	SYS_Init();
	SYS_LockReg();

	GPIO_SetMode(PC, BIT0, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT1, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT2, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT3, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT8, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT9, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT10, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT11, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT12, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT13, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PB, BIT4, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PB, BIT5, GPIO_MODE_OUTPUT);

	LEDFlash();

	I2C_Open(I2C1, 10000);

	ChargeBoardScan();
	ChargeBoardWriteVoltageAndCurrent();
//	ChargeBoardReadVoltageAndCurrent();
//	ChargeBoardRegisterRead();

	I2C_WriteMultiBytes(I2C1, IOExpanderAddress, IOExpanderState, sizeof(IOExpanderState));
	I2C_ReadMultiBytes(I2C1, IOExpanderAddress, IOExpanderState, sizeof(IOExpanderState));
	ChargeBatteryState = IOExpanderState[0] & 0b111111;

	GPIO_SetMode(PD, BIT3, GPIO_MODE_OPEN_DRAIN);
	SMBRST = 1;
	TIMER_Open(TIMER3, TIMER_PERIODIC_MODE, 0.5);
	TIMER_EnableInt(TIMER3);
	NVIC_EnableIRQ(TMR3_IRQn);
	TIMER_Start(TIMER3);
	GPIO_SetMode(PB, BIT14, GPIO_MODE_INPUT);
	GPIO_EnableInt(PB, 14, GPIO_INT_FALLING);
	NVIC_EnableIRQ(GPAB_IRQn);

	while (1) {

	}
}

