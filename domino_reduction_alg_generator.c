#include <stdio.h>
#include <string.h>


//personal note: this is working version EODRv10_all_movecounts

int algcount = 0;
int casecounter = 0;

char move[15][2] = {{'R', ' '},{'R', 'I'},{'R', '2'},{'U', ' '},{'U', 'I'}, {'U', '2'},
                    {'L', ' '},{'L', 'I'},{'L', '2'},{'D', ' '},{'D', 'I'}, {'D', '2'},
                    {'F', '2'},{'B', '2'} };
//char doku[100000000][8][2] = {};                      //anzahl algs, anzahl moves, zwei weil man 'R'+'I' schreibt

char fullmap[100][100][16][16][16][9][2] = {};        //  char lsg[6500][4100][8] erste überlegung so zu hashen(oder halt aufteilen nach ids), aber das sind mehr als 2mio einträge. noch zu viel

int state[25] = {};
int c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0, c6 = 0, c7 = 0, c8 = 0;
int e1 = 0, e2 = 0, e3 = 0, e4 = 0, e5 = 0, e6 = 0, e7 = 0, e8 = 0, e9 = 0, ea = 0, eb = 0, ec = 0;
int drucker = 0;

//Funktionen:

void setzero(){
    c1 = 0; c2 = 0; c3 = 0; c4 = 0; c5 = 0; c6 = 0; c7 = 0; c8 = 0; 
    e1 = 0; e2 = 0; e3 = 0; e4 = 0; e5 = 0; e6 = 0; e7 = 0; e8 = 0; e9 = 0; ea = 0; eb = 0; ec = 0;
}

void R(){
    //3->2->6->7
    int zwischen3 = c2;
    c2 = (c3 + 1) % 3;
    c3 = (c7 + 2) % 3;
    c7 = (c6 + 1) % 3;
    c6 = (zwischen3 + 2) % 3;
    //2->6->a->7
    int zwischen = e2;
    if (e7 == 0){e2 = 1;} else {e2 = 0;}
    if (ea == 0){e7 = 1;} else {e7 = 0;}
    if (e6 == 0){ea = 1;} else {ea = 0;}
    if (zwischen == 0){e6 = 1;} else {e6 = 0;}
}
void R2(){R();R();}
void RI(){R();R();R();}
void L(){
    //3->2->6->7
    //1 4 7 8
    int zwischen3 = c4;
    c4 = (c1 + 1) % 3;
    c1 = (c5 + 2) % 3;
    c5 = (c8 + 1) % 3;
    c8 = (zwischen3 + 2) % 3;
    //2->6->a->7
    //4 8 c 5
    int zwischen = e4;
    if (e5 == 0){e4 = 1;} else {e4 = 0;}
    if (ec == 0){e5 = 1;} else {e5 = 0;}
    if (e8 == 0){ec = 1;} else {ec = 0;}
    if (zwischen == 0){e8 = 1;} else {e8 = 0;}
}
void L2(){L();L();}
void LI(){
    L();L();L();
}

void F(){
    //3->2->6->7
    //4 3 7 8 
    int zwischen3 = c3;
    c3 = (c4 + 1) % 3;
    c4 = (c8 + 2) % 3;
    c8 = (c7 + 1) % 3;
    c7 = (zwischen3 + 2) % 3;
    //2->6->a->7
    //4 7 b 8
    int zwischen = e3;
    if (e8 == 0){e3 = 1;} else {e3 = 0;}
    if (eb == 0){e8 = 1;} else {e8 = 0;}
    if (e7 == 0){eb = 1;} else {eb = 0;}
    if (zwischen == 0){e7 = 1;} else {e7 = 0;}
}
void F2(){
    F();F();
}
void FI(){
    F();F();F();
}

void U(){
    int zwischen3 = c2;
    c2 = (c1);
    c1 = (c4);
    c4 = (c3);
    c3 = (zwischen3);
    int zwischen = e1;
    if (e4 == 0){e1=  0;} else {e1 = 1;}
    if (e3 == 0){e4 = 0;} else {e4 = 1;}
    if (e2 == 0){e3 = 0;} else {e3 = 1;}
    if (zwischen == 0){e2 = 0;} else {e2 = 1;}
}
void U2(){
    U();U();
}
void UI(){
    U();U();U();
}

void D(){
    int zwischen3 = c8;
    c8 = (c5);
    c5 = (c6);
    c6 = (c7);
    c7 = (zwischen3);
    int zwischen = e9;
    if (ea == 0){e9 = 0;} else {e9 = 1;}
    if (eb == 0){ea = 0;} else {ea = 1;}
    if (ec == 0){eb = 0;} else {eb = 1;}
    if (zwischen == 0){ec = 0;} else {ec = 1;}
}
void D2(){
    D();D();
}
void DI(){
    D();D();D();
}

void B(){
    int zwischen3 = c5;
    c5 = (c1 + 2) % 3;
    c1 = (c2 + 1) % 3;
    c2 = (c6 + 2) % 3;
    c6 = (zwischen3 + 1) % 3;
    int zwischen = e1;
    if (e6 == 0){e1 = 1;} else {e1 = 0;}
    if (e9 == 0){e6 = 1;} else {e6 = 0;}
    if (e5 == 0){e9 = 1;} else {e9 = 0;}
    if (zwischen == 0){e5 = 1;} else {e5 = 0;}
}
void B2(){
    B();B();
}
void BI(){
    B();B();B();
}

void printstate(){
    for(int i = 0; i < 4; i++){printf( "%d", state[i]);}
    printf("_");
    for(int i = 4; i < 8; i++){printf("%d", state[i]);}
    printf( "__");
    for(int i = 8; i < 12; i++){printf("%d", state[i]);}
    printf( "_");
    for(int i = 12; i < 16; i++){printf("%d", state[i]);}
    printf( "_");
    for(int i = 16; i < 20; i++){printf("%d", state[i]);}
    printf( " ");
}

//void fprintstate();

void scanstate(){ 
    state[0 ]= c1; state[1 ]= c2; state[2 ]= c3; state[3 ]= c4; 
    state[4 ]= c5; state[5 ]= c6; state[6 ]= c7; state[7 ]= c8; 
    state[8 ]= e1; state[9 ]= e2; state[10]= e3; state[11]= e4; 
    state[12]= e5; state[13]= e6; state[14]= e7; state[15]= e8; 
    state[16]= e9; state[17]= ea; state[18]= eb; state[19]= ec;
}

int isstatezero(){
    if( 
        (state[0 ]== 0) && (state[1 ]== 0) && (state[2 ]== 0) && (state[3 ]== 0) && 
        (state[4 ]== 0) && (state[5 ]== 0) && (state[6 ]== 0) && (state[7 ]== 0) && 
        (state[8 ]== 0) && (state[9 ]== 0) && (state[10]== 0) && (state[11]== 0) && 
        (state[12]== 0) && (state[13]== 0) && (state[14]== 0) && (state[15]== 0) && 
        (state[16]== 0) && (state[17]== 0) && (state[18]== 0) && (state[19]== 0))
     {return 1;}
     else{return 0;}
}

int stateid1(){
    int id = state[3]*1+state[2]*3+state[1]*9+state[0]*27;
    return id;
}
int stateid2(){
    int id2 = state[7]*1+state[6]*3+state[5]*9+state[4]*27;
    return id2;
}
int stateid3(){
    int id3 = state[8]*9+state[9]*4+state[10]*2+state[11]*1;
    return id3;
}
int stateid4(){
    int id4 = state[12]*9+state[13]*4+state[14]*2+state[15]*1;
    return id4;
}
int stateid5(){
    int id5 = state[16]*9+state[17]*4+state[18]*2+state[19]*1;
    return id5;
}

int MOVE(x){
    if(x == 0 ){R() ;}
    if(x == 1 ){RI();}
    if(x == 2 ){R2();}
    if(x == 3 ){U() ;}
    if(x == 4 ){UI();}
    if(x == 5 ){U2();}
    if(x == 6 ){L() ;}
    if(x == 7 ){LI();}
    if(x == 8 ){L2();}
    if(x == 9 ){D() ;}
    if(x == 10){DI();}
    if(x == 11){D2();}
    if(x == 12){F2();}
    if(x == 13){B2();}
    return 0;
}


