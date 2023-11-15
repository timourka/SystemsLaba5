/*
#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <set>
#include <ctime>

using namespace std;

class Driver {
private:
	int timeRest = 5;
public:
	int idProcess = -1;
	int idProcessFinish = -1;
	bool isUsing() {
		return idProcess != -1;
	}
	bool addProcess(int id, int needTime) {
		if (isUsing())
			return false;
		idProcess = id;
		timeRest = needTime;
		return true;
	}
	bool tick() {
		if (!isUsing())
			return false;
		if (--timeRest == 0) {
			idProcessFinish = idProcess;
			idProcess = -1;
		}
		return true;
	}
};

class Process {
private:
public:
	string stringToOut;
	int needTime = 100;
	int numOfBills = 10;
	int tickToRead = 4;
	int needTimeToRead = 5;
	bool isStop = false;
	Process(string name, int needTime, int numOfBills, int tickToRead, int needTimeToRead): stringToOut(name + " "), needTime(needTime), numOfBills(numOfBills), tickToRead(tickToRead), needTimeToRead(needTimeToRead){ }
	bool tick() {
		if (needTime == tickToRead) 
			isStop = true;
		needTime--;
		return true;
	}
};

class Kernel_1 {
	Driver driver;
	vector<int> bills;
	queue<int> queue;
public:
	vector<Process> processes;
	Kernel_1(vector<Process> processes) {
		this->processes = processes;
		for (int ind1 = 0; ind1 < processes.size(); ind1++) {
			for (int i = 0; i < processes[ind1].numOfBills; i++) {
				bills.push_back(ind1);
			}
		}
	}
	bool tick() {
		if (bills.size() == 0)
			return false;
		if (driver.isUsing()) {
			for (int ind1 = 0; ind1 < processes.size(); ind1++) {
				if (ind1 == driver.idProcess)
					processes[ind1].stringToOut += "R";
				else if (processes[ind1].isStop)
					processes[ind1].stringToOut += "W";
				else
					processes[ind1].stringToOut += " ";
			}
			driver.tick();
			if (driver.idProcessFinish != -1) {
				processes[driver.idProcessFinish].isStop = false;
				driver.idProcessFinish = -1;
			}
			return true;
		}
		else if (!queue.empty()) {
			driver.addProcess(queue.front(), processes[queue.front()].needTimeToRead);
			queue.pop();
			return true;
		}
		int rndBill = bills[rand() % bills.size()];
		for (int ind1 = 0; ind1 < processes.size(); ind1++) {
			if (ind1 == rndBill) {
				processes[ind1].tick();
				processes[ind1].stringToOut += "=";
				if (processes[ind1].needTime < 0) {
					vector<int> newBills;
					for (auto it : bills)
						if (it != ind1)
							newBills.push_back(it);
					bills = newBills;
				}
				else if (processes[ind1].isStop) {
					queue.push(ind1);
				}
			}
			else {
				if (ind1 == driver.idProcess)
					processes[ind1].stringToOut += "R";
				else if (processes[ind1].isStop)
					processes[ind1].stringToOut += "W";
				else
					processes[ind1].stringToOut += " ";
			}
		}
		return true;
	}
};

class Kernel_2 {
	Driver driver;
	vector<int> bills;
	queue<int> queue;
public:
	vector<Process> processes;
	Kernel_2(vector<Process> processes) {
		this->processes = processes;
		for (int ind1 = 0; ind1 < processes.size(); ind1++) {
			for (int i = 0; i < processes[ind1].numOfBills; i++) {
				bills.push_back(ind1);
			}
		}
	}
	bool tick() {
		if (bills.size() == 0)
			return false;
		if (driver.isUsing()) {
			driver.tick();
			if (driver.idProcessFinish != -1) {
				processes[driver.idProcessFinish].isStop = false;
				driver.idProcessFinish = -1;
			}
		}
		else if (!queue.empty()) {
			driver.addProcess(queue.front(), processes[queue.front()].needTimeToRead);
			queue.pop();
			for (int ind1 = 0; ind1 < processes.size(); ind1++) {
				if (ind1 == driver.idProcess)
					processes[ind1].stringToOut += "R";
				else if (processes[ind1].isStop)
					processes[ind1].stringToOut += "W";
				else
					processes[ind1].stringToOut += " ";
			}
			return true;
		}
		int rndBill = -2;
		for (int ind1 = 0; ind1 < processes.size(); ind1++) {
			if (!processes[ind1].isStop) {
				rndBill = -1;
				break;
			}
		}
		if (rndBill == -2) {
			for (int ind1 = 0; ind1 < processes.size(); ind1++) {
				if (ind1 == driver.idProcess)
					processes[ind1].stringToOut += "R";
				else if (processes[ind1].isStop)
					processes[ind1].stringToOut += "W";
				else
					processes[ind1].stringToOut += " ";
			}
			return true;
		}
		rndBill = bills[rand() % bills.size()];
		while (processes[rndBill].isStop)
			rndBill = ++rndBill % processes.size();
		for (int ind1 = 0; ind1 < processes.size(); ind1++) {
			if (ind1 == rndBill) {
				processes[ind1].tick();
				processes[ind1].stringToOut += "=";
				if (processes[ind1].needTime < 0) {
					processes[ind1].isStop = false;
					vector<int> newBills;
					for (auto it : bills)
						if (it != ind1)
							newBills.push_back(it);
					bills = newBills;
				}
				else if (processes[ind1].isStop) {
					queue.push(ind1);
				}
			}
			else {
				if (ind1 == driver.idProcess)
					processes[ind1].stringToOut += "R";
				else if (processes[ind1].isStop)
					processes[ind1].stringToOut += "W";
				else
					processes[ind1].stringToOut += " ";
			}
		}
		return true;
	}
};

int main() {

	srand(time(0));

	int n = 4;
	vector<Process> processes;
	for (int i = 0; i < n; i++) {
		int t = rand() % 20 + 1;
		int nb = rand();
		int tt = rand() % t + 1;
		int tr = rand() % 15 + 1;
		string s = "process" + to_string(i) + " needTime:" + to_string(t) + " numOfBills:" + to_string(nb) + " tickToRead:" + to_string(tt) + " needTimeToRead:" + to_string(tr);
		while (s.size() < 70)
			s += "-";
		//Process
		// string name, int needTime, int numOfBills, int tickToRead, int needTimeToRead
		processes.push_back({ s, t, nb, tt, tr});

		//int t = 4;
		//processes.push_back({ "process" + to_string(i), t, 10, 1 });
	}

	Kernel_1 kernel1(processes);

	int k = 0;

	while (kernel1.tick()) 
		k++;

	cout << "kernel_1: " << k << "\n";
	for (auto it : kernel1.processes) {
		cout << it.stringToOut << "\n";
	}
	cout << "\n";
	Kernel_2 kernel2(processes);
	
	k = 0;

	while (kernel2.tick())
		k++;

	cout << "kernel_2: " << k << "\n";
	for (auto it : kernel2.processes) {
		cout << it.stringToOut << "\n";
	}
	int t = 0;
}
*/

