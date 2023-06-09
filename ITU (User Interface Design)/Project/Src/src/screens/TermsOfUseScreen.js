import React from 'react';
import {VStack, Text,HStack,Divider} from 'native-base';
import {StyleSheet} from 'react-native'
import {Linking} from 'react-native'

export default function LanguageSelectorScreen() {
    return (
        <VStack style={styles.container}>
        <Text alignSelf='center'> This software was developed under GPL v3 </Text>
        <Text alignSelf='center'> and all of it's conditions apply</Text>

        <Text style={styles.text} alignSelf="center" fontWeight='bold'>GPL3 License synopsis</Text>
        <Divider bg="blue.400" thickness="2" mx="2"  />

        <Text style={styles.text}> 1. Anyone can copy, modify and distribute this software.</Text>

        <Divider bg="grey" thickness="1" mx="2"  />
        <Text style={styles.text}> 2. You have to include the license and copyright notice with each and every distribution.</Text>
        <Divider bg="grey" thickness="1" mx="2"  />
        <Text style={styles.text}> 3. You can use this software privately.</Text>
        <Divider bg="grey" thickness="1" mx="2"  />
        <Text style={styles.text}> 4. You can use this software for commercial purposes.</Text>
        <Divider bg="grey" thickness="1" mx="2"  />
        <Text style={styles.text}> 5. If you modify it, you have to indicate changes made to the code.</Text>
        <Divider bg="grey" thickness="1" mx="2"  />
        <Text style={styles.text}> 6. Any modifications of this code base MUST be distributed with the same license, GPLv3.</Text>
        <Divider bg="grey" thickness="1" mx="2"  />
        <Text style={styles.text}> 7. This software is provided without warranty.</Text>
        <Divider bg="grey" thickness="1" mx="2"  />
        <Text style={styles.text}> 8. The software author or license can not be held liable for any damages inflicted by the software.</Text>
        <Divider bg="grey" thickness="1" mx="2"  />
        <HStack justifyContent='center'>
        <Text  style={styles.text}>For the full license click </Text>  
        <Text  color='green' fontweight='bold' style={styles.text} onPress={() => Linking.openURL('https://www.gnu.org/licenses/gpl-3.0.en.html')}>here</Text>
        </HStack>

        <Divider bg="blue.400" thickness="2" mx="2"  />
        </VStack>
        );
}
const styles = StyleSheet.create({
    text: {
        marginBottom :10,
        marginTop: 10,
        fontSize:   15,
        fontFamily: "Roboto",
        },
  });
