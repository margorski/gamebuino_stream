#include <Gamebuino-Meta.h>

const float BALL_SPEED = 1;
const int BALL_SIZE = 3;

const int PADDLE_WIDTH = 3;
const int PADDLE_HEIGHT = 9;
const int PADDLE_SPEED = 2;

const int PLAYER1_X = 5;
const int PLAYER2_X = gb.display.width() - 5;

int ball_speed_x, ball_speed_y;
int ball_pos_x, ball_pos_y;
int player1_y, player2_y;
int score_p1, score_p2;
bool is_playing;

void setup() {
  gb.begin();
  init_after_score();
}

void init_after_score() {
  ball_pos_x = (gb.display.width() + BALL_SIZE)/2;
  ball_pos_y = (gb.display.height() + BALL_SIZE)/2;
  ball_speed_x = ball_speed_y = BALL_SPEED;
  player1_y = player2_y = gb.display.height() / 2;
  is_playing = false;
}

void check_winner() {
  if (score_p1 > 15 || score_p2 > 15) {
      score_p1 = score_p2 = 0;
  }
  init_after_score();
}

void ball_update() {
  ball_pos_x = ball_pos_x + ball_speed_x;
  
  if ((ball_pos_x <= PLAYER1_X + PADDLE_WIDTH) &&
      (ball_pos_y >= player1_y) &&
      (ball_pos_y < player1_y + PADDLE_HEIGHT)) {
    ball_speed_x = ball_speed_x * (-1);      
  }

  if ((ball_pos_x + BALL_SIZE >= PLAYER2_X) &&
      (ball_pos_y >= player2_y) &&
      (ball_pos_y < player2_y + PADDLE_HEIGHT)) {
    ball_speed_x = ball_speed_x * (-1);      
  }

  if (ball_pos_x + BALL_SIZE < 0) {
    score_p2 = score_p2 + 1;
    check_winner();
  }

  if (ball_pos_x > gb.display.width()) {
    score_p1 = score_p1 + 1;
    check_winner();
  }
 
  ball_pos_y = ball_pos_y + ball_speed_y;
  
  // sprawdzanie cyz uderzyla w sufit
  if (ball_pos_y <= 0) {
    ball_speed_y = ball_speed_y * (-1);
  }

  // sprawdzanie czy uderzyla w podloge
  if ((ball_pos_y + BALL_SIZE) >= gb.display.height()) {
    ball_speed_y = ball_speed_y * (-1); 
  }
}

void player_update() {
    if (gb.buttons.repeat(BUTTON_DOWN, 0)) {
      player1_y = player1_y + PADDLE_SPEED;

      int bottom_of_screen = gb.display.height() - PADDLE_HEIGHT; 
      if (player1_y > bottom_of_screen) {
        player1_y = bottom_of_screen;
      }
    }

    if (gb.buttons.repeat(BUTTON_UP, 0)) {
      player1_y = player1_y - PADDLE_SPEED;

      if (player1_y < 0) {
        player1_y = 0;
      }
    }
}

void computer_update() {
  if (ball_pos_y < player2_y) {
    player2_y = player2_y - 1;  
  }
  if (ball_pos_y > player2_y) {
    player2_y = player2_y + 1;
  }
}

void draw() {
  gb.display.fillRect(ball_pos_x, ball_pos_y, BALL_SIZE, BALL_SIZE);
  gb.display.fillRect(PLAYER1_X, player1_y, PADDLE_WIDTH, PADDLE_HEIGHT);
  gb.display.fillRect(PLAYER2_X, player2_y, PADDLE_WIDTH, PADDLE_HEIGHT);

  gb.display.setCursor(30, 5);
  gb.display.print(score_p1);
  gb.display.print(" : ");
  gb.display.print(score_p2);
}

void loop() {
  while (!gb.update());
  gb.display.clear();

  if (is_playing) {
    ball_update();
    player_update();
    computer_update();
  }
  else {
    if (gb.buttons.pressed(BUTTON_A)) {
      is_playing = true;
    }
  }
    
  draw();
}

