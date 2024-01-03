int blue = 2;
int red = 3;
int yellow = 4;
int green_start = 9;
int red_start = 8;

int output_wait_time = 750;
unsigned long input_wait_time = 2000;
int stack_top = -1;
int stack_length = 5;
unsigned long start_time;
int stack[10];
bool debug = true;
bool game_start = false;


void setup() {
  Serial.begin(9600);
  pinMode(green_start, OUTPUT);
  pinMode(red_start, OUTPUT);
  
//  Pin modes for blue, red, and yellow are dynamically changed from OUTPUT to INPUT, and back to OUTPUT throughout the program
  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);

//  Restart game function also begins the game.
  restart_game();
  
}

void loop() {

  
if (check_user_input()) {
//  If user gets a step correct we remove the element on the top of the stack
    stack_top = stack_top - 1;
    if (stack_top == -1) {
      if (debug) {
        Serial.println("User has completed sequence");
      }
      victory();
      restart_game();
    }
  } else {
      if (debug) {
        Serial.println("User has failed sequence");
      }
      // Incorrect input
      loss();
      // Restart the game
      restart_game();
  }
}

void generate_pattern() {
//  generates three random numbers between 1 and 3 and stores them in the stack
  for (int i = 0; i < stack_length; ++i) {
    stack[i] = random(1, 4);
//  Increment stack_top. this tells us the how full our stack is
    stack_top++;
    if (debug) {
      Serial.print("Iteration: ");
      Serial.print(i);
      Serial.print(" Stack Top: ");
      Serial.print(stack_top);
      Serial.print(" Stack Top Value: ");
      Serial.print(stack[i]);
      Serial.println();
    }
  }
}

void display_pattern() {
//  Change LED mode to output to display the lights
  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  long colour;
//  Traverse the stack from top to bottom displaying each element as an LED signal on the board
  for (int i = stack_length - 1; i >= 0; --i) {
    colour = stack[i];
    // Turn on LED based on colour
    if (colour == 1) {
      if (debug) {
        Serial.println("blue");
      }
      digitalWrite(blue, HIGH);
      delay(output_wait_time);
      digitalWrite(blue, LOW);
    } else if (colour == 2) {
        if (debug) {
          Serial.println("red");
        }
        digitalWrite(red, HIGH);
        delay(output_wait_time);
        digitalWrite(red, LOW);
    } else {
        if (debug) {
          Serial.println("yellow");
        }
        digitalWrite(yellow, HIGH);
        delay(output_wait_time);
        digitalWrite(yellow, LOW);
    }
  delay(output_wait_time / 2);
  }
}

bool check_user_input() {
  pinMode(blue, INPUT);
  pinMode(red, INPUT);
  pinMode(yellow, INPUT);
  while (true) {
    if (digitalRead(red) == HIGH) {
      if (debug) {
        Serial.println("User hit red button");
        Serial.println(2L == stack[stack_top]);
      }
      delay(300);
      return (2L == stack[stack_top]);
    }
    if (digitalRead(blue) == HIGH) {
      if (debug) {
        Serial.println("User hit blue button");
        Serial.println(1L == stack[stack_top]);
      }
      delay(300);
      return (1L == stack[stack_top]);
    }
    
    if (digitalRead(yellow) == HIGH) {
      if (debug) {
        Serial.println("User hit yellow button");
        Serial.println(3L == stack[stack_top]);
      }
      delay(300);
      return (3L == stack[stack_top]);
    }
    
  }
}

void restart_game() {
// Restart the game with a new LED sequence.
// when the arduino is first supplied power the game will not begin until the user hits one of the three buttons.
// after the user hits one of the three buttons to start the game the red and green start LED's will flash a couple of times to tell the player they have started the game.   
  pinMode(blue, INPUT);
  pinMode(red, INPUT);
  pinMode(yellow, INPUT);
  if (not game_start){
    game_start = true;
    while (1) {
      if (digitalRead(blue) == HIGH or digitalRead(red) == HIGH or digitalRead(yellow) == HIGH){
        break;
      }
    }
    digitalWrite(green_start, HIGH);
    digitalWrite(red_start, HIGH);
    delay(output_wait_time);
    digitalWrite(green_start, LOW);
    digitalWrite(red_start, LOW);
    delay(output_wait_time);
    digitalWrite(green_start, HIGH);
    digitalWrite(red_start, HIGH);
    delay(output_wait_time);
    digitalWrite(green_start, LOW);
    digitalWrite(red_start, LOW);
    delay(500);
    
  }
    
  
  stack_top = -1;
  generate_pattern();
  display_pattern();
}

void victory() {
//  Flashes the green LED to signify the user has completed the sequence.
  digitalWrite(green_start, HIGH);
  delay(output_wait_time);
  digitalWrite(green_start, LOW);
  delay(output_wait_time);
  digitalWrite(green_start, HIGH);
  delay(output_wait_time);
  digitalWrite(green_start, LOW);
  
}

void loss() {
//  Flashes the red LED to signify the user has not completed the sequence properly.
  digitalWrite(red_start, HIGH);
  delay(output_wait_time);
  digitalWrite(red_start, LOW);
  delay(output_wait_time);
  digitalWrite(red_start, HIGH);
  delay(output_wait_time);
  digitalWrite(red_start, LOW);
}
