/**************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * @brief    Use timer0 periodic time-out interrupt event to wake up system.
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global Interface Variables Declarations                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
extern int IsDebugFifoEmpty(void);
volatile uint8_t g_u8IsTMR0WakeupFlag = 0;
volatile uint32_t g_au32TMRINTCount[4] = {0};


/*---------------------------------------------------------------------------------------------------------*/
/*  Function for System Entry to Power Down Mode                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void PowerDownFunction(void)
{
    printf("\nSystem enter to power-down mode ...\n\n");

    /* To check if all the debug messages are finished */
    while (IsDebugFifoEmpty() == 0);

    SCB->SCR = 4;

    /* To program PWRCTL register, it needs to disable register protection first. */
    CLK->PWRCTL &= ~CLK_PWRCTL_PDEN_Msk;
    CLK->PWRCTL |= (CLK_PWRCTL_PDEN_Msk | CLK_PWRCTL_PDWKIEN_Msk);

    __WFI();
}

/**
 * @brief       Timer0 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The Timer0 default IRQ, declared in startup_M0564.s.
 */
void TMR0_IRQHandler(void)
{
    if (TIMER_GetIntFlag(TIMER0) == 1)
    {
        /* Clear Timer0 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER0);

        g_au32TMRINTCount[0]++;
    }

    if (TIMER_GetWakeupFlag(TIMER0) == 1)
    {
        /* Clear Timer0 wake-up flag */
        TIMER_ClearWakeupFlag(TIMER0);

        g_u8IsTMR0WakeupFlag = 1;
    }
}

void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Enable HIRC clock */
    CLK->PWRCTL |= CLK_PWRCTL_HIRCEN_Msk;

    /* Waiting for HIRC clock ready */
    while (!(CLK->STATUS & CLK_STATUS_HIRCSTB_Msk));

    /* Switch HCLK clock source to HIRC */
    CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLKSEL_Msk)) | CLK_CLKSEL0_HCLKSEL_HIRC;
    CLK->CLKDIV0 = (CLK->CLKDIV0 & (~CLK_CLKDIV0_HCLKDIV_Msk)) | CLK_CLKDIV0_HCLK(1);

    /* Enable LIRC */
    CLK->PWRCTL |= CLK_PWRCTL_LIRCEN_Msk;

    while (!(CLK->STATUS & CLK_STATUS_LIRCSTB_Msk));

    /*------------------------------------------------------------------------------------------------------*/
    /* Enable Module Clock                                                                                  */
    /*------------------------------------------------------------------------------------------------------*/
    /* Enable peripheral clock */
    CLK->APBCLK0 = CLK_APBCLK0_UART0CKEN_Msk | CLK_APBCLK0_TMR0CKEN_Msk;

    /* Peripheral clock source */
    CLK->CLKSEL1 = CLK_CLKSEL1_UARTSEL_HIRC_DIV2 | CLK_CLKSEL1_TMR0SEL_LIRC;

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* PB.0 is UART0_RX */
    SYS->GPB_MFPL &= ~SYS_GPB_MFPL_PB0MFP_Msk;
    SYS->GPB_MFPL |= (1 << SYS_GPB_MFPL_PB0MFP_Pos);
    /* PB.1 is UART0_TX */
    SYS->GPB_MFPL &= ~SYS_GPB_MFPL_PB1MFP_Msk;
    SYS->GPB_MFPL |= (1 << SYS_GPB_MFPL_PB1MFP_Pos);
}

void UART0_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART module */
    SYS->IPRST1 |=  SYS_IPRST1_UART0RST_Msk;
    SYS->IPRST1 &= ~SYS_IPRST1_UART0RST_Msk;

    /* Configure UART0 and set UART0 Baudrate */
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_MODE2_DIVIDER(__HIRC_DIV2, 115200);
    UART0->LINE = UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1;
}

/*---------------------------------------------------------------------------------------------------------*/
/*  MAIN function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{
    volatile uint32_t u32InitCount;

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART0 for printf */
    UART0_Init();

    /* Use P0.0 to measure enable/disable CNTEN bit active time */
    SYS->GPA_MFPH &= ~SYS_GPA_MFPH_PA11MFP_Msk;
    PA11 = 1;

    printf("\n\nCPU @ %d Hz\n", SystemCoreClock);
    printf("+-------------------------------------------+\n");
    printf("|    Timer0 Time-out Wake-up Sample Code    |\n");
    printf("+-------------------------------------------+\n\n");

    printf("# Timer0 Settings:\n");
    printf("    - Clock source is LIRC          \n");
    printf("    - Time-out frequency is 1 Hz    \n");
    printf("    - Periodic mode                 \n");
    printf("    - Interrupt enable              \n");
    printf("    - Wake-up function enable       \n");
    printf("# System will enter to Power-down mode while Timer0 interrupt counts is reaching 3.\n");
    printf("  And will be wakeup while Timer0 interrupt counts is reaching 4.\n");
    printf("# Measure P0.0 low period to check timer counter start/stop sync time should be around 1~2 TMR_CLK.\n\n");

    /* Enable Timer0 NVIC */
    NVIC_EnableIRQ(TMR0_IRQn);

    /* Open Timer0 time-out frequency to 1 Hz in periodic mode */
    TIMER0->CMP = __LIRC;
    TIMER0->CTL = TIMER_PERIODIC_MODE | TIMER_CTL_INTEN_Msk | TIMER_CTL_WKEN_Msk;

    PA11 = 0;
    /* Start Timer0 counting */
    TIMER_Start(TIMER0);

    while (TIMER_IS_ACTIVE(TIMER0) == 0);

    PA11 = 1; // Around 1~2 TMR_CLK

    u32InitCount = g_u8IsTMR0WakeupFlag = g_au32TMRINTCount[0] = 0;

    while (g_au32TMRINTCount[0] < 10)
    {
        if (g_au32TMRINTCount[0] != u32InitCount)
        {
            printf("Timer0 interrupt counts - %d\n", g_au32TMRINTCount[0]);

            if (g_au32TMRINTCount[0] == 3)
            {
                /* System enter to Power-down */
                /* To program PWRCTL register, it needs to disable register protection first. */
                SYS_UnlockReg();
                PowerDownFunction();

                /* Check if Timer0 time-out interrupt and wake-up flag occurred */
                while (1)
                {
                    if ((CLK->PWRCTL & CLK_PWRCTL_PDWKIF_Msk) && (g_u8IsTMR0WakeupFlag == 1))
                        break;
                }

                printf("System has been waken-up done. (Timer0 interrupt counts is %d)\n\n", g_au32TMRINTCount[0]);
            }

            u32InitCount = g_au32TMRINTCount[0];
        }
    }

    /* Stop Timer0 counting */
    PA11 = 0;
    TIMER_Stop(TIMER0);

    while (TIMER_IS_ACTIVE(TIMER0));

    PA11 = 1; // Around 1~2 TMR_CLK

    printf("*** PASS ***\n");

    while (1);
}

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
