memcmp = 0x4000c260;
memcpy = 0x4000c2c8;
memset = 0x4000c44c;
__sfp_lock = 0x3ffae0ac;
__sinit_lock = 0x3ffae0a8;
__env_lock_object = 0x3ffae0b8;
__tz_lock_object = 0x3ffae080;

PROVIDE ( ets_efuse_get_spiconfig = 0x40008658 );
PROVIDE ( ets_write_char_uart = 0x40007cf8 );
PROVIDE ( ets_printf = 0x40007d54 );
PROVIDE ( ets_delay_us = 0x40008534 );
PROVIDE ( cache_flash_mmu_set_rom = 0x400095e0 );
PROVIDE ( Cache_Flush_rom = 0x40009a14 );
PROVIDE ( Cache_Read_Disable_rom = 0x40009ab8 );
PROVIDE ( Cache_Read_Enable_rom = 0x40009a84 );
PROVIDE ( Cache_Read_Init_rom = 0x40009950 );
PROVIDE ( cache_sram_mmu_set_rom = 0x400097f4 );
PROVIDE ( ets_sha_disable = 0x4005c0a8 );
PROVIDE ( ets_sha_enable = 0x4005c07c );
PROVIDE ( ets_sha_finish = 0x4005c104 );
PROVIDE ( ets_sha_init = 0x4005c0d4 );
PROVIDE ( ets_sha_update = 0x4005c2a0 );
PROVIDE ( rtc_get_reset_reason = 0x400081d4 );
PROVIDE ( rtc_get_wakeup_cause = 0x400081f4 );
PROVIDE ( ets_install_putc1 = 0x40007d18 );
PROVIDE ( ets_install_putc2 = 0x40007d38 );
PROVIDE ( intr_matrix_set = 0x4000681c );


PROVIDE ( crc32_le = 0x4005cfec );
PROVIDE ( esp_crc8 = 0x4005d144 );
PROVIDE ( mmu_init = 0x400095a4 );

PROVIDE ( esp_rom_spiflash_config_param = 0x40063238 );
PROVIDE ( esp_rom_spiflash_read_user_cmd = 0x400621b0 );
PROVIDE ( esp_rom_spiflash_prepare_encrypted_data = 0x40062e1c );
PROVIDE ( g_rom_flashchip = 0x3ffae270 );
PROVIDE ( g_rom_spiflash_dummy_len_plus = 0x3ffae290 );
PROVIDE ( esp_rom_spiflash_write_encrypted_enable = 0x40062df4 );
PROVIDE ( esp_rom_spiflash_write_encrypted_disable = 0x40062e60 );
PROVIDE ( esp_rom_spiflash_config_clk = 0x40062bc8 );
PROVIDE ( g_rom_spiflash_chip = 0x3ffae270 );

PROVIDE ( rom_i2c_writeReg = 0x400041a4 );
PROVIDE ( roundup2 = 0x4000ab7c );
PROVIDE ( _xtos_set_intlevel = 0x4000bfdc );
PROVIDE ( phy_get_romfuncs = 0x40004100 );
PROVIDE ( syscall_table_ptr_app = 0x3ffae020 );
PROVIDE ( syscall_table_ptr_pro = 0x3ffae024 );
PROVIDE ( g_ticks_per_us_pro = 0x3ffe01e0 );
PROVIDE ( g_ticks_per_us_app = 0x3ffe40f0 );

PROVIDE ( gpio_pad_select_gpio = 0x40009fdc );
PROVIDE ( gpio_pad_pullup = 0x4000a22c );
PROVIDE ( gpio_matrix_in = 0x40009edc );
PROVIDE ( gpio_matrix_out = 0x40009f0c );
PROVIDE ( uart_tx_flush = 0x40009258 );
PROVIDE ( uart_div_modify = 0x400090cc );
PROVIDE ( uart_tx_one_char = 0x40009200 );
PROVIDE ( uart_rx_one_char = 0x400092d0 );

PROVIDE ( esp_rom_gpio_pad_select_gpio    = gpio_pad_select_gpio );
PROVIDE ( esp_rom_gpio_pad_pullup_only    = gpio_pad_pullup );
PROVIDE ( esp_rom_gpio_pad_set_drv        = gpio_pad_set_drv );
PROVIDE ( esp_rom_gpio_pad_unhold         = gpio_pad_unhold );
PROVIDE ( esp_rom_gpio_connect_in_signal  = gpio_matrix_in );
PROVIDE ( esp_rom_gpio_connect_out_signal = gpio_matrix_out );

PROVIDE ( esp_rom_efuse_mac_address_crc8       = esp_crc8 );
PROVIDE ( esp_rom_efuse_get_flash_gpio_info    = ets_efuse_get_spiconfig );
PROVIDE ( esp_rom_efuse_is_secure_boot_enabled = ets_efuse_secure_boot_enabled );

PROVIDE ( esp_rom_uart_flush_tx       = uart_tx_flush );
PROVIDE ( esp_rom_uart_tx_one_char    = uart_tx_one_char );
PROVIDE ( esp_rom_uart_tx_wait_idle   = uart_tx_wait_idle );
PROVIDE ( esp_rom_uart_rx_one_char    = uart_rx_one_char );
PROVIDE ( esp_rom_uart_rx_string      = UartRxString );
PROVIDE ( esp_rom_uart_set_as_console = uart_tx_switch );
PROVIDE ( esp_rom_uart_putc           = ets_write_char_uart );

/* wpa_supplicant re-implements the MD5 functions: MD5Init, MD5Update, MD5Final */
/* so here we directly assign the symbols with the ROM API address */
PROVIDE ( esp_rom_md5_init   = 0x4005da7c );
PROVIDE ( esp_rom_md5_update = 0x4005da9c );
PROVIDE ( esp_rom_md5_final  = 0x4005db1c );

PROVIDE ( esp_rom_printf   = ets_printf );
PROVIDE ( esp_rom_delay_us = ets_delay_us );
PROVIDE ( esp_rom_install_uart_printf = ets_install_uart_printf );

PROVIDE ( esp_rom_crc32_le = crc32_le );

PROVIDE ( ets_get_detected_xtal_freq = 0x40008588 );
PROVIDE ( ets_update_cpu_frequency_rom = 0x40008550 );