/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = sci_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [1] = sci_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [2] = sci_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [3] = sci_uart_eri_isr, /* SCI9 ERI (Receive error) */
            [4] = spi_rxi_isr, /* SPI0 RXI (Receive buffer full) */
            [5] = spi_txi_isr, /* SPI0 TXI (Transmit buffer empty) */
            [6] = spi_tei_isr, /* SPI0 TEI (Transmission complete event) */
            [7] = spi_eri_isr, /* SPI0 ERI (Error) */
<<<<<<< HEAD
            [8] = sci_uart_rxi_isr, /* SCI7 RXI (Receive data full) */
            [9] = sci_uart_txi_isr, /* SCI7 TXI (Transmit data empty) */
            [10] = sci_uart_tei_isr, /* SCI7 TEI (Transmit end) */
            [11] = sci_uart_eri_isr, /* SCI7 ERI (Receive error) */
            [12] = gpt_counter_overflow_isr, /* GPT0 COUNTER OVERFLOW (Overflow) */
            [13] = sci_uart_rxi_isr, /* SCI2 RXI (Receive data full) */
            [14] = sci_uart_txi_isr, /* SCI2 TXI (Transmit data empty) */
            [15] = sci_uart_tei_isr, /* SCI2 TEI (Transmit end) */
            [16] = sci_uart_eri_isr, /* SCI2 ERI (Receive error) */
            [17] = sci_uart_rxi_isr, /* SCI6 RXI (Receive data full) */
            [18] = sci_uart_txi_isr, /* SCI6 TXI (Transmit data empty) */
            [19] = sci_uart_tei_isr, /* SCI6 TEI (Transmit end) */
            [20] = sci_uart_eri_isr, /* SCI6 ERI (Receive error) */
            [21] = sci_uart_rxi_isr, /* SCI4 RXI (Receive data full) */
            [22] = sci_uart_txi_isr, /* SCI4 TXI (Transmit data empty) */
            [23] = sci_uart_tei_isr, /* SCI4 TEI (Transmit end) */
            [24] = sci_uart_eri_isr, /* SCI4 ERI (Receive error) */
            [25] = spi_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [26] = spi_txi_isr, /* SPI1 TXI (Transmit buffer empty) */
            [27] = spi_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [28] = spi_eri_isr, /* SPI1 ERI (Error) */
            [29] = sdhimmc_accs_isr, /* SDHIMMC0 ACCS (Card access) */
            [30] = sdhimmc_card_isr, /* SDHIMMC0 CARD (Card detect) */
            [31] = dmac_int_isr, /* DMAC0 INT (DMAC0 transfer end) */
            [32] = rtc_alarm_periodic_isr, /* RTC PERIOD (Periodic interrupt) */
            [33] = rtc_carry_isr, /* RTC CARRY (Carry interrupt) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,GROUP0), /* SCI9 RXI (Receive data full) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,GROUP1), /* SCI9 TXI (Transmit data empty) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TEI,GROUP2), /* SCI9 TEI (Transmit end) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,GROUP3), /* SCI9 ERI (Receive error) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_SPI0_RXI,GROUP4), /* SPI0 RXI (Receive buffer full) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TXI,GROUP5), /* SPI0 TXI (Transmit buffer empty) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TEI,GROUP6), /* SPI0 TEI (Transmission complete event) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_SPI0_ERI,GROUP7), /* SPI0 ERI (Error) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_SCI7_RXI,GROUP0), /* SCI7 RXI (Receive data full) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_SCI7_TXI,GROUP1), /* SCI7 TXI (Transmit data empty) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_SCI7_TEI,GROUP2), /* SCI7 TEI (Transmit end) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_SCI7_ERI,GROUP3), /* SCI7 ERI (Receive error) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_GPT0_COUNTER_OVERFLOW,GROUP4), /* GPT0 COUNTER OVERFLOW (Overflow) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_SCI2_RXI,GROUP5), /* SCI2 RXI (Receive data full) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TXI,GROUP6), /* SCI2 TXI (Transmit data empty) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TEI,GROUP7), /* SCI2 TEI (Transmit end) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_SCI2_ERI,GROUP0), /* SCI2 ERI (Receive error) */
            [17] = BSP_PRV_VECT_ENUM(EVENT_SCI6_RXI,GROUP1), /* SCI6 RXI (Receive data full) */
            [18] = BSP_PRV_VECT_ENUM(EVENT_SCI6_TXI,GROUP2), /* SCI6 TXI (Transmit data empty) */
            [19] = BSP_PRV_VECT_ENUM(EVENT_SCI6_TEI,GROUP3), /* SCI6 TEI (Transmit end) */
            [20] = BSP_PRV_VECT_ENUM(EVENT_SCI6_ERI,GROUP4), /* SCI6 ERI (Receive error) */
            [21] = BSP_PRV_VECT_ENUM(EVENT_SCI4_RXI,GROUP5), /* SCI4 RXI (Receive data full) */
            [22] = BSP_PRV_VECT_ENUM(EVENT_SCI4_TXI,GROUP6), /* SCI4 TXI (Transmit data empty) */
            [23] = BSP_PRV_VECT_ENUM(EVENT_SCI4_TEI,GROUP7), /* SCI4 TEI (Transmit end) */
            [24] = BSP_PRV_VECT_ENUM(EVENT_SCI4_ERI,GROUP0), /* SCI4 ERI (Receive error) */
            [25] = BSP_PRV_VECT_ENUM(EVENT_SPI1_RXI,GROUP1), /* SPI1 RXI (Receive buffer full) */
            [26] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TXI,GROUP2), /* SPI1 TXI (Transmit buffer empty) */
            [27] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TEI,GROUP3), /* SPI1 TEI (Transmission complete event) */
            [28] = BSP_PRV_VECT_ENUM(EVENT_SPI1_ERI,GROUP4), /* SPI1 ERI (Error) */
            [29] = BSP_PRV_VECT_ENUM(EVENT_SDHIMMC0_ACCS,GROUP5), /* SDHIMMC0 ACCS (Card access) */
            [30] = BSP_PRV_VECT_ENUM(EVENT_SDHIMMC0_CARD,GROUP6), /* SDHIMMC0 CARD (Card detect) */
            [31] = BSP_PRV_VECT_ENUM(EVENT_DMAC0_INT,GROUP7), /* DMAC0 INT (DMAC0 transfer end) */
            [32] = BSP_PRV_VECT_ENUM(EVENT_RTC_PERIOD,FIXED), /* RTC PERIOD (Periodic interrupt) */
            [33] = BSP_PRV_VECT_ENUM(EVENT_RTC_CARRY,FIXED), /* RTC CARRY (Carry interrupt) */
