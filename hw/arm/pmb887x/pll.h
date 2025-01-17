#pragma once
#include "qemu/osdep.h"

struct pmb887x_pll_t;

void pmb887x_pll_add_freq_update_callback(struct pmb887x_pll_t *p, void (*callback)(void *), void *opaque);

struct pmb887x_pll_t *pmb887x_pll_get_self(DeviceState *dev);
uint32_t pmb887x_pll_get_fosc(struct pmb887x_pll_t *p);
uint32_t pmb887x_pll_get_frtc(struct pmb887x_pll_t *p);
uint32_t pmb887x_pll_get_fsys(struct pmb887x_pll_t *p);
uint32_t pmb887x_pll_get_fstm(struct pmb887x_pll_t *p);
uint32_t pmb887x_pll_get_fcpu(struct pmb887x_pll_t *p);
uint32_t pmb887x_pll_get_fahb(struct pmb887x_pll_t *p);
uint32_t pmb887x_pll_get_fgptu(struct pmb887x_pll_t *p);
