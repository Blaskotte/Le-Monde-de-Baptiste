//   ==draw menus bar==
////////////////////////////////////////////////////////////////////////////

void drawHomePageBar() {
  if (happyMenuSelect == true) {
    u8g2.drawXBMP(2, 1, 9, 11, happyMenu_select_BM);
  } else {
    u8g2.drawXBMP(3, 2, 7, 9, happyMenu_BM);
  }

  if (chime_is_activated == true) {
    if (chimeSelect == true) {
      u8g2.drawXBMP(23, 1, 10, 11, chimeON_select_BM);
    } else {
      u8g2.drawXBMP(24, 2, 8, 9, chimeON_BM);
    }

  } else {
    if (chimeSelect == true) {
      u8g2.drawXBMP(23, 1, 10, 11, chimeOFF_select_BM);
    } else {
      u8g2.drawXBMP(24, 2, 8, 9, chimeOFF_BM);
    }
  }

  if (alarm_is_activated == true) {
    if (alarmSelect == true) {
      u8g2.drawXBMP(12, 1, 10, 11, alarmON_select_BM);
    } else {
      u8g2.drawXBMP(13, 2, 8, 9, alarmON_BM);
    }
        // Display the time of the alarm
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.setCursor(96, 10);
    u8g2.print(twoDigit(alarmHour));
    u8g2.print(':');
    u8g2.print(twoDigit(alarmMinute));
  } else {
    if (alarmSelect == true) {
      u8g2.drawXBMP(12, 1, 10, 11, alarmOFF_select_BM);

    } else {
      u8g2.drawXBMP(13, 1, 8, 11, alarmOFF_BM);
    }
  }



  u8g2.drawLine(0, 13, 128, 13);
}

void drawSettingsBar() {
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 128, 14);
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(2, 2, 8, 9, settings_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(40, 10, "Réglages");
  u8g2.drawLine(0, 13, 128, 13);
}

void drawAlarmBar() {
  u8g2.drawXBMP(2, 2, 8, 9, alarmON_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(46, 10, "Réveil");
  u8g2.drawLine(0, 13, 128, 13);
}

void drawChimeBar() {
  u8g2.drawXBMP(2, 2, 8, 9, chimeON_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(40, 10, "Carillon");
  u8g2.drawLine(0, 13, 128, 13);
}

void drawVolumeBar() {
  u8g2.drawXBMP(2, 3, 8, 7, volume_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawStr(46, 10, "Volume");
  u8g2.drawLine(0, 13, 128, 13);
}

void drawDisplayBar() {
  u8g2.drawXBMP(2, 2, 8, 9, display_BM);
  u8g2.setFont(u8g2_font_profont11_tr);
  u8g2.drawStr(37, 10, "Affichage");
  u8g2.drawLine(0, 13, 128, 13);
}

void drawClockDateBar() {
  u8g2.drawXBMP(2, 2, 8, 9, calendar_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawStr(25, 10, "Heure et date");
  u8g2.drawLine(0, 13, 128, 13);
}

void drawClockBar() {
  u8g2.drawXBM(3, 2, 8, 9, clock_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(22, 10, "Régler l'heure");
  u8g2.drawLine(0, 13, 128, 13);
}

void drawDateBar() {
  u8g2.drawXBMP(2, 2, 8, 9, calendar_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(22, 10, "Régler la date");
  u8g2.drawLine(0, 13, 128, 13);
}

void drawAboutBar() {
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 128, 14);
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(2, 3, 8, 7, about_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(40, 10, "À propos");
  u8g2.drawLine(0, 13, 127, 13);
}

////////////////////////////////////////////////////////////////////////////