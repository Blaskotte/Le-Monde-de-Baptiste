//   ==main settings menu==
////////////////////////////////////////////////////////////////////////////

void printSettingsMenu() {

  u8g2.clearBuffer();
  drawSettingsBar();
  drawScrollbar();
  drawSettingsScrollbar();
  u8g2.setFontMode(2);
  u8g2.setDrawColor(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_profont11_tf);
  switch (frameSettingsMenu) {
    case 1:
      u8g2.drawUTF8(6, 27, mainSettingsMenuTitles[1]);
      u8g2.drawUTF8(6, 43, mainSettingsMenuTitles[2]);
      u8g2.drawUTF8(6, 59, mainSettingsMenuTitles[3]);
      u8g2.drawUTF8(6, 75, mainSettingsMenuTitles[4]);
      u8g2.drawUTF8(6, 91, mainSettingsMenuTitles[5]);
      u8g2.setDrawColor(2);
      switch (menuItemSelect) {
        case 1:
          u8g2.drawBox(0, 16, 122, 15);
          u8g2.drawXBMP(112, 20, 4, 7, right_arrow_BM);
          break;

        case 2:
          u8g2.drawBox(0, 32, 122, 15);
          u8g2.drawXBMP(112, 36, 4, 7, right_arrow_BM);
          break;

        case 3:
          u8g2.drawBox(0, 48, 122, 15);
          u8g2.drawXBMP(112, 52, 4, 7, right_arrow_BM);
          break;

        case 4:
          u8g2.drawBox(0, 64, 122, 15);
          u8g2.drawXBMP(112, 68, 4, 7, right_arrow_BM);
          break;

        case 5:
          u8g2.drawBox(0, 80, 122, 15);
          u8g2.drawXBMP(112, 84, 4, 7, right_arrow_BM);
          break;
      }
      break;

    case 2:
      u8g2.drawUTF8(6, 27, mainSettingsMenuTitles[2]);
      u8g2.drawUTF8(6, 43, mainSettingsMenuTitles[3]);
      u8g2.drawUTF8(6, 59, mainSettingsMenuTitles[4]);
      u8g2.drawUTF8(6, 75, mainSettingsMenuTitles[5]);
      u8g2.drawUTF8(6, 91, mainSettingsMenuTitles[6]);
      u8g2.setDrawColor(2);
      switch (menuItemSelect) {
        case 2:
          u8g2.drawBox(0, 16, 122, 15);
          u8g2.drawXBMP(112, 20, 4, 7, right_arrow_BM);
          break;

        case 3:
          u8g2.drawBox(0, 32, 122, 15);
          u8g2.drawXBMP(112, 36, 4, 7, right_arrow_BM);
          break;

        case 4:
          u8g2.drawBox(0, 48, 122, 15);
          u8g2.drawXBMP(112, 52, 4, 7, right_arrow_BM);
          break;

        case 5:
          u8g2.drawBox(0, 64, 122, 15);
          u8g2.drawXBMP(112, 68, 4, 7, right_arrow_BM);
          break;

        case 6:
          u8g2.drawBox(0, 80, 122, 15);
          u8g2.drawXBMP(112, 84, 4, 7, right_arrow_BM);
          break;
      }
      break;

    case 3:
      u8g2.drawUTF8(6, 27, mainSettingsMenuTitles[3]);
      u8g2.drawUTF8(6, 43, mainSettingsMenuTitles[4]);
      u8g2.drawUTF8(6, 59, mainSettingsMenuTitles[5]);
      u8g2.drawUTF8(6, 75, mainSettingsMenuTitles[6]);
      u8g2.drawUTF8(6, 91, mainSettingsMenuTitles[7]);
      u8g2.setDrawColor(2);
      switch (menuItemSelect) {
        case 3:
          u8g2.drawBox(0, 16, 122, 15);
          u8g2.drawXBMP(112, 20, 4, 7, right_arrow_BM);
          break;

        case 4:
          u8g2.drawBox(0, 32, 122, 15);
          u8g2.drawXBMP(112, 36, 4, 7, right_arrow_BM);
          break;

        case 5:
          u8g2.drawBox(0, 48, 122, 15);
          u8g2.drawXBMP(112, 52, 4, 7, right_arrow_BM);
          break;

        case 6:
          u8g2.drawBox(0, 64, 122, 15);
          u8g2.drawXBMP(112, 68, 4, 7, right_arrow_BM);
          break;

        case 7:
          u8g2.drawBox(0, 80, 122, 15);
          u8g2.drawXBMP(112, 84, 4, 7, left_arrow_BM);
          break;
      }
      break;
  }
  u8g2.sendBuffer();
}

void updateSettingsMenu() {
  switch (rotaryEncoder.getCount() / 2) {
    case 1:
      menuItemSelect = 1;
      frameSettingsMenu = 1;
      if (frameSettingsMenu > 1 || frameSettingsMenu < 1) {
        frameSettingsMenu = 1;
      }
      rotatePrevious = rotateCounter;
      break;


    case 2:
      menuItemSelect = 2;
      if (rotatePrevious > rotateCounter && frameSettingsMenu == 3) {
        frameSettingsMenu--;
      }
      rotatePrevious = rotateCounter;
      break;

    case 3:
      menuItemSelect = 3;
      rotatePrevious = rotateCounter;
      break;

    case 4:
      menuItemSelect = 4;
      rotatePrevious = rotateCounter;
      break;

    case 5:
      menuItemSelect = 5;
      rotatePrevious = rotateCounter;
      break;

    case 6:
      menuItemSelect = 6;
      if (rotatePrevious < rotateCounter && frameSettingsMenu == 1) {
        frameSettingsMenu++;
      }
      rotatePrevious = rotateCounter;
      break;


    case 7:
      menuItemSelect = 7;
      frameSettingsMenu = 3;
      if (frameSettingsMenu > 3 || frameSettingsMenu < 3) {
        frameSettingsMenu = 3;
      }
      rotatePrevious = rotateCounter;
      break;

    default:
      if (rotateCounter > 7) {
        setRotary(7);
      }
      if (rotateCounter < 1) {
        setRotary(1);
      }
      break;
  }
}

