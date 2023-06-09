/**
 * Project:     VUT_ITU_Project
 * File name:   AboutScreen.js
 * Authors:     Juhász Tomáš (xjuhas04)
 * Description: 
 */

import React from 'react';
import {
    Slider,
    Box,
    NativeBaseProvider,
    View,
    Image,
    Text, VStack, Center
} from 'native-base';
import {Heading, ScrollView, HStack, Button, Icon} from 'native-base';

import ExportData from '../components/ExportData';


 
class ExportScreen extends React.Component {
    render() {
        
        const data = this.props.route.params.sensorData;
        const navigation = this.props.navigation;

        return (
            <VStack>
                <ExportData data={data.histData}/>
                <View style={{alignItems:'center'}}>
                                </View>
            </VStack>
        );
    }
}

export default ExportScreen;