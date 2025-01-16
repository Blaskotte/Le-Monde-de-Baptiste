#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // initialise le LCD

int settings = 1; // initialise la valeur par défaut du menu paramètres
int buzzer = A3; // initialise le pin du buzzer
int motion = 2;
int mode;

long duration;

String password="1234678";

String tempPassword;

boolean activationAlarmeJOUR = false;

boolean activationAlarmeNUIT = false;

boolean alarmeActiveJOUR = false;

boolean alarmeActiveNUIT = false;

boolean home = false; // pour savoir si afficher ou pas l'accueil

boolean enteredPassword; // Etat du mot de passe entré pour éteindre l'alarme

boolean passChangeMode = false;

boolean passChanged = false;

boolean param = false;

const byte ROWS = 4; //four rows
const byte COLS = 5; //four columns


char hexaKeys[ROWS][COLS] = { // Définie les symboles sur les touches du pad numérique
  {'E','1','2','3','A'},
  {'F','4','5','6','B'},
  {'G','7','8','9','C'},
  {'H','X','0','K','D'}
};
byte rowPins[ROWS] = {13, 12, 11, 10}; //broche des lignes du pad
byte colPins[COLS] = {9, 8, 7, 6, 5}; //broches des colonnes du pad

//initialize an instance of class NewKeypad
Keypad NumKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

ThreeWire myWire(A0, A1, 3);        // DAT, CLK, RST (fils pour le RTC)
RtcDS1302<ThreeWire> Rtc(myWire);    // RTC Object




