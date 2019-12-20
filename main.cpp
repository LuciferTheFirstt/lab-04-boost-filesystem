#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <map>
#include <exception>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using namespace boost::filesystem;
using namespace std;

map<string, map<int, pair<int, int>>> brokers;

void Print(string file_name, string extension,int account,int date,const path& path)
{
	string broker_name = x.path().parent_path().filename().string();
	cout << broker_name << " " << file_name << endl;
	brokers[broker_name][account].first = max(brokers[broker_name][account].first, date);
	brokers[broker_name][account].second++;

}

void fillup(const path& path) {
	cout << path.string() << endl;
	for (directory_entry& x : directory_iterator(path)) {
		if (is_directory(x) || is_symlink(x))
			fillup(x.path());

		if (!is_regular_file(x))
			continue;

		string file_name = x.path().filename().string();
		string extension = x.path().extension().string();

		if (file_name.find("balance_") != 0)
			continue;

		if (extension != ".txt")
			continue;

		if (file_name.length() != 29)
			continue;

		int account = 0;
		int date = 0;
		//try {
			account = stoi(file_name.substr(8, 8));
			date = stoi(file_name.substr(17, 8));
		//}
		/*catch (exception & e) {
			continue;
		}*/
		Print(file_name,extension,account,date,*path);
	}
}


int main(int argc, char* argv[])
{

	path* p;

	if (argc < 2)
	{
		p= new path(current_path());
	}
	else p = new path(argv[1]);

	vector<string> files;
	vector<string> folders;
	try{
		bool exists( const path& p );
	}
	catch(...){
		throw std::invalid_argument("p");
	}
	for (const auto& dirEntry : recursive_directory_iterator(*p))
	{
	
		if (is_directory(dirEntry))
		{
			continue;
		}

		if (is_regular_file(dirEntry))
		{
			const boost::regex file_filter("balance_[0-9]{8}_[0-9]{8}.txt");

			if (boost::regex_match(dirEntry.path().filename().string(), file_filter, boost::regex_constants::match_default))
			{
				files.push_back(dirEntry.path().filename().string());
				folders.push_back(dirEntry.path().parent_path().filename().string());
			}
		}
	}
	for (size_t i = 0; i < files.size(); ++i)
	{
		cout << folders[i] << " " << files[i] << endl;
	}

	fillup(*p);
	
	cout << "\n\tOverall:\n";
	if (!brokers.empty()) {
		for_each(brokers.begin(), brokers.end(), [](auto& i) {
			for (const auto& j : i.second)
				cout << "broker: " << i.first << " account: " << j.first << " files: "
				<< j.second.second << " lastdate: " << j.second.first << endl;
			});
	}
	else
		cout << endl << "Empty!" << endl;


	delete p;
	return 0;
}

