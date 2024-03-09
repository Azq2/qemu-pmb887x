#pragma once

#include "qemu/osdep.h"

struct _pmb887x_cfg_t;

typedef struct {
	char *key;
	char *value;
} pmb887x_cfg_item_t;

typedef struct {
	char *name;
	pmb887x_cfg_item_t *items;
	size_t items_count;
	struct _pmb887x_cfg_t *parent;
} pmb887x_cfg_section_t;

typedef struct _pmb887x_cfg_t {
	char *file;
	pmb887x_cfg_section_t *sections;
	size_t sections_count;
} pmb887x_cfg_t;

pmb887x_cfg_t *pmb887x_cfg_parse(const char *file);
pmb887x_cfg_section_t *pmb887x_cfg_section(pmb887x_cfg_t *cfg, const char *name, ssize_t index, bool required);
const char *pmb887x_cfg_get(pmb887x_cfg_t *cfg, const char *section_name, const char *key, bool required);
const char *pmb887x_cfg_section_get(pmb887x_cfg_section_t *section, const char *key, bool required);
size_t pmb887x_cfg_sections_cnt(pmb887x_cfg_t *cfg, const char *name);
void pmb887x_cfg_free(pmb887x_cfg_t  *cfg);