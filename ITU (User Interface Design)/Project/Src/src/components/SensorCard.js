/**
 * Project:     VUT_ITU_Project
 * File name:   SensorCard.js
 * Authors:     Roshka Andrei (xroshk00)
 * Description: 
 */

import React from 'react';
import {Text, HStack, Box, Heading, VStack, Pressable, Icon} from 'native-base';
import {MaterialCommunityIcons} from '@expo/vector-icons';
import {useSelector} from 'react-redux';
import {toFahrenheit} from '../utils/utils';
import { getNearbySensors } from '../utils/api/fakeApi';

export default function SensorCard({sensor, handleDetails}) {
    const {isFahrenheit} = useSelector(state => state.settings);
    const {name, color, currData, connected} = (sensor => {
        const temp = isFahrenheit ? toFahrenheit(sensor.currData.temp) : sensor.currData.temp;

        return {
            ...sensor,
            currData: {
                ...sensor.currData,
                temp: temp.toPrecision(3),
                humi: sensor.currData.humi.toPrecision(2)
            }
        };
    })(sensor);
    const measurement = isFahrenheit ? '°F' : '°C';

    return (
        <Pressable
            onPress={handleDetails}
        >
            <Box
                bg={connected ? color?color:"gray.600": "gray.400"}
                py="4"
                px="3"
                rounded="2xl"
                width="100%"
                mt="3"
                shadow="5"
            >
                <VStack>
                    <HStack justifyContent="space-between" alignItems="center" px="2">
                        <Heading color="white" numberOfLines={2} maxWidth="90%">
                            {!!name ? name : 'Unnamed'}
                        </Heading>
                        <HStack>
                            {connected ?
                                <Icon as={MaterialCommunityIcons} name="bluetooth-connect" size="sm" color="white" />
                                :
                                <Icon as={MaterialCommunityIcons} name="bluetooth-off" size="sm" color="white" />
                            }
                        </HStack>
                    </HStack>
                    {currData && connected &&
                        <HStack justifyContent="space-between" mt="2">
                            <HStack alignItems="center">
                                <Text fontSize="xl" color="white">
                                    🌡 {currData.temp}{measurement}
                                </Text>
                            </HStack>
                            <HStack alignItems="center">
                                <Text fontSize="xl" color="white">
                                    💧 {currData.humi}%
                                </Text>
                            </HStack>
                            <HStack alignItems="center">
                                <Text fontSize="xl" color="white">
                                    ⚡ {currData.batt}%
                                </Text>
                            </HStack>
                        </HStack>
                    }
                </VStack>
            </Box>
        </Pressable>
    );
}