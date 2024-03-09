#include "iGraphics.h"
#include <GL/glut.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <utility>
#include<vector>
#include<math.h>

#define Main_Menu 0
#define MENU_PLAY 1
#define MENU_INSTRUCTIONS 2
#define MENU_HIGH_SCORE 3
#define MENU_SETTINGS 4
#define MENU_CREDITS 5


#define GRID_SIZE_CONST 9 
#define CELL_SIZE 50

int chosenBackground = 0;
int GRID_SIZE = 9;

int musicOn = 1;

char backgroundArray[20][20] = {"9(1).bmp","9(2).bmp","9(3).bmp","9(4).bmp"};

char playStateArray[20][20] = {"0.bmp","1.bmp","2.bmp","3.bmp","4.bmp","5.bmp","6.bmp","7.bmp","8.bmp","9(1).bmp","10.bmp"};

char gameArray[20][20] = {"easy.txt", "medium.txt", "hard.txt"};
char solutionArray[20][20] = {"easySolution.txt", "mediumSolution.txt", "hardSolution.txt"};
int playState =1;

int selectedCell[2]={4,3};
int SelectedCellisEditable[GRID_SIZE_CONST][GRID_SIZE_CONST];

int selectedCellX = 50 + selectedCell[0]* CELL_SIZE;
int selectedCellY = 50 + selectedCell[1]* CELL_SIZE;

int puzzle[GRID_SIZE_CONST][GRID_SIZE_CONST]; 
int solution[GRID_SIZE_CONST][GRID_SIZE_CONST];

int difficulty = 1;



int menuState = 0;
//int musicState = 1;

char ansCopy;
int score = 0;
int match =0;



using namespace std;



vector<pair<string, int>> highScores;
string playerName = "";








void drawMenu() {
    // Display the background image for the menu
	
    iShowBMP(0, 0, "menuBackground.bmp");

    // Display menu buttons using BMP 
    iShowBMP(500, 465, "play.bmp");
    iShowBMP(500, 365, "instructions.bmp");
    iShowBMP(500, 265, "highScore.bmp");
	iShowBMP(500, 165, "settings.bmp");
	iShowBMP(500, 65, "credits.bmp");

}

void musicplay(int music){
	if(music){


    if (menuState == 1 && playState != 10) {
        // Play game.wav during menuState 1 playState 9
        PlaySoundA("game.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		cout << "muttakin";
    } 
	else if(menuState == 1 && playState == 10){
		cout << "muttakin";
		PlaySoundA("finish.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else {
        // Play background.wav in all other cases
        PlaySoundA("background.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
	}
	else{
		PlaySoundA(NULL,NULL,NULL);
	}
}

void drawPlaystate(){
	
	if(difficulty==0) iShowBMP(0,0,playStateArray[playState]); //change
	else if(difficulty==1) iShowBMP(0,0,playStateArray[playState]);
	else if(difficulty ==2) iShowBMP(0,0, playStateArray[playState]);
	if(playState == 9){
		iShowBMP(0,0,backgroundArray[chosenBackground]);
	}
	if(playState<9){
	iShowBMP(245,55, "return.bmp");
	iShowBMP(365,55, "next.bmp");
	if(playState!=5){
	iShowBMP(10,530, "skip.bmp");
	}
	
	}
	if(playState==10){
		iShowBMP(600,520,"playAgain.bmp");
		iShowBMP(20,520,"menu.bmp");
	}
	
	
}


void drawGrid() {
	if(difficulty !=0){
	iShowBMP(50,100, "grid.bmp");
	}
	else{
		iShowBMP(49,99, "gridSmall.bmp");
	}

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            
        string num = to_string(puzzle[i][j]);
        
       
        if (puzzle[i][j] != 0) {
            char numCopy[20];
            strcpy(numCopy, num.c_str()); 
			
			if(selectedCell[0] != -1 && selectedCell[1] != -1 && (SelectedCellisEditable[selectedCell[0]][selectedCell[1]] || !puzzle[selectedCell[0]][selectedCell[1]])){
				iSetColor(227,36,43);
				//iRectangle(selectedCellX+9,selectedCellY+9,30,30);
				iCircle(selectedCellX+25,selectedCellY+25,20);
				iCircle(selectedCellX+25,selectedCellY+25,21);
				iCircle(selectedCellX+25,selectedCellY+25,19);
			}

			if(SelectedCellisEditable[i][j]){
				
			iSetColor(0,128,255);
			}
			else{
				
				iSetColor(0,0,0);
			}
            iText(50+i * CELL_SIZE + CELL_SIZE / 2 - 5, 100+j * CELL_SIZE + CELL_SIZE / 2 - 5, numCopy, GLUT_BITMAP_HELVETICA_18);
        	}
			
		}
	}
	
}

void readPuzzleFromFile() {
    
	ifstream file(gameArray[difficulty]);
   

    // Read puzzle data from file
    for (int i = 0; i < GRID_SIZE; ++i) { 
        for (int j = 0; j < GRID_SIZE; ++j) {
			file >> puzzle[i][j];
        }
    }

    // Close the file
    file.close(); 
}



void readSolutionFromFile() {
    ifstream file(solutionArray[difficulty]);

    // Read solution data from file into another array
    
    for (int i = 0; i < GRID_SIZE; ++i) { 
        for (int j = 0; j < GRID_SIZE; ++j) {
            file >> solution[i][j];
        }
    }

    // Close the file
    file.close();

    // Compare user's puzzle with the solution
    for (int i = 0; i < GRID_SIZE; ++i) { //change
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (SelectedCellisEditable[i][j]) {
				if(puzzle[i][j] == solution[i][j] ){
					
            		score += 10; // Increment score for each correct cell
				} 
				else if(puzzle[i][j]!=0 && puzzle[i][j] != solution[i][j]){
					score -=4;
				}
            }
        }
    }
	
	
}

void readHighScoresFromFile() { 
    highScores.clear();
    ifstream file("highScores.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            size_t pos = line.find(',');
            if (pos != string::npos) {
                string name = line.substr(0, pos);
                int score = stoi(line.substr(pos + 1));
                highScores.push_back({ name, score });
            }
        }
        file.close();
    }
}

