/**
 * Project:     VUT_ITU_Project
 * File name:   fakeData.js
 * Authors:     Lukáš Nevrka (xnevrk03)
 * Description:
 */

/**
 * Paired sensors (not all must be connected)
 */
export const sensors = [
  {
      "name": "new-sensor-1",
      "mac": "A0:C1:38:7D:FB:E2",
      "connected": true,
  },
  {
      "name": "new-sensor-2",
      "mac": "A1:C1:38:7D:FB:E2",
      "connected": true,
  },
  {
      "name": "new-sensor-3",
      "mac": "A2:C1:38:7D:FB:E2",
      "connected": false,
  },
  {
      "name": "new-sensor-4",
      "mac": "A3:C1:38:7D:FB:E2",
      "connected": false,
  },
]

/**
 * Sensors that were currently found via Bluetooth
 * History data on lower indexes are the newest
 */

export const nearbySensors = [
  {
      "name": "new-sensor-1",
      "mac": "A0:C1:38:7D:FB:E2",
  },
  {
      "name": "new-sensor-2",
      "mac": "A1:C1:38:7D:FB:E2",
  },
  {
      "name": "new-sensor-3",
      "mac": "A2:C1:38:7D:FB:E2",
  },
  {
      "name": "new-sensor-4",
      "mac": "A3:C1:38:7D:FB:E2",
  },
  {
      "name": "",
      "mac": "A4:C1:38:7D:FB:E2",
  },
]