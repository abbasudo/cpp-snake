/*
* file    :  snakegame.cpp
*
* title   :  Snake game beta
*
* version :  beta
*
* C abbas makhzomi 
* copyrights received
*/

#include <iostream>
#include <conio.h>
#include <vector>

#include <windows.h>// sleep()

#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

using namespace std;

void output(int,int,int,int,vector<int>,vector<int>);
bool isgameover(int,int,vector<int>,vector<int>);
bool ispoint(int,int,int,int);
int iskeypressed(int,int);
int validation(int,int);


int main(){
	
	char chr;

	
	srand(time(0));  // Initialize random number generator.
	
	cout<<"tole zamin bazi morede nazar ra vared konid (byne 10-25)(tole monaseb '15') : ";
	int tol = validation(25,10);
	
	cout<<"tole zamin bazi morede nazar ra vared konid (byne 10-80)(arze monaseb '40') : ";
	int arz = validation(80,10);
	
	cout<<"sorate bazi ra vared konid (1 sari tarin - 20 kond tarin)(sorate monaseb '4') : ";
	int time = validation(20,1);              //1 yani 1*50 mili sanie be ezaye har frim
	
	
	cout<<"az W-S-A-D ya felesh ha baraye harekat estefade konid\nbraye shoro kilidi ra feshar dahid";
	
	getch();
	
	
	//mokhtasate mar ra zakhire mikonad
	vector<int> x;
	vector<int> y;

	x.push_back(int(arz/2)); //avalin element = khonye vasate bazi
    y.push_back(int(tol/2));
    
    x.push_back(int(arz/2+1)); //avalin element = khonye baqale khonye aval
    y.push_back(int(tol/2));
	
	//be in sorat yek mar ba toole 2 dar avale bazi tolid mishavad
	
	//harekate pishfarz dar shore bazi
	int lastmove = 3;

	//avalin tome be sorate random dar mahdodye bazi ijad mishavad
	int pointx = rand() % (arz - 1) + 1;
	int pointy = rand() % (tol - 1) + 1;
	
	while(true){
		
		
		//tol,arz,pointx,pointy,x,y = tol va arz , mokhtasate toome , vectore mokhtasate mar 
		output(tol,arz,pointx,pointy,x,y); //cout kardane shekle bazi

			
		
		//avalin ejra 3
		switch(lastmove){
			case 1:
				
				//cout<<"UP";
				x.push_back ( x.back() );   	//yek element be vector ezafe mikonad (manade akhrin elemente mojod dar vector)
				y.push_back ( y.back() - 1 );	//chon harkat dar jehate bala bode ast az y elemente qabli yki kaste ba be onvane
												//elemente jadid ezafe mishavad (harekat dar jehate - bordare y ha ast zira dar
												//in barname bordar be sorate varon ast)
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
				
		}
		
		lastmove = iskeypressed(time,lastmove);//chek kardane kilide feshari va daryafte move jadid 
											   //be zabone sade lastmove update mishavad
											   //lozoman taqier nmikonad (dar sorate feshar nadadne hich
											   //kilid haman meqdare qabli mimanad va dar haman jehat 
											   //harekat mikonad)
		
		if(!ispoint(x.back(),y.back(),pointx,pointy)){
			
			//agar emtiaz daryaft nashod avalin elemente vector (akhrin O) ra hazf mikonad
			//zire yek elemente jadid dar jehate harekat be vector ezafe shod
			x.erase (x.begin());
			y.erase (y.begin());
			
		} else {
			
			//agar tome khorde shod elementi hazf nmishavad dar natije mar bozorg mishavad
			//dar vaqe x va y tome qabli tabdil be x va y mishavad (be vector ezafe mishavad vali az mar chizi hazf nmishavad)
			
			//sepas yek x va y randome jadid dar mahdode bazi iejad mishavad
			//be in sorat tome qabli hazf mishavad
			pointx = rand() % (arz - 1) + 1;
			pointy = rand() % (tol - 1) + 1;
		
		}

		
		
		
		if(isgameover(tol,arz,x,y))	//agar game over shod halqe ra break mikonad ke be manaie khoroj az ravande bazi ast
			break;
		

	}
	

	
	
	cout<<"\n\nGame Over";
	
	cout<<"\ndar sorate vazeh nabodane bazi moshkel az cpu ya console shomast , tol va arze kamtar va sorate bishtar vared konid";
	getch();
	
	return 0;
}


