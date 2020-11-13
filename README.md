# esp8266-garage
ESP8266 device used for monitor my Garage. 
You can view the full tutorial here: 
- https://uncannyowly.com/2020/11/13/esp8266-garage-door-monitor/

This project had the following requirements for my environment: 
- Remote access to view data/trigger door. 
- Monitor garage door state: OPEN/CLOSE
- Read temperature and humidity
- Publish data to public expose endpoint in json format
- Secure access to site and data. 
- Include fancy interface for "dials" and a webcam view. 

This project has satisfied those requirements and provided me a platform for future additions. 

**Hardware:** 
- NodeMCU Lua ESP8266 CH340G ESP-12E
- 1 Channel 3V Relay 
- DHT22/AM2302 Digital Temperature And Humidity Sensor Module

**Prerequisites:**
- Arduino dev environment
- ESP8266 proper drivers installed. 
- Possibly some additional packages I forgot, sorry. Google is your friend?  

**Setup:**
- Arduino sketch configuration: Please review the arduino file to setup your 2.4GHZ compatible wifi, configure your web access user. 
- Please review the index.htm file in the data folder. This contains the path to your camera and lets you redesign the UI of the webportal. 

**Known Issues:**
- Slow UI. It downloads a lot on first load but it gets better. You can remedy this by removing some of the features or the webUI all together. 
- Ugly. I did what I could with what I had. 

**UI Preview: **
![HomePage](/images/screenshot-01.png)
![CameraOutput](/images/screenshot-02.png)
![FancyDials](/images/screenshot-03.png)

**Rest Info:**
![RestAPI](/images/screenshot-04.png)
