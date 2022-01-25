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

Answer subStringerRecursive(int startIndex, int endIndex, int file1, int file2, Answer ans, std::vector<FileObj> master) {
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
		if (startIndex >= endIndex) {
			return ans;
		}

		else{
			int i = startIndex;
			int j = endIndex;

			std::string subString = bString.substr(i, j - i + 1);
			int foundMatch = kmpAlgo(subString, aString); //returns the index in the larger file where that substring starts
			if (foundMatch > 0 && subString.size() > ans.length) {
				longestAns = subString.size();
				indexOfSmallerfile = i;
				indexOfLargerFile = foundMatch;

				if (longestAns > ans.length) {
					ans.length = longestAns;

					//inserts the smaller file data
					std::string pathSToString{ master[file2].getPath().filename().u8string() };
					ans.data.insert_or_assign(pathSToString, indexOfSmallerfile);

					//inserts the larger file data
					std::string pathLToString{ master[file1].getPath().filename().u8string() };
					ans.data.insert_or_assign(pathLToString, indexOfLargerFile);
				}
			}
			ans = subStringerRecursive(i, j/2, file1, file2, ans, master);
			if ((endIndex - startIndex) > 1) {
				ans = subStringerRecursive((j / 2 + 1) + (i/2), j, file1, file2, ans, master);
			}
		}
	}
	else {
		if (startIndex >= endIndex) {
			return ans;
		}

		else {
			int i = startIndex;
			int j = endIndex;

			std::string subString = aString.substr(i, j - i + 1);
			int foundMatch = kmpAlgo(subString, bString); //returns the index in the larger file where that substring starts
			if (foundMatch > 0 && subString.size() > ans.length) {
				longestAns = subString.size();
				indexOfSmallerfile = i;
				indexOfLargerFile = foundMatch;

				if (longestAns > ans.length) {
					ans.length = longestAns;

					//inserts the smaller file data
					std::string pathSToString{ master[file1].getPath().filename().u8string() };
					ans.data.insert_or_assign(pathSToString, indexOfSmallerfile);

					//inserts the larger file data
					std::string pathLToString{ master[file2].getPath().filename().u8string() };
					ans.data.insert_or_assign(pathLToString, indexOfLargerFile);
				}
			}
			ans = subStringerRecursive(i, j / 2, file1, file2, ans, master);
			if ((endIndex - startIndex) > 1) {
				ans = subStringerRecursive((j / 2 + 1) + (i / 2), j, file1, file2, ans, master);
			}
		}
	}
	return ans;
}

Answer longestCommonSequence(std::vector<FileObj> masterList, Answer ans) {
	int numberOfFiles = masterList.size();
	for (int i = 0; i < numberOfFiles - 1; i++) {
		for (int j = i + 1; j < numberOfFiles; j++) {

			std::string aString = converter(masterList[i].getData());
			std::string bString = converter(masterList[j].getData());

			if (aString.size() > bString.size()) {
				ans = subStringerRecursive(0, bString.size() - 1, i, j, ans, masterList);
			}
			else {
				ans = subStringerRecursive(0, aString.size() - 1, i, j, ans, masterList);
			}	
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
