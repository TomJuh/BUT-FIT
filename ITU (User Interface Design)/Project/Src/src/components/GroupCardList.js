/**
 * Project:     VUT_ITU_Project
 * File name:   SensorCardList.js
 * Authors:     Juhász Tomáš (xjuhas04)     
 *              Roshka Andrei (xroshk00)
 * Description: 
 */


import React from 'react';
import SensorCard from './SensorCard';
import {useSelector} from 'react-redux';
import {Text,Box} from 'native-base';

export default function SensorCardList({handleDetails}) {
       var toDelete = []; 
        const {groups} = useSelector(state => state.settings);
        const {pairedSensors, customNames, customColors,cSelGroup} = useSelector(state => {
        const {pairedSensors} = state.sensors;
        let pairedSensorsArray = pairedSensors ? Object.values(pairedSensors) : null;
        if (pairedSensorsArray) {
            pairedSensorsArray = pairedSensorsArray.sort((a, b) =>
                (a.connected === b.connected) ? 0 : !a.connected
            );
        }
        toDelete = pairedSensorsArray; 
        return {
            ...state.sensors,
            pairedSensors: pairedSensorsArray
        }
    });

    var others = []
    var usedGroups = []
    if (cSelGroup)
    {
                for (var i in groups)
                {
                    if (Object.values(cSelGroup).indexOf(groups[i]) > -1)
                    {
                        usedGroups.push(groups[i])
                    }
                {

                }
 

                }

        for (var obj in toDelete)
        {
            var found = false;
            for (var group in cSelGroup)
            {
                if (toDelete[obj].mac == group)
                {
                    found = true;
                    break;
                }
            }
            if (found == false)
            {
                others.push(toDelete[obj])
            }

        }
        }
        else
        {
            others = toDelete;
        }
    return (
        <>
            {usedGroups&&  !!usedGroups.length ?
                usedGroups.map((group) => (
                 <Box
                                    bg={"blue.600"}
                                    py="4"
                                    px="3"
                                    rounded="2xl"
                                    width="100%"
                                    mt="3"
                                    shadow="5">{group}  
                                   <>
                                    {pairedSensors && !!pairedSensors.length ?
                                    pairedSensors.map((sensor) => (
                                        <>
                                        {(cSelGroup ?cSelGroup[sensor.mac]??null:null) === group ? 
                                        <SensorCard
                                            key={sensor.mac}
                                            sensor={{...sensor, name: customNames && customNames[sensor.mac] ? customNames[sensor.mac] : sensor.name,color: customColors && customColors[sensor.mac] ? customColors[sensor.mac] : sensor.color}}
                                            isPaired={true}
                                            handleDetails={() => handleDetails(sensor.mac)}
                                        />
                                        :
                                            <Text></Text>
                                        }
                                        </>
                                    ))
                                    :
                                    <Text></Text>
                                    }
                                                       
                    </>   
                    
                </Box> 
                ))

                :
            
                <Text></Text>


            }

<>
            {others && !!others.length ?
                others.map((sensor) => (
                    <SensorCard
                        key={sensor.mac}
                        sensor={{...sensor, name: customNames && customNames[sensor.mac] ? customNames[sensor.mac] : sensor.name,color: customColors && customColors[sensor.mac] ? customColors[sensor.mac] : sensor.color}}
                        isPaired={true}
                        handleDetails={() => handleDetails(sensor.mac)}
                    />
                ))
                :
                <>
                {cSelGroup ?
                <Text></Text>
                :
                <Text>You have no paired sensors yet.</Text>
                }
                </>
            }
        </>
        </>
    );

}