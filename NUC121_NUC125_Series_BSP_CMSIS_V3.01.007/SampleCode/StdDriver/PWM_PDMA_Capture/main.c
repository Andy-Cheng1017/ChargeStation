/**************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * @brief    Capture the PWM1 Channel 0 waveform by PWM1 Channel 2, and use PDMA to transfer captured data.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define PLL_CLOCK       100000000

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint16_t g_u16Count[4];
volatile uint32_t g_u32IsTestOver = 0;


/**
 * @brief       PDMA IRQ Handler
 *
 * @param       None
 *
 * @return      None
 *
 * @details     ISR to handle PDMA interrupt event
 */
void PDMA_IRQHandler(void)
{
    uint32_t u32Status = PDMA_GET_INT_STATUS();

    if (u32Status & 0x1)   /* abort */
    {
        if (PDMA_GET_ABORT_STS() & 0x1)
            g_u32IsTestOver = 2;

        PDMA_CLR_ABORT_FLAG(PDMA_ABTSTS_ABTIF0_Msk);
    }
    else if (u32Status & 0x2)     /* done */
    {
        if (PDMA_GET_TD_STS() & 0x1)
            g_u32IsTestOver = 1;

        PDMA_CLR_TD_FLAG(PDMA_TDSTS_TDIF0_Msk);
    }
    else
        printf("unknown interrupt !!\n");
}

/*--------------------------------------------------------------------------------------*/
/* Capture function to calculate the input waveform information                         */
/* g_u16Count[4] : Keep the internal counter value when input signal rising / falling   */
/*               happens                                                                */
/*                                                                                      */
/* time    A    B     C     D                                                           */
/*           ___   ___   ___   ___   ___   ___   ___   ___                              */
/*      ____|   |_|   |_|   |_|   |_|   |_|   |_|   |_|   |_____                        */
/* index              0 1   2 3                                                         */
/*                                                                                      */
/* The capture internal counter down count from 0x10000, and reload to 0x10000 after    */
/* input signal falling happens (Time B/C/D)                                            */
/*--------------------------------------------------------------------------------------*/
void CalPeriodTime(PWM_T *PWM, uint32_t u32Ch)
{
    uint16_t u16RisingTime, u16FallingTime, u16HighPeriod, u16LowPeriod, u16TotalPeriod;
    uint32_t u32TimeOutCount;

    g_u32IsTestOver = 0;

    /* setup timeout */
    u32TimeOutCount = SystemCoreClock;

    /* Wait PDMA interrupt (g_u32IsTestOver will be set at IRQ_Handler function) */
    while (g_u32IsTestOver == 0)
    {
        if (u32TimeOutCount == 0)
        {
            printf("\nSomething is wrong, please check if pin connection is correct. \n");

            while (1);
        }

        u32TimeOutCount--;
    }

    u16RisingTime = g_u16Count[1];

    u16FallingTime = g_u16Count[0];

    u16HighPeriod = g_u16Count[1] - g_u16Count[2];

    u16LowPeriod = 0x10000 - g_u16Count[1];

    u16TotalPeriod = 0x10000 - g_u16Count[2];

    printf("\nPWM generate: \nHigh Period=14999 ~ 15001, Low Period=34999 ~ 35001, Total Period=49999 ~ 50001\n");
    printf("\nCapture Result: Rising Time = %d, Falling Time = %d \nHigh Period = %d, Low Period = %d, Total Period = %d.\n\n",
           u16RisingTime, u16FallingTime, u16HighPeriod, u16LowPeriod, u16TotalPeriod);

    if ((u16HighPeriod < 14999) || (u16HighPeriod > 15001) || (u16LowPeriod < 34999) || (u16LowPeriod > 35001) || (u16TotalPeriod < 49999) || (u16TotalPeriod > 50001))
        printf("Capture Test Fail!!\n");
    else
        printf("Capture Test Pass!!\n");
}

