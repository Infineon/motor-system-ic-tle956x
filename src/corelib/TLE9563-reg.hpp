/*!
 * \file        TLE9563-reg.hpp
 * \name        TLE9563-reg.hpp - Register enum for TLE9563
 * \author      Infineon Technologies AG
 * \copyright   2019-2020 Infineon Technologies AG
 * \version     0.0.1
 * \brief       This file lists all available control- and status-registers for a TLE9563 BLDC controller
 * \ref         tle9563corelib
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE9563_REG_HPP_
#define TLE9563_REG_HPP_

#include <stdint.h>
#include "TLE9563.hpp"

//! \brief enum for the SPI control registers in a TLE9563, page 173 in datasheet
		enum CtrlRegisters
		{
			/* SPI control registers, device control registers*/
			REG_ADDR_M_S_CTRL 			= (0x01),		/**< Mode and Supply Control*/
			REG_ADDR_HW_CTRL 			= (0x02),		/**< Hardware control*/
			REG_ADDR_WD_CTRL 			= (0x03),		/**< Watchdog control*/
			REG_ADDR_BUS_CTRL 			= (0x04),		/**< CAN Control*/
			REG_ADDR_WK_CTRL 			= (0x05),		/**< Wake-up control*/
			REG_ADDR_TIMER_CTRL 		= (0x06),		/**< Timer 1 and Timer 2control and selection*/
			REG_ADDR_SW_SD_CTRL 		= (0x07),		/**< High-side-switch shut down control*/
			REG_ADDR_HS_CTRL 			= (0x08),		/**< High-side-switch control*/
			REG_ADDR_INT_MASK 			= (0x09),		/**< Interrupt mask control*/
			REG_ADDR_PWM_CTRL 			= (0x0A),		/**< PWM configuration control*/
			REG_ADDR_SYS_STAT_CTRL 		= (0x0B),		/**< System status contorl*/
			/*SPI contorl registers, contorl registers bridge driver*/
			REG_ADDR_GENCTRL 			= (0x10),		/**< General bridge control*/
			REG_ADDR_CSA 				= (0x11),		/**< Current sense amplifier*/
			REG_ADDR_LS_VDS 			= (0x12),		/**< Drain-Source monitoring threshold*/
			REG_ADDR_HS_VDS 			= (0x13),		/**< Drain-Source monitoring threshold*/
			REG_ADDR_CCP_BLK 			= (0x14),		/**< CCP and times selection*/
			REG_ADDR_HBMODE 			= (0x15),		/**< Half-Bridge MODE*/
			REG_ADDR_TPRECHG 			= (0x16),		/**< PWM pre-charge and pre-discharge time*/	
			REG_ADDR_ST_ICHG 			= (0x17),		/**< Static charge/discharge current*/
			REG_ADDR_HB_ICHG 			= (0x18),		/**< PWM charge/discharge current*/
			REG_ADDR_HB_ICHG_MAX		= (0x19),		/**< PWM max. pre-charge/pre-discharge current*/
			REG_ADDR_HB_PCHG_INIT		= (0x1A),		/**< PWM pre-charge/pre-discharge initialization*/
			REG_ADDR_TDON_HB_CTRL 		= (0x1B),		/**< PWM inputs TON configuration*/
			REG_ADDR_TDOFF_HB_CTRL 		= (0x1C),		/**< PWM inputs TOFF configuration*/
			REG_ADDR_BRAKE	 			= (0x1D),		/**< Brake control*/
			/*SPI contorl registers, selective wake registers*/
			REG_ADDR_SWK_CTRL 			= (0x20),		/**< CAN Selective Wake Control*/
			REG_ADDR_SWK_BTL1_CTRL 		= (0x21),		/**< SWK Bit Timing Control*/
			REG_ADDR_SWK_ID1_CTRL 		= (0x22),		/**< SWK WUF Identifier bits 28...13*/
			REG_ADDR_SWK_ID0_CTRL 		= (0x23),		/**< SWK WUF Identifier bits 12...0*/
			REG_ADDR_SWK_MASK_ID1_CTRL	= (0x24),		/**< SWK WUF Identifier Mask bits 28...13*/ 
			REG_ADDR_SWK_MASK_ID0_CTRL 	= (0x25),		/**< SWK WUF Identifier Mask bits 12...0*/
			REG_ADDR_SWK_DLC_CTRL 		= (0x26),		/**< SWK Frame Data Length Code Control*/
			REG_ADDR_SWK_DATA3_CTRL 	= (0x27),		/**< SWK Data7-Data6 Register*/
			REG_ADDR_SWK_DATA2_CTRL 	= (0x28),		/**< SWK Data5-Data4 Register*/
			REG_ADDR_SWK_DATA1_CTRL 	= (0x29),		/**< SWK Data3-Data2 Register*/
			REG_ADDR_SWK_DATA0_CTRL 	= (0x2A),		/**< SWK Data1-Data0 Register*/
			REG_ADDR_SWK_CAN_FD_CTRL	= (0x2B),		/**< CAN FD Configuration Control Register*/
			/*SPI control registers, Selective Wake trim and configuration Registers*/
			REG_ADDR_SWK_OSC_TRIM_CTRL 	= (0x2C),		/**< SWK Oscillator Trimming and option Register*/
			REG_ADDR_SWK_OSC_CAL_STAT 	= (0x2D),		/**< SWK Oscillator Calibration Register*/
			REG_ADDR_SWK_CDR_CTRL 		= (0x2E),		/**< Clock Data Recovery Control Register*/
			REG_ADDR_SWK_CDR_LIMIT 		= (0x2F)		/**< SWK Clock Data Recovery Limit Control*/
		};

		//! \brief enum for the control registers in a TLE9563
		enum StatusRegisters
		{
            /* SPI status information registers, Device Status Registers */
			REG_ADDR_SUP_STAT			= (0x40),       /**< Supply Voltage Fail Status */
            REG_ADDR_THERM_STAT			= (0x41),       /**< Thermal Protection Status */
            REG_ADDR_DEV_STAT			= (0x42),       /**< Device Information Status */
            REG_ADDR_BUS_STAT			= (0x43),       /**< Bus Communication Status */
            REG_ADDR_WK_STAT			= (0x44),       /**< Wake-up Source and Information Status */
            REG_ADDR_WK_LVL_STAT		= (0x45),       /**< WK Input Level */
            REG_ADDR_HS_OL_OC_OT_STAT	= (0x46),       /**< High-Side Switch Status */
            /* SPI status information registers, Status registers bridge driver*/
            REG_ADDR_GEN_STAT			= (0x50),       /**< GEN Status register */
            REG_ADDR_TDREG				= (0x51),       /**< Turn-on/off delay regulation register */
            REG_ADDR_DSOV				= (0x52),       /**< Drain-source overvoltage HBVOUT */
            REG_ADDR_EFF_TDON_OFF1		= (0x53),       /**< Effective MOSFET turn-on/off delay - PWM half- bridge 1 */
            REG_ADDR_EFF_TDON_OFF2		= (0x54),       /**< Effective MOSFET turn-on/off delay - PWM half- bridge 2 */
			REG_ADDR_EFF_TDON_OFF3		= (0x55),       /**< Effective MOSFET turn-on/off delay - PWM half- bridge 3 */
			REG_ADDR_TRISE_FALL1		= (0x57),       /**< MOSFET rise/fall time - PWM half-bridge 1 */
			REG_ADDR_TRISE_FALL2		= (0x58),       /**< MOSFET rise/fall time - PWM half-bridge 2 */
			REG_ADDR_TRISE_FALL3		= (0x59),       /**< MOSFET rise/fall time - PWM half-bridge 3 */
			/* SPI status information registers, Selective wake status registers */
			REG_ADDR_SWK_STAT			= (0x60),       /**< Selective Wake Status */
			REG_ADDR_SWK_ECNT_STAT		= (0x61),       /**< Selective Wake ECNT Status */
			REG_ADDR_SWK_CDR_STAT		= (0x63),       /**< Selective Wake CDR Status */
			/* SPI status information registers, Family and product information register */
			REG_ADDR_FAM_PROD_STAT		= (0x70)        /**< Family and Product Identification Register */
		};
/** @} */

#endif /** TLE9563_REG_HPP_ **/