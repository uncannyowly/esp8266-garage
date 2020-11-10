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