#include "¬е_крест_крест.h"

внедрить хутор –усь;

династи€ ƒрайвер {
царский:
	целина timeRest = 5;
народный:
	целина idпроцесс = -1;
	целина idпроцессFinish = -1;
	розсуд isUsing() {
		воздать idпроцесс != -1;
	}
	розсуд addпроцесс(целина id, целина нужно¬рем) {
		коли (isUsing())
			воздать кривда;
		idпроцесс = id;
		timeRest = нужно¬рем;
		воздать правда;
	}
	розсуд тик() {
		коли (!isUsing())
			воздать кривда;
		коли (--timeRest == 0) {
			idпроцессFinish = idпроцесс;
			idпроцесс = -1;
		}
		воздать правда;
	}
};

династи€ процесс {
царский:
народный:
	много_букав много_букавToOut;
	целина нужно¬рем = 100;
	целина колЅил = 10;
	целина тикƒл€„тен = 4;
	целина врем€Ќа„тен = 5;
	розсуд isStop = кривда;
	процесс(много_букав name, целина нужно¬рем, целина колЅил, целина тикƒл€„тен, целина врем€Ќа„тен): много_букавToOut(name + " "), нужно¬рем(нужно¬рем), колЅил(колЅил), тикƒл€„тен(тикƒл€„тен), врем€Ќа„тен(врем€Ќа„тен){ }
	розсуд тик() {
		коли (нужно¬рем == тикƒл€„тен)
			isStop = правда;
		нужно¬рем--;
		воздать правда;
	}
};

