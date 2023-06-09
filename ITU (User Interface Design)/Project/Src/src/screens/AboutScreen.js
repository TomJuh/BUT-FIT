/**
 * Project:     VUT_ITU_Project
 * File name:   AboutScreen.js
 * Authors:     Juhász Tomáš (xjuhas04)
 * Description: 
 */

import React from 'react';
import {
    NativeBaseProvider,
    View,
    Image,
    Text, VStack, Center
} from 'native-base';

import {StyleSheet} from 'react-native'

export default () => {
  return (
    <NativeBaseProvider>
        <View style={styles.container}>
            <Image
                size = {150}
                alt="fallback text"
                borderRadius={100}
                style={styles.logo}
                source={require("../../assets/home.png")}
                />
        </View>
        <View style={styles.container}>
            <Center style={styles.Title}>
                <Text>Created by:</Text>
                <Text style={styles.boldText}>ITU team</Text>
                <Text style={styles.boldText}>Lukáš Nevrka (xnevrk03)</Text>
                <Text style={styles.boldText}>Roshka Andrei (xroshk00)</Text>
                <Text style={styles.boldText}>Juhász Tomáš (xjuhas04)</Text>
                <Text>Version:</Text>
                <Text style={styles.boldText}>1.0</Text>
            </Center>
        </View>
    </NativeBaseProvider>
  )
}


const styles = StyleSheet.create({
    container: {
      marginTop:180,
      marginBottom:20,
      justifyContent: 'center',
      alignItems: 'center',
    },
    logo: {
      width: 100,
      height: 100,
    },
    Title: {
        fontSize:   20,
        fontFamily: "Roboto",
        color:      "black"
        },
    boldText:{
        fontWeight: 'bold'
    }
  });