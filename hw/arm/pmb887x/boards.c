#include "hw/arm/pmb887x/boards.h"

static uint32_t board_c81_keymap[] = {
	[Q_KEY_CODE_KP_1]			= C81_KP_NUM1,
	[Q_KEY_CODE_1]				= C81_KP_NUM1,
	[Q_KEY_CODE_KP_4]			= C81_KP_NUM4,
	[Q_KEY_CODE_4]				= C81_KP_NUM4,
	[Q_KEY_CODE_KP_7]			= C81_KP_NUM7,
	[Q_KEY_CODE_7]				= C81_KP_NUM7,
	[Q_KEY_CODE_KP_DIVIDE]		= C81_KP_STAR,
	[Q_KEY_CODE_KP_2]			= C81_KP_NUM2,
	[Q_KEY_CODE_2]				= C81_KP_NUM2,
	[Q_KEY_CODE_KP_5]			= C81_KP_NUM5,
	[Q_KEY_CODE_5]				= C81_KP_NUM5,
	[Q_KEY_CODE_KP_8]			= C81_KP_NUM8,
	[Q_KEY_CODE_8]				= C81_KP_NUM8,
	[Q_KEY_CODE_KP_0]			= C81_KP_NUM0,
	[Q_KEY_CODE_0]				= C81_KP_NUM0,
	[Q_KEY_CODE_KP_3]			= C81_KP_NUM3,
	[Q_KEY_CODE_3]				= C81_KP_NUM3,
	[Q_KEY_CODE_KP_6]			= C81_KP_NUM6,
	[Q_KEY_CODE_6]				= C81_KP_NUM6,
	[Q_KEY_CODE_KP_9]			= C81_KP_NUM9,
	[Q_KEY_CODE_9]				= C81_KP_NUM9,
	[Q_KEY_CODE_KP_MULTIPLY]	= C81_KP_HASH,
	[Q_KEY_CODE_UP]				= C81_KP_NAV_UP,
	[Q_KEY_CODE_RIGHT]			= C81_KP_NAV_RIGHT,
	[Q_KEY_CODE_KP_ENTER]		= C81_KP_NAV_CENTER,
	[Q_KEY_CODE_RET]			= C81_KP_NAV_CENTER,
	[Q_KEY_CODE_LEFT]			= C81_KP_NAV_LEFT,
	[Q_KEY_CODE_DOWN]			= C81_KP_NAV_DOWN,
	[Q_KEY_CODE_F3]				= C81_KP_SEND,
	[Q_KEY_CODE_F5]				= C81_KP_BROWSER,
	[Q_KEY_CODE_F6]				= C81_KP_MUSIC,
	[Q_KEY_CODE_F1]				= C81_KP_SOFT_LEFT,
	[Q_KEY_CODE_F2]				= C81_KP_SOFT_RIGHT,
	[Q_KEY_CODE_F4]				= C81_KP_END_CALL,
	[Q_KEY_CODE_KP_SUBTRACT]	= C81_KP_VOLUME_DOWN,
	[Q_KEY_CODE_KP_ADD]			= C81_KP_VOLUME_UP,
};

static pmb887x_fixed_gpio_t board_c81_fixed_gpio[] = {
	{C81_GPIO_HW_DET_MOB_TYPE3,	1},
	{C81_GPIO_HW_DET_MOB_TYPE2,	0},
	{C81_GPIO_HW_DET_MOB_TYPE1,	0},
	{C81_GPIO_HW_DET_MOB_TYPE4,	1},
	{C81_GPIO_HW_DET_BLUETOOTH,	0},
	{C81_GPIO_HW_DET_BAND_SEL,	0},
};

static uint32_t board_c81_flashes[] = {
	0x0089880D,
	0x0089880D,
};