=======
            [8] = spi_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [9] = spi_txi_isr, /* SPI1 TXI (Transmit buffer empty) */
            [10] = spi_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [11] = spi_eri_isr, /* SPI1 ERI (Error) */
            [12] = sci_uart_rxi_isr, /* SCI7 RXI (Receive data full) */
            [13] = sci_uart_txi_isr, /* SCI7 TXI (Transmit data empty) */
            [14] = sci_uart_tei_isr, /* SCI7 TEI (Transmit end) */
            [15] = sci_uart_eri_isr, /* SCI7 ERI (Receive error) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,GROUP0), /* SCI9 RXI (Receive data full) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,GROUP1), /* SCI9 TXI (Transmit data empty) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TEI,GROUP2), /* SCI9 TEI (Transmit end) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,GROUP3), /* SCI9 ERI (Receive error) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_SPI0_RXI,GROUP4), /* SPI0 RXI (Receive buffer full) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TXI,GROUP5), /* SPI0 TXI (Transmit buffer empty) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TEI,GROUP6), /* SPI0 TEI (Transmission complete event) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_SPI0_ERI,GROUP7), /* SPI0 ERI (Error) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_SPI1_RXI,GROUP0), /* SPI1 RXI (Receive buffer full) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TXI,GROUP1), /* SPI1 TXI (Transmit buffer empty) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TEI,GROUP2), /* SPI1 TEI (Transmission complete event) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_SPI1_ERI,GROUP3), /* SPI1 ERI (Error) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_SCI7_RXI,GROUP4), /* SCI7 RXI (Receive data full) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_SCI7_TXI,GROUP5), /* SCI7 TXI (Transmit data empty) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_SCI7_TEI,GROUP6), /* SCI7 TEI (Transmit end) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_SCI7_ERI,GROUP7), /* SCI7 ERI (Receive error) */
>>>>>>> branch 'main' of git@github.com:Wendellee/Embedded-Competition-Renesas-Track.git
        };
        #endif
        #endif
