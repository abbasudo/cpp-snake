/*
* file : snakegamev5.cpp
*
* title : Snake game beta 5
*
* version : beta 5
*
* C abbas makhzomi
* copyrights reserved
*/
#include <iostream>
#include <conio.h>
#include <vector>

#include <windows.h>// sleep()

#include <ctime> // For time()
#include <cstdlib> // For srand() and rand()

using namespace std;

char** output(HANDLE,int,int,int,int,int,vector<int>,vector<int>,char**,int,int,int,bool,bool,int,int);
bool isgameover(int,int,vector<int>,vector<int>);
bool ispoint(int,int,int,int);
int iskeypressed(int,int);
int validation(int,int);

int main(){

	char chr;

	srand(time(0)); // Initialize random number generator.

	int tol = 25;

	int arz = 80;

	int mar = 4;

	int time = 4; // 1 means 1*50 milliseconds per frame


	cout<<"Use W-S-A-D or arrow keys to move\nPress any key to start";

	getch();

	system("cls");


	//dynamic array is created
	//to save each frame
	char **gamebord;
	gamebord = new char *[tol + 1];
	for(int i = 0; i <tol + 1; i++)
		gamebord[i] = new char[arz + 1];


	//saves snake coordinates
	vector<int> x;
	vector<int> y;


	int tempmar = mar; //we will need snake length later

	x.push_back(int(arz/2));
	y.push_back(int(tol/2));

	bool flag = 0;

	//snake vector is created with this loop
	while(true){

		for(int i = int(arz/2)-1 ; tempmar > 0 and i > 0;i--){


			x.insert(x.begin(), x.front() + (flag ? -1 : 1)); // once add, once subtract
			//to make the snake spiral, x's must be added first, and then x's must be decreased in the next turn

			y.insert(y.begin(), y.front());
			tempmar --;
		}
		if(tempmar != 0){

			x.insert(x.begin(), x.front());
			y.insert(y.begin(), y.front()+1); // so the snake is not on itself
			tempmar --;

			flag = flag ? 0 : 1 ; // reverses the flag

		} else
			break;


	}
	//this way, a snake with a length of 'tempmar' is created at the beginning of the game

	//default movement at the start of the game
	int oldmove,lastmove = 3;

	//the first point is created non-randomly within the game boundaries (arz / 4 -> if the board is large, the probability
	// of the point being within the snake's body is high
	// this division removes that possibility )
	int	pointx = rand() % int(arz/4) + 2;
	int pointy = rand() % int(tol/4) + 2;


	int randco1 = rand() % 251; // gives a random range (C color)
	int randco2 = randco1% 16 * 16 + randco1/16;// gives the opposite of the first range (C color)

	long long int frame = 0; // frame number
	int emtiaz = 0; // score counter

	bool stop = 0; // stop status ( space )
	bool pointflag = 1; // point status ( space )

	bool timeflag = 0; // time status (E turbo)
	int oldtime = time; // original speed (E turbo)

///////////////////////////////////////////////////////////////////////////////////////////////
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD mokhtasat; // COORD structure stores 2 short numbers, x and y
				 // later used as console coordinates
				 // (0,0) means the first cell top right

	mokhtasat.X = arz + 1;
	mokhtasat.Y = 0;
	SetConsoleCursorPosition(hOut, mokhtasat );
	printf(" Score : "); // x or y size is actually player's score + initial game size

	mokhtasat.Y = 1;
	SetConsoleCursorPosition(hOut, mokhtasat );
	printf(" Speed : ");

	mokhtasat.Y = 2;
	SetConsoleCursorPosition(hOut, mokhtasat );
	printf(" Frame Number : ");

	mokhtasat.Y = 3;
	SetConsoleCursorPosition(hOut, mokhtasat );
	printf(" Exit : [esc]");


	mokhtasat.Y = 4;
	SetConsoleCursorPosition(hOut, mokhtasat );
	printf(" Stop : [space]");

	mokhtasat.Y = 5;
	SetConsoleCursorPosition(hOut, mokhtasat );
	printf(" Color : [C]");

	mokhtasat.Y = 6;
	SetConsoleCursorPosition(hOut, mokhtasat );
	printf(" Turbo : hold [E]");
///////////////////////////////////////////////////////////////////////////////////////////////

	while(true){

		frame ++ ;

		//tol,arz,pointx,pointy,x,y = width and height, point coordinates, snake coordinate vector, 2D game board array to save last output
		gamebord = output(hOut,oldmove,tol,arz,pointx,pointy,x,y,gamebord,emtiaz,time,frame,0,stop,randco1,randco2); //cout game shape


		if(lastmove == 7 ){ //if key was c, change color and perform previous move (oldmove)
			randco1 = rand() % 251; // gives a random range
			randco2 = randco1 % 16 * 16 + randco1 / 16;	// gives the opposite of the first range
			lastmove = oldmove;
		}


		if(lastmove == 8 ){ // E key

			time = 0; //turbo speed
			lastmove = oldmove; //move before E key

			if(!timeflag) //executed only the first time E key is sent, not for subsequent times
				timeflag = 1;


		} else {
			if(timeflag){ //if E key was previously pressed and is now released
				timeflag = 0;
				time = oldtime;
			}
		}

		//first execution 3
		switch(lastmove){
			case 1:

				//cout<<"UP";
				x.push_back ( x.back() ); // adds an element to the vector (like the last existing element in the vector)

				y.push_back ( y.back() - 1 );	// since the movement was upwards, one is subtracted from the previous y element and added as a new element (movement is in the -y direction because in this program the vector is inverted)
				break;

			case 2:

				//cout<<"Down";
				x.push_back ( x.back() );
				y.push_back ( y.back() + 1 );
				break;

			case 3:

				//cout<<"left";
				x.push_back ( x.back() - 1 );
				y.push_back ( y.back() );
				break;

			case 4:

				//cout<<"Right";
				x.push_back ( x.back() + 1 );
				y.push_back ( y.back() );
				break;

			case 5: //stop

				pointflag = 0; // so that element is not removed from the end of the snake
				stop = stop ? 0 : 1 ; // reverses the flag
				// each time the space key is pressed, the stop status changes

				if(stop)
					lastmove = 0; // snake will not move in subsequent frames
				else
					lastmove = oldmove; // snake continues its movement before stop

				break;

			case 6: //exit
				mokhtasat.X = 0;
				mokhtasat.Y = tol+1;
				SetConsoleCursorPosition(hOut, mokhtasat );
				return 0;

		}

		if(isgameover(tol,arz,x,y))	//if game over, breaks the loop, which means exiting the game process
			break;



		if(lastmove != 0 and lastmove != 8) //to save movement before E and space
			oldmove = lastmove; //used for movement after stop or turbo speed

		lastmove = iskeypressed(time,lastmove);//check pressed key and get new move
											   //simply, lastmove is updated
											   //does not necessarily change (if no key is pressed in the specified time
											   //it remains the same as the previous value
											   //and continues to move in the same direction)


		if(!ispoint(x.back(),y.back(),pointx,pointy)){

			//if score was not received, removes the first element of the vector (last cell of the snake's body)
			//because a new element was added to the vector in the direction of movement
			if(lastmove != 0 ){ //if not stopped
				if(pointflag){ //do nothing the first time after exiting stop, because no element has been added yet when exiting
					x.erase (x.begin());
					y.erase (y.begin());
				} else
					pointflag = 1;
			}

		} else {
			//pointflag = 0; //activating this line adds 2 units to snake length for each point
			emtiaz++;
			//if point is eaten, no element is removed, so the snake grows
			//in fact, the previous point's x and y become x and y (added to the vector but nothing is removed from the snake)

			//then a new random x and y are created within the game boundaries
			//update: outside snake's body
			//this way, the previous point is removed

			//loop continues until point is not on the snake
			while(true){
				flag = 0;
				//pointx = rand() % (arz - 1) + 1; //possibility of falling near game edge (commented out to make game easier)
				//pointy = rand() % (tol - 2) + 2;

				pointx = rand() % (arz - 2) + 2; //random coordinates within game boundaries
				pointy = rand() % (tol - 2) + 2;

				for(int z = 0; z < x.size(); z++)
					if(x[z] == pointx and y[z] == pointy) //this ensures the random point is not on the snake
						flag = 1;

				if(!flag)
					break;
			}
		}
	}

	//print last frame with game over message 1 = game over
	output(hOut,oldmove,tol,arz,pointx,pointy,x,y,gamebord,emtiaz,time,frame,1,stop,randco1,randco2);
	Sleep(3000);
	getch();

	return 0;
}


