<h1>ESP8266 aREST DHT11/Led/Relay</h1>

## Overview

A simple example that implements a REST API for ESP8266 WiFi chip for handling DHT11 sensor and LED/Relay.  
If you want to know more about aREST, go over to [http://arest.io/](http://arest.io/).

## Setup

1. Rename `env.h.example` to `env.h`
2. Upload

## Endpoints
`http://wifi-ip`
```
{ 
   "variables":{ 
      "temperature":25.00,
      "humidity":33.00,
      "heatindex":24.42,
      "relay":0
   },
   "id":"1",
   "name":"Hall",
   "hardware":"esp8266",
   "connected":true
}
```

`http://wifi-ip/temperature`
```
{ 
   "temperature":25.00,
   "id":"1",
   "name":"Hall",
   "hardware":"esp8266",
   "connected":true
}
```

`http://wifi-ip/relay-switch`
```
{ 
   "return_value":0|1,
   "id":"1",
   "name":"Hall",
   "hardware":"esp8266",
   "connected":true
}
```

`http://wifi-ip/esp?params=reset`
```
Reset the module, returns http error due to restart hardware.
```