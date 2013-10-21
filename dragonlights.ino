// sketch setup
int delay_ms = 100;

// light pins (EL + LED)
int red_body_pin = 2;
int green_body_pin = 3;
int red_eye_pin = 12;
int green_eye_pin = 13;

// state tracking for lighting programs
int fade_index = 0;
int fade_ticks = 0;
int program_mode_id = 0;

// buttons!
int program_button_solid_green = 11;
int program_button_solid_red = 10;
int program_button_fade_to_green = 9;

// button debouncing
int button_pressed_state = LOW; // change for normally-open
int program_transition_to = 0;

// light helper methods

void turn_on_green() {
  digitalWrite(green_body_pin, HIGH);
  digitalWrite(green_eye_pin, HIGH);
}

void turn_on_red() {
  digitalWrite(red_body_pin, HIGH);
  digitalWrite(red_eye_pin, HIGH);
}

void turn_off_green() {
  digitalWrite(green_body_pin, LOW);
  digitalWrite(green_eye_pin, LOW);
}

void turn_off_red() {
  digitalWrite(red_body_pin, LOW);
  digitalWrite(red_eye_pin, LOW);
}

void pulse_green(int ticks) {
  turn_on_green();
  fade_ticks++;
  if (fade_ticks > ticks) {
    fade_index++;
    fade_ticks = 0;
  }
}

void pulse_red(int ticks) {
  turn_on_red();
  fade_ticks++;
  if (fade_ticks > ticks) {
    fade_index++;
    fade_ticks = 0;
  }
}

void setup() {
  pinMode(red_body_pin, OUTPUT);
  digitalWrite(red_body_pin, LOW);
  pinMode(green_body_pin, OUTPUT);
  digitalWrite(green_body_pin, LOW);
  pinMode(red_eye_pin, OUTPUT);
  digitalWrite(red_eye_pin, LOW);
  pinMode(green_eye_pin, OUTPUT);
  digitalWrite(green_eye_pin, LOW);
  
  pinMode(program_button_solid_green, INPUT);
  digitalWrite(program_button_solid_green, HIGH);
  pinMode(program_button_solid_red, INPUT);
  digitalWrite(program_button_solid_red, HIGH);
  pinMode(program_button_fade_to_green, INPUT);
  digitalWrite(program_button_fade_to_green, HIGH);
  
  Serial.begin(9600);
}

void run_program_mode(int id) {
  switch(id) {
    case 0:
      program_nothing();
      break;
    case 1:
      program_green_solid();
      break;
    case 2:
      program_red_solid();
      break;
    case 3:
      program_fade_to_green();
      break;
  }
}

void check_program_change() {
  check_button_program(program_button_solid_green, 1);
  check_button_program(program_button_solid_red, 2);
  check_button_program(program_button_fade_to_green, 3);
}

void check_button_program(int pin, int program_index) {
  if (digitalRead(pin) == button_pressed_state){
    if (program_transition_to == program_index) {
      program_mode_id = program_index;
      program_transition_to = 0;
    }
    else {
      program_transition_to = program_index;
      Serial.println("switch program");
      Serial.println(program_index);
    }
  }
}

void program_nothing() {}

void program_green_solid() {
  turn_off_red();
  turn_on_green();
}

void program_red_solid() {
  turn_off_green();
  turn_on_red();
}

void program_fade_to_green() {
  switch(fade_index) {
    
    // 3/4 time red, 1/4 time green
    case 0:
      turn_off_red();
      pulse_green(3);
      break;
    case 1:
      turn_off_green();
      pulse_red(9);
      break;

    // 1/2 time each color
    case 2:
      turn_off_red();
      pulse_green(6);
      break;
    case 3:
      turn_off_green();
      pulse_red(6);
      break;

    // 3/4 time green, 1/4 time red
    case 4:
      turn_off_red();
      pulse_green(9);
      break;
    case 5:
      turn_off_green();
      pulse_red(3);
      break;

    case 6:
      // solid green!
      turn_off_red();
      turn_on_green();
    }
}

void loop() {
 run_program_mode(program_mode_id);
 check_program_change();
 delay(delay_ms); // quarter-second ticks
}
