import React, { Component } from 'react';
import { BleManager } from 'react-native-ble-plx';
import { Button } from 'react-native';
import {
  Platform,
  StyleSheet,
  Text,
  View,
  TouchableOpacity
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
      var service = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E' // Strings that identify the bluetooth reciever
      var characteristicW = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E' // You can find them with a UART bluetooth app
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
                  device.name === 'SensorTag') { //Device name, can be changed

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
                       return this.setupNotifications(device); //useless
                    })
                    .then(() => {
                      console.log("Test"); //useless
                    })
                    .catch((error) => {
                        // Handle errors
                    });
              }
          });
      };
      console.log("This is the device ID " + this.deviceID)
      var deviceID = "E4:F1:49:C3:8D:29" //placeholder, can try to remove
      console.log("This is the device ID " + deviceID)

      this.scanAndConnect();
      // console.log(this.manager)

      var sendData = function(base) {
        manager.writeCharacteristicWithoutResponseForDevice(
          deviceID, service, characteristicW, base /* 0x01 in hex */ //sends base to device on characteristicW on service
        ).then(() => {console.log("Button Press Sent");})
      };
      this.right = function() {
        console.log("right")
        sendData(Base64.encode("right"));
      }
      this.createSender = function(code) {
        return function() {
          console.log(code)
          sendData(Base64.encode(code)); //sends base64 encoded message
        }
      }
      this.right(); //useless
  };

  async setupNotifications(device) { //useless
    const service = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E'
    const characteristicW = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E'
    const characteristicR = '6E400003-B5A3-F393-E0A9-E50E24DCCA9E'
    console.log("sending data");
    // const characteristic = await device.writeCharacteristicWithoutResponseForService(
    //   service, characteristicW, "Q29ubmVjdGluZw==" /* 0x01 in hex */
    // )
  }

  render() {
    return (
      <View style={styles.container}>
        <Text style={styles.welcome}>
          Welcome to JARL!
        </Text>
        <Text style={styles.instructions}>
          To change direction, press the Forward, Backward, Left, or Right arrow
        </Text>
        <Text style={styles.instructions}>
          To move up or down, press the Up or Down arrow
        </Text>
        <Text style={styles.instructions}>
          To stop JARL, press the stop button
        </Text>
        <TouchableOpacity
          style={styles.buttonForward}
          onPressIn={this.createSender("f1")}
          onPressOut={this.createSender("f0")}
        >
        <Text style={styles.buttonText}>F</Text>
        </TouchableOpacity>
        <TouchableOpacity
          style={styles.buttonBack}
          onPressIn={this.createSender('b1')}
          onPressOut={this.createSender("b0")}
        >
        <Text style={styles.buttonText}>B</Text>
        </TouchableOpacity>
        <TouchableOpacity
          style={styles.buttonLeft}
          onPressIn={this.createSender("l1")}
          onPressOut={this.createSender("l0")}
        >
        <Text style={styles.buttonText}>L</Text>
        </TouchableOpacity>
        <TouchableOpacity
          style={styles.buttonRight}
          onPressIn={this.createSender("r1")}
          onPressOut={this.createSender("r0")}
        >
        <Text style={styles.buttonText}>R</Text>
        </TouchableOpacity>
        <TouchableOpacity
          style={styles.buttonUp}
          onPressIn={this.createSender("u1")}
          onPressOut={this.createSender("u0")}
        >
        <Text style={styles.buttonText}>U</Text>
        </TouchableOpacity>
        <TouchableOpacity
          style={styles.buttonDown}
          onPressIn={this.createSender("d1")}
          onPressOut={this.createSender("d0")}
        >
        <Text style={styles.buttonDText}>D</Text>
        </TouchableOpacity>
        <TouchableOpacity
          style={styles.buttonStop}
          onPressIn={this.createSender("s1")}
          onPressOut={this.createSender("s0")}
        >
        <Text style={styles.buttonText}>STOP</Text>
        </TouchableOpacity>
      </View>
    );
  }
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
    top: 100
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    fontSize: 15,
    marginBottom: 10,
    top: 100
  },
  buttonForward: {
    width: 0,
    height: 0,
    borderLeftWidth: 30,
    borderRightWidth: 30,
    borderBottomWidth: 60,
    alignItems: 'center',
    backgroundColor: 'transparent',
    borderLeftColor: 'transparent',
    borderRightColor: 'transparent',
    borderBottomColor: 'black',
    padding: 0,
    right: 60,
    top: 200
  },
  buttonBack: {
    width: 0,
    height: 0,
    borderLeftWidth: 30,
    borderRightWidth: 30,
    borderTopWidth: 60,
    alignItems: 'center',
    backgroundColor: 'transparent',
    borderLeftColor: 'transparent',
    borderRightColor: 'transparent',
    borderTopColor: 'black',
    padding: 0,
    right: 60,
    top: 300
  },
  buttonLeft: {
    width: 0,
    height: 0,
    borderBottomWidth: 30,
    borderTopWidth: 30,
    borderRightWidth: 60,
    alignItems: 'center',
    backgroundColor: 'transparent',
    borderBottomColor: 'transparent',
    borderTopColor: 'transparent',
    borderRightColor: 'black',
    padding: 0,
    right: 140,
    top: 160
  },
  buttonRight: {
    width: 0,
    height: 0,
    borderBottomWidth: 30,
    borderTopWidth: 30,
    borderLeftWidth: 60,
    alignItems: 'center',
    backgroundColor: 'transparent',
    borderBottomColor: 'transparent',
    borderTopColor: 'transparent',
    borderLeftColor: 'black',
    padding: 0,
    left: 20,
    top: 100
  },
  buttonUp: {
    width: 0,
    height: 0,
    borderLeftWidth: 30,
    borderRightWidth: 30,
    borderBottomWidth: 60,
    alignItems: 'center',
    backgroundColor: 'transparent',
    borderLeftColor: 'transparent',
    borderRightColor: 'transparent',
    borderBottomColor: 'black',
    padding: 0,
    left: 130,
    bottom: 5
  },
  buttonDown: {
    width: 0,
    height: 0,
    borderLeftWidth: 30,
    borderRightWidth: 30,
    borderTopWidth: 60,
    alignItems: 'center',
    backgroundColor: 'transparent',
    borderLeftColor: 'transparent',
    borderRightColor: 'transparent',
    borderTopColor: 'black',
    padding: 0,
    left: 130,
    top: 25
  },
  buttonStop: {
    width: 60,
    height: 60,
    alignItems: 'center',
    backgroundColor: 'pink',
    padding: 0,
    right: 60,
    bottom: 80
  },
  buttonText: {
    color: 'black',
    fontWeight: 'bold',
    fontSize: 20,
    top: 15
  },
  buttonDText: {
    color: 'pink',
    fontWeight: 'bold',
    fontSize: 20,
    bottom: 40,
    right: 0
  },
});