static uint32_t board_cx75_keymap[] = {
	[Q_KEY_CODE_KP_1]	= CX75_KP_NUM1,
	[Q_KEY_CODE_1]		= CX75_KP_NUM1,
	[Q_KEY_CODE_KP_4]	= CX75_KP_NUM4,
	[Q_KEY_CODE_4]		= CX75_KP_NUM4,
	[Q_KEY_CODE_KP_7]	= CX75_KP_NUM7,
	[Q_KEY_CODE_7]		= CX75_KP_NUM7,
	[Q_KEY_CODE_KP_2]	= CX75_KP_NUM2,
	[Q_KEY_CODE_2]		= CX75_KP_NUM2,
	[Q_KEY_CODE_KP_5]	= CX75_KP_NUM5,
	[Q_KEY_CODE_5]		= CX75_KP_NUM5,
	[Q_KEY_CODE_KP_8]	= CX75_KP_NUM8,
	[Q_KEY_CODE_8]		= CX75_KP_NUM8,
	[Q_KEY_CODE_KP_3]	= CX75_KP_NUM3,
	[Q_KEY_CODE_3]		= CX75_KP_NUM3,
	[Q_KEY_CODE_KP_6]	= CX75_KP_NUM6,
	[Q_KEY_CODE_6]		= CX75_KP_NUM6,
	[Q_KEY_CODE_KP_9]	= CX75_KP_NUM9,
	[Q_KEY_CODE_9]		= CX75_KP_NUM9,
};

static pmb887x_fixed_gpio_t board_cx75_fixed_gpio[] = {
};

static uint32_t board_cx75_flashes[] = {
	0x0089880D,
};

static uint32_t board_sl75_keymap[] = {
	[Q_KEY_CODE_KP_1]			= SL75_KP_NUM1,
	[Q_KEY_CODE_1]				= SL75_KP_NUM1,
	[Q_KEY_CODE_KP_4]			= SL75_KP_NUM4,
	[Q_KEY_CODE_4]				= SL75_KP_NUM4,
	[Q_KEY_CODE_KP_7]			= SL75_KP_NUM7,
	[Q_KEY_CODE_7]				= SL75_KP_NUM7,
	[Q_KEY_CODE_KP_DIVIDE]		= SL75_KP_STAR,
	[Q_KEY_CODE_KP_2]			= SL75_KP_NUM2,
	[Q_KEY_CODE_2]				= SL75_KP_NUM2,
	[Q_KEY_CODE_KP_5]			= SL75_KP_NUM5,
	[Q_KEY_CODE_5]				= SL75_KP_NUM5,
	[Q_KEY_CODE_KP_8]			= SL75_KP_NUM8,
	[Q_KEY_CODE_8]				= SL75_KP_NUM8,
	[Q_KEY_CODE_KP_0]			= SL75_KP_NUM0,
	[Q_KEY_CODE_0]				= SL75_KP_NUM0,
	[Q_KEY_CODE_KP_3]			= SL75_KP_NUM3,
	[Q_KEY_CODE_3]				= SL75_KP_NUM3,
	[Q_KEY_CODE_KP_6]			= SL75_KP_NUM6,
	[Q_KEY_CODE_6]				= SL75_KP_NUM6,
	[Q_KEY_CODE_KP_9]			= SL75_KP_NUM9,
	[Q_KEY_CODE_9]				= SL75_KP_NUM9,
	[Q_KEY_CODE_KP_MULTIPLY]	= SL75_KP_HASH,
	[Q_KEY_CODE_UP]				= SL75_KP_NAV_UP,
	[Q_KEY_CODE_RIGHT]			= SL75_KP_NAV_RIGHT,
	[Q_KEY_CODE_KP_ENTER]		= SL75_KP_NAV_CENTER,
	[Q_KEY_CODE_RET]			= SL75_KP_NAV_CENTER,
	[Q_KEY_CODE_LEFT]			= SL75_KP_NAV_LEFT,
	[Q_KEY_CODE_DOWN]			= SL75_KP_NAV_DOWN,
	[Q_KEY_CODE_F3]				= SL75_KP_SEND,
	[Q_KEY_CODE_F5]				= SL75_KP_MUSIC,
	[Q_KEY_CODE_F6]				= SL75_KP_PLAY_PAUSE,
	[Q_KEY_CODE_F1]				= SL75_KP_SOFT_LEFT,
	[Q_KEY_CODE_F2]				= SL75_KP_SOFT_RIGHT,
	[Q_KEY_CODE_F4]				= SL75_KP_END_CALL,
	[Q_KEY_CODE_F8]				= SL75_KP_CAMERA,
	[Q_KEY_CODE_F7]				= SL75_KP_PTT,
	[Q_KEY_CODE_KP_ADD]			= SL75_KP_VOLUME_UP,
	[Q_KEY_CODE_KP_SUBTRACT]	= SL75_KP_VOLUME_DOWN,
};

