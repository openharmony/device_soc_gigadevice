/*!
    \file    fmc_operation.c
    \brief   flash program, erase

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
    \version 2022-03-09, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "fmc_operation.h"
#include "gd32f4xx.h"

/*!
    \brief      get the sector number, size and range of the given address
    \param[in]  address: The flash address
    \param[out] none
    \retval     fmc_sector_info_struct: The information of a sector
*/
static fmc_sector_info_struct sector_info;
static uint32_t temp = 0x00000000U;
void fmc_sector_info_get_low_addr(uint32_t addr)
{
    /* bank0 area */
    temp = (addr - FMC_BANK0_START_ADDRESS) / SIZE_16KB;
    if (temp < 4U) {
        sector_info.sector_name = (uint32_t)temp;
        sector_info.sector_num = CTL_SN(temp);
        sector_info.sector_size = SIZE_16KB;
        sector_info.sector_start_addr = FMC_BANK0_START_ADDRESS + (SIZE_16KB * temp);
        sector_info.sector_end_addr = sector_info.sector_start_addr + SIZE_16KB - 1;
    } else if (temp < 8U) {
        sector_info.sector_name = 0x00000004U;
        sector_info.sector_num = CTL_SN(0x4);
        sector_info.sector_size = SIZE_64KB;
        sector_info.sector_start_addr = 0x08010000U;
        sector_info.sector_end_addr = 0x0801FFFFU;
    } else {
        temp = (addr - FMC_BANK0_START_ADDRESS) / SIZE_128KB;
        sector_info.sector_name = (uint32_t)(temp + 0x4);
        sector_info.sector_num = CTL_SN(temp + 0x4);
        sector_info.sector_size = SIZE_128KB;
        sector_info.sector_start_addr = FMC_BANK0_START_ADDRESS + (SIZE_128KB * temp);
        sector_info.sector_end_addr = sector_info.sector_start_addr + SIZE_128KB - 1;
    }
}
fmc_sector_info_struct fmc_sector_info_get(uint32_t addr)
{
    if ((FMC_START_ADDRESS <= addr) && (FMC_END_ADDRESS >= addr)) {
        if ((FMC_BANK1_START_ADDRESS > addr)) {
            fmc_sector_info_get_low_addr(addr);
        } else {
            /* bank1 area */
            temp = (addr - FMC_BANK1_START_ADDRESS) / SIZE_16KB;
            if (temp < 4U) {
                sector_info.sector_name = (uint32_t)(temp + 0xC);
                sector_info.sector_num = CTL_SN(temp + 0x10);
                sector_info.sector_size = SIZE_16KB;
                sector_info.sector_start_addr = FMC_BANK0_START_ADDRESS + (SIZE_16KB * temp);
                sector_info.sector_end_addr = sector_info.sector_start_addr + SIZE_16KB - 1;
            } else if (temp < 8U) {
                sector_info.sector_name = 0x00000010;
                sector_info.sector_num = CTL_SN(0x14);
                sector_info.sector_size = SIZE_64KB;
                sector_info.sector_start_addr = 0x08110000U;
                sector_info.sector_end_addr = 0x0811FFFFU;
            } else if (temp < 64U) {
                temp = (addr - FMC_BANK1_START_ADDRESS) / SIZE_128KB;
                sector_info.sector_name = (uint32_t)(temp + 0x10);
                sector_info.sector_num = CTL_SN(temp + 0x14);
                sector_info.sector_size = SIZE_128KB;
                sector_info.sector_start_addr = FMC_BANK1_START_ADDRESS + (SIZE_128KB * temp);
                sector_info.sector_end_addr = sector_info.sector_start_addr + SIZE_128KB - 1;
            } else {
                temp = (addr - FMC_BANK1_START_ADDRESS) / SIZE_256KB;
                sector_info.sector_name = (uint32_t)(temp + 0x14);
                sector_info.sector_num = CTL_SN(temp + 0x8);
                sector_info.sector_size = SIZE_256KB;
                sector_info.sector_start_addr = FMC_BANK1_START_ADDRESS + (SIZE_256KB * temp);
                sector_info.sector_end_addr = sector_info.sector_start_addr + SIZE_256KB - 1;
            }
        }
    } else {
        /* invalid address */
        sector_info.sector_name = FMC_WRONG_SECTOR_NAME;
        sector_info.sector_num = FMC_WRONG_SECTOR_NUM;
        sector_info.sector_size = FMC_INVALID_SIZE;
        sector_info.sector_start_addr = FMC_INVALID_ADDR;
        sector_info.sector_end_addr = FMC_INVALID_ADDR;
    }
    return sector_info;
}

/*!
    \brief      get the sector number by a given sector name
    \param[in]  address: a given sector name
    \param[out] none
    \retval     uint32_t: sector number
*/
uint32_t sector_name_to_number(uint32_t sector_name)
{
    if (sector_name <= 0xB) {
        return CTL_SN(sector_name);
    } else if (sector_name <= 0x17) {
        return CTL_SN(sector_name + 0x4);
    } else if (sector_name <= 0x1B) {
        return CTL_SN(sector_name - 0xC);
    } else {
        while (1) {
            ;
        }
    }
}

/*!
    \brief      erases the sector of a given address
    \param[in]  address: a given address
    \param[out] none
    \retval     none
*/
void fmc_erase_sector_by_address(uint32_t address)
{
    fmc_sector_info_struct sector_info;
    /* get information about the sector in which the specified address is located */
    sector_info = fmc_sector_info_get(address);
    if (FMC_WRONG_SECTOR_NAME == sector_info.sector_name) {
        while (1) {
            ;
        }
    } else {
        /* unlock the flash program erase controller */
        fmc_unlock();
        /* clear pending flags */
        fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
        /* wait the erase operation complete */
        if (FMC_READY != fmc_sector_erase(sector_info.sector_num)) {
            while (1) {
                ;
            }
        }
        /* lock the flash program erase controller */
        fmc_lock();
    }
}

/*!
    \brief      write 8 bit length data to a given address
    \param[in]  address: a given address(0x08000000~0x082FFFFF)
    \param[in]  length: data length
    \param[in]  data_8: data pointer
    \param[out] none
    \retval     none
*/
void fmc_write_8bit_data(uint32_t address, uint16_t length, int8_t *data_8)
{
    fmc_sector_info_struct start_sector_info;
    fmc_sector_info_struct end_sector_info;
    uint32_t sector_num, i;
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    /* write data_8 to the corresponding address */
    for (i = 0; i < length; i++) {
        if (FMC_READY == fmc_byte_program(address, data_8[i])) {
            address++;
        } else {
            while (1) {
                ;
            }
        }
    }
    /* lock the flash program erase controller */
    fmc_lock();
}

/*!
    \brief      read 8 bit length data to a given address
    \param[in]  address: a given address(0x08000000~0x082FFFFF)
    \param[in]  length: data length
    \param[in]  data_8: data pointer
    \param[out] none
    \retval     none
*/
void fmc_read_8bit_data(uint32_t address, uint16_t length, int8_t *data_8)
{
    uint16_t i;
   
    for (i = 0; i < length; i++) {
        data_8[i] = *(__IO int8_t *)address;
        address++;
    }
}
