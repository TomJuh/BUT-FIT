/**
 * Project:     VUT_ITU_Project
 * File name:   HistData.js
 * Authors:     Lukáš Nevrka (xnevrk03)
 *              Juhász Tomáš (xjuhas04)
 * Description: 
 */

import React, {useState} from 'react';
import {HStack, Button, VStack, Text, Box,Slider} from 'native-base';

// Graphs
import {
    VictoryAxis, VictoryChart, VictoryZoomContainer, VictoryLine, VictoryArea,
    VictoryStack, VictoryLegend, VictoryLabel, VictoryTheme
} from "victory-native";
import { Dimensions, ToastAndroid,Platform } from "react-native";
import {parseToChartData, calcDateRangeSlider} from '../utils/charts';

import {useSelector} from 'react-redux';

export default function HistData({data}) {
    const {isFahrenheit, pollingInterval} = useSelector(state => state.settings);
    const screenWidth = Dimensions.get("window").width - 15;
    const screenHeight = Dimensions.get("window").height;
    const [onChangeValue, setOnChangeValue] = React.useState(70)
    const [onChangeEndValue, setOnChangeEndValue] = React.useState(70)
    const [onChangeString, setOnChangeString] = React.useState("day")

    const setString = (val) => {
        var range = "day";
        var count = 1;
        if (val < 10)
        {
            count = Math.floor(val/(10/6))+1;
            if (count > 1)
                range = "days";
            else
                range = "day"
            setOnChangeString(count + "  " + range)
        }
        else if (val >= 10 && val < 20)
        {
            count = Math.floor((val-10)/(10/3))+1;
            if (count == 1)
            {
                range = "week"
            }
            else 
            {
                range = "weeks"
            }
            setOnChangeString(count + "  " + range)
        }
        else 
        {
            count = Math.floor((val-20)/(80/12))+1;
            if (count >= 12)
            {
                range = "all"
                setOnChangeString(range)
            }
            else if (count > 1)
            {
                range = "months"
                setOnChangeString(count + "  " + range)
            }
            else
            {
                range = "month"
                setOnChangeString(count + "  " + range)
            }
        }
        setDateRange(range,count);

    }

    const [state, setState] = useState({
        ...calcDateRangeSlider(data, "day",1),
        selectedDomain: {}
    });

    const setDateRange = (range, count) => {

        setState({
            ...calcDateRangeSlider(data, range, count)
        });
    }

    const handleZoom = (domain) => {
        setState((prevState) => ({
            ...prevState,
            selectedDomain: domain
        }));
    }
    const toast = (msg) =>{
        if (Platform.OS === 'android')
        {
            ToastAndroid.show(msg,ToastAndroid.SHORT);
        }
    }


    return (
        <VStack alignItems="center">
            <HStack alignItems="center" px="4" mb="2">
                <Text marginTop="5"style={{fontSize:20, fontWeight:"bold"}} textAlign="center">{onChangeString} </Text>
                <Box borderRadius="2xl" bg="gray.300" borderColor="gray.300">

                        </Box>
            </HStack>

            <VictoryChart
                width={screenWidth}
                height={screenHeight / 2}
                scale={{ x: "time" }}
                domainPadding={{ x: 0, y: 30 }}
                padding={{ top: 5, bottom: 30, left: 0, right: 0 }}
                theme={VictoryTheme.material}
                >

                <VictoryAxis
                    tickFormat={state.tickFormat}
                    style={{
                        axis: { stroke: "#000", strokeWidth: "2" },
                        tickLabels: { fill: "#000", fontSize: "15" },
                    }} />

                <VictoryStack>
                    <VictoryArea
                        interpolation="natural"
                        style={{
                            data: {
                                strokeWidth: 0,
                                fill: "#fb923c",
                                opacity: 1
                            },
                        }}
                        samples={5}
                        data={parseToChartData(state.data, 'temp', screenWidth, isFahrenheit)}
                        labels={({ datum }) => 
                            `${datum.show ? (datum.y.toFixed(1) + (isFahrenheit ? "°F" : "°C")) : ""}`}
                        labelComponent={
                            <VictoryLabel
                                dy={-15}
                                style={{ fill: "#000", fontSize: "17", font: "bold 30px" }}
                            />}
                    />

                    <VictoryArea
                        interpolation="natural"
                        y={isFahrenheit ? ((d) => d.y * 4 / 3) : ((d) => d.y / 2)}
                        style={{
                            data: {
                                strokeWidth: 0,
                                fill: "#7dd3fc",
                            },
                        }}
                        samples={5}
                        data={parseToChartData(state.data, 'humi', screenWidth)}
                        labels={({ datum }) => `${datum.show ? datum.y.toFixed(0) + "%" : ""}`}
                        labelComponent={
                            <VictoryLabel
                                dy={-15}
                                style={{ fill: "#000", fontSize: "17", font: "bold 30px" }}
                            />}
                    />
                </VictoryStack>
            </VictoryChart>
            <Box style={{alignItems:'center'}} width="80%" >
        <Slider
          height="40%"
          colorScheme="#f97316"
          defaultValue={0}
          colorScheme="cyan"
          size = "lg"
          onChange={(v) => {
              setString(v)
            setOnChangeValue(v)
          }}
          onChangeEnd={(v) => {
            v && setOnChangeEndValue(Math.floor(v))
          }}
        >
          <Slider.Track>
            <Slider.FilledTrack />
          </Slider.Track>
          <Slider.Thumb />
        </Slider>
        <Button style={{backgroundColor:"#f97316"}}size="lg"
                            onPress={() => toast("Saved")}
                        height="10%"
                        width="50%"
                    >Save</Button>
 
</Box>
   
                    

</VStack>
    );
}

/*containerComponent={
<VictoryZoomContainer responsive={false}
    zoomDimension="x"
    zoomDomain={state.zoomDomain}
    onZoomDomainChange={handleZoom.bind(this)}
/>
}
*/