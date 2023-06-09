/**
 * Project:     VUT_ITU_Project
 * File name:   App.js
 * Authors:     Andrei Roshka (xroshk00)
 * Description:
 */

import React from 'react';
import {NavigationContainer} from '@react-navigation/native';
import {createNativeStackNavigator} from '@react-navigation/native-stack';
import {Icon, IconButton, Button, NativeBaseProvider} from 'native-base';
import {Provider} from 'react-redux';

import SensorDetailScreen from './src/screens/SensorDetailScreen.js';
import HomeScreen from './src/screens/HomeScreen';
import PairScreen from './src/screens/PairScreen';
import SettingsScreen from './src/screens/SettingsScreen';
import AboutScreen from './src/screens/AboutScreen';
import TermsOfUseScreen from './src/screens/TermsOfUseScreen';
import ExportScreen from './src/screens/ExportScreen';
import LangSelScn from './src/screens/LanguageSelectorScreen';
import {configureStore} from '@reduxjs/toolkit';
import sensorsSliceReducer from './src/utils/redux/sensorsSlice';
import settingsSliceReducer from './src/utils/redux/settingsSlice';
import {MaterialCommunityIcons} from '@expo/vector-icons';
import { backgroundColor } from 'react-native/Libraries/Components/View/ReactNativeStyleAttributes';

const Stack = createNativeStackNavigator();
const store = configureStore({
   reducer: {
       sensors: sensorsSliceReducer,
       settings: settingsSliceReducer,
   }
});

export default function App() {


    return (
        <Provider store={store}>
            <NativeBaseProvider>
                <NavigationContainer>
                    <Stack.Navigator initialRouteName="Home">
                        <Stack.Screen
                            name="Home"
                            component={HomeScreen}
                            options={({navigation}) => ({
                                headerRight: () => (
                                    <IconButton
                                        icon={<Icon as={MaterialCommunityIcons} name="cog" size="sm" />}
                                        onPress={() => navigation.navigate('Settings')}
                                        borderRadius="full"
                                        _pressed={{
                                            bg: "gray.200"
                                        }}
                                    />
                                )
                            })}
                        />
                        <Stack.Screen
                            name="PairScreen"
                            options={{
                                title: 'Find sensor',
                            }}
                            component={PairScreen}
                        />
                        <Stack.Screen name="SensorDetail"options={({route,navigation}) => ({
                            title: route.params.sensorData.name, 
                            headerStyle: {
                                backgroundColor: route.params.sensorData.color, 
                              },
                                 headerRight: () => (
                                    <Button
                                        style={{backgroundColor: "#f97316"}}
                                        onPress={() => navigation.navigate('ExportScreen',{sensorData:route.params.sensorData})}
                                        borderRadius="full"
                                        _pressed={{
                                            bg: "gray.200"
                                        }}
                                    >Export
                                    </Button>
                                ) 
                            
                            })} component={SensorDetailScreen}/>
                        <Stack.Screen name="Settings" component={SettingsScreen}/>
                        <Stack.Screen name="ExportScreen" component={ExportScreen}/>
                        <Stack.Screen name="Language" component={LangSelScn}/>
                        <Stack.Screen name="About" component={AboutScreen}/>
                        <Stack.Screen name="Terms Of Use" component={TermsOfUseScreen}/>
                    </Stack.Navigator>
                </NavigationContainer>
            </NativeBaseProvider>
        </Provider>
    );
}