династи€ ядро_1 {
	ƒрайвер ƒрайвер;
	наборъ<целина> билеты;
	пор€док<целина> пор€док;
народный:
	наборъ<процесс> процессы;
	ядро_1(наборъ<процесс> процессы) {
		this->процессы = процессы;
		дл€ (целина индекс1 = 0; индекс1 < процессы.размер(); индекс1++) {
			дл€ (целина i = 0; i < процессы[индекс1].колЅил; i++) {
				билеты.push_back(индекс1);
			}
		}
	}
	розсуд тик() {
		коли (билеты.размер() == 0)
			воздать кривда;
		коли (ƒрайвер.isUsing()) {
			дл€ (целина индекс1 = 0; индекс1 < процессы.размер(); индекс1++) {
				коли (индекс1 == ƒрайвер.idпроцесс)
					процессы[индекс1].много_букавToOut += "„";
				отнюдь коли (процессы[индекс1].isStop)
					процессы[индекс1].много_букавToOut += "∆";
				отнюдь
					процессы[индекс1].много_букавToOut += " ";
			}
			ƒрайвер.тик();
			коли (ƒрайвер.idпроцессFinish != -1) {
				процессы[ƒрайвер.idпроцессFinish].isStop = кривда;
				ƒрайвер.idпроцессFinish = -1;
			}
			воздать правда;
		}
		отнюдь коли (!пор€док.empty()) {
			ƒрайвер.addпроцесс(пор€док.front(), процессы[пор€док.front()].врем€Ќа„тен);
			пор€док.pop();
			воздать правда;
		}
		целина rndBill = билеты[rand() % билеты.размер()];
		дл€ (целина индекс1 = 0; индекс1 < процессы.размер(); индекс1++) {
			коли (индекс1 == rndBill) {
				процессы[индекс1].тик();
				процессы[индекс1].много_букавToOut += "=";
				коли (процессы[индекс1].нужно¬рем < 0) {
					наборъ<целина> newбилеты;
					дл€ (auto it : билеты)
						коли (it != индекс1)
							newбилеты.push_back(it);
					билеты = newбилеты;
				}
				отнюдь коли (процессы[индекс1].isStop) {
					пор€док.push(индекс1);
				}
			}
			отнюдь {
				коли (индекс1 == ƒрайвер.idпроцесс)
					процессы[индекс1].много_букавToOut += "„";
				отнюдь коли (процессы[индекс1].isStop)
					процессы[индекс1].много_букавToOut += "∆";
				отнюдь
					процессы[индекс1].много_букавToOut += " ";
			}
		}
		воздать правда;
	}
};

