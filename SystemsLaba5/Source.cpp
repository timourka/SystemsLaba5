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

#include "��_�����_�����.h"

�������� ����� ����;

�������� ������� {
�������:
	������ timeRest = 5;
��������:
	������ id������� = -1;
	������ id�������Finish = -1;
	������ isUsing() {
		������� id������� != -1;
	}
	������ add�������(������ id, ������ ���������) {
		���� (isUsing())
			������� ������;
		id������� = id;
		timeRest = ���������;
		������� ������;
	}
	������ ���() {
		���� (!isUsing())
			������� ������;
		���� (--timeRest == 0) {
			id�������Finish = id�������;
			id������� = -1;
		}
		������� ������;
	}
};

�������� ������� {
�������:
��������:
	�����_����� �����_�����ToOut;
	������ ��������� = 100;
	������ ������ = 10;
	������ ���������� = 4;
	������ ����������� = 5;
	������ isStop = ������;
	�������(�����_����� name, ������ ���������, ������ ������, ������ ����������, ������ �����������): �����_�����ToOut(name + " "), ���������(���������), ������(������), ����������(����������), �����������(�����������){ }
	������ ���() {
		���� (��������� == ����������)
			isStop = ������;
		���������--;
		������� ������;
	}
};

�������� ����_1 {
	������� �������;
	������<������> ������;
	�������<������> �������;
��������:
	������<�������> ��������;
	����_1(������<�������> ��������) {
		this->�������� = ��������;
		��� (������ ������1 = 0; ������1 < ��������.������(); ������1++) {
			��� (������ i = 0; i < ��������[������1].������; i++) {
				������.push_back(������1);
			}
		}
	}
	������ ���() {
		���� (������.������() == 0)
			������� ������;
		���� (�������.isUsing()) {
			��� (������ ������1 = 0; ������1 < ��������.������(); ������1++) {
				���� (������1 == �������.id�������)
					��������[������1].�����_�����ToOut += "�";
				������ ���� (��������[������1].isStop)
					��������[������1].�����_�����ToOut += "�";
				������
					��������[������1].�����_�����ToOut += " ";
			}
			�������.���();
			���� (�������.id�������Finish != -1) {
				��������[�������.id�������Finish].isStop = ������;
				�������.id�������Finish = -1;
			}
			������� ������;
		}
		������ ���� (!�������.empty()) {
			�������.add�������(�������.front(), ��������[�������.front()].�����������);
			�������.pop();
			������� ������;
		}
		������ rndBill = ������[rand() % ������.������()];
		��� (������ ������1 = 0; ������1 < ��������.������(); ������1++) {
			���� (������1 == rndBill) {
				��������[������1].���();
				��������[������1].�����_�����ToOut += "=";
				���� (��������[������1].��������� < 0) {
					������<������> new������;
					��� (auto it : ������)
						���� (it != ������1)
							new������.push_back(it);
					������ = new������;
				}
				������ ���� (��������[������1].isStop) {
					�������.push(������1);
				}
			}
			������ {
				���� (������1 == �������.id�������)
					��������[������1].�����_�����ToOut += "�";
				������ ���� (��������[������1].isStop)
					��������[������1].�����_�����ToOut += "�";
				������
					��������[������1].�����_�����ToOut += " ";
			}
		}
		������� ������;
	}
};

�������� ����_2 {
	������� �������;
	������<������> ������;
	�������<������> �������;
��������:
	������<�������> ��������;
	����_2(������<�������> ��������) {
		this->�������� = ��������;
		��� (������ ������1 = 0; ������1 < ��������.������(); ������1++) {
			��� (������ i = 0; i < ��������[������1].������; i++) {
				������.push_back(������1);
			}
		}
	}
	������ ���() {
		���� (������.������() == 0)
			������� ������;
		���� (�������.isUsing()) {
			�������.���();
			���� (�������.id�������Finish != -1) {
				��������[�������.id�������Finish].isStop = ������;
				�������.id�������Finish = -1;
			}
		}
		������ ���� (!�������.empty()) {
			�������.add�������(�������.front(), ��������[�������.front()].�����������);
			�������.pop();
			��� (������ ������1 = 0; ������1 < ��������.������(); ������1++) {
				���� (������1 == �������.id�������)
					��������[������1].�����_�����ToOut += "�";
				������ ���� (��������[������1].isStop)
					��������[������1].�����_�����ToOut += "�";
				������
					��������[������1].�����_�����ToOut += " ";
			}
			������� ������;
		}
		������ rndBill = -2;
		��� (������ ������1 = 0; ������1 < ��������.������(); ������1++) {
			���� (!��������[������1].isStop) {
				rndBill = -1;
				����_������;
			}
		}
		���� (rndBill == -2) {
			��� (������ ������1 = 0; ������1 < ��������.������(); ������1++) {
				���� (������1 == �������.id�������)
					��������[������1].�����_�����ToOut += "�";
				������ ���� (��������[������1].isStop)
					��������[������1].�����_�����ToOut += "�";
				������
					��������[������1].�����_�����ToOut += " ";
			}
			������� ������;
		}
		rndBill = ������[rand() % ������.������()];
		������ (��������[rndBill].isStop)
			rndBill = ++rndBill % ��������.������();
		��� (������ ������1 = 0; ������1 < ��������.������(); ������1++) {
			���� (������1 == rndBill) {
				��������[������1].���();
				��������[������1].�����_�����ToOut += "=";
				���� (��������[������1].��������� < 0) {
					��������[������1].isStop = ������;
					������<������> new������;
					��� (auto it : ������)
						���� (it != ������1)
							new������.push_back(it);
					������ = new������;
				}
				������ ���� (��������[������1].isStop) {
					�������.push(������1);
				}
			}
			������ {
				���� (������1 == �������.id�������)
					��������[������1].�����_�����ToOut += "�";
				������ ���� (��������[������1].isStop)
					��������[������1].�����_�����ToOut += "�";
				������
					��������[������1].�����_�����ToOut += " ";
			}
		}
		������� ������;
	}
};

����_�������_�������() {

	�����_�������;

	srand(time(0));

	������ n = 4;
	������<�������> ��������;
	��� (������ i = 0; i < n; i++) {
		������ t = rand() % 20 + 1;
		������ nb = rand();
		������ tt = rand() % t + 1;
		������ tr = rand() % 15 + 1;
		�����_����� s = "�������" + to_�����_�����(i) + " ���������.:" + to_�����_�����(t) + " ���.���.:" + to_�����_�����(nb) + " ����������.:" + to_�����_�����(tt) + " �����������.:" + to_�����_�����(tr);
		������ (s.������() < 70)
			s += "-";
		//�������
		// �����_����� name, ������ ���������, ������ ������, ������ ����������, ������ �����������
		��������.push_back({ s, t, nb, tt, tr});

		//������ t = 4;
		//��������.push_back({ "�������" + to_�����_�����(i), t, 10, 1 });
	}

	����_1 ����1(��������);

	������ k = 0;

	������ (����1.���())
		k++;

	������� << "����_1: " << k << "\n";
	��� (auto it : ����1.��������) {
		������� << it.�����_�����ToOut << "\n";
	}
	������� << "\n";
	����_2 ����2(��������);

	k = 0;

	������ (����2.���())
		k++;

	������� << "����_2: " << k << "\n";
	��� (auto it : ����2.��������) {
		������� << it.�����_�����ToOut << "\n";
	}
	������ t = 0;
}