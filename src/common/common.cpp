#include "common.hpp"
int _checked(int ret, const char* calling_function) {
  if (ret < 0) {
    perror(calling_function);
    exit(EXIT_FAILURE);
  }
  return ret;
}


void replace(string &str, const char old, const char nw)
{
  for (size_t i = 0; i < str.size(); ++i)
    if (str[i] == old)
      str[i] = nw;
}

int len(string str){
	int length = 0;
	for (uint i=0; str[i]!='\0'; i++){
		length++;
	}
	return length;
}

vector<string> split(string str, char seperator){
	uint i=0, startIndex=0, endIndex=0;
	vector<string> vect;
	while (static_cast<int>(i) <= len(str)){
		if (str[i] == seperator || static_cast<int>(i) == len(str)){
			endIndex = i;
			string subStr = "";
			subStr.append(str, startIndex, endIndex - startIndex);
			vect.push_back(subStr);
			startIndex = endIndex + 1;
		}
		i++;
	}
	return vect;
}

string vectToStr(vector<string> vect){
	/*
	Formatage d'un vecteur de string en string.
	*/
	string ret;
	for (uint i=0; i<vect.size();i++){
		ret.append(vect[i]);
		ret.append("\n");
	}
	return ret;
}
