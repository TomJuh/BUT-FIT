/**
 * Project:     VUT_ITU_Project
 * File name:   storage.js
 * Authors:     Roshka Andrei (xroshk00)
 * Description:
 */

import AsyncStorage from '@react-native-async-storage/async-storage';

export const storeData = async (key, value) => {
    try {
        return await AsyncStorage.setItem(key, JSON.stringify(value));
    } catch (e) {
        console.log(`storeData err: ${e}`);
    }
}

export const getData = async (key) => {
    try {
        const jsonValue = await AsyncStorage.getItem(key);
        return jsonValue !== null ? JSON.parse(jsonValue) : null;
    } catch (e) {
        console.log(`getData err: ${e}`);
    }
}
export const storeDataString = async (key, value) => {
    try {
        return await AsyncStorage.setItem(key, value);
    } catch (e) {
        console.log(`storeData err: ${e}`);
    }
}
export const getDataString = async (key) => {

    try {
        const value = await AsyncStorage.getItem(key);
        return value !== null ? value : null;
    } catch (e) {
        console.log(`getData err: ${e}`);
    }
}