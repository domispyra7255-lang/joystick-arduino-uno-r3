// Arduino UNO R3 - Odczytywanie pozycji modułu Joystick (tylko przy zmianie)
// OBRÓCONE O 90 STOPNI W PRAWO
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

// Zmienne do przechowywania poprzednich wartości
int prev_x_value = -1;
int prev_y_value = -1;
int prev_button_value = 1;

// Próg czułości (zmiana musi być większa niż ten próg, aby się wyświetlić)
const int sensitivity_threshold = 30;

void setup() {
  Serial.begin(9600);  // Inicjalizacja komunikacji szeregowej
  pinMode(joystick_SW, INPUT_PULLUP);  // Przycisk z wewnętrznym pull-up
  
  Serial.println("=== Joystick Arduino UNO R3 (OBRÓCONY 90° W PRAWO) ===");
  Serial.println("Pozycja będzie wyświetlana TYLKO przy zmianie pozycji");
  Serial.println("GÓRA -> LEWO | PRAWO -> GÓRA | DÓŁ -> PRAWO | LEWO -> DÓŁ");
  Serial.println("");
}

void loop() {
  // Odczyt wartości analogowych (0-1023)
  x_value = analogRead(joystick_X);
  y_value = analogRead(joystick_Y);
  
  // Odczyt wartości cyfrowej przycisku (0 = wciśnięty, 1 = zwolniony)
  button_value = digitalRead(joystick_SW);
  
  // Sprawdzenie czy X się zmienił o więcej niż próg
  bool x_changed = (prev_x_value == -1) || (abs(x_value - prev_x_value) > sensitivity_threshold);
  
  // Sprawdzenie czy Y się zmienił o więcej niż próg
  bool y_changed = (prev_y_value == -1) || (abs(y_value - prev_y_value) > sensitivity_threshold);
  
  // Sprawdzenie czy przycisk się zmienił
  bool button_changed = (prev_button_value != button_value);
  
  // Jeśli coś się zmieniło - wyświetl
  if (x_changed || y_changed || button_changed) {
    Serial.print("X: ");
    Serial.print(x_value);
    Serial.print(" | Y: ");
    Serial.print(y_value);
    Serial.print(" | Przycisk: ");
    Serial.println(button_value == 0 ? "WCISNIETY" : "ZWOLNIONY");
    
    // Określenie pozycji joystick'a (OBRÓCONA O 90 STOPNI)
    printJoystickPosition(x_value, y_value);
    
    Serial.println("---");
    
    // Zaktualizuj poprzednie wartości
    prev_x_value = x_value;
    prev_y_value = y_value;
    prev_button_value = button_value;
  }
  
  delay(50);  // Sprawdzaj co 50ms (nie blokuje, gdy się nic nie zmienia)
}

// Funkcja do wyświetlania kierunku joystick'a (OBRÓCONA O 90 STOPNI W PRAWO)
// Oryginalna mapa:  GÓRA(y>700) PRAWO(x>700) DÓŁ(y<300) LEWO(x<300)
// Nowa mapa:        LEWO(y>700) GÓRA(x>700)  PRAWO(y<300) DÓŁ(x<300)
void printJoystickPosition(int x, int y) {
  Serial.print("Pozycja: ");
  
  // Określenie kierunku (OBRÓCONE 90° W PRAWO)
  if (y > 700) {
    Serial.print("LEWO ");
  } else if (y < 300) {
    Serial.print("PRAWO ");
  } else {
    Serial.print("SRODEK_X ");
  }
  
  if (x > 700) {
    Serial.print("| GÓRA");
  } else if (x < 300) {
    Serial.print("| DÓŁ");
  } else {
    Serial.print("| SRODEK_Y");
  }
  
  Serial.println();
}
