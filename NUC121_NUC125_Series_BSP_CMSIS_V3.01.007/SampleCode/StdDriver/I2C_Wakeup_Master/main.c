/**************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * @brief
 *           Show how to wake up I2C slave from Power-down mode.
 *           This sample code needs to work with I2C_Wakeup_Slave.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
volatile uint8_t g_u8DeviceAddr;
volatile uint8_t g_au8MstTxData[3];
volatile uint8_t g_u8MstRxData;
volatile uint8_t g_u8MstDataLen;
volatile uint8_t g_u8MstEndFlag = 0;

typedef void (*I2C_FUNC)(uint32_t u32Status);

static volatile I2C_FUNC s_I2C0HandlerFn = NULL;

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C0 IRQ Handler                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void I2C0_IRQHandler(void)
{
    uint32_t u32Status;

    u32Status = I2C_GetStatus(I2C0);

    if (I2C_GET_TIMEOUT_FLAG(I2C0))
    {
        /* Clear I2C0 Timeout Flag */
        I2C_ClearTimeoutFlag(I2C0);
    }
    else
    {
        if (s_I2C0HandlerFn != NULL)
            s_I2C0HandlerFn(u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C Master Tx Wake Up Callback Function                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_MasterTxWakeup(uint32_t u32Status)
{
    if (u32Status == 0x08)                      /* START has been transmitted */
    {
        I2C_SetData(I2C0, (g_u8DeviceAddr << 1));    /* Write SLA+W to Register I2CDAT */
        //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
        I2C_Trigger(I2C0, 0, 0, 1, 0);
    }
    else if (u32Status == 0x18)                 /* SLA+W has been transmitted and ACK has been received */
    {
        //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_STO_SI);
        I2C_Trigger(I2C0, 0, 1, 1, 0);
        g_u8MstEndFlag = 1;
    }
    else if (u32Status == 0x20)                 /* SLA+W has been transmitted and NOT ACK has been received */
    {
        //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_STO_SI);
        I2C_Trigger(I2C0, 0, 1, 1, 0);
        g_u8MstEndFlag = 1;
    }
    else
    {
        /* TO DO */
        printf("Status 0x%x is NOT processed\n", u32Status);
    }
}
/*---------------------------------------------------------------------------------------------------------*/
/*  I2C Rx Callback Function                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_MasterRx(uint32_t u32Status)
{
    if (u32Status == 0x08)                      /* START has been transmitted and prepare SLA+W */
    {
        I2C_SetData(I2C0, (g_u8DeviceAddr << 1));    /* Write SLA+W to Register I2CDAT */
        //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
        I2C_Trigger(I2C0, 0, 0, 1, 0);
    }
    else if (u32Status == 0x18)                 /* SLA+W has been transmitted and ACK has been received */
    {
        I2C_SetData(I2C0, g_au8MstTxData[g_u8MstDataLen++]);
        //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
        I2C_Trigger(I2C0, 0, 0, 1, 0);
    }
    else if (u32Status == 0x20)                 /* SLA+W has been transmitted and NACK has been received */
    {
        I2C_STOP(I2C0);
        I2C_START(I2C0);
    }
    else if (u32Status == 0x28)                 /* DATA has been transmitted and ACK has been received */
    {
        if (g_u8MstDataLen != 2)
        {
            I2C_SetData(I2C0, g_au8MstTxData[g_u8MstDataLen++]);
            //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
            I2C_Trigger(I2C0, 0, 0, 1, 0);
        }
        else
        {
            //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_STA_SI);
            I2C_Trigger(I2C0, 1, 0, 1, 0);
        }
    }
    else if (u32Status == 0x10)                 /* Repeat START has been transmitted and prepare SLA+R */
    {
        I2C_SetData(I2C0, ((g_u8DeviceAddr << 1) | 0x01)); /* Write SLA+R to Register I2CDAT */
        //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
        I2C_Trigger(I2C0, 0, 0, 1, 0);
    }
    else if (u32Status == 0x40)                 /* SLA+R has been transmitted and ACK has been received */
    {
        //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
        I2C_Trigger(I2C0, 0, 0, 1, 0);
    }
    else if (u32Status == 0x58)                 /* DATA has been received and NACK has been returned */
    {
        g_u8MstRxData = (unsigned char) I2C_GetData(I2C0);
        //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_STO_SI);
        I2C_Trigger(I2C0, 0, 1, 1, 0);
        g_u8MstEndFlag = 1;
    }
    else
    {
        /* TO DO */
        printf("Status 0x%x is NOT processed\n", u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C Tx Callback Function                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_MasterTx(uint32_t u32Status)
{
    if (u32Status == 0x08)                      /* START has been transmitted */
    {
        I2C_SetData(I2C0, g_u8DeviceAddr << 1);    /* Write SLA+W to Register I2CDAT */
        //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
        I2C_Trigger(I2C0, 0, 0, 1, 0);
    }
    else if (u32Status == 0x18)                 /* SLA+W has been transmitted and ACK has been received */
    {
        I2C_SetData(I2C0, g_au8MstTxData[g_u8MstDataLen++]);
        //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
        I2C_Trigger(I2C0, 0, 0, 1, 0);
    }
    else if (u32Status == 0x20)                 /* SLA+W has been transmitted and NACK has been received */
    {
        I2C_STOP(I2C0);
        I2C_START(I2C0);
    }
    else if (u32Status == 0x28)                 /* DATA has been transmitted and ACK has been received */
    {
        if (g_u8MstDataLen != 3)
        {
            I2C_SetData(I2C0, g_au8MstTxData[g_u8MstDataLen++]);
            //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI);
            I2C_Trigger(I2C0, 0, 0, 1, 0);
        }
        else
        {
            //I2C_SET_CONTROL_REG(I2C0, I2C_CTL_STO_SI);
            I2C_Trigger(I2C0, 0, 1, 1, 0);
            g_u8MstEndFlag = 1;
        }
    }
    else
    {
        /* TO DO */
        printf("Status 0x%x is NOT processed\n", u32Status);
    }
}

void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable Internal RC 48MHz clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Enable I2C0 module clock */
    CLK_EnableModuleClock(I2C0_MODULE);

    /* Select UART module clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UARTSEL_HIRC_DIV2, CLK_CLKDIV0_UART(1));

    /* Update core clock */
    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set PB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPL = SYS_GPB_MFPL_PB0MFP_UART0_RXD | SYS_GPB_MFPL_PB1MFP_UART0_TXD;

    /* Set I2C0 PC multi-function pins */
    SYS->GPC_MFPH &= ~(SYS_GPC_MFPH_PC11MFP_Msk | SYS_GPC_MFPH_PC12MFP_Msk);
    SYS->GPC_MFPH |= (SYS_GPC_MFPH_PC11MFP_I2C0_SDA | SYS_GPC_MFPH_PC12MFP_I2C0_SCL);

    /* I2C pins enable schmitt trigger */
    PC->SMTEN |= GPIO_SMTEN_SMTEN11_Msk | GPIO_SMTEN_SMTEN12_Msk;

}

void UART0_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset IP */
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 Baudrate */
    UART_Open(UART0, 115200);
}

void I2C0_Init(void)
{
    /* Open I2C module and set bus clock */
    I2C_Open(I2C0, 100000);

    /* Get I2C0 Bus Clock */
    printf("I2C clock %d Hz\n", I2C_GetBusClockFreq(I2C0));

    /* Enable I2C interrupt */
    I2C_EnableInt(I2C0);

    NVIC_EnableIRQ(I2C0_IRQn);
}

void I2C0_Close(void)
{
    /* Disable I2C0 interrupt and clear corresponding NVIC bit */
    I2C_DisableInt(I2C0);
    NVIC_DisableIRQ(I2C0_IRQn);

    /* Disable I2C0 and close I2C0 clock */
    I2C_Close(I2C0);
    CLK_DisableModuleClock(I2C0_MODULE);

}

int32_t I2C0_ReadWriteSlave(uint8_t u8SlvAddr)
{
    uint32_t u32Index;

    g_u8DeviceAddr = u8SlvAddr;

    for (u32Index = 0; u32Index < 0x100; u32Index++)
    {
        g_au8MstTxData[0] = (uint8_t)((u32Index & 0xFF00) >> 8);
        g_au8MstTxData[1] = (uint8_t)(u32Index & 0x00FF);
        g_au8MstTxData[2] = (uint8_t)(g_au8MstTxData[1] + 3);

        g_u8MstDataLen = 0;
        g_u8MstEndFlag = 0;

        /* I2C function to write data to slave */
        s_I2C0HandlerFn = (I2C_FUNC)I2C_MasterTx;

        /* I2C as master sends START signal */
        I2C_START(I2C0);

        /* Wait I2C Tx Finish */
        while (g_u8MstEndFlag == 0);

        g_u8MstEndFlag = 0;

        /* I2C function to read data from slave */
        s_I2C0HandlerFn = (I2C_FUNC)I2C_MasterRx;

        g_u8MstDataLen = 0;
        g_u8DeviceAddr = u8SlvAddr;

        I2C_START(I2C0);

        /* Wait I2C Rx Finish */
        while (g_u8MstEndFlag == 0);

        /* Compare data */
        if (g_u8MstRxData != g_au8MstTxData[2])
        {
            printf("I2C Byte Write/Read Failed, Data 0x%x\n", g_u8MstRxData);
            return -1;
        }
    }

    printf("Master Access Slave (0x%X) Test OK\n", u8SlvAddr);
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, IP clock and multi-function I/O */
    SYS_Init();

    /* Init UART0 for printf */
    UART0_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /*
        This sample code sets I2C bus clock to 100kHz. After wake-up, then accesses Slave with Byte
        Write and Byte Read operations, and check if the read data is equal to the programmed data.
    */

    printf("+------------------------------------------------------------------------+\n");
    printf("| I2C Driver Sample Code (Master) for wake-up & access Slave test        |\n");
    printf("|                                                                        |\n");
    printf("| I2C Master (I2C0) <---> I2C Slave(I2C0)                                |\n");
    printf("+------------------------------------------------------------------------+\n");

    printf("Configure I2C0 as a master.\n");
    printf("The I/O connection for I2C0:\n");
    printf("I2C0_SDA(PC.11), I2C0_SCL(PC.12)\n");

    /* Init I2C0 to access Slave */
    I2C0_Init();

    printf("\n");
    printf("Check I2C slave at power down status.\n");
    printf("Press any key to Wake up slave.\n");
    getchar();

    /* Set the Slave address to wake-up*/
    g_u8DeviceAddr = 0x15;

    /* I2C function to wake-up slave*/
    s_I2C0HandlerFn = (I2C_FUNC)I2C_MasterTxWakeup;


    /* Send a START condition to bus */
    I2C_SET_CONTROL_REG(I2C0, I2C_CTL_STA);

    while (g_u8MstEndFlag == 0);

    /*Access to the corresponding address Slave*/
    printf("\n");
    printf(" == No Mask Address ==\n");
    I2C0_ReadWriteSlave(0x15);
    I2C0_ReadWriteSlave(0x35);
    I2C0_ReadWriteSlave(0x55);
    I2C0_ReadWriteSlave(0x75);
    printf("SLAVE Address test OK.\n");


    /* Access Slave with address mask */
    printf("\n");
    printf(" == Mask Address ==\n");
    I2C0_ReadWriteSlave(0x15 & ~0x01);
    I2C0_ReadWriteSlave(0x35 & ~0x04);
    I2C0_ReadWriteSlave(0x55 & ~0x01);
    I2C0_ReadWriteSlave(0x75 & ~0x04);
    printf("SLAVE Address Mask test OK.\n");

    s_I2C0HandlerFn = NULL;

    /* Close I2C0 */
    I2C0_Close();

    while (1);
}