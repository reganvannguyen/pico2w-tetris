#include "game.h"



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

          if(board[piece_location_y][piece_location_x] == 1){ //checks on the board to see if there is a colision.
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

          if(board[piece_location_y][piece_location_x] == 1){ //checks on the board to see if there is a colision.
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

          if(board[piece_location_y][piece_location_x] == 1){ //checks on the board to see if there is a colision.
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
            
            if(board[piece_location_y][piece_location_x] == 1){ //checks on the board to see if there is a colision.
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

  void Game::left(Piece& piece, int board[20][10]){
    if(can_move_left(piece, board) == true){
        piece.x--;
    }
  }

  void Game::right(Piece& piece, int board[20][10]){
    if(can_move_right(piece, board) == true){
        piece.x++;
    }
  }


  void Game::rotate(Piece& piece, int board[20][10]){ 
    if(can_rotate(piece, board)== true){
      for (int i = 0; i < 4; i++) {
          for (int j = i + 1; j < 4; j++) {
              std::swap(piece.shape[i][j], piece.shape[j][i]);
          }
      }

      for (int i = 0; i < 4; i++) {
          std::reverse(piece.shape[i], piece.shape[i] + 4);
      }

    }
    


}


 void Game::decide_when_to_lock_piece(Piece& piece, Board& board) {
    if (!can_move_down(piece, board.board)) {
        board.lock_piece(piece);

        int lines = board.clear_full_lines();

        if (lines > 0) {
            line_cleared += lines;
            set_score(lines);
            set_level();
        }

        add_piece_board(board.board);
    }
}

  void Game::create_piece() {
    int piece_type = std::rand() % 7; // gives 0 to 6
    q_pieces.emplace(piece_type);
  }

 void Game::add_piece_board(int board[20][10]) {
    if (q_pieces.empty()) {
        create_piece();
    }

    current_piece = q_pieces.front();
    q_pieces.pop();

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
                      if (board[boardY][boardX] == 1) {
                          return true;
                      }
                  }
              }
          }
      }

      return false;
  }