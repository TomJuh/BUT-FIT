/**
 * Project:     VUT_ITU_Project
 * File name:   SensorCardList.js
 * Authors:     Roshka Andrei (xroshk00)
 * Description:
 */

import * as data from "./fakeData";
import getSensorData from "./fakeDataGen";
const maxDelay = 500;

async function getNearbySensors() {
  return new Promise((resolve, reject) => {
      setTimeout(
          () => {
              const result = {};
              data.nearbySensors.map(sensor => {
                  result[sensor.mac] = {mac: sensor.mac, name: sensor.name};
              })
              resolve(result);
          },
          Math.floor(Math.random() * maxDelay + 500)
      );
  })
}

async function pairWithSensor(mac) {
    return new Promise((resolve, reject) => {
        setTimeout(
            () => {
                const sensor = data.sensors.find(sensor => sensor.mac === mac);
                if (sensor) {
                    resolve(getSensorData(sensor));
                } else {
                    reject({message: 'Sensor is not reachable.'});
                }
            },
            Math.floor(Math.random() * maxDelay)
        );
    })
}

export {
    getNearbySensors,
    pairWithSensor,
};