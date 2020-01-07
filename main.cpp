#include<iostream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<conio.h>
#include<Windows.h>
#include<vector>	// C++ vectror   : https://blockdmask.tistory.com/70
#include<fstream>	// C++ 파일입출력 : https://blockdmask.tistory.com/322
using namespace std;

// [행맨 게임] by. BlockDMask
// 안녕하세요.BlockDMask 입니다.
// 사용하실땐 꼭 출처를 남겨주세요.
// 블로그 : https://blockdmask.tistory.com/369

//콘솔 내부의 특정 위치로 커서를 이동시키는 함수 입니다.
void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = 2 * x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//keyboard 입력
int GetKeyDown()
{
	if (_kbhit() != 0)
	{
		return _getch();
	}
	return 0;
}

//콘솔 세팅
void SetConsoleView()
{
	system("mode con:cols=50 lines=20");
	system("title [HangManGame] by.BlockDMask");
}

//시작화면 draw
void DrawReadyGame()
{
	system("cls");
	gotoxy(5, 2);
	cout << "==============================";
	gotoxy(5, 3);
	cout << "======= HANG MAN GAME ========";
	gotoxy(5, 4);
	cout << "==============================";
	gotoxy(6, 6);
	cout << "시작하려면 's'를 눌러주세요";
	gotoxy(6, 7);
	cout << "종료하려면 'q'를 눌러주세요";
	gotoxy(9, 14);
	cout << "by. BlockDMask" << endl;
}

//게임화면 draw
void DrawStartGame(int life, int score, vector<string>& pastWord)
{
	system("cls");
	gotoxy(5, 1);
	cout << "life = " << life;
	gotoxy(5, 2);
	cout << "score = " << score;
	gotoxy(5, 8);
	cout << "past = ";
	for (int i = 0; i < static_cast<int>(pastWord.size()); ++i)
	{
		cout << pastWord[i] << " ";
	}

	gotoxy(5, 12);
	cout << "input = ";
	gotoxy(13, 14);
	cout << "메인화면 'qq'" << endl;
}

//사전을 세팅하는 함수 입니다.
//[C++] 파일입출력 포스팅 : https://blockdmask.tistory.com/322
void SetDictionary(vector<string>& strArr)
{
	static const int INIT_NUM = 4;
	static const string str[INIT_NUM] = { "apple", "banana", "code", "program" };	//샘플 단어
	ifstream readFromFile("words.txt");
	if (!readFromFile.is_open())
	{
		ofstream writeToFile("words.txt");
		for (int i = 0; i < INIT_NUM; ++i)
		{
			string tmp = str[i];
			if (i != INIT_NUM - 1)
			{
				tmp += "\n";
			}
			writeToFile.write(tmp.c_str(), tmp.size());
			strArr.push_back(str[i]);
		}
		writeToFile.close();
		return;
	}

	while (!readFromFile.eof())
	{
		string tmp;
		getline(readFromFile, tmp);
		strArr.push_back(tmp);
	}
	readFromFile.close();
	return;
}

//시작화면 기능
bool ReadyGame()
{
	DrawReadyGame();
	while (true)
	{
		int key = GetKeyDown();
		if (key == 's' || key == 'S')
		{
			return true;
		}
		else if (key == 'q' || key == 'Q')
		{
			break;
		}
	}
	return false;
}

//게임 시작 함수.
void StartGame()
{
	int score = 0;
	vector<string> pastWord;
	vector<string> strArr;
	SetDictionary(strArr);		//read from file

	while (true)	//하나의 탄을 표현하는 루프
	{
		//1 play
		int num = 0;
		srand((unsigned int)time(NULL));		//랜덤함수 : https://blockdmask.tistory.com/308
		num = rand() % static_cast<int>(strArr.size());

		string strQuestion;
		const string strOriginal = strArr[num];	//단어가 맞는지 정답확인용으로 저장
		const int originLen = static_cast<int>(strOriginal.length());

		//init
		for (int i = 0; i < originLen; ++i)
		{
			strQuestion += "_";
		}

		int life = originLen + 2;

		//1 question
		while (true)
		{
			DrawStartGame(life, score, pastWord);

			//draw question
			gotoxy(5, 5);
			for (int i = 0; i < originLen; ++i)
			{
				cout << strQuestion[i] << " ";
			}
			cout << endl;

			//input
			gotoxy(9, 12);
			string strInput;
			cin >> strInput;
			if (strInput == "qq")
			{
				return;
			}
			pastWord.push_back(strInput);

			if (strInput.length() == 1)
			{
				//alphabet
				for (int i = 0; i < originLen; ++i)
				{
					if (strOriginal[i] == strInput[0])
					{
						strQuestion[i] = strInput[0];
					}
				}
			}
			else if (strInput.length() > 1)
			{
				//word
				if (strOriginal == strInput)
				{
					//score up !!
					score += 5;
					pastWord.clear();
					break;
				}
			}

			life -= 1;
			if (life < 0)
			{
				score -= 5;
				if (score < 0)
				{
					score = 0;
				}
				pastWord.clear();
				break;
			}
		}
	}
}

//메인함수
int main(void)
{
	SetConsoleView();
	bool isStart = false;
	while (true)
	{
		isStart = ReadyGame();
		if (isStart)
		{
			StartGame();		//게임시작
		}
		else
		{
			break;
		}
	}
	return 0;
}