int iskeypressed(int sec,int lastmove){ //returns 1 (up) - 2 (down) - 3 (left) - 4 (right)
	char chr;

	bool flag = 0; // fixes bug v1 - during the time this function is working, you cannot press 2 keys consecutively
					// in v1, if you press w and immediately s, it returns without applying up, down, which caused a loss
					// with this flag, we ensure that no key is pressed during this function's runtime


	//sec is the number of 50 milliseconds that must pass
	//if no key is received, the previous key (lastmove) is returned
	//meaning the snake continues its previous movement
	//but if a key is pressed, the new movement is registered
	while( sec >= 0 ){

		if(!flag and kbhit() ) { //if there is a key in keyboard buffer
			chr = getch(); //get the char

			switch(chr){
				case 'W':
				case 'w':
				case 72: // up arrow
					if(lastmove != 2 and lastmove != 0)	//if snake was moving down, it cannot move up
						lastmove = 1;
					flag = 1;		//this way, only one key is captured per execution
					break;

				case 'S':
				case 's':
				case 80:
					if(lastmove != 1 and lastmove != 0) //lastmove != 0 : during stop, movement key has no effect
						lastmove = 2;
					flag = 1;
					break;

				case 'A':
				case 'a':
				case 75:

					if(lastmove != 4 and lastmove != 0)
						lastmove = 3;
					flag = 1;
					break;

				case 'D':
				case 'd':
				case 77:

					if(lastmove != 3 and lastmove != 0)
						lastmove = 4;
					flag = 1;
					break;

				case 32:

					lastmove = 5;
					flag = 1;
					break;

				case 27:

					lastmove = 6;
					flag = 1;
					break;

				case 'C':
				case 'c':
					if(lastmove != 0) // during stop, color should not change - removing this if causes problems
						lastmove = 7;
					flag = 1;
					break;

				case 'E':
				case 'e':
					if(lastmove != 0)
						lastmove = 8;
					flag = 1;
					break;
			}
		}
		Sleep(30); //30 milliseconds sleep
		--sec; //countdown a 50 milliseconds - game speed is set here (interval between frames)
	}
	return lastmove; //if no key is received, the previous key is sent
}