int zweimovesplausibel(previousmove, nextmove){
    int a_previousmove = previousmove;
    int a_nextmove = nextmove;
    if(((a_previousmove == 0)&&(a_nextmove == 1)) || ((a_previousmove == 1)&&(a_nextmove == 0))){return 0;}
    if(((a_previousmove == 2)&&(a_nextmove == 1)) || ((a_previousmove == 1)&&(a_nextmove == 2))){return 0;}
    if(((a_previousmove == 0)&&(a_nextmove == 2)) || ((a_previousmove == 2)&&(a_nextmove == 0))){return 0;}

    if(((a_previousmove == 3)&&(a_nextmove == 4)) || ((a_previousmove == 4)&&(a_nextmove == 3))){return 0;}
    if(((a_previousmove == 5)&&(a_nextmove == 4)) || ((a_previousmove == 4)&&(a_nextmove == 5))){return 0;}
    if(((a_previousmove == 3)&&(a_nextmove == 5)) || ((a_previousmove == 5)&&(a_nextmove == 3))){return 0;}

    if(((a_previousmove == 6)&&(a_nextmove == 7)) || ((a_previousmove == 7)&&(a_nextmove == 6))){return 0;}
    if(((a_previousmove == 8)&&(a_nextmove == 7)) || ((a_previousmove == 7)&&(a_nextmove == 8))){return 0;}
    if(((a_previousmove == 6)&&(a_nextmove == 8)) || ((a_previousmove == 8)&&(a_nextmove == 6))){return 0;}

    if(((a_previousmove == 9)&&(a_nextmove == 10)) || ((a_previousmove == 10)&&(a_nextmove == 9))){return 0;}
    if(((a_previousmove == 11)&&(a_nextmove == 10))|| ((a_previousmove == 10)&&(a_nextmove == 11))){return 0;}
    if(((a_previousmove == 9)&&(a_nextmove == 11)) || ((a_previousmove == 11)&&(a_nextmove == 9))){return 0;}
    else{return 1;}
}


void show(){
    printf("%d %d %d %d %d %d %d %d\n",c1,c2,c3,c4,c5,c6,c7,c8);
    printf("%d %d %d %d \n%d %d %d %d \n%d %d %d %d\n",e1,e2,e3,e4,e5,e6,e7,e8,e9,ea,eb,ec);
}
void S(){
    printf("%d%d%d%d_%d%d%d%d__",c1,c2,c3,c4,c5,c6,c7,c8);
    printf("%d%d%d%d_%d%d%d%d_%d%d%d%d   ",e1,e2,e3,e4,e5,e6,e7,e8,e9,ea,eb,ec);
}


char inv(extra){
    if(extra == ' ') {return 'I';}
    if(extra == 'I') {return ' ';}
    if(extra == '2') {return '2';}
    else return 'x';
}

void printscram(int movecount){
    for(int i = 0; i<movecount; i++){
        printf("%c%c ", fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][0],
                        fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][1]);
    }
}

void printalg(int movecount){
    movecount--;
    for(int i = movecount; i>(-1); i--){
        printf("%c%c ", fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][0],
                        inv(fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][1]));
    }
}

void findealg(int a, int b, int c, int d, int e){
    for(int k = 0; k<8; k++){
        printf("%c",fullmap[a][b][c][d][e][k][0]);
        printf("%c ",fullmap[a][b][c][d][e][k][1]);
    } 
} //teste mit 2020_2111__0010_1110_1010 = 6+54_54+9+3+1__2_14_10 = 60_67_2_14_10

//void fprintscram(int movecount);

//void fprintalg(int movecount);





/* int main v1
int main_v1(){
    //char *filename = "EODR_gen1_Alle_8mover_2.txt";
    //FILE *fp = fopen(filename, "w");
    //if (fp == NULL) {printf("Error opening the file %s", filename); return -1;}





     for(int firstmove = 0; firstmove <= 13; firstmove++){
         for(int secondmove = 0; secondmove <= 13; secondmove++){   
             for(int thirdmove = 0; thirdmove <= 13; thirdmove++){   
                 for(int fourthmove = 0; fourthmove <= 13; fourthmove++){
                    for(int fifthmove = 0; fifthmove <= 13; fifthmove++){   
                         for(int sixthmove = 0; sixthmove <= 13; sixthmove++){   
                                for(int seventhmove = 0; seventhmove <= 13; seventhmove++){   
                                     for(int eighthmove = 0; eighthmove <= 13; eighthmove++){   

                                        
                                        
                                        doku[algcount][0][0] = move[firstmove][0];  doku[algcount][0][1] = move[firstmove][1]; //Hier wird der erste Move itteriert
                                        doku[algcount][1][0] = move[secondmove][0]; doku[algcount][1][1] = move[secondmove][1]; //Hier wird der zweite Move itteriert
                                        doku[algcount][2][0] = move[thirdmove][0];  doku[algcount][2][1] = move[thirdmove][1]; //Hier wird der dritte Move itteriert
                                        doku[algcount][3][0] = move[fourthmove][0]; doku[algcount][3][1] = move[fourthmove][1]; 
                                        doku[algcount][4][0] = move[fifthmove][0];  doku[algcount][4][1] = move[fifthmove][1]; 
                                        doku[algcount][5][0] = move[sixthmove][0];  doku[algcount][5][1] = move[sixthmove][1]; 
                                        doku[algcount][6][0] = move[seventhmove][0];  doku[algcount][6][1] = move[seventhmove][1]; 
                                        doku[algcount][7][0] = move[eighthmove][0];  doku[algcount][7][1] = move[eighthmove][1]; 
                                        
                                        //S();
                                        //state wird nun abgespeichert anstatt algcount numeriert:
                                        

                                        MOVE(firstmove);  //S();
                                        MOVE(secondmove); //S();
                                        MOVE(thirdmove);  //S();
                                        MOVE(fourthmove);  //S();
                                        MOVE(fifthmove);  //S();
                                        MOVE(sixthmove);  //S();
                                        MOVE(seventhmove);  //S();
                                        MOVE(eighthmove);  //S();

                                        scanstate();
                                        if
                                        (!isstatezero() 
                                        && (firstmove != secondmove) 
                                        && (secondmove != thirdmove)
                                        && (thirdmove != fourthmove) 
                                        && (fourthmove != fifthmove) 
                                        && (fifthmove != sixthmove) 
                                        && (sixthmove != seventhmove) 
                                        && (seventhmove != eighthmove) 
                                        && (zweimovesplausibel(firstmove, secondmove)) 
                                        && (zweimovesplausibel(secondmove, thirdmove))
                                        && (zweimovesplausibel(thirdmove, fourthmove))
                                        && (zweimovesplausibel(fourthmove, fifthmove))
                                        && (zweimovesplausibel(fifthmove, sixthmove))
                                        && (zweimovesplausibel(sixthmove, seventhmove))
                                        && (zweimovesplausibel(seventhmove, eighthmove))
                                        && (fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] == 0)
                                        
                                        ){
                                            printf("ja, noch nicht gefüllt");
                                            //Abspeichern der characters
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] = move[firstmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][1] = move[firstmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][0] = move[secondmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][1] = move[secondmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][0] = move[thirdmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][1] = move[thirdmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][0] = move[fourthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][1] = move[fourthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][4][0] = move[fifthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][4][1] = move[fifthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][5][0] = move[sixthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][5][1] = move[sixthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][6][0] = move[seventhmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][6][1] = move[seventhmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][7][0] = move[eighthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][7][1] = move[eighthmove][1];





                                            fprintf(fp, " Algnummer: %08d,   state: ", algcount); 
                                
                                           
                                             for(int i = 0; i < 4; i++){fprintf(fp, "%d", state[i]);}
                                             fprintf(fp, "_");
                                             for(int i = 4; i < 8; i++){fprintf(fp,"%d", state[i]);}
                                             fprintf(fp, "__");
                                             for(int i = 8; i < 12; i++){fprintf(fp,"%d", state[i]);}
                                             fprintf(fp, "_");
                                             for(int i = 12; i < 16; i++){fprintf(fp,"%d", state[i]);}
                                             fprintf(fp, "_");
                                             for(int i = 16; i < 20; i++){fprintf(fp,"%d", state[i]);}
                                             fprintf(fp, " ");


                                
                                            fprintf(fp, "   Scramble: %c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c", 

                                            doku[algcount][0][0], doku[algcount][0][1], 
                                            doku[algcount][1][0], doku[algcount][1][1], 
                                            doku[algcount][2][0], doku[algcount][2][1],
                                            doku[algcount][3][0], doku[algcount][3][1], 
                                            doku[algcount][4][0], doku[algcount][4][1], 
                                            doku[algcount][5][0], doku[algcount][5][1],
                                            doku[algcount][6][0], doku[algcount][6][1],
                                            doku[algcount][7][0], doku[algcount][7][1]
                                            );
                                
                                            fprintf(fp, "   Solution: %c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c\n", 
                                            doku[algcount][7][0], inv(doku[algcount][7][1]), 
                                            doku[algcount][6][0], inv(doku[algcount][6][1]), 
                                            doku[algcount][5][0], inv(doku[algcount][5][1]), 
                                            doku[algcount][4][0], inv(doku[algcount][4][1]), 
                                            doku[algcount][3][0], inv(doku[algcount][3][1]),
                                            doku[algcount][2][0], inv(doku[algcount][2][1]), 
                                            doku[algcount][1][0], inv(doku[algcount][1][1]), 
                                            doku[algcount][0][0], inv(doku[algcount][0][1]));
                                            
                                            //drucker++;
                                        }

                                        setzero();
                                        algcount++;


                                }setzero();
                            }setzero();   
                        }setzero();
                    }setzero();
                }setzero();
            }setzero();
        }setzero();
     }setzero();

fprintf(fp, "\n Angezeigte Algs: %d von %d", algcount, algcount);
printf("done");
fclose(fp);
return 0;
}
*/

