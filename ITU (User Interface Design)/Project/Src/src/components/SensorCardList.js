/**
 * Project:     VUT_ITU_Project
 * File name:   SensorCardList.js
 * Authors:     Roshka Andrei (xroshk00)
 * Description: 
 */

import React from 'react';
import SensorCard from './SensorCard';
import {useSelector} from 'react-redux';
import {Text,Box} from 'native-base';

export default function SensorCardList({handleDetails}) {
    const {pairedSensors, customNames, customColors} = useSelector(state => {
        const {pairedSensors} = state.sensors;
        let pairedSensorsArray = pairedSensors ? Object.values(pairedSensors) : null;
        if (pairedSensorsArray) {
            pairedSensorsArray = pairedSensorsArray.sort((a, b) =>
                (a.connected === b.connected) ? 0 : !a.connected
            );
        }

        return {
            ...state.sensors,
            pairedSensors: pairedSensorsArray
        }
    });

    return (
        <>
            {pairedSensors && !!pairedSensors.length ?
                pairedSensors.map((sensor) => (
                    <SensorCard
                        key={sensor.mac}
                        sensor={{...sensor, name: customNames && customNames[sensor.mac] ? customNames[sensor.mac] : sensor.name,color: customColors && customColors[sensor.mac] ? customColors[sensor.mac] : sensor.color}}
                        isPaired={true}
                        handleDetails={() => handleDetails(sensor.mac)}
                    />
                ))
                :
                <Text>You have no paired sensors yet.</Text>
            }
        </>
    );
}