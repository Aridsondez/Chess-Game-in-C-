#include <SFML/Graphics.hpp>
#include "game.hpp"
using namespace sf;


//Defining Variables 

#define LENGTH 7;//8*8 but index 0-7
//Defines the starting position of the pieces 
int WHITE_PAWN= -1;
int BLACK_PAWN=1;


int WHITE_ROOK= -2;
int BLACK_ROOK =2;
int WHITE_KNIGHT= -3;
int BLACK_KNIGHT =3;
int WHITE_BISHOP =-4;
int BLACK_BISHOP= 4;
int WHITE_QUEEN =-5;
int BLACK_QUEEN= 5;
int WHITE_KING =-6;
int BLACK_KING =6;

//Backend position 
struct poz{
    int x, y;
}old_poz, white_king,black_king,transform_white, transform_black;

int size=64, is_moving;

//Chest Board Back-End
int chest_board[8][8]=
{
    2, 3, 4, 5, 6, 4, 3, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1,-1,-1,-1,-1,-1,-1,-1,
    -2,-3,-4,-5,-6,-4,-3,-2
};//the chest board used to render all the tiles/postions 

Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15;//texture manager of SFML

int right_white_rook_m, left_white_rook_m, white_king_first_m;
int right_black_rook_m, left_black_rook_m, black_king_first_m;

int turn;// 0 white is moving 1 black is moving
int check_white, check_black;
int transformation_white, transformation_black;


//Movement of the chest pieces
//old and new positions
int pawn_w(int ox, int oy, int nx, int ny)
{
    if(old_poz.y==6)//checks if the pawn hasnt moved yet if so can move 2 spaces 
    {
        //checks if the move is legal and that there isn't any pieces in front
        if((ny==oy-1 && nx==ox && chest_board[ox-1][ox]==0) || (ny==oy-2&&nx==ox&&chest_board[oy-1][ox]==0&&chest_board[oy-2][ox]==0))
        {
            return 1;
        }
    }
    //moving the pawn forward one
    else if(ny==oy-1 && nx==ox && chest_board[ox-1][ox]==0){
        return 1;
    }
    //checks if there is a pieces to its left or right to take
    if (chest_board[oy-1][ox-1]>0){//all of blacks pieces are positive numbers 
        if(ny==oy-1 && nx==ox-1){//valid move to take left or right enemy
            return 1;
        }
    }

    if(chest_board[oy-1][ox+1]>0){
        if(ny== oy-1 && nx== ox+1){
            return 1;
        }
    }
    return 0;//if these are not true you cannot move the pawn  

}


int pawn_b(int ox, int oy, int nx, int ny)
{
    if(old_poz.y==1)//checks if the pawn hasnt moved yet if so can move 2 spaces 
    {
        //checks if the move is legal and that there isn't any pieces in front
        if((ny==oy+1 && nx==ox && chest_board[ox+1][ox]==0) || (ny==oy+2&&nx==ox&&chest_board[oy+1][ox]==0&&chest_board[oy+2][ox]==0))
        {
            return 1;
        }
    }
    //moving the pawn forward one
    else if(ny==oy+1 && nx==ox && chest_board[ox+1][ox]==0){
        return 1;
    }
    //checks if there is a pieces to its left or right to take
    if (chest_board[oy+1][ox-1]<0){//all of white pieces are negative numbers 
        if(ny==oy+1 && nx==ox-1){//valid move to take left or right enemy
            return 1;
        }
    }

    if(chest_board[oy+1][ox+1]<0){
        if(ny== oy+1 && nx== ox+1){
            return 1;
        }
    }
    return 0;
}

