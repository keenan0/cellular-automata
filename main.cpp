/*
          IMPORTANT:

          The game will run only if you add a data.txt file that will contain the starting configuration.

          Example of a valid data.txt input:

          4
          1 0 0 1
          1 1 0 0
          0 0 1 0
          0 0 1 1

          A value of 1 represents a living cell.
          A value of 0 represents a dead cell.
*/

////CELULAR AUTOMATA @ CLAUDIU MIRONESCU

#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <iomanip>
#define ALIVE "\x1B[41m"
#define RESET "\x1B[40m"
#define DEAD "\x1B[40m"
using namespace std;

ifstream fin("data.txt");

const int WORLD_SIZE = 30;
const int SLEEP_TIME = 800;
int frame = 0;

bool world[WORLD_SIZE][WORLD_SIZE];
bool startingChunk[WORLD_SIZE][WORLD_SIZE];

bool bufferSwitch = true;
int buffer1[WORLD_SIZE][WORLD_SIZE];
int buffer2[WORLD_SIZE][WORLD_SIZE];

int di[] = { 0, 0, -1, -1, -1, 1, 1, 1 };
int dj[] = { 1, -1, 0, -1, 1, 0, -1, 1 };

void LoadChunk() {
	int chunkSize;
	fin >> chunkSize;

	if (chunkSize > WORLD_SIZE) return;

	int worldStart = (WORLD_SIZE - chunkSize) / 2;
	for (int i = 0; i < chunkSize; ++i)
		for (int j = 0; j < chunkSize; ++j) {
			fin >> startingChunk[i][j];

			world[worldStart + i][worldStart + j] = startingChunk[i][j];
		}

	for (int i = 0; i < WORLD_SIZE; ++i)
		for (int j = 0; j < WORLD_SIZE; ++j)
			buffer1[i][j] = world[i][j];
}

void Print() {
	cout << "    " << "FRAME NO: " << frame << '\n';
	
	if (bufferSwitch) {
		for (int i = 0; i < WORLD_SIZE; ++i) {
			for (int j = 0; j < WORLD_SIZE; ++j) {
				if (buffer1[i][j]) cout << ALIVE;
				else cout << DEAD;

				cout << setw(2) << ' ' << RESET;
			}
			cout << '\n';
		}
	}
	else {
		for (int i = 0; i < WORLD_SIZE; ++i) {
			for (int j = 0; j < WORLD_SIZE; ++j) {
				if (buffer2[i][j]) cout << ALIVE;
				else cout << DEAD;

				cout << setw(2) << ' ' << RESET;
			}
			cout << '\n';
		}
	}
}

int GetNeighbours(int x, int y) {
	int aliveNeighbours = 0;

	for (int k = 0; k < 8; ++k) {
		int newX = x + dj[k];
		int newY = y + di[k];

		if (0 <= newY && newY < WORLD_SIZE && 0 <= newX && newX < WORLD_SIZE) {
			if (bufferSwitch) {
				if (buffer1[newY][newX]) aliveNeighbours++;
			}
			else {
				if (buffer2[newY][newX]) aliveNeighbours++;
			}
		}
	}

	return aliveNeighbours;
}

void SimulateFrame() {
	for (int y = 0; y < WORLD_SIZE; ++y) {
		for (int x = 0; x < WORLD_SIZE; ++x) {
			int aliveNeighbours = GetNeighbours(x, y);

			if (bufferSwitch) {
				if (buffer1[y][x] == 0) {
					if (aliveNeighbours == 3)
						buffer2[y][x] = 1;
				}
				else if (buffer1[y][x] == 1) {
					if (aliveNeighbours != 2 && aliveNeighbours != 3)
						buffer2[y][x] = 0;
					else buffer2[y][x] = 1;
				}
			}
			else {
				if (buffer2[y][x] == 0) {
					if (aliveNeighbours == 3)
						buffer1[y][x] = 1;
				}
				else if (buffer2[y][x] == 1) {
					if (aliveNeighbours != 2 && aliveNeighbours != 3)
						buffer1[y][x] = 0;
					else buffer1[y][x] = 1;
				}
			}
		}
	}

	if (bufferSwitch) {
		for (int i = 0; i < WORLD_SIZE; ++i)
			for (int j = 0; j < WORLD_SIZE; ++j)
				buffer1[i][j] = 0;
	}
	else {
		for (int i = 0; i < WORLD_SIZE; ++i)
			for (int j = 0; j < WORLD_SIZE; ++j)
				buffer2[i][j] = 0;
	}

	bufferSwitch = !bufferSwitch;
	frame++;
}

int main() {
	cin.tie(NULL);
	ios_base::sync_with_stdio(false);
	LoadChunk();

	while (true) {
		Print();
		SimulateFrame();
		Sleep(SLEEP_TIME);
		system("CLS");
	}
}
