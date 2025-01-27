#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // initialise le LCD

int settings = 1;  // initialise la valeur par défaut du menu paramètres
int buzzer = A3;   // initialise le pin du buzzer
int motion = 2;    // initialise le pin du capteur de mouvement

String password = "12345678";

String tempPassword;

boolean triggered = false;  // Alarme est déclenchée

boolean alarm_is_activating = false;  // Sequence précédent l'armement de l'alarme

boolean alarm_is_active = false;  // Etat d'armement de l'alarme

boolean siren = false;  // Sirène = OFF

boolean homePage = false;  // pour savoir si afficher ou pas l'accueil

boolean enteredPassword;  // Etat du mot de passe entré pour éteindre l'alarme

boolean passChangeMode = false;

boolean passVerifMode = false;

boolean settiMenu = false;  // valeur d'activation du menu paramètres

char language;

const byte ROWS = 4;  //4 lignes
const byte COLS = 5;  //5 colonnes


char hexaKeys[ROWS][COLS] = {  // Définie les symboles sur les touches du pad numérique
  { 'E', '1', '2', '3', 'A' },
  { 'F', '4', '5', '6', 'B' },
  { 'G', '7', '8', '9', 'C' },
  { 'H', 'X', '0', 'K', 'D' }
};
byte rowPins[ROWS] = { 13, 12, 11, 10 };  //broche des lignes du pad
byte colPins[COLS] = { 9, 8, 7, 6, 5 };   //broches des colonnes du pad

//Initialise l'instance "NumKeypad"
Keypad NumKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

ThreeWire myWire(A0, A1, 3);       // DAT, CLK, RST (fils pour le RTC)
RtcDS1302<ThreeWire> Rtc(myWire);  // RTC Object





