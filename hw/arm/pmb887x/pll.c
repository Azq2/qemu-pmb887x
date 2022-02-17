/*
 * PLL
 * */
#include "qemu/osdep.h"
#include "hw/sysbus.h"
#include "hw/hw.h"
#include "exec/address-spaces.h"
#include "exec/memory.h"
#include "hw/qdev-properties.h"
#include "cpu.h"
#include "qemu/timer.h"
#include "hw/ptimer.h"

#include "hw/arm/pmb887x/pll.h"
#include "hw/arm/pmb887x/regs.h"
#include "hw/arm/pmb887x/io_bridge.h"
#include "hw/arm/pmb887x/regs_dump.h"
#include "hw/arm/pmb887x/mod.h"

#define PLL_DEBUG

#ifdef PLL_DEBUG
#define DPRINTF(fmt, ...) do { fprintf(stderr, "[pmb887x-pll]: " fmt , ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) do { } while (0)
#endif

#define TYPE_PMB887X_PLL	"pmb887x-pll"
#define PMB887X_PLL(obj)	OBJECT_CHECK(struct pmb887x_pll_t, (obj), TYPE_PMB887X_PLL)

struct pmb887x_pll_callback_t {
	void *opaque;
	void (*callback)(void *);
};

struct pmb887x_pll_t {
	SysBusDevice parent_obj;
	MemoryRegion mmio;
	
	struct pmb887x_pll_callback_t *callbacks;
	int callbacks_count;
	
	struct pmb887x_src_reg_t src;
	qemu_irq irq;
	
	uint32_t xtal;
	uint32_t hw_ns_div;
	
	uint32_t fsys;
	uint32_t fstm;
	uint32_t fahb;
	uint32_t fcpu;
	
	uint32_t osc;
	uint32_t con0;
	uint32_t con1;
	uint32_t con2;
	uint32_t con3;
};

// Apply dividers for AHB freq
static uint32_t pll_ahb_div(uint32_t freq, uint32_t k1, uint32_t k2) {
	if (freq == 0)
		return 0;
	if (k1 == 0)
		return freq / 8;
	return (freq * 12) / ((k1 * 6) + (k2 >= 1 ? k2 - 1 : 0));
}

// Freq after PLL
static uint32_t pll_freq(struct pmb887x_pll_t *p) {
	// fPLL = fOSC * (NDIV + 1)
	uint32_t ndiv = (p->osc & PLL_OSC_NDIV) >> PLL_OSC_NDIV_SHIFT;
	return p->xtal * (ndiv + 1);
}

// Get AHB bus freq
static uint32_t pll_get_ahb_freq(struct pmb887x_pll_t *p) {
	uint32_t k1, k2;
	switch ((p->con1 & PLL_CON1_AHB_CLKSEL)) {
		case PLL_CON1_AHB_CLKSEL_BYPASS:
			// fAHB = fOSC
			return p->xtal;
		break;
		
		case PLL_CON1_AHB_CLKSEL_PLL0:
			// fAHB = fOSC * (NDIV + 1)
			return pll_freq(p);
		break;
		
		case PLL_CON1_AHB_CLKSEL_PLL1:
			// PLL1_K1 > 0:		fAHB = (fOSC * (NDIV + 1) * 12) / (PLL1_K1 * 6 + (PLL1_K2 - 1))
			// PLL1_K1 = 0:		fAHB = (fOSC * (NDIV + 1) * 12) / 16
			k1 = (p->con0 & PLL_CON0_PLL1_K1) >> PLL_CON0_PLL1_K1_SHIFT;
			k2 = (p->con0 & PLL_CON0_PLL1_K2) >> PLL_CON0_PLL1_K2_SHIFT;
			return pll_ahb_div(pll_freq(p), k1, k2);
		break;
		
		case PLL_CON1_AHB_CLKSEL_PLL2:
			// PLL2_K1 > 0:		fAHB = (fOSC * (NDIV + 1) * 12) / (PLL1_K2 * 6 + (PLL2_K2 - 1))
			// PLL2_K1 = 0:		fAHB = (fOSC * (NDIV + 1) * 12) / 16
			k1 = (p->con0 & PLL_CON0_PLL2_K1) >> PLL_CON0_PLL2_K1_SHIFT;
			k2 = (p->con0 & PLL_CON0_PLL2_K2) >> PLL_CON0_PLL2_K2_SHIFT;
			return pll_ahb_div(pll_freq(p), k1, k2);
		break;
		
		case PLL_CON1_AHB_CLKSEL_PLL3:
			// PLL3_K1 > 0:		fAHB = (fOSC * (NDIV + 1) * 12) / (PLL1_K3 * 6 + (PLL3_K2 - 1))
			// PLL3_K1 = 0:		fAHB = (fOSC * (NDIV + 1) * 12) / 16
			k1 = (p->con0 & PLL_CON0_PLL3_K1) >> PLL_CON0_PLL3_K1_SHIFT;
			k2 = (p->con0 & PLL_CON0_PLL3_K2) >> PLL_CON0_PLL3_K2_SHIFT;
			return pll_ahb_div(pll_freq(p), k1, k2);
		break;
		
		case PLL_CON1_AHB_CLKSEL_PLL4:
			// PLL4_K1 > 0:		fAHB = (fOSC * (NDIV + 1) * 12) / (PLL4_K1 * 6 + (PLL4_K2 - 1))
			// PLL4_K1 = 0:		fAHB = (fOSC * (NDIV + 1) * 12) / 16
			k1 = (p->con0 & PLL_CON0_PLL4_K1) >> PLL_CON0_PLL4_K1_SHIFT;
			k2 = (p->con0 & PLL_CON0_PLL4_K2) >> PLL_CON0_PLL4_K2_SHIFT;
			return pll_ahb_div(pll_freq(p), k1, k2);
		break;
	}
	return 0;
}