const Base64 = {
    characters: "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=" ,

    encode: function( string )
    {
        var characters = Base64.characters;
        var result     = '';

        var i = 0;
        do {
            var a = string.charCodeAt(i++);
            var b = string.charCodeAt(i++);
            var c = string.charCodeAt(i++);

            a = a ? a : 0;
            b = b ? b : 0;
            c = c ? c : 0;

            var b1 = ( a >> 2 ) & 0x3F;
            var b2 = ( ( a & 0x3 ) << 4 ) | ( ( b >> 4 ) & 0xF );
            var b3 = ( ( b & 0xF ) << 2 ) | ( ( c >> 6 ) & 0x3 );
            var b4 = c & 0x3F;

            if( ! b ) {
                b3 = b4 = 64;
            } else if( ! c ) {
                b4 = 64;
            }

            result += Base64.characters.charAt( b1 ) + Base64.characters.charAt( b2 ) + Base64.characters.charAt( b3 ) + Base64.characters.charAt( b4 );

        } while ( i < string.length );

        return result;
    } ,

    decode: function( string )
    {
        var characters = Base64.characters;
        var result     = '';

        var i = 0;
        do {
            var b1 = Base64.characters.indexOf( string.charAt(i++) );
            var b2 = Base64.characters.indexOf( string.charAt(i++) );
            var b3 = Base64.characters.indexOf( string.charAt(i++) );
            var b4 = Base64.characters.indexOf( string.charAt(i++) );

            var a = ( ( b1 & 0x3F ) << 2 ) | ( ( b2 >> 4 ) & 0x3 );
            var b = ( ( b2 & 0xF  ) << 4 ) | ( ( b3 >> 2 ) & 0xF );
            var c = ( ( b3 & 0x3  ) << 6 ) | ( b4 & 0x3F );

            result += String.fromCharCode(a) + (b?String.fromCharCode(b):'') + (c?String.fromCharCode(c):'');

        } while( i < string.length );

        return result;
    }
};
