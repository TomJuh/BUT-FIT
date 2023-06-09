/**
 * Project:     VUT_ITU_Project
 * File name:   sensorsSlice.js
 * Authors:     Roshka Andrei (xroshk00)
 * Description:
 */

import {createAsyncThunk, createSlice} from '@reduxjs/toolkit';
import * as api from '../api/fakeApi';
import {getData, storeData} from '../storage/storage';

export const fetchNearbySensors = createAsyncThunk(
    'fetchNearbySensors',
    async (data, thunkAPI) => {
        thunkAPI.dispatch(loading(true));
        thunkAPI.dispatch(setNearbySensors(await api.getNearbySensors()));
        thunkAPI.dispatch(loading(false));
    }
);

export const fetchPairedSensors = createAsyncThunk(
    'fetchPairedSensors',
    async (data, thunkAPI) => {
        thunkAPI.dispatch(loading(true));
        thunkAPI.dispatch(setPairedSensors(await getData('pairedSensors')));
        thunkAPI.dispatch(loading(false));
    }
);

export const pairWithSensor = createAsyncThunk(
    'pairWithSensor',
    async (mac, thunkAPI) => {
        thunkAPI.dispatch(loadingNearbySensor({mac, value: true}));
        const sensor = await api.pairWithSensor(mac).catch(e => {
            thunkAPI.dispatch(error(e.message));
            return null;
        });

        if (sensor) {
            thunkAPI.dispatch(insertPairedSensor(sensor));
            const state = thunkAPI.getState().sensors;
            await storeData('pairedSensors', state.pairedSensors);
        }

        thunkAPI.dispatch(loadingNearbySensor({mac, value: false}));
        return thunkAPI.getState().sensors.error;
    }
);
export const fetchCustomColor = createAsyncThunk(
    'fetchCustomColor',
    async (mac, thunkAPI) => {
        thunkAPI.dispatch(setCustomColors(await getData('customColors')));
    }
);

export const addCustomColor = createAsyncThunk(
    'fetchCustomColor',
    async (data, thunkAPI) => {
        thunkAPI.dispatch(insertCustomColor(data));
        const state = thunkAPI.getState().sensors;
        await storeData('customColors', state.customColors);
    }
);
export const fetchGroups= createAsyncThunk(
    'fetchGroups',
    async (mac, thunkAPI) => {
        thunkAPI.dispatch(setSelGroup(await getData('selGroup')));
    }
);

export const assignSelGroup= createAsyncThunk(
    'assignGroup',
    async (data, thunkAPI) => {
        thunkAPI.dispatch(setGroup(data));
        const state = thunkAPI.sensors;
        await storeData('selGroup', state.cSelGroup);
    }
);
export const fetchCustomNames = createAsyncThunk(
    'fetchCustomNames',
    async (mac, thunkAPI) => {
        thunkAPI.dispatch(setCustomNames(await getData('customNames')));
    }
);

export const addCustomName = createAsyncThunk(
    'fetchCustomNames',
    async (data, thunkAPI) => {
        thunkAPI.dispatch(insertCustomName(data));

        const state = thunkAPI.getState().sensors;

        await storeData('customNames', state.customNames);
    }
);

export const unpairSensor = createAsyncThunk(
    'unpairSensor',
    async (mac, thunkAPI) => {
        thunkAPI.dispatch(deletePairedSensor(mac));
        const state = thunkAPI.getState().sensors;
        await storeData('pairedSensors', state.pairedSensors);
    }
);

const sensorsSlice = createSlice({
    name: 'sensors',
    initialState: {
        cSelGroup: "",
        nearbySensors: null,
        pairedSensors: null,
        customNames: null,
        customColors: null,
        isLoading: false,
        error: null
    },
    reducers: {
        setNearbySensors: (state, action) => {
            state.nearbySensors = action.payload;
        },
        setPairedSensors: (state, action) => {
            state.pairedSensors = action.payload;
        },
        setCustomNames: (state, action) => {
            state.customNames = action.payload;
        },
        setCustomColors: (state, action) => {
            state.customColors= action.payload;
        },
        setSelGroup: (state, action) => {
            state.cSelGroup = action.payload;
        },
        insertPairedSensor: (state, action) => {
            state.pairedSensors = {
                ...state.pairedSensors,
                [action.payload.mac]: action.payload
            };
        },
        insertCustomName: (state, action) => {
            state.customNames = {
                ...state.customNames,
                [action.payload.mac]: action.payload.name
            };
        },
        insertCustomColor: (state, action) => {
            state.customColors = {
                ...state.customColors,
                [action.payload.mac]: action.payload.color
            };
        },
        setGroup: (state, action) => {
            state.cSelGroup = {
                ...state.cSelGroup,
                [action.payload.mac]: action.payload.selGroup
            };
        },
        deletePairedSensor: (state, action) => {
            delete state.pairedSensors[action.payload];
        },
        loading: (state, action) => {
            state.isLoading = action.payload;
        },
        loadingNearbySensor: (state, action) => {
            state.nearbySensors[action.payload.mac]._isLoading = action.payload.value;
        },
        error: (state, action) => {
            state.error = action.payload;
        }
    }
});

export const {
    setNearbySensors,
    loadingNearbySensor,
    setPairedSensors,
    setCustomNames,
    setCustomColors,
    setSelGroup,
    insertPairedSensor,
    deletePairedSensor,
    insertCustomName,
    insertCustomColor,
    setGroup,
    loading,
    error
} = sensorsSlice.actions;

export default sensorsSlice.reducer;