static uint32_t pll_get_sys_freq(struct pmb887x_pll_t *p) {
	uint32_t freq = pll_freq(p);
	uint32_t clksel = p->con1 & PLL_CON1_FSYS_CLKSEL;
	
	// fSYS=0
	if (clksel == PLL_CON1_FSYS_CLKSEL_DISABLE)
		return 0;
	
	if (clksel == PLL_CON1_FSYS_CLKSEL_PLL) {
		// fSYS = fPLL / 2
		return freq / 2;
	}
	
	// fSYS = fOSC
	return p->xtal;
}

static uint32_t pll_get_stm_freq(struct pmb887x_pll_t *p) {
	uint32_t freq = p->xtal;
	if ((p->con1 & PLL_CON1_FSTM_DIV_EN)) {
		uint32_t div = (PLL_CON1 & PLL_CON1_FSTM_DIV) >> PLL_CON1_FSTM_DIV_SHIFT;
		return freq / div;
	}
	return freq;
}

// CPU freq from AHB
static uint32_t pll_get_cpu_freq(struct pmb887x_pll_t *p) {
	uint32_t ahb_freq = pll_get_ahb_freq(p);
	if ((p->con2 & PLL_CON2_CPU_DIV_EN)) {
		// fCPU = fAHB / (CPU_DIV + 1)
		uint32_t div = ((p->con2 & PLL_CON2_CPU_DIV) >> PLL_CON2_CPU_DIV_SHIFT) + 1;
		return ahb_freq / div;
	}
	// fCPU = fAHB
	return ahb_freq;
}

static void pll_update_state(struct pmb887x_pll_t *p) {
	uint32_t new_fsys = pll_get_sys_freq(p);
	uint32_t new_fstm = pll_get_stm_freq(p);
	uint32_t new_fcpu = pll_get_cpu_freq(p);
	uint32_t new_fahb = pll_get_ahb_freq(p);
	
	bool is_changed = (
		new_fsys != p->fsys ||
		new_fstm != p->fstm ||
		new_fcpu != p->fcpu ||
		new_fahb != p->fahb
	);
	
	if (is_changed) {
		p->fsys = new_fsys;
		p->fstm = new_fstm;
		p->fcpu = new_fcpu;
		p->fahb = new_fahb;
		
		DPRINTF("fCPU: %d Hz, fAHB: %d Hz, fSYS: %d Hz, fSTM: %d Hz\n", p->fcpu, p->fahb, p->fsys, p->fstm);
		
		for (int i = 0; i < p->callbacks_count; ++i)
			p->callbacks[i].callback(p->callbacks[i].opaque);
	}
}

static uint64_t pll_io_read(void *opaque, hwaddr haddr, unsigned size) {
	struct pmb887x_pll_t *p = (struct pmb887x_pll_t *) opaque;
	
	uint64_t value = 0;
	
	switch (haddr) {
		case PLL_OSC:
			value = p->osc;
		break;
		
		case PLL_CON0:
			value = p->con0;
		break;
		
		case PLL_CON1:
			value = p->con1;
		break;
		
		case PLL_CON2:
			value = p->con2;
		break;
		
		case PLL_STAT:
			value = PLL_STAT_LOCK;
		break;
		
		case PLL_CON3:
			value = p->con3;
		break;
		
		case PLL_SRC:
			value = pmb887x_src_get(&p->src);
		break;
		
		default:
			pmb887x_dump_io(haddr + p->mmio.addr, size, 0xFFFFFFFF, false);
			DPRINTF("unknown reg access: %02lX\n", haddr);
			exit(1);
		break;
	}
	
	pmb887x_dump_io(haddr + p->mmio.addr, size, value, false);
	
	return value;
}

