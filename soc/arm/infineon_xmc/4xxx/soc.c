/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2020 Linumiz
 * Author: Parthiban Nallathambi <parthiban@linumiz.com>
 *
 */

#include <kernel.h>
#include <init.h>
#include <soc.h>

#ifdef CONFIG_SOC_XMC4500
#define PMU_FLASH_WS		(0x3U)
#elif CONFIG_SOC_XMC4700
#define PMU_FLASH_WS		(0x4U)
#endif
void z_platform_init(void)
{
	uint32_t temp;
  /* __FPU_PRESENT = 1 defined in device header file */
  /* __FPU_USED value depends on compiler/linker options. */
  /* __FPU_USED = 0 if -mfloat-abi=soft is selected */
  /* __FPU_USED = 1 if -mfloat-abi=softfp or –mfloat-abi=hard */

#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
  SCB->CPACR |= ((3UL << 10*2) |                 /* set CP10 Full Access */
                 (3UL << 11*2)  );               /* set CP11 Full Access */
#else
  SCB->CPACR = 0;
#endif

  /* Enable unaligned memory access - SCB_CCR.UNALIGN_TRP = 0 */
  SCB->CCR &= ~(SCB_CCR_UNALIGN_TRP_Msk);

	/* setup flash wait state */
	temp = FLASH0->FCON;
	temp &= ~FLASH_FCON_WSPFLASH_Msk;
	temp |= PMU_FLASH_WS;
	FLASH0->FCON = temp;
  XMC_SCU_CLOCK_SetSleepConfig(XMC_SCU_CLOCK_SLEEP_MODE_CONFIG_SYSCLK_FPLL | XMC_SCU_CLOCK_SLEEP_MODE_CONFIG_ENABLE_ETH);
  
  // Make sure that SLEEPDEEP bit is cleared
  SCB->SCR &= ~ SCB_SCR_SLEEPDEEP_Msk;
	/* configure PLL & system clock */
	SystemCoreClockSetup();
}
