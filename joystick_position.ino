// Arduino UNO R3 - Joystick jako myszka USB + spacja na klawiaturze
// LEWO/PRAWO OK, GÓRA/DÓŁ ZAMIENIONE
// Pinowanie:
// VRx -> A0 (analogowy)
// VRy -> A1 (analogowy)
// SW  -> Pin 2 (cyfrowy - wciśnięcie = SPACJA)
// +5V -> 5V
// GND -> GND

#include <Mouse.h>
#include <Keyboard.h>

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
  Mouse.begin();
  Keyboard.begin();
  
  Serial.println("=== Joystick jako Myszka USB + Klawiatura ===");
  Serial.println("LEWO/PRAWO: ruch myszki OK");
  Serial.println("GÓRA/DÓŁ: ruch myszki ZAMIENIONY");
  Serial.println("PRZYCISK JOYSTICK'A: SPACJA");
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
  
  // Ruch myszki (X - lewo/prawo)
  if (x_changed) {
    int x_move = 0;
    if (x_value < 300) {
      x_move = -mouse_speed;  // LEWO
    } else if (x_value > 700) {
      x_move = mouse_speed;   // PRAWO
    }
    Mouse.move(x_move, 0, 0);
    prev_x_value = x_value;
  }
  
  // Ruch myszki (Y - góra/dół ZAMIENIONE)
  if (y_changed) {
    int y_move = 0;
    if (y_value < 300) {
      y_move = -mouse_speed;  // GÓRA (zamienione - było dół)
    } else if (y_value > 700) {
      y_move = mouse_speed;   // DÓŁ (zamienione - była góra)
    }
    Mouse.move(0, y_move, 0);
    prev_y_value = y_value;
  }
  
  // SPACJA na przycisk joystick'a
  if (button_changed) {
    if (button_value == 0) {
      Keyboard.press(' ');
      Serial.println("SPACJA: WCIŚNIĘTA");
    } else {
      Keyboard.release(' ');
      Serial.println("SPACJA: ZWOLNIONA");
    }
    prev_button_value = button_value;
  }
  
  delay(20);  // Sprawdzaj co 20ms dla płynnego ruchu
}
