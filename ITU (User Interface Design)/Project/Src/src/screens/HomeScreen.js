/**
 * Project:     VUT_ITU_Project
 * File name:   HomeScreen.js
 * Authors:     Roshka Andrei (xroshk00)
 * Description: 
 */

import React, {useEffect} from 'react';
import {StatusBar} from 'expo-status-bar';
import {Heading, ScrollView, HStack, IconButton, Icon} from 'native-base';
import { Entypo } from "@expo/vector-icons";
import {NativeBaseProvider} from 'native-base/src/core/NativeBaseProvider';
import SensorCardList from '../components/SensorCardList';
import GroupCardList from '../components/GroupCardList';
import {useDispatch, useSelector} from 'react-redux';
import {fetchCustomColor, fetchCustomNames, fetchGroups, fetchPairedSensors, unpairSensor} from '../utils/redux/sensorsSlice';
import AsyncStorage from '@react-native-async-storage/async-storage';
import {fetchSettings} from '../utils/redux/settingsSlice';

export default function HomeScreen({navigation}) {
    const dispatch = useDispatch();
    const {pairedSensors} = useSelector(state => state.sensors);
    const handleDetails = (mac) => {
        navigation.navigate('SensorDetail', {sensorData: pairedSensors[mac]});
    }

    useEffect(() => {
        const _init = async () => {
            await dispatch(fetchPairedSensors());
            await dispatch(fetchCustomNames());
            await dispatch(fetchCustomColor());
            await dispatch(fetchSettings());
            await dispatch(fetchGroups());
            // await dispatch(unpairSensor('A1:C1:38:7D:FB:E2'));
            // AsyncStorage.multiRemove(['customNames', 'pairedSensors', 'settings']);
            // dispatch(addCustomName({mac: 'A0:C1:38:7D:FB:E2', name: 'My test 1'}));
        }
        _init();
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
                <StatusBar style="auto" />
                <HStack justifyContent="space-between" alignItems="center">
                    <Heading>
                        Paired sensors
                    </Heading>
                    <IconButton
                        onPress={() => navigation.navigate('PairScreen')}
                        icon={<Icon as={Entypo} name="plus" />}
                        borderRadius="full"
                        _icon={{
                            size: "sm"
                        }}
                    />
                </HStack>
                <GroupCardList handleDetails={handleDetails}/>
            </ScrollView>
        </NativeBaseProvider>
    );
}