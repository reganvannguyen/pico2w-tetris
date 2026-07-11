#include "game.h"

void Game::reset() {
    std::queue<Piece> empty_queue;
    q_pieces.swap(empty_queue);
    fall_timing = 1;
    score = 0;
    level = 1;
    line_cleared = 0;
    is_game_over = false;
    current_piece = Piece();
    held_piece = Piece();
    has_held_piece = false;
    hold_used_this_turn = false;
    reset_lock_delay();
}

void Game::offset_lock_delay(unsigned long paused_duration) {
    if (lock_delay_active) {
        lock_delay_started_at += paused_duration;
    }
}



  void Game::set_fall_timing(){
    fall_timing = fall_timing +1;
  }


  bool Game::can_move_down(const Piece& piece, int board[20][10]){ //check and see if piece can move down
    for(int i=0; i < 4; i++){
      for(int j=0; j < 4; j++){
        if(piece.shape[i][j] == 1){
          int piece_location_x = j + piece.x; //side 
          int piece_location_y = i + piece.y + 1; //going down

          if(piece_location_y >=20){
            return false;
          }

          if(piece_location_x < 0 || piece_location_x >= 10){
            return false;
          }

          if(board[piece_location_y][piece_location_x] != 0){ //checks on the board to see if there is a colision.
            return false;
          }
        }
      }
    }
      return true;
  }

  bool Game::can_move_left(const Piece& piece, int board[20][10]){
    for(int i=0; i < 4; i++){
      for(int j=0; j < 4; j++){
        if(piece.shape[i][j] == 1){
          int piece_location_x = j + piece.x -1; //side 
          int piece_location_y = i + piece.y; //going down

          if(piece_location_x < 0){
            return false;
          }

          if(piece_location_y < 0 || piece_location_y >= 20){
            return false;
          }

          if(board[piece_location_y][piece_location_x] != 0){ //checks on the board to see if there is a colision.
            return false;
          }
        }
      }
    }
      return true;
    
  }

  bool Game::can_move_right(const Piece& piece, int board[20][10]){
    for(int i=0; i < 4; i++){
      for(int j=0; j < 4; j++){
        if(piece.shape[i][j] == 1){
          int piece_location_x = j + piece.x + 1; //side 
          int piece_location_y = i + piece.y; //going down

          if(piece_location_x >= 10){
            return false;
          }

          if(piece_location_y < 0 || piece_location_y >= 20){
            return false;
          }

          if(board[piece_location_y][piece_location_x] != 0){ //checks on the board to see if there is a colision.
            return false;
          }
        }
      }
    }
      return true;
  }

    bool Game::can_rotate(const Piece& piece, int board[20][10]){

    //does matrix algebra to rotate on copy of piece    
      Piece temp = piece;

      for (int i = 0; i < 4; i++) {
            for (int j = i + 1; j < 4; j++) {
                std::swap(temp.shape[i][j], temp.shape[j][i]);
            }
        }

      for (int i = 0; i < 4; i++) {
          std::reverse(temp.shape[i], temp.shape[i] + 4);
      }
      
      //check for colission
      for(int i=0; i < 4; i++){
        for(int j=0; j < 4; j++){
          if(temp.shape[i][j] == 1){
              int piece_location_x = j + temp.x; //side 
              int piece_location_y = i + temp.y; //going down
            
            if(piece_location_x >= 10 || piece_location_x < 0){ //walls of board
              return false;
            }
            if(piece_location_y < 0 || piece_location_y >= 20){ //ground of board
              return false;
            }
            
            if(board[piece_location_y][piece_location_x] != 0){ //checks on the board to see if there is a colision.
              return false;
            }
          }
        }
      }
        return true;
    }
  

  void Game::down(Piece& piece, int board[20][10]){
    if(can_move_down(piece, board) == true) {
        piece.y++;
    }
  }

  bool Game::left(Piece& piece, int board[20][10]){
    if(can_move_left(piece, board) == true){
        piece.x--;
        return true;
    }
    return false;
  }

  bool Game::right(Piece& piece, int board[20][10]){
    if(can_move_right(piece, board) == true){
        piece.x++;
        return true;
    }
    return false;
  }


  bool Game::rotate(Piece& piece, int board[20][10]){
    if(can_rotate(piece, board)== true){
      for (int i = 0; i < 4; i++) {
          for (int j = i + 1; j < 4; j++) {
              std::swap(piece.shape[i][j], piece.shape[j][i]);
          }
      }

      for (int i = 0; i < 4; i++) {
          std::reverse(piece.shape[i], piece.shape[i] + 4);
      }
      return true;
    }
    return false;
}

 int Game::hard_drop(Board& board) {
    int distance = 0;
    while (can_move_down(current_piece, board.grid)) {
        current_piece.y++;
        distance++;
    }

    score += distance * 2;
    lock_current_piece(board);
    return distance;
 }

 bool Game::hold_current_piece(Board& board) {
    if (hold_used_this_turn || is_game_over) {
        return false;
    }

    int outgoing_piece_id = current_piece.piece_id;
    hold_used_this_turn = true;
    reset_lock_delay();

    if (!has_held_piece) {
        held_piece = Piece(outgoing_piece_id);
        has_held_piece = true;
        add_piece_board(board.grid, false);
    } else {
        int incoming_piece_id = held_piece.piece_id;
        held_piece = Piece(outgoing_piece_id);
        current_piece = Piece(incoming_piece_id);
        if (game_over(current_piece, board.grid)) {
            is_game_over = true;
        }
    }

    return true;
 }

 const Piece* Game::next_piece() const {
    if (q_pieces.empty()) {
        return 0;
    }
    return &q_pieces.front();
 }

 void Game::reset_lock_delay() {
    lock_delay_active = false;
    lock_delay_started_at = 0;
    lock_reset_count = 0;
 }

 void Game::register_lock_delay_movement(unsigned long now) {
    if (lock_delay_active && lock_reset_count < MAX_LOCK_RESETS) {
        lock_delay_started_at = now;
        lock_reset_count++;
    }
 }

 void Game::decide_when_to_lock_piece(Piece& piece, Board& board, unsigned long now) {
    if (can_move_down(piece, board.grid)) {
        reset_lock_delay();
        return;
    }

    if (!lock_delay_active) {
        lock_delay_active = true;
        lock_delay_started_at = now;
        return;
    }

    if (now - lock_delay_started_at >= LOCK_DELAY_MS) {
        lock_current_piece(board);
    }
}

 void Game::lock_current_piece(Board& board) {
    board.lock_piece(current_piece);

    int lines = board.clear_full_lines();
    if (lines > 0) {
        line_cleared += lines;
        set_score(lines);
        set_level();
    }

    add_piece_board(board.grid, true);
    reset_lock_delay();
 }

  void Game::create_piece() {
    int piece_type = std::rand() % 7; // gives 0 to 6
    q_pieces.emplace(piece_type);
  }

 void Game::add_piece_board(int board[20][10], bool reset_hold) {
    if (q_pieces.empty()) {
        create_piece();
    }

    current_piece = q_pieces.front();
    q_pieces.pop();

    if (reset_hold) {
        hold_used_this_turn = false;
    }

    if (game_over(current_piece, board)) {
        is_game_over = true;
        return;
    }

    create_piece(); // keeps queue filled for next piece
  }


  void Game::set_score(int lines){
    score = score  + (100 * lines);
  }

  void Game::set_level(){
    if(line_cleared >= 5){
      line_cleared -= 5;
      level++;
    }
  }

  bool Game::game_over(const Piece& piece, int board[20][10]) {
      for (int i = 0; i < 4; i++) {
          for (int j = 0; j < 4; j++) {
              if (piece.shape[i][j] == 1) {
                  int boardX = piece.x + j;
                  int boardY = piece.y + i;

                  if (boardY >= 0 && boardY < 20 && boardX >= 0 && boardX < 10) {
                      if (board[boardY][boardX] != 0) {
                          return true;
                      }
                  }
              }
          }
      }

      return false;
  }
