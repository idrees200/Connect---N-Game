#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MaxRows = 14;
const int MaxCols = 14;

struct Player {
    string Name;
    char Mark;

    Player(char mark = 'o', string PlayerName = "Player Name") {
        Mark = mark;
        Name = PlayerName;
    }

    void setPlayerName(string PlayerName) {
        Name = PlayerName;
    }

    void setPlayerMark(char PlayerMark) {
        Mark = PlayerMark;
    }
};

struct GameBoard {
    char Board[MaxRows][MaxCols];
    GameBoard(char FILL = '.') {
        for (int r = 0; r < MaxRows; r++)
            for (int c = 0; c < MaxCols; c++)
                Board[r][c] = FILL;
    }
};

struct Connect_N {
    GameBoard Board; // Game board to hold move data
    int Rows, Columns; // Game board size
    int ConnectLimit;// How many Marks to connect to win

    Player Players[2];// list of players
    int TurningPlayer; // index of turning player

    int GameEndState;
    // GameEndState 0 means game is not over
    // GameEndState 1 means first player has won
    // GameEndState 2 means second player has won

    int Moves_Made[MaxCols];
    // An array to hold the count of moves made in each
    // column. It will help us determine the row for
    // each move in an efficient way.

    Connect_N(int R = 6, int C = 7, int Limit = 4) {
        if (R < 0 || R > MaxRows)
            R = 6;

        if (C < 0 || C > MaxCols)
            C = 7;

        if (Limit < 0)
            Limit = 4;

        Rows = R;
        Columns = C;
        ConnectLimit = Limit;
        TurningPlayer = 0;
        GameEndState = 0;

        for (int i = 0; i < MaxCols; i++)
            Moves_Made[i] = 0;
    }

    bool MakeMove(int Col) {
        if (Col < 0 || Col >= Columns || Moves_Made[Col] >= Rows) {
            return false;
        }

        int R = Moves_Made[Col];
        Board.Board[Rows - 1 - R][Col] = Players[TurningPlayer].Mark;
        Moves_Made[Col]++;
        CheckWin(Rows - 1 - R, Col);

        TurningPlayer = (TurningPlayer + 1) % 2;
        return true;
    }

    void CheckWin(int Row, int Col) {
        // Check vertical
        if (WinVertical(Row, Col) || WinHorizontal(Row, Col) ||
            WinDiagonal(Row, Col) || WinAntiDiagonal(Row, Col)) {
            GameEndState = TurningPlayer + 1;
            cout << "GAME TERMINATED! WINNER IS " << Players[TurningPlayer].Name << "!" << endl;
            system("pause");
        }
    }

    bool WinVertical(int Row, int Col) {
        int counter = 0;
        for (int r = 0; r < Rows; r++) {
            if (Board.Board[r][Col] == Players[TurningPlayer].Mark) {
                counter++;
                if (counter == ConnectLimit)
                    return true;
            } else {
                counter = 0;
            }
        }
        return false;
    }

    bool WinHorizontal(int Row, int Col) {
        int counter = 0;
        for (int c = 0; c < Columns; c++) {
            if (Board.Board[Row][c] == Players[TurningPlayer].Mark) {
                counter++;
                if (counter == ConnectLimit)
                    return true;
            } else {
                counter = 0;
            }
        }
        return false;
    }

    bool WinDiagonal(int Row, int Col) {
        int counter = 0;
        for (int r = Row, c = Col; r < Rows && c < Columns; r++, c++) {
            if (Board.Board[r][c] == Players[TurningPlayer].Mark) {
                counter++;
                if (counter == ConnectLimit)
                    return true;
            } else {
                counter = 0;
            }
        }
        counter = 0;
        for (int r = Row, c = Col; r >= 0 && c >= 0; r--, c--) {
            if (Board.Board[r][c] == Players[TurningPlayer].Mark) {
                counter++;
                if (counter == ConnectLimit)
                    return true;
            } else {
                counter = 0;
            }
        }
        return false;
    }

    bool WinAntiDiagonal(int Row, int Col) {
        int counter = 0;
        for (int r = Row, c = Col; r >= 0 && c < Columns; r--, c++) {
            if (Board.Board[r][c] == Players[TurningPlayer].Mark) {
                counter++;
                if (counter == ConnectLimit)
                    return true;
            } else {
                counter = 0;
            }
        }
        counter = 0;
        for (int r = Row, c = Col; r < Rows && c >= 0; r++, c--) {
            if (Board.Board[r][c] == Players[TurningPlayer].Mark) {
                counter++;
                if (counter == ConnectLimit)
                    return true;
            } else {
                counter = 0;
            }
        }
        return false;
    }

