#ifndef IPTOOL_H
#define IPTOOL_H
#include <vector>
#include <string>

struct ips{
	std::string addr;
	int total;
	int current;
}typedef ip;

struct ipchoose{

	std::vector<ip> ipList;

	void add(std::string address){
		ip *added;
		added->addr = address;
		added->total = 0;
		added->current =0;
		ipList.push_back(*added);
	}

	std::string getBest(){
		ip best = ipList[0];
		for (std::vector<ip>::iterator i = ipList.begin(); i != ipList.end(); ++i)
		{
			if (best.current > (*i).current)
			{
				best = *i;
			}
		}
		best.current++;
		best.total++;
		return best.addr;
	}
	void update(std::string s){
		for (std::vector<ip>::iterator i = ipList.begin(); i != ipList.end(); ++i)
		{
			if (s==(*i).addr)
			{
				(*i).current--;
			}
		}
	}
} typedef iptool;

#endif