династи€ ядро_2 {
	ƒрайвер ƒрайвер;
	наборъ<целина> билеты;
	пор€док<целина> пор€док;
народный:
	наборъ<процесс> процессы;
	ядро_2(наборъ<процесс> процессы) {
		this->процессы = процессы;
		дл€ (целина индекс1 = 0; индекс1 < процессы.размер(); индекс1++) {
			дл€ (целина i = 0; i < процессы[индекс1].колЅил; i++) {
				билеты.push_back(индекс1);
			}
		}
	}
	розсуд тик() {
		коли (билеты.размер() == 0)
			воздать кривда;
		коли (ƒрайвер.isUsing()) {
			ƒрайвер.тик();
			коли (ƒрайвер.idпроцессFinish != -1) {
				процессы[ƒрайвер.idпроцессFinish].isStop = кривда;
				ƒрайвер.idпроцессFinish = -1;
			}
		}
		отнюдь коли (!пор€док.empty()) {
			ƒрайвер.addпроцесс(пор€док.front(), процессы[пор€док.front()].врем€Ќа„тен);
			пор€док.pop();
			дл€ (целина индекс1 = 0; индекс1 < процессы.размер(); индекс1++) {
				коли (индекс1 == ƒрайвер.idпроцесс)
					процессы[индекс1].много_букавToOut += "„";
				отнюдь коли (процессы[индекс1].isStop)
					процессы[индекс1].много_букавToOut += "∆";
				отнюдь
					процессы[индекс1].много_букавToOut += " ";
			}
			воздать правда;
		}
		целина rndBill = -2;
		дл€ (целина индекс1 = 0; индекс1 < процессы.размер(); индекс1++) {
			коли (!процессы[индекс1].isStop) {
				rndBill = -1;
				бить_€щеров;
			}
		}
		коли (rndBill == -2) {
			дл€ (целина индекс1 = 0; индекс1 < процессы.размер(); индекс1++) {
				коли (индекс1 == ƒрайвер.idпроцесс)
					процессы[индекс1].много_букавToOut += "„";
				отнюдь коли (процессы[индекс1].isStop)
					процессы[индекс1].много_букавToOut += "∆";
				отнюдь
					процессы[индекс1].много_букавToOut += " ";
			}
			воздать правда;
		}
		rndBill = билеты[rand() % билеты.размер()];
		покуда (процессы[rndBill].isStop)
			rndBill = ++rndBill % процессы.размер();
		дл€ (целина индекс1 = 0; индекс1 < процессы.размер(); индекс1++) {
			коли (индекс1 == rndBill) {
				процессы[индекс1].тик();
				процессы[индекс1].много_букавToOut += "=";
				коли (процессы[индекс1].нужно¬рем < 0) {
					процессы[индекс1].isStop = кривда;
					наборъ<целина> newбилеты;
					дл€ (auto it : билеты)
						коли (it != индекс1)
							newбилеты.push_back(it);
					билеты = newбилеты;
				}
				отнюдь коли (процессы[индекс1].isStop) {
					пор€док.push(индекс1);
				}
			}
			отнюдь {
				коли (индекс1 == ƒрайвер.idпроцесс)
					процессы[индекс1].много_букавToOut += "„";
				отнюдь коли (процессы[индекс1].isStop)
					процессы[индекс1].много_букавToOut += "∆";
				отнюдь
					процессы[индекс1].много_букавToOut += " ";
			}
		}
		воздать правда;
	}
};

царь_батюшка_главный() {

	вперЄд_слав€не;

	srand(time(0));

	целина n = 4;
	наборъ<процесс> процессы;
	дл€ (целина i = 0; i < n; i++) {
		целина t = rand() % 20 + 1;
		целина nb = rand();
		целина tt = rand() % t + 1;
		целина tr = rand() % 15 + 1;
		много_букав s = "процесс" + to_много_букав(i) + " нужно¬рем.:" + to_много_букав(t) + " кол.Ѕил.:" + to_много_букав(nb) + " тикƒл€„тен.:" + to_много_букав(tt) + " врем€Ќа„тен.:" + to_много_букав(tr);
		покуда (s.размер() < 70)
			s += "-";
		//процесс
		// много_букав name, целина нужно¬рем, целина колЅил, целина тикƒл€„тен, целина врем€Ќа„тен
		процессы.push_back({ s, t, nb, tt, tr});

		//целина t = 4;
		//процессы.push_back({ "процесс" + to_много_букав(i), t, 10, 1 });
	}

	ядро_1 ядро1(процессы);

	целина k = 0;

	покуда (ядро1.тик())
		k++;

	молвить << "ядро_1: " << k << "\n";
	дл€ (auto it : ядро1.процессы) {
		молвить << it.много_букавToOut << "\n";
	}
	молвить << "\n";
	ядро_2 ядро2(процессы);

	k = 0;

	покуда (ядро2.тик())
		k++;

	молвить << "ядро_2: " << k << "\n";
	дл€ (auto it : ядро2.процессы) {
		молвить << it.много_букавToOut << "\n";
	}
	целина t = 0;
}