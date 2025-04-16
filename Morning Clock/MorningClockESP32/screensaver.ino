void printScreensaver() {
  DateTime now = rtc.now();
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_timR24_tf);
  u8g2.setCursor(SVX, SVY);
  u8g2.print(twoDigit(now.hour()));

  if (blink == true) {
    u8g2.print(":");
  } else {
    u8g2.print(" ");
  }

  u8g2.setCursor((SVX+41), SVY);
  u8g2.print(twoDigit(now.minute()));

  u8g2.sendBuffer();
}
void updateScreensaver() {
  SVX = SVX + xSpeed;
  SVY = SVY + ySpeed;

  if (SVX + 72 >= screenWidth || SVX <= 0) {
    xSpeed = xSpeed * -1;
  }
  if (SVY >= screenHeight || SVY <= (0+23)) {
    ySpeed = ySpeed * -1;
  }
  delay(80);
}
void executeScreenSaver() {
  if (mainButton.pressed()){
    screensaver = false;
  }
}