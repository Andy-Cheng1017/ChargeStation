/******************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * @brief
 *           Demonstrate how to transfer data between USB device and PC through USB HID interface.
 *           A windows tool is also included in this sample code to connect with USB device.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"
#include "hid_transfer.h"

#define CRYSTAL_LESS    1
#define HIRC_AUTO_TRIM  (SYS_IRCTCTL_REFCKSEL_Msk | 0x2);   /* Use USB signal to fine tune HIRC 48MHz */
#define TRIM_INIT       (SYS_BASE+0x110)
#define TRIM_THRESHOLD  16      /* Each value is 0.125%, max 2% */

#if CRYSTAL_LESS
    static volatile uint32_t s_u32DefaultTrim, s_u32LastTrim;
#endif

/*--------------------------------------------------------------------------*/
void SYS_Init(void)
{

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable Internal HIRC 48 MHz clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

#if (CRYSTAL_LESS)
    /* Switch HCLK clock source to Internal HIRC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Select module clock source */
    CLK_SetModuleClock(USBD_MODULE, CLK_CLKSEL3_USBDSEL_HIRC, CLK_CLKDIV0_USB(1));
#else
    /* Enable External XTAL (4~24 MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN);

    /* Waiting for 12MHz clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(FREQ_48MHZ);

    /* Select module clock source */
    CLK_SetModuleClock(USBD_MODULE, CLK_CLKSEL3_USBDSEL_PLL, CLK_CLKDIV0_USB(2));
#endif

    /* Enable module clock */
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(USBD_MODULE);

    /* Select module clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UARTSEL_HIRC_DIV2, CLK_CLKDIV0_UART(1));

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set PB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPL &= ~(SYS_GPB_MFPL_PB0MFP_Msk | SYS_GPB_MFPL_PB1MFP_Msk);
    SYS->GPB_MFPL = SYS_GPB_MFPL_PB0MFP_UART0_RXD | SYS_GPB_MFPL_PB1MFP_UART0_TXD;

}


void UART0_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset IP */
    SYS->IPRST1 |=  SYS_IPRST1_UART0RST_Msk;
    SYS->IPRST1 &= ~SYS_IPRST1_UART0RST_Msk;

    /* Configure UART0 and set UART0 Baudrate */
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_MODE2_DIVIDER(__HIRC_DIV2, 115200);
    UART0->LINE = UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1;
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{
    /*
        This sample code demonstrate how to use HID interface to transfer data
        between PC and USB device.
        A demo window tool are also included in "WindowsTool" directory with this
        sample code. User can use it to test data transfer with this sample code.

    */

    /* Unlock write-protected registers */
    SYS_UnlockReg();

    /* Init system and multi-funcition I/O */
    SYS_Init();

    /* Init UART for debug message */
    UART0_Init();

    printf("\n");
    printf("+--------------------------------------------------------+\n");
    printf("|          NuMicro USB HID Transfer Sample Code          |\n");
    printf("+--------------------------------------------------------+\n");

    /* Open USB controller */
    USBD_Open(&gsInfo, HID_ClassRequest, NULL);

    /*Init Endpoint configuration for HID */
    HID_Init();

    /* Start USB device */
    USBD_Start();

    /* Enable USB device interrupt */
    NVIC_EnableIRQ(USBD_IRQn);

#if CRYSTAL_LESS
    /* Backup default trim value */
    s_u32DefaultTrim = M32(TRIM_INIT);
    s_u32LastTrim = s_u32DefaultTrim;

    /* Clear SOF */
    USBD_CLR_INT_FLAG(USBD_INTSTS_SOFIF_Msk);
#endif

    while (SYS->PDID)
    {
#if CRYSTAL_LESS

        /* Start USB trim function if it is not enabled. */
        if ((SYS->IRCTCTL & SYS_IRCTCTL_FREQSEL_Msk) != 0x2)
        {
            /* Start USB trim only when USB signal arrived */
            if (USBD->INTSTS & USBD_INTSTS_SOFIF_Msk)
            {
                /* Clear SOF */
                USBD_CLR_INT_FLAG(USBD_INTSTS_SOFIF_Msk);

                /* Enable USB clock trim function */
                SYS->IRCTCTL = HIRC_AUTO_TRIM;
            }
        }

        /* Disable USB Trim when any error found */
        if (SYS->IRCTISTS & (SYS_IRCTISTS_CLKERRIF_Msk | SYS_IRCTISTS_TFAILIF_Msk))
        {
            /* Last TRIM */
            M32(TRIM_INIT) = s_u32LastTrim;

            /* Disable USB clock trim function */
            SYS->IRCTCTL = 0;

            /* Clear trim error flags */
            SYS->IRCTISTS = SYS_IRCTISTS_CLKERRIF_Msk | SYS_IRCTISTS_TFAILIF_Msk;

            /* Clear SOF */
            USBD_CLR_INT_FLAG(USBD_INTSTS_SOFIF_Msk);

        }

        /* Check trim value whether it is over the threshold */
        if ((M32(TRIM_INIT) > (s_u32DefaultTrim + TRIM_THRESHOLD)) || (M32(TRIM_INIT) < (s_u32DefaultTrim - TRIM_THRESHOLD)))
        {
            /* Write updated value */
            M32(TRIM_INIT) = s_u32LastTrim;
        }
        else
        {
            /* Backup trim value */
            s_u32LastTrim =  M32(TRIM_INIT);
        }

#endif
    }
}



/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
