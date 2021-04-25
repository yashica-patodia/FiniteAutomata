/*Name :Yashica Patodia
Roll No:19CS10067*/

#include<iostream>
#pragma GCC optimize("Ofast")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#define endl '\n'
using namespace std;
#define B_LEN 32

class N
{
public:
	int n;
	int m;
	int noOfStartStates;
	int noOfFinalStates;
	uint32_t startState;
	uint32_t finalState;
	uint32_t **transitions;
	N(int n_, int m_)
	{
		n = n_;
		m = m_;
		startState = 0;
		finalState = 0;
		noOfFinalStates = 0;
		noOfStartStates = 0;
		transitions = new uint32_t*[n_];
		for (int i = 0; i < n; i++)
			transitions[i] = new uint32_t[m_];

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
				transitions[i][j] = 0;
		}


	}


	~N() {}
	int getn()
	{
		return n;
	}
	int getm()
	{
		return m;
	}

};
class D
{
public:
	uint32_t n;
	uint32_t m;
	uint32_t startState;
	uint32_t *finalState;
	int noOfFinalStates;
	uint32_t **transitions;
	D(uint32_t n_, uint32_t m_)
	{
		n = n_;
		m = m_;
		startState = 0;
		uint32_t p = (n + 31) / 32;
		finalState = new uint32_t[p + 1];
		transitions = new uint32_t*[n_];
		for (uint32_t i = 0; i < n; i++)
			transitions[i] = new uint32_t[m_];

		for (uint32_t i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
				transitions[i][j] = 0;
			if (i <= p)
				finalState[i] = 0;
		}


	}
	~D() {}
	int getn()
	{
		return n;
	}
	int getm()
	{
		return m;
	}



};

struct  DSU
{
	int conn;
	int *par;
	int *sz;
	int n;
	DSU() {}
	DSU(int n_)
	{
		n = n_;
		par = new int [n_ + 1];
		sz = new int [n_ + 1];
		for (int i = 0; i < n; i++)
		{
			par[i] = i;
			sz[i] = 1;
		}
		conn = n;
	}
	int find(int k)
	{
		while (k != par[k])
		{
			par[k] = par[par[k]];
			k = par[k];
		}
		return k;
	}

