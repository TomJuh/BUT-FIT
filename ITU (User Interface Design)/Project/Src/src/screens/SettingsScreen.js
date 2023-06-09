/**
 * Project:     VUT_ITU_Project
 * File name:   SettingScreen.js
 * Authors:     Juhász Tomáš (xjuhas04)
 * Description: 
 */

import React from 'react';
import {Alert, Pressable} from 'react-native';
import {MaterialCommunityIcons} from '@expo/vector-icons';
import {useDispatch, useSelector} from 'react-redux';

import {
    Heading,
    VStack,
    ScrollView,
    Text,
    Switch,
    HStack, Icon, Select, Box
} from 'native-base';
import {pollingIntervals, updateSettings} from '../utils/redux/settingsSlice';


export default function SettingsScreen({navigation}) {
    const dispatch = useDispatch();
    const {isFahrenheit, pollingInterval} = useSelector(state => state.settings);

    const handleUpdates = () => {

        Alert.alert(
            'No new updates',
            'You have the newest version of this app',
            [
                {text: 'OK'}
            ]
        );
    };

    const toggleMeasurement = async () => {
        await dispatch(updateSettings({isFahrenheit: !isFahrenheit}));
    };

    const setPollingInterval = async (interval) => {
        await dispatch(updateSettings({pollingInterval: interval}));
    }

    return (
        <ScrollView style={{flex: 1}} showsVerticalScrollIndicator={false} px="3">
            <VStack
                space="25"
                px="2"
                mt="4"
            >
                <SettingsSection heading="Devices">
                    <ButtonArrow
                        title="Polling interval"
                        onPress={() => navigation.navigate('Notifications')}
                        customRight={
                            <Box
                                bg="gray.200"
                                borderRadius="2xl"
                            >
                                <Select
                                    selectedValue={pollingInterval.toString()}
                                    onValueChange={value => setPollingInterval(value)}
                                    minWidth={`${pollingInterval.toString().length * 28}`}
                                    fontSize="sm"
                                    borderWidth="0"
                                >
                                    {
                                        Object.keys(pollingIntervals).map(interval => (
                                            <Select.Item key={interval} label={pollingIntervals[interval].toLowerCase()} value={interval}/>
                                        ))
                                    }
                                </Select>
                            </Box>
                        }
                    />
                </SettingsSection>

                <SettingsSection heading="Updates">
                    <ButtonArrow title="Check for updates" onPress={handleUpdates}/>
                </SettingsSection>

                <SettingsSection heading="Language">
                    <ButtonArrow title="Language" onPress={() => navigation.navigate('Language')}/>
                </SettingsSection>

                <SettingsSection heading="Measurement">
                    <ButtonArrow
                        title="Use fahrenheit"
                        onPress={toggleMeasurement}
                        customRight={<Switch onToggle={toggleMeasurement} isChecked={isFahrenheit} />}
                    />
                </SettingsSection>

                <SettingsSection heading="About">
                    <ButtonArrow title="About this app" onPress={() => navigation.navigate('About')}/>
                    <ButtonArrow title="Terms of use" onPress={() => navigation.navigate('Terms Of Use')}/>
                </SettingsSection>
            </VStack>
        </ScrollView>
    );
}

const SettingsSection = ({heading, children}) => {
  return (
      <>
          <Heading size="sm" color="grey">{heading}</Heading>
          <VStack ml="4" mt="4" space="md">
              {children}
          </VStack>
      </>
  );
};

const ButtonArrow = ({title, onPress, customRight}) => {
    return (
        <Pressable size="lg" variant="ghost" onPress={onPress}>
            <HStack justifyContent="space-between" alignItems="center">
                <Text fontSize="18">{title}</Text>
                {
                    customRight ??
                    <Icon
                        as={MaterialCommunityIcons}
                        name="chevron-right"
                        size="25"
                    />
                }
            </HStack>
        </Pressable>
    );
};