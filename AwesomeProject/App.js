/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, { Component } from 'react';
import { BleManager } from 'react-native-ble-plx';
import { Button } from 'react-native';
import {
  Platform,
  StyleSheet,
  Text,
  View
} from 'react-native';



const instructions = Platform.select({
  ios: 'Press Cmd+R to reload,\n' +
    'Cmd+D or shake for dev menu',
  android: 'Double tap R on your keyboard to reload,\n' +
    'Shake or press menu button for dev menu',
});

type Props = {};
export default class App extends Component<Props> {
  constructor() {
      super();
      this.manager = new BleManager();
      var manager = this.manager
      var service = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E'
      var characteristicW = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E'
      this.device = null;
      var deviceID = null;

      this.scanAndConnect = function() {
        console.log("scanning and connecting")
          this.manager.startDeviceScan(null, null, (error, device) => {
              if (error) {
                  // Handle error (scanning will be stopped automatically)
                  return
              }

              console.log(device.name);

              // Check if it is a device you are looking for based on advertisement data
              // or other criteria.
              if (device.name === 'UART' ||
                  device.name === 'SensorTag') {

                  // Stop scanning as it's not necessary if you are scanning for one device.
                  this.manager.stopDeviceScan();
                  console.log(device.id);
                  this.deviceID = device.id;
                  device.connect()
                    .then((device) => {
                        return device.discoverAllServicesAndCharacteristics()
                      })
                    .then((device) => {
                       // Do work on device with services and characteristics
                       console.log(device.id)
                       return this.setupNotifications(device);
                    })
                    .then(() => {
                      console.log("Test");
                    })
                    .catch((error) => {
                        // Handle errors
                    });
              }
          });
      };
      console.log("This is the device ID " + this.deviceID)
      var deviceID = "E4:F1:49:C3:8D:29"
      console.log("This is the device ID " + this.deviceID)

      this.scanAndConnect();
      // console.log(this.manager)

      var sendData = function(base) {
        manager.writeCharacteristicWithoutResponseForDevice(
          deviceID, service, characteristicW, base /* 0x01 in hex */
        )//.then(() => {console.log("Button Press Sent");})
      };
      this.right = function() {
        sendData("cmlnaHQ=");
      }
  };

  async setupNotifications(device) {
    const service = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E'
    const characteristicW = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E'
    const characteristicR = '6E400003-B5A3-F393-E0A9-E50E24DCCA9E'
    console.log("sending data");
    const characteristic = await device.writeCharacteristicWithoutResponseForService(
      service, characteristicW, "Q29ubmVjdGluZw==" /* 0x01 in hex */
    )
  }

  render() {
    return (
      <View style={styles.container}>
        <Text style={styles.welcome}>
          Uh oh, not updating
        </Text>
        <Text style={styles.welcome}>
          HELLO WORLD!!!
        </Text>
		<Button
		  onPress={this.right}
		  title="Right"
		  color="#841584"
		  accessibilityLabel="Learn more about this purple button"
		/>
      </View>
    );
  }
}

// var scanAndConnect = function(manager) {
//   console.log("scanning and connecting")
//     manager.startDeviceScan(null, null, (error, device) => {
//         if (error) {
//             // Handle error (scanning will be stopped automatically)
//             return
//         }
//
//         console.log(device.name);
//
//         // Check if it is a device you are looking for based on advertisement data
//         // or other criteria.
//         if (device.name === 'UART' ||
//             device.name === 'SensorTag') {
//
//             // Stop scanning as it's not necessary if you are scanning for one device.
//             manager.stopDeviceScan();
//
//             device.connect()
//               .then((device) => {
//                   return device.discoverAllServicesAndCharacteristics()
//               })
//               .then((device) => {
//                  // Do work on device with services and characteristics
//                  console.log(device.services());
//                  for (service in device.services()) {
//                    console.log("Service Name " + service)
//                    console.log(seravice.characteristics())
//                    console.log("End Service Chars")
//                  }
//
//                   device.monitorCharacteristicForService(service, characteristicN, (error, characteristic) => {
//                     if (error) {
//                       this.error(error.message)
//                       return
//                     }
//                     this.updateValue(characteristic.uuid, characteristic.value)
//                   })
//
//               })
//               .catch((error) => {
//                   // Handle errors
//               });
//         }
//     });
// };

var onPressLearnMore = function() {
	console.log("nothing");
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F5FCFF',
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});
