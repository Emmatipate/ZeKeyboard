#include "tetris.h"

void draw_tetris_board(LED leds[][Ze::NUM_COLS],
        tetris::SquareType tetris_board[][tetris::NUM_COLS],
        tetris::Tetromino* falling_tetromino) {

    tetris::SquareType rendered_board[Ze::NUM_ROWS][Ze::NUM_COLS];

    // transfer the board, transposing it
    for (uint8_t row = 0; row < Ze::NUM_ROWS; ++row) {
        for (uint8_t col = 0; col < Ze::NUM_COLS; ++col) {
            Serial.print(row);
            Serial.print(", ");
            Serial.println(col);
            rendered_board[row][col] = tetris_board[col][row];
        }
    }


    Serial.println("Drawing tetromino");
    if (is_tetromino_falling(falling_tetromino)) {

        // transfer the falling tetromino, transposing it
        tetris::SquareType type = tetris::tetromino_type_to_square_type(
                falling_tetromino->type);
        uint8_t tx = falling_tetromino->x;
        uint8_t ty = falling_tetromino->y;

        for (uint8_t row = 0; row < tetris::TETROMINO_SIZE; ++row) {
            for (uint8_t col = 0; col < tetris::TETROMINO_SIZE; ++col) {
                if (falling_tetromino->body[row][col] != 0) {
                    Serial.println("WE FOUND AN ACTIVE CELL");
                    Serial.print(row + ty);
                    Serial.print(", ");
                    Serial.println(col + tx);
                    rendered_board[col + tx][row + ty] = type;
                } else {
                    Serial.print(row + ty);
                    Serial.print(", ");
                    Serial.println(col + tx);
                }
            }
        }

    }

    for (uint8_t row = 0; row < Ze::NUM_ROWS; ++row) {
        for (uint8_t col = 0; col < Ze::NUM_COLS; ++col) {
            Color c = tetris::square_type_to_color(rendered_board[row][col]);
            leds[row][col].r = c.r;
            leds[row][col].g = c.g;
            leds[row][col].b = c.b;
        }
    }
}

void tetris_setup(LED leds[][Ze::NUM_COLS], 
        tetris::SquareType tetris_board[][tetris::NUM_COLS],
        tetris::Tetromino* falling_tetromino) {
    Serial.println("Setting up");
    tetromino_deinit(falling_tetromino);
    for (uint8_t row = 0; row < Ze::NUM_ROWS; ++row) {
        for (uint8_t col = 0; col < Ze::NUM_COLS; ++col) {
            leds[row][col].r = 0;
            leds[row][col].g = 0;
            leds[row][col].b = 0;
            leds[row][col].aux = nullptr;
        }
    }
}

void tetris_update(LED leds[][Ze::NUM_COLS], Ze::Board* board, uint64_t it,
        tetris::SquareType tetris_board[][tetris::NUM_COLS],
        tetris::Tetromino* falling_tetromino) {
    if (it % TETRIS_DELAY == 0) {
        Serial.println("TICK");
        tetris::tick(tetris_board, falling_tetromino, it);
        draw_tetris_board(leds, tetris_board, falling_tetromino);
    }
}
