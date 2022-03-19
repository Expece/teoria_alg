#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include <conio.h>
#include<cstdio>
using namespace std;
const int DRP_SIZE = 21;

void init_drp(vector <vector <int>>& drp)
{
	vector <int> orderlist = { 0,0,0,0,1,1,0,0,1 };
	int ind = 0;
	for (int i = 0; i < DRP_SIZE; i++)
	{
		ind = 0;
		for (int j = 0; j < DRP_SIZE; j++)
		{
			drp[i][j] = orderlist[ind];
			ind++;
			if (ind > 2)
				ind = 0;
		}
		i++;
		ind = 3;
		for (int j = 0; j < 21; j++)
		{
			drp[i][j] = orderlist[ind];
			ind++;
			if (ind > 5)
				ind = 3;
		}
		i++;
		ind = 6;
		for (int j = 0; j < 21; j++)
		{
			drp[i][j] = orderlist[ind];
			ind++;
			if (ind > 8)
				ind = 6;
		}
	}
}

int init_drp_from_file(vector <vector <int>>& drp, int& argc, char* argv[])
{

	ifstream file_txt;
	string file;
	file = argv[1];
	file_txt.open(file);
	if (!(file_txt.is_open()))
	{
	C:
		cout << "File " << file << " is not found." << endl;
		cout << "Enter file name: ";
		cin >> file;
		file += ".txt";
		file_txt.open(file);
		if (!(file_txt.is_open()))
			goto C;
	}
	cout << "Open from " << file << endl;
	for (int i = 0; i < DRP_SIZE; i++)
	{
		for (int j = 0; j < DRP_SIZE; j++)
		{
			file_txt >> drp[i][j];
		}
	}
	file_txt.close();
	return 0;
}

int exit_file(vector <vector<int>> drp, vector <int> cell, int& argc, char* argv[])
{
	ofstream file_txt;
	file_txt.open(argv[2], ios::app);
	if (!(file_txt.is_open()))
		return 1;
	file_txt << cell[0] << endl;
	file_txt << cell[1] << endl;
	for (int i = 0; i < DRP_SIZE; i++)
	{
		for (int j = 0; j < DRP_SIZE; j++)
		{
			if (drp[i][j] == 0)
				file_txt << "_ ";
			else
				file_txt << "# ";
		}
		file_txt << endl;
	}
	file_txt.close();
	return 0;
}

void print_drp(vector <vector<int>> drp)
{
	for (int i = 0; i < DRP_SIZE; i++)
	{
		for (int j = 0; j < DRP_SIZE; j++)
		{
			if (drp[i][j] == 0)
				cout << "_ ";
			else
				cout << "# ";
		}
		cout << endl;
	}
}

int count_neighbour(vector <vector<int>> drp, int x, int y)
{
	int count = 0;
	for (int i = max(x - 1, 0); i <= min(x + 1, DRP_SIZE - 1); i++) {
		for (int j = max(y - 1, 0); j <= min(y + 1, DRP_SIZE - 1); j++) {
			if (drp[i][j] == 1)
				count++;
		}
	}
	if (drp[x][y] == 1)
		count--;

	return count;
}

vector<int> evolution(vector <vector<int>>& drp)
{
	vector <int> cell(2);	//cell[0]-dead cell[1]-alive
	vector <vector<int>> newDrp(21, vector<int>(DRP_SIZE));
	for (int i = 0; i < DRP_SIZE; i++)
	{
		for (int j = 0; j < DRP_SIZE; j++)
		{
			switch (drp[i][j])
			{
			case 0:
			{
				int live = count_neighbour(drp, i, j);
				if (live == 3)
				{
					newDrp[i][j] = 1;
					cell[1]++;
				}
				else
				{
					newDrp[i][j] = 0;
					cell[0]++;
				}
				break;
			}
			case 1:
			{
				int live = count_neighbour(drp, i, j);
				if (live < 2 || live>3)
				{
					newDrp[i][j] = 0;
					cell[0]++;
				}
				else
				{
					cell[1]++;
					newDrp[i][j] = 1;
				}
				break;
			}
			}
		}
	}
	drp = newDrp;
	return cell;
}

vector<int> input_and_print(vector <vector <int>>& drp, int& argc, char* argv[])
{
	vector <int> cell(2);
	char key;
	int keyToInt;
	for (;;)
	{
	C:
		cout << "Number of iteration: " << endl;
		key = _getch();
		if (key == 27)
			return cell;
		else
			keyToInt = key - '0';
		try
		{
			if ((int)key > 57 || (int)key < 48)
				throw 1;
		}
		catch (int)
		{
			system("cls");
			goto C;
		}

		for (int i = 0; i < keyToInt; i++)
		{
			cell = evolution(drp);
		}
		cout << keyToInt << " iteration:" << endl;
		print_drp(drp);
		if (keyToInt == 0)
			for (int i = 0; i < DRP_SIZE; i++)
			{
				for (int j = 0; j < DRP_SIZE; j++)
				{
					if (drp[i][j] == 0)
						cell[0]++;
					else
						cell[1]++;
				}
			}
		exit_file(drp, cell, argc, argv);

		system("pause");
		system("cls");
	}
}

int main(int argc, char* argv[])
{
	if (argc > 2)
	{
		fstream exit_file(argv[2], ios::out);
		exit_file << "";
		exit_file.close();
	}
	vector <vector <int>> drp(21, vector<int>(21));
	init_drp_from_file(drp, argc, argv);
	input_and_print(drp, argc, argv);

	return 0;
}