void clear(int a, int b, int c, int d, int e){
                fullmap[a][b][c][d][e][0][0] = 0;
                fullmap[a][b][c][d][e][0][1] = 0;
                fullmap[a][b][c][d][e][1][0] = 0;
                fullmap[a][b][c][d][e][1][1] = 0;
                fullmap[a][b][c][d][e][2][0] = 0;
                fullmap[a][b][c][d][e][2][1] = 0;
                fullmap[a][b][c][d][e][3][0] = 0;
                fullmap[a][b][c][d][e][3][1] = 0;
                fullmap[a][b][c][d][e][4][0] = 0;
                fullmap[a][b][c][d][e][4][1] = 0;
                fullmap[a][b][c][d][e][5][0] = 0;
                fullmap[a][b][c][d][e][5][1] = 0;
                fullmap[a][b][c][d][e][6][0] = 0;
                fullmap[a][b][c][d][e][6][1] = 0;
                fullmap[a][b][c][d][e][7][0] = 0;
                fullmap[a][b][c][d][e][7][1] = 0;
        

}


void sevenmover();

void eightmover(){
    for(int firstmove = 0; firstmove <= 13; firstmove++){
         for(int secondmove = 0; secondmove <= 13; secondmove++){
             if((firstmove == secondmove)||(!zweimovesplausibel(firstmove, secondmove)))  {continue;}
             for(int thirdmove = 0; thirdmove <= 13; thirdmove++){ 
                 if((secondmove == thirdmove)||(!zweimovesplausibel(secondmove, thirdmove))) {continue;}
                 for(int fourthmove = 0; fourthmove <= 13; fourthmove++){
                     if((thirdmove == fourthmove)||(!zweimovesplausibel(thirdmove, fourthmove))) {continue;}
                    for(int fifthmove = 0; fifthmove <= 13; fifthmove++){   
                        if((fourthmove == fifthmove)||(!zweimovesplausibel(fourthmove, fifthmove))) {continue;}
                         for(int sixthmove = 0; sixthmove <= 13; sixthmove++){
                             if((fifthmove == sixthmove)||(!zweimovesplausibel(fifthmove, sixthmove))) {continue;}   
                                for(int seventhmove = 0; seventhmove <= 13; seventhmove++){ 
                                    if((sixthmove == seventhmove)||(!zweimovesplausibel(sixthmove, seventhmove))) {continue;}  
                                     for(int eighthmove = 0; eighthmove <= 13; eighthmove++){   
                                        if((seventhmove == eighthmove)||(!zweimovesplausibel(seventhmove, eighthmove))) {continue;}  



                                        MOVE(firstmove);  //S();
                                        MOVE(secondmove); //S();
                                        MOVE(thirdmove);  //S();
                                        MOVE(fourthmove);  //S();
                                        MOVE(fifthmove);  //S();
                                        MOVE(sixthmove);  //S();
                                        MOVE(seventhmove);  //S();
                                        MOVE(eighthmove);  //S();

                                        scanstate();
                                        if
                                        (!isstatezero() 
                                        && (firstmove != secondmove) 
                                        && (secondmove != thirdmove)
                                        && (thirdmove != fourthmove) 
                                        && (fourthmove != fifthmove) 
                                        && (fifthmove != sixthmove) 
                                        && (sixthmove != seventhmove) 
                                        && (seventhmove != eighthmove) 
                                        && (zweimovesplausibel(firstmove, secondmove)) 
                                        && (zweimovesplausibel(secondmove, thirdmove))
                                        && (zweimovesplausibel(thirdmove, fourthmove))
                                        && (zweimovesplausibel(fourthmove, fifthmove))
                                        && (zweimovesplausibel(fifthmove, sixthmove))
                                        && (zweimovesplausibel(sixthmove, seventhmove))
                                        && (zweimovesplausibel(seventhmove, eighthmove))
                                        && (fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] == 0)
                                        
                                        ){
                                            clear(stateid1(),stateid2(),stateid3(),stateid4(),stateid5());
                                            //Abspeichern der characters
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] = move[firstmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][1] = move[firstmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][0] = move[secondmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][1] = move[secondmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][0] = move[thirdmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][1] = move[thirdmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][0] = move[fourthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][1] = move[fourthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][4][0] = move[fifthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][4][1] = move[fifthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][5][0] = move[sixthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][5][1] = move[sixthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][6][0] = move[seventhmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][6][1] = move[seventhmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][7][0] = move[eighthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][7][1] = move[eighthmove][1];
                                            
                                            drucker++;

                                            /*printstate();
                                            printf("  Scramble: ");
                                            printscram(8);
                                            printf("  Alg: ");
                                            printalg(8);
                                            printf("Nr. %d", drucker);
                                            printf("\n");*/

                                            //fprintstate();
                                            //fprintf(fp, "  Scramble: ");
                                            //fprintscram(8);
                                            //fprintf(fp, "  Alg: ");
                                            //fprintalg(8);
                                            //fprintf(fp, "Nr. %d", drucker);
                                            //fprintf(fp, "\n");
                                        }
                                        
                                        setzero();
                                        


                                    }setzero();
                                    algcount++; 
                                    //printf("o");
                                    //printf("%d, ", algcount);
                                    //printf("Algs gefunden: %d\n", drucker);

                            }setzero();
                        }setzero();
                    }setzero();//printf("\n\n\nforthmovealtered");
                }setzero();
            }setzero();//printf("\n\n\nsecondmovealtered");
        }setzero();

//fprintf(fp, "\n Angezeigte Algs: %d von %d (evtl Bugs in dieser Zahl)", drucker, algcount);
//fprintf(fp, "done");

/*printf("\n Angezeigte Algs: %d von %d", drucker, algcount);
printf("done");*/
    }
}



