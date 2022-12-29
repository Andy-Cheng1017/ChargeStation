#include <stdio.h>
#include "NuMicro.h"

uint32_t analogRead(uint32_t u32Channel)
{
    uint32_t u32ConversionData;

    /* Select conversion channel */
    ADC_SET_INPUT_CHANNEL(ADC, 0x1UL << u32Channel);

    /* Trigger sample module to start A/D conversion */
    ADC_START_CONV(ADC);

    /* Wait ADC conversion done */
    while(ADC_IS_DATA_VALID(ADC, u32Channel) == 0);

    /* Get the conversion result of specific channel */
    u32ConversionData = ADC_GET_CONVERSION_DATA(ADC, u32Channel);

    /* Turn off the channel */
    ADC_SET_INPUT_CHANNEL(ADC, 0);

    /* Return the conversion data */
    return u32ConversionData;
}
