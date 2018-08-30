
/*
library for IoTtweet.com, using via api.iottweet.com
Compatible use with ESP32 WiFi and BLE chip from espressif.

Created by : IoTtweet tech. team
Date : 2016.Oct.15
*/

#ifndef IoTtweetESP32_h
#define IoTtweetESP32_h

#include "Arduino.h"
#include "WiFi.h"
#include "ArduinoJson.h"

class IoTtweetESP32
{

public:
  bool begin(const char *ssid, const char *passw);

  String WriteDashboard(const char *userid, const char *key, float slot0, float slot1, float slot2, float slot3, String tw, String twpb);
  String ReadControlPanel(const char *userid, const char *key);
  String ReadDigitalSwitch(const char *userid, const char *key, uint8_t sw);
  String getVersion();

  float ReadAnalogSlider(const char *userid, const char *key, uint8_t slider);

private:
  const char *_ssid, *_passw;
  const char *_userid, *_key;
  String _tw, _twpb;
  float _slot0, _slot1, _slot2, _slot3;
  const char *_libversion;
  String _str, _response, _controlpanelstatus;
  const char *_sw1status, *_sw2status, *_sw3status, *_sw4status, *_sw5status, *_sl1status, *_sl2status, *_sl3status;
  float f_sl1status, f_sl2status, f_sl3status;
  String _allcontrol;
  uint8_t _sw, _slider;

};

#endif
