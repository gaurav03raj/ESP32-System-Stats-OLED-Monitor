#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define CUSTOM_SDA    9   // SDA pin
#define CUSTOM_SCL    10  // SCL pin

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  Wire.begin(CUSTOM_SDA, CUSTOM_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Waiting for Serial...");
  display.display();
}

void loop() {
  if (Serial.available()) {
    // Read the entire message until newline terminator
    String msg = Serial.readString();  

    // Clear once, then print all lines
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    int lineNum = 0;
    int start = 0;
    for (int i = 0; i < msg.length(); i++) {
      if (msg[i] == '\n' || i == msg.length() - 1) {
        String line = msg.substring(start, i + 1);
        display.setCursor(0, lineNum * 10);  // 10 pixels per line
        display.print(line);
        lineNum++;
        start = i + 1;
        if (lineNum >= 6) break;  // prevent overflow
      }
    }

    display.display();
  }
}
