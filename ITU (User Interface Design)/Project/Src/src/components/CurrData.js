/**
 * Project:     VUT_ITU_Project
 * File name:   CurrData.js
 * Authors:     Lukáš Nevrka (xnevrk03)
 * Description: 
 */

import React from 'react';
import {
    HStack, VStack, Box, Center,
    Text,
} from "native-base"
import { useSelector } from 'react-redux';

import {toFahrenheit} from '../utils/utils'

export default function CurrData({ currData, connected }) {
    const { isFahrenheit, pollingInterval } = useSelector(state => state.settings);

    return (

        <Center my="5">
            <VStack space="5">
                <HStack space="5">
                    <Box
                        rounded="2xl"
                        bg={connected ? "#fb923c" : "muted.300"}
                        space="10"
                        shadow="5"
                        minWidth="35%"
                    >
                        <Center px="5" py="3">
                            <Text fontSize="3xl">🌡</Text>
                            <Text fontSize="3xl" color="coolGray.800">
                                {connected ? 
                                    (isFahrenheit ? 
                                        ((toFahrenheit(currData.temp).toPrecision(3)) + "°F") : 
                                        currData.temp.toPrecision(3) + "°C") 
                                    : "-"}
                            </Text>
                        </Center>
                    </Box>

                    <Box
                        rounded="2xl"
                        bg={connected ? "#7dd3fc" : "muted.300"}
                        space="10"
                        shadow="5"
                        minWidth="35%"
                    >
                        <Center px="5" py="3">
                            <Text fontSize="3xl">💧</Text>
                            <Text fontSize="3xl">{connected ? currData.humi.toPrecision(2) + "%" : "-"}</Text>
                        </Center>
                    </Box>
                </HStack>
            </VStack>
        </Center>
    );
}