    bool GameisOn() {
        return GameEndState == 0;
    }

    bool SaveGame(string FileName) {
        ofstream FOUT(FileName);
        if (!FOUT) {
            return false;
        }

        FOUT << Rows << " " << Columns << " " << ConnectLimit << " "
             << TurningPlayer << " " << GameEndState << endl;

        for (int r = 0; r < Rows; r++) {
            for (int c = 0; c < Columns; c++) {
                FOUT << Board.Board[r][c] << " ";
            }
            FOUT << endl;
        }

        for (int i = 0; i < Columns; i++) {
            FOUT << Moves_Made[i] << " ";
        }
        FOUT << endl;

        FOUT.close();
        return true;
    }

    bool LoadSavedGame(string FileName) {
        ifstream FIN(FileName);
        if (!FIN) {
            return false;
        }

        FIN >> Rows >> Columns >> ConnectLimit >> TurningPlayer >> GameEndState;

        for (int r = 0; r < Rows; r++) {
            for (int c = 0; c < Columns; c++) {
                FIN >> Board.Board[r][c];
            }
        }

        for (int i = 0; i < Columns; i++) {
            FIN >> Moves_Made[i];
        }

        FIN.close();
        return true;
    }

    void ShowGame() {
        for (int r = 0; r < Rows; r++) {
            for (int c = 0; c < Columns; c++) {
                cout << Board.Board[r][c] << "\t";
            }
            cout << endl << endl;
        }
        if (GameisOn()) {
            cout << Players[TurningPlayer].Name
                 << " (" << Players[TurningPlayer].Mark << ") "
                 << "Move" << endl;
        }
    }

    void Play() {
        int Move;
        do {
            ShowGame();
            cout << "ENTER NEGATIVE NUMBER OR NUMBER GREATER THAN COLUMNS FOR THE MAIN MENU" << endl;
            cout << "Please Enter Your Move (0 - " << Columns - 1 << "): ";
            cin >> Move;
            if (Move < 0 || Move >= Columns) {
                cout << "YOU ENTERED INVALID COLUMN NUMBER. PLAY AGAIN TO CONTINUE." << endl;
                break;
            }
            MakeMove(Move);
        } while (GameisOn());
    }
};

int Menu() {
    int Choice;
    do {
        cout << "1. Play" << endl
             << "2. Save Game" << endl
             << "3. Load Game" << endl
             << "4. Exit" << endl
             << "Enter Your Choice (1 - 4): ";
        cin >> Choice;
    } while (Choice < 1 || Choice > 4);
    return Choice;
}

void initNewGame(Connect_N& C) {
    int Rows, Cols, Limit;
    cout << "How Many Rows? ";
    cin >> Rows;
    cout << "How Many Cols? ";
    cin >> Cols;
    cout << "Connectivity Limit? ";
    cin >> Limit;
    C.Rows = Rows;
    C.Columns = Cols;
    C.ConnectLimit = Limit;
    cout << "First Player Name? ";
    string N;
    cin >> N;
    C.Players[0].setPlayerName(N);
    cout << "Second Player Name? ";
    cin >> N;
    C.Players[1].setPlayerMark('x');
    C.Players[1].setPlayerName(N);
}

int main() {
    int Choice;
    string FileName;
    bool GameNotLoaded = true;
    Connect_N Connect_Game(MaxRows, MaxCols, MaxRows);
    do {
        Choice = Menu();
        switch (Choice) {
        case 1:
            if (GameNotLoaded) {
                initNewGame(Connect_Game);
                GameNotLoaded = false;
            }
            Connect_Game.Play();
            break;
        case 2:
            cout << "Enter File Name: ";
            cin >> FileName;
            if (Connect_Game.SaveGame(FileName)) {
                cout << "GAME Saved Successfully" << endl;
            } else {
                cout << "GAME Not Saved Successfully" << endl;
            }
            break;
        case 3:
            cout << "Enter File Name: ";
            cin >> FileName;
            if (Connect_Game.LoadSavedGame(FileName)) {
                cout << "Game Loaded Successfully" << endl;
                Connect_Game.Play();
            } else {
                cout << "Game Not Loaded Successfully" << endl;
            }
            break;
        }
    } while (Choice != 4);
    return 0;
}
