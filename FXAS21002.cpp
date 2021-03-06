/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, NXP
 * Copyright 2022, Lorenzo Calisti
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "FXAS21002.h"

FXAS21002::FXAS21002(PinName sda, PinName scl) : _gyro_i2c(sda, scl) {}

FXAS21002::~FXAS21002() {}

void FXAS21002::gyro_config()
{
  char d[2];

  // Puts device in standby mode
  d[0] = FXAS21002_CTRL_REG1;
  d[1] = 0x08;
  _gyro_i2c.write(FXAS21002_I2C_ADDRESS, d, 2);

  // sets FS =+/- 2000 dps
  d[0] = FXAS21002_CTRL_REG0;
  d[1] = 0x00;
  _gyro_i2c.write(FXAS21002_I2C_ADDRESS, d, 2);

  // Puts device in active mode
  d[0] = FXAS21002_CTRL_REG1;
  d[1] = 0x0A;
  _gyro_i2c.write(FXAS21002_I2C_ADDRESS, d, 2);
}

void FXAS21002::acquire_gyro_data_dps(float *g_data)
{
  char data_bytes[7];

  // Read the 6 data bytes - LSB and MSB for X, Y and Z Axes.
  _gyro_i2c.write(FXAS21002_I2C_ADDRESS, FXAS21002_STATUS, 1, true);
  _gyro_i2c.read(FXAS21002_I2C_ADDRESS, data_bytes, 7);

  g_data[0] = (float)((int16_t)((data_bytes[1] * 256) + (data_bytes[2]))) * 0.0625;
  g_data[1] = (float)((int16_t)((data_bytes[3] * 256) + (data_bytes[4]))) * 0.0625;
  g_data[2] = (float)((int16_t)((data_bytes[5] * 256) + (data_bytes[6]))) * 0.0625;
}
