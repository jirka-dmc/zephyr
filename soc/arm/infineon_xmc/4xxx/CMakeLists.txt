# SPDX-License-Identifier: Apache-2.0
#
# Copyright (c) 2020 Linumiz
# Author: Parthiban Nallathambi <parthiban@linumiz.com>

zephyr_sources(soc.c)
zephyr_sources(xmc_4xxx_usic.c)
zephyr_linker_sources(NOINIT noinit.ld)

zephyr_linker_sources_ifdef(CONFIG_XMC4XXX_CCM
  SECTIONS
  ccm.ld
  )
