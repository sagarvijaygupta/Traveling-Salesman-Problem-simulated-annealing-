#include <bits/stdc++.h>
using namespace std;

const double INFL = 1e100;
double T_MAX = 1e10 - 1;
const double UPDATE_T = 0.999;

vector< vector<double> > adj;
vector< pair<double, double> > coords;
string datatype;
vector<int> odds;
vector< pair<int, int> > perfect_match;
vector< vector<int> > joint;
vector< pair<int, int> > euler_tour;
vector<int> path_chris, best_path, new_path, present_path;
double best_cost = 0;
double best_cost_christofides = 0;
int n;



void read_file() {
	// ifstream fin;
	// fin.open(file);
	cin >> datatype;
	cin >> n;
	adj.assign(n, vector<double>(n));
	coords.assign(n, make_pair(0.0, 0.0));
	for(int i = 0; i < n; ++i)
		cin >> coords[i].first >> coords[i].second;
	for(int i = 0; i < n; ++i) 
		for(int j = 0; j < n; ++j)
			cin >> adj[i][j];
}

void build_mst() {
	priority_queue< pair<double, int>, vector< pair<double, int> >, greater< pair<double, int> > > pq;
	vector<bool> visited(n, false);
	vector<int> parent(n, -1);
	vector<double> dist(n, INFL);
	pq.push(make_pair(0, 0));
	vector<int> count(n, 0);
	dist[0] = 0;
	joint.assign(n, vector<int>());
	while(!pq.empty()) {
		int u = pq.top().second;
		visited[u] = true;
		pq.pop();
		for(int i = 0; i < n; ++i) {
			if(!visited[i]) {
				if(adj[u][i] < dist[i]) {
					dist[i] = adj[u][i];
					pq.push(make_pair(dist[i], i));
					parent[i] = u;
				}
			}
		}
	}
	for(int i = 0; i < n; ++i) {
		if(parent[i] == -1)
			continue;
		joint[i].push_back(parent[i]);
		joint[parent[i]].push_back(i);
	}
	for(int i = 0; i < n; ++i) {
		if(parent[i] == -1)
			continue;
		count[i]++;
		count[parent[i]]++;
	}
	for(int i = 0; i < n; ++i)
		if(count[i] % 2)
			odds.push_back(i);
}

void perfect_matching() {
	vector< pair<double, pair<int, int> > > match;
	vector<bool> visited(n, true);
	for(int i = 0; i < odds.size(); ++i) {
		for(int j = i + 1; j < odds.size(); ++j) {
			int u = odds[i];
			int v = odds[j];
			visited[u] = visited[v] = false;
			match.push_back(make_pair(adj[u][v], make_pair(u, v)));
		}
	}
	sort(match.begin(), match.end());
	for(int i = 0; i < match.size(); ++i) {
		int u = match[i].second.first;
		int v = match[i].second.second;
		if(!visited[u] && !visited[v]) {
			perfect_match.push_back(make_pair(u, v));
			visited[u] = visited[v] = true;
		}
	}
}

void unite() {
	for(int i = 0; i < perfect_match.size(); ++i) {
		int u = perfect_match[i].first;
		int v = perfect_match[i].second;
		joint[u].push_back(v);
		joint[v].push_back(u);
	}
}

void dfs(int u, vector<bool>& vis_dfs, int& count) {
	vis_dfs[u] = 1;
	count++;
	for(auto v : joint[u]) {
		if(v != -1 && !vis_dfs[v]) {
			dfs(v, vis_dfs, count);
		}
	}
}

bool check_bridge(int u, int v) {
	int ct = 0;
	for(int i = 0; i < joint[u].size(); ++i)
		if(joint[u][i] != -1)
			ct++;
	if(ct == 1)
		return false;
	int u_ct = 0;
	vector<bool> vis_dfs(n, false);
	dfs(u, vis_dfs, u_ct);
	for(int i = 0; i < joint[u].size(); ++i)
		if(joint[u][i] == v) {
			joint[u][i] = -1;
			break;
		}
	for(int i = 0; i < joint[v].size(); ++i)
		if(joint[v][i] == u) {
			joint[v][i] = -1;
			break;
		} 
	for(int i = 0; i < n; ++i)
		vis_dfs[i] = false;
	int v_ct = 0;
	dfs(v, vis_dfs, v_ct);
	joint[u].push_back(v);
	joint[v].push_back(u);
	return (u_ct != v_ct);
}

void euler_path(int i) {
	// cout<<i<<endl;
	for(int j = 0; j < joint[i].size(); ++j) {
		int v = joint[i][j];
		if(joint[i][j] != -1 && !check_bridge(i, joint[i][j])) {
			euler_tour.push_back(make_pair(i, v));
			for(int k = 0; k < joint[i].size(); ++k)
				if(joint[i][k] == v) {
					joint[i][k] = -1;
					break;
				}
			for(int k = 0; k < joint[v].size(); ++k)
				if(joint[v][k] == i) {
					joint[v][k] = -1;
					break;
				}
			euler_path(v);
		}
	}
}
void remove_duplicate() {
	vector<bool> vis(n,false);
	for(int i = 0; i < euler_tour.size(); ++i) {
		int v = euler_tour[i].first;
		if(!vis[v]) {
			vis[v] = true;
			path_chris.push_back(v);
		}
	}
	best_path = path_chris;
}

