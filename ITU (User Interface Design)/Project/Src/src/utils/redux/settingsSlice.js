/**
 * Project:     VUT_ITU_Project
 * File name:   settingsSlice.js
 * Authors:     Roshka Andrei (xroshk00)
 * Description:
 */

import {createAsyncThunk, createSlice} from '@reduxjs/toolkit';
import {getData, storeData} from '../storage/storage';

const defaultSettings = {
    pollingInterval: 3600,
    isFahrenheit: false,
    language: 'en'
};

export const pollingIntervals = {
    3600: 'Every hour',
    [3600 * 3]: 'Every 3 hours',
    [3600 * 6]: 'Every 6 hours',
    [3600 * 12]: 'Every 12 hours',
    86400: 'Every day'
};

export const fetchSettings = createAsyncThunk(
    'fetchSettings',
    async (_, thunkAPI) => {
        const settings = await getData('settings');

        if (settings) {
            thunkAPI.dispatch(mergeSettings(settings));
        } else {
            thunkAPI.dispatch(mergeSettings(defaultSettings));
            await storeData('settings', thunkAPI.getState().settings);
        }
    }
);

export const updateSettings = createAsyncThunk(
    'updateSettings',
    async (settingsNew, thunkAPI) => {
        thunkAPI.dispatch(mergeSettings(settingsNew));
        await storeData('settings', thunkAPI.getState().settings);
    }
);

const settingsSlice = createSlice({
    name: 'settings',
    initialState: {},
    reducers: {
        mergeSettings: (state, action) => {
            return Object.assign(state, action.payload);
        }
    }
});

export const {
    mergeSettings,
} = settingsSlice.actions;

export default settingsSlice.reducer;