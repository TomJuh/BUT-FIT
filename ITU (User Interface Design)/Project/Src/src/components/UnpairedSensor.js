/**
 * Project:     VUT_ITU_Project
 * File name:   UnpairedSensor.js
 * Authors:     Roshka Andrei (xroshk00)
 * Description: 
 */

import React, {useEffect, useLayoutEffect} from 'react';
import {Button, HStack, Icon, Text, useToast} from 'native-base';
import { MaterialCommunityIcons } from '@expo/vector-icons';
import {pairWithSensor} from '../utils/redux/sensorsSlice';
import {useDispatch, useSelector} from 'react-redux';

export default function UnpairedSensor({sensor, isPaired}) {
    const dispatch = useDispatch();
    const toast = useToast();
    const {name, mac} = sensor;
    const activeToasts = [];

    const handlePair = async (mac) => {
        const action = await dispatch(pairWithSensor(mac));
        if (action.payload) {
            activeToasts.push(toast.show({
                description: action.payload
            }));
        }
    };

    useEffect(() => {
        activeToasts.map(toast => toast.close(toast));
    }, []);

    return (
        <HStack px="2" justifyContent="space-between" alignItems="center" mt="2">
            <HStack alignItems="center">
                <Icon as={MaterialCommunityIcons} name={isPaired ? "bluetooth-connect" : "bluetooth"} size="sm"/>
                <Text ml="3" fontSize="lg">{name ? name : 'Unnamed'}</Text>
            </HStack>
            {isPaired ?
                <Text>Paired</Text>
                :
                <Button
                    onPress={() => handlePair(mac)}
                    isLoading={sensor._isLoading}
                >
                    Pair
                </Button>
            }
        </HStack>
    );
}