void twoopt() {
	int l = rand() % n;
	int r = rand() % n;
	if(l > r)
		swap(l, r);
	reverse(new_path.begin() + l, new_path.begin() + r);
}

double E(vector<int> &v) {
	double _cost = 0;
	for(int i = 0; i < v.size()-1; ++i) {
		_cost += adj[v[i]][v[i+1]];
	}
	_cost += adj[v[v.size()-1]][v[0]];	
	return _cost;
}
void print_best() {

	for(int i = 0; i < n; i++) {
		cout << best_path[i] << " ";		
	}
	cout << endl;
	// cout << best_cost << endl;
}
void threeopt() {
	set<int> s;
	while(s.size() < 3)
		s.insert(rand() % n);

	vector<int> l1, l2, l3;
	int a[3], i=0;
	for(auto it:s){
		a[i++] = it;
	}
	for(int i = a[0]; i<a[1]; i++)
		l1.push_back(new_path[i]);
	for(int i = a[1]; i<a[2]; i++)
		l2.push_back(new_path[i]);
	for(int i = a[2]; i<n; i++)
		l3.push_back(new_path[i]);
	for(int i = 0; i < a[0]; i++)
		l3.push_back(new_path[i]);

	double cv1 = adj[l1[l1.size() - 1]][l2[l2.size() - 1]] + adj[l2[0]][l3[l3.size() - 1]] + adj[l3[0]][l1[0]];
	double cv2 = adj[l1[l1.size() - 1]][l3[l3.size() - 1]] + adj[l3[0]][l2[0]] + adj[l2[l2.size() - 1]][l1[0]];
	double cv3 = adj[l1[l1.size() - 1]][l2[0]] + adj[l3[l3.size() - 1]][l2[0]] + adj[l2[l2.size() - 1]][l1[0]];
	double cv4 = adj[l1[l1.size() - 1]][l3[0]] + adj[l3[l3.size() - 1]][l2[l2.size() - 1]] + adj[l2[0]][l1[0]];
	double minC = min(cv1, min(cv2, min(cv3, cv4)));
	vector<int> v;
	if(cv1 == minC){
		v.insert(v.end(), l1.begin(), l1.end());
		v.insert(v.end(), l2.rbegin(), l2.rend());
		v.insert(v.end(), l3.rbegin(), l3.rend());
	}
	else
	if(cv2 == minC){
		v.insert(v.end(), l1.begin(), l1.end());		
		v.insert(v.end(), l3.rbegin(), l3.rend());
		v.insert(v.end(), l2.begin(), l2.end());
	}
	else
	if(cv3 == minC){
		v.insert(v.end(), l1.begin(), l1.end());
		v.insert(v.end(), l3.begin(), l3.end());
		v.insert(v.end(), l2.begin(), l2.end());
	}
	else{
		v.insert(v.end(), l1.begin(), l1.end());
		v.insert(v.end(), l3.begin(), l3.end());
		v.insert(v.end(), l2.rbegin(), l2.rend());
	}
	new_path = v;
}
void simulated_annealing() {
	double T = T_MAX;
	present_path = best_path;
	int not_changing = 0, multiplier = 1;
 	srand(time(NULL));
	while(true) {
		int cycles = 100;
		while(cycles--) {
			new_path = present_path;
				twoopt();
			if(not_changing == 100000)
				threeopt();
			double new_path_cost = E(new_path);
			double present_path_cost = E(present_path);
			double del_E = present_path_cost - new_path_cost;
			double prob = rand() / (double) RAND_MAX;
			double sigmoid_prob = 1/(1 + exp(-1*del_E/T));
			if(del_E > 0) {
				present_path = new_path;
			}
			else
			if(sigmoid_prob > prob) {
				present_path = new_path;
			}

			if(best_cost > new_path_cost) {
				best_path = new_path;
				best_cost = new_path_cost;
				if(best_cost <= best_cost_christofides)
					print_best();
				not_changing = 0;
			}
			else
				not_changing++;
		}
		T = T * UPDATE_T;
		if(not_changing > 100000) {
			T = (double)(n * multiplier * 1LL);
			multiplier = multiplier << 1;
			if(T > T_MAX) {
				T = T_MAX;
				multiplier = 1;
			}
			if(multiplier == 0)
				multiplier = 1;
			not_changing = 0;
		}
	}
}

int main(int argc, char const *argv[]) {
	// if(argc != 2) {
	// 	cout << "Invalid params" << endl;
	// 	return 0;
	// }
	read_file();
	build_mst();
	perfect_matching();
	unite();
	euler_path(0);
	remove_duplicate();
	print_best();
	best_cost_christofides = E(best_path);
	best_path.assign(n,0);
	for(int i=0; i<n; i++)
		best_path[i] = i;
	best_cost = E(best_path);
	// print_best();
	simulated_annealing();
	// cout << best_cost << endl;
}