/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *   BSP (Board Support Package)
 *   Target : STM32
 *            "STM32 Board"
 *   Button definition file.
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

#ifndef BSP_BUTTON_DEFS_H
#define BSP_BUTTON_DEFS_H

#include "bsp_board_defs.h"
#include "bsp_macros.h"

#define __bsp_NUM_BUTTONS__				1
#define __bsp_BUTTON_DEBOUNCE_WAIT__(expr)    st( int i; for(i = 0; i < 500; i++) { if (!(expr)) i = 0; } )

#ifdef MapleRET6
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 *                 BUTTON #1
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 *   Description :  Push Button
	 *   Polarity    :  Active High
	 *   GPIO        :  PB12
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	#define __bsp_BUTTON1_PIN__				PB12
	#define __bsp_BUTTON1_BIT__				(__bsp_BUTTON1_PIN__ & 0x0F)
	#define __bsp_BUTTON1_PORT__			GPIOB->IDR
	#define __bsp_BUTTON1_DDR__
	#define __bsp_BUTTON1_MODE__			((uint32_t)GPIO_Mode_IN_FLOATING)
	#define __bsp_BUTTON1_IS_ACTIVE_LOW__	1
#endif

#ifdef STM32VLDISCOVERY
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 *                 BUTTON #1
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 *   Description :  Push Button
	 *   Polarity    :  Active Low
	 *   GPIO        :  PA0
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	#define __bsp_BUTTON1_PIN__				PA0
	#define __bsp_BUTTON1_BIT__				(__bsp_BUTTON1_PIN__ & 0x0F)
	#define __bsp_BUTTON1_PORT__			GPIOA->IDR
	#define __bsp_BUTTON1_DDR__
	#define __bsp_BUTTON1_MODE__			((uint32_t)GPIO_Mode_IN_FLOATING)
	#define __bsp_BUTTON1_IS_ACTIVE_LOW__	0
#endif
/* ------------------------------------------------------------------------------------------------
 *                                Include Generic Button Macros
 * ------------------------------------------------------------------------------------------------
 */
#include "code/bsp_generic_buttons.h"

#undef  __bsp_BUTTON1_CONFIG__
#define __bsp_BUTTON1_CONFIG__()	__bsp_PIN_CONFIG__  ( __bsp_BUTTON1_PIN__, __bsp_BUTTON1_MODE__ )

#endif
