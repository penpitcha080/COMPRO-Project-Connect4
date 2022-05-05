#include <array>
#include <iostream>
#include <string>
#include <list>
#include <iterator>
using namespace std;

bool gameOver = false;
int player = 1;
int currentplayer = 0;
string inGameArray[6][7];

std::list<std::array<int, 2>> player1Moves;
std::list<std::array<int, 2>> player2Moves;

void PrintGrid(string a[7][7], int size, string realArray[6][7]) //function print board
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i == 0)
            {
                a[i][j] = j;
                printf(" %d ",j+1);
            }
            else
            {
                if (realArray[i - 1][j].empty())
                {
                    a[i][j] = "[ ]";
                }
                else
                {
                    a[i][j] = realArray[i - 1][j];
                }
                cout << "" << a[i][j] << "";
            }

        }
        printf("\n");
    }
}

void SwitchTurn() //function switch player
{
    currentplayer++;
    if (currentplayer > 1)
    {
        currentplayer = 0;
    }
}

void SetGameOver(bool setValue)
{
    gameOver = setValue;
}

bool FollowThePath(int pattern[2], string choicesInput, string gameArray[6][7], int currentSlot[2]) //check connected
{
    int connected = 1;
    for (int x = 1; x <= 4; x++)
    {
        int setX = -pattern[0] + currentSlot[0];
        int setY = pattern[1] + currentSlot[1];
        if (setX >= 0 && setX < 6 && setY >= 0 && setY < 7)
        {
            if (gameArray[setX][setY] == choicesInput)
            {
                connected++;
                currentSlot[0] = setX;
                currentSlot[1] = setY;
                if (connected >= 4)
                {
                    break;
                }
            }
            else
            {
                connected = 0;
                break;
            }
        }
        else
        {
            break;
        }
    }
    if (connected >= 4)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CheckTheBoardForMatch(string choicesInput, string gameArray[6][7], int pos[2]) //check win
{
    for (int x = pos[0] - 1; x <= pos[0] + 1; x++)
    {
        for (int y = pos[1] - 1; y <= pos[1] + 1; y++)
        {
            if (x >= 0 && x < 6 && y >= 0 && y < 7 && gameArray[x][y] == choicesInput && !gameOver)
            {
                int myX = pos[0];
                int myY = pos[1];
                if (myX != x || myY != y)
                {
                    string t = gameArray[x][y];
                    int diffX = myX - x;
                    int diffY = y - myY;
                    int patternSet[2] = { diffX, diffY };
                    int currentSlot[2] = { myX, myY };
                    bool foundConnection = FollowThePath(patternSet, choicesInput, gameArray, currentSlot);
                    if (foundConnection)
                    {
                        SetGameOver(true);
                        printf("\n===== GAME OVER! =====\n");
                    }
                }
            }
        }
    }
}


bool AddToBoard(int slot, int player, string realArray[6][7], string token) //function add to board
{
    bool isFull = false;
    int lowestX = 0;
    int rowX = 6;
    for (int i = 0; i < rowX; i++)
    {
        if (realArray[i][slot].empty())
        {
            lowestX = i;
        }
        else
        {
            if (i == 0)
            {
                isFull = true;
            }
        }
    }
    printf("\n");
    if (!isFull) //check full slot
    {
        realArray[lowestX][slot] = "[" + token + "]";
        int position[2] = { lowestX,slot };
        if (currentplayer == 0)
        {
            player1Moves.push_front({ {position[0], position[1]}});

        }
        else
        {
            player2Moves.push_front({ {position[0], position[1]} });

        }
        CheckTheBoardForMatch(realArray[lowestX][slot], realArray, position);
        return true;
    }
    else
    {
    	printf("Slot is full, please select another slot\n\n");
        return false;
    }
}

void LoopThroughPastMoves(int playerSelected, string playerToken, list<std::array<int, 2>> allMoves, string gameArray[6][7])
{
    for (auto const& i : allMoves) 
    {
        int currMove[2] = { i[0], i[1] };
        string realToken = "[" + playerToken + "]";
        CheckTheBoardForMatch(realToken, gameArray, currMove);
        if (gameOver)
        {
            break;
        }
    }
}

int main() //game start
{
    int size = 7;
    string showArray[7][7];
    string choices[2] = {"X", "O"};
    int selectSlot;
    printf("Welcome to Connect 4 game\n");
    printf("Player 1 is X\n");
    printf("Player 2 is O\n\n");
    while (!gameOver)
    {
        PrintGrid(showArray, size, inGameArray);
        printf("Player %d, Choose a slot: ",currentplayer+1);
        scanf("%d",&selectSlot);
        if (selectSlot > 0 && selectSlot <= size) //check number
        {
            string selectChoices = choices[currentplayer];
            bool isBoardFull = AddToBoard(selectSlot-1, currentplayer, inGameArray, selectChoices);
            if (isBoardFull && !gameOver)
            {
                if (currentplayer == 0)
                {
                    LoopThroughPastMoves(currentplayer, selectChoices, player1Moves, inGameArray);
                }
                else
                {
                    LoopThroughPastMoves(currentplayer, selectChoices, player2Moves, inGameArray);
                }
            }

            if (isBoardFull && !gameOver)
            {
                SwitchTurn();
            }
            if (gameOver)
            {
                PrintGrid(showArray, size, inGameArray);
                printf("Player %d is the winner!!!\n\n",currentplayer+1);

            }
        }
        else //number < 1 && number > 7
        {
        	printf("!!! Please enter number between 1 and 7 !!!\n\n");
        }
    }
}