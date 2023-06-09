/**
 * Project:     VUT_ITU_Project
 * File name:   SensorDetailScreen.js
 * Authors:     Lukáš Nevrka (xnevrk03)
 * Description: 
 */

import React from 'react';
import {VStack} from 'native-base';
import CurrData from '../components/CurrData';
import HistData from '../components/HistData';
import SensorInfo from '../components/SensorInfo';
  
import {Heading, ScrollView, HStack, IconButton, Icon} from 'native-base';

class SensorDetailScreen extends React.Component {
       render() {
        const data = this.props.route.params.sensorData;
        const navigation = this.props.navigation;
        return (
               <VStack>
                <SensorInfo data={data} navigation={navigation} />
                <CurrData currData={data.currData} connected={data.connected} />
                <HistData data={data.histData} />
            </VStack>
        );
    }
}

export default SensorDetailScreen;