bool ispoint(int lastx,int lasty,int pointx,int pointy){ // if point is eaten, returns 1, otherwise 0

	if(lastx == pointx and lasty == pointy) //checking snake's head
											//if we checked the whole vector, the point might accidentally be on the snake
											//by checking only the last element, we prevent extra points
											//update v4: it's impossible for the point to be on the snake, but it doesn't hurt to be robust ;)
		return 1;
	else
		return 0;

}

bool isgameover(int row,int col,vector<int> x,vector<int> y){ //if game over, returns 1, otherwise 0

	for(int z = 0; z < x.size(); z++){

		if(x[z] == 0 or x[z] == col or y[z] == 0 or y[z] == row) //if snake hits boundary, returns 1
			return 1;
		for(int k = 0; k < x.size(); k++)
			if(k != z and x[k] == x[z] and y[k] == y[z]) //if there is a duplicate element in the vector, returns 1
														 //if x and y are duplicated, it means the snake crossed over itself
														 //if the first condition (k != z) is always true
				return 1;
	}
	return 0; //if 1 has not been returned up to this stage, 0 is returned, meaning game is not over

}

char** output(HANDLE hOut,int dir,int row,int col,int pointx,int pointy,vector<int> x ,vector<int> y,char** gamebord,int emtiaz,int time,int frame,bool gameover,bool stop,int randco1,int randco2){
	//cout game, this function is called for each frame

	char head;

	switch(dir){
		case 1 :
			head ='A';
			break;
		case 2 :
			head ='V';
			break;
		case 3 :
			head ='<';
			break;
		case 4 :
			head ='>';
			break;
	}


	bool flag;

	COORD mokhtasat; // COORD structure stores 2 short numbers, x and y
					 // later used as console coordinates
					 // (0,0) means the first cell top right


	int rang[row+1][col+1];
	char shekl[row+1][col+1];
	//for each element of the 'shekl' array, there is an element in the 'rang' array that determines the color
	// of the 'shekl' array element

	for(int i = 0 ; i <= row ; i++ ){

		for(int j = 0 ; j <= col ; j++ )

			if(i == 0 or i == row or j == 0 or j == col){ //game boundary - row 0 - last and column 0 - last

				rang[i][j]=68;	//saving color in array
				shekl[i][j]=' '; //saving boundary in array

			} else if(i == int(row/2) and (gameover or stop )) {

				rang[i][j]=15;
				shekl[i][j]='_';

			} else {

				flag = 0;

				for(int z = 0; z < x.size(); z++) //for each element of x and y

					if(j == x[z] and i == y[z]){ //if current i and j coordinates match element of x and y vector

						if(z%2 == 0) { //alternating

							if(gameover){
								rang[i][j]=240;
								shekl[i][j]='+';
							} else {

								rang[i][j]=randco1;//random color

								if(z == x.size() - 1) //snake's head
									shekl[i][j]=head;
								else
									shekl[i][j]='X';
							}


						} else {
							if(gameover){
								rang[i][j]=240;
								shekl[i][j]='#';
							} else {
								rang[i][j]=randco2;//random color

								if(z == x.size() - 1)
									shekl[i][j]=head;
								else
									shekl[i][j]='O';

							}
						}
						flag = 1;
						break;

					}

				if(!flag)//if no snake existed at i and j

					if(j == pointx and i == pointy){//if point existed at i and j

						rang[i][j]=45;
						shekl[i][j]='X';

					} else { //if neither point nor snake nor boundary, fill with ' '

						rang[i][j]=240;
						shekl[i][j]=' ';

					}
			}
	}

	//system("cls"); //no need to clear screen, this code changes parts that changed

	//simply draws the 2D array
	//update: only draws elements that changed from the last frame, this way speed increases several times
	for(int i = 0 ; i <= row ; i++ ){
		for(int j = 0 ; j <= col ; j++ ){

			//SetConsoleTextAttribute(hOut,rang[i][j]);


			//printf("%c",shekl[i][j]); //slow

			//cout<<shekl[i][j]; //very slow

			//if (shekl[i][j] != ' '){ //fast


			//'shekl' array is current output and 'gamebord' is previous frame's output
			//if they match, no action is taken, so i and j coordinates remain as they were in the previous frame

			if (shekl[i][j] != gamebord[i][j]){ //very fast //v3 bug: some coordinates remain black
				SetConsoleTextAttribute(hOut,rang[i][j]);

				mokhtasat.X = j;
				mokhtasat.Y = i;

				SetConsoleCursorPosition(hOut, mokhtasat );
				printf("%c",shekl[i][j]);

				gamebord[i][j] = shekl[i][j];
				//gamebord[i][j] = rang[i][j]; //v3 bug: some coordinates remain black
			}
		}
	}

	//mokhtasat.X = 0; //uncomment to see :|
	//mokhtasat.Y = row+1;


	SetConsoleTextAttribute(hOut,15);//default color

	mokhtasat.X = col + 11;
	mokhtasat.Y = 0;
	SetConsoleCursorPosition(hOut, mokhtasat );
	printf("%i",emtiaz); // x or y size is actually player's score + initial game size

	mokhtasat.X -= 1;
	mokhtasat.Y = 1;
	SetConsoleCursorPosition(hOut, mokhtasat );
	printf("%i",time);

	mokhtasat.X += 8;
	mokhtasat.Y = 2;
	SetConsoleCursorPosition(hOut, mokhtasat );
	printf("%i",frame);


	if(gameover){
		mokhtasat.X = int(col/2)-4;
		mokhtasat.Y = int(row/2);
		SetConsoleCursorPosition(hOut, mokhtasat );
		printf("Game Over");


		mokhtasat.X = 0;
		mokhtasat.Y = row+1;
		SetConsoleCursorPosition(hOut, mokhtasat );
	}

	if(stop){
		mokhtasat.X = int(col/2)-2;
		mokhtasat.Y = int(row/2);
		SetConsoleCursorPosition(hOut, mokhtasat );
		printf("STOP");
	}


	return gamebord; //returns the current output
}

int validation(int max,int min){ //cin and check entered value
								 //if greater than min and less than max
								 //value is returned

	string str ;
	int t , num ;
	bool flag ;

	while(true){

		cin>>str;

		num = 0;
		flag = 0;


		for( int i = 0 ; i < str.length() ; i++ ){

			t = int(str[i]) - 48 ; //ascii code 48 : 0

			if( t < 9 and t >= 0 )
				for( int j = str.length() - i ; j > 1 ; j-- )
					t *= 10;

			else
				flag = 1; //if a non-numeric character is entered, flag becomes true

			num += t;

		}

		if( !flag ) //if only numbers were entered, it's false, which becomes true with !

			if(num <= max and num >= min)
				return num;
			else
				cout<<"\nnumber must be between "<<min - 1<<" and "<<max + 1<<" : ";

		else
			cout<<"\nenter a positive integer : ";
	}
}