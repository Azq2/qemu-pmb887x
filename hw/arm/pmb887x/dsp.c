/*
 * DSP
 * */
#define PMB887X_TRACE_ID		DSP
#define PMB887X_TRACE_PREFIX	"pmb887x-dsp"

#include "qemu/osdep.h"
#include "hw/sysbus.h"
#include "hw/hw.h"
#include "hw/ptimer.h"
#include "exec/address-spaces.h"
#include "exec/memory.h"
#include "cpu.h"
#include "qapi/error.h"
#include "qemu/timer.h"
#include "qemu/main-loop.h"
#include "hw/qdev-properties.h"
#include "qapi/error.h"

#include "hw/arm/pmb887x/pll.h"
#include "hw/arm/pmb887x/regs.h"
#include "hw/arm/pmb887x/io_bridge.h"
#include "hw/arm/pmb887x/regs_dump.h"
#include "hw/arm/pmb887x/mod.h"
#include "hw/arm/pmb887x/trace.h"

#define DSP_RAM_SIZE		0x1000
#define TYPE_PMB887X_DSP	"pmb887x-dsp"
#define PMB887X_DSP(obj)	OBJECT_CHECK(struct pmb887x_dsp_t, (obj), TYPE_PMB887X_DSP)

struct pmb887x_dsp_t {
	SysBusDevice parent_obj;
	MemoryRegion mmio;
	
	uint32_t unk[2];
	uint8_t ram[DSP_RAM_SIZE];
	
	pmb887x_clc_reg_t clc;
};

static void dsp_update_state(struct pmb887x_dsp_t *p) {
	// TODO
}

static uint32_t dsp_ram_read(struct pmb887x_dsp_t *p, uint32_t offset, unsigned size) {
	uint8_t *data = p->ram;
	switch (size) {
		case 1:		return data[offset];
		case 2:		return data[offset] | (data[offset + 1] << 8);
		case 4:		return data[offset] | (data[offset + 1] << 8) | (data[offset + 2] << 16) | (data[offset + 3] << 24);
	}
    return 0;
}

static void dsp_ram_write(struct pmb887x_dsp_t *p, uint32_t offset, uint32_t value, unsigned size) {
	uint8_t *data = p->ram;
	switch (size) {
		case 1:
			data[offset] = value & 0xFF;
		break;
		
		case 2:
			data[offset] = value & 0xFF;
			data[offset + 1] = (value >> 8) & 0xFF;
		break;
		
		case 4:
			data[offset] = value & 0xFF;
			data[offset + 1] = (value >> 8) & 0xFF;
			data[offset + 2] = (value >> 16) & 0xFF;
			data[offset + 3] = (value >> 24) & 0xFF;
		break;
	}
}

static uint64_t dsp_io_read(void *opaque, hwaddr haddr, unsigned size) {
	struct pmb887x_dsp_t *p = (struct pmb887x_dsp_t *) opaque;
	
	uint64_t value = 0;
	
	switch (haddr) {
		case DSP_CLC:
			value = pmb887x_clc_get(&p->clc);
		break;
		
		case DSP_ID:
			value = 0xF022C031;
		break;
		
		case DSP_UNK0:
			value = p->unk[0];
		break;
		
		case DSP_UNK1:
			value = p->unk[1];
		break;
		
		case DSP_RAM0 ... (DSP_RAM0 + DSP_RAM_SIZE):
			value = dsp_ram_read(p, haddr - DSP_RAM0, size);
		break;
		
		default:
			IO_DUMP(haddr + p->mmio.addr, size, 0xFFFFFFFF, false);
			EPRINTF("unknown reg access: %02"PRIX64"\n", haddr);
			//exit(1);
		break;
	}
	
	IO_DUMP(haddr + p->mmio.addr, size, value, false);
	
	return value;
}

static void dsp_io_write(void *opaque, hwaddr haddr, uint64_t value, unsigned size) {
	struct pmb887x_dsp_t *p = (struct pmb887x_dsp_t *) opaque;
	
	IO_DUMP(haddr + p->mmio.addr, size, value, true);
	
	switch (haddr) {
		case DSP_CLC:
			pmb887x_clc_set(&p->clc, value);
		break;
		
		case DSP_UNK0:
			p->unk[0] = value;
		break;
		
		case DSP_UNK1:
			p->unk[1] = value;
		break;
		
		case DSP_RAM0 ... (DSP_RAM0 + DSP_RAM_SIZE):
			dsp_ram_write(p, haddr - DSP_RAM0, value, size);
		break;
		
		default:
			EPRINTF("unknown reg access: %02"PRIX64"\n", haddr);
			//exit(1);
		break;
	}
	
	dsp_update_state(p);
}

static const MemoryRegionOps io_ops = {
	.read			= dsp_io_read,
	.write			= dsp_io_write,
	.endianness		= DEVICE_NATIVE_ENDIAN,
	.valid			= {
		.min_access_size	= 1,
		.max_access_size	= 4
	}
};

static void dsp_init(Object *obj) {
	struct pmb887x_dsp_t *p = PMB887X_DSP(obj);
	memory_region_init_io(&p->mmio, obj, &io_ops, p, "pmb887x-dsp", DSP_IO_SIZE);
	sysbus_init_mmio(SYS_BUS_DEVICE(obj), &p->mmio);
}

static void dsp_realize(DeviceState *dev, Error **errp) {
	struct pmb887x_dsp_t *p = PMB887X_DSP(dev);
	
	pmb887x_clc_init(&p->clc);
	
	p->unk[0] = 0x01;
	p->unk[1] = 0x00;
	
	dsp_ram_write(p, 0, 0x0801, 2);
	
	dsp_update_state(p);
}

static Property dsp_properties[] = {
    DEFINE_PROP_END_OF_LIST(),
};

static void dsp_class_init(ObjectClass *klass, void *data) {
	DeviceClass *dc = DEVICE_CLASS(klass);
	device_class_set_props(dc, dsp_properties);
	dc->realize = dsp_realize;
}

static const TypeInfo dsp_info = {
    .name          	= TYPE_PMB887X_DSP,
    .parent        	= TYPE_SYS_BUS_DEVICE,
    .instance_size 	= sizeof(struct pmb887x_dsp_t),
    .instance_init 	= dsp_init,
    .class_init    	= dsp_class_init,
};

static void dsp_register_types(void) {
	type_register_static(&dsp_info);
}
type_init(dsp_register_types)
