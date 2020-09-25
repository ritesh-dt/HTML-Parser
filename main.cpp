#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

using namespace std;

#define TAG_START '<'
#define TAG_END '>'

class Node {
	public:
		string name;
		string attr;
//		Node parent;
};

Node getTag(ifstream& fin) {
	char nextChar;
	fin.get(nextChar);

	// Get the tag name
	string tagName = "";
	while (1) {
		fin.get(nextChar);
		if (nextChar == TAG_END || nextChar == ' ')
			break;
		tagName.push_back(nextChar);
	}

	// Get the tag's attributes
	string tagAttr = "";
	if (nextChar != TAG_END)
	{
		while (1) {
			fin.get(nextChar);
			if (nextChar == TAG_END)
				break;
			tagAttr.push_back(nextChar);
		}
	} else {
		tagAttr = "null";
	}

	Node newNode;
	newNode.name = tagName;
	newNode.attr = tagAttr;

	return newNode;
}

Node getText(ifstream& fin) {
	char nextChar;

	// Get the tag name
	string text = "";
	while (1) {
		if (fin.peek() == TAG_START)
			break;
		fin.get(nextChar);
		text.push_back(nextChar);
	}

	Node newNode;
	newNode.name = "text";
	newNode.attr = text;

	return newNode;
}

int main()
{
	ifstream html;
	html.open("example.html");

	Node nodes[30];
	int nodeLength = 0;

	while (html) {
		// char nextChar;

		// html.get(nextChar);
		if (html.eof())
			break;
		if (html.peek() == TAG_START) {
			nodes[nodeLength++] = getTag(html);
			cout << nodes[nodeLength-1].name << "\tattr: " << nodes[nodeLength-1].attr << "\t";
			cout << (char) html.peek() << endl;
		}
		else {
			nodes[nodeLength++] = getText(html);
			cout << (char) html.get() << endl;
		}
	}
	nodeLength--;
	for (int index=0; index < nodeLength; index++)
		cout << nodes[index].name << "\tattr: " << nodes[index].attr << endl;
	
	html.close();
	return 0;
}