int main(){
    char *filename = "EODRv8_gen4.txt";
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {printf("Error opening the file %s", filename); return -1;}
    //sevenmover();


      for(int firstmove = 0; firstmove <= 13; firstmove++){
         for(int secondmove = 0; secondmove <= 13; secondmove++){
             if((firstmove == secondmove)||(!zweimovesplausibel(firstmove, secondmove)))  {continue;}

                                        MOVE(firstmove);  //S();
                                        MOVE(secondmove); //S();
                                     

                                        scanstate();
                                        if
                                        (!isstatezero() 
                                        && (firstmove != secondmove) 
                                      
                                        && (zweimovesplausibel(firstmove, secondmove)) 
                                     
                                        && (fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] == 0)
                                        
                                        ){
                                            clear(stateid1(),stateid2(),stateid3(),stateid4(),stateid5());
                                            //Abspeichern der characters
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] = move[firstmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][1] = move[firstmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][0] = move[secondmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][1] = move[secondmove][1];
                                            
                                            drucker++;

                                            if(stateid2() == 0){
                                            printstate();
                                            printf("  Scramble: ");
                                            printscram(2);
                                            printf("  Alg: ");
                                            printalg(2);
                                            printf("Nr. %d", drucker); printf("Casecounter: %d", casecounter);
                                            printf("\n");
                                            casecounter++;
                                            }
                                        setzero();
                                        algcount++;
                           
      
            }setzero();//printf("\n\n\nsecondmovealtered");
        }setzero();
   }
    fprintf(fp, "\n Angezeigte Algs: %d von %d", drucker, algcount);
    fprintf(fp, "done");

//#########################################


      for(int firstmove = 0; firstmove <= 13; firstmove++){
         for(int secondmove = 0; secondmove <= 13; secondmove++){
             if((firstmove == secondmove)||(!zweimovesplausibel(firstmove, secondmove)))  {continue;}
             for(int thirdmove = 0; thirdmove <= 13; thirdmove++){ 
                 if((secondmove == thirdmove)||(!zweimovesplausibel(secondmove, thirdmove))) {continue;}


                                        MOVE(firstmove);  //S();
                                        MOVE(secondmove); //S();
                                        MOVE(thirdmove);  //S();
                                     

                                        scanstate();
                                        if
                                        (!isstatezero() 
                                        && (firstmove != secondmove) 
                                        && (secondmove != thirdmove)
                                      
                                        && (zweimovesplausibel(firstmove, secondmove)) 
                                        && (zweimovesplausibel(secondmove, thirdmove))
                                     
                                        && (fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] == 0)
                                        
                                        ){
                                            clear(stateid1(),stateid2(),stateid3(),stateid4(),stateid5());
                                            //Abspeichern der characters
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] = move[firstmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][1] = move[firstmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][0] = move[secondmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][1] = move[secondmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][0] = move[thirdmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][1] = move[thirdmove][1];
                                            
                                            drucker++;

                                            if(stateid2() == 0){
                                            printstate();
                                            printf("  Scramble: ");
                                            printscram(3);
                                            printf("  Alg: ");
                                            printalg(3);
                                            printf("Nr. %d", drucker); printf("Casecounter: %d", casecounter);
                                            printf("\n");
                                            casecounter++;
                                            }
                                        setzero();
                                        algcount++;
                                    
                          
                }setzero();
            }setzero();//printf("\n\n\nsecondmovealtered");
        }setzero();
   }
    fprintf(fp, "\n Angezeigte Algs: %d von %d", drucker, algcount);
    fprintf(fp, "done");

//#########################################



      for(int firstmove = 0; firstmove <= 13; firstmove++){
         for(int secondmove = 0; secondmove <= 13; secondmove++){
             if((firstmove == secondmove)||(!zweimovesplausibel(firstmove, secondmove)))  {continue;}
             for(int thirdmove = 0; thirdmove <= 13; thirdmove++){ 
                 if((secondmove == thirdmove)||(!zweimovesplausibel(secondmove, thirdmove))) {continue;}
                 for(int fourthmove = 0; fourthmove <= 13; fourthmove++){
                     if((thirdmove == fourthmove)||(!zweimovesplausibel(thirdmove, fourthmove))) {continue;}



                                        MOVE(firstmove);  //S();
                                        MOVE(secondmove); //S();
                                        MOVE(thirdmove);  //S();
                                        MOVE(fourthmove);  //S();
                                     

                                        scanstate();
                                        if
                                        (!isstatezero() 
                                        && (firstmove != secondmove) 
                                        && (secondmove != thirdmove)
                                        && (thirdmove != fourthmove) 
                                      
                                        && (zweimovesplausibel(firstmove, secondmove)) 
                                        && (zweimovesplausibel(secondmove, thirdmove))
                                        && (zweimovesplausibel(thirdmove, fourthmove))
                                     
                                        && (fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] == 0)
                                        
                                        ){
                                            clear(stateid1(),stateid2(),stateid3(),stateid4(),stateid5());
                                            //Abspeichern der characters
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] = move[firstmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][1] = move[firstmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][0] = move[secondmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][1] = move[secondmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][0] = move[thirdmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][1] = move[thirdmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][0] = move[fourthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][1] = move[fourthmove][1];
                                            
                                            drucker++;

                                            if(stateid2() == 0){
                                            printstate();
                                            printf("  Scramble: ");
                                            printscram(4);
                                            printf("  Alg: ");
                                            printalg(4);
                                            printf("Nr. %d", drucker); printf("Casecounter: %d", casecounter);
                                            printf("\n");
                                            casecounter++;
                                            }
                                        setzero();
                                        algcount++;
                            
                    }setzero();//printf("\n\n\nforthmovealtered");
                }setzero();
            }setzero();//printf("\n\n\nsecondmovealtered");
        }setzero();
   }
    fprintf(fp, "\n Angezeigte Algs: %d von %d", drucker, algcount);
    fprintf(fp, "done");

//#########################################


  for(int firstmove = 0; firstmove <= 13; firstmove++){
         for(int secondmove = 0; secondmove <= 13; secondmove++){
             if((firstmove == secondmove)||(!zweimovesplausibel(firstmove, secondmove)))  {continue;}
             for(int thirdmove = 0; thirdmove <= 13; thirdmove++){ 
                 if((secondmove == thirdmove)||(!zweimovesplausibel(secondmove, thirdmove))) {continue;}
                 for(int fourthmove = 0; fourthmove <= 13; fourthmove++){
                     if((thirdmove == fourthmove)||(!zweimovesplausibel(thirdmove, fourthmove))) {continue;}
                    for(int fifthmove = 0; fifthmove <= 13; fifthmove++){   
                        if((fourthmove == fifthmove)||(!zweimovesplausibel(fourthmove, fifthmove))) {continue;}
                      




                                        MOVE(firstmove);  //S();
                                        MOVE(secondmove); //S();
                                        MOVE(thirdmove);  //S();
                                        MOVE(fourthmove);  //S();
                                        MOVE(fifthmove);  //S();
                                     

                                        scanstate();
                                        if
                                        (!isstatezero() 
                                        && (firstmove != secondmove) 
                                        && (secondmove != thirdmove)
                                        && (thirdmove != fourthmove) 
                                        && (fourthmove != fifthmove) 
                                      
                                        && (zweimovesplausibel(firstmove, secondmove)) 
                                        && (zweimovesplausibel(secondmove, thirdmove))
                                        && (zweimovesplausibel(thirdmove, fourthmove))
                                        && (zweimovesplausibel(fourthmove, fifthmove))
                                     
                                        && (fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] == 0)
                                        
                                        ){
                                            clear(stateid1(),stateid2(),stateid3(),stateid4(),stateid5());
                                            //Abspeichern der characters
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] = move[firstmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][1] = move[firstmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][0] = move[secondmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][1] = move[secondmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][0] = move[thirdmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][1] = move[thirdmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][0] = move[fourthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][1] = move[fourthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][4][0] = move[fifthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][4][1] = move[fifthmove][1];
                                            
                                            drucker++;

                                            if(stateid2() == 0){
                                            printstate();
                                            printf("  Scramble: ");
                                            printscram(5);
                                            printf("  Alg: ");
                                            printalg(5);
                                            printf("Nr. %d", drucker); printf("Casecounter: %d", casecounter);
                                            printf("\n");
                                            casecounter++;
                                            }

                                        
                                        setzero();
                                        algcount++;
                                        

                              
                            }setzero();
                        }setzero();
                    }setzero();//printf("\n\n\nforthmovealtered");
                }setzero();
            }setzero();//printf("\n\n\nsecondmovealtered");
        }
    fprintf(fp, "\n Angezeigte Algs: %d von %d", drucker, algcount);
    fprintf(fp, "done");

