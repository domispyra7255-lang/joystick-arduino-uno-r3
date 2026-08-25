// Arduino UNO R3 - Odczytywanie pozycji modułu Joystick
// Pinowanie:
// VRx -> A0 (analogowy)
// VRy -> A1 (analogowy)
// SW  -> Pin 2 (cyfrowy)
// +5V -> 5V
// GND -> GND

// Definiowanie pinów
const int joystick_X = A0;  // Oś X
const int joystick_Y = A1;  // Oś Y
const int joystick_SW = 2;  // Przycisk (switch)

// Zmienne do przechowywania wartości
int x_value = 0;
int y_value = 0;
int button_value = 0;

void setup() {
  Serial.begin(9600);  // Inicjalizacja komunikacji szeregowej
  pinMode(joystick_SW, INPUT_PULLUP);  // Przycisk z wewnętrznym pull-up
  
  Serial.println("=== Joystick Arduino UNO R3 ===");
  Serial.println("Pozycja joystick'a będzie wyświetlana co 500ms");
  Serial.println("");
}

void loop() {
  // Odczyt wartości analogowych (0-1023)
  x_value = analogRead(joystick_X);
  y_value = analogRead(joystick_Y);
  
  // Odczyt wartości cyfrowej przycisku (0 = wciśnięty, 1 = zwolniony)
  button_value = digitalRead(joystick_SW);
  
  // Wyświetlenie surowych wartości
  Serial.print("X: ");
  Serial.print(x_value);
  Serial.print(" | Y: ");
  Serial.print(y_value);
  Serial.print(" | Przycisk: ");
  Serial.println(button_value == 0 ? "WCISNIETY" : "ZWOLNIONY");
  
  // Określenie pozycji joystick'a
  printJoystickPosition(x_value, y_value);
  
  Serial.println("---");
  delay(500);  // Czekaj 500ms przed następnym odczytem
}

// Funkcja do wyświetlania kierunku joystick'a
void printJoystickPosition(int x, int y) {
  Serial.print("Pozycja: ");
  
  // Określenie kierunku
  if (x < 300) {
    Serial.print("LEWO ");
  } else if (x > 700) {
    Serial.print("PRAWO ");
  } else {
    Serial.print("SRODEK_X ");
  }
  
  if (y < 300) {
    Serial.print("| DÓŁ");
  } else if (y > 700) {
    Serial.print("| GÓRA");
  } else {
    Serial.print("| SRODEK_Y");
  }
  
  Serial.println();
}