void setup() {
  lcd.init();   
  lcd.backlight();
  // updateSettings(); // initialise la fonction qui met à jour le menu paramètres
  clic(); // initialise la fonction qui fait un bip à chaque clic
  // sequ();
  //pinMode(A2, INPUT); // broche pour la touche haute
  //pinMode(1, INPUT); // broche pour la touche basse
  //pinMode(4, INPUT); // broche pour la touche select
  //pinMode(5, INPUT); // broche pour la touche Paramètres
  pinMode(motion, INPUT);
  Rtc.Begin();
  // RtcDateTime currentTime = RtcDateTime(__DATE__ , __TIME__);
  // Rtc.SetDateTime(currentTime);

  lcd.setCursor(0, 0);          // Séquence de démarrage visuelle
  lcd.print("=====Alarme=====");
  lcd.setCursor(0, 1);
  lcd.print("  Demarrage     ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("  Demarrage.    ");
  lcd.setCursor(0, 1);
  delay(1000);
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
  lcd.print("=====Alarme=====");
  lcd.setCursor(0, 1);
  lcd.print("   Bienvenue!   ");
  soundenter();
  lcd.clear(); // Fin séquence de démarrage visuelle
  home = true; // Active le menu home
}

void loop() {

  char key_pressed = NumKeypad.getKey(); // Variable qui récupère la touche qui a été pressée

  if(activationAlarmeJOUR == false && activationAlarmeNUIT == false && alarmeActiveJOUR == false && alarmeActiveNUIT == false && home == true) // vérifie que l'alarme est désactivée, si c'est le cas = accueil
  {
    accueil();
  }

  if (key_pressed == 'B'){ // Activation de l'alarme jour
    lcd.backlight();
    activationAlarmeJOUR = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("=====Alarme=====");
    lcd.setCursor(0, 1);
    lcd.print("    Aurevoir    ");
    soundquit();
    sequ();
    activationAlarmeJOUR = false;
  }

  if (key_pressed == 'C'){ // Activation de l'alarme nuit
    lcd.backlight();
    activationAlarmeNUIT = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("=====Alarme=====");
    lcd.setCursor(0, 1);
    lcd.print("Zz Bonne nuit zZ");
    soundquit();
    sequ();
    activationAlarmeNUIT = false;
  }

  if (alarmeActiveJOUR == true){ // Alarme armée en mode jour

  }

  if (alarmeActiveNUIT == true){ // Alarme armée en mode nuit
  }

  if (home == false && param == true && key_pressed == 'F'){
    settings++;
    updateSettings();
    delay(70);
  }
  if (home == false && param == true && key_pressed == 'E'){
    settings--;
    updateSettings();
    delay(70);
  }
  if (home == false && param == true &&  key_pressed == 'G'){
    param = false;
    executeAction();
  }

  if (home == true && key_pressed == 'H'){
    home = false;
    param = true;
    settings = 1;
    updateSettings();
  }
}


void soundenter(){
  tone(buzzer, 1047);
  delay(500);
  tone(buzzer, 1175);
  delay(500);
  tone(buzzer, 1568);
  delay(500);
  tone(buzzer, 1319);
  delay(1300);
  noTone(buzzer);

}

void soundquit(){
  tone(buzzer, 1319);
  delay(500);
  tone(buzzer, 1568);
  delay(500);
  tone(buzzer, 1175);
  delay(500);
  tone(buzzer, 1047);
  delay(1300);
  noTone(buzzer);
}

void accueil(){ // Fonction qui met en page l'accueil
  if(digitalRead(motion) == HIGH){
    lcd.backlight();
  }
  else{
    lcd.noBacklight();
  }
  RtcDateTime now = Rtc.GetDateTime();
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

void print2digits(int number) { //permet de rendre toutes dates à un chiffre -> à deux
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);
}

void clic(){ // Fonction qui produit un bip dans les menus
  tone(buzzer, 1047);
  delay(150);
  noTone(buzzer);
}


void sequ(){ // séquence avant l'enclanchement de l'alarme

  if(activationAlarmeJOUR == true){
    mode = 1976;
  }
  else{
    mode = 330;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("=====Alarme=====");
  lcd.setCursor(0, 1);
  lcd.print(" Verrouillage.. ");
  delay(1000);
    int i=0;
  while (i < 10)
  {
    tone(buzzer, mode);
    delay(150);
    noTone(buzzer);
    delay(100);
    i++;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("=====Alarme=====");
  lcd.setCursor(0, 1);
  lcd.print(" Verrouillage.");
  delay(1500);
    int m=0;
  while (m < 3)
  {
    tone(buzzer, mode);
    delay(600);
    noTone(buzzer);
    delay(600);
    m++;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("=====Alarme=====");
  lcd.setCursor(0, 1);
  lcd.print(" Verrouillage");
  delay(1500);
  tone(buzzer, mode);
  delay(1500);
  noTone(buzzer);
  lcd.clear();
  lcd.noBacklight();
  lcd.setCursor(0, 0);
  lcd.print("=====Alarme=====");
  lcd.setCursor(0, 1);
  lcd.print("   En marche.   ");
  delay(2000);
}

void updateSettings() { // Fonction qui gère le menu
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


void executeAction() { // fonction qui execute l'action sélectionnée
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



void action1() { // Réglage de l'heure
  clic();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print("Heure(s) : ");
  int hour = getData();
  if(hour >= 24){
    hour = 1;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print("Minute(s) : ");
  int minute = getData();
  if(minute >= 60){
    minute = 1;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print("Seconde(s) : ");
  int second = getData();
    if(second >= 60){
    second = 1;
  }

  RtcDateTime temporaire = Rtc.GetDateTime();
  int year = temporaire.Year();
  int month = temporaire.Month();
  int day = temporaire.Day();

  RtcDateTime nouveauTime = RtcDateTime(year, month, day, hour, minute, second);
  Rtc.SetDateTime(nouveauTime);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print(" Sauvegarde OK");
  tone(buzzer, 1047);
  delay(1000);
  noTone(buzzer);
  delay(1000);

  home = true;
}

void action2() { // Réglage de la date
  clic();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print("Annee : ");
  int year = getData();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print("Mois : ");
  int month = getData();
  if(month > 12){
    month = 1;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print("Jour : ");
  int day = getData();

  RtcDateTime temporaire = Rtc.GetDateTime();
  int hour = temporaire.Hour();
  int minute = temporaire.Minute();
  int second = temporaire.Second();
  
  RtcDateTime nouveauTime = RtcDateTime(year, month, day, hour, minute, second);
  Rtc.SetDateTime(nouveauTime);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====Reglages====");
  lcd.setCursor(0, 1);
  lcd.print(" Sauvegarde OK");
  tone(buzzer, 1047);
  delay(1000);
  noTone(buzzer);
  delay(1000);
  home = true;
}

void action3() { // Réglage du mot de passe
  clic();
  lcd.clear();
  lcd.print("hey3");
  delay(1000);
  home = true;
}

void action4() { // bouton retour
  clic();
  home = true;
}

int getData() { // permet de récupérer les numéros tapés pour date / heure
  String container = "";
  while (true) {
    char c = NumKeypad.getKey();
    if (c == 'K') {
      clic();
      break;
    } 
    if (digitalRead(4) == HIGH) {
      clic();
      break;
    } 
    else if (isDigit(c)) {
      container += c;
      lcd.print(c);
    } 
    else {
    }
  }
  return container.toInt();
}
