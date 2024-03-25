#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>


#define BOARD_SIZE 15
#define CELL_SIZE 90
#define BOARD_WIDTH (BOARD_SIZE * CELL_SIZE)
#define BOARD_HEIGHT (BOARD_SIZE * CELL_SIZE)
#define SCOREBOARD_WIDTH 600
#define NUM_PLAYERS 4  // Number of players


int counter = 1;
int startGame = 1;
int startTurn = 1;
int endGame = 1;
int seeResult = 1;


int jm1 = 8, jm2 = 6, jm3 = 4;

//int sw0 = 2, sw1 = 2, sw2 = 2, sw3 = 2;

int A[4] = {0}, B[4] = {0};


struct Cat {
    int x;
    int y;
    int attack;
    int defence;
    int allow;
    int score;
    int nm1; // number of mouse1 eaten
    int nm2; // number of mouse2 eaten
    int nm3; // number of mouse3 eaten
    int nf;// number of fish eaten
    char turn;
    int start;
    int number;
};


struct Dog {
    int x;
    int y;
    int attack;
    int defence;
};


struct fish {
    int x;
    int y;
};

struct mouse {
    int x;
    int y;
};

struct meat {
    int x;
    int y;
};

struct Trap {
    int x;
    int y;
};




void drawScoreboard(struct Cat cats[]) {

    // Draw a rectangular table on the right side for scores
    al_draw_filled_rectangle(BOARD_WIDTH, 0, BOARD_WIDTH + SCOREBOARD_WIDTH, BOARD_HEIGHT, al_map_rgb(128, 255, 118));

    // Draw horizontal lines for the table
    al_draw_line(BOARD_WIDTH, 1 * CELL_SIZE, BOARD_WIDTH + SCOREBOARD_WIDTH, 1 * CELL_SIZE, al_map_rgb(255, 0, 0), 3.0);
    for (int i = 2; i <= NUM_PLAYERS + 1; i++) {
        al_draw_line(BOARD_WIDTH, i * CELL_SIZE, BOARD_WIDTH + (SCOREBOARD_WIDTH - SCOREBOARD_WIDTH / 6), i * CELL_SIZE, al_map_rgb(85, 85, 85), 3.0);
    }

    // Draw vertical lines for the table
    al_draw_line(BOARD_WIDTH + SCOREBOARD_WIDTH / 6, 0, BOARD_WIDTH + SCOREBOARD_WIDTH / 6, BOARD_HEIGHT, al_map_rgb(0, 0, 0), 3.0);
    al_draw_line(BOARD_WIDTH + SCOREBOARD_WIDTH * 2 / 6, 0, BOARD_WIDTH + SCOREBOARD_WIDTH * 2 / 6, BOARD_HEIGHT, al_map_rgb(0, 0, 0), 3.0);
    al_draw_line(BOARD_WIDTH + SCOREBOARD_WIDTH * 3 / 6, 0, BOARD_WIDTH + SCOREBOARD_WIDTH * 3 / 6, BOARD_HEIGHT, al_map_rgb(0, 0, 0), 3.0);
    al_draw_line(BOARD_WIDTH + SCOREBOARD_WIDTH * 4 / 6, 0, BOARD_WIDTH + SCOREBOARD_WIDTH * 4 / 6, BOARD_HEIGHT, al_map_rgb(0, 0, 0), 3.0);
    al_draw_line(BOARD_WIDTH + SCOREBOARD_WIDTH * 5 / 6, 0, BOARD_WIDTH + SCOREBOARD_WIDTH * 5 / 6, BOARD_HEIGHT, al_map_rgb(255, 0, 0), 3.0);

    ALLEGRO_FONT *font = al_load_ttf_font("SFNSRounded.ttf", 25, 0);

    // Draw headers
    al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH + SCOREBOARD_WIDTH / 12, CELL_SIZE / 2, ALLEGRO_ALIGN_CENTER, "Cat");
    al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH + SCOREBOARD_WIDTH / 4, CELL_SIZE / 2, ALLEGRO_ALIGN_CENTER, "Attack");
    al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH + SCOREBOARD_WIDTH * 3 / 7.25, CELL_SIZE / 2, ALLEGRO_ALIGN_CENTER, "Defence");
    al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH + SCOREBOARD_WIDTH * 5 / 8.5, CELL_SIZE / 2, ALLEGRO_ALIGN_CENTER, "Score");
    al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH + SCOREBOARD_WIDTH * 7 / 9.25, CELL_SIZE / 2, ALLEGRO_ALIGN_CENTER, "Turn");
    al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH + SCOREBOARD_WIDTH * 7 / 7.75, CELL_SIZE / 2, ALLEGRO_ALIGN_CENTER, "Counter");

    // Draw each player's scores
    for (int i = 0; i < NUM_PLAYERS; i++) {
        al_draw_textf(font, al_map_rgb(0, 0, 0), BOARD_WIDTH + SCOREBOARD_WIDTH / 12, i * CELL_SIZE + CELL_SIZE / 0.75, ALLEGRO_ALIGN_CENTER, "%d", i + 1);
        al_draw_textf(font, al_map_rgb(0, 0, 0), BOARD_WIDTH + SCOREBOARD_WIDTH / 4, i * CELL_SIZE + CELL_SIZE / 0.75, ALLEGRO_ALIGN_CENTER, "%d", cats[i].attack);
        al_draw_textf(font, al_map_rgb(0, 0, 0), BOARD_WIDTH + SCOREBOARD_WIDTH * 3/ 7.25, i * CELL_SIZE + CELL_SIZE / 0.75, ALLEGRO_ALIGN_CENTER, "%d", cats[i].defence);
        al_draw_textf(font, al_map_rgb(0, 0, 0), BOARD_WIDTH + SCOREBOARD_WIDTH * 5 / 8.5, i * CELL_SIZE + CELL_SIZE / 0.75, ALLEGRO_ALIGN_CENTER, "%d", cats[i].score);
        al_draw_textf(font, al_map_rgb(0, 0, 0), BOARD_WIDTH + SCOREBOARD_WIDTH * 7 / 9.25, i * CELL_SIZE + CELL_SIZE / 0.75, ALLEGRO_ALIGN_CENTER, "%c", cats[i].turn);
    }

    al_draw_textf(font, al_map_rgb(0, 0, 0), BOARD_WIDTH + SCOREBOARD_WIDTH * 7 / 7.75, 0 * CELL_SIZE + CELL_SIZE / 0.75, ALLEGRO_ALIGN_CENTER, "%d", counter);

    al_destroy_font(font);
}






// draw dog for first time
void draw_dog(struct Dog dogs[],long long int board[BOARD_SIZE][BOARD_SIZE]){
    int x, y;
    for(int i=0; i<4; i++){
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
        if(board[x][y] == 1 || board[x][y] == 3 || board[x][y] == 5 || board[x][y] == 7 || board[x][y] == 11 || board[x][y] == 13 || board[x][y] == 17){
            if(i==0){ // bulldog
                dogs[0].x = x;
                dogs[0].y = y;
                dogs[0].attack = 5;
                dogs[0].defence = 30;
                board[x][y] *= 19;
            }
            else if(i==1){ // pitbull
                dogs[1].x = x;
                dogs[1].y = y;
                dogs[1].attack = 2;
                dogs[1].defence = 15;
                board[x][y] *= 23;
            }
            else if(i==2){ // shepherd
                dogs[2].x = x;
                dogs[2].y = y;
                dogs[2].attack = 2;
                dogs[2].defence = 20;
                board[x][y] *= 29;
            }
            else if(i==3){ // bulldog Junior
                dogs[3].x = x;
                dogs[3].y = y;
                dogs[3].attack = 1;
                dogs[3].defence = 5;
                board[x][y] *= 31;
            }
        }
        else i--;
    }
}





// draw meat for first time
void draw_meat(struct meat meats[],long long int board[BOARD_SIZE][BOARD_SIZE]){
    int x, y;
    for(int i=0; i<8; i++){
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
        if(board[x][y] == 1 || board[x][y] == 3 || board[x][y] == 5 || board[x][y] == 7 || board[x][y] == 11 || board[x][y] == 13 || board[x][y] == 17){
            meats[i].x = x;
            meats[i].y = y;
            board[x][y] *= 37;
        }
        else i--;
    }
}



// draw trap for first time
void draw_trap(struct Trap traps[],long long int board[BOARD_SIZE][BOARD_SIZE]) {
    int x, y;
    for (int i = 0; i < 8; i++) {
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
        if (board[x][y] == 1 || board[x][y] == 3 || board[x][y] == 5 || board[x][y] == 7 || board[x][y] == 11 || board[x][y] == 13 || board[x][y] == 17) {
            traps[i].x = x;
            traps[i].y = y;
            board[x][y] *= 67;
        }
        else i--;
    }
}



// draw mouse for first time
void draw_mouse1(struct mouse m1[],long long int board[BOARD_SIZE][BOARD_SIZE], int jm1){
    int x, y;
    for(int i=0; i<jm1; i++){
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
        if(board[x][y] == 1 || board[x][y] == 3 || board[x][y] == 5 || board[x][y] == 7 || board[x][y] == 11 || board[x][y] == 13 || board[x][y] == 17){
            m1[i].x = x;
            m1[i].y = y;
            board[x][y] *= 41;
        }
        else i--;
    }
}


void draw_mouse2(struct mouse m2[],long long int board[BOARD_SIZE][BOARD_SIZE], int jm2){
    int x, y;
    for(int i=0; i<jm2; i++){
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
        if(board[x][y] == 1 || board[x][y] == 3 || board[x][y] == 5 || board[x][y] == 7 || board[x][y] == 11 || board[x][y] == 13 || board[x][y] == 17){
            m2[i].x = x;
            m2[i].y = y;
            board[x][y] *= 43;
        }
        else i--;
    }
}


void draw_mouse3(struct mouse m3[],long long int board[BOARD_SIZE][BOARD_SIZE], int jm3){
    int x, y;
    for(int i=0; i<jm3; i++){
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
        if(board[x][y] == 1 || board[x][y] == 3 || board[x][y] == 5 || board[x][y] == 7 || board[x][y] == 11 || board[x][y] == 13 || board[x][y] == 17){
            m3[i].x = x;
            m3[i].y = y;
            board[x][y] *= 47;
        }
        else i--;
    }
}



// draw fish for first time
void draw_fish2(struct fish f2[],long long int board[BOARD_SIZE][BOARD_SIZE], int j){
    int x, y;
    for(int i=0; i<j; i++){
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
        if(board[x][y] == 1 || board[x][y] == 3 || board[x][y] == 5 || board[x][y] == 7 || board[x][y] == 11 || board[x][y] == 13 || board[x][y] == 17){
            f2[i].x = x;
            f2[i].y = y;
            board[x][y] *= 53;
        }
        else i--;
    }
}


void draw_fish3(struct fish f3[],long long int board[BOARD_SIZE][BOARD_SIZE], int j){
    int x, y;
    for(int i=0; i<j; i++){
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
        if(board[x][y] == 1 || board[x][y] == 3 || board[x][y] == 5 || board[x][y] == 7 || board[x][y] == 11 || board[x][y] == 13 || board[x][y] == 17){
            f3[i].x = x;
            f3[i].y = y;
            board[x][y] *= 59;
        }
        else i--;
    }
}


void draw_fish4(struct fish f4[],long long int board[BOARD_SIZE][BOARD_SIZE], int j){
    int x, y;
    for(int i=0; i<j; i++){
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
        if(board[x][y] == 1 || board[x][y] == 3 || board[x][y] == 5 || board[x][y] == 7 || board[x][y] == 11 || board[x][y] == 13 || board[x][y] == 17){
            f4[i].x = x;
            f4[i].y = y;
            board[x][y] *= 61;
        }
        else i--;
    }
}




// draw dog for ever time
void draw_scale_bitmap_bulldog(ALLEGRO_BITMAP *bitmap, struct Dog a){
    int originalWidth = al_get_bitmap_width(bitmap);
    int originalHeight = al_get_bitmap_height(bitmap);
    int newWidth = CELL_SIZE;
    int newHeight = CELL_SIZE;

    int x = a.x *CELL_SIZE;
    int y = a.y *CELL_SIZE;

    al_draw_scaled_bitmap(bitmap, 0, 0, originalWidth, originalHeight, x, y, newWidth, newHeight, 0);
}


void draw_scale_bitmap_pitbull(ALLEGRO_BITMAP *bitmap, struct Dog a){
    int originalWidth = al_get_bitmap_width(bitmap);
    int originalHeight = al_get_bitmap_height(bitmap);
    int newWidth = CELL_SIZE;
    int newHeight = CELL_SIZE;


    int x = a.x *CELL_SIZE;
    int y = a.y *CELL_SIZE;


    al_draw_scaled_bitmap(bitmap, 0, 0, originalWidth, originalHeight, x, y, newWidth, newHeight, 0);
}


void draw_scale_bitmap_shepherd(ALLEGRO_BITMAP *bitmap, struct Dog a){
    int originalWidth = al_get_bitmap_width(bitmap);
    int originalHeight = al_get_bitmap_height(bitmap);
    int newWidth = CELL_SIZE;
    int newHeight = CELL_SIZE;


    int x = a.x *CELL_SIZE;
    int y = a.y *CELL_SIZE;


    al_draw_scaled_bitmap(bitmap, 0, 0, originalWidth, originalHeight, x, y, newWidth, newHeight, 0);
}


void draw_scale_bitmap_bulldog_J(ALLEGRO_BITMAP *bitmap, struct Dog a){
    int originalWidth = al_get_bitmap_width(bitmap);
    int originalHeight = al_get_bitmap_height(bitmap);
    int newWidth = CELL_SIZE;
    int newHeight = CELL_SIZE;


    int x = a.x *CELL_SIZE;
    int y = a.y *CELL_SIZE;


    al_draw_scaled_bitmap(bitmap, 0, 0, originalWidth, originalHeight, x, y, newWidth, newHeight, 0);
}



// draw meat for ever time

void draw_scale_bitmap_meat(ALLEGRO_BITMAP *bitmap, struct meat a){
    int originalWidth = al_get_bitmap_width(bitmap);
    int originalHeight = al_get_bitmap_height(bitmap);
    int newWidth = CELL_SIZE;
    int newHeight = CELL_SIZE;

    int x = a.x *CELL_SIZE;
    int y = a.y *CELL_SIZE;

    al_draw_scaled_bitmap(bitmap, 0, 0, originalWidth, originalHeight, x, y, newWidth, newHeight, 0);
}


// draw traps for ever time

void draw_scale_bitmap_trap(ALLEGRO_BITMAP *bitmap, struct Trap a){
    int originalWidth = al_get_bitmap_width(bitmap);
    int originalHeight = al_get_bitmap_height(bitmap);
    int newWidth = CELL_SIZE;
    int newHeight = CELL_SIZE;

    int x = a.x *CELL_SIZE;
    int y = a.y *CELL_SIZE;

    al_draw_scaled_bitmap(bitmap, 0, 0, originalWidth, originalHeight, x, y, newWidth, newHeight, 0);
}



// draw fish for ever time

void draw_scale_bitmap_fish(ALLEGRO_BITMAP *bitmap, struct fish a){
    int originalWidth = al_get_bitmap_width(bitmap);
    int originalHeight = al_get_bitmap_height(bitmap);
    int newWidth = CELL_SIZE;
    int newHeight = CELL_SIZE;

    int x = a.x *CELL_SIZE;
    int y = a.y *CELL_SIZE;

    al_draw_scaled_bitmap(bitmap, 0, 0, originalWidth, originalHeight, x, y, newWidth, newHeight, 0);
}



// draw mouse for ever time

void draw_scale_bitmap_mouse(ALLEGRO_BITMAP *bitmap,  struct mouse a){
    int originalWidth = al_get_bitmap_width(bitmap);
    int originalHeight = al_get_bitmap_height(bitmap);
    int newWidth = CELL_SIZE;
    int newHeight = CELL_SIZE;

    int x = a.x *CELL_SIZE;
    int y = a.y *CELL_SIZE;

    al_draw_scaled_bitmap(bitmap, 0, 0, originalWidth, originalHeight, x, y, newWidth, newHeight, 0);
}



// for cats (players)

void draw_scale_bitmap_cat1(ALLEGRO_BITMAP *bitmap, struct Cat a){
    int originalWidth = al_get_bitmap_width(bitmap);
    int originalHeight = al_get_bitmap_height(bitmap);
    int newWidth = CELL_SIZE;
    int newHeight = CELL_SIZE;

    int x = a.x *CELL_SIZE;
    int y = a.y *CELL_SIZE;

    al_draw_scaled_bitmap(bitmap, 0, 0, originalWidth, originalHeight, x, y, newWidth, newHeight, 0);
}

void draw_scale_bitmap_cat2(ALLEGRO_BITMAP *bitmap, struct Cat a){
    int originalWidth = al_get_bitmap_width(bitmap);
    int originalHeight = al_get_bitmap_height(bitmap);
    int newWidth = CELL_SIZE;
    int newHeight = CELL_SIZE;

    int x = a.x *CELL_SIZE;
    int y = a.y *CELL_SIZE;

    al_draw_scaled_bitmap(bitmap, 0, 0, originalWidth, originalHeight, x, y, newWidth, newHeight, 0);
}

void draw_scale_bitmap_cat3(ALLEGRO_BITMAP *bitmap, struct Cat a){
    int originalWidth = al_get_bitmap_width(bitmap);
    int originalHeight = al_get_bitmap_height(bitmap);
    int newWidth = CELL_SIZE;
    int newHeight = CELL_SIZE;

    int x = a.x *CELL_SIZE;
    int y = a.y *CELL_SIZE;

    al_draw_scaled_bitmap(bitmap, 0, 0, originalWidth, originalHeight, x, y, newWidth, newHeight, 0);
}

void draw_scale_bitmap_cat4(ALLEGRO_BITMAP *bitmap, struct Cat a){
    int originalWidth = al_get_bitmap_width(bitmap);
    int originalHeight = al_get_bitmap_height(bitmap);
    int newWidth = CELL_SIZE;
    int newHeight = CELL_SIZE;

    int x = a.x *CELL_SIZE;
    int y = a.y *CELL_SIZE;

    al_draw_scaled_bitmap(bitmap, 0, 0, originalWidth, originalHeight, x, y, newWidth, newHeight, 0);
}






void move_bulldog(struct Dog dogs[],long long int board[BOARD_SIZE][BOARD_SIZE]) {

    int j = rand() % 4;

    if(j == 0){ // go up

        if(dogs[0].y>4 && 255255 % board[dogs[0].x][dogs[0].y - 5] == 0){
            board[dogs[0].x][dogs[0].y - 5] *= 19;
            board[dogs[0].x][dogs[0].y] /= 19;
            dogs[0].y -= 5;

        }
    }
    else if(j == 1){ // go down

        if(dogs[0].y<10 && 255255 % board[dogs[0].x][dogs[0].y + 5] == 0){
            board[dogs[0].x][dogs[0].y + 5] *= 19;
            board[dogs[0].x][dogs[0].y] /= 19;
            dogs[0].y += 5;

        }
    }
    else if(j == 2){ // go right

        if(dogs[0].x<10 && 255255 % board[dogs[0].x + 5][dogs[0].y] == 0){
            board[dogs[0].x + 5][dogs[0].y] *= 19;
            board[dogs[0].x][dogs[0].y] /= 19;
            dogs[0].x += 5;

        }
    }
    else if(j == 3){ // go left

        if(dogs[0].x>4 && 255255 % board[dogs[0].x - 5][dogs[0].y] == 0){
            board[dogs[0].x - 5][dogs[0].y] *= 19;
            board[dogs[0].x][dogs[0].y] /= 19;
            dogs[0].x -= 5;

        }
    }
}


