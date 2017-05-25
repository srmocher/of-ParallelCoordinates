#include "cluster.h"
#include <algorithm>
#include <random>

int min(int x, int y, int z)
{
	return min(min(x, y), z);
}
ClusterCategories::ClusterCategories(vector<string> data)
{
	
	this->data = data;
}

vector<string> ClusterCategories::cluster()
{
	vector<string> test;
	return test;
}

int ClusterCategories::calculateDistance(string str1, string str2, int m, int n)
{
	// insert all characters of second string into first
	if (m == 0) return n;

	// If second string is empty, the only option is to
	// remove all characters of first string
	if (n == 0) return m;

	// If last characters of two strings are same, nothing
	// much to do. Ignore last characters and get count for
	// remaining strings.
	if (str1[m - 1] == str2[n - 1])
		return calculateDistance(str1, str2, m - 1, n - 1);

	// If last characters are not same, consider all three
	// operations on last character of first string, recursively
	// compute minimum cost for all three operations and take
	// minimum of three values.
	return 1 + min(calculateDistance(str1, str2, m, n - 1),    // Insert
		calculateDistance(str1, str2, m - 1, n),   // Remove
		calculateDistance(str1, str2, m - 1, n - 1) // Replace
	);
}