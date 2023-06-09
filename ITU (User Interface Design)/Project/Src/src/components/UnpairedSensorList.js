/**
 * Project:     VUT_ITU_Project
 * File name:   UnpairedSensorList.js
 * Authors:     Roshka Andrei (xroshk00)
 * Description: 
 */

import React from 'react';
import UnpairedSensor from './UnpairedSensor';

export default function UnpairedSensorList({nearbySensors, pairedSensors}) {
    return (
        <>
            {nearbySensors &&
                Object.values(nearbySensors).map(sensor => {
                    return (
                        <UnpairedSensor
                            key={sensor.mac}
                            sensor={sensor}
                            isPaired={!!pairedSensors && pairedSensors[sensor.mac] !== undefined}
                        />
                    );
                })
            }
        </>
    );
}