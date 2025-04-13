//about page
////////////////////////////////////////////////////////////////////////////

void printAboutPage() {

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_profont17_tf);
  u8g2.drawStr(3, text1, "Morning Clock");

  u8g2.setFont(u8g2_font_profont12_tf);
  u8g2.drawUTF8(19, text2, "Imaginé, conçu");

  u8g2.setFont(u8g2_font_profont15_tf);
  u8g2.drawStr(61, text3, "Baptiste");

  u8g2.setFont(u8g2_font_profont12_tf);
  u8g2.drawStr(45, text4, "WILD-CARUANA");
  u8g2.drawUTF8(9, text5, " et fabriqué par ");
  u8g2.drawUTF8(4, text6, "De février");
  u8g2.drawUTF8(4, text7, "à avril 2025");
  u8g2.drawStr(6, text8, "Exemplaire de test");
  u8g2.drawUTF8(25, text9, "attribué à :");

  u8g2.setFont(u8g2_font_profont15_tf);
  u8g2.drawStr(30, text10, "Version :");
  u8g2.drawStr(32, text11, "BETA 3.0");
  u8g2.drawUTF8(33, text12, "Baptiste");

  drawAboutBar();
  drawScrollbar();
  scrollbarAbout();
  u8g2.sendBuffer();
}

void updateAboutPage() {

  aboutNumberStep = aboutPageSize / aboutStepSize;

  if (rotateCounter < 0) {
    setRotary(0);
  }
  if (rotateCounter > aboutNumberStep) {
    setRotary(aboutNumberStep);
  }

  aboutCurrentStep = map(rotateCounter, 0, aboutNumberStep, 0, aboutPageSize);

  text1 = 34 - aboutCurrentStep;
  text2 = 85 - aboutCurrentStep;
  text3 = 111 - aboutCurrentStep;
  text4 = 123 - aboutCurrentStep;
  text5 = 96 - aboutCurrentStep;
  text6 = 137 - aboutCurrentStep;
  text7 = 147 - aboutCurrentStep;
  text8 = 171 - aboutCurrentStep;
  text9 = 182 - aboutCurrentStep;
  text10 = 51 - aboutCurrentStep;
  text11 = 66 - aboutCurrentStep;
  text12 = 198 - aboutCurrentStep;
}

void executeAboutPage() {
  if (mainButton.pressed()) {
    aboutPage = false;
    setRotary(6);
    menuItemSelect = 6;
    about_to_settingsTransition();
  }
}

void settings_to_aboutTransition() {

  int speed = 2;

  int pageTransition = 128;

  int settingItems = 6;
  int settingsBox = 0;
  int settingsArrow = 112;

  int textTransition1 = 3 + pageTransition;
  int textTransition2 = 30 + pageTransition;
  int textTransition3 = 32 + pageTransition;

  do {
    u8g2.clearBuffer();
    u8g2.setFontMode(2);
    u8g2.setDrawColor(1);
    u8g2.setBitmapMode(1);
    switch (frameSettingsMenu) {
      case 2:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[2]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[6]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 80, 122, 15);
        u8g2.drawXBMP(settingsArrow, 84, 4, 7, right_arrow_BM);
        break;

      case 3:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[4]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[6]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[7]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 64, 122, 15);
        u8g2.drawXBMP(settingsArrow, 68, 4, 7, right_arrow_BM);
        break;
    }
    u8g2.setFontMode(2);
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont17_tf);
    u8g2.drawStr(textTransition1, 34, "Morning Clock");
    u8g2.setFont(u8g2_font_profont15_tf);
    u8g2.drawStr(textTransition2, 51, "Version :");
    u8g2.drawStr(textTransition3, 66, "BETA 3.0");
    drawAboutBar();
    drawScrollbar();
    scrollbarAbout();
    u8g2.sendBuffer();

    settingItems = settingItems - speed;
    settingsBox = settingsBox - speed;
    settingsArrow = settingsArrow - speed;

    textTransition1 = textTransition1 - speed;
    textTransition2 = textTransition2 - speed;
    textTransition3 = textTransition3 - speed;
    pageTransition = pageTransition - speed;
    delayMicroseconds(100);
  } while (pageTransition > 0);
}

void about_to_settingsTransition() {
  int speed = 2;

  int pageTransition = -128;

  int settingItems = 6 + pageTransition;
  int settingsBox = 0 + pageTransition;
  int settingsArrow = 112 + pageTransition;

  int textScroll1 = 3;
  int textScroll2 = 19;
  int textScroll3 = 61;
  int textScroll4 = 45;
  int textScroll5 = 9;
  int textScroll6 = 4;
  int textScroll7 = 6;
  int textScroll8 = 25;
  int textScroll9 = 30;
  int textScroll10 = 32;
  int textScroll11 = 33;

  do {
    u8g2.clearBuffer();
    u8g2.setFontMode(2);
    u8g2.setDrawColor(1);
    u8g2.setBitmapMode(1);
    switch (frameSettingsMenu) {
      case 2:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[2]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[6]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 80, 122, 15);
        u8g2.drawXBMP(settingsArrow, 84, 4, 7, right_arrow_BM);
        break;

      case 3:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[4]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[6]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[7]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 64, 122, 15);
        u8g2.drawXBMP(settingsArrow, 68, 4, 7, right_arrow_BM);
        break;
    }
    u8g2.setFontMode(2);
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont17_tf);
    u8g2.drawStr(textScroll1, text1, "Morning Clock");

    u8g2.setFont(u8g2_font_profont12_tf);
    u8g2.drawUTF8(textScroll2, text2, "Imaginé, conçu");

    u8g2.setFont(u8g2_font_profont15_tf);
    u8g2.drawStr(textScroll3, text3, "Baptiste");

    u8g2.setFont(u8g2_font_profont12_tf);
    u8g2.drawStr(textScroll4, text4, "WILD-CARUANA");
    u8g2.drawUTF8(textScroll5, text5, " et fabriqué par ");
    u8g2.drawUTF8(textScroll6, text6, "Février");
    u8g2.drawUTF8(textScroll6, text7, "à Mars 2025");
    u8g2.drawStr(textScroll7, text8, "Exemplaire de test");
    u8g2.drawUTF8(textScroll8, text9, "attribué à :");

    u8g2.setFont(u8g2_font_profont15_tf);
    u8g2.drawStr(textScroll9, text10, "Version :");
    u8g2.drawStr(textScroll10, text11, "BETA 2.0");
    u8g2.drawUTF8(textScroll11, text12, "Baptiste");

    drawSettingsBar();
    drawScrollbar();
    drawSettingsScrollbar();
    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;

    settingItems = settingItems + speed;
    settingsBox = settingsBox + speed;
    settingsArrow = settingsArrow + speed;

    textScroll1 = textScroll1 + speed;
    textScroll2 = textScroll2 + speed;
    textScroll3 = textScroll3 + speed;
    textScroll4 = textScroll4 + speed;
    textScroll5 = textScroll5 + speed;
    textScroll6 = textScroll6 + speed;
    textScroll7 = textScroll7 + speed;
    textScroll8 = textScroll8 + speed;
    textScroll9 = textScroll9 + speed;
    textScroll10 = textScroll10 + speed;
    textScroll11 = textScroll11 + speed;
    delayMicroseconds(100);
  } while (pageTransition < 0);
}

////////////////////////////////////////////////////////////////////////////