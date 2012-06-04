/*
 * Copyright 2009 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
********************************************************************************
*   WARNING: This file is automatically generated
*                 DO NOT MODIFY THIS FILE DIRECTLY
********************************************************************************
********************************************************************************
*******************************************************************************/

#ifndef  HW_IRQ_DEFS
#define HW_IRQ_DEFS  1

#define HW_IRQ_DEBUG_UART          (0)
#define HW_IRQ_COMMS_RX            (1)
#define HW_IRQ_COMMS_TX            (1)
#define HW_IRQ_SSP2_ERROR          (2)
#define HW_IRQ_VDD5V               (3)
#define HW_IRQ_HEADPHONE_SHORT     (4)
#define HW_IRQ_DAC_DMA             (5)
#define HW_IRQ_DAC_ERROR           (6)
#define HW_IRQ_ADC_DMA             (7)
#define HW_IRQ_ADC_ERROR           (8)
#define HW_IRQ_SPDIF_DMA           (9)
#define HW_IRQ_SAIF2_DMA           (9)
#define HW_IRQ_SPDIF_ERROR         (10)
#define HW_IRQ_SAIF1_IRQ           (10)
#define HW_IRQ_SAIF2_IRQ           (10)
#define HW_IRQ_USB_CTRL            (11)
#define HW_IRQ_USB_WAKEUP          (12)
#define HW_IRQ_GPMI_DMA            (13)
#define HW_IRQ_SSP1_DMA            (14)
#define HW_IRQ_SSP_ERROR           (15)
#define HW_IRQ_GPIO0               (16)
#define HW_IRQ_GPIO1               (17)
#define HW_IRQ_GPIO2               (18)
#define HW_IRQ_SAIF1_DMA           (19)
#define HW_IRQ_SSP2_DMA            (20)
#define HW_IRQ_ECC8_IRQ            (21)
#define HW_IRQ_RTC_ALARM           (22)
#define HW_IRQ_UARTAPP_TX_DMA      (23)
#define HW_IRQ_UARTAPP_INTERNAL    (24)
#define HW_IRQ_UARTAPP_RX_DMA      (25)
#define HW_IRQ_I2C_DMA             (26)
#define HW_IRQ_I2C_ERROR           (27)
#define HW_IRQ_TIMER0              (28)
#define HW_IRQ_TIMER1              (29)
#define HW_IRQ_TIMER2              (30)
#define HW_IRQ_TIMER3              (31)
#define HW_IRQ_BATT_BRNOUT         (32)
#define HW_IRQ_VDDD_BRNOUT         (33)
#define HW_IRQ_VDDIO_BRNOUT        (34)
#define HW_IRQ_VDD18_BRNOUT        (35)
#define HW_IRQ_TOUCH_DETECT        (36)
#define HW_IRQ_LRADC_CH0           (37)
#define HW_IRQ_LRADC_CH1           (38)
#define HW_IRQ_LRADC_CH2           (39)
#define HW_IRQ_LRADC_CH3           (40)
#define HW_IRQ_LRADC_CH4           (41)
#define HW_IRQ_LRADC_CH5           (42)
#define HW_IRQ_LRADC_CH6           (43)
#define HW_IRQ_LRADC_CH7           (44)
#define HW_IRQ_LCDIF_DMA           (45)
#define HW_IRQ_LCDIF_ERROR         (46)
#define HW_IRQ_DIGCTL_DEBUG_TRAP   (47)
#define HW_IRQ_RTC_1MSEC           (48)
#define HW_IRQ_DRI_DMA             (49)
#define HW_IRQ_DRI_ATTENTION       (50)
#define HW_IRQ_GPMI                (51)
#define HW_IRQ_IR                  (52)
#define HW_IRQ_DCP_VMI             (53)
#define HW_IRQ_DCP                 (54)
#define HW_IRQ_RESERVED_55         (55)
#define HW_IRQ_BCH                 (56)
#define HW_IRQ_PXP                 (57)
#define HW_IRQ_UARTAPP2_TX_DMA     (58)
#define HW_IRQ_UARTAPP2_INTERNAL   (59)
#define HW_IRQ_UARTAPP2_RX_DMA     (60)
#define HW_IRQ_VDAC_DETECT         (61)
#define HW_IRQ_SYDMA               (62)
#define HW_IRQ_SY                  (63)
#define HW_IRQ_VDD5V_DROOP         (64)
#define HW_IRQ_DCDC4P2_BO          (65)
#define HW_IRQ_RESERVED_66         (66)
#define HW_IRQ_RESERVED_67         (67)
#define HW_IRQ_RESERVED_68         (68)
#define HW_IRQ_RESERVED_69         (69)
#define HW_IRQ_RESERVED_70         (70)
#define HW_IRQ_RESERVED_71         (71)
#define HW_IRQ_RESERVED_72         (72)
#define HW_IRQ_RESERVED_73         (73)
#define HW_IRQ_RESERVED_74         (74)
#define HW_IRQ_RESERVED_75         (75)
#define HW_IRQ_RESERVED_76         (76)
#define HW_IRQ_RESERVED_77         (77)
#define HW_IRQ_RESERVED_78         (78)
#define HW_IRQ_RESERVED_79         (79)
#define HW_IRQ_RESERVED_80         (80)
#define HW_IRQ_RESERVED_81         (81)
#define HW_IRQ_RESERVED_82         (82)
#define HW_IRQ_RESERVED_83         (83)
#define HW_IRQ_RESERVED_84         (84)
#define HW_IRQ_RESERVED_85         (85)
#define HW_IRQ_RESERVED_86         (86)
#define HW_IRQ_RESERVED_87         (87)
#define HW_IRQ_RESERVED_88         (88)
#define HW_IRQ_RESERVED_89         (89)
#define HW_IRQ_RESERVED_90         (90)
#define HW_IRQ_RESERVED_91         (91)
#define HW_IRQ_RESERVED_92         (92)
#define HW_IRQ_RESERVED_93         (93)
#define HW_IRQ_RESERVED_94         (94)
#define HW_IRQ_RESERVED_95         (95)
#define HW_IRQ_RESERVED_96         (96)
#define HW_IRQ_RESERVED_97         (97)
#define HW_IRQ_RESERVED_98         (98)
#define HW_IRQ_RESERVED_99         (99)
#define HW_IRQ_RESERVED_100        (100)
#define HW_IRQ_RESERVED_101        (101)
#define HW_IRQ_RESERVED_101        (101)
#define HW_IRQ_RESERVED_102        (102)
#define HW_IRQ_RESERVED_103        (103)
#define HW_IRQ_RESERVED_104        (104)
#define HW_IRQ_RESERVED_105        (105)
#define HW_IRQ_RESERVED_106        (106)
#define HW_IRQ_RESERVED_107        (107)
#define HW_IRQ_RESERVED_108        (108)
#define HW_IRQ_RESERVED_109        (109)
#define HW_IRQ_RESERVED_110        (110)
#define HW_IRQ_RESERVED_111        (111)
#define HW_IRQ_RESERVED_112        (112)
#define HW_IRQ_RESERVED_113        (113)
#define HW_IRQ_RESERVED_114        (114)
#define HW_IRQ_RESERVED_115        (115)
#define HW_IRQ_RESERVED_116        (116)
#define HW_IRQ_RESERVED_117        (117)
#define HW_IRQ_RESERVED_118        (118)
#define HW_IRQ_RESERVED_119        (119)
#define HW_IRQ_RESERVED_120        (120)
#define HW_IRQ_RESERVED_121        (121)
#define HW_IRQ_RESERVED_122        (122)
#define HW_IRQ_RESERVED_123        (123)
#define HW_IRQ_RESERVED_124        (124)
#define HW_IRQ_RESERVED_125        (125)
#define HW_IRQ_RESERVED_126        (126)
#define HW_IRQ_RESERVED_127        (127)



