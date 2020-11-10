# esp8266-garage
ESP8266 device used for monitor my Garage. 

This project had the following requirements for my environment: 
- Remote access to view data/trigger door. 
- Monitor garage door state: OPEN/CLOSE
- Read temperature and humidity
- Publish data to public expose endpoint in json format
- Secure access to site and data. 
- Include fancy interface for "dials" and a webcam view. 

This project has satisfied those requirements and provided me a platform for future additions. 

Hardware: 
- NodeMCU Lua ESP8266 CH340G ESP-12E
- 1 Channel 3V Relay 
- DHT22/AM2302 Digital Temperature And Humidity Sensor Module

<U>Prerequisites</U>
- Arduino dev environment
- ESP8266 proper drivers installed. 
- Possibly some additional packages I forgot, sorry. Google is your friend?  

<U>Setup:</U>
- Arduino sketch configuration: Please review the arduino file to setup your 2.4GHZ compatible wifi, configure your web access user. 
- Please review the index.htm file in the data folder. This contains the path to your camera and lets you redesign the UI of the webportal. 

<U>Known Issues:</U>
- Slow UI. It downloads a lot on first load but it gets better. You can remedy this by removing some of the features or the webUI all together. 
- Ugly. I did what I could with what I had. 

<U>UI Preview: </U>
<img "screenshot-01.png">
<img "screenshot-02.png">
<img "screenshot-03.png">

<U>Rest Info:</U>
<img "screenshot-04.png">

<U></U>