static pmb887x_fixed_gpio_t board_sl75_fixed_gpio[] = {
	{SL75_GPIO_HW_DET_MOB_TYPE3,	0},
	{SL75_GPIO_HW_DET_MOB_TYPE2,	0},
	{SL75_GPIO_HW_DET_MOB_TYPE1,	0},
	{SL75_GPIO_HW_DET_MOB_TYPE4,	1},
	{SL75_GPIO_HW_DET_BLUETOOTH,	0},
	{SL75_GPIO_HW_DET_BAND_SEL,		0},
	{SL75_GPIO_OPEN_CLOSE_SW2,		1},
};

static uint32_t board_sl75_flashes[] = {
	0x0089880D,
	0x0089880D,
	0x0089880D,
};

static uint32_t board_el71_keymap[] = {
	[Q_KEY_CODE_KP_1]			= EL71_KP_NUM1,
	[Q_KEY_CODE_1]				= EL71_KP_NUM1,
	[Q_KEY_CODE_KP_4]			= EL71_KP_NUM4,
	[Q_KEY_CODE_4]				= EL71_KP_NUM4,
	[Q_KEY_CODE_KP_7]			= EL71_KP_NUM7,
	[Q_KEY_CODE_7]				= EL71_KP_NUM7,
	[Q_KEY_CODE_KP_DIVIDE]		= EL71_KP_STAR,
	[Q_KEY_CODE_KP_2]			= EL71_KP_NUM2,
	[Q_KEY_CODE_2]				= EL71_KP_NUM2,
	[Q_KEY_CODE_KP_5]			= EL71_KP_NUM5,
	[Q_KEY_CODE_5]				= EL71_KP_NUM5,
	[Q_KEY_CODE_KP_8]			= EL71_KP_NUM8,
	[Q_KEY_CODE_8]				= EL71_KP_NUM8,
	[Q_KEY_CODE_KP_0]			= EL71_KP_NUM0,
	[Q_KEY_CODE_0]				= EL71_KP_NUM0,
	[Q_KEY_CODE_KP_3]			= EL71_KP_NUM3,
	[Q_KEY_CODE_3]				= EL71_KP_NUM3,
	[Q_KEY_CODE_KP_6]			= EL71_KP_NUM6,
	[Q_KEY_CODE_6]				= EL71_KP_NUM6,
	[Q_KEY_CODE_KP_9]			= EL71_KP_NUM9,
	[Q_KEY_CODE_9]				= EL71_KP_NUM9,
	[Q_KEY_CODE_KP_MULTIPLY]	= EL71_KP_HASH,
	[Q_KEY_CODE_UP]				= EL71_KP_NAV_UP,
	[Q_KEY_CODE_RIGHT]			= EL71_KP_NAV_RIGHT,
	[Q_KEY_CODE_KP_ENTER]		= EL71_KP_NAV_CENTER,
	[Q_KEY_CODE_RET]			= EL71_KP_NAV_CENTER,
	[Q_KEY_CODE_LEFT]			= EL71_KP_NAV_LEFT,
	[Q_KEY_CODE_DOWN]			= EL71_KP_NAV_DOWN,
	[Q_KEY_CODE_F3]				= EL71_KP_SEND,
	[Q_KEY_CODE_F5]				= EL71_KP_MUSIC,
	[Q_KEY_CODE_F6]				= EL71_KP_PLAY_PAUSE,
	[Q_KEY_CODE_F1]				= EL71_KP_SOFT_LEFT,
	[Q_KEY_CODE_F2]				= EL71_KP_SOFT_RIGHT,
	[Q_KEY_CODE_F4]				= EL71_KP_END_CALL,
	[Q_KEY_CODE_F8]				= EL71_KP_CAMERA,
	[Q_KEY_CODE_F7]				= EL71_KP_PTT,
	[Q_KEY_CODE_KP_ADD]			= EL71_KP_VOLUME_UP,
	[Q_KEY_CODE_KP_SUBTRACT]	= EL71_KP_VOLUME_DOWN,
};

