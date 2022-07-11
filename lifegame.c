#include<ncurses.h>
#include<unistd.h>
#include<locale.h>
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

#define WIDTH  100
#define HEIGHT 50

char CheckNeighborhodd(char* val[], int i, int j);
void DrawBoard(char* val[]);
void DrawBoarderLine(char* val[]);
void Initialize(void);

int main(int argc, char** argv){

    Initialize();

    char val[WIDTH][HEIGHT];
    char tmp_val[WIDTH][HEIGHT]; 
    unsigned char second = 1;

    //prepare pointers for the game board
    char* p_val[WIDTH];
    for(int i=0;i<WIDTH;i++){
        p_val[i] = val[i];
    }

    char* p_tmp_val[WIDTH];
    for(int i=0;i<WIDTH;i++){
        p_tmp_val[i] = tmp_val[i];
    }

    //clear the game board
    for(int j=0; j<HEIGHT;j++){
        for(int i=0; i<WIDTH;i++){
            val[i][j] = ' ';
            tmp_val[i][j] = ' ';
        }
    }

    //draw boarder line
    DrawBoarderLine(p_val);  

    /* generate 1000 live cells when the game start*/
    for(int i=0;i<1000;i++){
        int x = rand()%(WIDTH-2)+1;
        int y = rand()%(HEIGHT-2)+1;
        val[x][y] = '*';
    }

    for(;;){
        //draw the game board
        DrawBoard(p_val);  

        //copy to tmp
        for(int j=1; j<HEIGHT-1; j++){
            for(int i=1; i<WIDTH-1; i++){
                tmp_val[i][j] = val[i][j];
            }
        }

        DrawBoarderLine(p_val);

        //update 
        for(int j=1; j<HEIGHT-1; j++){
            for(int i=1; i<WIDTH-1; i++){
                val[i][j] = CheckNeighborhodd(p_tmp_val,i,j);
            }
        }

        sleep(second);
        clear();
    }
    endwin();
    return 0;
}


char CheckNeighborhodd(char* val[], int i, int j){
    int count = 0;
    bool isalive;
    
    // check if the cell is alive
    if(val[i][j] == '*'){
        isalive = true;
    }else{
        isalive = false; 
    }

    //check 8 neighborhood
    for(int dj=-1;dj<2;dj++){
        for(int di=-1;di<2;di++){
            if(val[i+di][j+dj] == '*' && !(di==0 && dj==0)){
                count++;
            }
        }
    }
   
    /*
     a: (3 live neighborhood) and (the cell is dead) -> reproduction
     b: ((2 live neighborhood) or (3 live neighborhood)) and (the cell is alive) -> next generation
     c: (the number of live neighborhood <= 1) and (the cell is alive) -> under population
     d: (the number of live neighborhood >= 4) and (the cell is alive) -> over population
    */
    if(count==3 && !isalive){    //case a
        return '*';
    }else if((count==2 || count==3) && isalive){//case b
        return '*';
    }else if((count<=1) && isalive){//case c
        return ' ';
    }else if((count>=4) && isalive){//case d
        return ' ';
    }else{
        return ' ';
    }
}


void DrawBoard(char* val[]){
    for(int j=0; j<HEIGHT; j++){
        for(int i=0; i<WIDTH; i++){
            mvprintw(j,i,"%c",val[i][j]);
            refresh();
        }
    }
}

void DrawBoarderLine(char* val[]){
    for(int i=0;i<WIDTH;i++){
        val[i][0] = '-';
        val[i][HEIGHT-1] = '-';
    }

    for(int i=0;i<HEIGHT;i++){
        val[0][i] = '|';
        val[WIDTH-1][i] = '|';
    } 
}

void Initialize(void){
    setlocale(LC_ALL,"");
    initscr();
    cbreak();
    noecho();
}