void move_pitbull(struct Dog dogs[],long long int board[BOARD_SIZE][BOARD_SIZE]) {

    int j = rand() % 4;

    if(j == 0){ // go up

        if(dogs[1].y>1 && 255255 % board[dogs[1].x][dogs[1].y - 2] == 0){
            board[dogs[1].x][dogs[1].y - 2] *= 23;
            board[dogs[1].x][dogs[1].y] /= 23;
            dogs[1].y -= 2;

        }
    }
    else if(j == 1){ // go down

        if(dogs[1].y<13 && 255255 % board[dogs[1].x][dogs[1].y + 2] == 0){
            board[dogs[1].x][dogs[1].y + 2] *= 23;
            board[dogs[1].x][dogs[1].y] /= 23;
            dogs[1].y += 2;

        }
    }
    else if(j == 2){ // go right

        if(dogs[1].x<13 && 255255 % board[dogs[1].x + 2][dogs[1].y] == 0){
            board[dogs[1].x + 2][dogs[1].y] *= 23;
            board[dogs[1].x][dogs[1].y] /= 23;
            dogs[1].x += 2;

        }
    }
    else if(j == 3){ // go left

        if(dogs[1].x>1 && 255255 % board[dogs[1].x - 2][dogs[1].y] == 0){
            board[dogs[1].x - 2][dogs[1].y] *= 23;
            board[dogs[1].x][dogs[1].y] /= 23;
            dogs[1].x -= 2;

        }
    }
}



void move_shepherd(struct Dog dogs[],long long int board[BOARD_SIZE][BOARD_SIZE]) {

    int j = rand() % 4;

    if(j == 0){ // go up

        if(dogs[2].y>2 && 255255 % board[dogs[2].x][dogs[2].y - 3] == 0){
            board[dogs[2].x][dogs[2].y - 3] *= 29;
            board[dogs[2].x][dogs[2].y] /= 29;
            dogs[2].y -= 3;

        }
    }
    else if(j == 1){ // go down

        if(dogs[2].y<12 && 255255 % board[dogs[2].x][dogs[2].y + 3] == 0){
            board[dogs[2].x][dogs[2].y + 3] *= 29;
            board[dogs[2].x][dogs[2].y] /= 29;
            dogs[2].y += 3;

        }
    }
    else if(j == 2){ // go right

        if(dogs[2].x<12 && 255255 % board[dogs[2].x + 3][dogs[2].y] == 0){
            board[dogs[2].x + 3][dogs[2].y] *= 29;
            board[dogs[2].x][dogs[2].y] /= 29;
            dogs[2].x += 3;

        }
    }
    else if(j == 3){ // go left

        if(dogs[2].x>2 && 255255 % board[dogs[2].x - 3][dogs[2].y] == 0){
            board[dogs[2].x - 3][dogs[2].y] *= 29;
            board[dogs[2].x][dogs[2].y] /= 29;
            dogs[2].x -= 3;

        }
    }
}



void move_bulldog_J(struct Dog dogs[], long long int board[BOARD_SIZE][BOARD_SIZE]) {

    int j = rand() % 4;

    if(j == 0){ // go up

        if(dogs[3].y>0 && 255255 % board[dogs[3].x][dogs[3].y - 1] == 0){
            board[dogs[3].x][dogs[3].y - 1] *= board[dogs[3].x][dogs[3].y];
            board[dogs[3].x][dogs[3].y] /= 31;
            dogs[3].y -= 1;

        }
    }
    else if(j == 1){ // go down

        if(dogs[3].y<14 && 255255 % board[dogs[3].x][dogs[3].y + 1] == 0){
            board[dogs[3].x][dogs[3].y + 1] *= board[dogs[3].x][dogs[3].y];
            board[dogs[3].x][dogs[3].y] /= 31;
            dogs[3].y += 1;

        }
    }
    else if(j == 2){ // go right

        if(dogs[3].x<14 && 255255 % board[dogs[3].x + 1][dogs[3].y] == 0){
            board[dogs[3].x + 1][dogs[3].y] *= board[dogs[3].x][dogs[3].y];
            board[dogs[3].x][dogs[3].y] /= 31;
            dogs[3].x += 1;


        }
    }
    else if(j == 3){ // go left

        if(dogs[3].x>0 && 255255 % board[dogs[3].x - 1][dogs[3].y] == 0){
            board[dogs[3].x - 1][dogs[3].y] *= board[dogs[3].x][dogs[3].y];
            board[dogs[3].x][dogs[3].y] /= 31;
            dogs[3].x -= 1;


        }
    }
}



void move_mouse1(struct mouse m[],long long int board[BOARD_SIZE][BOARD_SIZE]) {

    for(int i=0; i<8; i++) {
        int j = rand() % 8;

        if(j == 0) { // go up


            if(m[i].y>0 && (255255 % board[m[i].x][m[i].y - 1] == 0)) {
                board[m[i].x][m[i].y - 1] *= 41;
                board[m[i].x][m[i].y] /= 41;
                m[i].y -= 1;

            }
        }
        else if(j == 1) { // go down


            if(m[i].y<14 && (255255 % board[m[i].x][m[i].y + 1] == 0)){
                board[m[i].x][m[i].y + 1] *= 41;
                board[m[i].x][m[i].y] /= 41;
                m[i].y += 1;

            }
        }
        else if(j == 2) { // go right

            if(m[i].x<14 && (255255 % board[m[i].x + 1][m[i].y] == 0)) {
                board[m[i].x + 1][m[i].y] *= 41;
                board[m[i].x][m[i].y] /= 41;
                m[i].x += 1;

            }

        }
        else if(j == 3) { // go left



            if(m[i].x>0 && (255255 % board[m[i].x - 1][m[i].y] == 0)) {
                board[m[i].x - 1][m[i].y] *= 41;
                board[m[i].x][m[i].y] /= 41;
                m[i].x -= 1;

            }
        }
        else if(j == 4) { // go up and left


            if(m[i].x>0 && m[i].y>0 && (255255 % board[m[i].x - 1][m[i].y - 1] == 0)) {
                board[m[i].x - 1][m[i].y - 1] *= 41;
                board[m[i].x][m[i].y] /= 41;
                m[i].x -= 1;
                m[i].y -= 1;


            }
        }
        else if(j == 5) { // go up and right


            if(m[i].x<14 && m[i].y>0 && (255255 % board[m[i].x + 1][m[i].y - 1] == 0)) {
                board[m[i].x + 1][m[i].y - 1] *= 41;
                board[m[i].x][m[i].y] /= 41;
                m[i].x += 1;
                m[i].y -= 1;

            }

        }
        else if(j == 6) { // go down and right


            if(m[i].x<14 && m[i].y<14 && (255255 % board[m[i].x + 1][m[i].y + 1] == 0)) {
                board[m[i].x + 1][m[i].y + 1] *= 41;
                board[m[i].x][m[i].y] /= 41;
                m[i].x += 1;
                m[i].y += 1;

            }

        }
        else if(j == 7) { // go down and left



            if(m[i].x>0 && m[i].y<14 && (255255 % board[m[i].x - 1][m[i].y + 1] == 0)) {
                board[m[i].x - 1][m[i].y + 1] *= 41;
                board[m[i].x][m[i].y] /= 41;
                m[i].x -= 1;
                m[i].y += 1;

            }
        }
        //else i--;

    }

}







void move_mouse2(struct mouse m[],long long int board[BOARD_SIZE][BOARD_SIZE]) {

    for(int i=0; i<6; i++) {
        int j = rand() % 8;

        if(j == 0) { // go up


            if(m[i].y>1 && 255255 % board[m[i].x][m[i].y - 2] == 0) {
                board[m[i].x][m[i].y - 2] *= 43;
                board[m[i].x][m[i].y] /= 43;
                m[i].y -= 2;


            }
        }
        else if(j == 1) { // go down


            if(m[i].y<13 && 255255 % board[m[i].x][m[i].y + 2] == 0){

                board[m[i].x][m[i].y + 2] *= 43;
                board[m[i].x][m[i].y] /= 43;
                m[i].y += 2;

            }
        }
        else if(j == 2) { // go right

            if(m[i].x<13 && 255255 % board[m[i].x + 2][m[i].y] == 0) {
                board[m[i].x + 2][m[i].y] *= 43;
                board[m[i].x][m[i].y] /= 43;
                m[i].x += 2;

            }

        }
        else if(j == 3) { // go left



            if(m[i].x>1 && 255255 % board[m[i].x - 2][m[i].y] == 0) {
                board[m[i].x - 2][m[i].y] *= 43;
                board[m[i].x][m[i].y] /= 43;
                m[i].x -= 2;

            }
        }
        else if(j == 4) { // go up and left


            if(m[i].x>1 && m[i].y>1 && 255255 % board[m[i].x - 2][m[i].y - 2] == 0) {
                board[m[i].x - 2][m[i].y - 2] *= 43;
                board[m[i].x][m[i].y] /= 43;
                m[i].x -= 2;
                m[i].y -= 2;


            }
        }
        else if(j == 5) { // go up and right


            if(m[i].x<13 && m[i].y>1 && 255255 % board[m[i].x + 2][m[i].y - 2] == 0) {
                board[m[i].x + 2][m[i].y - 2] *= 43;
                board[m[i].x][m[i].y] /= 43;
                m[i].x += 2;
                m[i].y -= 2;

            }

        }
        else if(j == 6) { // go down and right


            if(m[i].x<13 && m[i].y<13 && 255255 % board[m[i].x + 2][m[i].y + 2] == 0) {
                board[m[i].x + 2][m[i].y + 2] *= 43;
                board[m[i].x][m[i].y] /= 43;
                m[i].x += 2;
                m[i].y += 2;

            }

        }
        else if(j == 7) { // go down and left



            if(m[i].x>1 && m[i].y<13 && 255255 % board[m[i].x - 2][m[i].y + 2] == 0) {
                board[m[i].x - 2][m[i].y + 2] *= 43;
                board[m[i].x][m[i].y] /= 43;
                m[i].x -= 2;
                m[i].y += 2;
            }
        }
        //else i--;

    }

}




void move_mouse3(struct mouse m[], long long int board[BOARD_SIZE][BOARD_SIZE]) {

    for(int i=0; i<4; i++) {
        int j = rand() % 8;

        if(j == 0) { // go up


            if(m[i].y>2 && 255255 % board[m[i].x][m[i].y - 3] == 0) {
                board[m[i].x][m[i].y - 3] *= 47;
                board[m[i].x][m[i].y] /= 47;
                m[i].y -= 3;


            }
        }
        else if(j == 1) { // go down


            if(m[i].y<12 && 255255 % board[m[i].x][m[i].y + 3] == 0){
                board[m[i].x][m[i].y + 3] *= 47;
                board[m[i].x][m[i].y] /= 47;
                m[i].y += 3;

            }
        }
        else if(j == 2) { // go right

            if(m[i].x<12 && 255255 % board[m[i].x + 3][m[i].y] == 0) {
                board[m[i].x + 3][m[i].y] *= 47;
                board[m[i].x][m[i].y] /= 47;
                m[i].x += 3;

            }

        }
        else if(j == 3) { // go left



            if(m[i].x>2 && 255255 % board[m[i].x - 3][m[i].y] == 0) {
                board[m[i].x - 3][m[i].y] *= 47;
                board[m[i].x][m[i].y] /= 47;
                m[i].x -= 3;

            }
        }
        else if(j == 4) { // go up and left


            if(m[i].x>2 && m[i].y>2 && 255255 % board[m[i].x - 3][m[i].y - 3] == 0) {
                board[m[i].x - 3][m[i].y - 3] *= 47;
                board[m[i].x][m[i].y] /= 47;
                m[i].x -= 3;
                m[i].y -= 3;


            }
        }
        else if(j == 5) { // go up and right


            if(m[i].x<12 && m[i].y>2 && 255255 % board[m[i].x + 3][m[i].y - 3] == 0) {
                board[m[i].x + 3][m[i].y - 3] *= 47;
                board[m[i].x][m[i].y] /= 47;
                m[i].x += 3;
                m[i].y -= 3;

            }

        }
        else if(j == 6) { // go down and right


            if(m[i].x<12 && m[i].y<12 && 255255 % board[m[i].x + 3][m[i].y + 3] == 0) {
                board[m[i].x + 3][m[i].y + 3] *= 47;
                board[m[i].x][m[i].y] /= 47;
                m[i].x += 3;
                m[i].y += 3;

            }

        }
        else if(j == 7) { // go down and left



            if(m[i].x>2 && m[i].y<12 && 255255 % board[m[i].x - 3][m[i].y + 3] == 0) {
                board[m[i].x - 3][m[i].y + 3] *= 47;
                board[m[i].x][m[i].y] /= 47;
                m[i].x -= 3;
                m[i].y += 3;

            }
        }
        //else i--;

    }

}



int fight_between_cats(struct Cat a1, struct Cat a2) {
    int rec1, rec2;

    rec1 = a1.defence / a2.attack;
    rec2 = a2.defence / a1.attack;

    if(rec1 > rec2)
        return 1;
    else if(rec1 < rec2)
        return 2;

}


int fight_between_cats_dogs(struct Cat a, struct Dog b) {
    int rec1, rec2;

    rec1 = a.defence / b.attack;
    rec2 = b.defence / a.attack;

    if(rec1 > rec2)
        return 1;
    else if(rec1 < rec2)
        return 2;

}



int Dice() {
    int x = (rand() % 6) + 1;
    return x;
}



void swap(struct Cat *a, struct Cat *b) {
    struct Cat temp = *a;
    *a = *b;
    *b = temp;
}

void selectionSort(struct Cat cats[], int n, int B[]) {
    for (int i = 0; i < n; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (cats[j].score > cats[maxIndex].score ||
                (cats[j].score == cats[maxIndex].score && cats[j].attack > cats[maxIndex].attack) ||
                (cats[j].score == cats[maxIndex].score && cats[j].attack == cats[maxIndex].attack && cats[j].defence > cats[maxIndex].defence)) {
                maxIndex = j;
            }
        }
        swap(&cats[maxIndex], &cats[i]);
        B[i] = cats[i].number;
    }
}






void drawBoard(struct Cat cats[]) {
    for (int i = 0; i <= BOARD_SIZE; ++i) {
        al_draw_line(0, i * CELL_SIZE, BOARD_WIDTH, i * CELL_SIZE, al_map_rgb(255, 0, 0), 3.0);
    }
    for (int i = 0; i <= BOARD_SIZE; ++i) {
        al_draw_line(i * CELL_SIZE, 0, i * CELL_SIZE, BOARD_HEIGHT, al_map_rgb(255, 0, 0), 3.0);
    }

    al_draw_line(4 * CELL_SIZE, 4 * CELL_SIZE, 4 * CELL_SIZE, 6 * CELL_SIZE, al_map_rgb(128, 64, 0), 14.0);
    al_draw_line(5 * CELL_SIZE, 12 * CELL_SIZE, 10 * CELL_SIZE, 12 * CELL_SIZE, al_map_rgb(128, 64, 0), 14.0);
    al_draw_line(2 * CELL_SIZE, 9 * CELL_SIZE, 2 * CELL_SIZE, 11 * CELL_SIZE, al_map_rgb(128, 64, 0), 14.0);
    al_draw_line(12 * CELL_SIZE, 5 * CELL_SIZE, 12 * CELL_SIZE, 8 * CELL_SIZE, al_map_rgb(128, 64, 0), 14.0);
    al_draw_line(10 * CELL_SIZE, 8 * CELL_SIZE, 12 * CELL_SIZE, 8 * CELL_SIZE, al_map_rgb(128, 64, 0), 14.0);
    al_draw_line(11 * CELL_SIZE, 1 * CELL_SIZE, 11 * CELL_SIZE, 2 * CELL_SIZE, al_map_rgb(128, 64, 0), 14.0);
    al_draw_line(3 * CELL_SIZE, 4 * CELL_SIZE, 4 * CELL_SIZE, 4 * CELL_SIZE, al_map_rgb(128, 64, 0), 14.0);
    al_draw_line(4 * CELL_SIZE, 2 * CELL_SIZE, 6 * CELL_SIZE, 2 * CELL_SIZE, al_map_rgb(128, 64, 0), 14.0);
    al_draw_line(13 * CELL_SIZE, 10 * CELL_SIZE, 13 * CELL_SIZE, 11 * CELL_SIZE, al_map_rgb(128, 64, 0), 14.0);



    drawScoreboard(cats);  // Draw the scoreboard
}




