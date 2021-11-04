/**
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie Firmware
 *
 * Copyright (C) 2018 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * ranges.c: Centralize range measurements for different directions
 *           and make them available as log
 */
#include <stdint.h>

#include "log.h"

#include "debug.h"
#include "range.h"
#include "stabilizer_types.h"
#include "estimator.h"

static uint16_t ranges[RANGE_T_END] = {0,};
static uint16_t roiIndex[1] = {0};
static uint16_t topLeftX[1] = {0};
static uint16_t topLeftY[1] = {0};
static uint16_t botRightX[1] = {0};
static uint16_t botRightY[1] = {0};
static uint16_t orientation[1] = {0};

void rangeSet(rangeDirection_t direction, float range_m)
{
  if (direction > (RANGE_T_END-1)) return;

  ranges[direction] = range_m * 1000;
  // DEBUG_PRINT("[INFO]range_m in rangeSet(): %f\n", range_m);
}

void roiSet(uint16_t index)
{
  roiIndex[0] = index;
  DEBUG_PRINT("[INFO]roiIndex in roiSet(): %d\n", roiIndex[0]);
}

void topLeftXSet(uint16_t index)
{
  topLeftX[0] = index;
}

void topLeftYSet(uint16_t index)
{
  topLeftY[0] = index;
}

void botRightXSet(uint16_t index)
{
  botRightX[0] = index;
}

void botRightYSet(uint16_t index)
{
  botRightY[0] = index;
}

void orientationSet(uint16_t index)
{
  orientation[0] = index;
}

float rangeGet(rangeDirection_t direction)
{
  if (direction > (RANGE_T_END-1)) return 0;

  return ranges[direction];
}

void rangeEnqueueDownRangeInEstimator(float distance, float stdDev, uint32_t timeStamp) {
  tofMeasurement_t tofData;
  tofData.timestamp = timeStamp;
  tofData.distance = distance;
  tofData.stdDev = stdDev;
  estimatorEnqueueTOF(&tofData);
}

/**
 * Log group for the multi ranger and Z-ranger decks
 */
LOG_GROUP_START(range)
/**
 * @brief Distance from the front sensor to an obstacle [mm]
 */
LOG_ADD_CORE(LOG_UINT16, front, &ranges[rangeFront])

/**
 * @brief Distance from the back sensor to an obstacle [mm]
 */
LOG_ADD_CORE(LOG_UINT16, back, &ranges[rangeBack])

/**
 * @brief Distance from the top sensor to an obstacle [mm]
 */
LOG_ADD_CORE(LOG_UINT16, up, &ranges[rangeUp])

/**
 * @brief Distance from the left sensor to an obstacle [mm]
 */
LOG_ADD_CORE(LOG_UINT16, left, &ranges[rangeLeft])

/**
 * @brief Distance from the right sensor to an obstacle [mm]
 */
LOG_ADD_CORE(LOG_UINT16, right, &ranges[rangeRight])

/**
 * @brief Distance from the Z-ranger (bottom) sensor to an obstacle [mm]
 */
LOG_ADD_CORE(LOG_UINT16, zrange, &ranges[rangeDown])

/**
 * @brief Distance from the certain sensor to an obstacle [mm]
 */
LOG_ADD_CORE(LOG_UINT16, single, &ranges[rangeSingle])

/**
 * @brief Index of selected ROI
 */
LOG_ADD_CORE(LOG_UINT16, roiindex, &roiIndex[0])

/**
 * @brief Top-left point's x-coordinate of the ROI
 */
LOG_ADD_CORE(LOG_UINT16, topleftx, &topLeftX[0])

/**
 * @brief Top-left point's y-coordinate of the ROI
 */
LOG_ADD_CORE(LOG_UINT16, toplefty, &topLeftY[0])

/**
 * @brief Bottom-right point's x-coordinate of the ROI
 */
LOG_ADD_CORE(LOG_UINT16, botrightx, &botRightX[0])

/**
 * @brief Bottom-right point's y-coordinate of the ROI
 */
LOG_ADD_CORE(LOG_UINT16, botrighty, &botRightY[0])

/**
 * @brief TODO, orientation selection
 */
LOG_ADD_CORE(LOG_UINT16, orientation, &orientation[0])

LOG_GROUP_STOP(range)