//#########################################



   for(int firstmove = 0; firstmove <= 13; firstmove++){
         for(int secondmove = 0; secondmove <= 13; secondmove++){
             if((firstmove == secondmove)||(!zweimovesplausibel(firstmove, secondmove)))  {continue;}
             for(int thirdmove = 0; thirdmove <= 13; thirdmove++){ 
                 if((secondmove == thirdmove)||(!zweimovesplausibel(secondmove, thirdmove))) {continue;}
                 for(int fourthmove = 0; fourthmove <= 13; fourthmove++){
                     if((thirdmove == fourthmove)||(!zweimovesplausibel(thirdmove, fourthmove))) {continue;}
                    for(int fifthmove = 0; fifthmove <= 13; fifthmove++){   
                        if((fourthmove == fifthmove)||(!zweimovesplausibel(fourthmove, fifthmove))) {continue;}
                         for(int sixthmove = 0; sixthmove <= 13; sixthmove++){
                             if((fifthmove == sixthmove)||(!zweimovesplausibel(fifthmove, sixthmove))) {continue;}   
                              




                                        MOVE(firstmove);  //S();
                                        MOVE(secondmove); //S();
                                        MOVE(thirdmove);  //S();
                                        MOVE(fourthmove);  //S();
                                        MOVE(fifthmove);  //S();
                                        MOVE(sixthmove);  //S();
                                     

                                        scanstate();
                                        if
                                        (!isstatezero() 
                                        && (firstmove != secondmove) 
                                        && (secondmove != thirdmove)
                                        && (thirdmove != fourthmove) 
                                        && (fourthmove != fifthmove) 
                                        && (fifthmove != sixthmove) 
                                      
                                        && (zweimovesplausibel(firstmove, secondmove)) 
                                        && (zweimovesplausibel(secondmove, thirdmove))
                                        && (zweimovesplausibel(thirdmove, fourthmove))
                                        && (zweimovesplausibel(fourthmove, fifthmove))
                                        && (zweimovesplausibel(fifthmove, sixthmove))
                                     
                                        && (fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] == 0)
                                        
                                        ){
                                            clear(stateid1(),stateid2(),stateid3(),stateid4(),stateid5());
                                            //Abspeichern der characters
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] = move[firstmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][1] = move[firstmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][0] = move[secondmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][1] = move[secondmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][0] = move[thirdmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][1] = move[thirdmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][0] = move[fourthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][1] = move[fourthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][4][0] = move[fifthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][4][1] = move[fifthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][5][0] = move[sixthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][5][1] = move[sixthmove][1];
                                            
                                            drucker++;

                                            if(stateid2() == 0){
                                            printstate();
                                            printf("  Scramble: ");
                                            printscram(6);
                                            printf("  Alg: ");
                                            printalg(6);
                                            printf("Nr. %d", drucker); printf("Casecounter: %d", casecounter);
                                            printf("\n");
                                            casecounter++;
                                            }

                                        
                                        setzero();
                                        algcount++;
                                        

                              
                            }setzero();
                        }setzero();
                    }setzero();//printf("\n\n\nforthmovealtered");
                }setzero();
            }setzero();//printf("\n\n\nsecondmovealtered");
        }setzero();
   }
    fprintf(fp, "\n Angezeigte Algs: %d von %d", drucker, algcount);
    fprintf(fp, "done");


//#########################################



   for(int firstmove = 0; firstmove <= 13; firstmove++){
         for(int secondmove = 0; secondmove <= 13; secondmove++){
             if((firstmove == secondmove)||(!zweimovesplausibel(firstmove, secondmove)))  {continue;}
             for(int thirdmove = 0; thirdmove <= 13; thirdmove++){ 
                 if((secondmove == thirdmove)||(!zweimovesplausibel(secondmove, thirdmove))) {continue;}
                 for(int fourthmove = 0; fourthmove <= 13; fourthmove++){
                     if((thirdmove == fourthmove)||(!zweimovesplausibel(thirdmove, fourthmove))) {continue;}
                    for(int fifthmove = 0; fifthmove <= 13; fifthmove++){   
                        if((fourthmove == fifthmove)||(!zweimovesplausibel(fourthmove, fifthmove))) {continue;}
                         for(int sixthmove = 0; sixthmove <= 13; sixthmove++){
                             if((fifthmove == sixthmove)||(!zweimovesplausibel(fifthmove, sixthmove))) {continue;}   
                                for(int seventhmove = 0; seventhmove <= 13; seventhmove++){ 
                                    if((sixthmove == seventhmove)||(!zweimovesplausibel(sixthmove, seventhmove))) {continue;}  




                                        MOVE(firstmove);  //S();
                                        MOVE(secondmove); //S();
                                        MOVE(thirdmove);  //S();
                                        MOVE(fourthmove);  //S();
                                        MOVE(fifthmove);  //S();
                                        MOVE(sixthmove);  //S();
                                        MOVE(seventhmove);  //S();
                                     

                                        scanstate();
                                        if
                                        (!isstatezero() 
                                        && (firstmove != secondmove) 
                                        && (secondmove != thirdmove)
                                        && (thirdmove != fourthmove) 
                                        && (fourthmove != fifthmove) 
                                        && (fifthmove != sixthmove) 
                                        && (sixthmove != seventhmove) 
                                      
                                        && (zweimovesplausibel(firstmove, secondmove)) 
                                        && (zweimovesplausibel(secondmove, thirdmove))
                                        && (zweimovesplausibel(thirdmove, fourthmove))
                                        && (zweimovesplausibel(fourthmove, fifthmove))
                                        && (zweimovesplausibel(fifthmove, sixthmove))
                                        && (zweimovesplausibel(sixthmove, seventhmove))
                                     
                                        && (fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] == 0)
                                        
                                        ){
                                            clear(stateid1(),stateid2(),stateid3(),stateid4(),stateid5());
                                            //Abspeichern der characters
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] = move[firstmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][1] = move[firstmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][0] = move[secondmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][1] = move[secondmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][0] = move[thirdmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][1] = move[thirdmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][0] = move[fourthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][1] = move[fourthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][4][0] = move[fifthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][4][1] = move[fifthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][5][0] = move[sixthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][5][1] = move[sixthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][6][0] = move[seventhmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][6][1] = move[seventhmove][1];
                                            
                                            drucker++;

                                            if(stateid2() == 0){
                                            printstate();
                                            printf("  Scramble: ");
                                            printscram(7);
                                            printf("  Alg: ");
                                            printalg(7);
                                            printf("Nr. %d", drucker); printf("Casecounter: %d", casecounter);
                                            printf("\n");
                                            casecounter++;
                                            }

                                            /*
                                            printstate();
                                            printf("  Scramble: ");
                                            printscram(7);
                                            printf("  Alg: ");
                                            printalg(7);
                                            printf("Nr. %d", drucker);
                                            printf("\n");

                                            
                                            //fprintstate();

                                               for(int i = 0; i < 4; i++){fprintf(fp,  "%d", state[i]);}
                                               fprintf(fp, "_");
                                               for(int i = 4; i < 8; i++){fprintf(fp, "%d", state[i]);}
                                               fprintf(fp,  "__");
                                               for(int i = 8; i < 12; i++){fprintf(fp, "%d", state[i]);}
                                               fprintf(fp,  "_");
                                               for(int i = 12; i < 16; i++){fprintf(fp, "%d", state[i]);}
                                               fprintf(fp,  "_");
                                               for(int i = 16; i < 20; i++){fprintf(fp, "%d", state[i]);}
                                               fprintf(fp,  " ");


                                            fprintf(fp, "  Scramble: ");
                                            //fprintscram(7);

                                             for(int i = 0; i<7; i++){
                                                 fprintf(fp, "%c%c ", fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][0],
                                                                 fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][1]);
                                             }


                            
                                            fprintf(fp, "  Alg: ");
                                            //fprintalg(7);


                                            for(int i = 6; i>(-1); i--){
                                                   fprintf(fp, "%c%c ", fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][0],
                                                                   inv(fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][1]));}


                                            fprintf(fp, "Nr. %d", drucker);
                                            fprintf(fp, "\n");*/
                                        
                                        setzero();
                                        algcount++;
                                        

                                }setzero(); 
                            }setzero();
                        }setzero();
                    }setzero();//printf("\n\n\nforthmovealtered");
                }setzero();
            }setzero();//printf("\n\n\nsecondmovealtered");
        }setzero();
   }
    fprintf(fp, "\n Angezeigte Algs: %d von %d", drucker, algcount);
    fprintf(fp, "done");