void setup() {

  boolean firstsetup = true;

  lcd.init();
  lcd.backlight();
  pinMode(motion, INPUT);
  Rtc.Begin();
  lcd.setCursor(0, 0);
  lcd.print("1. Francais");
  lcd.setCursor(0, 1);
  lcd.print("2. English");

  unsigned long prevMillis = millis();

  while (firstsetup == true) {

    unsigned long currentTime = millis();

    char key_pressed = NumKeypad.getKey();  // enregistre la touche qui a été pressée

    if (currentTime - prevMillis >= 20000) {
      key_pressed = '1';
    }

    if (key_pressed == '1') {
      language = 'FR';
      clic();
      lcd.setCursor(0, 0);
      lcd.print("===Alarme2003===");
      lcd.setCursor(0, 1);
      lcd.print("  Demarrage     ");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("  Demarrage.    ");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("  Demarrage..   ");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("  Demarrage...  ");
      delay(1000);
      clic();
      delay(100);
      clic();
      delay(100);
      clic();
      delay(100);

      lcd.setCursor(0, 1);
      lcd.print("Pensez a changer le code");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("ensez a changer le code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("nsez a changer le code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("sez a changer le code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("ez a changer le code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("z a changer le code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print(" a changer le code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("a changer le code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print(" changer le code");
      delay(500);
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("===Alarme2003===");
      lcd.setCursor(0, 1);
      lcd.print("   Bienvenue!   ");
      soundenter();
      lcd.clear();
      homePage = true;
      firstsetup = false;
    }

    if (key_pressed == '2') {
      language = 'EN';
      clic();
      lcd.setCursor(0, 0);
      lcd.print("===Alarme2003===");
      lcd.setCursor(0, 1);
      lcd.print(" Starting up    ");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print(" Starting up.   ");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print(" Starting up..  ");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print(" Starting up... ");
      delay(1000);
      clic();
      delay(100);
      clic();
      delay(100);
      clic();
      delay(100);

      lcd.setCursor(0, 1);
      lcd.print("DON'T forget to change the code");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("ON'T forget to change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("N'T forget to change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("'T forget to change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("T forget to change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print(" forget to change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("forget to change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("orget to change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("rget to change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("get to change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("et to change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("t to change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print(" to change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("to change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("o change the code");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print(" change the code");
      delay(500);
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("===Alarme2003===");
      lcd.setCursor(0, 1);
      lcd.print("    Welcome!    ");
      soundenter();
      lcd.clear();
      homePage = true;
      firstsetup = false;
    }
  }
}

void loop() {

  char key_pressed = NumKeypad.getKey();  // Variable qui récupère la touche qui a été pressée

  if (alarm_is_activating == false && alarm_is_active == false && homePage == true) {  // vérifie que l'alarme est désactivée, si c'est le cas = accueil
    homeConf();
  }

  if (key_pressed == 'B' && alarm_is_active == false) {  // Activation de l'alarme jour
    alarm_is_activating = true;
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("===Alarme2003===");
    lcd.setCursor(0, 1);
    lcd.print("    Aurevoir    ");
    soundquit();
    lockingSequ();
    alarm_is_activating = false;
    alarm_is_active = true;
    settiMenu = false;
    homePage = false;
  }

  if (alarm_is_active == true) {
    if (digitalRead(motion) == LOW) {
    }
    if (digitalRead(motion) == HIGH) {
      lcd.backlight();
      enterPassword();
    }
  }

  if (homePage == false && settiMenu == true && alarm_is_active == false && key_pressed == 'F') {
    settings++;
    updateSettings();
  }

  if (homePage == false && settiMenu == true && alarm_is_active == false && key_pressed == 'E') {
    settings--;
    updateSettings();
  }

  if (homePage == false && settiMenu == true && alarm_is_active == false && key_pressed == 'G') {
    settiMenu = false;
    executeAction();
  }

  if (homePage == false && settiMenu == true && alarm_is_active == false && key_pressed == 'H') {
    clic();
    homePage = true;
    settiMenu = false;
    homeConf();
    key_pressed = '-';
  }

  if (homePage == true && alarm_is_active == false && key_pressed == 'H') {
    homePage = false;
    settiMenu = true;
    lcd.backlight();
    settings = 1;
    updateSettings();
  }
}




void soundenter() {  // Mélodie qui se joue au démarrage et au désarmement de l'alarme
  tone(buzzer, 1047);
  delay(500);
  tone(buzzer, 1175);
  delay(500);
  tone(buzzer, 1568);
  delay(500);
  tone(buzzer, 1319);
  delay(700);
  noTone(buzzer);
}

void soundquit() {  // Mélodie qui se joue quand on arme l'alarme
  tone(buzzer, 1319);
  delay(500);
  tone(buzzer, 1568);
  delay(500);
  tone(buzzer, 1175);
  delay(500);
  tone(buzzer, 1047);
  delay(700);
  noTone(buzzer);
}

void homeConf() {  // Fonction qui met en page l'accueil
  RtcDateTime now = Rtc.GetDateTime();

  if (digitalRead(motion) == HIGH) {
    lcd.backlight();
  }

  else {
    lcd.noBacklight();
  }

  lcd.setCursor(0, 0);
  lcd.print("====Accueil=====");
  lcd.setCursor(0, 1);
  print2digits(now.Hour());
  lcd.print(":");
  print2digits(now.Minute());
  lcd.print(" ");
  print2digits(now.Day());
  lcd.print("/");
  print2digits(now.Month());
  lcd.print("/");
  lcd.print(now.Year());
}

void print2digits(int number) {  //permet de transformer toutes les dates en un chiffre, à des dates en deux chiffres
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);
}

void clic() {  // Fonction qui produit un bip dans les menus
  tone(buzzer, 1047);
  delay(150);
  noTone(buzzer);
}

void lockingSequ() {  // séquence avant l'enclanchement de l'alarme


  int mode = 1976;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("===Alarme2003===");
  lcd.setCursor(0, 1);
  lcd.print(" Verrouillage.. ");
  delay(1000);

  int i = 0;
  while (i < 10) {
    tone(buzzer, mode);
    delay(150);
    noTone(buzzer);
    delay(100);
    i++;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("===Alarme2003===");
  lcd.setCursor(0, 1);
  lcd.print(" Verrouillage.");
  delay(1500);

  int m = 0;
  while (m < 3) {
    tone(buzzer, mode);
    delay(500);
    noTone(buzzer);
    delay(500);
    m++;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("===Alarme2003===");
  lcd.setCursor(0, 1);
  lcd.print(" Verrouillage");
  delay(1500);
  tone(buzzer, mode);
  delay(1500);
  noTone(buzzer);

  lcd.clear();
  lcd.noBacklight();
  lcd.setCursor(0, 0);
  lcd.print("===Alarme2003===");
  lcd.setCursor(0, 1);
  lcd.print("   En marche.   ");
  delay(2000);
}

void updateSettings() {  // Fonction qui gère le menu
  switch (settings) {
    case 0:
      settings = 1;
      break;
    case 1:
      clic();
      lcd.clear();
      lcd.print(">Horloge");
      lcd.setCursor(0, 1);
      lcd.print(" Calendrier");
      break;
    case 2:
      clic();
      lcd.clear();
      lcd.print(" Horloge");
      lcd.setCursor(0, 1);
      lcd.print(">Calendrier");
      break;
    case 3:
      clic();
      lcd.clear();
      lcd.print(">Changer le code");
      lcd.setCursor(0, 1);
      lcd.print(" Retour");
      break;
    case 4:
      clic();
      lcd.clear();
      lcd.print(" Changer le code");
      lcd.setCursor(0, 1);
      lcd.print(">Retour");
      break;
    case 5:
      settings = 4;
      break;
  }
}

void executeAction() {  // fonction qui execute l'action sélectionnée
  switch (settings) {
    case 1:
      action1();
      break;
    case 2:
      action2();
      break;
    case 3:
      action3();
      break;
    case 4:
      action4();
      break;
  }
}

void action1() {  // Réglage de l'heure (paramètres)
  clic();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print("Heure(s) : ");

  int hour = getData();
  if (hour >= 24) {
    hour = 1;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print("Minute(s) : ");

  int minute = getData();
  if (minute >= 60) {
    minute = 1;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print("Seconde(s) : ");

  int second = getData();
  if (second >= 60) {
    second = 1;
  }

  RtcDateTime temporaire = Rtc.GetDateTime();
  int year = temporaire.Year();
  int month = temporaire.Month();
  int day = temporaire.Day();

  RtcDateTime nouveauTime = RtcDateTime(year, month, day, hour, minute, second);
  Rtc.SetDateTime(nouveauTime);

  saveSuccesfully();

  homePage = true;
}

void action2() {  // Réglage de la date (paramètres)
  clic();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print("Jour : ");
  int day = getData();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print("Mois : ");
  int month = getData();
  if (month > 12) {
    month = 1;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print("Annee : ");
  int year = getData();

  RtcDateTime temporaire = Rtc.GetDateTime();
  int hour = temporaire.Hour();
  int minute = temporaire.Minute();
  int second = temporaire.Second();

  RtcDateTime nouveauTime = RtcDateTime(year, month, day, hour, minute, second);
  Rtc.SetDateTime(nouveauTime);

  saveSuccesfully();

  homePage = true;
}

void action3() {  // Réglage du mot de passe (paramètres)
  clic();
  lcd.clear();
  int mdp = 0;
  lcd.setCursor(0, 0);
  lcd.print("Code actuel :");
  passChangeMode = true;
  passVerifMode = true;
  while (passVerifMode) {
    char key_pressed = NumKeypad.getKey();
    if (key_pressed != NO_KEY) {
      if (key_pressed == '0' || key_pressed == '1' || key_pressed == '2' || key_pressed == '3' || key_pressed == '4' || key_pressed == '5' || key_pressed == '6' || key_pressed == '7' || key_pressed == '8' || key_pressed == '9') {
        tempPassword += key_pressed;
        lcd.setCursor(mdp + 4, 1);
        lcd.print("*");
        mdp++;
        clic();
      }
      if (key_pressed == 'H') {
        tempPassword = "";
        mdp = 0;
        passChangeMode = false;
        passVerifMode = false;
        settiMenu = true;
        settings = 3;
        updateSettings();
      }
    }
    if (mdp > 8 || key_pressed == 'X') {
      tempPassword = "";
      mdp = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Code actuel :");
    }
    if (key_pressed == 'K') {
      mdp = 0;
      clic();
      if (password == tempPassword) {
        tempPassword = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Nouveau code :");
        while (passChangeMode) {
          char key_pressed = NumKeypad.getKey();
          if (key_pressed != NO_KEY) {
            if (key_pressed == '0' || key_pressed == '1' || key_pressed == '2' || key_pressed == '3' || key_pressed == '4' || key_pressed == '5' || key_pressed == '6' || key_pressed == '7' || key_pressed == '8' || key_pressed == '9') {
              tempPassword += key_pressed;
              lcd.setCursor(mdp + 4, 1);
              lcd.print("*");
              mdp++;
              clic();
            }
          }
          if (mdp > 8 || key_pressed == 'X') {
            tempPassword = "";
            mdp = 0;
            clic();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Nouveau code :");
          }
          if (key_pressed == 'K') {
            mdp = 0;
            clic();
            password = tempPassword;
            passChangeMode = false;
            passVerifMode = false;
            homePage = true;
            saveSuccesfully();
          }
        }
      } else {
        tempPassword = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Code actuel :");
        lcd.setCursor(0, 1);
        lcd.print("   Code faux.   ");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Code actuel :");
      }
    }
  }
}

void action4() {  // bouton retour
  clic();
  homePage = true;
}

int getData() {  // permet de récupérer les numéros tapés pour date / heure
  String container = "";
  while (true) {
    char c = NumKeypad.getKey();
    if (c == 'K') {
      clic();
      break;
    }
    if (c == 'G') {
      clic();
      break;
    } else if (isDigit(c)) {
      container += c;
      lcd.print(c);
      clic();
    } else {
    }
  }
  return container.toInt();
}

void saveSuccesfully() {  //séquence qui affiche un message si les informations ont bien été enregistrées.
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print(" Sauvegarde OK");
  tone(buzzer, 1047);
  delay(1000);
  noTone(buzzer);
  delay(1000);
}

void enterPassword() {
  int k = 0;
  tempPassword = "";
  triggered = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" *** ALERTE *** ");
  lcd.setCursor(0, 1);
  lcd.print("Code :");
  tone(buzzer, 1976);
  delay(1000);
  noTone(buzzer);
  unsigned long prevMillis = millis();

  while (triggered) {
    delay(1);
    unsigned long currentTime = millis();

    if (currentTime - prevMillis >= 12000) {
      siren = true;
    }

    char key_pressed = NumKeypad.getKey();
    if (key_pressed != NO_KEY) {
      if (key_pressed == '0' || key_pressed == '1' || key_pressed == '2' || key_pressed == '3' || key_pressed == '4' || key_pressed == '5' || key_pressed == '6' || key_pressed == '7' || key_pressed == '8' || key_pressed == '9') {
        tempPassword += key_pressed;
        lcd.setCursor(k + 7, 1);
        lcd.print("*");
        k++;
      }
    }

    if (siren == true) {
      tone(buzzer, 2093);
    }

    if (k > 8) {
      tempPassword = "";
      k = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" *** ALERTE *** ");
      lcd.setCursor(0, 1);
      lcd.print("   Code faux.   ");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" *** ALERTE *** ");
      lcd.setCursor(0, 1);
      lcd.print("Code :");
    }
    if (key_pressed == 'X') {
      tempPassword = "";
      k = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" *** ALERTE *** ");
      lcd.setCursor(0, 1);
      lcd.print("Code :");
    }
    if (key_pressed == 'K') {
      if (tempPassword == password) {
        triggered = false;
        alarm_is_active = false;
        homePage = true;
        siren = false;
        noTone(buzzer);
        lcd.setCursor(0, 0);
        lcd.print("===Alarme2003===");
        lcd.setCursor(0, 1);
        lcd.print("   Bienvenue!   ");
        soundenter();
        lcd.clear();
      } else if (tempPassword != password) {
        k = 0;
        tempPassword = "";
        lcd.setCursor(0, 1);
        lcd.print("   Code faux.   ");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" *** ALERTE *** ");
        lcd.setCursor(0, 1);
        lcd.print("Code :");
      }
    }
  }
}