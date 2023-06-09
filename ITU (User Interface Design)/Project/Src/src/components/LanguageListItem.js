import React from 'react';
import {Box, VStack, Text} from 'native-base';
import {TouchableOpacity} from 'react-native-gesture-handler';
import {Alert} from 'react-native';
import {updateSettings} from '../utils/redux/settingsSlice';
import {useDispatch} from 'react-redux';

export default function LanguageListItem({locale, name, englishName, isActive}) {
    const dispatch = useDispatch();
    const key = locale.toString();
    const handleUpdate = () => {
        Alert.alert(
            `Change language to ${englishName}?`,
            null,
            [
                {text: 'Cancel', style: 'cancel'},
                {
                    text: 'Change',
                    onPress: () => changeLanguage(),
                    style: 'destructive'
                }
            ]
        );
    };
    const changeLanguage = async () => {
        await dispatch(updateSettings({language: key}));
    };

    return (
        <TouchableOpacity
            onPress={handleUpdate}
        >
            <Box
                flexDirection="row"
                alignItems="center"
                px={4}
                py={2}
                bg={isActive ? 'gray.200' : null}
            >
                <VStack>
                    <Text fontSize="18" color={isActive ? 'primary.500' : '#434343'}>
                        {name}
                    </Text>
                    <Text color="#AAAAAA">
                        {englishName}
                    </Text>
                </VStack>
            </Box>
        </TouchableOpacity>
    );
}