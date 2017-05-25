#pragma once
#include <vector>
using namespace std;
class ClusterCategories
{
private:
	std::vector<std::string> data;
	int calculateDistance(string, string, int, int);
public:
	ClusterCategories(vector<string>);
	vector<string> cluster();
};