////////////////////////////////////////////////////////////////

///   IRQ ENUMERATIONS

////////////////////////////////////////////////////////////////

#ifndef __LANGUAGE_ASM__
typedef enum
{
        FIRST_IRQ_ICOLL_VECTOR    = 0,
        VECTOR_IRQ_DEBUG_UART     = 0,
        VECTOR_IRQ_COMMS_RX       = 1,
        VECTOR_IRQ_COMMS_TX       = 1,
        VECTOR_IRQ_SSP2_ERROR     = 2,
        VECTOR_IRQ_VDD5V          = 3,
        VECTOR_IRQ_HEADPHONE_SHORT= 4,
        VECTOR_IRQ_DAC_DMA        = 5,
        VECTOR_IRQ_DAC_ERROR      = 6,
        VECTOR_IRQ_ADC_DMA        = 7,
        VECTOR_IRQ_ADC_ERROR      = 8,
        VECTOR_IRQ_SPDIF_DMA      = 9,
        VECTOR_IRQ_SAIF2_DMA      = 9,
        VECTOR_IRQ_SPDIF_ERROR    = 10,
        VECTOR_IRQ_SAIF1_IRQ      = 10,
        VECTOR_IRQ_SAIF2_IRQ      = 10,
        VECTOR_IRQ_USB_CTRL       = 11,
        VECTOR_IRQ_USB_WAKEUP     = 12,
        VECTOR_IRQ_GPMI_DMA       = 13,
        VECTOR_IRQ_SSP_DMA        = 14,
        VECTOR_IRQ_SSP_ERROR      = 15,
        VECTOR_IRQ_GPIO0          = 16,
        VECTOR_IRQ_GPIO1          = 17,
        VECTOR_IRQ_GPIO2          = 18,
        VECTOR_IRQ_SAIF1_DMA      = 19,
        VECTOR_IRQ_SSP2_DMA       = 20,
        VECTOR_IRQ_ECC8           = 21,
        VECTOR_IRQ_RTC_ALARM      = 22,
        VECTOR_IRQ_UARTAPP_TX_DMA = 23,
        VECTOR_IRQ_UARTAPP_INTERNAL= 24,
        VECTOR_IRQ_UARTAPP_RX_DMA = 25,
        VECTOR_IRQ_I2C_DMA        = 26,
        VECTOR_IRQ_I2C_ERROR      = 27,
        VECTOR_IRQ_TIMER0         = 28,
        VECTOR_IRQ_TIMER1         = 29,
        VECTOR_IRQ_TIMER2         = 30,
        VECTOR_IRQ_TIMER3         = 31,
        VECTOR_IRQ_BATT_BRNOUT    = 32,
        VECTOR_IRQ_VDDD_BRNOUT    = 33,
        VECTOR_IRQ_VDDIO_BRNOUT   = 34,
        VECTOR_IRQ_VDD18_BRNOUT   = 35,
        VECTOR_IRQ_TOUCH_DETECT   = 36,
        VECTOR_IRQ_LRADC_CH0      = 37,
        VECTOR_IRQ_LRADC_CH1      = 38,
        VECTOR_IRQ_LRADC_CH2      = 39,
        VECTOR_IRQ_LRADC_CH3      = 40,
        VECTOR_IRQ_LRADC_CH4      = 41,
        VECTOR_IRQ_LRADC_CH5      = 42,
        VECTOR_IRQ_LRADC_CH6      = 43,
        VECTOR_IRQ_LRADC_CH7      = 44,
        VECTOR_IRQ_LCDIF_DMA      = 45,
        VECTOR_IRQ_LCDIF_ERROR    = 46,
        VECTOR_IRQ_DIGCTL_DEBUG_TRAP = 47,
        VECTOR_IRQ_RTC_1MSEC      = 48,
        VECTOR_IRQ_DRI_DMA        = 49,
        VECTOR_IRQ_DRI_ATTENTION  = 50,
        VECTOR_IRQ_GPMI           = 51,
        VECTOR_IRQ_IR             = 52,
        VECTOR_IRQ_DCP_VMI        = 53,
        VECTOR_IRQ_DCP            = 54,
        VECTOR_IRQ_RESERVED_55    = 55,
        VECTOR_IRQ_BCH            = 56,
        VECTOR_IRQ_PXP            = 57,
        VECTOR_IRQ_UARTAPP2_TX_DMA = 58,
        VECTOR_IRQ_UARTAPP2_INTERNAL = 59,
        VECTOR_IRQ_UARTAPP2_RX_DMA = 60,
        VECTOR_IRQ_VDAC_DETECT     = 61,
        VECTOR_IRQ_SYDMA           = 62,
        VECTOR_IRQ_SY              = 63,
        VECTOR_IRQ_VDD5V_DROOP     = 64,
        VECTOR_IRQ_DCDC4P2_BO      = 65,
        VECTOR_IRQ_RESERVED_66         = 66,
        VECTOR_IRQ_RESERVED_67         = 67,
        VECTOR_IRQ_RESERVED_68         = 68,
        VECTOR_IRQ_RESERVED_69         = 69,
        VECTOR_IRQ_RESERVED_70         = 70,
        VECTOR_IRQ_RESERVED_71         = 71,
        VECTOR_IRQ_RESERVED_72         = 72,
        VECTOR_IRQ_RESERVED_73         = 73,
        VECTOR_IRQ_RESERVED_74         = 74,
        VECTOR_IRQ_RESERVED_75         = 75,
        VECTOR_IRQ_RESERVED_76         = 76,
        VECTOR_IRQ_RESERVED_77         = 77,
        VECTOR_IRQ_RESERVED_78         = 78,
        VECTOR_IRQ_RESERVED_79         = 79,
        VECTOR_IRQ_RESERVED_80         = 80,
        VECTOR_IRQ_RESERVED_81         = 81,
        VECTOR_IRQ_RESERVED_82         = 82,
        VECTOR_IRQ_RESERVED_83         = 83,
        VECTOR_IRQ_RESERVED_84         = 84,
        VECTOR_IRQ_RESERVED_85         = 85,
        VECTOR_IRQ_RESERVED_86         = 86,
        VECTOR_IRQ_RESERVED_87         = 87,
        VECTOR_IRQ_RESERVED_88         = 88,
        VECTOR_IRQ_RESERVED_89         = 89,
        VECTOR_IRQ_RESERVED_90         = 90,
        VECTOR_IRQ_RESERVED_91         = 91,
        VECTOR_IRQ_RESERVED_92         = 92,
        VECTOR_IRQ_RESERVED_93         = 93,
        VECTOR_IRQ_RESERVED_94         = 94,
        VECTOR_IRQ_RESERVED_95         = 95,
        VECTOR_IRQ_RESERVED_96         = 96,
        VECTOR_IRQ_RESERVED_97         = 97,
        VECTOR_IRQ_RESERVED_98         = 98,
        VECTOR_IRQ_RESERVED_99         = 99,
        VECTOR_IRQ_RESERVED_100        = 100,
        VECTOR_IRQ_RESERVED_101        = 101,
        VECTOR_IRQ_RESERVED_102        = 102,
        VECTOR_IRQ_RESERVED_103        = 103,
        VECTOR_IRQ_RESERVED_104        = 104,
        VECTOR_IRQ_RESERVED_105        = 105,
        VECTOR_IRQ_RESERVED_106        = 106,
        VECTOR_IRQ_RESERVED_107        = 107,
        VECTOR_IRQ_RESERVED_108        = 108,
        VECTOR_IRQ_RESERVED_109        = 109,
        VECTOR_IRQ_RESERVED_110        = 110,
        VECTOR_IRQ_RESERVED_111        = 111,
        VECTOR_IRQ_RESERVED_112        = 112,
        VECTOR_IRQ_RESERVED_113        = 113,
        VECTOR_IRQ_RESERVED_114        = 114,
        VECTOR_IRQ_RESERVED_115        = 115,
        VECTOR_IRQ_RESERVED_116        = 116,
        VECTOR_IRQ_RESERVED_117        = 117,
        VECTOR_IRQ_RESERVED_118        = 118,
        VECTOR_IRQ_RESERVED_119        = 119,
        VECTOR_IRQ_RESERVED_120        = 120,
        VECTOR_IRQ_RESERVED_121        = 121,
        VECTOR_IRQ_RESERVED_122        = 122,
        VECTOR_IRQ_RESERVED_123        = 123,
        VECTOR_IRQ_RESERVED_124        = 124,
        VECTOR_IRQ_RESERVED_125        = 125,
        VECTOR_IRQ_RESERVED_126        = 126,
        VECTOR_IRQ_RESERVED_127        = 127,
        LAST_IRQ_ICOLL_VECTOR          = 127,
        END_IRQ_ICOLL_VECTOR           = 128
} ICOLL_IRQ_enums_t;
#endif

////////////////////////////////////////////////////////////////

///   FIQ ENUMERATIONS

////////////////////////////////////////////////////////////////

#ifndef __LANGUAGE_ASM__
typedef enum
{
        FIRST_FIQ_ICOLL_VECTOR    = 32,
        VECTOR_FIQ_BATT_BRNOUT    = 32,
        VECTOR_FIQ_VDDD_BRNOUT    = 33,
        VECTOR_FIQ_VDDIO_BRNOUT   = 34,
        VECTOR_FIQ_VDD18_BRNOUT   = 35,
        VECTOR_FIQ_VDD5V_DROOP    = 64,
        VECTOR_FIQ_DCDC4P2_BO     = 65,
        LAST_FIQ_ICOLL_VECTOR     = 65
} ICOLL_FIQ_enums_t;
#endif
/*******************************************************************************
********************************************************************************
*   CVS LOG:
*  $Log:
*
********************************************************************************
*******************************************************************************/

#endif
