/*
* file : snake_game.cpp
*
* title : Snake game beta
*
* version : beta
*
* C abbas makhzomi
* copyrights received
*/

#include <iostream>
#include <conio.h>
#include <vector>

#include <windows.h>// sleep()

#include <ctime> // For time()
#include <cstdlib> // For srand() and rand()

using namespace std;

void display_output(int,int,int,int,vector<int>,vector<int>);
bool is_game_over(int,int,vector<int>,vector<int>);
bool has_point_been_eaten(int,int,int,int);
int get_key_pressed(int,int);
int validate_input(int,int);


int main(){

	char character;


	srand(time(0)); // Initialize random number generator.

	int board_height = 15;

	int board_width = 40;

	int game_speed = 4; // 1 means 1*50 milliseconds per frame

	cout<<"Use W-S-A-D or arrow keys to move\nPress any key to start";

	getch();


	//saves snake coordinates
	vector<int> snake_x_coordinates;
	vector<int> snake_y_coordinates;

	snake_x_coordinates.push_back(int(board_width/2)); //first element = middle of the board
	snake_y_coordinates.push_back(int(board_height/2));

	snake_x_coordinates.push_back(int(board_width/2+1)); //first element = next to the first element
	snake_y_coordinates.push_back(int(board_height/2));

	//this way, a snake with a length of 2 is created at the beginning of the game

	//default movement at the start of the game
	int last_move = 3;

	//the first point is created randomly within the game boundaries
	int point_x = rand() % (board_width - 1) + 1;
	int point_y = rand() % (board_height - 1) + 1;

	while(true){


		//board_height,board_width,point_x,point_y,snake_x_coordinates,snake_y_coordinates = board_height and board_width , point coordinates , snake coordinate vector
		display_output(board_height,board_width,point_x,point_y,snake_x_coordinates,snake_y_coordinates); //cout game shape


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

		}

		last_move = get_key_pressed(game_speed,last_move);//check pressed key and get new move
											   //simply, last_move is updated
											   //does not necessarily change (if no key is pressed
											   //it remains the same as the previous value and moves
											   //in the same direction)

		if(!has_point_been_eaten(snake_x_coordinates.back(),snake_y_coordinates.back(),point_x,point_y)){

			//if score was not received, removes the first element of the vector (last 'O')
			//because a new element was added to the vector in the direction of movement
			snake_x_coordinates.erase (snake_x_coordinates.begin());
			snake_y_coordinates.erase (snake_y_coordinates.begin());

		} else {

			//if point is eaten, no element is removed, so the snake grows
			//in fact, the previous point's x and y become x and y (added to the vector but nothing is removed from the snake)

			//then a new random x and y are created within the game boundaries
			//this way, the previous point is removed
			point_x = rand() % (board_width - 1) + 1;
			point_y = rand() % (board_height - 1) + 1;

		}


		if(is_game_over(board_height,board_width,snake_x_coordinates,snake_y_coordinates))	//if game over, breaks the loop, which means exiting the game process
			break;


	}


	cout<<"\n\nGame Over";

	cout<<"\nif the game is not clear, the problem is with your CPU or console. Enter smaller width and height and higher speed";
	getch();

	return 0;
}


int get_key_pressed(int delay_in_ms,int last_move){ //returns 1 (up) - 2 (down) - 3 (left) - 4 (right)
	char character;

	//delay_in_ms is the number of 50 milliseconds that must pass
	//if no key is received, the previous key (last_move) is returned
	//meaning the snake continues its previous movement
	//but if a key is pressed, the new movement is registered

	while( delay_in_ms > 0 ){


		if( kbhit() ) { //if there is a key in keyboard buffer
			character = getch(); //get the char

			switch(character){
				case 'W':
				case 'w':
				case 72: //up arrow
					if(last_move != 2)//if snake was moving down, it cannot move up
						last_move = 1;
					break;

				case 'S':
				case 's':
				case 80:
					if(last_move != 1)
						last_move = 2;
					break;

				case 'A':
				case 'a':
				case 75:

					if(last_move != 4)
						last_move = 3;
					break;

				case 'D':
				case 'd':
				case 77:

					if(last_move != 3)
						last_move = 4;
					break;

			}

		}

		Sleep(50); //50 milliseconds sleep
		--delay_in_ms; //countdown a 50 milliseconds
	}


	return last_move;

}

bool has_point_been_eaten(int last_x,int last_y,int point_x,int point_y){ // if point is eaten, returns 1, otherwise 0

	if(last_x == point_x and last_y == point_y) //checking snake's head
											//if we checked the whole vector, the point might accidentally be on the snake
											//by checking only the last element, we prevent extra points
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

void display_output(int rows,int columns,int point_x,int point_y,vector<int> snake_x_coordinates ,vector<int> snake_y_coordinates){ //cout game, this function is called for each frame
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	bool flag;

	system("cls"); //the closer to cout, the more stable the game, but if it enters the loop
				   //the screen will be cleared every time, so nothing will be displayed
				   //here is the best place to put this command

	for(int i = 0 ; i <= rows ; i++ ){
		for(int j = 0 ; j <= columns ; j++ )
			if(i == 0 or i == rows or j == 0 or j == columns){ //game boundary - row 0 and last and column 0 and last

				SetConsoleTextAttribute(hOut,48); //game boundary in blue
				cout<<" ";
				SetConsoleTextAttribute(hOut,15); //reset to default color

			} else {

				flag = 0;
				for(int z = 0; z < snake_x_coordinates.size(); z++){
						if(j == snake_x_coordinates[z] and i == snake_y_coordinates[z]){

						SetConsoleTextAttribute(hOut,202); //snake in green on red
						cout<<'O';
						SetConsoleTextAttribute(hOut,15); //reset to default color

						flag = 1;
						break;

					}
				}
				if(!flag){
					if(j == point_x and i == point_y){
						SetConsoleTextAttribute(hOut,45); //point in green
						cout<<'X';
						SetConsoleTextAttribute(hOut,15); ////reset to default color

					} else {
						SetConsoleTextAttribute(hOut,240); //game background in white
						cout<<" ";
						SetConsoleTextAttribute(hOut,15); //reset to default color
					}
				}
			}

		cout<<endl;
	}
	cout<<"score : "<<snake_x_coordinates.size() - 2;	//size of x or y is actually player's score + 2 initial sizes of the game
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

		cout<<"\nenter a positive integer : ";

	}
}