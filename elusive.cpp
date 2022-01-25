#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <map>
/* note to self
   g++ needs "-std=c++17" because filesystem::directory_iterator is relatively new
*/

class FileObj {
private:
	std::filesystem::path fileName;
	std::vector<std::string> data;
public:
	std::vector<std::string> getData() {
		return data;
	}
	std::filesystem::path getPath() {
		return fileName;
	}

	void setData(std::vector<std::string> d) {
		data = d;
	}
	void setPath(std::filesystem::path p) {
		fileName = p;
	}
};

struct Answer {
	int length = 0;
	std::map<std::string, int> data;
};

std::vector<FileObj> fileReader() {
	std::vector<FileObj> results;

	try {
		//std::filesystem::path dir = std::filesystem::current_path();
		//std::cout << dir;
		const std::filesystem::path dir{ "files" }; //change string here to change folder path

		auto it = std::filesystem::directory_iterator(dir);
		for (auto const& i : it) {
			//'i' is the each individual file inside the folder 'it'
			FileObj fo;
			fo.setPath(i);

			std::vector<std::string> data;
			std::ifstream ifs(fo.getPath(), std::ios::binary);
			if (ifs.good()) {
				while (ifs.good()) {
					char c = ifs.get();
					std::string s(1, c);
					data.push_back(s);
				}
			}
			else {
				std::cout << "could not read specific file";
			}
			fo.setData(data);
			results.push_back(fo);
		}
	}
	catch (std::filesystem::filesystem_error const& e) {
		std::cout << "general file reading error" << "\n";
		std::cerr << e.what();
	}

	return results;
}

std::string converter(std::vector<std::string> vec) {
	std::string result;
	for (const auto& i : vec) {
		result += i;
	}

	return result;
}

int  lcsRecursion() {
	/*
	note to self: figure out longest common sequence (not subsequence, most on internet assumes non-consecutive)
	and how to work with large 2d, currently these files make a 2d grid of 17000x1400 = 200mil+ and i think it's
	overflowing the stack size;
	try to do this problem in a more efficient way;
	solution1?: break each file down into halves and compare the halves using lcs. Problem: halves will break sequence
	and require concats, which not a very graceful method
	*/
	return 0;
}

void findLPS(std::string pattern, int maxLength, std::vector<int> lps) {
	int length = 0;
	int i = 1;
	lps[0] = 0;
	while (i < maxLength) {
		if (pattern[i] == pattern[length]) {
			lps[i] = length + 1;
			length++;
			i++;
		}
		else {
			if (length != 0) {
				length = lps[length - 1];
			}
			else {
				lps[i] = 0;
				i++;
			}

		}
	}
}

int kmpAlgo(std::string patternToMatch, std::string entireText) {
	int result = 0;

	int entireTextSize = entireText.size();
	int patternSize = patternToMatch.size();
	

		std::vector<int> lps(patternSize);
		findLPS(patternToMatch, patternSize, lps);
		int i = 0;
		int j = 0;
		while (i < entireTextSize) {
			if (entireText[i] == patternToMatch[j]) {
				i++;
				j++;
			}
			else {
				if (j != 0) {
					j = lps[j - 1];
				}
				else {
					i++;
				}
			}
			if (j == patternSize) {
				result = i - j;
				j = lps[j - 1];
			}
		}	
	return result;
}