void storePlayerScore() {
    ofstream file("highScores.txt", ios::app);
    if (file.is_open()) {
        file << playerName << "," << score << endl;
        file.close();
    }
}
void drawInstructions() {
    // Display the instruction image
    iShowBMP(0, 0, "instructionsBackground.bmp");
	iShowBMP(10, 530, "return.bmp");
}

void drawHighScore() {
    // Display the high score image
    iShowBMP(0, 0, "highScoreBackground.bmp");
	iShowBMP(10, 530, "return.bmp");

	// Sort the high scores
    sort(highScores.begin(), highScores.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    // Display the top three high scores
    for (int i = 0; i < min(3, (int)highScores.size()); ++i) {
        char scoreName[50];
		char scorePoint[20];

        sprintf(scoreName, "%d. %s", i + 1, highScores[i].first.c_str());
		sprintf(scorePoint,"%d", highScores[i].second );

		iShowBMP(35, 405-70*i ,"nameBackground.bmp");
        iText(50, 430 - i * 70, scoreName, GLUT_BITMAP_TIMES_ROMAN_24);
		iText(480, 430-i*70, scorePoint, GLUT_BITMAP_TIMES_ROMAN_24);
    }


}

void drawSettings(){

	iShowBMP(0,0,backgroundArray[chosenBackground]);
	if(difficulty==0){
		iShowBMP(310,350,"easy.bmp");
	}
	else if(difficulty==1){
		iShowBMP(310,350,"medium.bmp");
	}
	else if(difficulty==2){
		iShowBMP(310,350,"hard.bmp");
	}
	if(musicOn){
		iShowBMP(310, 250, "musicOn1.bmp");
	}
	else{
		iShowBMP(310, 250, "musicOff1.bmp");
	}
	

	iShowBMP(310, 150, "background.bmp");
	
	iShowBMP(10, 530, "return.bmp");
}

void drawCredits(){
	iShowBMP(0,0,"creditsBackground1.bmp");
	iShowBMP(10, 530, "return.bmp");
}


/*
	function iDraw() is called again and again by the system.

	*/


	void iDraw() {
    iClear();
	
    if (menuState == 0) {
        drawMenu();
		
    }
	else if(menuState == 1){
		//Game codes
		drawPlaystate();
		if(playState==5){
	char playerNameBuffer[100];

	iShowBMP(365, 55, "buttonBackground.bmp");
	iSetColor(255,255,255);
	if(playerName.empty()){
		iSetColor(211,211,211);
		iText(375, 79, "  Enter Name", GLUT_BITMAP_TIMES_ROMAN_24);
	
	}
	strcpy(playerNameBuffer, playerName.c_str());
	iText(375, 79, playerNameBuffer, GLUT_BITMAP_TIMES_ROMAN_24);
	
	
	}
	if(playState==9){
		
		
		drawGrid();
		iShowBMP(550,290,"submit.bmp");
		iSetColor(230,230,230);
		
		
	
	}

	if (playState == 10) {
    
    char scoreText[20];
    sprintf(scoreText, "Your Score: %d", score);
	

    iText(240, 405, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);
	
}

	
	}
	else if(menuState == 2){
		drawInstructions();
	}

	else if(menuState == 3){
		readHighScoresFromFile();
		drawHighScore();
	}

	else if(menuState ==4){
		drawSettings();
	}
	else if(menuState == 5){
		drawCredits();
	}

	
}


/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	printf("x = %d, y= %d\n",mx,my);
	//place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/

	void iMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		
        if (menuState == 0) {
			//things that happen in main menu

			
            // Check if play button is clicked
            if (x >= 500 && x <= 680 && y >= 465 && y <= 535) {
                
                menuState = MENU_PLAY;
				musicplay(musicOn);
				playState = 1;
				readPuzzleFromFile();
            }
            // Check if instructions button is clicked
            else if (x >= 500 && x <= 680 && y >= 365 && y <= 435) {
    
                menuState = MENU_INSTRUCTIONS;

            }
            // Check if high score button is clicked
            else if (x >= 500 && x <= 680 && y >= 265 && y <= 335) {
                menuState = MENU_HIGH_SCORE;
            }

			 // Check if credits button is clicked
            else if (x >= 500 && x <= 680 && y >= 165 && y <= 235) {
                menuState = MENU_SETTINGS; 
            }
			else if (x >= 500 && x <= 680 && y >= 65 && y <= 135) {
                menuState = MENU_CREDITS; 
            }
        }

		else if (menuState == 1){
			//What happens in game with mouse clicks
			
			if(playState<9){
			if((x >= 245 && x <= 345 && y >= 5 && y <= 115) && playState == 1){
				menuState= 0;
			}
			else if((x >= 365 && x <= 465 && y >= 55 && y <= 115) && playState!=5 ){
				playState++;
				
				
			}

			else if(x >= 245 && x <= 345 && y >= 55 && y <= 115){
				playState--;
				if(playState==0){
					musicplay(musicOn);
				}
			}
			else if(x >= 10 && x <= 75 && y >= 530 && y <= 570){
				if(playState<5){
				playState = 5;
				}
				if(playState>5){
				playState = 9;
				
				
				}

			}

			}

			if(playState==9){
				//musicplay(musicOn);
				for (int i = 0; i < GRID_SIZE; ++i) { 
        			for (int j = 0; j < GRID_SIZE; ++j) {
						if(x > (50 + j * CELL_SIZE) && x < 50 + (j+1) * CELL_SIZE && y> (100+ i * CELL_SIZE) && y> (50+ (i+1) * CELL_SIZE)){
							selectedCell[0]=j;
							selectedCell[1]=i;
							
							selectedCellX= 50 + j * CELL_SIZE;
							selectedCellY= 100+ i * CELL_SIZE;


						}
					}
				}

				if(x >550 && x< 730 && y>290 && y<360){
					readSolutionFromFile();
					storePlayerScore();
					playState =10;
				}

				
			}

			if(playState==10){
				musicplay(musicOn);
				if(x>600 && x<780 && y>520 && 590){
					playState=9;
					musicplay(musicOn);
					match =0;
					score = 0;
					/*for(int i=0;i<GRID_SIZE;i++){
					for(int j=0;j<GRID_SIZE;j++){
						puzzle[i][j]=0;
					}
				}*/
					readPuzzleFromFile();
					
				}
				else if(x>20 && x<200 && y>10 && 80){
					menuState=0;
					musicplay(musicOn);
					match =0;
					score = 0;
					playerName = "";
					
					
				}
			}



		}
		else if (menuState == 2){
			if (x >= 10 && x <= 110 && y >= 530 && y <= 590) {
                // return button
                menuState = Main_Menu;
            }
		}
		else if (menuState == 3){
			if (x >= 10 && x <= 110 && y >= 530 && y <= 590) {
                // return button
                menuState = Main_Menu;
            }
		}

		else if (menuState == 4){
			if (x >= 10 && x <= 110 && y >= 530 && y <= 590) {
                // return button
                menuState = Main_Menu;
            }
			else if( x>= 310 && x <= 490 && y >= 350 && y<= 420){

				if(difficulty==0){
					difficulty = 1;
					GRID_SIZE = 9;
				} 
				else if(difficulty==1) {
					difficulty = 2;
					GRID_SIZE = 9;
				}
				else if(difficulty==2) {
					difficulty = 0;
					GRID_SIZE = 6;
				}
			}

			else if( x>= 310 && x <= 490 && y >= 250 && y<= 320){
				if(musicOn) musicOn = 0;
				else musicOn =1;
				musicplay(musicOn);
			}
			else if(x>= 310 && x <= 490 && y >= 150 && y<= 220){
				if(chosenBackground<3) chosenBackground++;
				else chosenBackground =0;
			}
		}

		else if (menuState == 5){
			if (x >= 10 && x <= 110 && y >= 530 && y <= 590) {
                // return button
                menuState = Main_Menu;
            }
		}
	
    }


}


