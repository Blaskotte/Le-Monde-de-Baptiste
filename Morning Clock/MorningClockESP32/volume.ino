/*
//volume page
////////////////////////////////////////////////////////////////////////////

void updateVolumePage() {
  volumeSet_page = true;
  int volumeSetStep = 1;
  rotateCounter = volume;
  buttonPressedState = false;

  do {
    u8g2.firstPage();
    do {
      switch (volumeSetStep) {
        case 1:
          drawVolumeMenu();
          u8g2.drawFrame(16, 28, 96, 21);
          u8g2.drawBox(19, 31, volume * 3, 15);
          break;

        case 2:
          drawVolumeMenu();
          choice_is_OK();
          break;

        case 3:
          drawVolumeMenu();
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(16, 35, "Niveau du volume");
          u8g2.drawUTF8(28, 48, "enregistré !");
          break;
      }

    } while (u8g2.nextPage());

    switch (volumeSetStep) {

      case 1:  //Setting volume

        if (rotateCounter >= 15) {
          rotateCounter = 15;
        }
        if (rotateCounter < 0) {
          rotateCounter = 0;
        }
        volume = rotateCounter * 2;
        if (buttonPressedState == true) {
          rotateCounter = 1;
          buttonPressedState = false;
          volumeSetStep = 2;
        }
        break;

      case 2:  //Validate the choice
        if (rotateCounter < 1) {
          rotateCounter = 1;
        }
        if (rotateCounter > 2) {
          rotateCounter = 2;
        }
        if (buttonPressedState == true && rotateCounter == 1) {
          buttonPressedState = false;
          volumeSetStep = 3;
        }
        if (buttonPressedState == true && rotateCounter == 2) {
          buttonPressedState = false;
          volumeSetStep = 1;
          rotateCounter = volume;
        }
        break;

      case 3:
        volumeSet_page = false;
        delay(500);
        break;
    }
  } while (volumeSet_page == true);
}

////////////////////////////////////////////////////////////////////////////
*/