//#############################################################################




   for(int firstmove = 0; firstmove <= 13; firstmove++){
         for(int secondmove = 0; secondmove <= 13; secondmove++){
             if((firstmove == secondmove)||(!zweimovesplausibel(firstmove, secondmove)))  {continue;}
             for(int thirdmove = 0; thirdmove <= 13; thirdmove++){ 
                 if((secondmove == thirdmove)||(!zweimovesplausibel(secondmove, thirdmove))) {continue;}
                 for(int fourthmove = 0; fourthmove <= 13; fourthmove++){
                     if((thirdmove == fourthmove)||(!zweimovesplausibel(thirdmove, fourthmove))) {continue;}
                    for(int fifthmove = 0; fifthmove <= 13; fifthmove++){   
                        if((fourthmove == fifthmove)||(!zweimovesplausibel(fourthmove, fifthmove))) {continue;}
                         for(int sixthmove = 0; sixthmove <= 13; sixthmove++){
                             if((fifthmove == sixthmove)||(!zweimovesplausibel(fifthmove, sixthmove))) {continue;}   
                                for(int seventhmove = 0; seventhmove <= 13; seventhmove++){ 
                                    if((sixthmove == seventhmove)||(!zweimovesplausibel(sixthmove, seventhmove))) {continue;}  
                                     for(int eighthmove = 0; eighthmove <= 13; eighthmove++){   
                                        if((seventhmove == eighthmove)||(!zweimovesplausibel(seventhmove, eighthmove))) {continue;}  

                                        MOVE(firstmove);  //S();
                                        MOVE(secondmove); //S();
                                        MOVE(thirdmove);  //S();
                                        MOVE(fourthmove);  //S();
                                        MOVE(fifthmove);  //S();
                                        MOVE(sixthmove);  //S();
                                        MOVE(seventhmove);  //S();
                                        MOVE(eighthmove);  //S();

                                        scanstate();
                                        if
                                        (!isstatezero() 
                                        && (firstmove != secondmove) 
                                        && (secondmove != thirdmove)
                                        && (thirdmove != fourthmove) 
                                        && (fourthmove != fifthmove) 
                                        && (fifthmove != sixthmove) 
                                        && (sixthmove != seventhmove) 
                                        && (seventhmove != eighthmove) 
                                        && (zweimovesplausibel(firstmove, secondmove)) 
                                        && (zweimovesplausibel(secondmove, thirdmove))
                                        && (zweimovesplausibel(thirdmove, fourthmove))
                                        && (zweimovesplausibel(fourthmove, fifthmove))
                                        && (zweimovesplausibel(fifthmove, sixthmove))
                                        && (zweimovesplausibel(sixthmove, seventhmove))
                                        && (zweimovesplausibel(seventhmove, eighthmove))
                                        && (fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] == 0)
                                        
                                        ){
                                            clear(stateid1(),stateid2(),stateid3(),stateid4(),stateid5());
                                            //Abspeichern der characters
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][0] = move[firstmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][0][1] = move[firstmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][0] = move[secondmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][1][1] = move[secondmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][0] = move[thirdmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][2][1] = move[thirdmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][0] = move[fourthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][3][1] = move[fourthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][4][0] = move[fifthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][4][1] = move[fifthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][5][0] = move[sixthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][5][1] = move[sixthmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][6][0] = move[seventhmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][6][1] = move[seventhmove][1];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][7][0] = move[eighthmove][0];
                                            fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][7][1] = move[eighthmove][1];
                                            
                                            drucker++;

                                            if(stateid2() == 0){
                                            printstate();
                                            printf("  Scramble: ");
                                            printscram(8);
                                            printf("  Alg: ");
                                            printalg(8);
                                            printf("Nr. %d", drucker); printf("Casecounter: %d", casecounter);
                                            printf("\n");
                                            casecounter++;
                                            }
                                            
                                            


                                            
                                        }
                                        
                                        setzero();
                                    }setzero(); algcount++; 
                                }setzero(); 
                            }setzero();
                        }setzero();
                    }setzero();//printf("\n\n\nforthmovealtered");
                }setzero();
            }setzero();//printf("\n\n\nsecondmovealtered");
        }setzero();

printf("Casecounter: %d", casecounter);
fprintf(fp, "\n Angezeigte Algs: %d von %d (evtl Bugs in dieser Zahl)", drucker, algcount);
fprintf(fp, "done");

 //60_67_2_14_10
//####################

//findealg(60,67,2,14,10);


 

/*
for(int i = 0; i < 4; i++){fprintf(fp,  "%d", state[i]);}
fprintf(fp, "_");
for(int i = 4; i < 8; i++){fprintf(fp, "%d", state[i]);}
fprintf(fp,  "__");
for(int i = 8; i < 12; i++){fprintf(fp, "%d", state[i]);}
fprintf(fp,  "_");
for(int i = 12; i < 16; i++){fprintf(fp, "%d", state[i]);}
fprintf(fp,  "_");
for(int i = 16; i < 20; i++){fprintf(fp, "%d", state[i]);}
fprintf(fp,  " ");
fprintf(fp, "  Scramble: ");
//printscram
for(int i = 0; i<8; i++){
fprintf(fp, "%c%c ", fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][0],
fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][1]);}
fprintf(fp, "  Alg: ");
//printalg                    
for(int i = 7; i>(-1); i--){
fprintf(fp, "%c%c ", fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][0],
                   inv(fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][1]));
fprintf(fp, "Nr. %d", drucker);
fprintf(fp, "\n");*/


//####################


    //eightmover();
    fclose(fp);
    return 0;
}

/*
void fprintstate(){
    for(int i = 0; i < 4; i++){fprintf(fp,  "%d", state[i]);}
    fprintf(fp, "_");
    for(int i = 4; i < 8; i++){fprintf(fp, "%d", state[i]);}
    fprintf(fp,  "__");
    for(int i = 8; i < 12; i++){fprintf(fp, "%d", state[i]);}
    fprintf(fp,  "_");
    for(int i = 12; i < 16; i++){fprintf(fp, "%d", state[i]);}
    fprintf(fp,  "_");
    for(int i = 16; i < 20; i++){fprintf(fp, "%d", state[i]);}
    fprintf(fp,  " ");
}

void fprintscram(int movecount){
    for(int i = 0; i<movecount; i++){
        fprintf(fp, "%c%c ", fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][0],
                        fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][1]);
    }
}

void fprintalg(int movecount){
    movecount--;
    for(int i = movecount; i>(-1); i--){
        fprintf(fp, "%c%c ", fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][0],
                        inv(fullmap[stateid1()][stateid2()][stateid3()][stateid4()][stateid5()][i][1]));
    }
}

*/ 





