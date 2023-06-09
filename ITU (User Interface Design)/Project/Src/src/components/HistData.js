/**
 * Project:     VUT_ITU_Project
 * File name:   HistData.js
 * Authors:     Lukáš Nevrka (xnevrk03)
 * Description: 
 */

import React, {useState} from 'react';
import {HStack, Select, VStack, Text, Box} from 'native-base';

// Graphs
import {
    VictoryAxis, VictoryChart, VictoryZoomContainer, VictoryLine, VictoryArea,
    VictoryStack, VictoryLegend, VictoryLabel, VictoryTheme
} from "victory-native";
import { Dimensions } from "react-native";
import {useSelector} from 'react-redux';
import {calcDateRange, parseToChartData} from '../utils/charts';


export default function HistData({data}) {
    const {isFahrenheit, pollingInterval} = useSelector(state => state.settings);
    const screenWidth = Dimensions.get("window").width;
    const screenHeight = Dimensions.get("window").height;
    const [state, setState] = useState({
        ...calcDateRange(data, "day"),
        selectedDomain: {}
    });

    const setDateRange = (range) => {
        setState({
            ...calcDateRange(data, range)
        });
    }

    const handleZoom = (domain) => {
        setState((prevState) => ({
            ...prevState,
            selectedDomain: domain
        }));
    }

    return (
        <VStack alignItems="center">
            <HStack alignItems="center" px="4" mb="2">
                <Text fontSize="xl">Last&nbsp;</Text>
                <Box borderRadius="2xl" bg="gray.300" borderColor="gray.300">
                    <Select
                        selectedValue={state.dateRange}
                        onValueChange={(nextValue) => {
                            setDateRange(nextValue)
                        }}
                        minWidth={`${51 + (state.dateRange?.length ?? 3) * 10}`}
                        width="auto"
                        fontSize="xl"
                        borderWidth="0"
                    >
                        <Select.Item label="day" value="day"/>
                        <Select.Item label="week" value="week" />
                        <Select.Item label="month" value="month" />
                        <Select.Item label="year" value="year" />
                        <Select.Item label="all" value="all" />
                    </Select>
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