#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int take_corners(int);

// explicit instantiation
int moves[9] = {0};
int moves_made = 0;
int computer_mv = 0;

enum { human_player = 1, computer_player = 2 };
enum { top_left, top_middle, top_right, middle_left, middle, middle_right,
       bottom_left, bottom_middle, bottom_right };

void display_tutorial_board() {
  int move;
  for (int i = 0; i < 3; i++) {
    printf("|");
    for (int j = 0; j < 3; j++) {
      move = j + i * 3 + 1;
      printf("_%d_|", move);
    }
    printf("\n\n");
  }
}

void display_board() {
  char mark;
  int move;
  printf("\n");
  for (int i = 0; i < 3; i++) {
    printf("|");
    for (int j = 0; j < 3; j++) {

      move = moves[j + i * 3];

      if ( !move ) mark = '_';
      else if (move == human_player) mark = 'X';
      else mark = 'O';

      printf("_%c_|", mark);
    }
    printf("\n\n");
  }
}

int _check_win(int x) {
  int player_wins = x;
  int aux = 0;
  for (int i = 0; i < 9; i += 3) {
    aux |= moves[i] & moves[i+1] & moves[i+2]; // linii
  }
  for (int i = 0; i < 3; i++) {
    aux |= moves[i] & moves[i+3] & moves[i+6]; // coloane
  }
  aux |= moves[top_left] & moves[middle] & moves[bottom_right]; // principal
  aux |= moves[top_right] & moves[middle] & moves[bottom_left]; // secondary
  player_wins &= aux;
  return player_wins;
}

int check_win() {
  int player_wins = human_player;
  player_wins &= _check_win(player_wins);
  int computer_wins = computer_player;
  computer_wins &= _check_win(computer_wins);
  return player_wins | computer_wins;
}

int block(int player) {
  int move = -1;

  for (int i = 0; i < 9; i += 3) { // blocheaza liniile.
    if ( ((moves[i] & moves[i + 1]) == player)  && !moves[i + 2] ) {
      moves[i + 2] = computer_player;
      move = i + 2;
      return move + 1;
    } else if ( ((moves[i] & moves[i + 2]) == player)  && !moves[i + 1] ) {
      moves[i + 1] = computer_player;
      move = i + 1;
      return move + 1;
    } else if ( ((moves[i + 1] & moves[i + 2]) == player)  && !moves[i] ) {
      moves[i] = computer_player;
      move = i;
      return move + 1;
    }
  }

  for (int i = 0; i < 3; i++) { //blocheaza coloanele
    if ( ((moves[i] & moves[i + 3]) == player)  && !moves[i + 6] ) {
      moves[i+6] = computer_player;
      move = i+6;
      return move +1;
    } else if ( ((moves[i] & moves[i + 6]) == player)  && !moves[i + 3] ) {
      moves[i+3] = computer_player;
      move = i+3;
      return move +1;
    } else if ( ((moves[i + 3] & moves[i + 6]) == player)  && !moves[i] ) {
      moves[i] = computer_player;
      move = i;
      return move +1;
    }
  }

  // principal
  if ( ((moves[top_left] & moves[middle]) == player) && !moves[bottom_right]  ) {
    moves[bottom_right] = computer_player;
    return bottom_right + 1;
  } else if ( ((moves[top_left] & moves[bottom_right]) == player) && !moves[middle]  ) {
    moves[middle] = computer_player;
    return middle + 1;
  } if ( ((moves[bottom_right] & moves[middle]) == player) && !moves[top_left]  ) {
    moves[top_left] = computer_player;
    return top_left + 1;
  }

  // secondary
  if ( ((moves[top_right] & moves[middle]) == player) && !moves[bottom_left]  ) {
    moves[bottom_left] = computer_player;
    return bottom_left + 1;
  } else if ( ((moves[top_right] & moves[bottom_left]) == player) && !moves[middle]  ) {
    moves[middle] = computer_player;
    return middle + 1;
  } if ( ((moves[bottom_left] & moves[middle]) == player) && !moves[top_right]  ) {
    moves[top_right] = computer_player;
    return top_right + 1;
  }
  return move + 1;
}

