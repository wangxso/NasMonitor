#include<U8g2lib.h>
#include<Arduino.h>
#include<WiFi.h>
#include <HTTPClient.h>
#include<Netdata.h>
#define RGB_BRIGHTNESS 25
#define IIC_CLOCK 7
#define IIC_DATA 6
#define RGB_LED_PIN 39


NetChartData netChartData;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, IIC_CLOCK, IIC_DATA);

// 定义初始化进度条长度
int progress = 0;
String local_ip = "";

#define SSID "WANG"
#define PASS "wh197408183514"

double cpu_usage = 0;


void setup_wifi() {
  delay(10);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  local_ip = WiFi.localIP().toString();
}


void http_get(String url) {
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode == 200) {
    Serial.println("Success");
    Serial.println(http.getString());
  } else {
    Serial.println("Error");
    Serial.println(httpCode);
  }
  http.end();
}

void setup() {
  neopixelWrite(RGB_LED_PIN, RGB_BRIGHTNESS, 0, 0);
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_wqy12_t_chinese2);
  setup_wifi();

  // 通过板载led显示连接状态
  // 红色未连接
  // 绿色已连接
  if (WiFi.status() == WL_CONNECTED) {
      neopixelWrite(RGB_LED_PIN, 0, RGB_BRIGHTNESS, 0);
  }

  

}

void getCPUInfo() {
  if (getNetdataWithdimension("system.cpu", netChartData, "system"))
    {
        Serial.print("CPU Usage: ");
        Serial.println(String(netChartData.max).c_str());

        cpu_usage = netChartData.max;
    }
}


void loop() {
  u8g2.clearBuffer();
  u8g2.setCursor(0, 20);
  u8g2.print("SSID: ");
  u8g2.print(SSID);
  u8g2.setCursor(0, 40);
  u8g2.print("IP: ");
  u8g2.print(local_ip);
  u8g2.setCursor(0, 60);
  getCPUInfo();
  u8g2.print("CPU: ");
  u8g2.print(cpu_usage*100);
  u8g2.print("%");
  u8g2.sendBuffer();

  delay(2000);
}