//rooks
int rook_w(int ox, int oy, int nx, int ny)
{
    /*Have to check every sqaure until we are blocked by a piece*/
    for(int i=ox-1;i>=0;i--){

        if(chest_board[oy][i]>=0 && (nx==i && ny==oy)){
            return 1;//if moving to the left and there is a opposite piece
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for (int i=oy-1; i>=0; i--){//going up
        
        if(chest_board[i][ox]>=0 && (ny==i && nx==ox)){
            return 1; //moving down until you reach an opposite piece
        }
        else if(chest_board[i][ox]!=0){
            break;//there is a piece blocking 
        }
    }
    for(int i=ox+1;i<=7;i++){//to the right
        if(chest_board[oy][i]>=0 && (ny==oy && nx==i)){
            return 1;
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for(int i=oy+1;i<=7;i++){//going down

        if(chest_board[i][ox]>=0 && (ny==i && nx==ox)){
            return 1;
        }
        else if(chest_board[i][ox]!=0){
            break;
        }
    }
    return 0;
}
int rook_b(int ox, int oy, int nx, int ny)
{
    /*Have to check every sqaure until we are blocked by a piece*/
    for(int i=ox-1;i>=0;i--){

        if(chest_board[oy][i]<=0 && (nx==i && ny==oy)){
            return 1;//if moving to the left and there is a opposite piece
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for (int i=oy-1; i>=0; i--){//going up
        
        if(chest_board[i][ox]<=0 && (ny==i && nx==ox)){
            return 1; //moving down until you reach an opposite piece
        }
        else if(chest_board[i][ox]!=0){
            break;//there is a piece blocking 
        }
    }
    for(int i=ox+1;i<=7;i++){//to the right
        if(chest_board[oy][i]<=0 && (ny==oy && nx==i)){
            return 1;
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for(int i=oy+1;i<=7;i++){//going down

        if(chest_board[i][ox]<=0 && (ny==i && nx==ox)){
            return 1;
        }
        else if(chest_board[i][ox]!=0){
            break;
        }
    }
    return 0;
}

//bishops
int bishop_w(int ox, int oy, int nx, int ny)
{
    int j=ox-1;
    for(int i=oy-1; i>=0;i--){
        if(chest_board[i][j]>=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }
    
    j=ox+1;
    for(int i=oy-1; i>=0; i--){
        if(chest_board[i][j]>=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    
    j=ox-1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]>=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }

    j=ox+1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]>=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    return 0;
}
int bishop_b(int ox, int oy, int nx, int ny)
{
    int j=ox-1;
    for(int i=oy-1; i>=0;i--){
        if(chest_board[i][j]<=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }
    
    j=ox+1;
    for(int i=oy-1; i>=0; i--){
        if(chest_board[i][j]<=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    
    j=ox-1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]<=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }

    j=ox+1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]<=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    return 0;
}

//knight
int knight_w(int ox, int oy, int nx, int ny){

    if( oy-2 >=0 && ox-1>=0 && ny==oy-2 && nx==ox-1 && chest_board[ny][nx]>=0){
        return 1;
    }

    if (oy-2>=0 && ox+1 <=7 && ny==oy-2 && nx==ox+1 && chest_board[ny][nx]>=0){
        return 1;
    }

    if(oy-1>= 0 && ox+2 <=7 && ny==oy-1 && nx==ox+2 && chest_board[ny][nx]>=0){
        return 1;
    }

    if(oy+1>= 7 && ox+2 <= 7 && ny==oy+1 && nx==ox+2 && chest_board[ny][nx]>=0){
        return 1;
    }

    if(oy+2>= 7 && ox+1 <= 7 && ny==oy+2 && nx==ox+1 && chest_board[ny][nx]>=0){
        return 1;
    }

    if(oy+2 >= 7 && ox-1>= 0 && ny==oy+2 && nx==ox-1 && chest_board[ny][nx]>=0){
        return 1;
    }

    if(oy+1 <= 7 && ox-2>= 0&& ny==oy+1 && nx==ox-2 && chest_board[ny][nx]>=0){
        return 1; 
    }

    if (oy-1 >=0 && ox-2>=0 && ny==oy-1 && nx==ox-2 && chest_board[ny][nx]>=0){
        return 1; 
    }
    return 0;


}
int knight_b(int ox, int oy, int nx, int ny){
    if( oy-2 >=0 && ox-1>=0 && ny==oy-2 && nx==ox-1 && chest_board[ny][nx]<=0){
        return 1;
    }

    if (oy-2>=0 && ox+1 <=7 && ny==oy-2 && nx==ox+1 && chest_board[ny][nx]<=0){
        return 1;
    }

    if(oy-1>= 0 && ox+2 <=7 && ny==oy-1 && nx==ox+2 && chest_board[ny][nx]<=0){
        return 1;
    }

    if(oy+1>= 7 && ox+2 <= 7 && ny==oy+1 && nx==ox+2 && chest_board[ny][nx] <=0){
        return 1;
    }

    if(oy+2>= 7 && ox+1 <= 7 && ny==oy+2 && nx==ox+1 && chest_board[ny][nx] <=0){
        return 1;
    }

    if(oy+2 >= 7 && ox-1>= 0 && ny==oy+2 && nx==ox-1 && chest_board[ny][nx] <=0){
        return 1;
    }

    if(oy+1 <= 7 && ox-2>= 0&& ny==oy+1 && nx==ox-2 && chest_board[ny][nx] <=0){
        return 1; 
    }

    if (oy-1 >=0 && ox-2>=0 && ny==oy-1 && nx==ox-2 && chest_board[ny][nx]<=0){
        return 1; 
    }
    return 0;

}   

//queens
int queen_w(int ox, int oy, int nx, int ny){
    /*Have to check every sqaure until we are blocked by a piece*/
    for(int i=ox-1;i>=0;i--){

        if(chest_board[oy][i]>=0 && (nx==i && ny==oy)){
            return 1;//if moving to the left and there is a opposite piece
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for (int i=oy-1; i>=0; i--){//going up
        
        if(chest_board[i][ox]>=0 && (ny==i && nx==ox)){
            return 1; //moving down until you reach an opposite piece
        }
        else if(chest_board[i][ox]!=0){
            break;//there is a piece blocking 
        }
    }
    for(int i=ox+1;i<=7;i++){//to the right
        if(chest_board[oy][i]>=0 && (ny==oy && nx==i)){
            return 1;
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for(int i=oy+1;i<=7;i++){//going down

        if(chest_board[i][ox]>=0 && (ny==i && nx==ox)){
            return 1;
        }
        else if(chest_board[i][ox]!=0){
            break;
        }
    }
    int j=ox-1;
    for(int i=oy-1; i>=0;i--){
        if(chest_board[i][j]>=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }
    
    j=ox+1;
    for(int i=oy-1; i>=0; i--){
        if(chest_board[i][j]>=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    
    j=ox-1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]>=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }

    j=ox+1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]>=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    return 0;
    
}
int queen_b(int ox, int oy, int nx, int ny){
     /*Have to check every sqaure until we are blocked by a piece*/
    for(int i=ox-1;i>=0;i--){

        if(chest_board[oy][i]<=0 && (nx==i && ny==oy)){
            return 1;//if moving to the left and there is a opposite piece
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for (int i=oy-1; i>=0; i--){//going up
        
        if(chest_board[i][ox]<=0 && (ny==i && nx==ox)){
            return 1; //moving down until you reach an opposite piece
        }
        else if(chest_board[i][ox]!=0){
            break;//there is a piece blocking 
        }
    }
    for(int i=ox+1;i<=7;i++){//to the right
        if(chest_board[oy][i]<=0 && (ny==oy && nx==i)){
            return 1;
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for(int i=oy+1;i<=7;i++){//going down

        if(chest_board[i][ox]<=0 && (ny==i && nx==ox)){
            return 1;
        }
        else if(chest_board[i][ox]!=0){
            break;
        }
    }
    int j=ox-1;
    for(int i=oy-1; i>=0;i--){
        if(chest_board[i][j]<=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }
    
    j=ox+1;
    for(int i=oy-1; i>=0; i--){
        if(chest_board[i][j]<=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    
    j=ox-1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]<=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }

    j=ox+1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]<=0 && (ny==1 && nx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    return 0;

}


//all possible checks for white 

int pawn_w_check(int ox,int oy, int kingx,int kingy){
    if (chest_board[oy-1][ox-1]>=0){

        if(oy-1 == kingy && ox-1 ==kingx){
            return 1;
        }
    }
    if (chest_board[oy-1][ox+1]>=0){

        if(oy-1==kingy && ox+1==kingx){
            return 1;
        }
    }
    return 0;
}

int rook_w_check(int ox,int oy, int kingx,int kingy){

    for(int i=ox-1;i>=0;i--){

        if(chest_board[oy][i]>=0 && (kingx==i && kingy==oy)){
            return 1;//if moving to the left and there is a opposite piece
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for (int i=oy-1; i>=0; i--){//going up
        
        if(chest_board[i][ox]>=0 && (kingy==i && kingx==ox)){
            return 1; //moving down until you reach an opposite piece
        }
        else if(chest_board[i][ox]!=0){
            break;//there is a piece blocking 
        }
    }
    for(int i=ox+1;i<=7;i++){//to the right
        if(chest_board[oy][i]>=0 && (kingy==oy && kingx==i)){
            return 1;
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for(int i=oy+1;i<=7;i++){//going down

        if(chest_board[i][ox]>=0 && (kingy==i && kingx==ox)){
            return 1;
        }
        else if(chest_board[i][ox]!=0){
            break;
        }
    }
    return 0;

}

int bishop_w_check(int ox, int oy, int kingx, int kingy){

    int j=ox-1;
    for(int i=oy-1; i>=0;i--){
        if(chest_board[i][j]>=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }
    
    j=ox+1;
    for(int i=oy-1; i>=0; i--){
        if(chest_board[i][j]>=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    
    j=ox-1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]>=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }

    j=ox+1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]>=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    return 0;
}

int queen_w_check(int ox, int oy, int kingx, int kingy){
    for(int i=ox-1;i>=0;i--){

        if(chest_board[oy][i]>=0 && (kingx==i && kingy==oy)){
            return 1;//if moving to the left and there is a opposite piece
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for (int i=oy-1; i>=0; i--){//going up
        
        if(chest_board[i][ox]>=0 && (kingy==i && kingx==ox)){
            return 1; //moving down until you reach an opposite piece
        }
        else if(chest_board[i][ox]!=0){
            break;//there is a piece blocking 
        }
    }
    for(int i=ox+1;i<=7;i++){//to the right
        if(chest_board[oy][i]>=0 && (kingy==oy && kingx==i)){
            return 1;
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for(int i=oy+1;i<=7;i++){//going down

        if(chest_board[i][ox]>=0 && (kingy==i && kingx==ox)){
            return 1;
        }
        else if(chest_board[i][ox]!=0){
            break;
        }
    }
    int j=ox-1;
    for(int i=oy-1; i>=0;i--){
        if(chest_board[i][j]>=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }
    
    j=ox+1;
    for(int i=oy-1; i>=0; i--){
        if(chest_board[i][j]>=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    
    j=ox-1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]>=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }

    j=ox+1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]>=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    return 0;
}

int knight_w_check(int ox, int oy, int kingx, int kingy){
    if( oy-2 >=0 && ox-1>=0 && kingy==oy-2 && kingx==ox-1 && chest_board[kingy][kingx]>=0){
        return 1;
    }

    if (oy-2>=0 && ox+1 <=7 && kingy==oy-2 && kingx==ox+1 && chest_board[kingy][kingx]>=0){
        return 1;
    }

    if(oy-1>= 0 && ox+2 <=7 && kingy==oy-1 && kingx==ox+2 && chest_board[kingy][kingx]>=0){
        return 1;
    }

    if(oy+1>= 7 && ox+2 <= 7 && kingy==oy+1 && kingx==ox+2 && chest_board[kingy][kingx]>=0){
        return 1;
    }

    if(oy+2>= 7 && ox+1 <= 7 && kingy==oy+2 && kingx==ox+1 && chest_board[kingy][kingx]>=0){
        return 1;
    }

    if(oy+2 >= 7 && ox-1>= 0 && kingy==oy+2 && kingx==ox-1 && chest_board[kingy][kingx]>=0){
        return 1;
    }

    if(oy+1 <= 7 && ox-2>= 0&& kingy==oy+1 && kingx==ox-2 && chest_board[kingy][kingx]>=0){
        return 1; 
    }

    if (oy-1 >=0 && ox-2>=0 && kingy==oy-1 && kingx==ox-2 && chest_board[kingy][kingx]>=0){
        return 1; 
    }
    return 0;

}

int king_w_check(int ox, int oy, int kingx, int kingy){

    if(ox-1>=0 && oy-1>=0 && kingy==oy-1 && kingx==ox-1 && chest_board[kingy][kingx]<=0){
        return 1;
    }

    if(oy-1>=0 && kingy==ox && kingy==oy -1 && chest_board[kingy][kingx]<=0){
        return 1;
    }

    if(oy-1>=0 && ox-1<7 && kingx==ox+1 && kingy==oy-1 && chest_board[kingy][kingx]<=0){
        return 1;
    }

    if(ox+1<=7 && kingy==oy && kingx==ox+1 && chest_board[kingy][kingx]<=0){
        return 1;
    }

    if(ox+1 <= 7 &&  oy+1<=7 && kingy==oy+1 && kingx==ox+1 && chest_board[kingy][kingx]<=0){
        return 1;
    }

    if(oy +1 <=7 && kingy==oy+1 && kingx==ox && chest_board[kingy][kingx]<=0){
        return 1;
    }

    if(ox-1>=0 && oy+1 <= 7 && kingx==ox+1 && kingy== oy+1 && chest_board[kingy][kingx]<=0){
        return 1;
    }

    if(ox-1>=0 && kingy==oy && kingx==ox-1&& chest_board[kingy][kingx]<=0){
        return 1;
    }
    return 0;
}

//all possible checks for black

int pawn_b_check(int ox, int oy, int kingx, int kingy){
    if(chest_board[oy+1][ox-1]<=0){
        if(kingy==oy +1 && kingx==ox -1){
            return 1;
        }
    }

    if(chest_board[oy+1][ox+1]<=0){
        if(kingy==oy +1 && kingx==ox+1){
            return 1;
        }
    }
    return 0;

}

int rook_b_check(int ox, int oy, int kingx, int kingy){

     for(int i=ox-1;i>=0;i--){

        if(chest_board[oy][i]<=0 && (kingx==i && kingy==oy)){
            return 1;//if moving to the left and there is a opposite piece
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for (int i=oy-1; i>=0; i--){//going up
        
        if(chest_board[i][ox]<=0 && (kingy==i && kingx==ox)){
            return 1; //moving down until you reach an opposite piece
        }
        else if(chest_board[i][ox]!=0){
            break;//there is a piece blocking 
        }
    }
    for(int i=ox+1;i<=7;i++){//to the right
        if(chest_board[oy][i]<=0 && (kingy==oy && kingx==i)){
            return 1;
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for(int i=oy+1;i<=7;i++){//going down

        if(chest_board[i][ox]<=0 && (kingy==i && kingx==ox)){
            return 1;
        }
        else if(chest_board[i][ox]!=0){
            break;
        }
    }
    return 0;
}

int bishop_b_check(int ox, int oy, int kingx, int kingy){
    int j=ox-1;
    for(int i=oy-1; i>=0;i--){
        if(chest_board[i][j]<=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }
    
    j=ox+1;
    for(int i=oy-1; i>=0; i--){
        if(chest_board[i][j]<=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    
    j=ox-1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]<=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }

    j=ox+1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]<=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    return 0;
}

int queen_b_check(int ox, int oy, int kingx, int kingy){

    for(int i=ox-1;i>=0;i--){

        if(chest_board[oy][i]<=0 && (kingx==i && kingy==oy)){
            return 1;//if moving to the left and there is a opposite piece
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for (int i=oy-1; i>=0; i--){//going up
        
        if(chest_board[i][ox]<=0 && (kingy==i && kingx==ox)){
            return 1; //moving down until you reach an opposite piece
        }
        else if(chest_board[i][ox]!=0){
            break;//there is a piece blocking 
        }
    }
    for(int i=ox+1;i<=7;i++){//to the right
        if(chest_board[oy][i]<=0 && (kingy==oy && kingx==i)){
            return 1;
        }
        else if(chest_board[oy][i]!=0){
            break;
        }
    }
    for(int i=oy+1;i<=7;i++){//going down

        if(chest_board[i][ox]<=0 && (kingy==i && kingx==ox)){
            return 1;
        }
        else if(chest_board[i][ox]!=0){
            break;
        }
    }
    int j=ox-1;
    for(int i=oy-1; i>=0;i--){
        if(chest_board[i][j]<=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }
    
    j=ox+1;
    for(int i=oy-1; i>=0; i--){
        if(chest_board[i][j]<=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    
    j=ox-1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]<=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j--;
    }

    j=ox+1;
    for(int i=oy+1;i<=7;i++){
        if(chest_board[i][j]<=0 && (kingy==1 && kingx==j)){
            return 1;
        }
        else if(chest_board[i][j]!=0){
            break;
        }
        j++;
    }
    return 0;
}

int knight_b_check(int ox, int oy, int kingx, int kingy){
    if( oy-2 >=0 && ox-1>=0 && kingy==oy-2 && kingx==ox-1 && chest_board[kingy][kingx]<=0){
        return 1;
    }

    if (oy-2>=0 && ox+1 <=7 && kingy==oy-2 && kingx==ox+1 && chest_board[kingy][kingx]<=0){
        return 1;
    }

    if(oy-1>= 0 && ox+2 <=7 && kingy==oy-1 && kingx==ox+2 && chest_board[kingy][kingx]<=0){
        return 1;
    }

    if(oy+1>= 7 && ox+2 <= 7 && kingy==oy+1 && kingx==ox+2 && chest_board[kingy][kingx]<=0){
        return 1;
    }

    if(oy+2>= 7 && ox+1 <= 7 && kingy==oy+2 && kingx==ox+1 && chest_board[kingy][kingx]<=0){
        return 1;
    }

    if(oy+2 >= 7 && ox-1>= 0 && kingy==oy+2 && kingx==ox-1 && chest_board[kingy][kingx]<=0){
        return 1;
    }

    if(oy+1 <= 7 && ox-2>= 0&& kingy==oy+1 && kingx==ox-2 && chest_board[kingy][kingx]<=0){
        return 1; 
    }

    if (oy-1 >=0 && ox-2>=0 && kingy==oy-1 && kingx==ox-2 && chest_board[kingy][kingx]<=0){
        return 1; 
    }
    return 0;
}

int king_b_check(int ox, int oy, int kingx, int kingy){
    if(ox-1>=0 && oy-1>=0 && kingy==oy-1 && kingx==ox-1 && chest_board[kingy][kingx]>=0){
        return 1;
    }

    if(oy-1>=0 && kingx==ox && kingy==oy -1 && chest_board[kingy][kingx]>=0){
        return 1;
    }

    if(oy-1>=0 && ox+1<=7 && kingx==ox+1 && kingy==oy-1 && chest_board[kingy][kingx]>=0){
        return 1;
    }

    if(ox+1<=7 && kingy==oy && kingx==ox+1 && chest_board[kingy][kingx]>=0){
        return 1;
    }

    if(ox+1 <= 7 &&  oy+1<=7 && kingy==oy+1 && kingx==ox+1 && chest_board[kingy][kingx]>=0){
        return 1;
    }

    if(oy +1 <=7 && kingy==oy+1 && kingx==ox && chest_board[kingy][kingx]>=0){
        return 1;
    }

    if(ox-1>=0 && oy+1 <= 7 && kingx==ox+1 && kingy== oy+1 && chest_board[kingy][kingx]>=0){
        return 1;
    }

    if(ox-1>=0 && kingy==oy && kingx==ox-1&& chest_board[kingy][kingx]>=0){
        return 1;
    }
    return 0;
}

int black_king_is_check(int poskingx, int poskingy){

    int ok=0;
    for (int i=0; i<=7; i++){
        for(int j=0; j<= 7; j++){

            if(chest_board[i][j]<=0){

               

                if(chest_board[i][j]==-1){//white pawn
                    ok=pawn_w_check(j,i,poskingx,poskingy);
                }

               

                if(chest_board[i][j]==-2){
                    ok=rook_w_check(j,i,poskingx,poskingy);
                }

               
                if (chest_board[i][j]==-3){
                    ok=knight_w_check(j,i,poskingx,poskingy);
                }
               
                if(chest_board[i][j]==-4 ){
                    ok=bishop_w_check(j,i,poskingx,poskingy);
                }

               

                if(chest_board[i][j]==-5 ){
                    ok=queen_w_check(j,i,poskingx,poskingy);
                }


                if(chest_board[i][j]==-6){
                    ok=king_w_check(j,i,poskingx,poskingy);
                }

                if(ok==1){
                    return 0;
                }
            }
        }   
    }
    return 1;
}

int white_king_is_check(int poskingx, int poskingy){

    int ok=1;

    for(int i; i<=7; i++){
        for (int j; j<=7; j++){

            if(chest_board[i][j]>0){
                if(chest_board[i][j]==1){//black pawn
                    ok=pawn_b_check(j,i,poskingx,poskingy);
                }

               

                if(chest_board[i][j]==2){
                    ok=rook_b_check(j,i,poskingx,poskingy);
                }

               
                if (chest_board[i][j]==3){
                    ok=knight_b_check(j,i,poskingx,poskingy);
                }
               
                if(chest_board[i][j]==4 ){
                    ok=bishop_b_check(j,i,poskingx,poskingy);
                }

               

                if(chest_board[i][j]==5 ){
                    ok=queen_b_check(j,i,poskingx,poskingy);
                }


                if(chest_board[i][j]==6){
                    ok=king_b_check(j,i,poskingx,poskingy);
                }

                if(ok==1){
                    return 0;
                }
            }
        }
    }
    return 1;
}
//kings 
int king_w(int ox, int oy, int nx, int ny){
    if(ox-1>=0 && oy-1>=0 && ny==oy-1 && nx==ox-1 && chest_board[ny][nx]>=0){
        int ok=white_king_is_check(ox-1,oy-1);
        if(ok==1){
            return 1;
        }
    }

    if(oy-1>=0 && nx==ox && ny==oy -1 && chest_board[ny][nx]>=0){
        int ok=white_king_is_check(ox,oy-1);
        if(ok==1){
            return 1;
        }
    }

    if(oy-1>=0 && ox+1<=7 && nx==ox+1 && ny==oy-1 && chest_board[ny][nx]>=0){
        int ok=white_king_is_check(ox+1,oy-1);
        if(ok==1){
            return 1;
        }
    }

    if(ox+1<=7 && ny==oy && nx==ox+1 && chest_board[ny][nx]>=0){
        int ok=white_king_is_check(ox+1,oy);  
        if(ok==1){
            return 1;
        }
    }

    if(ox+1 <= 7 &&  oy+1<=7 && ny==oy+1 && nx==ox+1 && chest_board[ny][nx]>=0){
        int ok=white_king_is_check(ox+1,oy+1);
        if(ok==1){
            return 1;
        }
    }

    if(oy +1 <=7 && ny==oy+1 && nx==ox && chest_board[ny][nx]>=0){
        int ok=white_king_is_check(ox,oy+1);
        if(ok==1){
            return 1;
        }
    }

    if(ox-1>=0 && oy+1 <= 7 && nx==ox+1 && ny== oy+1 && chest_board[ny][nx]>=0){
        int ok=white_king_is_check(ox-1,oy+1);
        if(ok==1){
            return 1;
        }
    }

    if(ox-1>=0 && ny==oy && nx==ox-1&& chest_board[ny][nx]>=0){
        int ok=white_king_is_check(ox-1,oy);
        if(ok==1){
            return 1;
        }
    }
    //castling
    if(right_white_rook_m==0&&white_king_first_m==0&&chest_board[7][6]==0&& chest_board[7][6]==0&& ny==7 && nx==6){
        int ok=white_king_is_check(4,7);
        if(ok==1){
            ok=white_king_is_check(5,7);
            if(ok==1){
                ok=white_king_is_check(6,7);
                if(ok==1){
                    chest_board[7][7]=0;
                    chest_board[7][5]=WHITE_ROOK;
                    white_king_first_m=1;
                    right_white_rook_m=1;
                    return 1;

                }

            }       
        }
    }
    if(left_white_rook_m==0 && white_king_first_m==0 && chest_board[7][3]==0&& chest_board[7][2]==0&&chest_board[7][1]==0&& ny==7 && nx==2){
        int ok=white_king_is_check(4,7);
        if(ok==1){
            ok=white_king_is_check(3,7);
            if(ok==1){
                ok=white_king_is_check(2,7);
                if(ok==1){
                    ok=white_king_is_check(1,7);{
                        if(ok==1){
                            chest_board[7][0]=0;
                            chest_board[7][3]=WHITE_ROOK;
                            white_king_first_m=1;
                            left_white_rook_m=1;
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int king_b(int ox, int oy, int nx, int ny){
    if(ox-1>=0 && oy-1>=0 && ny==oy-1 && nx==ox-1 && chest_board[ny][nx]<=0){
        int ok=black_king_is_check(ox-1,oy-1);
        if(ok==1){
            return 1;
        }
    }

    if(oy-1>=0 && nx==ox && ny==oy -1 && chest_board[ny][nx]<=0){
        int ok=black_king_is_check(ox,oy-1);
        if(ok==1){
            return 1;
        }
    }

    if(oy-1>=0 && ox+1<=7 && nx==ox+1 && ny==oy-1 && chest_board[ny][nx]<=0){
        int ok=black_king_is_check(ox+1,oy-1);
        if(ok==1){
            return 1;
        }
    }

    if(ox+1<=7 && ny==oy && nx==ox+1 && chest_board[ny][nx]<=0){
        int ok=black_king_is_check(ox+1,oy);  
        if(ok==1){
            return 1;
        }
    }

    if(ox+1 <= 7 &&  oy+1<=7 && ny==oy+1 && nx==ox+1 && chest_board[ny][nx]<=0){
        int ok=black_king_is_check(ox+1,oy+1);
        if(ok==1){
            return 1;
        }
    }

    if(oy +1 <=7 && ny==oy+1 && nx==ox && chest_board[ny][nx]<=0){
        int ok=black_king_is_check(ox,oy+1);
        if(ok==1){
            return 1;
        }
    }

    if(ox-1>=0 && oy+1 <= 7 && nx==ox+1 && ny== oy+1 && chest_board[ny][nx]<=0){
        int ok=black_king_is_check(ox-1,oy+1);
        if(ok==1){
            return 1;
        }
    }

    if(ox-1>=0 && ny==oy && nx==ox-1&& chest_board[ny][nx]<=0){
        int ok=black_king_is_check(ox-1,oy);
        if(ok==1){
            return 1;
        }
    }
    //castling
    if(right_black_rook_m==0 && black_king_first_m==0 && chest_board[0][5]==0 && chest_board[0][6]==0 && ny==0 && nx==6){
        int ok=black_king_is_check(4,0);
        if(ok==1){
            ok=black_king_is_check(5,0);
            if(ok==1){
                ok=black_king_is_check(6,0);
                if(ok==1){
                    black_king_first_m=1;
                    right_black_rook_m=1;
                    chest_board[0][7]=0;
                    chest_board[0][5]=BLACK_ROOK;
                    return 1;
                }
            }
        }
    }
    if(left_black_rook_m ==0 && black_king_first_m==0 && chest_board[0][3]==0 && chest_board[0][2]==0&& chest_board[0][1]==0 && ny==0 && nx==2){
        int ok=black_king_is_check(4,0);
        if(ok==1){
            ok=black_king_is_check(3,0);
            if(ok==1){
                ok=black_king_is_check(2,0);
                if(ok==1){
                    ok=black_king_is_check(1,0);
                    if(ok==1){
                        black_king_first_m=1;
                        left_black_rook_m=1;
                        chest_board[0][0]=0;
                        chest_board[0][3]=BLACK_ROOK;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}


void pos_white_king(){
    for(int i=0;i<=7;i++){
        for(int j=0; j<=7; j++){
            if (chest_board[i][j]==-6){
                white_king.x=j;
                white_king.y=i;
                break;
            }
        }
    }

}

void pos_black_king(){
    for(int i=0;i<=7;i++){
        for(int j=0; j<=7; j++){
            if (chest_board[i][j]==6){
                black_king.x=j;
                black_king.y=i;
                break;
            }
        }
    }
}


Game::Game(){
    screen_width=512;
    screen_height=512;

}


void Game::run(){
    game_state=1;
    init(0,0,screen_width, screen_height, "Chess Game");
}

/*Loads all the images to each piece needed for the game*/
void Game::textures(){

    t1.loadFromFile("images/WoodChestBoard.png");//The chest board
    t2.loadFromFile("images/64_12.png"); //The white pawns
    t3.loadFromFile("images/64_06.png"); //The black pawns
    t4.loadFromFile("images/64_07.png"); //The white rook
    t5.loadFromFile("images/64_01.png"); //The black rook
    t6.loadFromFile("images/64_08.png"); //The white knight
    t7.loadFromFile("images/64_02.png"); //The black knight
    t8.loadFromFile("images/64_09.png"); //The white bishop
    t9.loadFromFile("images/64_03.png"); //The black bishop
    t10.loadFromFile("images/64_10.png"); //The white queen
    t11.loadFromFile("images/64_04.png"); //The black queen
    t12.loadFromFile("images/64_11.png"); //THe white king
    t13.loadFromFile("images/64_05.png"); //the black king 
    t14.loadFromFile("images/"); //Transforming white
    t15.loadFromFile("images/");//Transforming black 


}

//The main function of the game 
//Reason: Window declaration can only be accessed in the same function
void Game::init ( int x, int y, int h, int w, const char title[11]){
    RenderWindow window(VideoMode(h, w), title);//drawing the whole screen

    //Loading all the pieces of the game
    textures();
    //the tile 
    Sprite table(t1);
    //drawing the white pawns 8
    Sprite pawn_white(t2);
    //drawing the black pawns 8
    Sprite pawn_black(t3);


    //Drawing all the pieces for black
    Sprite rook_black(t5), bishop_black(t9), knight_black(t7), queen_black(t11), 
    king_black(t13);

    //Drawing all the pieces for white
    Sprite rook_white(t4), bishop_white(t8), knight_white(t6), queen_white(t10),
    king_white(t12);

    Sprite move_images;
    Sprite white_transformation(t14);
    Sprite black_transformation(t5);
    //sprite move images
    
    //Putting all the pieces in their positions 



    pawn_white.move(sf::Vector2f(0.f, 50.f));
    table.setScale(1,1);









    float dx=0, dy=0;
    int noMovedPiece=0;



    //Runnig the game
    while (window.isOpen()){

        //getting the mouse posistion white the window is open
        Vector2i pos=Mouse::getPosition(window);

        //Provides the backend data for the moving pieces 
        int x= pos.x/size;
        int y=pos.y/size;

        Event event;

        while(window.pollEvent(event)){
            if(event.type==Event::Closed){
                window.close();
            }
        }

        window.clear();
            if(event.type==Event::MouseButtonPressed){
                if(event.key.code==Mouse::Left){

                    //transform
                    if (transformation_white==1){
                        if(pos.y>transform_white.y*size&& pos.y<=(transform_white.y+1)* size && pos.x>=transform_white.x*size && pos.x <=(transform_white.x+1)*size){
                            int xx=pos.x%size, yy=pos.y%size;
                            if(xx<32 && yy<32 &&xx> 0 && yy>0){
                                chest_board[transform_white.y][transform_white.x]=WHITE_ROOK;
                                transformation_white=0;
                            }
                            if(xx > 32 && xx < 64 && yy > 32 && yy < 64){
                                chest_board[transform_white.y][transform_white.x]=WHITE_KNIGHT;
                                transformation_white=0;
                            }
                            if(xx<32 && xx>0 && yy>32 && yy< 64 ){
                                chest_board[transform_white.y][transform_white.x]=WHITE_BISHOP;
                                transformation_white=0;
                            }
                            if(transformation_white==0){
                                pos_black_king();
                                int h=black_king_is_check(black_king.x, black_king.y);
                                if(h==0){
                                    check_black=1;
                                }
                            }
                            
                        }
                    }

                    if(transformation_black==1){
                        if(pos.y>= transform_black.y*size && pos.y<=(transform_black.y+1)*size&& pos.x>= transform_black.x*size&& pos.x<=(transform_black.x+1)*size){
                            int xx=pos.x%size, yy=pos.y%size;
                            if(xx<32 && y< 32 && xx> 0 && yy>0){
                                chest_board[transform_black.y][transform_black.x]=BLACK_ROOK;
                                transformation_black=0;
                            }
                            if(xx>50&& xx<64&&yy<32&&yy>0){
                                chest_board[transform_black.y][transform_black.x]=BLACK_QUEEN;
                                transformation_black=0;
                            }
                            if(xx>50 && xx<64 && y>32 && yy<64){
                                chest_board[transform_black.y][transform_black.x]=BLACK_KNIGHT;
                                transformation_black=0;
                            }
                            if(xx<64 && xx>0 && yy>50 && yy<64){
                                chest_board[transform_black.y][transform_black.x]=BLACK_BISHOP;
                                transformation_black=0;
                            }
                            if(transformation_black==0){
                                pos_white_king();
                                int h=white_king_is_check(white_king.x,white_king.y);
                                if(h==1){
                                    check_white=1;
                                }
                            }
                        }
                    }
                    if(chest_board[y][x]!=0){
                        /*Checks if there is a pieced on the board
                        where the mouse is clicked*/
                        dx=pos.x-x*size;
                        dy=pos.y-y*size;

                        if((chest_board[y][x]==BLACK_PAWN) && (turn == 1)){
                            noMovedPiece=BLACK_PAWN;
                            move_images= pawn_black;
                            chest_board[y][x]=0;
                        }

                        if((chest_board[y][x]==-1) && (turn == 0)){
                            noMovedPiece=WHITE_PAWN;
                            move_images=pawn_white;
                            chest_board[y][x]=0;
                        }

                        if((chest_board[y][x]==2) && (turn == 1)){
                            noMovedPiece=BLACK_ROOK;
                            move_images=rook_black;
                            chest_board[y][x]=0;
                        }

                        if((chest_board[y][x]==-2) && (turn == 0)){
                            noMovedPiece=WHITE_ROOK;
                            move_images=rook_white;
                            chest_board[y][x]=0;
                        }

                        if((chest_board[y][x]==3) && (turn ==1)){
                            noMovedPiece=BLACK_KNIGHT;
                            move_images=knight_black;
                            chest_board[y][x]=0;
                        }
                        if ((chest_board[y][x]==-3)&& (turn == 0)){
                            noMovedPiece=WHITE_KNIGHT;
                            move_images=knight_white;
                            chest_board[y][x]=0;
                        }
                        if (chest_board[y][x]==4 && turn == 1){
                            noMovedPiece=BLACK_BISHOP;
                            move_images=bishop_black;
                            chest_board[y][x]=0;
                        }
                        if(chest_board[y][x]==-4 && turn==0){
                            noMovedPiece=WHITE_BISHOP;
                            move_images=bishop_white;
                            chest_board[y][x]=0;
                        }
                        if (chest_board[y][x]==5 && turn==1){
                            noMovedPiece=BLACK_QUEEN;
                            move_images=queen_black;
                            chest_board[y][x]=0;
                        }
                        if(chest_board[y][x]==-5 && turn==0){
                            noMovedPiece=WHITE_QUEEN;
                            move_images=queen_white;
                            chest_board[y][x]=0;
                        }
                        if (chest_board[y][x]==6 && turn==1){
                            noMovedPiece=BLACK_KING;
                            move_images=king_black;
                            chest_board[y][x]=0;
                        }
                        if(chest_board[y][x]==-6&& turn==0){
                            noMovedPiece=WHITE_KING;
                            move_images=king_white;
                            chest_board[y][x]=0;
                        }
                        if(chest_board[y][x]==0){
                            is_moving=1;
                            old_poz.x=x;
                            old_poz.y=y;
                        }
                    }
                }   
            }
            if(event.type==Event::MouseButtonReleased){

                if(event.key.code==Mouse::Left){
                    int ok;
                    //pieces moved
                    if (noMovedPiece== WHITE_PAWN && is_moving==1){
                        ok=pawn_w(old_poz.x,old_poz.y, x, y);
                    }
                    if (noMovedPiece== BLACK_PAWN && is_moving==1){
                        ok=pawn_b(old_poz.x,old_poz.y, x, y);
                    }
                    if(noMovedPiece== WHITE_ROOK && is_moving==1){
                        ok=rook_w(old_poz.x,old_poz.y, x, y);
                        if(ok==1 && left_white_rook_m==0 && old_poz.y==7 && old_poz.x==0){
                            left_white_rook_m=1;
                        }
                        if(ok==1 && right_white_rook_m==0 && old_poz.y==7 && old_poz.x==7){
                            right_white_rook_m=1;
                        }
                    }
                    if(noMovedPiece== BLACK_ROOK && is_moving==1){
                        ok=rook_b(old_poz.x,old_poz.y, x, y);
                        if(ok==1 && right_black_rook_m==0 && old_poz.y==0 && old_poz.x==7){
                            right_black_rook_m=1;
                        }
                        if(ok==1 && left_black_rook_m==0 && old_poz.y==0 && old_poz.x==0){
                            left_black_rook_m=1;
                        }
                    }
                    if(noMovedPiece== WHITE_BISHOP && is_moving==1){
                        ok=bishop_w(old_poz.x,old_poz.y, x, y);

                    }
                    if(noMovedPiece== BLACK_BISHOP && is_moving==1){
                        ok=bishop_b(old_poz.x,old_poz.y, x, y);
                    }
                    if(noMovedPiece== WHITE_QUEEN && is_moving==1){
                        ok=queen_w(old_poz.x,old_poz.y, x, y);
                    }
                    if(noMovedPiece==BLACK_QUEEN && is_moving==1){
                        ok=queen_b(old_poz.x,old_poz.y, x, y);
                    }
                    if(noMovedPiece == WHITE_KNIGHT&& is_moving==1){
                        ok=knight_w(old_poz.x,old_poz.y, x, y);
                    }
                    if(noMovedPiece==BLACK_KNIGHT && is_moving==1){
                        ok=knight_b(old_poz.x,old_poz.y, x, y);
                    }
                    if(noMovedPiece==WHITE_KING && is_moving==1){
                        ok=king_w(old_poz.x,old_poz.y, x, y);
                        if(ok==1 && white_king_first_m==0){
                            white_king_first_m=1;
                        }
                    }
                    if(noMovedPiece== BLACK_KING && is_moving==1){
                        ok=king_b(old_poz.x,old_poz.y, x, y);
                        if(ok==1 && black_king_first_m==0){
                            black_king_first_m=1;
                        }
                    }
                    if(ok==1)
                    {
                        int nr=chest_board[y][x];
                        chest_board[y][x]=noMovedPiece;
                        if(y==0 && noMovedPiece==WHITE_PAWN){
                            transformation_white=1;
                            transform_white.x=x;
                            transform_white.y=y;
                            chest_board[y][x]=0;
                        }
                        if(y==7 && noMovedPiece==BLACK_PAWN){
                            transformation_black=1;
                            transform_black.x=x;
                            transform_black.y=y;
                            chest_board[y][x]=0;
                        }
                                                if (turn == 0) {
                            if (check_white == 1) {
                                pos_white_king();
                                int s = white_king_is_check(white_king.x, white_king.y);
                                if (s == 0) {
                                    chest_board[old_poz.y][old_poz.x] = noMovedPiece;
                                    chest_board[y][x] = nr;
                                }
                                else {
                                    check_white = 0;
                                    pos_black_king();
                                    int check = black_king_is_check(black_king.x, black_king.y);
                                    if (check == 0) {
                                        check_black = 1;
                                    }
                                    turn = 1;
                                }
                            }
                            else {
                                pos_white_king();
                                int sa = white_king_is_check(white_king.x, white_king.y);
                                if (sa == 0) {
                                    chest_board[old_poz.y][old_poz.x] = noMovedPiece;
                                    chest_board[y][x] = nr;
                                }
                                else {
                                    pos_black_king();
                                    int check = black_king_is_check(black_king.x, black_king.y);
                                    if (check == 0) {
                                        check_black = 1;
                                    }
                                    turn = 1;
                                }
                            }
                        }
                        else {
                            if (check_black == 1) {
                                int s = black_king_is_check(black_king.x, black_king.y);
                                if (s == 0) {
                                    chest_board[old_poz.y][old_poz.x] = noMovedPiece;
                                    chest_board[y][x] = nr;
                                }
                            }
                            else {
                                check_black = 0;
                                pos_white_king();
                                int check = white_king_is_check(white_king.x, white_king.y);
                                if (check == 0) {
                                    check_white = 1;
                                }
                                turn = 0;
                            }
                        }

                    }
                    else if(ok==0)
                    {
                        chest_board[old_poz.y][old_poz.x]=noMovedPiece;
                    }
                    is_moving=0;
                }
            }
        
        window.draw(table);

        /*if a pawn reaches the end of the board*/
        if(transformation_white==1){
            white_transformation.setPosition(transform_white.x*size, transform_white.y*size);
            window.draw(white_transformation);
        }
        if (transformation_black ==1){
            black_transformation.setPosition(transform_black.x*size, transform_white.y*size);
            window.draw(black_transformation);   
        }
        if(is_moving==1){
            move_images.setPosition(pos.x-dx, pos.y-dy);
            window.draw(move_images);
        }

        /*Continues to render the table and its pieces at the position
        described in the array*/

        for(int i=0; i<= 7;  i++){
            for(int j=0; j<= 7; j++){

                if(chest_board[i][j]!=0)
                {

                    if(chest_board[i][j]==BLACK_PAWN){
                       pawn_black.setPosition(j*size,i*size);
                       window.draw(pawn_black);
                    }

                    if(chest_board[i][j]==-1){//white pawn
                       pawn_white.setPosition(j*size,i*size);
                       window.draw(pawn_white);
                    }

                    if(chest_board[i][j]==2){//black rook
                        rook_black.setPosition(j*size,i*size);
                        window.draw(rook_black);
                    }

                    if(chest_board[i][j]==-2){
                        rook_white.setPosition(j*size,i*size);
                        window.draw(rook_white);
                    }

                    if(chest_board[i][j]==3){
                        knight_black.setPosition(j*size,i*size);
                        window.draw(knight_black);
                    }
                    if (chest_board[i][j]==-3){
                        knight_white.setPosition(j*size,i*size);
                        window.draw(knight_white);
                    }
                    if (chest_board[i][j]==4 ){
                        bishop_black.setPosition(j*size,i*size);
                        window.draw(bishop_black);
                    }
                    if(chest_board[i][j]==-4 ){
                        bishop_white.setPosition(j*size,i*size);
                        window.draw(bishop_white);
                    }

                    if (chest_board[i][j]==5 ){
                        queen_black.setPosition(j*size, i*size);
                        window.draw(queen_black);
                    }

                    if(chest_board[i][j]==-5 ){
                        queen_white.setPosition(j*size,i*size);
                        window.draw(queen_white);
                    }

                    if (chest_board[i][j]==6 ){
                        king_black.setPosition(j*size,i*size);
                        window.draw(king_black);
                    }

                    if(chest_board[i][j]==-6){
                        king_white.setPosition(j*size,i*size);
                        window.draw(king_white);
                    }
                }
            }
        }
        window.display();
    }
}