void executeSettingsMenu() {
  if (mainButton.pressed()) {
    switch (rotateCounter) {
      case 1:
        alarmSettingsMenu = true;
        setRotary(1);
        menuItemSelect = 1;
        frameAlarmMenu = 1;
        settings_to_alarmTransition();
        break;

      case 2:
        break;

      case 3:
        break;

      case 4:
        displaySettingsMenu = true;
        setRotary(1);
        menuItemSelect = 1;
        settings_to_displayTransition();
        break;

      case 5:
        clockDateSettingsMenu = true;
        setRotary(1);
        menuItemSelect = 1;
        settings_to_clockDateTransition();
        break;

      case 6:
        aboutPage = true;
        setRotary(0);
        aboutCurrentStep = 0;
        settings_to_aboutTransition();
        break;

      case 7:
        mainSettingsMenu = false;
        homePage = true;
        setRotary(0);
        happyMenuSelect = false;
        soundSystem.play(3);
        settings_to_homePageTransition();
        break;
    }
  }
}

void homePage_to_settingsTransition() {
  int speed = 2;
  int pageTransition = 82;

  int background = 96;
  int menuItem1 = 27 + pageTransition;
  int menuItem2 = 43 + pageTransition;
  int menuItem3 = 59 + pageTransition;
  int menuItem4 = 75 + pageTransition;
  int menuItem5 = 91 + pageTransition;
  int menuItemBox = 16 + pageTransition;
  int arrow = 20 + pageTransition;

  do {
    DateTime now = rtc.now();

    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    drawSettingsBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_timR24_tn);
    //u8g2.setCursor(28, 58);
    //u8g2.print(twoDigit(now.hour()));
    //u8g2.print(':');
    //u8g2.print(twoDigit(now.minute()));
    //drawDate();
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, background, 128, 82);
    u8g2.setDrawColor(1);
    drawScrollbar();
    drawSettingsScrollbar();
    u8g2.drawUTF8(6, menuItem1, mainSettingsMenuTitles[1]);
    u8g2.drawStr(6, menuItem2, mainSettingsMenuTitles[2]);
    u8g2.drawStr(6, menuItem3, mainSettingsMenuTitles[3]);
    u8g2.drawUTF8(6, menuItem4, mainSettingsMenuTitles[4]);
    u8g2.drawUTF8(6, menuItem5, mainSettingsMenuTitles[5]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, menuItemBox, 122, 15);
    u8g2.drawXBMP(112, arrow, 4, 7, right_arrow_BM);
    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    background = background - speed;
    menuItem1 = menuItem1 - speed;
    menuItem2 = menuItem2 - speed;
    menuItem3 = menuItem3 - speed;
    menuItem4 = menuItem4 - speed;
    menuItem5 = menuItem5 - speed;
    menuItemBox = menuItemBox - speed;
    arrow = arrow - speed;
    delay(1);
  } while (pageTransition > 0);
}

void settings_to_homePageTransition() {
  int speed = 2;
  int pageTransition = 82;

  int background = 14;
  int menuItem1 = 27;
  int menuItem2 = 43;
  int menuItem3 = 59;
  int menuItem4 = 75;
  int menuItem5 = 91;
  int menuItemBox = 80;
  int arrow = 84;
  int scrollbar = 13;
  int elevator = 42;

  do {
    DateTime now = rtc.now();

    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    drawHomePageBar();
    u8g2.setFont(u8g2_font_timR24_tn);
    u8g2.setCursor(28, 58);
    u8g2.print(twoDigit(now.hour()));
    u8g2.print(':');
    u8g2.print(twoDigit(now.minute()));
    drawDate();
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, background, 128, 82);
    u8g2.setDrawColor(1);
    //draw scrollbar
    u8g2.drawFrame(122, scrollbar, 6, 83);
    u8g2.drawFrame(124, elevator, 2, 20);
    u8g2.drawUTF8(6, menuItem1, mainSettingsMenuTitles[3]);
    u8g2.drawUTF8(6, menuItem2, mainSettingsMenuTitles[4]);
    u8g2.drawUTF8(6, menuItem3, mainSettingsMenuTitles[5]);
    u8g2.drawUTF8(6, menuItem4, mainSettingsMenuTitles[6]);
    u8g2.drawUTF8(6, menuItem5, mainSettingsMenuTitles[7]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, menuItemBox, 122, 15);
    u8g2.drawXBMP(112, arrow, 4, 7, left_arrow_BM);
    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    background = background + speed;
    menuItem1 = menuItem1 + speed;
    menuItem2 = menuItem2 + speed;
    menuItem3 = menuItem3 + speed;
    menuItem4 = menuItem4 + speed;
    menuItem5 = menuItem5 + speed;
    menuItemBox = menuItemBox + speed;
    arrow = arrow + speed;
    scrollbar = scrollbar + speed;
    elevator = elevator + speed;
    delay(3);
  } while (pageTransition > 0);
}

////////////////////////////////////////////////////////////////////////////