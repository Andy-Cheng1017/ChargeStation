/***************************************************************************//**
 * @file     main.c
 * @version  V3.00

 * @brief    Show how to wake up system from Power-down mode
 *           by USCI interrupt in UART mode.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "stdio.h"
#include "NuMicro.h"


/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void);
void USCI_UART_DataWakeUp(void);
void USCI_UART_CTSWakeUp(void);
void USCI_UART_PowerDown_TestItem(void);
void USCI_UART_PowerDownWakeUpTest(void);


void PowerDownFunction(void)
{
    /* Check if all the debug messages are finished */
    UART_WAIT_TX_EMPTY(UART0);

    /* Enter to Power-down mode */
    CLK_PowerDown();
}

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

    /* Enable UART and USCI module clock */
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(USCI0_MODULE);

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

    /* Set PB multi-function pins for USCI0_DAT0(PB.4), USCI0_DAT1(PB.5), and USCI0_CTL0(PB.7) */
    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB4MFP_Msk)) | SYS_GPB_MFPL_PB4MFP_USCI0_DAT0;
    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB5MFP_Msk)) | SYS_GPB_MFPL_PB5MFP_USCI0_DAT1;
    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB7MFP_Msk)) | SYS_GPB_MFPL_PB7MFP_USCI0_CTL0;
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

void USCI0_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init USCI                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset USCI0 */
    SYS_ResetModule(USCI0_RST);

    /* Configure USCI0 as UART mode */
    UUART_Open(UUART0, 9600);
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

    /* Init USCI0 for test */
    USCI0_Init();

    /*---------------------------------------------------------------------------------------------------------*/
    /* SAMPLE CODE                                                                                             */
    /*---------------------------------------------------------------------------------------------------------*/

    printf("\n\nCPU @ %d Hz\n", SystemCoreClock);

    printf("\nUSCI UART Sample Program\n");

    /* USCI UART Power-down and Wake-up sample function */
    USCI_UART_PowerDownWakeUpTest();

    while (1);

}

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle USCI interrupt event                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
void USCI_IRQHandler(void)
{
    uint32_t u32IntSts = UUART_GET_PROT_STATUS(UUART0);
    uint32_t u32WkSts = UUART_GET_WAKEUP_FLAG(UUART0);

    if (u32WkSts & UUART_WKSTS_WKF_Msk)   /* USCI UART wake-up flag */
    {
        UUART_CLR_WAKEUP_FLAG(UUART0);
        printf("USCI UART wake-up.\n");
    }
    else if (u32IntSts & UUART_PROTSTS_RXENDIF_Msk)     /* USCI UART receive end interrupt flag */
    {
        UUART_CLR_PROT_INT_FLAG(UUART0, UUART_PROTSTS_RXENDIF_Msk);

        while (UUART_GET_RX_EMPTY(UUART0) == 0)
        {
            printf("Data: 0x%X\n", UUART_READ(UUART0));
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  UART nCTS Wake-up Function                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void USCI_UART_CTSWakeUp(void)
{
    /* Enable UART nCTS wake-up function */
    UUART_EnableWakeup(UUART0, UUART_PROTCTL_CTSWKEN_Msk);
    printf("System enter to Power-down mode.\n");
    printf("Toggle USCI-UART0 nCTS to wake-up system.\n\n");
}

/*---------------------------------------------------------------------------------------------------------*/
/*  UART Data Wake-up Function                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void USCI_UART_DataWakeUp(void)
{
    /* Enable UART data wake-up function */
    UUART_EnableWakeup(UUART0, UUART_PROTCTL_DATWKEN_Msk);

    /* Set UART data wake-up counter */
    UUART0->PROTCTL = (UUART0->PROTCTL & (~UUART_PROTCTL_WAKECNT_Msk)) | (4 << UUART_PROTCTL_WAKECNT_Pos);

    printf("System enter to Power-down mode.\n");
    printf("Send data with baud rate 9600bps to USCI-UART0 to wake-up system.\n\n");
}

/*---------------------------------------------------------------------------------------------------------*/
/*  UART Power-down and Wake-up Menu                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void USCI_UART_PowerDown_TestItem(void)
{
    printf("+-----------------------------------------------------------+\n");
    printf("|  USCI-UART Power-down and wake-up test                    |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| [1] nCTS wake-up test                                     |\n");
    printf("| [2] Data wake-up test                                     |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| Quit                                              - [ESC] |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("Please Select key (1~2): ");
}

/*---------------------------------------------------------------------------------------------------------*/
/*  UART Power-down and Wake-up Test Function                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void USCI_UART_PowerDownWakeUpTest(void)
{
    uint32_t u32Item;

    /* Enable UART receive end interrupt */
    UUART_ENABLE_TRANS_INT(UUART0, UUART_INTEN_RXENDIEN_Msk);
    NVIC_EnableIRQ(USCI_IRQn);

    USCI_UART_PowerDown_TestItem();
    u32Item = getchar();
    printf("%c\n\n", u32Item);

    switch (u32Item)
    {
        case '1':
            USCI_UART_CTSWakeUp();
            break;

        case '2':
            USCI_UART_DataWakeUp();
            break;

        default:
            break;
    }

    /* Unlock protected registers before entering Power-down mode */
    SYS_UnlockReg();

    /* Enter to Power-down mode */
    PowerDownFunction();

    /* Lock protected registers after entering Power-down mode */
    SYS_LockReg();

    printf("Enter any key to end test.\n\n");
    getchar();

    /* Disable UART wake-up function */
    UUART_DisableWakeup(UUART0);

    /* Disble UART receive end interrupt */
    UUART_DISABLE_TRANS_INT(UUART0, UUART_INTEN_RXENDIEN_Msk);
    NVIC_DisableIRQ(USCI_IRQn);

    printf("USCI UART Sample Program End.\n");
}
