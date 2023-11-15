#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <ctime>

using namespace std;

class myThread {
public:
	int needTime;
	int numOfBills;
	string stringToOut;
	myThread(string name, int needTime_, int numOfThread_) {
		stringToOut = name + " | ";
		needTime = needTime_;
		numOfBills = numOfThread_;
	}
};

class myProcess {
public:
	vector<myThread> threads;
	string stringToOut;
	myProcess(string name, int n) {
		stringToOut = name + "---";
		for (int i = 0; i < n; i++) {
			threads.push_back(myThread("    thread" + to_string(i), rand() % 10 + 1, rand() % 10000 + 1));
		}
	}
};

class myPlanizer {
public:
	vector<myProcess> processes;
	vector<pair<int, int>> bills;
	myPlanizer(int n) {
		for (int i = 0; i < n; i++) {
			processes.push_back(myProcess("  process" + to_string(i) + ":", rand() % 4 + 1));
		}
		for (int ind1 = 0; ind1 < processes.size(); ind1++) {
			for (int ind2 = 0; ind2 < processes[ind1].threads.size(); ind2++) {
				for (int i = 0; i < processes[ind1].threads[ind2].numOfBills; i++) {
					bills.push_back({ ind1, ind2 });
				}
			}
		}
	}
	bool tick() {
		if (bills.size() == 0)
			return false;
		pair<int, int> rndBill = bills[rand() % bills.size()];
		for (int ind1 = 0; ind1 < processes.size(); ind1++) {
			processes[ind1].stringToOut += "-";
			for (int ind2 = 0; ind2 < processes[ind1].threads.size(); ind2++) {
				if (ind1 == rndBill.first && ind2 == rndBill.second) {
					processes[ind1].threads[ind2].needTime--;
					processes[ind1].threads[ind2].stringToOut += "=";
					if (processes[ind1].threads[ind2].needTime <= 0) {
						vector<pair<int, int>> newBills;
						for (auto it : bills)
							if (it.first != ind1 || it.second != ind2)
								newBills.push_back(it);
						bills = newBills;
					}
				}
				else {
					processes[ind1].threads[ind2].stringToOut += " ";
				}
			}
		}
		return true;
	}
};

int main() {

	srand(time(0));

	myPlanizer planizer(rand() % 6 + 1);

	while (planizer.tick());

	for (auto it : planizer.processes) {
		cout << it.stringToOut << "\n";
		for (auto it2 : it.threads)
			cout << it2.stringToOut << "\n";
	}

}