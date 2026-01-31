#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define CUSTOM_SDA    9
#define CUSTOM_SCL    10

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  Wire.begin(CUSTOM_SDA, CUSTOM_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setRotation(3);  // Try 3 if you prefer the opposite direction
  display.setTextWrap(false);  // Disable text wrapping to prevent spillover
  display.setCursor(1, 50);
  display.println("Waiting...");
  display.display();
}

void loop() {
  if (Serial.available() > 0) {
    String msg = Serial.readString();
    msg.trim();

    int pos1 = msg.indexOf('\n');
    int pos2 = msg.indexOf('\n', pos1 + 1);
    int pos3 = msg.indexOf('\n', pos2 + 1);

    if (pos1 == -1 || pos2 == -1 || pos3 == -1) return;

    String l1 = msg.substring(0, pos1);
    String l2 = msg.substring(pos1 + 1, pos2);
    String l3 = msg.substring(pos2 + 1, pos3);
    String l4 = msg.substring(pos3 + 1);

    // Parse percentages
    uint8_t cpu = 0, ram = 0, gpu = 0;
    int cpu_start = l1.indexOf("CPU:") + 4;
    if (cpu_start > 3) {
      int cpu_end = l1.indexOf('%', cpu_start);
      if (cpu_end != -1) cpu = l1.substring(cpu_start, cpu_end).toInt();
    }
    int ram_start = l1.indexOf("RAM:") + 4;
    if (ram_start > 3) {
      int ram_end = l1.indexOf('%', ram_start);
      if (ram_end != -1) ram = l1.substring(ram_start, ram_end).toInt();
    }
    int gpu_start = l4.indexOf(':') + 1;
    if (gpu_start > 0) {
      int gpu_end = l4.indexOf('%', gpu_start);
      if (gpu_end != -1) gpu = l4.substring(gpu_start, gpu_end).toInt();
    }

    // Parse network strings (skip colon and space)
    String net_up = l2.substring(l2.indexOf(':') + 2);
    String net_dn = l3.substring(l3.indexOf(':') + 2);

    // Draw everything
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    const int bar_w = 18;
    const int gap = 5;
    const int bar_h = 70;
    int x1 = 0;
    int x2 = x1 + bar_w + gap;
    int x3 = x2 + bar_w + gap;
    int y_label = 6;
    int y_bar = 20;
    int y_percent = y_bar + bar_h + 8;
    int y_net = y_percent + 10;

    // Labels on single line above bars
    display.setCursor(x1 + (bar_w - 18) / 2, y_label);
    display.print("CPU");
    display.setCursor(x2 + (bar_w - 18) / 2, y_label);
    display.print("RAM");
    display.setCursor(x3 + (bar_w - 18) / 2, y_label);
    display.print("GPU");

    // Vertical bars (fill from bottom)
    auto drawBar = [&](int x, uint8_t percent) {
      display.drawRoundRect(x, y_bar, bar_w, bar_h, 4, SSD1306_WHITE);
      uint16_t fill_h = map(percent, 0, 100, 0, bar_h - 4);
      if (fill_h > 0) {
        display.fillRoundRect(x + 2, y_bar + bar_h - fill_h - 2, bar_w - 4, fill_h, 2, SSD1306_WHITE);
      }
    };
    drawBar(x1, cpu);
    drawBar(x2, ram);
    drawBar(x3, gpu);

    // Percentages below bars (with % sign, centered where possible)
    auto drawPercent = [&](int x, uint8_t percent) {
      char buf[5];
      sprintf(buf, "%d%%", percent);
      int text_w = strlen(buf) * 6;
      int offset = (bar_w - text_w) / 2;
      if (offset < 0) offset = 0;
      display.setCursor(x + offset, y_percent);
      display.print(buf);
    };
    drawPercent(x1, cpu);
    drawPercent(x2, ram);
    drawPercent(x3, gpu);

    // Network speeds: labels and values positioned separately for precise half-char gap
    const int label_x = 2;
    const int value_x = label_x + 15;  // 12px for "U:"/ "D:", +3px for ~half character space

    display.setCursor(label_x, y_net);
    display.print("U:");
    display.setCursor(value_x, y_net);
    display.print(net_up);

    display.setCursor(label_x, y_net + 10);
    display.print("D:");
    display.setCursor(value_x, y_net + 10);
    display.print(net_dn);

    display.display();
  }
}