void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Enable HIRC clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Waiting for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Select HCLK clock source as HIRC and and HCLK clock divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);

    /* Waiting for PLL clock ready */
    CLK_WaitClockReady(CLK_STATUS_PLLSTB_Msk);

    /* Enable PWM1 module clock */
    CLK_EnableModuleClock(PWM1_MODULE);

    /*---------------------------------------------------------------------------------------------------------*/
    /* PWM clock frequency configuration                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Select HCLK clock source as PLL and and HCLK clock divider as 2 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_PLL, CLK_CLKDIV0_HCLK(2));

    /* PWM clock frequency can be set equal or double to HCLK by choosing case 1 or case 2 */
    /* case 1.PWM clock frequency is set equal to HCLK: select PWM module clock source as PCLK */
    CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL1_PWM1SEL_PCLK1, 0);

    /* case 2.PWM clock frequency is set double to HCLK: select PWM module clock source as PLL */
    //CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL1_PWM1SEL_PLL, 0);
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable PDMA module clock */
    CLK_EnableModuleClock(PDMA_MODULE);

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART module clock source as HIRC_DIV2 and UART module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UARTSEL_HIRC_DIV2, CLK_CLKDIV0_UART(1));

    /* Reset PWM1 module */
    SYS_ResetModule(PWM1_RST);

    /* Reset PDMA module */
    SYS_ResetModule(PDMA_RST);

    /* Update System Core Clock */
    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set PB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPL &= ~(SYS_GPB_MFPL_PB0MFP_Msk | SYS_GPB_MFPL_PB1MFP_Msk);
    SYS->GPB_MFPL |= (SYS_GPB_MFPL_PB0MFP_UART0_RXD | SYS_GPB_MFPL_PB1MFP_UART0_TXD);

    /* Set PC multi-function pins for PWM1 Channel 0 and 2 */
    SYS->GPC_MFPL = (SYS->GPC_MFPL & (~SYS_GPC_MFPL_PC0MFP_Msk));
    SYS->GPC_MFPL |= SYS_GPC_MFPL_PC0MFP_PWM1_CH0;
    SYS->GPC_MFPL = (SYS->GPC_MFPL & (~SYS_GPC_MFPL_PC2MFP_Msk));
    SYS->GPC_MFPL |= SYS_GPC_MFPL_PC2MFP_PWM1_CH2;
}

void UART0_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART module */
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);
}


