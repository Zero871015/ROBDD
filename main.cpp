// Name: zero871015(B10615032)
// Date: 2018/10/11
// Last Update: 2018/10/13
// Problem statement: ROBDD Generation

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

//Data structure of a node.
struct Node
{
	char Variable;	//the node's name
	int Else_edge, Then_edge;	//1 and 0 to go.
	bool isRedundant;	//if there is simplification in this node.
};

void ROBDD(ifstream&);
void Show(string, Node*, int);
void Redundant(Node*, int);
void Save(string, Node*, int);

int main(int argc, char *argv[])
{
	//Load pla file.
	if (argc == 2)
	{
		ifstream fin(argv[1]);
		if (!fin)
		{
			cout << "file error!" << endl;
			return -1;
		}
		else
		{
			ROBDD(fin);
		}
	}
	else
	{
		cout << "parameter error!" << endl;
		return -1;
	}

	return 0;
}

void ROBDD(ifstream& fin)
{
	string str;
	Node *n = NULL;
	int num_In = 0, num_Out = 0;
	char *ilb = NULL;
	char *ob = NULL;
	while (!fin.eof())
	{
		fin >> str;
		if (str == ".i")
		{
			fin >> num_In;	//How many input.
			n = new Node[(int)pow(2, num_In) + 1];	//How many node.
			//Initialization node.
			for (int i = 0; i < (int)pow(2, num_In) + 1; i++)
			{
				n[i].isRedundant = false;
			}
			ilb = new char[num_In];
			for (int i = 1; i < pow(2,num_In-1); i++)
			{
				n[i].Else_edge = i * 2;
				n[i].Then_edge = i * 2 + 1;
			}
			for (int i = (int)pow(2, num_In - 1); i < (int)pow(2, num_In); i++)
			{
				n[i].Else_edge = 0;
				n[i].Then_edge = 0;
			}
			n[0].Variable = '0';
			n[(int)pow(2, num_In)].Variable = '1';
		}
		else if (str == ".o")
		{
			//How many output, but I think it always one in the situation.
			fin >> num_Out;
			ob = new char[num_Out];
		}
		else if (str == ".ilb")
		{
			for (int i = 0; i < num_In; i++)
			{
				fin >> ilb[i];
			}
			//Input variables' name.
			for (int i = 1; i < pow(2, num_In); i++)
			{
				n[i].Variable = ilb[(int)log2(i)];
			}
		}
		else if (str == ".ob")
		{
			//Output variables' name.
			for (int i = 0; i < num_Out; i++)
			{
				fin >> ob[i];
			}
		}
		else if (str == ".p")
		{
			//The SOP formula.
			//In this part, we need to save all variable is 0, 1 or don't care.
			//And update all else_edge and then_edge.
			int temp;
			bool *bit = new bool[(int)pow(2,num_In)];
			
			fin >> temp;
			for (int i = 0; i < temp; i++)
			{
				string temp;
				fin >> temp;
				reverse(temp.begin(), temp.end());
				//Bits part.
				for (int i = 0; i < pow(2, num_In); i++)
				{
					bit[i] = false;
				}
				for (int j = 0; j < (int)temp.length(); j++)
				{
					if (temp[j] == '1')
						bit[j * 2+1] = true;
					else if (temp[j] == '0')
						bit[j * 2] = true;
					else
					{
						bit[j * 2] = true;
						bit[j * 2 + 1] = true;
					}
				}
				fin >> temp;
				//Node part.
				for (int i = (int)pow(2,num_In-1); i < (int)pow(2, num_In); i++)
				{
					bool f=true;
					int temp = i * 2 - (int)pow(2, num_In);
					for (int j = 0; j < num_In; j++)
					{
						if (bit[temp % 2 + j * 2] == false)
							f = false;
						temp /= 2;
					}
					if (n[i].Else_edge == 0 && f == true)
						n[i].Else_edge = (int)pow(2, num_In);

					f = true;
					temp = i * 2 + 1 - (int)pow(2, num_In);
					for (int j = 0; j < num_In; j++)
					{
						if (bit[temp % 2 + j * 2] == false)
							f = false;
						temp /= 2;
					}
					if (n[i].Then_edge == 0 && f == true)
						n[i].Then_edge = (int)pow(2, num_In);
				}
			}
		}
		else if (str == ".e")
		{
			//Input is end.
			//Show data of node and output dot file.
			Show("OBDD", n, (int)pow(2, num_In));
			Save("obdd.dot", n, (int)pow(2, num_In));
			//Check Redundant and Isomorphic node, and output dot file again.
			Redundant(n, (int)pow(2, num_In));
			Show("ROBDD",n, (int)pow(2, num_In));
			Save("robdd.dot", n, (int)pow(2, num_In));
			break;
		}
	}
}

void Show(string name,Node* n,int size)
{
	cout << "digraph " << name << " {" << endl;
	cout << "\t0 [label=\"0\", shape=box];" << endl;
	for (int i = 1; i < size; i++)
	{
		if(!n[i].isRedundant)
		printf("\t%d [label=\"%c\"];\n",i,n[i].Variable);
	}
	printf("\t%d [label=\"1\", shape=box];\n", size);
	cout << endl;
	for (int i = 1; i < size; i++)
	{
		if (!n[i].isRedundant)
		{
			printf("\t%d -> %d [label=\"0\", style=dotted]\n", i, n[i].Else_edge);
			printf("\t%d -> %d [label=\"1\", style=solid]\n", i, n[i].Then_edge);
		}
	}
	cout << "}" << endl;
}

void Redundant(Node* n, int size)
{
	bool f = false;
	for (int i = 1; i < size; i++)
	{
		if (n[i].isRedundant) continue;
		if (n[i].Else_edge == n[i].Then_edge)
		{
			n[i].isRedundant = true;
			f = true;
			for (int j = 1; j < size; j++)
			{
				if (n[j].Else_edge == i)
					n[j].Else_edge = n[i].Else_edge;
				if (n[j].Then_edge == i)
					n[j].Then_edge = n[i].Else_edge;
			}
		}
	}
	for (int i = 1; i < size; i++)
	{
		for (int j = i+1; j < size; j++)
		{
			if (n[i].isRedundant)continue;
			if (n[j].isRedundant)continue;

			if (n[i].Else_edge == n[j].Else_edge &&
				n[i].Then_edge == n[j].Then_edge &&
				n[i].Variable == n[j].Variable)
			{
				n[j].isRedundant = true;
				f = true;
				for (int k = 1; k < size; k++)
				{
					if (n[k].Else_edge == j)
						n[k].Else_edge = i;
					if (n[k].Then_edge == j)
						n[k].Then_edge = i;
				}
			}
		}
	}

	if (f)
		Redundant(n, size);
}

void Save(string fileName,Node *n, int size)
{
	ofstream fout(fileName, ofstream::out);
	fout << "digraph " << fileName << " {" << endl;
	fout << "\t0 [label=\"0\", shape=box];" << endl;
	for (int i = 1; i < size; i++)
	{
		if (!n[i].isRedundant)
			fout << "\t" << i << " [label=\"" << n[i].Variable << "\"];\n";
	}
	fout << "\t" << size << " [label=\"1\", shape=box];\n";
	fout << endl;
	for (int i = 1; i < size; i++)
	{
		if (!n[i].isRedundant)
		{
			fout << "\t" << i << " -> " << n[i].Else_edge << " [label=\"0\", style=dotted]\n";
			fout << "\t" << i << " -> " << n[i].Then_edge << " [label=\"1\", style=solid]\n";
		}
	}
	fout << "}";

	fout.close();
}