	void unite(int u, int v)
	{
		int p1 = find(u);
		int p2 = find(v);
		if (p1 == p2)
			return;
		conn--;
		if (sz[p1] >= sz[p2])
		{
			sz[p1] += sz[p2];
			par[p2] = p1;
		}
		else
		{
			sz[p2] += sz[p1];
			par[p1] = p2;
		}
	}
};
int countOnes(uint32_t val)
{
	int cnt = 0;
	for (int i = 0; i < B_LEN; i++)
	{
		int x = (val & (1U << i));
		if (x)
			cnt++;
	}
	return cnt;

}
N* readNFA(const char* fileName)
{
	freopen(fileName, "r", stdin);

	int n_, m_;
	cin >> n_ >> m_;
	N *obj;
	obj = new N(n_, m_);
	uint32_t start = 0;
	int sc = 0;
	while (1)
	{

		int x;
		cin >> x;
		if (x == -1)
			break;
		start |= (1U << x);


		sc++;
	}
	obj->startState = start;
	obj->noOfStartStates = sc;
	sc = 0;
	uint32_t final = 0;
	while (1)
	{
		int x;
		cin >> x;
		if (x == -1)
			break;
		final |= (1U << x);

		sc++;
	}
	obj->finalState = final;
	obj->noOfFinalStates = sc;
	while (1)
	{
		int x;
		cin >> x;
		if (x == -1)
			break;
		int y, z;
		cin >> y >> z;
		//cout << x << " " << y << " " << z << endl;
		(obj->transitions[x][y]) |= (1U << z);


	}
	return obj;
}
void printNFA(N *obj)
{
	cout << "\tNumber of states: " << obj->n << endl;
	cout << "\tInput alphabet:  {";
	for (int i = 0; i < obj->m - 1; i++)
		cout << i << ",";
	cout << obj->m - 1 << "}" << endl;
	int cnt = 0;
	int s1 = obj->noOfStartStates;
	int s2 = obj->noOfFinalStates;
	//cout << s1 << " " << s2 << endl;
	if (s1 > 1)
		cout << "\tStart states: {";
	else
		cout << "\tStart state: {";

	for (int i = 0; i < 32; i++)
	{
		int x = (obj->startState) & (1U << i);
		if (x)
		{
			cout << i ;
			cnt++;
			if (cnt < s1)
				cout << ",";
			else
			{
				cout << "}" << endl;
				break;
			}

		}
	}
	cnt = 0;
	if (s2 > 1)
		cout << "\tFinal states: {";
	else
		cout << "\tFinal state: {";
	for (int i = 0; i < 32; i++)
	{
		int x = (obj->finalState) & (1U << i);
		if (x)
		{
			cout << i;
			cnt++;
			if (cnt < s2)
				cout << ",";
			else
			{
				cout << "}" << endl;
				break;
			}

		}
	}
	cout << "\tTransition function" << endl;
	for (int i = 0; i < obj->n; i++)
	{
		for (int j = 0; j < obj->m; j++)
		{
			bool flag = false;
			int cnt1 = countOnes(obj->transitions[i][j]);
			if (cnt1 > 0)
			{
				int f = 0;
				cout << "\t\tDelta(" << i << "," << j << ") = {";

				for (int k = 0; k < 32; k++)
				{
					int x = (obj->transitions[i][j]) & (1U << k);
					if (x)
					{
						f++;
						if (f < cnt1)
							cout <<  k << ",";
						else
						{
							cout <<  k << "}" << endl;
							break;
						}
					}
				}
			}
			else
			{
				cout << "\t\tDelta(" << i << "," << j << ") = { }" << endl;

			}
		}
	}
	cout << endl;
	cout << endl;


}
void printDFA(D *obj)
{

	cout << "\tNumber of states: " << obj->n << endl;
	cout << "\tInput alphabet:  {";
	for (int i = 0; i < obj->m - 1; i++)
		cout << i << ",";
	cout << obj->m - 1 << "}" << endl;
	cout << "\tStart state: " << obj->startState << endl;
	if (obj->n > 64)
	{
		if (obj->noOfFinalStates > 1 )
			cout << "\t" << obj->noOfFinalStates << " final states" << endl;
		else
			cout << "\t" << obj->noOfFinalStates << " final state" << endl;
		cout << "\tTransition function: Skipped" << endl;
		cout << endl;
		cout << endl;

	}




	else
	{
		cout << "\tFinal states:  {";
		int p = (obj->n + 31) / 32;
		int s1 = 0;
		for (uint32_t i = 0; i <= p; i++)
		{
			for (int j = 0; j < 32; j++)
			{
				uint32_t x = (obj->finalState[i]) & (1U << j);
				uint32_t state = 32 * i + j;
				if (x)
				{
					s1++;
					if (s1 < obj->noOfFinalStates)
						cout << state << ",";
					else
					{
						cout << state << "}" << endl;
						break;
					}
				}
			}
		}
		cout << "\tTransition function" << endl;
		cout << endl;
		cout << "\ta\\p|\t";
		for (int i = 0; i < obj->n; i++)
			cout  << i << "\t";
		cout << endl;
		cout << "\t---+";
		for (int i = 0; i < obj->n; i++)
			cout << "----";
		cout << endl;
		for (int i = 0; i < obj->m; i++)
		{
			cout << "\t " << i << " |\t";
			for (int j = 0; j < obj->n; j++)
			{
				cout << obj->transitions[j][i] << " \t";
			}
			cout << endl;
		}

		cout << endl;
		cout << endl;



	}
}
D* subsetcons(N * nfa)
{

	uint32_t n_nfa = nfa->n;
	uint32_t m_nfa = nfa->m;
	uint32_t n_ = (1U << n_nfa);
	uint32_t m_ = m_nfa;
	D *dfa;
	dfa = new D(n_, m_);
	uint32_t cnt = 0;
	for (uint32_t i = 0; i < n_; i++)
	{
		for (uint32_t j = 0; j < m_; j++)
		{
			for (int k = 0; k < 32; k++)
			{
				uint32_t p = (1U << k);
				if (p > i)
					break;
				uint32_t x = i & p;
				if (x)
				{
					dfa->transitions[i][j] |= nfa->transitions[k][j];
				}
			}

		}
		uint32_t mark = 0;

		bool flag = false;
		for (int k = 0; k < 32; k++)
		{
			uint32_t p = (1U << k);
			if (p > i)
				break;

			uint32_t x = i & p;
			uint32_t xx = (nfa->finalState) & (1U << k);
			// if (i == (1U << 11))
			// 	cout << x << " " << xx << "l" << endl;
			if (x && xx)
			{

				//cout << k << " ";
				mark = i;
				flag = true;
				break;

			}


		}
		if (flag)
		{
			cnt++;
			//cout << i << " " << cnt << " " << "l" << endl;
			uint32_t x = (i) / 32;
			uint32_t pos = i - 32 * x;
			dfa->finalState[x] |= (1U << pos);
		}


	}
	dfa->startState = nfa->startState;
	dfa->noOfFinalStates = cnt;
	return dfa;





}
void dfs(bool * vis, D * dfa, uint32_t state)
{
	vis[state] = true;
	for (int i = 0; i < dfa->m; i++)
	{
		uint32_t nxt = dfa->transitions[state][i];
		if (vis[nxt])
			continue;
		dfs(vis, dfa, nxt);


	}
}
uint32_t *findreachable(D * dfa)
{
	uint32_t *reachable;

	bool *vis;
	uint32_t p = (dfa->n + 31) / 32;
	reachable = new uint32_t[p + 1];
	//level = new long long int[dfa->n + 1];
	vis = new bool[dfa->n + 1];
	for (uint32_t i = 0; i <= min(p, dfa->n - 1); i++)
	{
		reachable[i] = 0;
		//level[i] = 1e15;
		vis[i] = 0;

	}
	for (uint32_t i = min(p, dfa->n - 1); i < dfa->n; i++)
	{

		vis[i] = 0;
	}
	dfs(vis, dfa, dfa->startState);
	cout << "{";
	uint32_t cnt = 0;
	for (uint32_t i = 0; i < dfa->n; i++)
	{
		if (vis[i])
		{
			cnt++;
			uint32_t x = i / 32;
			uint32_t pos = i - 32 * x;
			reachable[x] |= (1U << pos);
		}

	}
	uint32_t s1 = 0;
	for (uint32_t i = 0; i < dfa->n; i++)
	{
		if (vis[i])
		{
			s1++;
			if (s1 < cnt)
				cout << i << ",";
			else
				cout << i << "}" << endl;
		}
	}
	cout << endl;
	cout << endl;

	return reachable;


}
D *rmunreachable(D * dfa, uint32_t *reachable)
{
	uint32_t p = (dfa->n + 31) / 32;
	uint32_t cnt = 0;
	uint32_t *hashMap;
	hashMap = new uint32_t[dfa->n + 1];
	uint32_t s1 = 0;
	for (uint32_t i = 0; i < p; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			uint32_t state = 32 * i + j;
			uint32_t x = (reachable[i] & (1U << j));
			uint32_t xx = (dfa->finalState[i] & (1U << j));
			if (x)
			{



				hashMap[state] = cnt;
				cnt++;


			}
			else
				hashMap[state] = -1;
			if (x && xx)
			{
				s1++;
			}
		}
	}
	D *rdfa;
	rdfa = new D(cnt, dfa->m);
	cnt = 0;
	rdfa->startState = hashMap[dfa->startState];
	rdfa->noOfFinalStates = s1;
	for (uint32_t i = 0; i < dfa->n; i++)
	{
		if (hashMap[i] == -1)
			continue;
		for (int j = 0; j < dfa->m; j++)
		{
			//if it is reachable then

			uint32_t nxt = (dfa->transitions[i][j]);
			if (hashMap[nxt] == -1)
				continue;
			rdfa->transitions[hashMap[i]][j] = hashMap[nxt];






		}
		uint32_t x = (i) / 32;
		uint32_t pos = i - 32 * x;
		uint32_t ch = (dfa->finalState[x]) & (1U << pos);

		if (ch)
		{
			uint32_t hashX = hashMap[i] / 32;
			uint32_t hashPos = hashMap[i] - 32 * hashX;
			rdfa->finalState[hashX] |= (1U << hashPos);
		}
	}
	delete [] dfa->finalState;
	for (int i = 0; i < dfa->n; i++)
	{
		delete[] dfa->transitions[i];
	}
	delete[]dfa->transitions;
	return rdfa;


}
int **findequiv(D * dfa)
{

	int **M;
	M = new int*[dfa->n];
	for (int i = 0; i < dfa->n; i++)
	{
		M[i] = new int[dfa->n];
	}
	for (int i = 0; i < dfa->n; i++)
	{
		for (int j = 0; j < i; j++)
			M[i][j] = 0;
		for (int j = i; j < dfa->n; j++)
			M[i][j] = -1;
	}

	//Initialization
	for (int i = 0; i < dfa->n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			int x = i / 32;
			int pos = i - 32 * x;
			int ch = (dfa->finalState[x]) & (1U << pos);
			int x2 = j / 32;
			int pos2 = j - 32 * x2;
			int ch2 = (dfa->finalState[x2]) & (1U << pos2);

			if ((ch && !ch2) || (!ch && ch2))
			{
				//cout << i << " " << j << "k" << endl;
				M[i][j] = 1;
				//M[j][i] = true;
			}

		}
	}
	// for (int i = 0; i < dfa->n; i++)
	// {
	// 	for (int j = 0; j < i; j++)
	// 		cout << M[i][j] << " ";
	// 	cout << endl;
	// }
	while (1)
	{
		bool flag = false;
		for (int i = 0; i < dfa->n; i++)
		{
			for (int j = 0; j < i; j++)
			{
				if (M[i][j] == 1)
					continue;
				for (int k = 0; k < dfa->m; k++)
				{
					int del1 = dfa->transitions[i][k];
					int del2 = dfa->transitions[j][k];
					if (M[del1][del2] == 1 || M[del2][del1] == 1)
					{
						M[i][j] = 1;
						flag = true;
						break;
					}
				}
			}
		}
		if (!flag)
			break;

	}
	// for (int i = 0; i < dfa->n; i++)
	// {
	// 	cout << i << " ";
	// 	for (int j = 0; j < i; j++)
	// 		cout << M[i][j] << " ";
	// 	cout << endl;
	// }
	return M;


}
D *collapse(int **M, D * dfa)
{
	DSU uf(dfa->n);
	for (int i = 0; i < dfa->n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (!M[i][j])
			{
				//cout << i << " " << j << " yp" << endl;
				uf.unite(i, j);
			}

		}
	}
	int cnt = 0;
	int cnt2 = 0;
	// int grp = 0;
	// for (int i = 0; i < dfa->n; i++)
	// {
	// 	if (uf.find(i) == i)

	// }
	bool *vis;
	vis = new bool[dfa->n + 1];
	int *hashMap;
	hashMap = new int[dfa->n];

	for (int i = 0; i < dfa->n; i++)
	{
		vis[i] = false;
	}
	cout << "+++ Equivalent states" << endl;
	for (int i = 0; i < dfa->n; i++)
	{
		if (vis[i])
			continue;
		else
		{
			int grp_id = uf.find(i);
			cnt2 = 0;
			cout << "\tGroup " << cnt << ": {";

			for (int j = 0; j < dfa->n; j++)
			{
				if (grp_id == uf.find(j))
				{
					vis[j] = true;
					hashMap[j] = cnt;
					cnt2++;
					if (cnt2 < uf.sz[grp_id])
						cout << j << ",";
					else
					{

						cout << j << "}" << endl;
						break;
					}

				}
			}
			cnt++;

		}
	}

	cout << endl;
	cout << endl;
	D *rdfa;
	rdfa = new D(cnt, dfa->m);
	rdfa->startState = hashMap[dfa->startState];
	uint32_t p = (dfa->n + 31) / 32;
	uint32_t s1 = 0;
	for (int i = 0; i < dfa->n; i++)
		vis[i] = false;
	for (uint32_t i = 0; i <= p; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			uint32_t state = 32 * i + j;
			uint32_t xx = (dfa->finalState[i] & (1U << j));
			if (xx)
			{
				if (vis[state])
					continue;

				s1++;
				uint32_t hashX = hashMap[state] / 32;
				uint32_t hashPos = hashMap[state] - 32 * hashX;
				rdfa->finalState[hashX] |= (1U << hashPos);
				for (int l = 0; l < dfa->n; l++)
				{
					if (uf.find(state) == uf.find(l))
					{
						vis[l] = true;
					}
				}
			}
		}
	}
	rdfa->noOfFinalStates = s1;
	for (int i = 0; i < dfa->n; i++)
	{
		for (int j = 0; j < dfa->m; j++)
		{
			rdfa->transitions[hashMap[i]][j] = hashMap[dfa->transitions[i][j]];
		}
	}



	return rdfa;




}
int main()
{
	const char *ch;

	//PART 1
	string fileName ;
	cout << "Enter the input filename in .txt format" << endl;
	cin >> fileName;
	//string fileName = "input.txt";
	ch = fileName.c_str();

	freopen("output.txt", "w", stdout);
	cout << "+++ Input NFA" << endl;
	N *obj = readNFA(ch);
	printNFA(obj);
	//PART 2
	D *obj2 = subsetcons(obj);
	cout << "+++ Converted DFA" << endl;
	printDFA(obj2);
	//PART 3
	cout << "+++ Reachable states:  " << endl;
	uint32_t *reachable;
	uint32_t p = (obj2->n + 31) / 32;
	reachable = new uint32_t[p + 1];
	reachable = findreachable(obj2);
	D *obj3 = rmunreachable(obj2, reachable);
	cout << "+++ Reduced DFA after removing unreachable states" << endl;
	printDFA(obj3);
	int **M;
	M = new int*[obj3->n];
	for (int i = 0; i < obj3->n; i++)
	{
		M[i] = new int[obj3->n];
	}
	M = findequiv(obj3);
	//PART 4
	D *obj4 = collapse(M, obj3);
	cout << "+++ Reduced DFA after collapsing equivalent states" << endl;
	printDFA(obj4);




}