static void pll_io_write(void *opaque, hwaddr haddr, uint64_t value, unsigned size) {
	struct pmb887x_pll_t *p = (struct pmb887x_pll_t *) opaque;
	
	pmb887x_dump_io(haddr + p->mmio.addr, size, value, true);
	
	switch (haddr) {
		case PLL_OSC:
			p->osc = value;
		break;
		
		case PLL_CON0:
			p->con0 = value;
		break;
		
		case PLL_CON1:
			p->con1 = value;
		break;
		
		case PLL_CON2:
			p->con2 = value;
		break;
		
		case PLL_CON3:
			p->con3 = value;
		break;
		
		case PLL_SRC:
			pmb887x_src_set(&p->src, value);
		break;
		
		default:
			DPRINTF("unknown reg access: %02lX\n", haddr);
			exit(1);
		break;
	}
	
	pll_update_state(p);
}

static const MemoryRegionOps io_ops = {
	.read			= pll_io_read,
	.write			= pll_io_write,
	.endianness		= DEVICE_NATIVE_ENDIAN,
	.valid			= {
		.min_access_size	= 1,
		.max_access_size	= 4
	}
};

uint64_t pmb887x_pll_get_hw_ns(struct pmb887x_pll_t *p) {
	return qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) / p->hw_ns_div;
}

uint64_t pmb887x_pll_get_real_ns(struct pmb887x_pll_t *p) {
	return qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
}

uint64_t pmb887x_pll_hw_to_real_ns(struct pmb887x_pll_t *p, uint64_t hw_ns) {
	return hw_ns * p->hw_ns_div;
}

uint32_t pmb887x_pll_get_fsys(struct pmb887x_pll_t *p) {
	return p->fsys;
}

uint32_t pmb887x_pll_get_fstm(struct pmb887x_pll_t *p) {
	return p->fstm;
}

uint32_t pmb887x_pll_get_fcpu(struct pmb887x_pll_t *p) {
	return p->fcpu;
}

uint32_t pmb887x_pll_get_fahb(struct pmb887x_pll_t *p) {
	return p->fahb;
}

void pmb887x_pll_add_freq_update_callback(struct pmb887x_pll_t *p, void (*callback)(void *), void *opaque) {
	p->callbacks = g_realloc(p->callbacks, (p->callbacks_count + 1) * sizeof(struct pmb887x_pll_callback_t));
	p->callbacks[p->callbacks_count].opaque = opaque;
	p->callbacks[p->callbacks_count].callback = callback;
	p->callbacks_count++;
}

static void pll_init(Object *obj) {
	struct pmb887x_pll_t *p = PMB887X_PLL(obj);
	memory_region_init_io(&p->mmio, obj, &io_ops, p, "pmb887x-pll", PLL_IO_SIZE);
	sysbus_init_mmio(SYS_BUS_DEVICE(obj), &p->mmio);
	sysbus_init_irq(SYS_BUS_DEVICE(obj), &p->irq);
}

static void pll_realize(DeviceState *dev, Error **errp) {
	struct pmb887x_pll_t *p = PMB887X_PLL(dev);
	
	if (!p->irq) {
		error_report("pmb887x-pll: irq not set");
		abort();
	}
	
	pmb887x_src_init(&p->src, p->irq);
	pmb887x_src_set(&p->src, MOD_SRC_SRE);
	
	p->fsys = p->xtal;
	
	p->callbacks = NULL;
	p->callbacks_count = 0;
	
	// Initial values
	p->osc	= 0x01070001;
	p->con0	= 0x22000012;
	p->con1	= 0x00000000;
	p->con2	= 0x00000000;
	p->con3	= 0x00000000;
	
	pll_update_state(p);
}

static Property pll_properties[] = {
	DEFINE_PROP_UINT32("xtal", struct pmb887x_pll_t, xtal, 26000000),
	DEFINE_PROP_UINT32("hw-ns-throttle", struct pmb887x_pll_t, hw_ns_div, 1),
    DEFINE_PROP_END_OF_LIST(),
};

static void pll_class_init(ObjectClass *klass, void *data) {
	DeviceClass *dc = DEVICE_CLASS(klass);
	device_class_set_props(dc, pll_properties);
	dc->realize = pll_realize;
}

static const TypeInfo pll_info = {
    .name          	= TYPE_PMB887X_PLL,
    .parent        	= TYPE_SYS_BUS_DEVICE,
    .instance_size 	= sizeof(struct pmb887x_pll_t),
    .instance_init 	= pll_init,
    .class_init    	= pll_class_init,
};

static void pll_register_types(void) {
	type_register_static(&pll_info);
}
type_init(pll_register_types)