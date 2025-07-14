/*
* file : snake_game_v5.cpp
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

char** display_output(HANDLE,int,int,int,int,int,vector<int>,vector<int>,char**,int,int,int,bool,bool,int,int);
bool is_game_over(int,int,vector<int>,vector<int>);
bool has_point_been_eaten(int,int,int,int);
int get_key_pressed(int,int);
int validate_input(int,int);

int main(){

	char character;

	srand(time(0)); // Initialize random number generator.

	int board_height = 25;

	int board_width = 80;

	int snake_length = 4;

	int game_speed = 4; // 1 means 1*50 milliseconds per frame


	cout<<"Use W-S-A-D or arrow keys to move\nPress any key to start";

	getch();

	system("cls");


	//dynamic array is created
	//to save each frame
	char **game_board;
	game_board = new char *[board_height + 1];
	for(int i = 0; i < board_height + 1; i++)
		game_board[i] = new char[board_width + 1];


	//saves snake coordinates
	vector<int> snake_x_coordinates;
	vector<int> snake_y_coordinates;


	int temp_snake_length = snake_length; //we will need snake length later

	snake_x_coordinates.push_back(int(board_width/2));
	snake_y_coordinates.push_back(int(board_height/2));

	bool flag = 0;

	//snake vector is created with this loop
	while(true){

		for(int i = int(board_width/2)-1 ; temp_snake_length > 0 and i > 0;i--){


			snake_x_coordinates.insert(snake_x_coordinates.begin(), snake_x_coordinates.front() + (flag ? -1 : 1)); // once add, once subtract
			//to make the snake spiral, x's must be added first, and then x's must be decreased in the next turn

			snake_y_coordinates.insert(snake_y_coordinates.begin(), snake_y_coordinates.front());
			temp_snake_length --;
		}
		if(temp_snake_length != 0){

			snake_x_coordinates.insert(snake_x_coordinates.begin(), snake_x_coordinates.front());
			snake_y_coordinates.insert(snake_y_coordinates.begin(), snake_y_coordinates.front()+1); // so the snake is not on itself
			temp_snake_length --;

			flag = flag ? 0 : 1 ; // reverses the flag

		} else
			break;


	}
	//this way, a snake with a length of 'temp_snake_length' is created at the beginning of the game

	//default movement at the start of the game
	int old_move,last_move = 3;

	//the first point is created non-randomly within the game boundaries (board_width / 4 -> if the board is large, the probability
	// of the point being within the snake's body is high
	// this division removes that possibility )
	int	point_x = rand() % int(board_width/4) + 2;
	int point_y = rand() % int(board_height/4) + 2;


	int random_color_1 = rand() % 251; // gives a random range (C color)
	int random_color_2 = random_color_1 % 16 * 16 + random_color_1/16;// gives the opposite of the first range (C color)

	long long int frame_number = 0; // frame number
	int score = 0; // score counter

	bool stop_game = 0; // stop status ( space )
	bool point_flag = 1; // point status ( space )

	bool turbo_mode_flag = 0; // time status (E turbo)
	int old_game_speed = game_speed; // original speed (E turbo)

///////////////////////////////////////////////////////////////////////////////////////////////
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD coordinates; // COORD structure stores 2 short numbers, x and y
				 // later used as console coordinates
				 // (0,0) means the first cell top right

	coordinates.X = board_width + 1;
	coordinates.Y = 0;
	SetConsoleCursorPosition(hOut, coordinates );
	printf(" Score : "); // x or y size is actually player's score + initial game size

	coordinates.Y = 1;
	SetConsoleCursorPosition(hOut, coordinates );
	printf(" Speed : ");

	coordinates.Y = 2;
	SetConsoleCursorPosition(hOut, coordinates );
	printf(" Frame Number : ");

	coordinates.Y = 3;
	SetConsoleCursorPosition(hOut, coordinates );
	printf(" Exit : [esc]");


	coordinates.Y = 4;
	SetConsoleCursorPosition(hOut, coordinates );
	printf(" Stop : [space]");

	coordinates.Y = 5;
	SetConsoleCursorPosition(hOut, coordinates );
	printf(" Color : [C]");

	coordinates.Y = 6;
	SetConsoleCursorPosition(hOut, coordinates );
	printf(" Turbo : hold [E]");
///////////////////////////////////////////////////////////////////////////////////////////////

	while(true){

		frame_number ++ ;

		//board_height,board_width,point_x,point_y,snake_x_coordinates,snake_y_coordinates = width and height, point coordinates, snake coordinate vector, 2D game board array to save last output
		game_board = display_output(hOut,old_move,board_height,board_width,point_x,point_y,snake_x_coordinates,snake_y_coordinates,game_board,score,game_speed,frame_number,0,stop_game,random_color_1,random_color_2); //cout game shape


		if(last_move == 7 ){ //if key was c, change color and perform previous move (old_move)
			random_color_1 = rand() % 251; // gives a random range
			random_color_2 = random_color_1 % 16 * 16 + random_color_1 / 16;	// gives the opposite of the first range
			last_move = old_move;
		}


		if(last_move == 8 ){ // E key

			game_speed = 0; //turbo speed
			last_move = old_move; //move before E key

			if(!turbo_mode_flag) //executed only the first time E key is sent, not for subsequent times
				turbo_mode_flag = 1;


		} else {
			if(turbo_mode_flag){ //if E key was previously pressed and is now released
				turbo_mode_flag = 0;
				game_speed = old_game_speed;
			}
		}

		//first execution 3
		switch(last_move){
			case 1:

				//cout<<"UP";
				snake_x_coordinates.push_back ( snake_x_coordinates.back() ); // adds an element to the vector (like the last existing element in the vector)

				snake_y_coordinates.push_back ( snake_y_coordinates.back() - 1 );	// since the movement was upwards, one is subtracted from the previous y element and added as a new element (movement is in the -y direction because in this program the vector is inverted)
				break;

			case 2:

				//cout<<"Down";
				snake_x_coordinates.push_back ( snake_x_coordinates.back() );
				snake_y_coordinates.push_back ( snake_y_coordinates.back() + 1 );
				break;

			case 3:

				//cout<<"left";
				snake_x_coordinates.push_back ( snake_x_coordinates.back() - 1 );
				snake_y_coordinates.push_back ( snake_y_coordinates.back() );
				break;

			case 4:

				//cout<<"Right";
				snake_x_coordinates.push_back ( snake_x_coordinates.back() + 1 );
				snake_y_coordinates.push_back ( snake_y_coordinates.back() );
				break;

			case 5: //stop

				point_flag = 0; // so that element is not removed from the end of the snake
				stop_game = stop_game ? 0 : 1 ; // reverses the flag
				// each time the space key is pressed, the stop status changes

				if(stop_game)
					last_move = 0; // snake will not move in subsequent frames
				else
					last_move = old_move; // snake continues its movement before stop

				break;

			case 6: //exit
				coordinates.X = 0;
				coordinates.Y = board_height+1;
				SetConsoleCursorPosition(hOut, coordinates );
				return 0;

		}

		if(is_game_over(board_height,board_width,snake_x_coordinates,snake_y_coordinates))	//if game over, breaks the loop, which means exiting the game process
			break;



		if(last_move != 0 and last_move != 8) //to save movement before E and space
			old_move = last_move; //used for movement after stop or turbo speed

		last_move = get_key_pressed(game_speed,last_move);//check pressed key and get new move
											   //simply, last_move is updated
											   //does not necessarily change (if no key is pressed in the specified time
											   //it remains the same as the previous value
											   //and continues to move in the same direction)


		if(!has_point_been_eaten(snake_x_coordinates.back(),snake_y_coordinates.back(),point_x,point_y)){

			//if score was not received, removes the first element of the vector (last cell of the snake's body)
			//because a new element was added to the vector in the direction of movement
			if(last_move != 0 ){ //if not stopped
				if(point_flag){ //do nothing the first time after exiting stop, because no element has been added yet when exiting
					snake_x_coordinates.erase (snake_x_coordinates.begin());
					snake_y_coordinates.erase (snake_y_coordinates.begin());
				} else
					point_flag = 1;
			}

		} else {
			//point_flag = 0; //activating this line adds 2 units to snake length for each point
			score++;
			//if point is eaten, no element is removed, so the snake grows
			//in fact, the previous point's x and y become x and y (added to the vector but nothing is removed from the snake)

			//then a new random x and y are created within the game boundaries
			//update: outside snake's body
			//this way, the previous point is removed

			//loop continues until point is not on the snake
			while(true){
				flag = 0;
				//point_x = rand() % (board_width - 1) + 1; //possibility of falling near game edge (commented out to make game easier)
				//point_y = rand() % (board_height - 2) + 2;

				point_x = rand() % (board_width - 2) + 2; //random coordinates within game boundaries
				point_y = rand() % (board_height - 2) + 2;

				for(int z = 0; z < snake_x_coordinates.size(); z++)
					if(snake_x_coordinates[z] == point_x and snake_y_coordinates[z] == point_y) //this ensures the random point is not on the snake
						flag = 1;

				if(!flag)
					break;
			}
		}
	}

	//print last frame with game over message 1 = game over
	display_output(hOut,old_move,board_height,board_width,point_x,point_y,snake_x_coordinates,snake_y_coordinates,game_board,score,game_speed,frame_number,1,stop_game,random_color_1,random_color_2);
	Sleep(3000);
	getch();

	return 0;
}


int get_key_pressed(int delay_in_ms,int last_move){ //returns 1 (up) - 2 (down) - 3 (left) - 4 (right)
	char character;

	bool key_pressed_this_frame = 0; // fixes bug v1 - during the time this function is working, you cannot press 2 keys consecutively
					// in v1, if you press w and immediately s, it returns without applying up, down, which caused a loss
					// with this key_pressed_this_frame, we ensure that no key is pressed during this function's runtime


	//delay_in_ms is the number of 50 milliseconds that must pass
	//if no key is received, the previous key (last_move) is returned
	//meaning the snake continues its previous movement
	//but if a key is pressed, the new movement is registered
	while( delay_in_ms >= 0 ){

		if(!key_pressed_this_frame and kbhit() ) { //if there is a key in keyboard buffer
			character = getch(); //get the char

			switch(character){
				case 'W':
				case 'w':
				case 72: // up arrow
					if(last_move != 2 and last_move != 0)	//if snake was moving down, it cannot move up
						last_move = 1;
					key_pressed_this_frame = 1;		//this way, only one key is captured per execution
					break;

				case 'S':
				case 's':
				case 80:
					if(last_move != 1 and last_move != 0) //last_move != 0 : during stop, movement key has no effect
						last_move = 2;
					key_pressed_this_frame = 1;
					break;

				case 'A':
				case 'a':
				case 75:

					if(last_move != 4 and last_move != 0)
						last_move = 3;
					key_pressed_this_frame = 1;
					break;

				case 'D':
				case 'd':
				case 77:

					if(last_move != 3 and last_move != 0)
						last_move = 4;
					key_pressed_this_frame = 1;
					break;

				case 32:

					last_move = 5;
					key_pressed_this_frame = 1;
					break;

				case 27:

					last_move = 6;
					key_pressed_this_frame = 1;
					break;

				case 'C':
				case 'c':
					if(last_move != 0) // during stop, color should not change - removing this if causes problems
						last_move = 7;
					key_pressed_this_frame = 1;
					break;

				case 'E':
				case 'e':
					if(last_move != 0)
						last_move = 8;
					key_pressed_this_frame = 1;
					break;
			}
		}
		Sleep(30); //30 milliseconds sleep
		--delay_in_ms; //countdown a 50 milliseconds - game speed is set here (interval between frames)
	}
	return last_move; //if no key is received, the previous key is sent
}

bool has_point_been_eaten(int last_x,int last_y,int point_x,int point_y){ // if point is eaten, returns 1, otherwise 0

	if(last_x == point_x and last_y == point_y) //checking snake's head
											//if we checked the whole vector, the point might accidentally be on the snake
											//by checking only the last element, we prevent extra points
											//update v4: it's impossible for the point to be on the snake, but it doesn't hurt to be robust ;)
		return 1;
	else
		return 0;

}

bool is_game_over(int rows,int columns,vector<int> snake_x_coordinates,vector<int> snake_y_coordinates){ //if game over, returns 1, otherwise 0

	for(int z = 0; z < snake_x_coordinates.size(); z++){

		if(snake_x_coordinates[z] == 0 or snake_x_coordinates[z] == columns or snake_y_coordinates[z] == 0 or snake_y_coordinates[z] == rows) //if snake hits boundary, returns 1
			return 1;
		for(int k = 0; k < snake_x_coordinates.size(); k++)
			if(k != z and snake_x_coordinates[k] == snake_x_coordinates[z] and snake_y_coordinates[k] == snake_y_coordinates[z]) //if there is a duplicate element in the vector, returns 1
														 //if x and y are duplicated, it means the snake crossed over itself
														 //if the first condition (k != z) is always true
				return 1;
	}
	return 0; //if 1 has not been returned up to this stage, 0 is returned, meaning game is not over

}

char** display_output(HANDLE hOut,int direction,int rows,int columns,int point_x,int point_y,vector<int> snake_x_coordinates ,vector<int> snake_y_coordinates,char** game_board,int score,int game_speed,int frame_number,bool game_over,bool stop_game,int random_color_1,int random_color_2){
	//cout game, this function is called for each frame

	char head_character;

	switch(direction){
		case 1 :
			head_character ='A';
			break;
		case 2 :
			head_character ='V';
			break;
		case 3 :
			head_character ='<';
			break;
		case 4 :
			head_character ='>';
			break;
	}


	bool flag;

	COORD coordinates; // COORD structure stores 2 short numbers, x and y
					 // later used as console coordinates
					 // (0,0) means the first cell top right


	int colors[rows+1][columns+1];
	char shapes[rows+1][columns+1];
	//for each element of the 'shapes' array, there is an element in the 'colors' array that determines the color
	// of the 'shapes' array element

	for(int i = 0 ; i <= rows ; i++ ){

		for(int j = 0 ; j <= columns ; j++ )

			if(i == 0 or i == rows or j == 0 or j == columns){ //game boundary - row 0 - last and column 0 - last

				colors[i][j]=68;	//saving color in array
				shapes[i][j]=' '; //saving boundary in array

			} else if(i == int(rows/2) and (game_over or stop_game )) {

				colors[i][j]=15;
				shapes[i][j]='_';

			} else {

				flag = 0;

				for(int z = 0; z < snake_x_coordinates.size(); z++) //for each element of x and y

					if(j == snake_x_coordinates[z] and i == snake_y_coordinates[z]){ //if current i and j coordinates match element of x and y vector

						if(z%2 == 0) { //alternating

							if(game_over){
								colors[i][j]=240;
								shapes[i][j]='+';
							} else {

								colors[i][j]=random_color_1;//random color

								if(z == snake_x_coordinates.size() - 1) //snake's head
									shapes[i][j]=head_character;
								else
									shapes[i][j]='X';
							}


						} else {
							if(game_over){
								colors[i][j]=240;
								shapes[i][j]='#';
							} else {
								colors[i][j]=random_color_2;//random color

								if(z == snake_x_coordinates.size() - 1)
									shapes[i][j]=head_character;
								else
									shapes[i][j]='O';

							}
						}
						flag = 1;
						break;

					}

				if(!flag)//if no snake existed at i and j

					if(j == point_x and i == point_y){//if point existed at i and j

						colors[i][j]=45;
						shapes[i][j]='X';

					} else { //if neither point nor snake nor boundary, fill with ' '

						colors[i][j]=240;
						shapes[i][j]=' ';

					}
			}
	}

	//system("cls"); //no need to clear screen, this code changes parts that changed

	//simply draws the 2D array
	//update: only draws elements that changed from the last frame, this way speed increases several times
	for(int i = 0 ; i <= rows ; i++ ){
		for(int j = 0 ; j <= columns ; j++ ){

			//SetConsoleTextAttribute(hOut,colors[i][j]);


			//printf("%c",shapes[i][j]); //slow

			//cout<<shapes[i][j]; //very slow

			//if (shapes[i][j] != ' '){ //fast


			//'shapes' array is current output and 'game_board' is previous frame's output
			//if they match, no action is taken, so i and j coordinates remain as they were in the previous frame

			if (shapes[i][j] != game_board[i][j]){ //very fast //v3 bug: some coordinates remain black
				SetConsoleTextAttribute(hOut,colors[i][j]);

				coordinates.X = j;
				coordinates.Y = i;

				SetConsoleCursorPosition(hOut, coordinates );
				printf("%c",shapes[i][j]);

				game_board[i][j] = shapes[i][j];
				//game_board[i][j] = colors[i][j]; //v3 bug: some coordinates remain black
			}
		}
	}

	//coordinates.X = 0; //uncomment to see :|
	//coordinates.Y = rows+1;


	SetConsoleTextAttribute(hOut,15);//default color

	coordinates.X = columns + 11;
	coordinates.Y = 0;
	SetConsoleCursorPosition(hOut, coordinates );
	printf("%i",score); // x or y size is actually player's score + initial game size

	coordinates.X -= 1;
	coordinates.Y = 1;
	SetConsoleCursorPosition(hOut, coordinates );
	printf("%i",game_speed);

	coordinates.X += 8;
	coordinates.Y = 2;
	SetConsoleCursorPosition(hOut, coordinates );
	printf("%i",frame_number);


	if(game_over){
		coordinates.X = int(columns/2)-4;
		coordinates.Y = int(rows/2);
		SetConsoleCursorPosition(hOut, coordinates );
		printf("Game Over");


		coordinates.X = 0;
		coordinates.Y = rows+1;
		SetConsoleCursorPosition(hOut, coordinates );
	}

	if(stop_game){
		coordinates.X = int(columns/2)-2;
		coordinates.Y = int(rows/2);
		SetConsoleCursorPosition(hOut, coordinates );
		printf("STOP");
	}


	return game_board; //returns the current output
}

int validate_input(int max_val,int min_val){ //cin and check entered value
								 //if greater than min_val and less than max_val
								 //value is returned

	string str_input ;
	int temp_char_val , number_val ;
	bool is_invalid_input ;

	while(true){

		cin>>str_input;

		number_val = 0;
		is_invalid_input = 0;


		for( int i = 0 ; i < str_input.length() ; i++ ){

			temp_char_val = int(str_input[i]) - 48 ; //ascii code 48 : 0

			if( temp_char_val < 9 and temp_char_val >= 0 )
				for( int j = str_input.length() - i ; j > 1 ; j-- )
					temp_char_val *= 10;

			else
				is_invalid_input = 1; //if a non-numeric character is entered, is_invalid_input becomes true

			number_val += temp_char_val;

		}

		if( !is_invalid_input ) //if only numbers were entered, it's false, which becomes true with !

			if(number_val <= max_val and number_val >= min_val)
				return number_val;
			else
				cout<<"\nnumber must be between "<<min_val - 1<<" and "<<max_val + 1<<" : ";

		else
			cout<<"\nenter a positive integer : ";
	}
}