/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{
    uint32_t u32TimeOutCount;

    /* Init System, IP clock and multi-function I/O
       In the end of SYS_Init() will issue SYS_LockReg()
       to lock protected register. If user want to write
       protected register, please issue SYS_UnlockReg()
       to unlock protected register if necessary */

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, IP clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART to 115200-8n1 for print message */
    UART0_Init();

    printf("\n\nCPU @ %dHz(PLL@ %dHz)\n", SystemCoreClock, PllClock);
    printf("+------------------------------------------------------------------------+\n");
    printf("|                          PWM Driver Sample Code                        |\n");
    printf("+------------------------------------------------------------------------+\n");
    printf("  This sample code will use PWM1 channel 2 to capture the signal from PWM1 channel 0.\n");
    printf("  And the captured data is transferred by PDMA channel 0.\n");
    printf("  I/O configuration:\n");
    printf("    PWM1 channel 2(PC.2) <--> PWM1 channel 0(PC.0)\n\n");
    printf("Use PWM1 Channel 2(PC.2) to capture the PWM1 Channel 0(PC.0) Waveform\n");

    while (1)
    {
        printf("\n\nPress any key to start PWM Capture Test\n");
        getchar();

        /*--------------------------------------------------------------------------------------*/
        /* Set the PWM1 Channel 0 as PWM output function.                                       */
        /*--------------------------------------------------------------------------------------*/

        /* Assume PWM output frequency is 250Hz and duty ratio is 30%, user can calculate PWM settings by follows.
            duty ratio = (CMR)/(CNR+1)
            cycle time = CNR+1
            High level = CMR
            PWM clock source frequency = PLL = 50000000
            (CNR+1) = PWM clock source frequency/prescaler/PWM output frequency
                    = 50000000/4/250 = 50000
            (Note: CNR is 16 bits, so if calculated value is larger than 65536, user should increase prescale value.)
            CNR = 49999
            duty ratio = 30% ==> (CMR)/(CNR+1) = 30%
            CMR = 15000
            Prescale value is 3 : prescaler= 4
         */

        /* Set PWM1 channel 0 output configuration */
        PWM_ConfigOutputChannel(PWM1, 0, 250, 30);

        /* Enable PWM Output path for PWM1 channel 0 */
        PWM_EnableOutput(PWM1, PWM_CH_0_MASK);

        /* Enable Timer for PWM1 channel 0 */
        PWM_Start(PWM1, PWM_CH_0_MASK);

        /*--------------------------------------------------------------------------------------*/
        /* Configure PDMA peripheral mode form PWM to memory                                    */
        /*--------------------------------------------------------------------------------------*/
        /* Open Channel 0 */
        PDMA_Open(0x1);

        /* Transfer width is half word(16 bit) and transfer count is 4 */
        PDMA_SetTransferCnt(0, PDMA_WIDTH_16, 4);

        /* Set source address as PWM capture channel PDMA register(no increment) and destination address as g_u16Count array(increment) */
        PDMA_SetTransferAddr(0, (uint32_t)&PWM1->PDMACAP2_3, PDMA_SAR_FIX, (uint32_t)&g_u16Count[0], PDMA_DAR_INC);

        /* Select PDMA request source as PWM RX(PWM1 channel 2 should be PWM1 pair 2) */
        PDMA_SetTransferMode(0, PDMA_PWM1_P1_RX, FALSE, 0);

        /* Set PDMA as single request type for PWM */
        PDMA_SetBurstType(0, PDMA_REQ_SINGLE, PDMA_BURST_4);

        PDMA_EnableInt(0, PDMA_INT_TRANS_DONE);
        NVIC_EnableIRQ(PDMA_IRQn);

        /* Enable PDMA for PWM1 channel 2 capture function, and set capture order as falling first, */
        /* And select capture mode as both rising and falling to do PDMA transfer. */
        PWM_EnablePDMA(PWM1, 2, FALSE, PWM_CAPTURE_PDMA_RISING_FALLING_LATCH);

        /*--------------------------------------------------------------------------------------*/
        /* Set the PWM1 channel 2 for capture function                                          */
        /*--------------------------------------------------------------------------------------*/

        /* If input minimum frequency is 250Hz, user can calculate capture settings by follows.
           Capture clock source frequency = PLL = 50000000 in the sample code.
           (CNR+1) = Capture clock source frequency/prescaler/minimum input frequency
                   = 50000000/4/250 = 50000
           (Note: CNR is 16 bits, so if calculated value is larger than 65536, user should increase prescale value.)
           CNR = 0xFFFF
           (Note: In capture mode, user should set CNR to 0xFFFF to increase capture frequency range.)

           Capture unit time = 1/(Capture clock source frequency/prescaler)
           80ns = 1/(50000000/4)
        */

        /* Set PWM1 channel 2 capture configuration */
        PWM_ConfigCaptureChannel(PWM1, 2, 80, 0);

        /* Enable Timer for PWM1 channel 2 */
        PWM_Start(PWM1, PWM_CH_2_MASK);

        /* Enable Capture Function for PWM1 channel 2 */
        PWM_EnableCapture(PWM1, PWM_CH_2_MASK);

        /* Enable falling capture reload */
        PWM1->CAPCTL |= PWM_CAPCTL_FCRLDEN2_Msk;

        /* setup timeout */
        u32TimeOutCount = SystemCoreClock;

        /* Wait until PWM1 channel 2 Timer start to count */
        while ((PWM1->CNT[2]) == 0)
        {
            if (u32TimeOutCount == 0)
            {
                printf("\nSomething is wrong, please check if pin connection is correct. \n");

                while (1);
            }

            u32TimeOutCount--;
        }

        /* Capture the Input Waveform Data */
        CalPeriodTime(PWM1, 2);
        /*---------------------------------------------------------------------------------------------------------*/
        /* Stop PWM1 channel 0 (Recommended procedure method 1)                                                    */
        /* Set PWM Timer loaded value(Period) as 0. When PWM internal counter(CNT) reaches to 0, disable PWM Timer */
        /*---------------------------------------------------------------------------------------------------------*/

        /* Set PWM1 channel 0 loaded value as 0 */
        PWM_Stop(PWM1, PWM_CH_0_MASK);

        /* setup timeout */
        u32TimeOutCount = SystemCoreClock;

        /* Wait until PWM1 channel 0 Timer Stop */
        while ((PWM1->CNT[0] & PWM_CNT_CNT_Msk) != 0)
        {
            if (u32TimeOutCount == 0)
            {
                printf("\nSomething is wrong, please check if pin connection is correct. \n");

                while (1);
            }

            u32TimeOutCount--;
        }

        /* Disable Timer for PWM1 channel 0 */
        PWM_ForceStop(PWM1, PWM_CH_0_MASK);

        /* Disable PWM Output path for PWM1 channel 0 */
        PWM_DisableOutput(PWM1, PWM_CH_0_MASK);

        /*---------------------------------------------------------------------------------------------------------*/
        /* Stop PWM1 channel 2 (Recommended procedure method 1)                                                    */
        /* Set PWM Timer loaded value(Period) as 0. When PWM internal counter(CNT) reaches to 0, disable PWM Timer */
        /*---------------------------------------------------------------------------------------------------------*/

        /* Set loaded value as 0 for PWM1 channel 2 */
        PWM_Stop(PWM1, PWM_CH_2_MASK);

        /* setup timeout */
        u32TimeOutCount = SystemCoreClock;

        /* Wait until PWM1 channel 2 current counter reach to 0 */
        while ((PWM1->CNT[2] & PWM_CNT_CNT_Msk) != 0)
        {
            if (u32TimeOutCount == 0)
            {
                printf("\nSomething is wrong, please check if pin connection is correct. \n");

                while (1);
            }

            u32TimeOutCount--;
        }

        /* Disable Timer for PWM1 channel 2 */
        PWM_ForceStop(PWM1, PWM_CH_2_MASK);

        /* Disable Capture Function and Capture Input path for  PWM1 channel 2*/
        PWM_DisableCapture(PWM1, PWM_CH_2_MASK);

        /* Clear Capture Interrupt flag for PWM1 channel 2 */
        PWM_ClearCaptureIntFlag(PWM1, 2, PWM_CAPTURE_INT_FALLING_LATCH);

        /* Disable PDMA NVIC */
        NVIC_DisableIRQ(PDMA_IRQn);

        PDMA_Close();
    }
}

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