int main() {
    long long int board[BOARD_SIZE][BOARD_SIZE];

    for(int i=0; i<BOARD_SIZE; i++) {
        for(int j=0; j<BOARD_SIZE; j++){
            board[i][j] = 1;
        }
    }


    ALLEGRO_DISPLAY *display = NULL;
    srand(time(NULL));



    if (!al_init()) {
        printf("Failed to initialize Allegro!\n");
        return 0;
    }




    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();




    al_install_keyboard();
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    ALLEGRO_EVENT ev;


    display = al_create_display(BOARD_WIDTH + SCOREBOARD_WIDTH, BOARD_HEIGHT);

    if (!display) {
        printf("Failed to create display!\n");
        return 0;
    }


    // for walls
    // does not can to down
    board[4][1] *= 3;
    board[5][1] *= 3;
    board[3][3] *= 3;
    board[10][7] *= 3;
    board[11][7] *= 3;
    board[5][11] *= 3;
    board[6][11] *= 3;
    board[7][11] *= 3;
    board[8][11] *= 3;
    board[9][11] *= 3;

    // does not can to up
    board[4][2] *= 5;
    board[5][2] *= 5;
    board[10][8] *= 5;
    board[11][8] *= 5;
    board[5][12] *= 5;
    board[6][12] *= 5;
    board[7][12] *= 5;
    board[8][12] *= 5;
    board[9][12] *= 5;

    // does not can to right
    board[10][1] *= 7;
    board[3][5] *= 7;
    board[8][4] *= 7;
    board[11][5] *= 7;
    board[11][6] *= 7;
    board[1][9] *= 7;
    board[1][10] *= 7;
    board[12][10] *= 7;

    // does not can to left
    board[4][4] *= 11;
    board[4][5] *= 11;
    board[2][9] *= 11;
    board[2][10] *= 11;
    board[11][1] *= 11;
    board[12][5] *= 11;
    board[12][6] *= 11;
    board[12][7] *= 11;
    board[13][10] *= 11;

    // does not can to up and right
    board[3][4] *= 13;

    // does not can to down and right
    board[11][7] *= 17;


    // have bug
    board[8][4] = 1;


    // for home
    board[7][7] *= 2;

    struct Trap traps[8];
    draw_trap(traps, board);

    struct Dog dogs[4];
    draw_dog(dogs, board);



    struct mouse m1[8];
    struct mouse m2[6];
    struct mouse m3[4];
    draw_mouse1(m1, board, jm1);
    draw_mouse2(m2, board, jm2);
    draw_mouse3(m3, board, jm3);


    struct meat meats[8];
    draw_meat(meats, board);




    struct fish f2[4];
    struct fish f3[4];
    struct fish f4[2];
    draw_fish2(f2, board, 4);
    draw_fish3(f3, board, 4);
    draw_fish4(f4, board, 2);


    struct Cat cats[4];
    for(int i=0; i<4; i++){
        cats[i].x = 7;
        cats[i].y = 7;
        cats[i].defence = 5;
        cats[i].attack = 2;
        cats[i].allow = 3;
        cats[i].score = 0;
        cats[i].nm1 = 0;
        cats[i].nm2 = 0;
        cats[i].nm3 = 0;
        cats[i].nf = 0;
        cats[i].number = i + 1;
    }




    al_clear_to_color(al_map_rgb(173, 121, 255));
    drawBoard(cats);



    // import all of the elements image in source code
    ALLEGRO_BITMAP *home = al_load_bitmap("home.png");
    ALLEGRO_BITMAP *bulldog = al_load_bitmap("bulldog.png");
    ALLEGRO_BITMAP *shepherd = al_load_bitmap("shepherd.png");
    ALLEGRO_BITMAP *pitbull = al_load_bitmap("pitbull.png");
    ALLEGRO_BITMAP *bulldog_J = al_load_bitmap("bulldog_J.png");
    ALLEGRO_BITMAP *mouse1 = al_load_bitmap("mouse1.png");
    ALLEGRO_BITMAP *mouse2 = al_load_bitmap("mouse2.png");
    ALLEGRO_BITMAP *mouse3 = al_load_bitmap("mouse3.png");
    ALLEGRO_BITMAP *fish = al_load_bitmap("fish.png");
    ALLEGRO_BITMAP *meat = al_load_bitmap("meat.png");
    ALLEGRO_BITMAP *cat1 = al_load_bitmap("Cat1.png");
    ALLEGRO_BITMAP *cat2 = al_load_bitmap("Cat2.png");
    ALLEGRO_BITMAP *cat3 = al_load_bitmap("Cat3.png");
    ALLEGRO_BITMAP *cat4 = al_load_bitmap("Cat4.png");
    ALLEGRO_BITMAP *background = al_load_bitmap("background.jpg");
    ALLEGRO_BITMAP *finish = al_load_bitmap("finish.jpeg");
    ALLEGRO_BITMAP *trap = al_load_bitmap("trap.png");



    ALLEGRO_FONT *font = al_load_ttf_font("PinyonScript-Regular.ttf", 90, 0);


    while (startGame == 1) {

        int originalWidth_S = al_get_bitmap_width(background);
        int originalHeight_S = al_get_bitmap_height(background);
        int newWidth_S = BOARD_WIDTH + SCOREBOARD_WIDTH;
        int newHeight_S = BOARD_HEIGHT;
        int SX = 0;
        int SY = 0;
        al_draw_scaled_bitmap(background, 0, 0, originalWidth_S, originalHeight_S, SX, SY, newWidth_S, newHeight_S, 0);


        al_draw_text(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 2, BOARD_HEIGHT / 40, ALLEGRO_ALIGN_CENTER, "Welcome to Mr. Clobber’s backyard");
        al_draw_text(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 2.25, BOARD_HEIGHT / 10, ALLEGRO_ALIGN_CENTER, "Please enter 's' for start game");
        //al_draw_text(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 4.25, BOARD_HEIGHT / 4, ALLEGRO_ALIGN_CENTER, "Ali Akhavan");
        al_draw_text(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 2.15, BOARD_HEIGHT * 3 / 3.35, ALLEGRO_ALIGN_CENTER, "Please enter 'escape' for exit game");
        al_flip_display();





        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_S:
                    startGame = 0;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    return -1;
            }
        }
    }

    while (startTurn == 1) {

        for(int i=0; i<4; i++) {
            int x = Dice();
            if(x != A[0] && x != A[1] && x != A[2])
                A[i] = x;
            else
                i--;
        }

        cats[0].start = A[0];
        cats[1].start = A[1];
        cats[2].start = A[2];
        cats[3].start = A[3];

        al_clear_to_color(al_map_rgb(173, 121, 255));

        al_draw_text(font, al_map_rgb(80, 18, 18), BOARD_WIDTH / 1.35, BOARD_HEIGHT / 40, ALLEGRO_ALIGN_CENTER, "Ali Akhavan");


        al_draw_text(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 2, BOARD_HEIGHT / 5, ALLEGRO_ALIGN_CENTER, "first playe for start: ");
        al_draw_text(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 2, BOARD_HEIGHT / 3.5, ALLEGRO_ALIGN_CENTER, "second playe for start: ");
        al_draw_text(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 2, BOARD_HEIGHT / 2.5, ALLEGRO_ALIGN_CENTER, "thrid playe for start: ");
        al_draw_text(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 2, BOARD_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "fourth playe for start: ");


        al_draw_textf(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 1, BOARD_HEIGHT / 5, ALLEGRO_ALIGN_CENTER, "  %d", cats[0].start);
        al_draw_textf(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 1, BOARD_HEIGHT / 3.5, ALLEGRO_ALIGN_CENTER, "  %d", cats[1].start);
        al_draw_textf(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 1, BOARD_HEIGHT / 2.5, ALLEGRO_ALIGN_CENTER, "  %d", cats[2].start);
        al_draw_textf(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 1, BOARD_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "%d", cats[3].start);

        al_draw_text(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 1.75, BOARD_HEIGHT / 1.45, ALLEGRO_ALIGN_CENTER, "for start game please enter 'S'");
        al_draw_text(font, al_map_rgb(199, 21, 133), BOARD_WIDTH / 1.5, BOARD_HEIGHT / 1.25, ALLEGRO_ALIGN_CENTER, "for exit from game please enter 'escape'");


        al_flip_display();





        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_S:
                    startTurn = 0;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    return -1;
            }
        }
    }







    while(counter <= 15){


        al_wait_for_event(event_queue, &ev);



        if(ev.type == ALLEGRO_EVENT_KEY_DOWN && cats[0].allow>0 && (cats[0].start > cats[1].start) && (cats[0].start > cats[2].start) && (cats[0].start > cats[3].start)){
            cats[0].turn = '*';
            cats[1].turn = ' ';
            cats[2].turn = ' ';
            cats[3].turn = ' ';


            switch(ev.keyboard.keycode) {

                case ALLEGRO_KEY_ENTER:

                    cats[0].allow = 0;
                    break;


                case ALLEGRO_KEY_ESCAPE:
                    return -1;
                    break;






                case ALLEGRO_KEY_UP:
                    if(cats[0].y > 0 && board[cats[0].x][cats[0].y] % 5 != 0 && board[cats[0].x][cats[0].y] % 13 != 0){
                        if(board[cats[0].x][cats[0].y - 1] % 37 == 0){
                            cats[0].attack++;
                            board[cats[0].x][cats[0].y - 1] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[0].x][cats[0].y - 1]) {
                                i++;
                            }
                            meats[i].x = -13;
                            meats[i].y = -22;

                        }
                        else if(board[cats[0].x][cats[0].y - 1] % 41 == 0){
                            cats[0].score++;
                            board[cats[0].x][cats[0].y - 1] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[0].x][cats[0].y - 1]) {
                                i++;
                            }
                            m1[i].x = -2;
                            m1[i].y = -2;
                            cats[0].nm1++;

                        }
                        else if(board[cats[0].x][cats[0].y - 1] % 43 == 0){
                            cats[0].score += 2;
                            board[cats[0].x][cats[0].y - 1] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[0].x][cats[0].y - 1]) {
                                i++;
                            }
                            m2[i].x = -3;
                            m2[i].y = -3;
                            cats[0].nm2++;

                        }
                        else if(board[cats[0].x][cats[0].y - 1] % 47 == 0){
                            cats[0].score += 3;
                            board[cats[0].x][cats[0].y - 1] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[0].x][cats[0].y - 1]) {
                                i++;
                            }
                            m3[i].x = -4;
                            m3[i].y = -4;
                            cats[0].nm3++;

                        }
                        else if(board[cats[0].x][cats[0].y - 1] % 53 == 0){
                            cats[0].defence += 2;
                            board[cats[0].x][cats[0].y - 1] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[0].x][cats[0].y - 1]) {
                                i++;
                            }
                            f2[i].x = -54;
                            f2[i].y = -93;
                            cats[0].nf++;

                        }
                        else if(board[cats[0].x][cats[0].y - 1] % 59 == 0){
                            cats[0].defence += 3;
                            board[cats[0].x][cats[0].y - 1] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[0].x][cats[0].y - 1]) {
                                i++;
                            }
                            f3[i].x = -43;
                            f3[i].y = -85;
                            cats[0].nf++;

                        }
                        else if(board[cats[0].x][cats[0].y - 1] % 61 == 0){
                            cats[0].defence += 4;
                            board[cats[0].x][cats[0].y - 1] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[0].x][cats[0].y - 1]) {
                                i++;
                            }
                            f4[i].x = -34;
                            f4[i].y = -25;
                            cats[0].nf++;
                        }
                        else if(board[cats[0].x][cats[0].y - 1] % 67 == 0){
                            if(cats[0].nm3 > 0) {
                                cats[0].nm3--;
                                cats[0].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[0].nm2 > 0) {
                                cats[0].nm2--;
                                cats[0].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[0].nm1 > 0) {
                                cats[0].nm1--;
                                cats[0].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[0].attack > 1) {
                                cats[0].attack -= 2;
                            }
                            else if(cats[0].defence > 2) {
                                cats[0].defence -= 3;
                            }
                            cats[0].allow = 1;
                        }
                        else if(cats[0].x == cats[1].x && (cats[0].y - 1) == cats[1].y) {
                            if (fight_between_cats(cats[0], cats[1]) == 1) {

                                cats[0].score += cats[1].score;
                                cats[0].nm1 += cats[1].nm1;
                                cats[0].nm2 += cats[1].nm2;
                                cats[0].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[0].defence -= (cats[1].attack * (cats[1].defence / cats[0].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[1]) == 2) {

                                cats[1].score += cats[0].score;
                                cats[1].nm1 += cats[0].nm1;
                                cats[1].nm2 += cats[0].nm2;
                                cats[1].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[1].defence -= (cats[0].attack * (cats[0].defence / cats[1].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(cats[0].x == cats[2].x && (cats[0].y - 1) == cats[2].y) {
                            if (fight_between_cats(cats[0], cats[2]) == 1) {

                                cats[0].score += cats[2].score;
                                cats[0].nm1 += cats[2].nm1;
                                cats[0].nm2 += cats[2].nm2;
                                cats[0].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[0].defence -= (cats[2].attack * (cats[2].defence / cats[0].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[2]) == 2) {

                                cats[2].score += cats[0].score;
                                cats[2].nm1 += cats[0].nm1;
                                cats[2].nm2 += cats[0].nm2;
                                cats[2].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[2].defence -= (cats[0].attack * (cats[0].defence / cats[2].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(cats[0].x == cats[3].x && (cats[0].y - 1) == cats[3].y) {
                            if (fight_between_cats(cats[0], cats[3]) == 1) {

                                cats[0].score += cats[3].score;
                                cats[0].nm1 += cats[3].nm1;
                                cats[0].nm2 += cats[3].nm2;
                                cats[0].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[0].defence -= (cats[3].attack * (cats[3].defence / cats[0].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[3]) == 2) {

                                cats[3].score += cats[0].score;
                                cats[3].nm1 += cats[0].nm1;
                                cats[3].nm2 += cats[0].nm2;
                                cats[3].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[3].defence -= (cats[0].attack * (cats[0].defence / cats[3].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x][cats[0].y - 1] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -88;
                                dogs[0].y = -34;

                                cats[0].defence -= (dogs[0].attack * (dogs[0].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[0]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[0].attack * (cats[0].defence / dogs[0].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x][cats[0].y - 1] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -22;
                                dogs[1].y = -19;

                                cats[0].defence -= (dogs[1].attack * (dogs[1].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[1]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[0].attack * (cats[0].defence / dogs[1].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x][cats[0].y - 1] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -44;
                                dogs[2].y = -67;

                                cats[0].defence -= (dogs[2].attack * (dogs[2].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[2]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[0].attack * (cats[0].defence / dogs[2].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(cats[0].x == dogs[3].x && cats[0].y - 1 == dogs[3].y) {
                            if(fight_between_cats_dogs(cats[0], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -11;
                                dogs[3].y = -98;

                                cats[0].defence -= (dogs[3].attack * (dogs[3].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[3]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[0].attack * (cats[0].defence / dogs[3].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }










                        cats[0].y--;
                        if(cats[0].defence > 0)
                            cats[0].defence--;
                        cats[0].allow--;

                    }
                    break;







                case ALLEGRO_KEY_DOWN:
                    if(cats[0].y < 14 && board[cats[0].x][cats[0].y] % 3 != 0 && board[cats[0].x][cats[0].y] % 17 != 0){
                        if(board[cats[0].x][cats[0].y + 1] % 37 == 0){
                            cats[0].attack++;
                            board[cats[0].x][cats[0].y + 1] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[0].x][cats[0].y + 1]) {
                                i++;
                            }
                            meats[i].x = -5;
                            meats[i].y = -5;

                        }
                        else if(board[cats[0].x][cats[0].y + 1] % 41 == 0){
                            cats[0].score++;
                            board[cats[0].x][cats[0].y + 1] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[0].x][cats[0].y + 1]) {
                                i++;
                            }
                            m1[i].x = -6;
                            m1[i].y = -6;
                            cats[0].nm1++;

                        }
                        else if(board[cats[0].x][cats[0].y + 1] % 43 == 0){
                            cats[0].score += 2;
                            board[cats[0].x][cats[0].y + 1] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[0].x][cats[0].y + 1]) {
                                i++;
                            }
                            m2[i].x = -7;
                            m2[i].y = -7;
                            cats[0].nm2++;

                        }
                        else if(board[cats[0].x][cats[0].y + 1] % 47 == 0){
                            cats[0].score += 3;
                            board[cats[0].x][cats[0].y + 1] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[0].x][cats[0].y + 1]) {
                                i++;
                            }
                            m3[i].x = -8;
                            m3[i].y = -8;
                            cats[0].nm3++;

                        }
                        else if(board[cats[0].x][cats[0].y + 1] % 53 == 0){
                            cats[0].defence += 2;
                            board[cats[0].x][cats[0].y + 1] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[0].x][cats[0].y + 1]) {
                                i++;
                            }
                            f2[i].x = -24;
                            f2[i].y = -33;
                            cats[0].nf++;

                        }
                        else if(board[cats[0].x][cats[0].y + 1] % 59 == 0){
                            cats[0].defence += 3;
                            board[cats[0].x][cats[0].y + 1] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[0].x][cats[0].y + 1]) {
                                i++;
                            }
                            f3[i].x = -66;
                            f3[i].y = -39;
                            cats[0].nf++;

                        }
                        else if(board[cats[0].x][cats[0].y + 1] % 61 == 0){
                            cats[0].defence += 4;
                            board[cats[0].x][cats[0].y + 1] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[0].x][cats[0].y + 1]) {
                                i++;
                            }
                            f4[i].x = -23;
                            f4[i].y = -32;
                            cats[0].nf++;

                        }
                        else if(board[cats[0].x][cats[0].y + 1] % 67 == 0) {
                            if(cats[0].nm3 > 0) {
                                cats[0].nm3--;
                                cats[0].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[0].nm2 > 0) {
                                cats[0].nm2--;
                                cats[0].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[0].nm1 > 0) {
                                cats[0].nm1--;
                                cats[0].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[0].attack > 1) {
                                cats[0].attack -= 2;
                            }
                            else if(cats[0].defence > 2) {
                                cats[0].defence -= 3;
                            }
                            cats[0].allow = 1;
                        }
                        else if(cats[0].x == cats[1].x && (cats[0].y + 1) == cats[1].y) {
                            if (fight_between_cats(cats[0], cats[1]) == 1) {

                                cats[0].score += cats[1].score;
                                cats[0].nm1 += cats[1].nm1;
                                cats[0].nm2 += cats[1].nm2;
                                cats[0].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[0].defence -= (cats[1].attack * (cats[1].defence / cats[0].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[1]) == 2) {

                                cats[1].score += cats[0].score;
                                cats[1].nm1 += cats[0].nm1;
                                cats[1].nm2 += cats[0].nm2;
                                cats[1].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[1].defence -= (cats[0].attack * (cats[0].defence / cats[1].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(cats[0].x == cats[2].x && (cats[0].y + 1) == cats[2].y) {
                            if (fight_between_cats(cats[0], cats[2]) == 1) {

                                cats[0].score += cats[2].score;
                                cats[0].nm1 += cats[2].nm1;
                                cats[0].nm2 += cats[2].nm2;
                                cats[0].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[0].defence -= (cats[2].attack * (cats[2].defence / cats[0].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[2]) == 2) {

                                cats[2].score += cats[0].score;
                                cats[2].nm1 += cats[0].nm1;
                                cats[2].nm2 += cats[0].nm2;
                                cats[2].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[2].defence -= (cats[0].attack * (cats[0].defence / cats[2].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(cats[0].x == cats[3].x && (cats[0].y + 1) == cats[3].y) {
                            if (fight_between_cats(cats[0], cats[3]) == 1) {

                                cats[0].score += cats[3].score;
                                cats[0].nm1 += cats[3].nm1;
                                cats[0].nm2 += cats[3].nm2;
                                cats[0].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[0].defence -= (cats[3].attack * (cats[3].defence / cats[0].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[3]) == 2) {

                                cats[3].score += cats[0].score;
                                cats[3].nm1 += cats[0].nm1;
                                cats[3].nm2 += cats[0].nm2;
                                cats[3].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[3].defence -= (cats[0].attack * (cats[0].defence / cats[3].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x][cats[0].y + 1] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -22;
                                dogs[0].y = -32;

                                cats[0].defence -= (dogs[0].attack * (dogs[0].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[0]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[0].attack * (cats[0].defence / dogs[0].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x][cats[0].y + 1] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -45;
                                dogs[1].y = -79;

                                cats[0].defence -= (dogs[1].attack * (dogs[1].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[1]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[0].attack * (cats[0].defence / dogs[1].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x][cats[0].y + 1] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -63;
                                dogs[2].y = -23;

                                cats[0].defence -= (dogs[2].attack * (dogs[2].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[2]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[0].attack * (cats[0].defence / dogs[2].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x][cats[0].y + 1] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -90;
                                dogs[3].y = -62;

                                cats[0].defence -= (dogs[3].attack * (dogs[3].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[3]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[0].attack * (cats[0].defence / dogs[3].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }








                        cats[0].y++;
                        if(cats[0].defence > 0)
                            cats[0].defence--;
                        cats[0].allow--;
                    }
                    break;






                case ALLEGRO_KEY_LEFT:
                    if(cats[0].x > 0 && board[cats[0].x][cats[0].y] % 11 != 0){
                        if(board[cats[0].x - 1][cats[0].y] % 37 == 0){
                            cats[0].attack++;
                            board[cats[0].x - 1][cats[0].y] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[0].x - 1][cats[0].y]) {
                                i++;
                            }
                            meats[i].x = -9;
                            meats[i].y = -9;

                        }
                        else if(board[cats[0].x - 1][cats[0].y] % 41 == 0){
                            cats[0].score++;
                            board[cats[0].x - 1][cats[0].y] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[0].x - 1][cats[0].y]) {
                                i++;
                            }
                            m1[i].x = -10;
                            m1[i].y = -10;
                            cats[0].nm1++;

                        }
                        else if(board[cats[0].x - 1][cats[0].y] % 43 == 0){
                            cats[0].score += 2;
                            board[cats[0].x - 1][cats[0].y] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[0].x - 1][cats[0].y]) {
                                i++;
                            }
                            m2[i].x = -11;
                            m2[i].y = -11;
                            cats[0].nm2++;

                        }
                        else if(board[cats[0].x - 1][cats[0].y] % 47 == 0){
                            cats[0].score += 3;
                            board[cats[0].x - 1][cats[0].y] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[0].x - 1][cats[0].y]) {
                                i++;
                            }
                            m3[i].x = -12;
                            m3[i].y = -12;
                            cats[0].nm3++;

                        }
                        else if(board[cats[0].x - 1][cats[0].y] % 53 == 0){
                            cats[0].defence += 2;
                            board[cats[0].x - 1][cats[0].y] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[0].x - 1][cats[0].y]) {
                                i++;
                            }
                            f2[i].x = -40;
                            f2[i].y = -50;
                            cats[0].nf++;

                        }
                        else if(board[cats[0].x - 1][cats[0].y] % 59 == 0){
                            cats[0].defence += 3;
                            board[cats[0].x - 1][cats[0].y] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[0].x - 1][cats[0].y]) {
                                i++;
                            }
                            f3[i].x = -54;
                            f3[i].y = -12;
                            cats[0].nf++;

                        }
                        else if(board[cats[0].x - 1][cats[0].y] % 61 == 0){
                            cats[0].defence += 4;
                            board[cats[0].x - 1][cats[0].y] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[0].x - 1][cats[0].y]) {
                                i++;
                            }
                            f4[i].x = -21;
                            f4[i].y = -12;
                            cats[0].nf++;

                        }
                        else if(board[cats[0].x - 1][cats[0].y] % 67 == 0) {
                            if(cats[0].nm3 > 0) {
                                cats[0].nm3--;
                                cats[0].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[0].nm2 > 0) {
                                cats[0].nm2--;
                                cats[0].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[0].nm1 > 0) {
                                cats[0].nm1--;
                                cats[0].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[0].attack > 1) {
                                cats[0].attack -= 2;
                            }
                            else if(cats[0].defence > 2) {
                                cats[0].defence -= 3;
                            }
                            cats[0].allow = 1;
                        }
                        else if((cats[0].x - 1) == cats[1].x && cats[0].y == cats[1].y) {
                            if (fight_between_cats(cats[0], cats[1]) == 1) {

                                cats[0].score += cats[1].score;
                                cats[0].nm1 += cats[1].nm1;
                                cats[0].nm2 += cats[1].nm2;
                                cats[0].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[0].defence -= (cats[1].attack * (cats[1].defence / cats[0].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[1]) == 2) {

                                cats[1].score += cats[0].score;
                                cats[1].nm1 += cats[0].nm1;
                                cats[1].nm2 += cats[0].nm2;
                                cats[1].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[1].defence -= (cats[0].attack * (cats[0].defence / cats[1].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if((cats[0].x - 1) == cats[2].x && cats[0].y == cats[2].y) {
                            if (fight_between_cats(cats[0], cats[2]) == 1) {

                                cats[0].score += cats[2].score;
                                cats[0].nm1 += cats[2].nm1;
                                cats[0].nm2 += cats[2].nm2;
                                cats[0].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[0].defence -= (cats[2].attack * (cats[2].defence / cats[0].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[2]) == 2) {

                                cats[2].score += cats[0].score;
                                cats[2].nm1 += cats[0].nm1;
                                cats[2].nm2 += cats[0].nm2;
                                cats[2].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[2].defence -= (cats[0].attack * (cats[0].defence / cats[2].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if((cats[0].x - 1) == cats[3].x && cats[0].y == cats[3].y) {
                            if (fight_between_cats(cats[0], cats[3]) == 1) {

                                cats[0].score += cats[3].score;
                                cats[0].nm1 += cats[3].nm1;
                                cats[0].nm2 += cats[3].nm2;
                                cats[0].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[0].defence -= (cats[3].attack * (cats[3].defence / cats[0].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[3]) == 2) {

                                cats[3].score += cats[0].score;
                                cats[3].nm1 += cats[0].nm1;
                                cats[3].nm2 += cats[0].nm2;
                                cats[3].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[3].defence -= (cats[0].attack * (cats[0].defence / cats[3].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x - 1][cats[0].y] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -23;
                                dogs[0].y = -90;

                                cats[0].defence -= (dogs[0].attack * (dogs[0].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[0]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[0].attack * (cats[0].defence / dogs[0].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x - 1][cats[0].y] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -40;
                                dogs[1].y = -12;

                                cats[0].defence -= (dogs[1].attack * (dogs[1].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[1]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[0].attack * (cats[0].defence / dogs[1].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x - 1][cats[0].y] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -66;
                                dogs[2].y = -93;

                                cats[0].defence -= (dogs[2].attack * (dogs[2].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[2]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[0].attack * (cats[0].defence / dogs[2].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x - 1][cats[0].y] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -12;
                                dogs[3].y = -15;

                                cats[0].defence -= (dogs[3].attack * (dogs[3].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[3]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[0].attack * (cats[0].defence / dogs[3].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }








                        cats[0].x--;
                        if(cats[0].defence > 0)
                            cats[0].defence--;
                        cats[0].allow--;
                    }
                    break;





                case ALLEGRO_KEY_RIGHT:
                    if(cats[0].x < 14 && board[cats[0].x][cats[0].y] % 7 != 0 && board[cats[0].x][cats[0].y] % 13 != 0 && board[cats[0].x][cats[0].y] % 17 != 0){
                        if(board[cats[0].x + 1][cats[0].y] % 37 == 0){
                            cats[0].attack++;
                            board[cats[0].x + 1][cats[0].y] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[0].x + 1][cats[0].y]) {
                                i++;
                            }
                            meats[i].x = -13;
                            meats[i].y = -13;

                        }
                        else if(board[cats[0].x + 1][cats[0].y] % 41 == 0){
                            cats[0].score++;
                            board[cats[0].x + 1][cats[0].y] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[0].x + 1][cats[0].y]) {
                                i++;
                            }
                            m1[i].x = -14;
                            m1[i].y = -14;
                            cats[0].nm1++;

                        }
                        else if(board[cats[0].x + 1][cats[0].y] % 43 == 0){
                            cats[0].score += 2;
                            board[cats[0].x + 1][cats[0].y] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[0].x + 1][cats[0].y]) {
                                i++;
                            }
                            m2[i].x = -15;
                            m2[i].y = -15;
                            cats[0].nm2++;

                        }
                        else if(board[cats[0].x + 1][cats[0].y] % 47 == 0){
                            cats[0].score += 3;
                            board[cats[0].x + 1][cats[0].y] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[0].x + 1][cats[0].y]) {
                                i++;
                            }
                            m3[i].x = -16;
                            m3[i].y = -16;
                            cats[0].nm3++;

                        }
                        else if(board[cats[0].x + 1][cats[0].y] % 53 == 0){
                            cats[0].defence += 2;
                            board[cats[0].x + 1][cats[0].y] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[0].x + 1][cats[0].y]) {
                                i++;
                            }
                            f2[i].x = -12;
                            f2[i].y = -90;
                            cats[0].nf++;

                        }
                        else if(board[cats[0].x + 1][cats[0].y] % 59 == 0){
                            cats[0].defence += 3;
                            board[cats[0].x + 1][cats[0].y] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[0].x + 1][cats[0].y]) {
                                i++;
                            }
                            f3[i].x = -56;
                            f3[i].y = -73;
                            cats[0].nf++;

                        }
                        else if(board[cats[0].x + 1][cats[0].y] % 61 == 0){
                            cats[0].defence += 4;
                            board[cats[0].x + 1][cats[0].y] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[0].x + 1][cats[0].y]) {
                                i++;
                            }
                            f4[i].x = -94;
                            f4[i].y = -13;
                            cats[0].nf++;

                        }
                        else if(board[cats[0].x + 1][cats[0].y] % 67 == 0) {
                            if(cats[0].nm3 > 0) {
                                cats[0].nm3--;
                                cats[0].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[0].nm2 > 0) {
                                cats[0].nm2--;
                                cats[0].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[0].nm1 > 0) {
                                cats[0].nm1--;
                                cats[0].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[0].attack > 1) {
                                cats[0].attack -= 2;
                            }
                            else if(cats[0].defence > 2) {
                                cats[0].defence -= 3;
                            }
                            cats[0].allow = 1;
                        }
                        else if((cats[0].x + 1) == cats[1].x && cats[0].y == cats[1].y) {
                            if (fight_between_cats(cats[0], cats[1]) == 1) {

                                cats[0].score += cats[1].score;
                                cats[0].nm1 += cats[1].nm1;
                                cats[0].nm2 += cats[1].nm2;
                                cats[0].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[0].defence -= (cats[1].attack * (cats[1].defence / cats[0].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[1]) == 2) {

                                cats[1].score += cats[0].score;
                                cats[1].nm1 += cats[0].nm1;
                                cats[1].nm2 += cats[0].nm2;
                                cats[1].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[1].defence -= (cats[0].attack * (cats[0].defence / cats[1].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if((cats[0].x + 1) == cats[2].x && cats[0].y == cats[2].y) {
                            if (fight_between_cats(cats[0], cats[2]) == 1) {

                                cats[0].score += cats[2].score;
                                cats[0].nm1 += cats[2].nm1;
                                cats[0].nm2 += cats[2].nm2;
                                cats[0].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[0].defence -= (cats[2].attack * (cats[2].defence / cats[0].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[2]) == 2) {

                                cats[2].score += cats[0].score;
                                cats[2].nm1 += cats[0].nm1;
                                cats[2].nm2 += cats[0].nm2;
                                cats[2].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[2].defence -= (cats[0].attack * (cats[0].defence / cats[2].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if((cats[0].x + 1) == cats[3].x && cats[0].y == cats[3].y) {
                            if (fight_between_cats(cats[0], cats[3]) == 1) {

                                cats[0].score += cats[3].score;
                                cats[0].nm1 += cats[3].nm1;
                                cats[0].nm2 += cats[3].nm2;
                                cats[0].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[0].defence -= (cats[3].attack * (cats[3].defence / cats[0].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[3]) == 2) {

                                cats[3].score += cats[0].score;
                                cats[3].nm1 += cats[0].nm1;
                                cats[3].nm2 += cats[0].nm2;
                                cats[3].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;


                                cats[3].defence -= (cats[0].attack * (cats[0].defence / cats[3].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x + 1][cats[0].y] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -53;
                                dogs[0].y = -21;

                                cats[0].defence -= (dogs[0].attack * (dogs[0].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[0]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[0].attack * (cats[0].defence / dogs[0].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x + 1][cats[0].y] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -21;
                                dogs[1].y = -33;

                                cats[0].defence -= (dogs[1].attack * (dogs[1].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[1]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[0].attack * (cats[0].defence / dogs[1].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x + 1][cats[0].y] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -55;
                                dogs[2].y = -76;

                                cats[0].defence -= (dogs[2].attack * (dogs[2].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[2]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[0].attack * (cats[0].defence / dogs[2].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }
                        else if(board[cats[0].x + 1][cats[0].y] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[0], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -22;
                                dogs[3].y = -68;

                                cats[0].defence -= (dogs[3].attack * (dogs[3].defence / cats[0].attack));
                                cats[0].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[0], dogs[3]) == 2) {
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;
                                cats[0].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[0].attack * (cats[0].defence / dogs[3].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 1;
                            }
                        }










                        cats[0].x++;
                        if(cats[0].defence > 0)
                            cats[0].defence--;
                        cats[0].allow--;
                    }
                    break;


            }
            if(cats[0].allow == 0)
                cats[0].start = 0;
        }

        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN && cats[1].allow>0 && (cats[1].start > cats[0].start) && (cats[1].start > cats[2].start) && (cats[1].start > cats[3].start)){
            cats[0].turn = ' ';
            cats[1].turn = '*';
            cats[2].turn = ' ';
            cats[3].turn = ' ';


            switch(ev.keyboard.keycode) {

                case ALLEGRO_KEY_ENTER:

                    cats[1].allow = 0;
                    break;


                case ALLEGRO_KEY_ESCAPE:
                    return -1;
                    break;




                case ALLEGRO_KEY_UP:
                    if(cats[1].y > 0 && board[cats[1].x][cats[1].y] % 5 != 0 && board[cats[1].x][cats[1].y] % 13 != 0){
                        if(board[cats[1].x][cats[1].y - 1] % 37 == 0){
                            cats[1].attack++;
                            board[cats[1].x][cats[1].y - 1] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[1].x][cats[1].y - 1]) {
                                i++;
                            }
                            meats[i].x = -17;
                            meats[i].y = -17;

                        }
                        else if(board[cats[1].x][cats[1].y - 1] % 41 == 0){
                            cats[1].score++;
                            board[cats[1].x][cats[1].y - 1] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[1].x][cats[1].y - 1]) {
                                i++;
                            }
                            m1[i].x = -18;
                            m1[i].y = -18;
                            cats[1].nm1++;

                        }
                        else if(board[cats[1].x][cats[1].y - 1] % 43 == 0){
                            cats[1].score += 2;
                            board[cats[1].x][cats[1].y - 1] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[1].x][cats[1].y - 1]) {
                                i++;
                            }
                            m2[i].x = -19;
                            m2[i].y = -19;
                            cats[1].nm2++;

                        }
                        else if(board[cats[1].x][cats[1].y - 1] % 47 == 0){
                            cats[1].score += 3;
                            board[cats[1].x][cats[1].y - 1] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[1].x][cats[1].y - 1]) {
                                i++;
                            }
                            m3[i].x = -20;
                            m3[i].y = -20;
                            cats[1].nm3++;

                        }
                        else if(board[cats[1].x][cats[1].y - 1] % 53 == 0){
                            cats[1].defence += 2;
                            board[cats[1].x][cats[1].y - 1] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[1].x][cats[1].y - 1]) {
                                i++;
                            }
                            f2[i].x = -43;
                            f2[i].y = -99;
                            cats[1].nf++;

                        }
                        else if(board[cats[1].x][cats[1].y - 1] % 59 == 0){
                            cats[1].defence += 3;
                            board[cats[1].x][cats[1].y - 1] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[1].x][cats[1].y - 1]) {
                                i++;
                            }
                            f3[i].x = -32;
                            f3[i].y = -55;
                            cats[1].nf++;

                        }
                        else if(board[cats[1].x][cats[1].y - 1] % 61 == 0){
                            cats[1].defence += 4;
                            board[cats[1].x][cats[1].y - 1] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[1].x][cats[1].y - 1]) {
                                i++;
                            }
                            f4[i].x = -32;
                            f4[i].y = -46;
                            cats[1].nf++;
                        }
                        else if(board[cats[1].x][cats[1].y - 1] % 67 == 0){
                            if(cats[1].nm3 > 0) {
                                cats[1].nm3--;
                                cats[1].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[1].nm2 > 0) {
                                cats[1].nm2--;
                                cats[1].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[1].nm1 > 0) {
                                cats[1].nm1--;
                                cats[1].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[1].attack > 1) {
                                cats[1].attack -= 2;
                            }
                            else if(cats[1].defence > 2) {
                                cats[1].defence -= 3;
                            }
                            cats[1].allow = 1;
                        }
                        else if(cats[1].x == cats[0].x && (cats[1].y - 1) == cats[0].y) {
                            if (fight_between_cats(cats[1], cats[0]) == 1) {

                                cats[1].score += cats[0].score;
                                cats[1].nm1 += cats[0].nm1;
                                cats[1].nm2 += cats[0].nm2;
                                cats[1].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[1].defence -= (cats[0].attack * (cats[0].defence / cats[1].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 0;

                            }
                            else if(fight_between_cats(cats[1], cats[0]) == 2) {

                                cats[0].score += cats[1].score;
                                cats[0].nm1 += cats[1].nm1;
                                cats[0].nm2 += cats[1].nm2;
                                cats[0].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[0].defence -= (cats[1].attack * (cats[1].defence / cats[0].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(cats[1].x == cats[2].x && (cats[1].y - 1) == cats[2].y) {
                            if (fight_between_cats(cats[1], cats[2]) == 1) {

                                cats[1].score += cats[2].score;
                                cats[1].nm1 += cats[2].nm1;
                                cats[1].nm2 += cats[2].nm2;
                                cats[1].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[1].defence -= (cats[2].attack * (cats[2].defence / cats[1].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[2]) == 2) {

                                cats[2].score += cats[1].score;
                                cats[2].nm1 += cats[1].nm1;
                                cats[2].nm2 += cats[1].nm2;
                                cats[2].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[2].defence -= (cats[1].attack * (cats[1].defence / cats[2].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(cats[1].x == cats[3].x && (cats[1].y - 1) == cats[3].y) {
                            if (fight_between_cats(cats[1], cats[3]) == 1) {

                                cats[1].score += cats[3].score;
                                cats[1].nm1 += cats[3].nm1;
                                cats[1].nm2 += cats[3].nm2;
                                cats[1].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[1].defence -= (cats[3].attack * (cats[3].defence / cats[1].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 0;

                            }
                            else if(fight_between_cats(cats[1], cats[3]) == 2) {

                                cats[3].score += cats[1].score;
                                cats[3].nm1 += cats[1].nm1;
                                cats[3].nm2 += cats[1].nm2;
                                cats[3].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[3].defence -= (cats[1].attack * (cats[1].defence / cats[3].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x][cats[1].y - 1] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -321;
                                dogs[0].y = -984;

                                cats[1].defence -= (dogs[0].attack * (dogs[0].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[0]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[1].attack * (cats[1].defence / dogs[0].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x][cats[1].y - 1] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -110;
                                dogs[1].y = -110;

                                cats[1].defence -= (dogs[1].attack * (dogs[1].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[1]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[1].attack * (cats[1].defence / dogs[1].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x][cats[1].y - 1] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -120;
                                dogs[2].y = -120;

                                cats[1].defence -= (dogs[2].attack * (dogs[2].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[2]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[1].attack * (cats[1].defence / dogs[2].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x][cats[1].y - 1] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -120;
                                dogs[3].y = -120;

                                cats[1].defence -= (dogs[3].attack * (dogs[3].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[3]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[1].attack * (cats[1].defence / dogs[3].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }








                        cats[1].y--;
                        if(cats[1].defence > 0)
                            cats[1].defence--;
                        cats[1].allow--;
                    }
                    break;







                case ALLEGRO_KEY_DOWN:
                    if(cats[1].y < 14 && board[cats[1].x][cats[1].y] % 3 != 0 && board[cats[1].x][cats[1].y] % 17 != 0){
                        if(board[cats[1].x][cats[1].y + 1] % 37 == 0){
                            cats[1].attack++;
                            board[cats[1].x][cats[1].y + 1] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[1].x][cats[1].y + 1]) {
                                i++;
                            }
                            meats[i].x = -21;
                            meats[i].y = -21;

                        }
                        else if(board[cats[1].x][cats[1].y + 1] % 41 == 0){
                            cats[1].score++;
                            board[cats[1].x][cats[1].y + 1] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[1].x][cats[1].y + 1]) {
                                i++;
                            }
                            m1[i].x = -22;
                            m1[i].y = -22;
                            cats[1].nm1++;

                        }
                        else if(board[cats[1].x][cats[1].y + 1] % 43 == 0){
                            cats[1].score += 2;
                            board[cats[1].x][cats[1].y + 1] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[1].x][cats[1].y + 1]) {
                                i++;
                            }
                            m2[i].x = -23;
                            m2[i].y = -23;
                            cats[1].nm2++;

                        }
                        else if(board[cats[1].x][cats[1].y + 1] % 47 == 0){
                            cats[1].score += 3;
                            board[cats[1].x][cats[1].y + 1] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[1].x][cats[1].y + 1]) {
                                i++;
                            }
                            m3[i].x = -24;
                            m3[i].y = -24;
                            cats[1].nm3++;

                        }
                        else if(board[cats[1].x][cats[1].y + 1] % 53 == 0){
                            cats[1].defence += 2;
                            board[cats[1].x][cats[1].y + 1] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[1].x][cats[1].y + 1]) {
                                i++;
                            }
                            f2[i].x = -67;
                            f2[i].y = -76;
                            cats[1].nf++;

                        }
                        else if(board[cats[1].x][cats[1].y + 1] % 59 == 0){
                            cats[1].defence += 3;
                            board[cats[1].x][cats[1].y + 1] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[1].x][cats[1].y + 1]) {
                                i++;
                            }
                            f3[i].x = -88;
                            f3[i].y = -93;
                            cats[1].nf++;

                        }
                        else if(board[cats[1].x][cats[1].y + 1] % 61 == 0){
                            cats[1].defence += 4;
                            board[cats[1].x][cats[1].y + 1] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[1].x][cats[1].y + 1]) {
                                i++;
                            }
                            f4[i].x = -30;
                            f4[i].y = -93;
                            cats[1].nf++;

                        }
                        else if(board[cats[1].x][cats[1].y + 1] % 67 == 0) {
                            if(cats[1].nm3 > 0) {
                                cats[1].nm3--;
                                cats[1].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[1].nm2 > 0) {
                                cats[1].nm2--;
                                cats[1].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[1].nm1 > 0) {
                                cats[1].nm1--;
                                cats[1].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[1].attack > 1) {
                                cats[1].attack -= 2;
                            }
                            else if(cats[1].defence > 2) {
                                cats[1].defence -= 3;
                            }
                            cats[1].allow = 1;
                        }
                        else if(cats[1].x == cats[0].x && (cats[1].y + 1) == cats[0].y) {
                            if (fight_between_cats(cats[1], cats[0]) == 1) {

                                cats[1].score += cats[0].score;
                                cats[1].nm1 += cats[0].nm1;
                                cats[1].nm2 += cats[0].nm2;
                                cats[1].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[1].defence -= (cats[0].attack * (cats[0].defence / cats[1].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 0;

                            }
                            else if(fight_between_cats(cats[1], cats[0]) == 2) {

                                cats[0].score += cats[1].score;
                                cats[0].nm1 += cats[1].nm1;
                                cats[0].nm2 += cats[1].nm2;
                                cats[0].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[0].defence -= (cats[1].attack * (cats[1].defence / cats[0].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(cats[1].x == cats[2].x && (cats[1].y + 1) == cats[2].y) {
                            if (fight_between_cats(cats[1], cats[2]) == 1) {

                                cats[1].score += cats[2].score;
                                cats[1].nm1 += cats[2].nm1;
                                cats[1].nm2 += cats[2].nm2;
                                cats[1].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[1].defence -= (cats[2].attack * (cats[2].defence / cats[1].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[2]) == 2) {

                                cats[2].score += cats[1].score;
                                cats[2].nm1 += cats[1].nm1;
                                cats[2].nm2 += cats[1].nm2;
                                cats[2].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[2].defence -= (cats[1].attack * (cats[1].defence / cats[2].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(cats[1].x == cats[3].x && (cats[1].y + 1) == cats[3].y) {
                            if (fight_between_cats(cats[1], cats[3]) == 1) {

                                cats[1].score += cats[3].score;
                                cats[1].nm1 += cats[3].nm1;
                                cats[1].nm2 += cats[3].nm2;
                                cats[1].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[1].defence -= (cats[3].attack * (cats[3].defence / cats[1].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 0;

                            }
                            else if(fight_between_cats(cats[1], cats[3]) == 2) {

                                cats[3].score += cats[1].score;
                                cats[3].nm1 += cats[1].nm1;
                                cats[3].nm2 += cats[1].nm2;
                                cats[3].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[3].defence -= (cats[1].attack * (cats[1].defence / cats[3].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x][cats[1].y + 1] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -100;
                                dogs[0].y = -100;

                                cats[1].defence -= (dogs[0].attack * (dogs[0].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[0]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[1].attack * (cats[1].defence / dogs[0].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x][cats[1].y + 1] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -110;
                                dogs[1].y = -110;

                                cats[1].defence -= (dogs[1].attack * (dogs[1].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[1]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[1].attack * (cats[1].defence / dogs[1].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x][cats[1].y + 1] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -120;
                                dogs[2].y = -120;

                                cats[1].defence -= (dogs[2].attack * (dogs[2].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[2]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[1].attack * (cats[1].defence / dogs[2].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x][cats[1].y + 1] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -120;
                                dogs[3].y = -120;

                                cats[1].defence -= (dogs[3].attack * (dogs[3].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[3]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[1].attack * (cats[1].defence / dogs[3].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }









                        cats[1].y++;
                        if(cats[1].defence > 0)
                            cats[1].defence--;
                        cats[1].allow--;
                    }
                    break;






                case ALLEGRO_KEY_LEFT:
                    if(cats[1].x > 0 && board[cats[1].x][cats[1].y] % 11 != 0){
                        if(board[cats[1].x - 1][cats[1].y] % 37 == 0){
                            cats[1].attack++;
                            board[cats[1].x - 1][cats[1].y] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[1].x - 1][cats[1].y]) {
                                i++;
                            }
                            meats[i].x = -25;
                            meats[i].y = -25;

                        }
                        else if(board[cats[1].x - 1][cats[1].y] % 41 == 0){
                            cats[1].score++;
                            board[cats[1].x - 1][cats[1].y] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[1].x - 1][cats[1].y]) {
                                i++;
                            }
                            m1[i].x = -26;
                            m1[i].y = -26;
                            cats[1].nm1++;

                        }
                        else if(board[cats[1].x - 1][cats[1].y] % 43 == 0){
                            cats[1].score += 2;
                            board[cats[1].x - 1][cats[1].y] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[1].x - 1][cats[1].y]) {
                                i++;
                            }
                            m2[i].x = -27;
                            m2[i].y = -27;
                            cats[1].nm2++;

                        }
                        else if(board[cats[1].x - 1][cats[1].y] % 47 == 0){
                            cats[1].score += 3;
                            board[cats[1].x - 1][cats[1].y] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[1].x - 1][cats[1].y]) {
                                i++;
                            }
                            m3[i].x = -28;
                            m3[i].y = -28;
                            cats[1].nm3++;

                        }
                        else if(board[cats[1].x - 1][cats[1].y] % 53 == 0){
                            cats[1].defence += 2;
                            board[cats[1].x - 1][cats[1].y] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[1].x - 1][cats[1].y]) {
                                i++;
                            }
                            f2[i].x = -44;
                            f2[i].y = -65;
                            cats[1].nf++;

                        }
                        else if(board[cats[1].x - 1][cats[1].y] % 59 == 0){
                            cats[1].defence += 3;
                            board[cats[1].x - 1][cats[1].y] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[1].x - 1][cats[1].y]) {
                                i++;
                            }
                            f3[i].x = -32;
                            f3[i].y = -87;
                            cats[1].nf++;

                        }
                        else if(board[cats[1].x - 1][cats[1].y] % 61 == 0){
                            cats[1].defence += 4;
                            board[cats[1].x - 1][cats[1].y] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[1].x - 1][cats[1].y]) {
                                i++;
                            }
                            f4[i].x = -54;
                            f4[i].y = -39;
                            cats[1].nf++;

                        }
                        else if(board[cats[1].x - 1][cats[1].y] % 67 == 0) {
                            if(cats[1].nm3 > 0) {
                                cats[1].nm3--;
                                cats[1].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[1].nm2 > 0) {
                                cats[1].nm2--;
                                cats[1].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[1].nm1 > 0) {
                                cats[1].nm1--;
                                cats[1].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[1].attack > 1) {
                                cats[1].attack -= 2;
                            }
                            else if(cats[1].defence > 2) {
                                cats[1].defence -= 3;
                            }
                            cats[1].allow = 1;
                        }
                        else if((cats[1].x - 1) == cats[0].x && cats[1].y == cats[0].y) {
                            if (fight_between_cats(cats[1], cats[0]) == 1) {

                                cats[1].score += cats[0].score;
                                cats[1].nm1 += cats[0].nm1;
                                cats[1].nm2 += cats[0].nm2;
                                cats[1].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[1].defence -= (cats[0].attack * (cats[0].defence / cats[1].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 0;

                            }
                            else if(fight_between_cats(cats[1], cats[0]) == 2) {

                                cats[0].score += cats[1].score;
                                cats[0].nm1 += cats[1].nm1;
                                cats[0].nm2 += cats[1].nm2;
                                cats[0].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[0].defence -= (cats[1].attack * (cats[1].defence / cats[0].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if((cats[1].x - 1) == cats[2].x && cats[1].y == cats[2].y) {
                            if (fight_between_cats(cats[1], cats[2]) == 1) {

                                cats[1].score += cats[2].score;
                                cats[1].nm1 += cats[2].nm1;
                                cats[1].nm2 += cats[2].nm2;
                                cats[1].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[1].defence -= (cats[2].attack * (cats[2].defence / cats[1].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[2]) == 2) {

                                cats[2].score += cats[1].score;
                                cats[2].nm1 += cats[1].nm1;
                                cats[2].nm2 += cats[1].nm2;
                                cats[2].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[2].defence -= (cats[1].attack * (cats[1].defence / cats[2].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if((cats[1].x - 1) == cats[3].x && cats[1].y == cats[3].y) {
                            if (fight_between_cats(cats[1], cats[3]) == 1) {

                                cats[1].score += cats[3].score;
                                cats[1].nm1 += cats[3].nm1;
                                cats[1].nm2 += cats[3].nm2;
                                cats[1].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[1].defence -= (cats[3].attack * (cats[3].defence / cats[1].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 0;

                            }
                            else if(fight_between_cats(cats[1], cats[3]) == 2) {

                                cats[3].score += cats[1].score;
                                cats[3].nm1 += cats[1].nm1;
                                cats[3].nm2 += cats[1].nm2;
                                cats[3].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[3].defence -= (cats[1].attack * (cats[1].defence / cats[3].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x - 1][cats[1].y] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -100;
                                dogs[0].y = -100;

                                cats[1].defence -= (dogs[0].attack * (dogs[0].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[0]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[1].attack * (cats[1].defence / dogs[0].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x - 1][cats[1].y] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -110;
                                dogs[1].y = -110;

                                cats[1].defence -= (dogs[1].attack * (dogs[1].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[1]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[1].attack * (cats[1].defence / dogs[1].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x - 1][cats[1].y] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -120;
                                dogs[2].y = -120;

                                cats[1].defence -= (dogs[2].attack * (dogs[2].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[2]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[1].attack * (cats[1].defence / dogs[2].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x - 1][cats[1].y] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -120;
                                dogs[3].y = -120;

                                cats[1].defence -= (dogs[3].attack * (dogs[3].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[3]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[1].attack * (cats[1].defence / dogs[3].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }








                        cats[1].x--;
                        if(cats[1].defence > 0)
                            cats[1].defence--;
                        cats[1].allow--;
                    }
                    break;





                case ALLEGRO_KEY_RIGHT:
                    if(cats[1].x < 14 && board[cats[1].x][cats[1].y] % 7 != 0 && board[cats[1].x][cats[1].y] % 13 != 0 && board[cats[1].x][cats[1].y] % 17 != 0){
                        if(board[cats[1].x + 1][cats[1].y] % 37 == 0){
                            cats[1].attack++;
                            board[cats[1].x + 1][cats[1].y] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[1].x + 1][cats[1].y]) {
                                i++;
                            }
                            meats[i].x = -29;
                            meats[i].y = -29;

                        }
                        else if(board[cats[1].x + 1][cats[1].y] % 41 == 0){
                            cats[1].score++;
                            board[cats[1].x + 1][cats[1].y] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[1].x + 1][cats[1].y]) {
                                i++;
                            }
                            m1[i].x = -1;
                            m1[i].y = -1;
                            cats[1].nm1++;

                        }
                        else if(board[cats[1].x + 1][cats[1].y] % 43 == 0){
                            cats[1].score += 2;
                            board[cats[1].x + 1][cats[1].y] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[1].x + 1][cats[1].y]) {
                                i++;
                            }
                            m2[i].x = -30;
                            m2[i].y = -30;
                            cats[1].nm2++;

                        }
                        else if(board[cats[1].x + 1][cats[1].y] % 47 == 0){
                            cats[1].score += 3;
                            board[cats[1].x + 1][cats[1].y] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[1].x + 1][cats[1].y]) {
                                i++;
                            }
                            m3[i].x = -31;
                            m3[i].y = -31;
                            cats[1].nm3++;

                        }
                        else if(board[cats[1].x + 1][cats[1].y] % 53 == 0){
                            cats[1].defence += 2;
                            board[cats[1].x + 1][cats[1].y] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[1].x + 1][cats[1].y]) {
                                i++;
                            }
                            f2[i].x = -18;
                            f2[i].y = -81;
                            cats[1].nf++;

                        }
                        else if(board[cats[1].x + 1][cats[1].y] % 59 == 0){
                            cats[1].defence += 3;
                            board[cats[1].x + 1][cats[1].y] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[1].x + 1][cats[1].y]) {
                                i++;
                            }
                            f3[i].x = -33;
                            f3[i].y = -34;
                            cats[1].nf++;

                        }
                        else if(board[cats[1].x + 1][cats[1].y] % 61 == 0){
                            cats[1].defence += 4;
                            board[cats[1].x + 1][cats[1].y] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[1].x + 1][cats[1].y]) {
                                i++;
                            }
                            f4[i].x = -54;
                            f4[i].y = -91;
                            cats[1].nf++;

                        }
                        else if(board[cats[1].x + 1][cats[1].y] % 67 == 0) {
                            if(cats[1].nm3 > 0) {
                                cats[1].nm3--;
                                cats[1].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[1].nm2 > 0) {
                                cats[1].nm2--;
                                cats[1].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[1].nm1 > 0) {
                                cats[1].nm1--;
                                cats[1].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[1].attack > 1) {
                                cats[1].attack -= 2;
                            }
                            else if(cats[1].defence > 2) {
                                cats[1].defence -= 3;
                            }
                            cats[1].allow = 1;
                        }
                        else if((cats[1].x + 1) == cats[0].x && cats[1].y == cats[0].y) {
                            if (fight_between_cats(cats[1], cats[0]) == 1) {

                                cats[1].score += cats[0].score;
                                cats[1].nm1 += cats[0].nm1;
                                cats[1].nm2 += cats[0].nm2;
                                cats[1].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[1].defence -= (cats[0].attack * (cats[0].defence / cats[1].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 0;

                            }
                            else if(fight_between_cats(cats[1], cats[0]) == 2) {

                                cats[0].score += cats[1].score;
                                cats[0].nm1 += cats[1].nm1;
                                cats[0].nm2 += cats[1].nm2;
                                cats[0].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[0].defence -= (cats[1].attack * (cats[1].defence / cats[0].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if((cats[1].x + 1) == cats[2].x && cats[1].y == cats[2].y) {
                            if (fight_between_cats(cats[1], cats[2]) == 1) {

                                cats[1].score += cats[2].score;
                                cats[1].nm1 += cats[2].nm1;
                                cats[1].nm2 += cats[2].nm2;
                                cats[1].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[1].defence -= (cats[2].attack * (cats[2].defence / cats[1].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 0;

                            }
                            else if(fight_between_cats(cats[0], cats[2]) == 2) {

                                cats[2].score += cats[1].score;
                                cats[2].nm1 += cats[1].nm1;
                                cats[2].nm2 += cats[1].nm2;
                                cats[2].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[2].defence -= (cats[1].attack * (cats[1].defence / cats[2].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if((cats[1].x + 1) == cats[3].x && cats[1].y == cats[3].y) {
                            if (fight_between_cats(cats[1], cats[3]) == 1) {

                                cats[1].score += cats[3].score;
                                cats[1].nm1 += cats[3].nm1;
                                cats[1].nm2 += cats[3].nm2;
                                cats[1].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[1].defence -= (cats[3].attack * (cats[3].defence / cats[1].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 0;

                            }
                            else if(fight_between_cats(cats[1], cats[3]) == 2) {

                                cats[3].score += cats[1].score;
                                cats[3].nm1 += cats[1].nm1;
                                cats[3].nm2 += cats[1].nm2;
                                cats[3].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[3].defence -= (cats[1].attack * (cats[1].defence / cats[3].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x + 1][cats[1].y] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -100;
                                dogs[0].y = -100;

                                cats[1].defence -= (dogs[0].attack * (dogs[0].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[0]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[1].attack * (cats[1].defence / dogs[0].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x + 1][cats[1].y] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -72;
                                dogs[1].y = -23;

                                cats[1].defence -= (dogs[1].attack * (dogs[1].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[1]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[1].attack * (cats[1].defence / dogs[1].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x + 1][cats[1].y] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -120;
                                dogs[2].y = -120;

                                cats[1].defence -= (dogs[2].attack * (dogs[2].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[2]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[1].attack * (cats[1].defence / dogs[2].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }
                        else if(board[cats[1].x + 1][cats[1].y] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[1], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -120;
                                dogs[3].y = -120;

                                cats[1].defence -= (dogs[3].attack * (dogs[3].defence / cats[1].attack));
                                cats[1].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[1], dogs[3]) == 2) {
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;
                                cats[1].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[1].attack * (cats[1].defence / dogs[3].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 1;
                            }
                        }







                        cats[1].x++;
                        if(cats[1].defence > 0)
                            cats[1].defence--;
                        cats[1].allow--;
                    }
                    break;


            }
            if(cats[1].allow == 0)
                cats[1].start = 0;
        }

        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN && cats[2].allow>0 && (cats[2].start > cats[0].start) && (cats[2].start > cats[1].start) && (cats[2].start > cats[3].start)){
            cats[0].turn = ' ';
            cats[1].turn = ' ';
            cats[2].turn = '*';
            cats[3].turn = ' ';


            switch(ev.keyboard.keycode) {

                case ALLEGRO_KEY_ENTER:

                    cats[2].allow = 0;
                    break;



                case ALLEGRO_KEY_ESCAPE:
                    return -1;
                    break;



                case ALLEGRO_KEY_UP:
                    if(cats[2].y > 0 && board[cats[2].x][cats[2].y] % 5 != 0 && board[cats[2].x][cats[2].y] % 13 != 0){
                        if(board[cats[2].x][cats[2].y - 1] % 37 == 0){
                            cats[2].attack++;
                            board[cats[2].x][cats[2].y - 1] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[2].x][cats[2].y - 1]) {
                                i++;
                            }
                            meats[i].x = -12;
                            meats[i].y = -15;

                        }
                        else if(board[cats[2].x][cats[2].y - 1] % 41 == 0){
                            cats[2].score++;
                            board[cats[2].x][cats[2].y - 1] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[2].x][cats[2].y - 1]) {
                                i++;
                            }
                            m1[i].x = -8;
                            m1[i].y = -9;
                            cats[2].nm1++;

                        }
                        else if(board[cats[2].x][cats[2].y - 1] % 43 == 0){
                            cats[2].score += 2;
                            board[cats[2].x][cats[2].y - 1] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[2].x][cats[2].y - 1]) {
                                i++;
                            }
                            m2[i].x = -11;
                            m2[i].y = -13;
                            cats[2].nm2++;

                        }
                        else if(board[cats[2].x][cats[2].y - 1] % 47 == 0){
                            cats[2].score += 3;
                            board[cats[2].x][cats[2].y - 1] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[2].x][cats[2].y - 1]) {
                                i++;
                            }
                            m3[i].x = -17;
                            m3[i].y = -13;
                            cats[2].nm3++;

                        }
                        else if(board[cats[2].x][cats[2].y - 1] % 53 == 0){
                            cats[2].defence += 2;
                            board[cats[2].x][cats[2].y - 1] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[2].x][cats[2].y - 1]) {
                                i++;
                            }
                            f2[i].x = -45;
                            f2[i].y = -78;
                            cats[2].nf++;

                        }
                        else if(board[cats[2].x][cats[2].y - 1] % 59 == 0){
                            cats[2].defence += 3;
                            board[cats[2].x][cats[2].y - 1] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[2].x][cats[2].y - 1]) {
                                i++;
                            }
                            f3[i].x = -24;
                            f3[i].y = -19;
                            cats[2].nf++;

                        }
                        else if(board[cats[2].x][cats[2].y - 1] % 61 == 0){
                            cats[2].defence += 4;
                            board[cats[2].x][cats[2].y - 1] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[2].x][cats[2].y - 1]) {
                                i++;
                            }
                            f4[i].x = -15;
                            f4[i].y = -12;
                            cats[2].nf++;
                        }
                        else if(board[cats[2].x][cats[2].y - 1] % 67 == 0){
                            if(cats[2].nm3 > 0) {
                                cats[2].nm3--;
                                cats[2].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[2].nm2 > 0) {
                                cats[2].nm2--;
                                cats[2].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[2].nm1 > 0) {
                                cats[2].nm1--;
                                cats[2].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[2].attack > 1) {
                                cats[2].attack -= 2;
                            }
                            else if(cats[2].defence > 2) {
                                cats[2].defence -= 3;
                            }
                            cats[2].allow = 1;
                        }
                        else if(cats[2].x == cats[0].x && (cats[2].y - 1) == cats[0].y) {
                            if (fight_between_cats(cats[2], cats[0]) == 1) {

                                cats[2].score += cats[0].score;
                                cats[2].nm1 += cats[0].nm1;
                                cats[2].nm2 += cats[0].nm2;
                                cats[2].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[2].defence -= (cats[0].attack * (cats[0].defence / cats[2].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 0;

                            }
                            else if(fight_between_cats(cats[2], cats[0]) == 2) {

                                cats[0].score += cats[2].score;
                                cats[0].nm1 += cats[2].nm1;
                                cats[0].nm2 += cats[2].nm2;
                                cats[0].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[0].defence -= (cats[2].attack * (cats[2].defence / cats[0].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(cats[2].x == cats[1].x && (cats[2].y - 1) == cats[1].y) {
                            if (fight_between_cats(cats[2], cats[1]) == 1) {

                                cats[2].score += cats[1].score;
                                cats[2].nm1 += cats[1].nm1;
                                cats[2].nm2 += cats[1].nm2;
                                cats[2].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[2].defence -= (cats[1].attack * (cats[1].defence / cats[2].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 0;

                            }
                            else if(fight_between_cats(cats[2], cats[1]) == 2) {

                                cats[1].score += cats[2].score;
                                cats[1].nm1 += cats[2].nm1;
                                cats[1].nm2 += cats[2].nm2;
                                cats[1].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[1].defence -= (cats[2].attack * (cats[2].defence / cats[1].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(cats[2].x == cats[3].x && (cats[2].y - 1) == cats[3].y) {
                            if (fight_between_cats(cats[2], cats[3]) == 1) {

                                cats[2].score += cats[3].score;
                                cats[2].nm1 += cats[3].nm1;
                                cats[2].nm2 += cats[3].nm2;
                                cats[2].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[2].defence -= (cats[3].attack * (cats[3].defence / cats[2].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 0;

                            }
                            else if(fight_between_cats(cats[2], cats[3]) == 2) {

                                cats[3].score += cats[2].score;
                                cats[3].nm1 += cats[2].nm1;
                                cats[3].nm2 += cats[2].nm2;
                                cats[3].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[3].defence -= (cats[2].attack * (cats[2].defence / cats[3].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x][cats[2].y - 1] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -100;
                                dogs[0].y = -100;

                                cats[2].defence -= (dogs[0].attack * (dogs[0].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[0]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[2].attack * (cats[2].defence / dogs[0].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x][cats[2].y - 1] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -110;
                                dogs[1].y = -110;

                                cats[2].defence -= (dogs[1].attack * (dogs[1].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[1]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[2].attack * (cats[2].defence / dogs[1].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x][cats[2].y - 1] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -120;
                                dogs[2].y = -120;

                                cats[2].defence -= (dogs[2].attack * (dogs[2].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[2]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[2].attack * (cats[2].defence / dogs[2].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x][cats[2].y - 1] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -120;
                                dogs[3].y = -120;

                                cats[2].defence -= (dogs[3].attack * (dogs[3].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[3]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[2].attack * (cats[2].defence / dogs[3].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }





                        cats[2].y--;
                        if(cats[2].defence > 0)
                            cats[2].defence--;
                        cats[2].allow--;
                    }
                    break;







                case ALLEGRO_KEY_DOWN:
                    if(cats[2].y < 14 && board[cats[2].x][cats[2].y] % 3 != 0 && board[cats[2].x][cats[2].y] % 17 != 0){
                        if(board[cats[2].x][cats[2].y + 1] % 37 == 0){
                            cats[2].attack++;
                            board[cats[2].x][cats[2].y + 1] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[2].x][cats[2].y + 1]) {
                                i++;
                            }
                            meats[i].x = -1;
                            meats[i].y = -1;

                        }
                        else if(board[cats[2].x][cats[2].y + 1] % 41 == 0){
                            cats[2].score++;
                            board[cats[2].x][cats[2].y + 1] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[2].x][cats[2].y + 1]) {
                                i++;
                            }
                            m1[i].x = -1;
                            m1[i].y = -1;
                            cats[2].nm1++;

                        }
                        else if(board[cats[2].x][cats[2].y + 1] % 43 == 0){
                            cats[2].score += 2;
                            board[cats[2].x][cats[2].y + 1] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[2].x][cats[2].y + 1]) {
                                i++;
                            }
                            m2[i].x = -1;
                            m2[i].y = -1;
                            cats[2].nm2++;

                        }
                        else if(board[cats[2].x][cats[2].y + 1] % 47 == 0){
                            cats[2].score += 3;
                            board[cats[2].x][cats[2].y + 1] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[2].x][cats[2].y + 1]) {
                                i++;
                            }
                            m3[i].x = -1;
                            m3[i].y = -1;
                            cats[2].nm3++;

                        }
                        else if(board[cats[2].x][cats[2].y + 1] % 53 == 0){
                            cats[2].defence += 2;
                            board[cats[2].x][cats[2].y + 1] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[2].x][cats[2].y + 1]) {
                                i++;
                            }
                            f2[i].x = -60;
                            f2[i].y = -37;
                            cats[2].nf++;

                        }
                        else if(board[cats[2].x][cats[2].y + 1] % 59 == 0){
                            cats[2].defence += 3;
                            board[cats[2].x][cats[2].y + 1] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[2].x][cats[2].y + 1]) {
                                i++;
                            }
                            f3[i].x = -99;
                            f3[i].y = -17;
                            cats[2].nf++;

                        }
                        else if(board[cats[2].x][cats[2].y + 1] % 61 == 0){
                            cats[2].defence += 4;
                            board[cats[2].x][cats[2].y + 1] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[2].x][cats[2].y + 1]) {
                                i++;
                            }
                            f4[i].x = -91;
                            f4[i].y = -59;
                            cats[2].nf++;

                        }
                        else if(board[cats[2].x][cats[2].y + 1] % 67 == 0) {
                            if(cats[2].nm3 > 0) {
                                cats[2].nm3--;
                                cats[2].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[2].nm2 > 0) {
                                cats[2].nm2--;
                                cats[2].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[2].nm1 > 0) {
                                cats[2].nm1--;
                                cats[2].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[2].attack > 1) {
                                cats[2].attack -= 2;
                            }
                            else if(cats[2].defence > 2) {
                                cats[2].defence -= 3;
                            }
                            cats[2].allow = 1;
                        }
                        else if(cats[2].x == cats[0].x && (cats[2].y + 1) == cats[0].y) {
                            if (fight_between_cats(cats[2], cats[0]) == 1) {

                                cats[2].score += cats[0].score;
                                cats[2].nm1 += cats[0].nm1;
                                cats[2].nm2 += cats[0].nm2;
                                cats[2].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[2].defence -= (cats[0].attack * (cats[0].defence / cats[2].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 0;

                            }
                            else if(fight_between_cats(cats[2], cats[0]) == 2) {

                                cats[0].score += cats[2].score;
                                cats[0].nm1 += cats[2].nm1;
                                cats[0].nm2 += cats[2].nm2;
                                cats[0].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[0].defence -= (cats[2].attack * (cats[2].defence / cats[0].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(cats[2].x == cats[1].x && (cats[2].y + 1) == cats[1].y) {
                            if (fight_between_cats(cats[2], cats[1]) == 1) {

                                cats[2].score += cats[1].score;
                                cats[2].nm1 += cats[1].nm1;
                                cats[2].nm2 += cats[1].nm2;
                                cats[2].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[2].defence -= (cats[1].attack * (cats[1].defence / cats[2].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 0;

                            }
                            else if(fight_between_cats(cats[2], cats[1]) == 2) {

                                cats[1].score += cats[2].score;
                                cats[1].nm1 += cats[2].nm1;
                                cats[1].nm2 += cats[2].nm2;
                                cats[1].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[1].defence -= (cats[2].attack * (cats[2].defence / cats[1].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(cats[2].x == cats[3].x && (cats[2].y + 1) == cats[3].y) {
                            if (fight_between_cats(cats[2], cats[3]) == 1) {

                                cats[2].score += cats[3].score;
                                cats[2].nm1 += cats[3].nm1;
                                cats[2].nm2 += cats[3].nm2;
                                cats[2].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[2].defence -= (cats[3].attack * (cats[3].defence / cats[2].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 0;

                            }
                            else if(fight_between_cats(cats[2], cats[3]) == 2) {

                                cats[3].score += cats[2].score;
                                cats[3].nm1 += cats[2].nm1;
                                cats[3].nm2 += cats[2].nm2;
                                cats[3].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[3].defence -= (cats[2].attack * (cats[2].defence / cats[3].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x][cats[2].y + 1] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -100;
                                dogs[0].y = -100;

                                cats[2].defence -= (dogs[0].attack * (dogs[0].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[0]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[2].attack * (cats[2].defence / dogs[0].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x][cats[2].y + 1] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -110;
                                dogs[1].y = -110;

                                cats[2].defence -= (dogs[1].attack * (dogs[1].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[1]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[2].attack * (cats[2].defence / dogs[1].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x][cats[2].y + 1] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -63;
                                dogs[2].y = -87;

                                cats[2].defence -= (dogs[2].attack * (dogs[2].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[2]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[2].attack * (cats[2].defence / dogs[2].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x][cats[2].y + 1] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -120;
                                dogs[3].y = -120;

                                cats[2].defence -= (dogs[3].attack * (dogs[3].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[3]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[2].attack * (cats[2].defence / dogs[3].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }








                        cats[2].y++;
                        if(cats[2].defence > 0)
                            cats[2].defence--;
                        cats[2].allow--;
                    }
                    break;






                case ALLEGRO_KEY_LEFT:
                    if(cats[2].x > 0 && board[cats[2].x][cats[2].y] % 11 != 0){
                        if(board[cats[2].x - 1][cats[2].y] % 37 == 0){
                            cats[2].attack++;
                            board[cats[2].x - 1][cats[2].y] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[2].x - 1][cats[2].y]) {
                                i++;
                            }
                            meats[i].x = -1;
                            meats[i].y = -1;

                        }
                        else if(board[cats[2].x - 1][cats[2].y] % 41 == 0){
                            cats[2].score++;
                            board[cats[2].x - 1][cats[2].y] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[2].x - 1][cats[2].y]) {
                                i++;
                            }
                            m1[i].x = -1;
                            m1[i].y = -1;
                            cats[2].nm1++;

                        }
                        else if(board[cats[2].x - 1][cats[2].y] % 43 == 0){
                            cats[2].score += 2;
                            board[cats[2].x - 1][cats[2].y] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[2].x - 1][cats[2].y]) {
                                i++;
                            }
                            m2[i].x = -1;
                            m2[i].y = -1;
                            cats[2].nm2++;

                        }
                        else if(board[cats[2].x - 1][cats[2].y] % 47 == 0){
                            cats[2].score += 3;
                            board[cats[2].x - 1][cats[2].y] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[2].x - 1][cats[2].y]) {
                                i++;
                            }
                            m3[i].x = -1;
                            m3[i].y = -1;
                            cats[2].nm3++;

                        }
                        else if(board[cats[2].x - 1][cats[2].y] % 53 == 0){
                            cats[2].defence += 2;
                            board[cats[2].x - 1][cats[2].y] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[2].x - 1][cats[2].y]) {
                                i++;
                            }
                            f2[i].x = -16;
                            f2[i].y = -17;
                            cats[2].nf++;

                        }
                        else if(board[cats[2].x - 1][cats[2].y] % 59 == 0){
                            cats[2].defence += 3;
                            board[cats[2].x - 1][cats[2].y] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[2].x - 1][cats[2].y]) {
                                i++;
                            }
                            f3[i].x = -11;
                            f3[i].y = -84;
                            cats[2].nf++;

                        }
                        else if(board[cats[2].x - 1][cats[2].y] % 61 == 0){
                            cats[2].defence += 4;
                            board[cats[2].x - 1][cats[2].y] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[2].x - 1][cats[2].y]) {
                                i++;
                            }
                            f4[i].x = -14;
                            f4[i].y = -96;
                            cats[2].nf++;

                        }
                        else if(board[cats[2].x - 1][cats[2].y] % 67 == 0) {
                            if(cats[2].nm3 > 0) {
                                cats[2].nm3--;
                                cats[2].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[2].nm2 > 0) {
                                cats[2].nm2--;
                                cats[2].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[2].nm1 > 0) {
                                cats[2].nm1--;
                                cats[2].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[2].attack > 1) {
                                cats[2].attack -= 2;
                            }
                            else if(cats[2].defence > 2) {
                                cats[2].defence -= 3;
                            }
                            cats[2].allow = 1;
                        }
                        else if((cats[2].x - 1) == cats[0].x && cats[2].y == cats[0].y) {
                            if (fight_between_cats(cats[2], cats[0]) == 1) {

                                cats[2].score += cats[0].score;
                                cats[2].nm1 += cats[0].nm1;
                                cats[2].nm2 += cats[0].nm2;
                                cats[2].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[2].defence -= (cats[0].attack * (cats[0].defence / cats[2].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 0;

                            }
                            else if(fight_between_cats(cats[2], cats[0]) == 2) {

                                cats[0].score += cats[2].score;
                                cats[0].nm1 += cats[2].nm1;
                                cats[0].nm2 += cats[2].nm2;
                                cats[0].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[0].defence -= (cats[2].attack * (cats[2].defence / cats[0].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if((cats[2].x - 1) == cats[1].x && cats[2].y == cats[1].y) {
                            if (fight_between_cats(cats[2], cats[1]) == 1) {

                                cats[2].score += cats[1].score;
                                cats[2].nm1 += cats[1].nm1;
                                cats[2].nm2 += cats[1].nm2;
                                cats[2].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[2].defence -= (cats[1].attack * (cats[1].defence / cats[2].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 0;

                            }
                            else if(fight_between_cats(cats[2], cats[1]) == 2) {

                                cats[1].score += cats[2].score;
                                cats[1].nm1 += cats[2].nm1;
                                cats[1].nm2 += cats[2].nm2;
                                cats[1].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[1].defence -= (cats[2].attack * (cats[2].defence / cats[1].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if((cats[2].x - 1) == cats[3].x && cats[2].y == cats[3].y) {
                            if (fight_between_cats(cats[2], cats[3]) == 1) {

                                cats[2].score += cats[3].score;
                                cats[2].nm1 += cats[3].nm1;
                                cats[2].nm2 += cats[3].nm2;
                                cats[2].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[2].defence -= (cats[3].attack * (cats[3].defence / cats[2].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 0;

                            }
                            else if(fight_between_cats(cats[2], cats[3]) == 2) {

                                cats[3].score += cats[2].score;
                                cats[3].nm1 += cats[2].nm1;
                                cats[3].nm2 += cats[2].nm2;
                                cats[3].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[3].defence -= (cats[2].attack * (cats[2].defence / cats[3].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x - 1][cats[2].y] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -100;
                                dogs[0].y = -100;

                                cats[2].defence -= (dogs[0].attack * (dogs[0].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[0]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[2].attack * (cats[2].defence / dogs[0].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x - 1][cats[2].y] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -110;
                                dogs[1].y = -110;

                                cats[2].defence -= (dogs[1].attack * (dogs[1].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[1]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[2].attack * (cats[2].defence / dogs[1].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x - 1][cats[2].y] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -120;
                                dogs[2].y = -120;

                                cats[2].defence -= (dogs[2].attack * (dogs[2].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[2]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[2].attack * (cats[2].defence / dogs[2].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x - 1][cats[2].y] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -120;
                                dogs[3].y = -120;

                                cats[2].defence -= (dogs[3].attack * (dogs[3].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[3]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[2].attack * (cats[2].defence / dogs[3].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }







                        cats[2].x--;
                        if(cats[2].defence > 0)
                            cats[2].defence--;
                        cats[2].allow--;
                    }
                    break;





                case ALLEGRO_KEY_RIGHT:
                    if(cats[2].x < 14 && board[cats[2].x][cats[2].y] % 7 != 0 && board[cats[2].x][cats[2].y] % 13 != 0 && board[cats[2].x][cats[2].y] % 17 != 0){
                        if(board[cats[2].x + 1][cats[2].y] % 37 == 0){
                            cats[2].attack++;
                            board[cats[2].x + 1][cats[2].y] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[2].x + 1][cats[2].y]) {
                                i++;
                            }
                            meats[i].x = -1;
                            meats[i].y = -1;

                        }
                        else if(board[cats[2].x + 1][cats[2].y] % 41 == 0){
                            cats[2].score++;
                            board[cats[2].x + 1][cats[2].y] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[2].x + 1][cats[2].y]) {
                                i++;
                            }
                            m1[i].x = -1;
                            m1[i].y = -1;
                            cats[2].nm1++;

                        }
                        else if(board[cats[2].x + 1][cats[2].y] % 43 == 0){
                            cats[2].score += 2;
                            board[cats[2].x + 1][cats[2].y] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[2].x + 1][cats[2].y]) {
                                i++;
                            }
                            m2[i].x = -1;
                            m2[i].y = -1;
                            cats[2].nm2++;

                        }
                        else if(board[cats[2].x + 1][cats[2].y] % 47 == 0){
                            cats[2].score += 3;
                            board[cats[2].x + 1][cats[2].y] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[2].x + 1][cats[2].y]) {
                                i++;
                            }
                            m3[i].x = -1;
                            m3[i].y = -1;
                            cats[2].nm3++;

                        }
                        else if(board[cats[2].x + 1][cats[2].y] % 53 == 0){
                            cats[2].defence += 2;
                            board[cats[2].x + 1][cats[2].y] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[2].x + 1][cats[2].y]) {
                                i++;
                            }
                            f2[i].x = -19;
                            f2[i].y = -91;
                            cats[2].nf++;

                        }
                        else if(board[cats[2].x + 1][cats[2].y] % 59 == 0){
                            cats[2].defence += 3;
                            board[cats[2].x + 1][cats[2].y] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[2].x + 1][cats[2].y]) {
                                i++;
                            }
                            f3[i].x = -11;
                            f3[i].y = -88;
                            cats[2].nf++;

                        }
                        else if(board[cats[2].x + 1][cats[2].y] % 61 == 0){
                            cats[2].defence += 4;
                            board[cats[2].x + 1][cats[2].y] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[2].x + 1][cats[2].y]) {
                                i++;
                            }
                            f4[i].x = -15;
                            f4[i].y = -61;
                            cats[2].nf++;

                        }
                        else if(board[cats[2].x + 1][cats[2].y] % 67 == 0) {
                            if(cats[2].nm3 > 0) {
                                cats[2].nm3--;
                                cats[2].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[2].nm2 > 0) {
                                cats[2].nm2--;
                                cats[2].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[2].nm1 > 0) {
                                cats[2].nm1--;
                                cats[2].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[2].attack > 1) {
                                cats[2].attack -= 2;
                            }
                            else if(cats[2].defence > 2) {
                                cats[2].defence -= 3;
                            }
                            cats[2].allow = 1;
                        }
                        else if((cats[2].x + 1) == cats[0].x && cats[2].y == cats[0].y) {
                            if (fight_between_cats(cats[2], cats[0]) == 1) {

                                cats[2].score += cats[0].score;
                                cats[2].nm1 += cats[0].nm1;
                                cats[2].nm2 += cats[0].nm2;
                                cats[2].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[2].defence -= (cats[0].attack * (cats[0].defence / cats[2].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 0;

                            }
                            else if(fight_between_cats(cats[2], cats[0]) == 2) {

                                cats[0].score += cats[2].score;
                                cats[0].nm1 += cats[2].nm1;
                                cats[0].nm2 += cats[2].nm2;
                                cats[0].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[0].defence -= (cats[2].attack * (cats[2].defence / cats[0].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if((cats[2].x + 1) == cats[1].x && cats[2].y == cats[1].y) {
                            if (fight_between_cats(cats[2], cats[1]) == 1) {

                                cats[2].score += cats[1].score;
                                cats[2].nm1 += cats[1].nm1;
                                cats[2].nm2 += cats[1].nm2;
                                cats[2].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[2].defence -= (cats[1].attack * (cats[1].defence / cats[2].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 0;

                            }
                            else if(fight_between_cats(cats[2], cats[1]) == 2) {

                                cats[1].score += cats[2].score;
                                cats[1].nm1 += cats[2].nm1;
                                cats[1].nm2 += cats[2].nm2;
                                cats[1].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[1].defence -= (cats[2].attack * (cats[2].defence / cats[1].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if((cats[2].x + 1) == cats[3].x && cats[2].y == cats[3].y) {
                            if (fight_between_cats(cats[2], cats[3]) == 1) {

                                cats[2].score += cats[3].score;
                                cats[2].nm1 += cats[3].nm1;
                                cats[2].nm2 += cats[3].nm2;
                                cats[2].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[2].defence -= (cats[3].attack * (cats[3].defence / cats[2].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 0;

                            }
                            else if(fight_between_cats(cats[2], cats[3]) == 2) {

                                cats[3].score += cats[2].score;
                                cats[3].nm1 += cats[2].nm1;
                                cats[3].nm2 += cats[2].nm2;
                                cats[3].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[3].defence -= (cats[2].attack * (cats[2].defence / cats[3].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x + 1][cats[2].y] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -100;
                                dogs[0].y = -100;

                                cats[2].defence -= (dogs[0].attack * (dogs[0].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[0]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[2].attack * (cats[2].defence / dogs[0].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x + 1][cats[2].y] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -110;
                                dogs[1].y = -110;

                                cats[2].defence -= (dogs[1].attack * (dogs[1].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[1]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[2].attack * (cats[2].defence / dogs[1].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x + 1][cats[2].y] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -120;
                                dogs[2].y = -120;

                                cats[2].defence -= (dogs[2].attack * (dogs[2].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[2]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[2].attack * (cats[2].defence / dogs[2].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }
                        else if(board[cats[2].x + 1][cats[2].y] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[2], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -120;
                                dogs[3].y = -120;

                                cats[2].defence -= (dogs[3].attack * (dogs[3].defence / cats[2].attack));
                                cats[2].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[2], dogs[3]) == 2) {
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;
                                cats[2].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[2].attack * (cats[2].defence / dogs[3].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 1;
                            }
                        }







                        cats[2].x++;
                        if(cats[2].defence > 0)
                            cats[2].defence--;
                        cats[2].allow--;
                    }
                    break;


            }
            if(cats[2].allow == 0)
                cats[2].start = 0;
        }

        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN && cats[3].allow>0 && (cats[3].start > cats[0].start) && (cats[3].start > cats[1].start) && (cats[3].start > cats[2].start)){

            cats[0].turn = ' ';
            cats[1].turn = ' ';
            cats[2].turn = ' ';
            cats[3].turn = '*';


            switch(ev.keyboard.keycode) {

                case ALLEGRO_KEY_ENTER:

                    cats[3].allow = 0;
                    break;


                case ALLEGRO_KEY_ESCAPE:
                    return -1;
                    break;




                case ALLEGRO_KEY_UP:
                    if(cats[3].y > 0 && board[cats[3].x][cats[3].y] % 5 != 0 && board[cats[3].x][cats[3].y] % 13 != 0){
                        if(board[cats[3].x][cats[3].y - 1] % 37 == 0){
                            cats[3].attack++;
                            board[cats[3].x][cats[3].y - 1] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[3].x][cats[3].y - 1]) {
                                i++;
                            }
                            meats[i].x = -1;
                            meats[i].y = -1;

                        }
                        else if(board[cats[3].x][cats[3].y - 1] % 41 == 0){
                            cats[3].score++;
                            board[cats[3].x][cats[3].y - 1] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[3].x][cats[3].y - 1]) {
                                i++;
                            }
                            m1[i].x = -2;
                            m1[i].y = -2;
                            cats[3].nm1++;

                        }
                        else if(board[cats[3].x][cats[3].y - 1] % 43 == 0){
                            cats[3].score += 2;
                            board[cats[3].x][cats[3].y - 1] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[3].x][cats[3].y - 1]) {
                                i++;
                            }
                            m2[i].x = -1;
                            m2[i].y = -1;
                            cats[3].nm2++;

                        }
                        else if(board[cats[3].x][cats[3].y - 1] % 47 == 0){
                            cats[3].score += 3;
                            board[cats[3].x][cats[3].y - 1] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[3].x][cats[3].y - 1]) {
                                i++;
                            }
                            m3[i].x = -1;
                            m3[i].y = -1;
                            cats[3].nm3++;

                        }
                        else if(board[cats[3].x][cats[3].y - 1] % 53 == 0){
                            cats[3].defence += 2;
                            board[cats[3].x][cats[3].y - 1] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[3].x][cats[3].y - 1]) {
                                i++;
                            }
                            f2[i].x = -14;
                            f2[i].y = -18;
                            cats[3].nf++;

                        }
                        else if(board[cats[3].x][cats[3].y - 1] % 59 == 0){
                            cats[3].defence += 3;
                            board[cats[3].x][cats[3].y - 1] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[3].x][cats[3].y - 1]) {
                                i++;
                            }
                            f3[i].x = -12;
                            f3[i].y = -69;
                            cats[3].nf++;

                        }
                        else if(board[cats[3].x][cats[3].y - 1] % 61 == 0){
                            cats[3].defence += 4;
                            board[cats[3].x][cats[3].y - 1] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[3].x][cats[3].y - 1]) {
                                i++;
                            }
                            f4[i].x = -77;
                            f4[i].y = -63;
                            cats[3].nf++;
                        }
                        else if(board[cats[3].x][cats[3].y - 1] % 67 == 0){
                            if(cats[3].nm3 > 0) {
                                cats[3].nm3--;
                                cats[3].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[3].nm2 > 0) {
                                cats[3].nm2--;
                                cats[3].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[3].nm1 > 0) {
                                cats[3].nm1--;
                                cats[3].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[3].attack > 1) {
                                cats[3].attack -= 2;
                            }
                            else if(cats[3].defence > 2) {
                                cats[3].defence -= 3;
                            }
                            cats[3].allow = 1;
                        }
                        else if(cats[3].x == cats[0].x && (cats[3].y - 1) == cats[0].y) {
                            if (fight_between_cats(cats[3], cats[0]) == 1) {

                                cats[3].score += cats[0].score;
                                cats[3].nm1 += cats[0].nm1;
                                cats[3].nm2 += cats[0].nm2;
                                cats[3].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[3].defence -= (cats[0].attack * (cats[0].defence / cats[3].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 0;

                            }
                            else if(fight_between_cats(cats[3], cats[0]) == 2) {

                                cats[0].score += cats[3].score;
                                cats[0].nm1 += cats[3].nm1;
                                cats[0].nm2 += cats[3].nm2;
                                cats[0].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[0].defence -= (cats[3].attack * (cats[3].defence / cats[0].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(cats[3].x == cats[1].x && (cats[3].y - 1) == cats[1].y) {
                            if (fight_between_cats(cats[3], cats[1]) == 1) {

                                cats[3].score += cats[1].score;
                                cats[3].nm1 += cats[1].nm1;
                                cats[3].nm2 += cats[1].nm2;
                                cats[3].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[3].defence -= (cats[1].attack * (cats[1].defence / cats[3].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 0;

                            }
                            else if(fight_between_cats(cats[3], cats[1]) == 2) {

                                cats[1].score += cats[3].score;
                                cats[1].nm1 += cats[3].nm1;
                                cats[1].nm2 += cats[3].nm2;
                                cats[1].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[1].defence -= (cats[3].attack * (cats[3].defence / cats[1].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(cats[3].x == cats[2].x && (cats[3].y - 1) == cats[2].y) {
                            if (fight_between_cats(cats[3], cats[2]) == 1) {

                                cats[3].score += cats[2].score;
                                cats[3].nm1 += cats[2].nm1;
                                cats[3].nm2 += cats[2].nm2;
                                cats[3].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[3].defence -= (cats[2].attack * (cats[2].defence / cats[3].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 0;

                            }
                            else if(fight_between_cats(cats[3], cats[2]) == 2) {

                                cats[2].score += cats[3].score;
                                cats[2].nm1 += cats[3].nm1;
                                cats[2].nm2 += cats[3].nm2;
                                cats[2].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[2].defence -= (cats[3].attack * (cats[3].defence / cats[2].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x][cats[3].y - 1] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -100;
                                dogs[0].y = -100;

                                cats[3].defence -= (dogs[0].attack * (dogs[0].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[0]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[3].attack * (cats[3].defence / dogs[0].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x][cats[3].y - 1] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -110;
                                dogs[1].y = -110;

                                cats[3].defence -= (dogs[1].attack * (dogs[1].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[1]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[3].attack * (cats[3].defence / dogs[1].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x][cats[3].y - 1] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -120;
                                dogs[2].y = -120;

                                cats[3].defence -= (dogs[2].attack * (dogs[2].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[2]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[3].attack * (cats[3].defence / dogs[2].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x][cats[3].y - 1] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -120;
                                dogs[3].y = -120;

                                cats[3].defence -= (dogs[3].attack * (dogs[3].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[3]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[3].attack * (cats[3].defence / dogs[3].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }







                        cats[3].y--;
                        if(cats[3].defence > 0)
                            cats[3].defence--;
                        cats[3].allow--;
                    }
                    break;







                case ALLEGRO_KEY_DOWN:
                    if(cats[3].y < 14 && board[cats[3].x][cats[3].y] % 3 != 0 && board[cats[3].x][cats[3].y] % 17 != 0){
                        if(board[cats[3].x][cats[3].y + 1] % 37 == 0){
                            cats[3].attack++;
                            board[cats[3].x][cats[3].y + 1] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[3].x][cats[3].y + 1]) {
                                i++;
                            }
                            meats[i].x = -1;
                            meats[i].y = -1;

                        }
                        else if(board[cats[3].x][cats[3].y + 1] % 41 == 0){
                            cats[3].score++;
                            board[cats[3].x][cats[3].y + 1] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[3].x][cats[3].y + 1]) {
                                i++;
                            }
                            m1[i].x = -1;
                            m1[i].y = -1;
                            cats[3].nm1++;

                        }
                        else if(board[cats[3].x][cats[3].y + 1] % 43 == 0){
                            cats[3].score += 2;
                            board[cats[3].x][cats[3].y + 1] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[3].x][cats[3].y + 1]) {
                                i++;
                            }
                            m2[i].x = -1;
                            m2[i].y = -1;
                            cats[3].nm2++;

                        }
                        else if(board[cats[3].x][cats[3].y + 1] % 47 == 0){
                            cats[3].score += 3;
                            board[cats[3].x][cats[3].y + 1] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[3].x][cats[3].y + 1]) {
                                i++;
                            }
                            m3[i].x = -1;
                            m3[i].y = -1;
                            cats[3].nm3++;

                        }
                        else if(board[cats[3].x][cats[3].y + 1] % 53 == 0){
                            cats[3].defence += 2;
                            board[cats[3].x][cats[3].y + 1] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[3].x][cats[3].y + 1]) {
                                i++;
                            }
                            f2[i].x = -62;
                            f2[i].y = -85;
                            cats[3].nf++;

                        }
                        else if(board[cats[3].x][cats[3].y + 1] % 59 == 0){
                            cats[3].defence += 3;
                            board[cats[3].x][cats[3].y + 1] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[3].x][cats[3].y + 1]) {
                                i++;
                            }
                            f3[i].x = -13;
                            f3[i].y = -31;
                            cats[3].nf++;

                        }
                        else if(board[cats[3].x][cats[3].y + 1] % 61 == 0){
                            cats[3].defence += 4;
                            board[cats[3].x][cats[3].y + 1] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[3].x][cats[3].y + 1]) {
                                i++;
                            }
                            f4[i].x = -50;
                            f4[i].y = -57;
                            cats[3].nf++;

                        }
                        else if(board[cats[3].x][cats[3].y + 1] % 67 == 0) {
                            if(cats[3].nm3 > 0) {
                                cats[3].nm3--;
                                cats[3].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[3].nm2 > 0) {
                                cats[3].nm2--;
                                cats[3].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[3].nm1 > 0) {
                                cats[3].nm1--;
                                cats[3].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[3].attack > 1) {
                                cats[3].attack -= 2;
                            }
                            else if(cats[3].defence > 2) {
                                cats[3].defence -= 3;
                            }
                            cats[3].allow = 1;
                        }
                        else if(cats[3].x == cats[0].x && (cats[3].y + 1) == cats[0].y) {
                            if (fight_between_cats(cats[3], cats[0]) == 1) {

                                cats[3].score += cats[0].score;
                                cats[3].nm1 += cats[0].nm1;
                                cats[3].nm2 += cats[0].nm2;
                                cats[3].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[3].defence -= (cats[0].attack * (cats[0].defence / cats[3].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 0;

                            }
                            else if(fight_between_cats(cats[3], cats[0]) == 2) {

                                cats[0].score += cats[3].score;
                                cats[0].nm1 += cats[3].nm1;
                                cats[0].nm2 += cats[3].nm2;
                                cats[0].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[0].defence -= (cats[3].attack * (cats[3].defence / cats[0].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(cats[3].x == cats[1].x && (cats[3].y + 1) == cats[1].y) {
                            if (fight_between_cats(cats[3], cats[1]) == 1) {

                                cats[3].score += cats[1].score;
                                cats[3].nm1 += cats[1].nm1;
                                cats[3].nm2 += cats[1].nm2;
                                cats[3].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[3].defence -= (cats[1].attack * (cats[1].defence / cats[3].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 0;

                            }
                            else if(fight_between_cats(cats[3], cats[1]) == 2) {

                                cats[1].score += cats[3].score;
                                cats[1].nm1 += cats[3].nm1;
                                cats[1].nm2 += cats[3].nm2;
                                cats[1].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[1].defence -= (cats[3].attack * (cats[3].defence / cats[1].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(cats[3].x == cats[2].x && (cats[3].y + 1) == cats[2].y) {
                            if (fight_between_cats(cats[3], cats[2]) == 1) {

                                cats[3].score += cats[2].score;
                                cats[3].nm1 += cats[2].nm1;
                                cats[3].nm2 += cats[2].nm2;
                                cats[3].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[3].defence -= (cats[2].attack * (cats[2].defence / cats[3].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 0;

                            }
                            else if(fight_between_cats(cats[3], cats[2]) == 2) {

                                cats[2].score += cats[3].score;
                                cats[2].nm1 += cats[3].nm1;
                                cats[2].nm2 += cats[3].nm2;
                                cats[2].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[2].defence -= (cats[3].attack * (cats[3].defence / cats[2].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x][cats[3].y + 1] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -100;
                                dogs[0].y = -100;

                                cats[3].defence -= (dogs[0].attack * (dogs[0].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[0]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[3].attack * (cats[3].defence / dogs[0].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x][cats[3].y + 1] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -110;
                                dogs[1].y = -110;

                                cats[3].defence -= (dogs[1].attack * (dogs[1].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[1]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[3].attack * (cats[3].defence / dogs[1].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x][cats[3].y + 1] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -120;
                                dogs[2].y = -120;

                                cats[3].defence -= (dogs[2].attack * (dogs[2].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[2]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[3].attack * (cats[3].defence / dogs[2].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x][cats[3].y + 1] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -120;
                                dogs[3].y = -120;

                                cats[3].defence -= (dogs[3].attack * (dogs[3].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[3]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[3].attack * (cats[3].defence / dogs[3].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }








                        cats[3].y++;
                        if(cats[3].defence > 0)
                            cats[3].defence--;
                        cats[3].allow--;
                    }
                    break;






                case ALLEGRO_KEY_LEFT:
                    if(cats[3].x > 0 && board[cats[3].x][cats[3].y] % 11 != 0){
                        if(board[cats[3].x - 1][cats[3].y] % 37 == 0){
                            cats[3].attack++;
                            board[cats[3].x - 1][cats[3].y] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[3].x - 1][cats[3].y]) {
                                i++;
                            }
                            meats[i].x = -1;
                            meats[i].y = -1;

                        }
                        else if(board[cats[3].x - 1][cats[3].y] % 41 == 0){
                            cats[3].score++;
                            board[cats[3].x - 1][cats[3].y] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[3].x - 1][cats[3].y]) {
                                i++;
                            }
                            m1[i].x = -1;
                            m1[i].y = -1;
                            cats[3].nm1++;

                        }
                        else if(board[cats[3].x - 1][cats[3].y] % 43 == 0){
                            cats[3].score += 2;
                            board[cats[3].x - 1][cats[3].y] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[3].x - 1][cats[3].y]) {
                                i++;
                            }
                            m2[i].x = -1;
                            m2[i].y = -1;
                            cats[3].nm2++;

                        }
                        else if(board[cats[3].x - 1][cats[3].y] % 47 == 0){
                            cats[3].score += 3;
                            board[cats[3].x - 1][cats[3].y] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[3].x - 1][cats[3].y]) {
                                i++;
                            }
                            m3[i].x = -1;
                            m3[i].y = -1;
                            cats[3].nm3++;

                        }
                        else if(board[cats[3].x - 1][cats[3].y] % 53 == 0){
                            cats[3].defence += 2;
                            board[cats[3].x - 1][cats[3].y] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[3].x - 1][cats[3].y]) {
                                i++;
                            }
                            f2[i].x = -14;
                            f2[i].y = -61;
                            cats[3].nf++;

                        }
                        else if(board[cats[3].x - 1][cats[3].y] % 59 == 0){
                            cats[3].defence += 3;
                            board[cats[3].x - 1][cats[3].y] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[3].x - 1][cats[3].y]) {
                                i++;
                            }
                            f3[i].x = -12;
                            f3[i].y = -77;
                            cats[3].nf++;

                        }
                        else if(board[cats[3].x - 1][cats[3].y] % 61 == 0){
                            cats[3].defence += 4;
                            board[cats[3].x - 1][cats[3].y] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[3].x - 1][cats[3].y]) {
                                i++;
                            }
                            f4[i].x = -44;
                            f4[i].y = -97;
                            cats[3].nf++;

                        }
                        else if(board[cats[3].x - 1][cats[3].y] % 67 == 0) {
                            if(cats[3].nm3 > 0) {
                                cats[3].nm3--;
                                cats[3].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[3].nm2 > 0) {
                                cats[3].nm2--;
                                cats[3].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[3].nm1 > 0) {
                                cats[3].nm1--;
                                cats[3].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[3].attack > 1) {
                                cats[3].attack -= 2;
                            }
                            else if(cats[3].defence > 2) {
                                cats[3].defence -= 3;
                            }
                            cats[3].allow = 1;
                        }
                        else if((cats[3].x - 1) == cats[0].x && cats[3].y == cats[0].y) {
                            if (fight_between_cats(cats[3], cats[0]) == 1) {

                                cats[3].score += cats[0].score;
                                cats[3].nm1 += cats[0].nm1;
                                cats[3].nm2 += cats[0].nm2;
                                cats[3].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[3].defence -= (cats[0].attack * (cats[0].defence / cats[3].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 0;

                            }
                            else if(fight_between_cats(cats[3], cats[0]) == 2) {

                                cats[0].score += cats[3].score;
                                cats[0].nm1 += cats[3].nm1;
                                cats[0].nm2 += cats[3].nm2;
                                cats[0].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[0].defence -= (cats[3].attack * (cats[3].defence / cats[0].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if((cats[3].x - 1) == cats[1].x && cats[3].y == cats[1].y) {
                            if (fight_between_cats(cats[3], cats[1]) == 1) {

                                cats[3].score += cats[1].score;
                                cats[3].nm1 += cats[1].nm1;
                                cats[3].nm2 += cats[1].nm2;
                                cats[3].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[3].defence -= (cats[1].attack * (cats[1].defence / cats[3].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 0;

                            }
                            else if(fight_between_cats(cats[3], cats[1]) == 2) {

                                cats[1].score += cats[3].score;
                                cats[1].nm1 += cats[3].nm1;
                                cats[1].nm2 += cats[3].nm2;
                                cats[1].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[1].defence -= (cats[3].attack * (cats[3].defence / cats[1].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if((cats[3].x - 1) == cats[2].x && cats[3].y == cats[2].y) {
                            if (fight_between_cats(cats[3], cats[2]) == 1) {

                                cats[3].score += cats[2].score;
                                cats[3].nm1 += cats[2].nm1;
                                cats[3].nm2 += cats[2].nm2;
                                cats[3].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[3].defence -= (cats[2].attack * (cats[2].defence / cats[3].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 0;

                            }
                            else if(fight_between_cats(cats[3], cats[2]) == 2) {

                                cats[2].score += cats[3].score;
                                cats[2].nm1 += cats[3].nm1;
                                cats[2].nm2 += cats[3].nm2;
                                cats[2].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[2].defence -= (cats[3].attack * (cats[3].defence / cats[2].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x - 1][cats[3].y] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -100;
                                dogs[0].y = -100;

                                cats[3].defence -= (dogs[0].attack * (dogs[0].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[0]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[3].attack * (cats[3].defence / dogs[0].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x - 1][cats[3].y] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -110;
                                dogs[1].y = -110;

                                cats[3].defence -= (dogs[1].attack * (dogs[1].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[1]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[3].attack * (cats[3].defence / dogs[1].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x - 1][cats[3].y] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -120;
                                dogs[2].y = -120;

                                cats[3].defence -= (dogs[2].attack * (dogs[2].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[2]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[3].attack * (cats[3].defence / dogs[2].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x - 1][cats[3].y] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -120;
                                dogs[3].y = -120;

                                cats[3].defence -= (dogs[3].attack * (dogs[3].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[3]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[3].attack * (cats[3].defence / dogs[3].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }







                        cats[3].x--;
                        if(cats[3].defence > 0)
                            cats[3].defence--;
                        cats[3].allow--;
                    }
                    break;





                case ALLEGRO_KEY_RIGHT:
                    if(cats[3].x < 14 && board[cats[3].x][cats[3].y] % 7 != 0 && board[cats[3].x][cats[3].y] % 13 != 0 && board[cats[3].x][cats[3].y] % 17 != 0){
                        if(board[cats[3].x + 1][cats[3].y] % 37 == 0){
                            cats[3].attack++;
                            board[cats[3].x + 1][cats[3].y] /= 37;
                            int i=0;
                            while(board[meats[i].x][meats[i].y] != board[cats[3].x + 1][cats[3].y]) {
                                i++;
                            }
                            meats[i].x = -1;
                            meats[i].y = -1;

                        }
                        else if(board[cats[3].x + 1][cats[3].y] % 41 == 0){
                            cats[3].score++;
                            board[cats[3].x + 1][cats[3].y] /= 41;
                            int i=0;
                            while(board[m1[i].x][m1[i].y] != board[cats[3].x + 1][cats[3].y]) {
                                i++;
                            }
                            m1[i].x = -1;
                            m1[i].y = -1;
                            cats[3].nm1++;

                        }
                        else if(board[cats[3].x + 1][cats[3].y] % 43 == 0){
                            cats[3].score += 2;
                            board[cats[3].x + 1][cats[3].y] /= 43;
                            int i=0;
                            while(board[m2[i].x][m2[i].y] != board[cats[3].x + 1][cats[3].y]) {
                                i++;
                            }
                            m2[i].x = -1;
                            m2[i].y = -1;
                            cats[3].nm2++;

                        }
                        else if(board[cats[3].x + 1][cats[3].y] % 47 == 0){
                            cats[3].score += 3;
                            board[cats[3].x + 1][cats[3].y] /= 47;
                            int i=0;
                            while(board[m3[i].x][m3[i].y] != board[cats[3].x + 1][cats[3].y]) {
                                i++;
                            }
                            m3[i].x = -1;
                            m3[i].y = -1;
                            cats[3].nm3++;

                        }
                        else if(board[cats[3].x + 1][cats[3].y] % 53 == 0){
                            cats[3].defence += 2;
                            board[cats[3].x + 1][cats[3].y] /= 53;
                            int i=0;
                            while(board[f2[i].x][f2[i].y] != board[cats[3].x + 1][cats[3].y]) {
                                i++;
                            }
                            f2[i].x = -89;
                            f2[i].y = -20;
                            cats[3].nf++;

                        }
                        else if(board[cats[3].x + 1][cats[3].y] % 59 == 0){
                            cats[3].defence += 3;
                            board[cats[3].x + 1][cats[3].y] /= 59;
                            int i=0;
                            while(board[f3[i].x][f3[i].y] != board[cats[3].x + 1][cats[3].y]) {
                                i++;
                            }
                            f3[i].x = -44;
                            f3[i].y = -90;
                            cats[3].nf++;

                        }
                        else if(board[cats[3].x + 1][cats[3].y] % 61 == 0){
                            cats[3].defence += 4;
                            board[cats[3].x + 1][cats[3].y] /= 61;
                            int i=0;
                            while(board[f4[i].x][f4[i].y] != board[cats[3].x + 1][cats[3].y]) {
                                i++;
                            }
                            f4[i].x = -70;
                            f4[i].y = -31;
                            cats[3].nf++;

                        }
                        else if(board[cats[3].x + 1][cats[3].y] % 67 == 0) {
                            if(cats[3].nm3 > 0) {
                                cats[3].nm3--;
                                cats[3].score -= 3;
                                for(int i=0; i<4; i++) {
                                    board[m3[i].x][m3[i].y] /= 47;
                                }
                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);
                            }
                            else if(cats[3].nm2 > 0) {
                                cats[3].nm2--;
                                cats[3].score -= 2;
                                for(int i=0; i<6; i++) {
                                    board[m2[i].x][m2[i].y] /= 43;
                                }
                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);
                            }
                            else if(cats[3].nm1 > 0) {
                                cats[3].nm1--;
                                cats[3].score -= 1;
                                for(int i=0; i<8; i++) {
                                    board[m1[i].x][m1[i].y] /= 41;
                                }
                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);
                            }
                            else if(cats[3].attack > 1) {
                                cats[3].attack -= 2;
                            }
                            else if(cats[3].defence > 2) {
                                cats[3].defence -= 3;
                            }
                            cats[3].allow = 1;
                        }
                        else if((cats[3].x + 1) == cats[0].x && cats[3].y == cats[0].y) {
                            if (fight_between_cats(cats[3], cats[0]) == 1) {

                                cats[3].score += cats[0].score;
                                cats[3].nm1 += cats[0].nm1;
                                cats[3].nm2 += cats[0].nm2;
                                cats[3].nm3 += cats[0].nm3;

                                cats[0].score = 0;
                                cats[0].nm1 = 0;
                                cats[0].nm2 = 0;
                                cats[0].nm3 = 0;

                                cats[3].defence -= (cats[0].attack * (cats[0].defence / cats[3].attack));
                                cats[0].attack = 2;
                                cats[0].defence = 5;
                                cats[0].allow = 0;

                            }
                            else if(fight_between_cats(cats[3], cats[0]) == 2) {

                                cats[0].score += cats[3].score;
                                cats[0].nm1 += cats[3].nm1;
                                cats[0].nm2 += cats[3].nm2;
                                cats[0].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[0].defence -= (cats[3].attack * (cats[3].defence / cats[0].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if((cats[3].x + 1) == cats[1].x && cats[3].y == cats[1].y) {
                            if (fight_between_cats(cats[3], cats[1]) == 1) {

                                cats[3].score += cats[1].score;
                                cats[3].nm1 += cats[1].nm1;
                                cats[3].nm2 += cats[1].nm2;
                                cats[3].nm3 += cats[1].nm3;

                                cats[1].score = 0;
                                cats[1].nm1 = 0;
                                cats[1].nm2 = 0;
                                cats[1].nm3 = 0;

                                cats[3].defence -= (cats[1].attack * (cats[1].defence / cats[3].attack));
                                cats[1].attack = 2;
                                cats[1].defence = 5;
                                cats[1].allow = 0;

                            }
                            else if(fight_between_cats(cats[3], cats[1]) == 2) {

                                cats[1].score += cats[3].score;
                                cats[1].nm1 += cats[3].nm1;
                                cats[1].nm2 += cats[3].nm2;
                                cats[1].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[1].defence -= (cats[3].attack * (cats[3].defence / cats[1].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if((cats[3].x + 1) == cats[2].x && cats[3].y == cats[2].y) {
                            if (fight_between_cats(cats[3], cats[2]) == 1) {

                                cats[3].score += cats[2].score;
                                cats[3].nm1 += cats[2].nm1;
                                cats[3].nm2 += cats[2].nm2;
                                cats[3].nm3 += cats[2].nm3;

                                cats[2].score = 0;
                                cats[2].nm1 = 0;
                                cats[2].nm2 = 0;
                                cats[2].nm3 = 0;

                                cats[3].defence -= (cats[2].attack * (cats[2].defence / cats[3].attack));
                                cats[2].attack = 2;
                                cats[2].defence = 5;
                                cats[2].allow = 0;

                            }
                            else if(fight_between_cats(cats[3], cats[2]) == 2) {

                                cats[2].score += cats[3].score;
                                cats[2].nm1 += cats[3].nm1;
                                cats[2].nm2 += cats[3].nm2;
                                cats[2].nm3 += cats[3].nm3;

                                cats[3].score = 0;
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;

                                cats[2].defence -= (cats[3].attack * (cats[3].defence / cats[2].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x + 1][cats[3].y] % 19 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[0]) == 1) {
                                board[dogs[0].x][dogs[0].y] /= 19;
                                dogs[0].x = -100;
                                dogs[0].y = -100;

                                cats[3].defence -= (dogs[0].attack * (dogs[0].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[0]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[0].defence -= (cats[3].attack * (cats[3].defence / dogs[0].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x + 1][cats[3].y] % 23 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[1]) == 1) {
                                board[dogs[1].x][dogs[1].y] /= 23;
                                dogs[1].x = -110;
                                dogs[1].y = -110;

                                cats[3].defence -= (dogs[1].attack * (dogs[1].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[1]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[1].defence -= (cats[3].attack * (cats[3].defence / dogs[1].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x + 1][cats[3].y] % 29 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[2]) == 1) {
                                board[dogs[2].x][dogs[2].y] /= 29;
                                dogs[2].x = -120;
                                dogs[2].y = -120;

                                cats[3].defence -= (dogs[2].attack * (dogs[2].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[2]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[2].defence -= (cats[3].attack * (cats[3].defence / dogs[2].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }
                        else if(board[cats[3].x + 1][cats[3].y] % 31 == 0) {
                            if(fight_between_cats_dogs(cats[3], dogs[3]) == 1) {
                                board[dogs[3].x][dogs[3].y] /= 31;
                                dogs[3].x = -120;
                                dogs[3].y = -120;

                                cats[3].defence -= (dogs[3].attack * (dogs[3].defence / cats[3].attack));
                                cats[3].allow = 1;
                            }
                            else if(fight_between_cats_dogs(cats[3], dogs[3]) == 2) {
                                cats[3].nm1 = 0;
                                cats[3].nm2 = 0;
                                cats[3].nm3 = 0;
                                cats[3].score = 0;

                                jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                                draw_mouse1(m1, board, jm1);

                                jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                                draw_mouse2(m2, board, jm2);

                                jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                                draw_mouse3(m3, board, jm3);


                                dogs[3].defence -= (cats[3].attack * (cats[3].defence / dogs[3].attack));
                                cats[3].attack = 2;
                                cats[3].defence = 5;
                                cats[3].allow = 1;
                            }
                        }







                        cats[3].x++;
                        if(cats[3].defence > 0)
                            cats[3].defence--;
                        cats[3].allow--;
                    }
                    break;


            }
            if(cats[3].allow == 0)
                cats[3].start = 0;
        }


        else if(cats[0].allow == 0 && cats[1].allow == 0 && cats[2].allow == 0 && cats[3].allow == 0){

            cats[0].allow = 3;
            cats[0].defence++;

            cats[1].allow = 3;
            cats[1].defence++;

            cats[2].allow = 3;
            cats[2].defence++;

            cats[3].allow = 3;
            cats[3].defence++;


            counter++;

            move_bulldog(dogs, board);
            move_pitbull(dogs, board);
            move_shepherd(dogs, board);
            move_bulldog_J(dogs, board);
            move_mouse1(m1, board);
            move_mouse2(m2, board);
            move_mouse3(m3, board);


            for(int j=0; j<4; j++) {
                for (int i=0; i<4; i++) {
                    if (dogs[j].x == cats[i].y && dogs[j].y == cats[i].y) {
                        if (fight_between_cats_dogs(cats[i], dogs[j]) == 1) {

                            if(j == 0)
                                board[dogs[j].x][dogs[j].y] /= 19;
                            else if(j == 1)
                                board[dogs[j].x][dogs[j].y] /= 23;
                            else if(j == 2)
                                board[dogs[j].x][dogs[j].y] /= 29;
                            else if(j == 3)
                                board[dogs[j].x][dogs[j].y] /= 31;

                            dogs[j].x = -600;
                            dogs[j].y = -600;

                            cats[i].defence -= (dogs[j].attack * (dogs[j].defence / cats[i].attack));
                        }
                        else if (fight_between_cats_dogs(cats[i], dogs[j]) == 2) {
                            cats[i].nm1 = 0;
                            cats[i].nm2 = 0;
                            cats[i].nm3 = 0;
                            cats[i].score = 0;

                            jm1 = 8 - (cats[0].nm1 + cats[1].nm1 + cats[2].nm1 + cats[3].nm1);
                            draw_mouse1(m1, board, jm1);

                            jm2 = 6 - (cats[0].nm2 + cats[1].nm2 + cats[2].nm2 + cats[3].nm2);
                            draw_mouse2(m2, board, jm2);

                            jm3 = 4 - (cats[0].nm3 + cats[1].nm3 + cats[2].nm3 + cats[3].nm3);
                            draw_mouse3(m3, board, jm3);


                            dogs[j].defence -= (cats[i].attack * (cats[i].defence / dogs[j].attack));
                            cats[i].attack = 2;
                            cats[i].defence = 5;

                        }
                    }
                }
            }






            cats[0].start = A[0];
            cats[1].start = A[1];
            cats[2].start = A[2];
            cats[3].start = A[3];



            if((cats[0].nf + cats[1].nf + cats[2].nf + cats[3].nf) > 6) {

                draw_fish2(f2, board, 4);
                draw_fish3(f3, board, 4);
                draw_fish4(f4, board, 2);

                cats[0].nf = 0;
                cats[1].nf = 0;
                cats[2].nf = 0;
                cats[3].nf = 0;

            }

        }




        al_clear_to_color(al_map_rgb(173, 121, 255));
        drawBoard(cats);



        draw_scale_bitmap_bulldog(bulldog, dogs[0]);
        draw_scale_bitmap_pitbull(pitbull, dogs[1]);
        draw_scale_bitmap_shepherd(shepherd, dogs[2]);
        draw_scale_bitmap_bulldog_J(bulldog_J, dogs[3]);


        // for mouse 1
        for(int i=0; i<jm1; i++){
            draw_scale_bitmap_mouse(mouse1, m1[i]);
        }

        // for mouse 2
        for(int i=0; i<jm2; i++){
            draw_scale_bitmap_mouse(mouse2, m2[i]);
        }


        // for mouse 3
        for(int i=0; i<jm3; i++){
            draw_scale_bitmap_mouse(mouse3, m3[i]);
        }


        // for fish2
        for(int i=0; i<4; i++){
            draw_scale_bitmap_fish(fish, f2[i]);
        }

        // for fish3
        for(int i=0; i<4; i++){
            draw_scale_bitmap_fish(fish, f3[i]);
        }

        // for fish4
        for(int i=0; i<2; i++){
            draw_scale_bitmap_fish(fish, f4[i]);
        }


        // for meat
        for(int i=0; i<8; i++){
            draw_scale_bitmap_meat(meat, meats[i]);
        }


        // for cats
        draw_scale_bitmap_cat1(cat1,cats[0]);
        draw_scale_bitmap_cat2(cat2, cats[1]);
        draw_scale_bitmap_cat3(cat3,cats[2]);
        draw_scale_bitmap_cat4(cat4, cats[3]);


        // for home
        int originalWidth_H = al_get_bitmap_width(home);
        int originalHeight_H = al_get_bitmap_height(home);
        int newWidth_H = CELL_SIZE;
        int newHeight_H = CELL_SIZE;
        int homeX = 7 * CELL_SIZE;
        int homeY = 7 * CELL_SIZE;
        al_draw_scaled_bitmap(home, 0, 0, originalWidth_H, originalHeight_H, homeX, homeY, newWidth_H, newHeight_H, 0);


        // for display traps
        for(int i=0; i<4; i++) {
            if (cats[i].x == traps[0].x && cats[i].y == traps[0].y) {
                draw_scale_bitmap_trap(trap, traps[0]);
            }
            else if (cats[i].x == traps[1].x && cats[i].y == traps[1].y) {
                draw_scale_bitmap_trap(trap, traps[1]);
            }
            else if (cats[i].x == traps[2].x && cats[i].y == traps[2].y) {
                draw_scale_bitmap_trap(trap, traps[2]);
            }
            else if (cats[i].x == traps[3].x && cats[i].y == traps[3].y) {
                draw_scale_bitmap_trap(trap, traps[3]);
            }
            else if (cats[i].x == traps[4].x && cats[i].y == traps[4].y) {
                draw_scale_bitmap_trap(trap, traps[4]);
            }
            else if (cats[i].x == traps[5].x && cats[i].y == traps[5].y) {
                draw_scale_bitmap_trap(trap, traps[5]);
            }
            else if (cats[i].x == traps[6].x && cats[i].y == traps[6].y) {
                draw_scale_bitmap_trap(trap, traps[6]);
            }
            else if (cats[i].x == traps[7].x && cats[i].y == traps[7].y) {
                draw_scale_bitmap_trap(trap, traps[7]);
            }
        }

        al_flip_display();

    }



    while (endGame == 1) {

        int originalWidth_E = al_get_bitmap_width(finish);
        int originalHeight_E = al_get_bitmap_height(finish);
        int newWidth_E = BOARD_WIDTH + SCOREBOARD_WIDTH;
        int newHeight_E = BOARD_HEIGHT;
        int EX = 0;
        int EY = 0;

        al_draw_scaled_bitmap(finish, 0, 0, originalWidth_E, originalHeight_E, EX, EY, newWidth_E, newHeight_E, 0);


        al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH / 2, BOARD_HEIGHT / 40, ALLEGRO_ALIGN_CENTER, "Enter 'R' for see result of game");
        al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH / 2, BOARD_HEIGHT / 1.25, ALLEGRO_ALIGN_CENTER, "Enter 'escape' for exit from game");
        al_flip_display();



        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_R:
                    endGame = 0;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    return -1;
            }
        }
    }



    while (seeResult == 1) {

        selectionSort(cats, 4, B);

        al_clear_to_color(al_map_rgb(173, 121, 255));

        al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH / 2, BOARD_HEIGHT / 5, ALLEGRO_ALIGN_CENTER, "First person:");
        al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH / 2, BOARD_HEIGHT / 3.5, ALLEGRO_ALIGN_CENTER, "Second person:");
        al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH / 2, BOARD_HEIGHT / 2.5, ALLEGRO_ALIGN_CENTER, "Third person:");
        al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH / 2, BOARD_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Fourth person:");

        al_draw_textf(font, al_map_rgb(0, 0, 0), BOARD_WIDTH / 1, BOARD_HEIGHT / 5, ALLEGRO_ALIGN_CENTER, "  %d", B[0]);
        al_draw_textf(font, al_map_rgb(0, 0, 0), BOARD_WIDTH / 1, BOARD_HEIGHT / 3.5, ALLEGRO_ALIGN_CENTER, "  %d", B[1]);
        al_draw_textf(font, al_map_rgb(0, 0, 0), BOARD_WIDTH / 1, BOARD_HEIGHT / 2.5, ALLEGRO_ALIGN_CENTER, "  %d", B[2]);
        al_draw_textf(font, al_map_rgb(0, 0, 0), BOARD_WIDTH / 1, BOARD_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "  %d", B[3]);

        al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH / 1.75, BOARD_HEIGHT / 1.45, ALLEGRO_ALIGN_CENTER, "Thanks for Attending");
        al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH / 1.5, BOARD_HEIGHT / 1.25, ALLEGRO_ALIGN_CENTER, "For exit from game enter 'scape'");

        al_flip_display();

        FILE *Result;
        Result = fopen("Result.txt", "wt");

        fprintf(Result, "First person: %d\n", B[0]);
        fprintf(Result, "Second person: %d\n", B[1]);
        fprintf(Result, "Third person: %d\n", B[2]);
        fprintf(Result, "Fourth person: %d\n", B[3]);

        fclose(Result);

        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {

                case ALLEGRO_KEY_ESCAPE:
                    return -1;
            }
        }
    }

    return 0;
}