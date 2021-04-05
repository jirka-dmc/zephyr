/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2020 Linumiz
 * Author: Parthiban Nallathambi <parthiban@linumiz.com>
 *
 */

/* Add include for DTS generated information */
#include <devicetree.h>
#ifdef CONFIG_SOC_XMC4500
    #include <system_XMC4500.h>
    #include <XMC4500.h>
#elif CONFIG_SOC_XMC4700
    #include <system_XMC4700.h>
    #include <XMC4700.h>
    #include <xmc_scu.h>
#endif