/* Kommentare 1
    Frage zunächst:
    Wie viele EO-DR-Fälle gibt es überhaupt?

    0e
        2c0e
            2c0e:
            1c0c1c:
        3c0e
            3c0e:
            2c0e1c:
        4c0e
            4c0e:
            2c0e2c:
        6c0e

        7c0e
        8c0e

    1e
    2e
    3e
    4e



    Andere Herangehensweise:
    3^7 Möglichkeiten für die Ecken
    1(0e)+8*4(1e)+6*(7+6+5+4+3+2+1)(2e)+4*56(3e)+1(4e)
    = 1+32+28*6+4*56+1= 426

    Die ober rechnung war falsch. Hier neu:

    2e: 8*4/2= 16 Fälle * 2 = 32
    4e: 6(Anzahl Midlayercombs)*8*7/2= 168 Fälle

    4e: 
        beide unten: 6 Fälle für Unten * 6 Fälle in Mitte = 36
        beide oben: 6 Fälle für oben * 6 Fälle in Mitte = 36
        1oben 1 unten: 16 Fälle * 6 Fälle = 96 Fälle     

    6e: 4(Anzahl Midlayercombs)*8*7*6/(3*2*1)= 224 Fälle
    alle3oben: 4  * 4(mid) = 16
    alle3unten: 4  * 4(mid) = 16
    2-1= 6*4  *4(mid) = 96
    1-2=6*4* 4(mid) = 96
    8e: 
        4-0: 1
        0-4: 1
        1-3: 16
        3-1: 16
        2-2: 36 = total8e: 70

    insgesamt: 32+168+36+36+96+224+70= 662

    

 662*27 = 17874 Rohfälle hat (EODR-4eD)



    8e: 1*8*7*6*5/(4*3*2)= 70 Fälle
    Neue Zahl für Edge-Cases: 3392 statt 426 (verachtfachung) 
    Neue Neue Zahl: 478


    Total: 2187*3392= 7418304 cases (upper bound) of EO-DR
    neueneustotal: 2187*478= 1045386

    Reduzieren:
    Arten der Reduktion (manchmal möglich): rotieren. U-Set-OLL gibt es wahrscheinlich


    Eckenkombinationen kann man wahrscheinlich maßig reduzieren:
    Schreibe jeden Fall auf:


    Notation: Für die obere und untere Face könnte man sagen: 
    OLL03_2+ oder OLL40_4- (die Zahl steht für die Nummer der Ecke und + oder - steht für Drehungsrichtung)


    Nummerierung der Cases für die Tabelle (vorerst):
    (es gibt für Mid-Layer-Edges wenige Anordnungen: 0000,1000,0100,0010,0001,
    1100,1010,1001,0110,0101,0011,0111,1011,1101,1110,1111), dann 16 Kombinationen für oben und 16 Kombinationen für unten
    Bsp: 21022110_16_16_16 (natürlich sind aber nicht alle Kombinationen möglich)

    Noch eindeutiger wäre:

    21022110_1001_0100_0001 als Beispiel (VorteiL: Es wird alles auf einmal ersichtlich)

    Andere Art und Weise, die Ecken anzugeben:

    U2_Sune+2 oder U_Pi-1 (Erstmal AUF, dann Bezeichnung aus OLL, dann die eine Ecke, die nicht dazupasst)

    Optimiert auf recog (aber so will ich es ja eben nicht machen):

    Bezeichnung, Bezeichnung, bad edges positionen (1,2,3,4,5,6,7,8,9,a,b,c)

    Bsp: U2Antisune-4, U3Pi+1, 46ac

    Gute Mischung:

    21001221_1a9b


    Wie man das riesige EODR-Set in gute Subsets einteilen kann:
    Beispielsweise auf einer Seite (unten zum beispiel, dann kann man den Würfel rotieren) alle 4 corner orienten
    Dann wäre der upper bound: 3^3*478 = 12906 (sehr wesentlich weniger)
    4cxe kann man wahrscheinlich fast immer in weniger als 3 pre-moves erreichen (vermute ich)
    Außerdem kann man mit y2 die nicht-symetrischen Fälle streichen.
    Ich werde wohl zunächst mit den Code eine Liste mit all diesen Fällen generieren. Kategorisiert nach Ecken-Cases:
    (Memo-sIdee für später: Weil es 27 Fälle gibt für diese, könnte ich durch ausschließen von 3 Ecken-Fällen siehe nächster Abschnitt die Fälle einzelnen Buchstaben zuordnen)

        ------

    ###### das hier geht doch nicht, weil man die Edges mit einem y2 verändert: geht nur bei spiegelbaren oder y2-baren edge-cases, oder? geht doch######
    Prognose wie viele Cases man streichen kann:
        1020 zu y2+2010
        1020 und 0201 sind mirrors (L-Set OLL)
        2010 und 0102 sind mirrors (auch L-Set OLL)
    Also haben 1020, 2010, 0201, 0201 gegenseitige Abwandlungen von Algs. (L-Set)
    Auch: 1200 und 0012 (T-Set)
    Auch: 0120 und 2001 (T-Set)
    Auch: 2100 und 0021 (U-Set)
    Auch: 0210 und 1002 (U-Set)
    Auch: 2112 und 1221 (Pi-Set)
    Auch: 1122 und 2211 (Pi-Set)
    Auch: 0111, 1101, 2022 und 2220 (Sunes, ASunes)
    Auch: 1011, 1110, 0222 und 2202 (Sunes, ASunes)
    Also Eck-Cases von 3^3=27 um 15 Kategorien. Also bleiben nur noch 13 Kategorien.
    Es bleibt also ein upper-bound von:
    13*478=6214 Cases, was sehr überschaubar ist.
    
    Man könnte auf dieselbe Art und Weise aber auch die Edge-Cases reduzieren. Vllt wäre das sogar besser. Aber dann bleiben 27 Eck-Cases
    Kommt auf das Memo-Mapping an, was besser ist. Recon-mäßig wäre wahrscheinlich nicht schlecht die doppelten Edge-Cases zu reduzieren.

    ###### das hier geht doch nicht, weil man die Edges mit einem y2 verändert: geht nur bei spiegelbaren oder y2-baren edge-cases, oder?geht doch######

    Bad Edge-Unterteilung:
        2e: 8*4= 32 Fälle
            U-layer solved: 8 Fälle
            D-layer solved: 8 Fälle
        4e: 6(Anzahl Midlayercombs)*8*7/2 (Anpassung: geteilt durch 2 (weil 2 Permutationen der Lücken))= 168 Fälle
            U-layer solved: 36 Fälle
            D-layer solved: 36 Fälle
            Both layers unsolved: 96 Fälle
        6e: 4(Anzahl Midlayercombs)*8*7*6/(3*2*1)= 224 Fälle
            U-layer solved: 16 Fälle
            D-layer solved: 16 Fälle
            1unsolved in U, 2 unsolved in D: 4*4*6 = 96 Fälle
            2unsolved in U, 1 unsolved in D: 4*4*6 = 96 Fälle
        8e: 1*8*7*6*5/(4*3*2)= 70 Fälle
            U-solved: 1
            D-solved: 1
            1unsolvedinU, 3unsolvedinD: 16 Fälle
            2unsolved in each: 36 Fälle
            3unsolvedinU, 1unsolvedinU: 16 Fälle


        -----

    Nice bei EODR ist, dass auch EO-unfriendly moves gemacht weden können, wenn der alg eine Abkürzunb bietet. Wobei das eine programmiererweiterung wäre
    Informationsgehalt 1 8-mover-alg (wahrscheinlich realistisch für mein neues Alg-Set EODR) sind bei moveset <U*,D*,L*,R*, F2, B2>
    4 Letter Pairs. Also sind (ohne Ausgangszustand Cube!) 2,5 Algs ein Cube. 11502 Algs sind 4600 Cubes somit 4600 10LP-Locirooms,
    das sind 77 MBLD-WR-Memos. Das ist mir noch zu viel. Ziel wären zunächst eher 300 Räume oder 5 MBLD-WR-Memos.

    Man müsste die Move-Möglichkeiten besser verpacken. 5*3 Möglichkeiten gibt es immer für den nächsten Move (6*3 Möglichkeite mit nicht-floating move-bezeichnungen, also fixen buchstaben)
    
    Mapping eines 8-Movers in direkte Move-Buchstaben (nur 18 Buchstaben werden verwendet):
    4 Letter Pairs

    Mapping von zwei Moves gemeinsam:
    Alle Kombinationen von nur Letters (ohne direction oder double-bezeichnung) und zunächst werden nur RLUD betrachtet.

    Alle erlaubten moves, um auf DR zu kommen (noch ohne mit EO zu interferien):
    R RI R2 L LI L2 U UI U2 F FI F2 D DI D2 B2 F2 - 14 mögliche moves

    # Kombinationen zwei Moves nacheinander aus dem <U*,D*,L*,R*, F2, B2>Set: 12*9+2*12= 132 Kombinationen 

    => es macht keinen Unterschied bei zwei Moves nacheinander, ob man diese Mappt (sodass z.b. AA nicht vorkommt als letter pair)
    oder ob man zwei unabhängige Buchstaben hat. (196 vs. 132 Kombinationen) fallen beide auf zwei Letter als Memo

    Andere herangeehensweise mit "Maske":
    Es gibt 6 Faces, die möglich sind: FRULDB und dann 3 Zusätze: clockwise, ccws und double. Also eine Maske in der Form 01210221 (bei einem 8mover)
    Faces in der Form 01234021
    Also: 5^8 Möglichkeiten: 390625 (mit 4 Lettern, 331776 möglichekiten nicht abdeckbar, also 5 Letter nötig)
    3^8 Möglichkeiten für die Maske = 6561 (mit 3 lettern abdeckbar),
    so zu mappen lohnt sich also nicht. 
    
    Selbst 1zu1 mapping von 8movern (mit B2 und F2) lohnt sich nicht, da 7 Letter nötig wären. Man sollte einfach jedem Move einen Buchstaben zuordnen

    Codierung 8-Mover in <R RI R2 L LI L2 U UI U2 F FI F2 D DI D2>Set - nächster logischer Move hat immer 9 Möglichkeiten:
    Also 12*9*9*9 *9*9*9*9 = 57.395.628 Möglichkeiten (6 LPs max, wahrscheinlich aber nicht sehr nützlich, weil Übersetzungsvorgang ist wesentlich länger)
    

    Habe 24 Letter und somit 576 LPs

    UF UR UL UD
    FR FU FL FD
    RF RU RL RD
    LU LD LR LF

    Rotations:
    00 0I 02 01
    II ...
    
    Im Idealzustand kann man mit 2 LPs 331776 Möglichkeiten abdecken. Die Kunst ist es diese richtig zuzuordnen mit system als baum.



    Wie gut funktioniert der Filter?: 
    1. Filter: 0000er werden gelöscht: if(!isstatezero()
    Angezeigte Algs: 2304 von 2744
    2.Filter: if(!isstatezero() && (firstmove != secondmove) && (secondmove != thirdmove)
    Diesmal wo zwei moves nacheinander gleich sind.
    Angezeigte Algs: 2088 von 2744
    U und D waren bei den corners und den edges falsch. korrigiert und jetzt:
    Angezeigte Algs: 1460 von 2744
    4.Filter: plausibel filter.
    Angezeigte Algs: 1164 von 2744

 

    for(int z = 0; z <= dauer; z++){
        printf("%c", doku[z][0][0]); printf("%c", doku[z][0][1]);
        printf("%c", doku[z][1][0]); printf("%c", doku[z][1][1]);
        printf(", ");
    }
 alg0 firstmove 0 secondmove 0
 alg1 firstmove 0 secondmove 1
...
 algx firstmove 0 secondmove 13
 algxy firstmove 1 secondmove 0
...
 algxyz firstmove 1 secondmove 13
 algxyz13 firstmove 13 secondmove 13
 algzzz firstmove 0 secondmove 0


        for(int every = 0, eyery < 14, every++){
            MOVE(every); 
        }
        MOVE(every); 
        printf("\n"); 
        sol[algcount][counter][0] = move[counter][0]; sol[algcount][counter][1] = move[counter][1]; 
        
        printf("solution: %s\n", sol[counter]); MOVE(counter); MOVE(counter); MOVE(counter); //Triple Move könnte man mit setzero in der Zeit dritteln

    }
    S(); 
    printf("solution: %s%s\n", sol[algcount][0]); MOVE(counter); MOVE(counter); MOVE(counter); //Triple Move könnte man mit setzero in der Zeit dritteln

     //  if (counter ==  1){RI(); printf("\n"); sol[counter][0] = 'R';                        S(); printf("solution: %s\n", sol[counter]); R();}
     //  if (counter ==  2){R2(); printf("\n"); sol[counter][0] = 'R'; sol[counter][1] = '2'; S(); printf("solution: %s\n", sol[counter]); R2();}
     //  if (counter ==  3){U() ; printf("\n"); sol[counter][0] = 'U'; sol[counter][1] = 'I'; S(); printf("solution: %s\n", sol[counter]); UI();}
     //  if (counter ==  4){UI(); printf("\n"); sol[counter][0] = 'U';                      ; S(); printf("solution: %s\n", sol[counter]); U();}
     //  if (counter ==  5){U2(); printf("\n"); sol[counter][0] = 'U'; sol[counter][1] = '2'; S(); printf("solution: %s\n", sol[counter]); U2();}
     //  if (counter ==  6){L() ; printf("\n"); sol[counter][0] = 'L'; sol[counter][1] = 'I'; S(); printf("solution: %s\n", sol[counter]); LI();}
     //  if (counter ==  7){LI(); printf("\n"); sol[counter][0] = 'L';                      ; S(); printf("solution: %s\n", sol[counter]); L();}
     //  if (counter ==  8){L2(); printf("\n"); sol[counter][0] = 'L'; sol[counter][1] = '2'; S(); printf("solution: %s\n", sol[counter]); L2();}
     //  if (counter ==  9){D() ; printf("\n"); sol[counter][0] = 'D'; sol[counter][1] = 'I'; S(); printf("solution: %s\n", sol[counter]); DI();}
     //  if (counter == 10){DI(); printf("\n"); sol[counter][0] = 'D';                      ; S(); printf("solution: %s\n", sol[counter]); D();}
     //  if (counter == 11){D2(); printf("\n"); sol[counter][0] = 'D'; sol[counter][1] = '2'; S(); printf("solution: %s\n", sol[counter]); D2();}
     //  if (counter == 12){F2(); printf("\n"); sol[counter][0] = 'F'; sol[counter][1] = '2'; S(); printf("solution: %s\n", sol[counter]); F2();}
     //  if (counter == 13){B2(); printf("\n"); sol[counter][0] = 'B'; sol[counter][1] = '2'; S(); printf("solution: %s\n", sol[counter]); B2();}
     
     //Testlog
     //Scramble einen Würfel mit R U R' U' D L U2 R'
     // state müsste sein: 2002_1202__0101_1101_0001
     //habe mal im ersten generierten txt file paar Scrambles mit strg f gesucht, die ich aus der liste habe. einer kam 6 mal vor, ein anderer war
     //359mal doppelt, 131mal, 257mal, 

    //zu filter:
    // 01 02 10 20 21 12
    // dasselbe + 3
    // dasselbe + 6
    // dasselbe + 9

//geschwindigkeit: Aktuell bis secondmovealtereed etwa 30sek. Also insgesamat etwa 90min ladedauer komplett. Es werden unnötig viele scrambled gecheckt, die sich auf weniger moves reduzieren lassen.
//gesucht sind ja eig die reinen 8mover algs. bestimmt werden im o/x-Test 99,9999% der rückgaben reduzierbare algs sein.
//ich verändere nun den Algorithmus und überspringe den for-loop schon, wenn unplausibilität vorher nachgewiesen wird.
//doch etwas schneller://
///secondmovealtered
// Angezeigte Algs: 105413504 von 105413504done
//Done] exited with code=0 in 458.576 seconds
//drucker variable gefixt:
//jetzt habe ich printf von "o" entfernt und die bed. dass zwei moves nacheinander verschieden sein müssen:
//secondmovealtered
 //Angezeigte Algs: 891244 von 67575326done
//[Done] exited with code=0 in 289.273 seconds
//nun noch die plausibel bed.:
//secondmovealtered
//Angezeigte Algs: 891244 von 29438750done
//[Done] exited with code=0 in 137.57 seconds


Es gibt also 891 244 verschiedene plausible 8mover.
Habe das Layout der ausgegebenen tabelle verbessert

 Angezeigte Algs: 891244 von 29438750done
[Done] exited with code=0 in 148.244 seconds

nun werde ich das ganze entweder in ein Dokument printen lassen, oder als EODRv8 alle weniger-als-8mover finden. sollte von hier aus in 20min gehen.

interessant ist, dass nach 1021_1010__1010_1111_1100   Scramble: L  B2 DI B2 D2 RI B2 U    Alg: L  B2 DI B2 D2 RI B2 U  , keine weiteren scrambles mehr gefunden wurden, die nicht erreicht werden.
kann also sein, dass 891244 auch die anzahl am verschiedenen states ist. rechne ich mal nach.


8mover:   Angezeigte Algs: 891244 von 29438750 (evtl Bugs in dieser Zahl)done
        */


