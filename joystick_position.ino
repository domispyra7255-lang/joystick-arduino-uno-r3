// Arduino UNO R3 - Joystick jako myszka (komunikacja przez port szeregowy)
// LEWO/PRAWO OK, GÓRA/DÓŁ ZAMIENIONE
// Pinowanie:
// VRx -> A0 (analogowy)
// VRy -> A1 (analogowy)
// SW  -> Pin 2 (cyfrowy - wciśnięcie = SPACJA)
// +5V -> 5V
// GND -> GND

// Definiowanie pinów
const int joystick_X = A0;  // Oś X (lewo/prawo)
const int joystick_Y = A1;  // Oś Y (góra/dół - ZAMIENIONE)
const int joystick_SW = 2;  // Przycisk (switch) - SPACJA

// Zmienne do przechowywania wartości
int x_value = 0;
int y_value = 0;
int button_value = 0;

// Zmienne do przechowywania poprzednich wartości
int prev_x_value = 512;
int prev_y_value = 512;
int prev_button_value = 1;

// Próg czułości
const int sensitivity_threshold = 30;

// Prędkość ruchu myszki
const int mouse_speed = 5;

void setup() {
  Serial.begin(9600);
  pinMode(joystick_SW, INPUT_PULLUP);
  
  Serial.println("=== Joystick jako Myszka (przez port szeregowy) ===");
  Serial.println("LEWO/PRAWO: ruch myszki OK");
  Serial.println("GÓRA/DÓŁ: ruch myszki ZAMIENIONY");
  Serial.println("PRZYCISK JOYSTICK'A: SPACJA");
  Serial.println("Format: X|Y|BUTTON");
  Serial.println("");
}

void loop() {
  // Odczyt wartości analogowych (0-1023)
  x_value = analogRead(joystick_X);
  y_value = analogRead(joystick_Y);
  
  // Odczyt wartości cyfrowej przycisku
  button_value = digitalRead(joystick_SW);
  
  // Sprawdzenie czy X się zmienił o więcej niż próg
  bool x_changed = abs(x_value - prev_x_value) > sensitivity_threshold;
  
  // Sprawdzenie czy Y się zmienił o więcej niż próg
  bool y_changed = abs(y_value - prev_y_value) > sensitivity_threshold;
  
  // Sprawdzenie czy przycisk się zmienił
  bool button_changed = (prev_button_value != button_value);
  
  // Jeśli coś się zmieniło - wyślij dane
  if (x_changed || y_changed || button_changed) {
    // Oblicz ruchy myszki
    int x_move = 0;
    if (x_value < 300) {
      x_move = -mouse_speed;  // LEWO
    } else if (x_value > 700) {
      x_move = mouse_speed;   // PRAWO
    }
    
    // Y - ZAMIENIONE
    int y_move = 0;
    if (y_value < 300) {
      y_move = -mouse_speed;  // GÓRA (zamienione)
    } else if (y_value > 700) {
      y_move = mouse_speed;   // DÓŁ (zamienione)
    }
    
    // Przycisk: 0 = wciśnięty (SPACJA), 1 = zwolniony (nic)
    int button_send = (button_value == 0) ? 1 : 0;
    
    // Wyślij dane w formacie: X|Y|BUTTON
    Serial.print(x_move);
    Serial.print("|");
    Serial.print(y_move);
    Serial.print("|");
    Serial.println(button_send);
    
    // Zaktualizuj poprzednie wartości
    if (x_changed) prev_x_value = x_value;
    if (y_changed) prev_y_value = y_value;
    if (button_changed) prev_button_value = button_value;
  }
  
  delay(20);  // Sprawdzaj co 20ms dla płynnego ruchu
}