int try_to_win(int player) {
  int move = -1;
  if (moves[middle] == computer_player) {
    for (int i = 0; i < 9; i += 3) { // tries to make a line in order to not pun random things.
      if ( (moves[i] | moves[i+1] | moves[i+2]) == player ) {
        if (!moves[i]) {
          moves[i] = player;
          move = i;
          return move + 1;
        } else if (!moves[i+1]) {
          moves[i + 1] = player;
          move = i + 1;
          return move + 1;
        } else if (!moves[i+2]) {
          moves[i + 2] = player;
          move = i + 2;
          return move + 1;
        }
      }
    }
  } else {
    return take_corners(computer_player);
  }
  return move + 1;
}

int computer_move() {
  int move = rand() % 9;
  if ( moves[move] == 0 ) {
    moves[move] = 2;
  } else {
    return computer_move();
  }
  return move + 1;
}

int take_corners(int player) {
  int move = rand() % 4;
  int corners[4] = {top_left, top_right, bottom_left, bottom_right};

  // prevent the human from playing tricks on us!
  if ( ((moves[top_middle] & moves[middle_left]) == human_player) && !moves[top_left] ) {
    moves[top_left] = computer_player;
    return top_left + 1;
  } else if ( ((moves[bottom_middle] & moves[middle_left]) == human_player) && !moves[bottom_left] ) {
    moves[bottom_left] = computer_player;
    return bottom_left + 1;
  } else if ( ((moves[top_middle] & moves[middle_right]) == human_player) && !moves[top_right] ) {
    moves[top_right] = computer_player;
    return top_right + 1;
  } else if ( ((moves[bottom_middle] & moves[middle_right]) == human_player) && !moves[bottom_right] ) {
    moves[bottom_right] = computer_player;
    return bottom_left + 1;
  }

  if (!moves[corners[move]]) { // random corner
    moves[corners[move]] = player;
    move = corners[move] + 1;
  } else {
    for (int i = 0; i < 4; i++) { // take first available corner
      if (!moves[corners[i]]) {
        moves[corners[i]] = computer_player;
        return corners[i] + 1;
      }
    }
    return computer_move(); // dumb move
  }

  return move;
}

int computer_move_i() {
  int move = 0;

  if ( !moves[middle] ) { // O marks the spot
    moves[middle] = computer_player;
    move = middle;
    return move + 1;
  }

  if ( (move = block(computer_player)) ) {
    return move;
  } else if ( ( move = block(human_player) ) ) { // O is taken, take corners.
    return move;
  } else if ( ( move = try_to_win(computer_player) ) ) { // force block.
    return move;
  } else {
    return take_corners(computer_player);
  }
  return move;
}

int game() {
  int choice;
  printf("Type a number between 1-9: ");

  int c = scanf("%d", &choice);
  if ( c == 0 ) {
    printf("\nINVALID MOVE! WATCH IT KID!\n");
    while( (c = getchar() != '\n') ); // clear STDIN
    return game();
  } else if (choice < 1 || choice > 9) {
    printf("\nINVALID MOVE! WATCH IT KID!\n");
    return game();
  }

  if ( moves[choice - 1] == 0) {
    moves[choice - 1] = 1;
  } else {
    printf("\nWOHO, KID, WHATCHA DOING?!\n");
    return game();
  }
  if (check_win()) {
    return check_win();
  }

  moves_made++;
  if ( moves_made < 9) computer_mv = computer_move_i(); // replace with computer_move for dumb play
  moves_made++;

  return check_win();
}

void game_start() {
  srand( time(NULL) );

  printf("Hello. Imagine every box is number from 1-9.\nType your number and hit" \
  " enter.\n You are playing X.\n\n");
  display_tutorial_board();
  int winner;
  int moves = 0;

  while( !( winner = game() ) && moves++ < 4 ) {
    display_board();
    printf("Computer made a move at: %d\n\n", computer_mv);
  }; // plays until someone wins or the board is filled.

  display_board();
  if ( winner == 1 ) { // Never going to happen.
    printf("Congratulations! YOU'RE WINNER!\n");
  } else if ( winner == 2 ) {
    printf("Congratulations! YOU'RE LOSSER!\n");
  } else {
    printf("Congratulations! YOU'RE DRAFT!\n");
  }
}

int main(int argc, char const *argv[]) {
  game_start();
  return 0;
}