int iskeypressed(int sec,int lastmove){ //1 (bala) - 2 (paien) - 3 (chap) - 4 (rast) midahad
	char chr;

	//sec tedade 50 mili sanie ast ke bayad tey shavad 
	//dar sorate in ke hich kilidi daryaft nashod kilide qabli (lastmove) return mishavad
	//yani mar be harkate qablie khod edame midahad
	//ama agar kilid feshor dade shod harkate jadid sabt mishavad
	
	while( sec > 0 ){ 
		
		

        if( kbhit() ) { //if there is a key in keyboard buffer
            chr = getch(); //get the char
            
	        switch(chr){
				case 'W':
				case 'w':
				case 72: //feleshe bala
					if(lastmove != 2)//agar mar ro be paien harekat mikard nmitavanad ro be bala harekat konad
						lastmove = 1;
					break;
					
				case 'S':
				case 's':
				case 80:
					if(lastmove != 1)
						lastmove = 2;
					break;
					
				case 'A':
				case 'a':
				case 75:
					
					if(lastmove != 4)
						lastmove = 3;
					break;
					
				case 'D':
				case 'd':
				case 77:
					
					if(lastmove != 3)
						lastmove = 4;
					break;
					
			}

        }

        Sleep(50); //50 milliseconds sleep
        --sec; //countdown a 50 milliseconds
    }
			
		
	return lastmove;

}

bool ispoint(int lastx,int lasty,int pointx,int pointy){ // agar tome khorde shod 1 dar qyre in sorat 0 midahad
	
	if(lastx == pointx and lasty == pointy) //chek kardane raase mar 
											//agar kole vector ra chek mikardim , momken bod tome tasadofi roye mar bashad 
											//ba chek kardane faqat akhrin element az emtiaze ezafe jelo giri mishavad
		return 1;
	else 
		return 0;

}


bool isgameover(int row,int col,vector<int> x,vector<int> y){ //agar game over shod 1 dar qyre in sorat 0 midahad
	
	for(int z = 0; z < x.size(); z++){

		if(x[z] == 0 or x[z] == col or y[z] == 0 or y[z] == row) //agar mar be marz khord 1 return mishavad
			return 1;
		for(int k = 0; k < x.size(); k++)
			if(k != z and x[k] == x[z] and y[k] == y[z]) //agar elemente tekrari dar vector bod 1 return midahad
														 //x va y tekrari bashand yani mar az roye khodash rad shode ast
														 //agar sharte aval (k != z ) hamishe shart true ast
				return 1;
	}
	return 0; //agar ta in marhale 1 return nashode 0 return mishavad yani game over nashode ast

}

void output(int row,int col,int pointx,int pointy,vector<int> x ,vector<int> y){ //cout kardane bazi , har fraime in tabe call mishavad
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	bool flag;
	
	system("cls"); //harche be cout nazdiktar bashad bazi stable tar ast , vali agar dar loob dakhel shavad 
				   //har bar safhe pak mishavad dar natije chizi namayesh nmidahad 
				   //inja behtarin jaie ast ke mitavan in dastor ra gharar dad
	
	for(int i = 0 ; i <= row ; i++ ){
		for(int j = 0 ; j <= col ; j++ )
			if(i == 0 or i == row or j == 0 or j == col){ //marze bazi - radife 0 va akhar va sotone 0 va akhar 
				
				SetConsoleTextAttribute(hOut,48); //marze bazi be range abi
				cout<<" ";
				SetConsoleTextAttribute(hOut,15); //bargardandane range pishfarz
				
			} else {
				
				flag = 0;
				for(int z = 0; z < x.size(); z++){
						if(j == x[z] and i == y[z]){
						
						SetConsoleTextAttribute(hOut,202); //mar be range sabz roye qemez
						cout<<'O';
						SetConsoleTextAttribute(hOut,15); //bargardandane range pishfarz
						
						flag = 1;
						break;
						
					}
				}
				if(!flag){
					if(j == pointx and i == pointy){
						SetConsoleTextAttribute(hOut,45); //tome be range sabz
						cout<<'X';
						SetConsoleTextAttribute(hOut,15); ////bargardandane range pishfarz
						
					} else {
						SetConsoleTextAttribute(hOut,240); //zamine bazi be range sefid
						cout<<" ";
						SetConsoleTextAttribute(hOut,15); //bargardandane range pishfarz
					}
				}			
			}
		
		cout<<endl;
	}
	cout<<"emtiaz : "<<x.size() - 2;	//size x ha ya y ha dar vaqe emtiaze bazikon + 2 size avale bazi
}

int validation(int max,int min){ //cin kardan va chek kardane meqdare vared shode 
								 //agar az min bozorgtar va az max kochaktr bashad
								 //meqdar return mishavad
	
	
	string str ;
	int    t , num ;
	bool   flag ;
	
	while(true){
		
		cin>>str;
		
		num  = 0;
		flag = 0;
		
		
		for( int i = 0 ; i < str.length() ; i++ ){
			
			t = int(str[i]) - 48 ; //ascii code 48 : 0
			
			if( t < 9 and t >= 0 )
				for( int j = str.length() - i ; j > 1 ; j-- )
					t *= 10;
			
			 else 
				flag = 1; //agar yek char qyre shomare vared shod flag true mishavad

			num += t;
			
		}
		
		if( !flag ) //agar faqat adad vared shode bod fulse ast ke ba ! true mishavad
			if(num <= max and num >= min)
				return num;
			else
				cout<<"\nadad bayad byne "<<min - 1<<" va "<<max + 1<<" bashad : ";
		
		cout<<"\nadade sahih va mosbat vared konid : ";
		
	}
}