/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key) {

	
	
	iSetColor(255,255,255);

	if (menuState == 1) {
        if(playState == 5){
            if (key == '\b') {
                // Handle backspace to delete last character from player name
                if (!playerName.empty()) {
                    playerName.pop_back();

					glutPostRedisplay();
                }
            } else if (key == '\r') {
                // Handle enter to confirm player name input
                
                ofstream file("playerName.txt");
                if (file.is_open()) {
                    file << playerName << endl;
                    file.close();
                }
                // Proceed to play the game
                playState = 6;
            } else {
                // Append typed character to player name
                playerName += key;
				glutPostRedisplay();
            }

             // Trigger a redraw after modifying player name
        } 

		if(playState==9){
			if (selectedCell[0] != -1 && selectedCell[1] != -1 && (SelectedCellisEditable[selectedCell[0]][selectedCell[1]] || !puzzle[selectedCell[0]][selectedCell[1]])) {
        // Update puzzle number at selected cell
		if(key>48 && key<58){
		ansCopy = key;
		SelectedCellisEditable[selectedCell[0]][selectedCell[1]]=1;
        puzzle[selectedCell[0]][selectedCell[1]] = key - '0'; // Convert char to int
		}
		else if(key == '\b'){
			
			puzzle[selectedCell[0]][selectedCell[1]] = 0;
			

		}

		
		
			
		

        // Redraw the grid

        iClear();
        glutPostRedisplay();

    }
		}
	//place your codes for other keys here
}
	
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/

void iSpecialKeyboard(unsigned char key) {

	if (key == GLUT_KEY_END) {
		exit(0);
	}

	if(key == GLUT_KEY_RIGHT){
		if(selectedCell[0]<GRID_SIZE-1){
			
			selectedCell[0]++;
			while(selectedCell[0]<GRID_SIZE-1 && !((SelectedCellisEditable[selectedCell[0]][selectedCell[1]] || !puzzle[selectedCell[0]][selectedCell[1]]))){
			selectedCell[0]++;
			}
		}
	}
	else if(key == GLUT_KEY_LEFT){
		if(selectedCell[0]>0){			
			selectedCell[0]--;
			while(selectedCell[0]>0 && !((SelectedCellisEditable[selectedCell[0]][selectedCell[1]] || !puzzle[selectedCell[0]][selectedCell[1]]))){
			selectedCell[0]--;
			}
		}
	}
	else if(key == GLUT_KEY_UP){
		if(selectedCell[1]<GRID_SIZE-1){
			
			selectedCell[1]++;
			while(selectedCell[1]<GRID_SIZE-1 && !((SelectedCellisEditable[selectedCell[0]][selectedCell[1]] || !puzzle[selectedCell[0]][selectedCell[1]]))){
			selectedCell[1]++;
			}
		}
	}
	else if(key == GLUT_KEY_DOWN){
		if(selectedCell[1]>0){
			
			selectedCell[1]--;
			while(selectedCell[1]>0 && !((SelectedCellisEditable[selectedCell[0]][selectedCell[1]] || !puzzle[selectedCell[0]][selectedCell[1]]))){
			selectedCell[1]--;
			}
		}
	}

	selectedCellX= 50 + selectedCell[0] * CELL_SIZE;
	selectedCellY= 100+ selectedCell[1] * CELL_SIZE;

	//place your codes for other keys here
}



int main() {
	//place your own initialization codes here.
	
	readPuzzleFromFile();
	musicplay(musicOn);
	iInitialize(800, 600, "SudokU N Titan");
	
	
	return 0;
}