static pmb887x_fixed_gpio_t board_el71_fixed_gpio[] = {
	{EL71_GPIO_HW_DET_MOB_TYPE3,	0},
	{EL71_GPIO_HW_DET_MOB_TYPE2,	0},
	{EL71_GPIO_HW_DET_MOB_TYPE1,	0},
	{EL71_GPIO_HW_DET_MOB_TYPE4,	1},
	{EL71_GPIO_HW_DET_BLUETOOTH,	0},
	{EL71_GPIO_HW_DET_BAND_SEL,		0},
	{EL71_GPIO_OPEN_CLOSE_SW2,		1},
};

static uint32_t board_el71_flashes[] = {
	0x00208819,
};

static pmb887x_board_t boards_list[] = {
	{
		.name				= "C81",
		.width				= 132,
		.height				= 176,
		.display			= "jbt6k71",
		.display_rotation	= 0,
		.cpu				= CPU_PMB8876,
		.flash_banks		= board_c81_flashes,
		.flash_banks_cnt	= ARRAY_SIZE(board_c81_flashes),
		.keymap				= board_c81_keymap,
		.keymap_cnt			= ARRAY_SIZE(board_c81_keymap),
		.fixed_gpios		= board_c81_fixed_gpio,
		.fixed_gpios_cnt	= ARRAY_SIZE(board_c81_fixed_gpio),

	},
	{
		.name				= "CX75",
		.width				= 132,
		.height				= 176,
		.display			= NULL,
		.display_rotation	= 0,
		.cpu				= CPU_PMB8875,
		.flash_banks		= board_cx75_flashes,
		.flash_banks_cnt	= ARRAY_SIZE(board_cx75_flashes),
		.keymap				= board_cx75_keymap,
		.keymap_cnt			= ARRAY_SIZE(board_cx75_keymap),
		.fixed_gpios		= board_cx75_fixed_gpio,
		.fixed_gpios_cnt	= ARRAY_SIZE(board_cx75_fixed_gpio),

	},
	{
		.name				= "SL75",
		.width				= 240,
		.height				= 320,
		.display			= "jbt6k71",
		.display_rotation	= 180,
		.cpu				= CPU_PMB8876,
		.flash_banks		= board_sl75_flashes,
		.flash_banks_cnt	= ARRAY_SIZE(board_sl75_flashes),
		.keymap				= board_sl75_keymap,
		.keymap_cnt			= ARRAY_SIZE(board_sl75_keymap),
		.fixed_gpios		= board_sl75_fixed_gpio,
		.fixed_gpios_cnt	= ARRAY_SIZE(board_sl75_fixed_gpio),

	},
	{
		.name				= "EL71",
		.width				= 240,
		.height				= 320,
		.display			= "jbt6k71",
		.display_rotation	= 180,
		.cpu				= CPU_PMB8876,
		.flash_banks		= board_el71_flashes,
		.flash_banks_cnt	= ARRAY_SIZE(board_el71_flashes),
		.keymap				= board_el71_keymap,
		.keymap_cnt			= ARRAY_SIZE(board_el71_keymap),
		.fixed_gpios		= board_el71_fixed_gpio,
		.fixed_gpios_cnt	= ARRAY_SIZE(board_el71_fixed_gpio),

	},
};

pmb887x_board_t *pmb887x_get_board(int board) {
	return &boards_list[board];
}

