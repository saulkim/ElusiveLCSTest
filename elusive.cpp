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
	int length;
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

Answer kmpAlgo(int file1, int file2, Answer ans, std::vector<FileObj> master) {
	/*
	note to self: seems like good practice and possibly the best solution. definitely better than naive
	*/

	return ans;
}

std::string converter(std::vector<std::string> vec) {
	std::string result;
	for (const auto& i : vec) {
		result += i;
	}

	return result;
}

Answer naiveChecking(int file1, int file2, Answer ans, std::vector<FileObj> master) {
	int longestAns = 0;
	int indexOfLargerFile = 0;

	std::string aString = converter(master[file1].getData());
	std::string bString = converter(master[file2].getData());

	bool aLarger = false;
	if (aString.size() > bString.size()) {
		aLarger = true;
	}

	if (aLarger) {
		for (int i = 0; i < bString.size(); i++) {
			std::string subString = bString.substr(0, i);
			if (aString.find(subString) != std::string::npos) {
				indexOfLargerFile = aString.find(subString);
				longestAns = i;
			}
		}
	}
	else {
		for (int i = 0; i < aString.size(); i++) {
			std::string subString = aString.substr(0, i);
			if (bString.find(subString) != std::string::npos) {
				indexOfLargerFile = bString.find(subString);
				longestAns = i;
			}
		}
	}

	ans.length = longestAns;

	//inserts the smaller file data
	std::string path1ToString{ master[0].getPath().filename().u8string() };
	ans.data.insert(std::pair<std::string, int>(path1ToString, 0));

	//inserts the larger file data
	std::string path2ToString{ master[1].getPath().filename().u8string() };
	ans.data.insert(std::pair<std::string, int>(path2ToString, indexOfLargerFile));
	return ans;

}

Answer longestCommonSequence(std::vector<FileObj> masterList, Answer ans) {

	//looping through all files in folder
	int numberOfFiles = masterList.size();
	for (int i = 0; i < numberOfFiles - 1; i++) {
		for (int j = i + 1; j < numberOfFiles; j++) {
			ans = naiveChecking(i, j, ans, masterList);
			// ans = kmpAlgo(i, j, ans, masterList);
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
