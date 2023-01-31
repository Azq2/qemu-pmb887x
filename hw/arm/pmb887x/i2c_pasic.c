/*
 * Dialog D1601XX or similar Power ASIC IC
 * */
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
#include "hw/i2c/i2c.h"
#include "hw/arm/pmb887x/trace.h"

#ifdef PMB887X_PASIC_DEBUG
#define DPRINTF(fmt, ...) do { qemu_log_mask(LOG_TRACE, "[pmb887x-pasic]: " fmt , ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) do { } while (0)
#endif

#define TYPE_PMB887X_PASIC	"pmb887x-pasic"
#define PMB887X_PASIC(obj)	OBJECT_CHECK(pmb887x_pasic_t, (obj), TYPE_PMB887X_PASIC)

typedef struct {
	I2CSlave parent_obj;
	int reg_id;
	int wcycle;
	uint8_t regs[0xFF];
	uint32_t revision;
} pmb887x_pasic_t;

static const uint8_t regs_d1601xx[256] = {
	0x85, 0x01, 0x00, 0x00, 0x00, 0x02, 0x2F, 0x09, 0x00, 0xFF, 0x06, 0x01, 0x06, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x85, 0x04,
	0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2F, 0x09, 0x00, 0xFF, 0x06, 0x01, 0x06, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x85, 0x04,
	0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t regs_d1094xx[256] = {
	0x94, 0x01, 0x00, 0x00, 0x00, 0x00, 0x2F, 0x09, 0x00, 0xFF, 0x0E, 0x01, 0x06, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAB, 0x40, 0x0C,
	0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x94, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2F, 0x09, 0x00, 0xFF, 0x0E, 0x01, 0x06, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAB, 0x40, 0x0C,
	0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static int pasic_event(I2CSlave *s, enum i2c_event event) {
	pmb887x_pasic_t *p = PMB887X_PASIC(s);

    switch (event) {
		case I2C_START_SEND:
			// Nothing
		break;
		
		case I2C_START_RECV:
			p->wcycle = 0;
		break;
		
		case I2C_NACK:
			p->wcycle = 0;
		break;
		
		case I2C_FINISH:
			// Nothing
		break;
		
		case I2C_START_SEND_ASYNC:
			// Nothing
		break;
	}
    
    return 0;
}

static uint8_t pasic_recv(I2CSlave *s) {
	pmb887x_pasic_t *p = PMB887X_PASIC(s);
	
	uint8_t data = p->regs[p->reg_id];
	DPRINTF("read reg %02X: %02X\n", p->reg_id, data);
	p->reg_id = (p->reg_id + 1) % ARRAY_SIZE(p->regs);
	
	return data;
}

static int pasic_send(I2CSlave *s, uint8_t data) {
	pmb887x_pasic_t *p = PMB887X_PASIC(s);
	
	if (p->wcycle == 0) {
		p->reg_id = data % ARRAY_SIZE(p->regs);
	} else {
		p->reg_id = (p->reg_id + 1) % ARRAY_SIZE(p->regs);
		DPRINTF("write reg %02X: %02X\n", p->reg_id, data);
	}
	
	p->wcycle++;
	
    return 0;
}

static void pasic_realize(DeviceState *dev, Error **errp) {
	pmb887x_pasic_t *p = PMB887X_PASIC(dev);
	
	if (p->revision == 1601) {
		memcpy(p->regs, regs_d1601xx, sizeof(regs_d1601xx));
	} else if (p->revision == 1094) {
		memcpy(p->regs, regs_d1094xx, sizeof(regs_d1094xx));
	} else {
		hw_error("pmb887x-pasic: unknown revision %d", p->revision);
	}
}

static Property pasic_properties[] = {
	DEFINE_PROP_UINT32("revision", pmb887x_pasic_t, revision, 1601),
    DEFINE_PROP_END_OF_LIST(),
};

static void pasic_class_init(ObjectClass *klass, void *data) {
	DeviceClass *dc = DEVICE_CLASS(klass);
	device_class_set_props(dc, pasic_properties);
	dc->realize = pasic_realize;
	
    I2CSlaveClass *k = I2C_SLAVE_CLASS(klass);
    k->event = &pasic_event;
    k->recv = &pasic_recv;
    k->send = &pasic_send;
}

static const TypeInfo pasic_info = {
    .name          	= TYPE_PMB887X_PASIC,
    .parent        	= TYPE_I2C_SLAVE,
    .instance_size 	= sizeof(pmb887x_pasic_t),
    .class_init    	= pasic_class_init,
};

static void pasic_register_types(void) {
	type_register_static(&pasic_info);
}
type_init(pasic_register_types)
