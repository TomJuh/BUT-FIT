/**
 * Project:     VUT_ITU_Project
 * File name:   PairScreen.js
 * Authors:     Roshka Andrei (xroshk00)
 * Description: 
 */

import React, {useEffect} from 'react';
import {Heading, HStack, Icon, IconButton, ScrollView, Text, useToast} from 'native-base';
import {NativeBaseProvider} from 'native-base/src/core/NativeBaseProvider';
import {MaterialCommunityIcons} from '@expo/vector-icons';

import UnpairedSensorList from '../components/UnpairedSensorList';
import {useDispatch, useSelector} from 'react-redux';
import {fetchNearbySensors} from '../utils/redux/sensorsSlice';

export default function PairScreen() {
    const {nearbySensors, pairedSensors, isLoading} = useSelector(state => state.sensors);
    const dispatch = useDispatch();

    const handleDiscover = async () => {
        dispatch(fetchNearbySensors());
    };

    useEffect( () => {
        dispatch(fetchNearbySensors());
    }, []);

    return (
        <NativeBaseProvider>
            <ScrollView
                _contentContainerStyle={{
                    px: "15px",
                    py: "20px",
                    mb: "4",
                    minW: "72",
                }}
            >
                <HStack justifyContent="space-between" alignItems="center">
                    <Heading>
                        Discovered sensors
                    </Heading>
                    <IconButton
                        onPress={() => handleDiscover()}
                        icon={<Icon as={MaterialCommunityIcons} name="sync" />}
                        borderRadius="full"
                        _icon={{
                            size: "sm"
                        }}
                    />
                </HStack>

                {isLoading &&
                    <Text>Searching sensors...</Text>
                }

                <UnpairedSensorList nearbySensors={nearbySensors} pairedSensors={pairedSensors} />
            </ScrollView>
        </NativeBaseProvider>
    );
}