Answer naiveChecking(int file1, int file2, Answer ans, std::vector<FileObj> master) {
	int longestAns = 0;
	int indexOfSmallerfile = 0;
	int indexOfLargerFile = 0;

	std::string aString = converter(master[file1].getData());
	std::string bString = converter(master[file2].getData());

	bool aLarger = false;
	if (aString.size() > bString.size()) {
		aLarger = true;
	}

	if (aLarger) {
		for (int i = 0; i < bString.size(); i++) {
			for (int j = 1; j <= bString.size() - i; j++) {
				std::string subString = bString.substr(i, j);
				if (aString.find(subString) != std::string::npos && j > longestAns) {
					longestAns = j;
					indexOfSmallerfile = i;
					indexOfLargerFile = aString.find(subString);
				}
			}

		}
	}
	else {
		for (int i = 0; i < aString.size(); i++) {
			for (int j = 1; j <= aString.size() - i; j++) {
				std::string subString = aString.substr(i, j);
				if (bString.find(subString) != std::string::npos && j > longestAns) {
					longestAns = j;
					indexOfSmallerfile = i;
					indexOfLargerFile = bString.find(subString);
				}
			}
		}
	}

	if (longestAns > ans.length) {
		ans.length = longestAns;

		//inserts the smaller file data
		std::string path1ToString{ master[0].getPath().filename().u8string() };
		//ans.data.insert(std::pair<std::string, int>(path1ToString, 0));
		ans.data.insert_or_assign(path1ToString, indexOfSmallerfile);

		//inserts the larger file data
		std::string path2ToString{ master[1].getPath().filename().u8string() };
		//ans.data.insert(std::pair<std::string, int>(path2ToString, indexOfLargerFile));
		ans.data.insert_or_assign(path2ToString, indexOfLargerFile);
	}
	return ans;

}

Answer subStringer(int file1, int file2, Answer ans, std::vector<FileObj> master) {
	int longestAns = 0;
	int indexOfSmallerfile = 0;
	int indexOfLargerFile = 0;

	std::string aString = converter(master[file1].getData());
	std::string bString = converter(master[file2].getData());

	bool aLarger = false;
	if (aString.size() > bString.size()) {
		aLarger = true;
	}

	if (aLarger) {
		for (int i = 0; i < bString.size(); i++) {
			for (int j = 1; j <= bString.size() - i; j++) {
				std::string subString = bString.substr(i, j);
				int foundMatch = kmpAlgo(subString, aString); //returns the index in the larger file where that substring starts
				if (foundMatch > 0 && subString.size() > longestAns) {
					longestAns = subString.size();
					indexOfSmallerfile = i;
					indexOfLargerFile = foundMatch;
				}
			}
		}
	}
	else {
		for (int i = 0; i < aString.size(); i++) {
			for (int j = 1; j <= aString.size() - i; j++) {
				std::string subString = aString.substr(i, j);
				int foundMatch = kmpAlgo(subString, bString); //returns the index in the larger file where that substring starts
				if (foundMatch > 0 && subString.size() > longestAns) {
					longestAns = subString.size();
					indexOfSmallerfile = i;
					indexOfLargerFile = foundMatch;
				}
			}
		}
	}

	if (longestAns > ans.length) {
		ans.length = longestAns;

		if (aLarger) {
			//inserts the smaller file data
			std::string pathSToString{ master[file2].getPath().filename().u8string() };
			ans.data.insert_or_assign(pathSToString, indexOfSmallerfile);

			//inserts the larger file data
			std::string pathLToString{ master[file1].getPath().filename().u8string() };
			ans.data.insert_or_assign(pathLToString, indexOfLargerFile);
		}
		else {
			//inserts the smaller file data
			std::string pathSToString{ master[file1].getPath().filename().u8string() };
			ans.data.insert_or_assign(pathSToString, indexOfSmallerfile);

			//inserts the larger file data
			std::string pathLToString{ master[file2].getPath().filename().u8string() };
			ans.data.insert_or_assign(pathLToString, indexOfLargerFile);
		}

	}


	return ans;
}

Answer longestCommonSequence(std::vector<FileObj> masterList, Answer ans) {
	int numberOfFiles = masterList.size();
	for (int i = 0; i < numberOfFiles - 1; i++) {
		for (int j = i + 1; j < numberOfFiles; j++) {

			ans = subStringer(i, j, ans, masterList);
		}
	}

	return ans;
}

void printAnswer(Answer ans) {
	std::cout << ans.length << "\n";
	for (const auto& i : ans.data) {
		std::cout << i.first << ":" << i.second << "\n";
	}
}



int main() {

	std::vector<FileObj> allFilesParsed = fileReader();
	Answer ans;

	ans = longestCommonSequence(allFilesParsed, ans);
	printAnswer(ans);

}
