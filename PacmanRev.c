#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<conio.h>
int count =0 ;
char bufferedDisplay[19][14] = 
	{{'|','|','|','|','|','|','|','|','|','|','|','|','|','|'},
	 {'|','*','*','*','*','*','|','*','*','*','*','*','*','|'},
	 {'|','*','|','*','|','*','|','*','|','*','|','*','*','|'},
 	 {'|','*','*','*','*','*','*','*','*','*','*','*','*','|'},
 	 {'|','|','|','*','|','*','|','|','*','|','*','|','|','|'},
 	 {'|','|','|','*','|','*','*','*','*','|','*','|','|','|'},
 	 {'|','|','|','*','|','|','*','*','|','|','*','|','|','|'},
 	 {'|','|','|','*','|','*','*','*','*','|','*','|','|','|'},
 	 {'|','|','|','*','|','*','*','*','*','|','*','|','|','|'},
 	 {'<','*','*','*','*','*','|','|','*','*','*','*','*','>'},
 	 {'|','|','|','*','|','*','*','*','*','|','*','|','|','|'},
 	 {'|','|','|','*','|','*','|','|','*','|','*','|','|','|'},
 	 {'|','|','|','*','|','*','|','|','*','|','*','|','|','|'},
 	 {'|','*','*','*','*','*','*','*','*','*','*','*','*','|'},
 	 {'|','*','|','|','|','*','|','|','*','|','|','|','*','|'},
 	 {'|','*','*','*','*','*','|','|','*','*','*','*','*','|'},
 	 {'|','*','|','*','|','*','|','|','*','|','*','|','*','|'},
 	 {'|','*','*','*','*','*','*','*','*','*','*','*','*','|'},
 	 {'|','|','|','|','|','|','|','|','|','|','|','|','|','|'}
	};
//struct untuk player
typedef struct playerMove{
	int x;
	int y;
}playerMove;
typedef struct ghostLinkedList{
	int x;
	int y;
	struct ghostLinkedList *next;
}ghostLinkedList;
//linkedlist utk point
typedef struct pointLinkedList{
	int x;
	int y;
	struct pointLinkedList *next;
}pointLinkedList;
//head tail utk point
pointLinkedList *headPoint = NULL;
pointLinkedList *tailPoint = NULL;

//head tail utk ghost
ghostLinkedList *headGhost = NULL;
ghostLinkedList *tailGhost = NULL;
playerMove player;

