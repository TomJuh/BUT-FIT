/**
 * Project:     VUT_ITU_Project
 * File name:   CurrData.js
 * Authors:     Lukáš Nevrka (xnevrk03)
 *              Juhász Tomáš (xjuhas04)
 * Description: 
 */

import React, {useState} from 'react';
import {
    FormControl,
    HStack, Input, Button, Center, Text, Heading,
    Modal, Icon,CheckBox

} from "native-base";
import {MaterialCommunityIcons} from '@expo/vector-icons';
import {useDispatch, useSelector} from 'react-redux';
import {addCustomName, assignSelGroup, addCustomColor, unpairSensor} from '../utils/redux/sensorsSlice'; // addCustomName({mac, name: 'Nove Jmeno'})
import { TouchableOpacity } from "react-native";
import { backgroundColor } from 'react-native/Libraries/Components/View/ReactNativeStyleAttributes';
import {updateSettings} from '../utils/redux/settingsSlice';
import hstack from 'native-base/src/theme/components/hstack';
export default function SensorInfo({data, navigation}) {
    const dispatch = useDispatch();

    const {groups} = useSelector(state => state.settings);
    const {customNames} = useSelector(state => state.sensors); // {'A0:....:01': 'Nove Jmeno'}
    const {customColors} = useSelector(state => state.sensors); 
    const [isSelected, setSelection] = useState(false);

    let groupsArray = groups ? Object.values(groups) : null;
    const name = customNames ? (customNames[data.mac] ?? data.name) : data.name;
    const color = customColors ? (customColors[data.mac] ?? data.color) : data.color;
    const [state, setState] = useState({
        editOpened: false,
        groupOpened: false,
        colorPick: false,
        newName: name,
        newGroup: groups,
        selGroup: null,
        newColor: color,
        checked: false
    });

    const handleChangeName = (mac, newName) => {
        dispatch(addCustomName({mac, name: newName}));
    };
    const handleAssignGroup= (mac, group) => {
        dispatch(assignSelGroup({mac, selGroup: group}));
    };
    const handleAddGroup = (mac, newGroup) => {
        var groupsArr = []
        if (groups != null)
            groupsArr = groupsArr.concat(groups)
        groupsArr.push(newGroup)
        dispatch(updateSettings({groups: groupsArr}));
    };
    const handleChangeColor = (mac, newColor) => {
        dispatch(addCustomColor({mac, color: newColor}));
    };
    
    const handleUnpair = (mac) => {
        dispatch(unpairSensor(mac));
      };

    const setEditOpened = (value) => {
        setState((prevState) => ({...prevState, editOpened: value}) );
    };
    const setColorPickerOpen = (value) => {
        setState((prevState) => ({...prevState, colorPick : value}) );
    };
    const setGroupOpen = (value) => {
        setState((prevState) => ({...prevState, groupOpened : value}) );
    };

    const setNewName = (value) => {
        setState((prevState) => ({...prevState, newName: value}) );
    };
    const addNewGroup = (value) => {
        setState((prevState) => ({...prevState, newGroup : value}) );
    };
    const setNewColor = (value) => {
        setState((prevState) => ({...prevState, newColor: value}) );
        handleChangeColor(data.mac,value);
        setColorPickerOpen(false);
        setEditOpened(false);
    };
    
    const assignGroup= (value) => {
        setState((prevState) => ({...prevState, selGroup: value}) );
        handleAssignGroup(data.mac,value);
        setGroupOpen(false);
        console.log("tests");
        setEditOpened(false);
    };


    return (
        <Center my="3">
            <HStack space="5" alignItems="center">
                <HStack>
                    {data.connected ?
                        <Icon as={MaterialCommunityIcons} name="bluetooth-connect" size="md" color="black" />
                        :
                        <Icon as={MaterialCommunityIcons} name="bluetooth-off" size="md" color="black" />
                    }
                </HStack>
                <Heading fontSize="3xl" numberOfLines={2} maxWidth="65%">{name}</Heading>
                <Button
                    variant="unstyled"
                    borderRadius="full"
                    ml="-3"
                    _pressed={{
                        bg: 'primary.100'
                    }}
                    onPress={() => { setEditOpened(true) }}
                >
                    <Text fontSize="md">✏️</Text>
                </Button>
            </HStack>
            <Modal
                isOpen={state.editOpened}
                onClose={() => setEditOpened(false)}
            >
                <Modal.Content maxWidth="400px">
                    <Modal.CloseButton />
                    <Modal.Header>Edit sensor</Modal.Header>
                    <Modal.Body>
                        <FormControl>
                            <FormControl.Label>Name</FormControl.Label>
                            <Input value={state.newName} onChangeText={setNewName}/>


                        </FormControl>
                    </Modal.Body>
                    <Modal.Body>
                        <FormControl>
                            <Button style={{backgroundColor:color? color:'#424242'}} onPress={() => {
                                setColorPickerOpen(true);

                            }}>
                                Change Color
                            </Button>

                        </FormControl>
                        <FormControl marginTop="2">
                            <Button style={{backgroundColor:color? color:'#424242'}} onPress={() => {
                                setGroupOpen(true);

                            }}>
                                Assign Group
                            </Button>

                        </FormControl>
                    </Modal.Body>
                    <Modal.Footer justifyContent="space-between">
                        <Button
                            colorScheme="secondary"
                            onPress={() => {
                                handleUnpair(data.mac);
                                navigation.goBack();
                            }}
                            mt="2"
                        >
                            Unpair
                        </Button>
                        <Button.Group space={2}>
                            <Button
                                variant="ghost"
                                colorScheme="blueGray"
                                onPress={() => {
                                    setEditOpened(false)
                                    setNewName(name);
                                }}
                            >
                                Cancel
                            </Button>
                            <Button
                                onPress={() => {
                                    setEditOpened(false);
                                    handleChangeName(data.mac, state.newName);
                                }}
                            >
                                Save
                            </Button>
                        </Button.Group>
                    </Modal.Footer>
                </Modal.Content>
            </Modal>
            <Modal
                isOpen={state.colorPick}
                onClose={() => setColorPickerOpen(false)}
            >
                <Modal.Content maxWidth="400px">
                    <Modal.CloseButton />
                    <Modal.Header>Pick a color</Modal.Header>
                    <Modal.Body>
                        <FormControl>
                        <Button.Group space={2}>
                            <TouchableOpacity style={{ backgroundColor: "#fef9c3", height: 50,  width:60 }}
                            onPress={() => setNewColor("#fef9c3")}
                            >
                            </TouchableOpacity>
                            <TouchableOpacity style={{ backgroundColor: "#fbbf24", height: 50, marginTop: 10, width:60 }}
                            onPress={() => setNewColor("#fbbf24")}>
                            </TouchableOpacity>
                            <TouchableOpacity style={{ backgroundColor: "#eab308", height: 50, marginTop: 10, width:60 }}
                            onPress={() => setNewColor("#eab308")}>
                            </TouchableOpacity>
                            <TouchableOpacity style={{ backgroundColor: "#f97316", height: 50, marginTop: 10, width:60 }}
                            onPress={() => setNewColor("#f97316")}>
                            </TouchableOpacity>
                        </Button.Group>
                        <Button.Group space={2}>
                            <TouchableOpacity style={{ backgroundColor: "#86efac", height: 50, marginTop: 10, width:60 }}
                            onPress={() => setNewColor("#86efac")}>
                            </TouchableOpacity>
                            <TouchableOpacity style={{ backgroundColor: "#16a34a", height: 50, marginTop: 10, width:60 }}
                            onPress={() => setNewColor("#16a34a")}>
                            </TouchableOpacity>
                            <TouchableOpacity style={{ backgroundColor: "#22d3ee", height: 50, marginTop: 10, width:60 }}
                            onPress={() => setNewColor("#22d3ee")}>
                            </TouchableOpacity>
                            <TouchableOpacity style={{ backgroundColor: "#0077e6", height: 50, marginTop: 10, width:60 }}
                            onPress={() => setNewColor("#0077e6")}>
                            </TouchableOpacity>
                       </Button.Group>
                        <Button.Group space={2}>
                            <TouchableOpacity style={{ backgroundColor: "#a855f7", opacity: color == "#a855f7" ? 0.2:1,height: 50, marginTop: 10, width:60 }}
                            onPress={() => 
                                    setNewColor("#a855f7") 
                                    } >
                            </TouchableOpacity>
                            <TouchableOpacity style={{ backgroundColor: "#7c3aed", height: 50, marginTop: 10, width:60 }}
                            onPress={() => setNewColor("#7c3aed")}>
                            </TouchableOpacity>
                            <TouchableOpacity style={{ backgroundColor: "#991b1b", height: 50, marginTop: 10, width:60 }}
                            onPress={() => setNewColor("#991b1b")}>
                            </TouchableOpacity>
                            <TouchableOpacity style={{ backgroundColor: "#dc2626", height: 50, marginTop: 10, width:60 }}
                            onPress={() => setNewColor("#dc2626")}>
                            </TouchableOpacity>
                        </Button.Group>
                         <Button.Group space={2}>
                            <TouchableOpacity style={{ backgroundColor: "#57534e", opacity: color == "#a855f7" ? 0.2:1,height: 50, marginTop: 10, width:60 }}
                            onPress={() => 
                                    setNewColor("#57534e") 
                                    } >
                            </TouchableOpacity>
                            <TouchableOpacity style={{ backgroundColor: "#18181b", height: 50, marginTop: 10, width:60 }}
                            onPress={() => setNewColor("#18181b")}>
                            </TouchableOpacity>
                            <TouchableOpacity style={{ backgroundColor: "#7c2d12", height: 50, marginTop: 10, width:60 }}
                            onPress={() => setNewColor(" #7c2d12")}>
                            </TouchableOpacity>
                            <TouchableOpacity style={{ backgroundColor: "#fafafa", height: 50, marginTop: 10, width:60 }}
                            onPress={() => setNewColor("#fafafa")}>
                            </TouchableOpacity>
                        </Button.Group>
                
                         </FormControl>
                    </Modal.Body>
               </Modal.Content>
            </Modal>
             <Modal
                isOpen={state.groupOpened}
                onClose={() => setGroupOpen(false)}
            >
                <Modal.Content maxWidth="400px">
                    <Modal.CloseButton />
                    <Modal.Header>Edit sensor</Modal.Header>
                    <Modal.Body>
                        <FormControl>
<>
                            {groups && !!groups.length ?
                                groups.map((group) => (
                                    <Button marginBottom="2.5" onPress={() => assignGroup(group)}>{group}</Button>
                                ))
                            :
                            <Text> No groups created</Text>
                            }
                            </>
                            <FormControl.Label>Add group</FormControl.Label>
                            <Input value={state.newGroup} onChangeText={addNewGroup}/>
                            

                        </FormControl>
                    </Modal.Body>
                    <Modal.Footer justifyContent="flex-end">
                        <Button.Group space={2}>
                            <Button
                                variant="ghost"
                                colorScheme="blueGray"
                                onPress={() => {
                                    setGroupOpened(false)
                                }}
                            >
                                Cancel
                            </Button>
                            <Button
                                onPress={() => {
                                    setGroupOpen(false);
                                    handleAddGroup(data.mac, state.newGroup);
                                    assignGroup(state.newGroup);
                                }}
                            >
                                Save
                            </Button>
                        </Button.Group>
                    </Modal.Footer>
                </Modal.Content>
            </Modal>

        </Center>
    );
};