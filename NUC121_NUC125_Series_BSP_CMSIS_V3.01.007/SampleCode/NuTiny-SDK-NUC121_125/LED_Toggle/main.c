/**************************************************************************/ /**
                                                                              * @file     main.c
                                                                              * @version  V3.00
                                                                              * @brief    Toggle PB.4 to turn on/off LED.
                                                                              *
                                                                              * SPDX-License-Identifier: Apache-2.0
                                                                              * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
                                                                              ******************************************************************************/
#include "stdio.h"
#include "NuMicro.h"

#define COK1 PC4
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


volatile uint32_t *COK[6] = {&COK1, &COK2, &COK3, &COK4, &COK5, &COK6};
volatile uint32_t *CRG[6] = {&CRG1, &CRG2, &CRG3, &CRG4, &CRG5, &CRG6};

void SYS_Init(void)
{

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable HIRC clock (Internal RC 48MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Select HCLK clock source as HIRC and HCLK clock divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART module clock source as HIRC/2 and UART module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UARTSEL_HIRC_DIV2, CLK_CLKDIV0_UART(1));

    /* Update core clock */
    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set PB multi-function pins for UART0 RXD(PB.0) and TXD(PB.1) */
    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB0MFP_Msk)) | SYS_GPB_MFPL_PB0MFP_UART0_RXD;
    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB1MFP_Msk)) | SYS_GPB_MFPL_PB1MFP_UART0_TXD;
}

void UART0_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART0 */
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);
}

void delay_loop(void)
{
    __IO uint32_t j;

    for (j = 0; j < 480000; j++);
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART0 for printf */
    UART0_Init();

    printf("\n\nCPU @ %u Hz\n", SystemCoreClock);
    printf("+-------------------------------------------------+\n");
    printf("|        NUC121/125 Toggle LED Sample Code        |\n");
    printf("+-------------------------------------------------+\n");

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

    while (1)
    {
    	//after change
        for (int i = 0; i < 6; i++)
        {
            *COK[i] ^= 1;
        }
        for (int i = 0; i < 6; i++)
        {
            *CRG[i] ^= 1;
        }
        //before change
//        PC0 ^= 1;
//        PC1 ^= 1;
//        PC2 ^= 1;
//        PC3 ^= 1;
//        PC8 ^= 1;
//        PC9 ^= 1;
//        PC10 ^= 1;
//        PC11 ^= 1;
//        PC12 ^= 1;
//        PC13 ^= 1;
//        PB4 ^= 1;
//        PB5 ^= 1;
        delay_loop();
    }
}