void addPoint(int x,int y){
	pointLinkedList *temp = (struct pointLinkedList *) malloc(sizeof(struct pointLinkedList));
	temp->x = x;
	temp->y = y;
	if(headPoint == NULL){
		temp->next = NULL;
		headPoint = temp;
		tailPoint = temp;
	}
	else{
		tailPoint->next = temp;
		temp->next = NULL;
		tailPoint = temp;
	}
}
void addGhost(int x,int y){
	ghostLinkedList *temp = (struct ghostLinkedList*) malloc(sizeof(struct ghostLinkedList));
	temp->x = x;
	temp->y = y;
	temp->next = headGhost;
	headGhost = temp;
}
void renderUpdate(){
	ghostLinkedList *temp = headGhost;
	while(temp->next != NULL){
		bufferedDisplay[temp->y][temp->x] = 'G';
		temp = temp->next;
	}
	bufferedDisplay[temp->y][temp->x] = 'G';
	bufferedDisplay[player.y][player.x] = 'P';
}
void displayMap(){
	pointLinkedList *temp = headPoint;
	for(int i = 0 ; i < 19 ; i ++){
		for(int j = 0 ; j < 14 ; j ++){
			printf("%c ",bufferedDisplay[i][j]);
		}
		printf("\n");
	}
	int i = 0;
	while(temp != NULL){
		i ++;
		temp= temp->next;
	}
	printf("%d\n",i);
}
void deletePoint(int x,int y){
	pointLinkedList *temp = headPoint;
	if(headPoint->x == x && headPoint->y == y)
	{
		headPoint = temp->next;
		free(temp);		
	}
	else
	{
		while(temp->next != NULL){
			if(temp->next->x == x && temp->next->y == y)
				break;
			temp = temp->next;
		}
		pointLinkedList *temp1 = temp->next;
		if(temp1->next == NULL){
			temp->next = NULL;
			free(temp1);
		}
		else
		{
			temp->next = temp1->next;
			free(temp1);
		}
	}
}
bool checkPoint(int x,int y){
	ghostLinkedList *temp = headGhost;
	if(headGhost->x == x && headGhost->y == y)
		return true;
	else{
		while(temp->next == NULL)
		{
			if(x == temp->x && y == temp->y)
				return true;
			temp = temp->next;
		}
		return false;
	}
}
void moveGhost()
{
	ghostLinkedList *temp = headGhost;
	ghostMovement(temp->x,temp->y);
	if(ghostIsSafe(temp->x + 1,temp->y) == 0 && temp->x < player.x)
	{
		if(checkPoint(temp->x,temp->y) == false)
			bufferedDisplay[temp->y][temp->x] = ' ';
		else
			bufferedDisplay[temp->y][temp->x] = '*';
		temp->x += 1;
	}
	else if(ghostIsSafe(temp->x - 1,temp->y) == 0 && temp->x > player.x){
		if(checkPoint(temp->x,temp->y) == false)
			bufferedDisplay[temp->y][temp->x] = ' ';
		else
			bufferedDisplay[temp->y][temp->x] = '*';
		temp->x -= 1;
	}
	else if(ghostIsSafe(temp->x,temp->y - 1) == 0 && temp->y > player.y){
		if(checkPoint(temp->x,temp->y) == false)
			bufferedDisplay[temp->y][temp->x] = ' ';
		else
			bufferedDisplay[temp->y][temp->x] = '*';
		temp->y -= 1;
	}
	else if(ghostIsSafe(temp->x,temp->y + 1) == 0 && temp->y < player.y){
		if(checkPoint(temp->x,temp->y) == false)
			bufferedDisplay[temp->y][temp->x] = ' ';
		else
			bufferedDisplay[temp->y][temp->x] = '*';
		temp->y += 1;
	}
	while(temp->next != NULL)
	{
		temp = temp->next;	
		ghostMovement(temp->x,temp->y);	
	}
}
void ghostMovement(int x,int y)
{
	printf("fail");

	
}
int ghostIsSafe(int x,int y){
	if(bufferedDisplay[y][x] == '|')
		return -1;
	if(bufferedDisplay[y][x] == '*'){
		return 0;		
	}
	if(bufferedDisplay[y][x] == ' ')
		return 0;
}
int isSafe(int x,int y){
//	if(bufferedDisplay[y][x] == 'G')
//		return -1;
	if(bufferedDisplay[y][x] == '|')
		return -1;
	if(bufferedDisplay[y][x] == '>')
		return 2;
	if(bufferedDisplay[y][x] == '<')
		return 3;
	if(bufferedDisplay[y][x] == '*'){
		deletePoint(x, y);
		return 0;		
	}
	return 0;
}
void moveNorth(){
	if(isSafe(player.x,player.y - 1) == 0){
		player.y -= 1;
		bufferedDisplay[player.y + 1][player.x] = ' ';
	}
}
void moveWest(){
	if(isSafe(player.x - 1,player.y) == 0){
		player.x -= 1;
		bufferedDisplay[player.y][player.x + 1] = ' ';
	}
	else if(isSafe(player.x - 1,player.y) == 3){
		bufferedDisplay[player.y][player.x] = ' ';
		if(isSafe(player.x + 11,player.y) == 0)
			player.x += 11;
	}
}
void moveEast(){
	if(isSafe(player.x + 1,player.y) == 0){
		player.x += 1;	
		bufferedDisplay[player.y][player.x - 1] = ' ';
	}
	else if(isSafe(player.x + 1,player.y) == 2){
		bufferedDisplay[player.y][player.x ] = ' ';
		if(isSafe(player.x - 11,player.y) == 0)
			player.x -= 11;
	}
}
void moveSouth(){

	if(isSafe(player.x,player.y + 1) == 0){
		player.y += 1;
		bufferedDisplay[player.y - 1][player.x] =' ';
	}
	
}
int main (){
	int score = 0;
	char input;
	bool validMove = false;
	addGhost(1,1);
	player.x = 7;
	player.y = 13;
	renderUpdate();
	for(int i = 0 ; i < 19 ; i ++){
		for(int j = 0 ; j < 14 ; j ++){
			if(bufferedDisplay[i][j] == '*' || bufferedDisplay[i][j] == 'G'){
				addPoint(j,i);
			}
		}
	}
	do{
		system("cls");
		printf("SCORE : %d\n",score);
		displayMap();
		printf("<PRESS W TO GO NORTH>\t");
		printf("<PRESS A TO GO WEST>\t");
		printf("<PRESS S TO GO SOUTH>\t");	
		printf("<PRESS D TO GO EAST>\n");
		do{	
			input = getch();
			if(input == 87 || input == 65 || input == 68 || input == 83 || input == 97 || input == 100 || input == 115 || input == 119)
				validMove = true;
		}while(validMove == false);
		if(input == 87 || input == 119){
			moveNorth();
		}
		else if(input == 65 || input == 97){
			moveWest();
		}
		else if(input == 83 || input == 115){
			moveSouth();
		}
		else if(input == 68 || input == 100){
			moveEast();
		}
		moveGhost();
		renderUpdate();
		validMove = false;
	}while(headPoint != NULL);
	

	return 0;
	
	
}
