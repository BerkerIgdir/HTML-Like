#include <algorithm>
#include <iostream>
#include <iterator>
#include <regex>
#include <utility>
#include <string>
#include <vector>
#include <sstream>
#include <map>

using namespace std;


int main(int argc, char* argv[])
{
	uint32_t QueryNum;
	uint32_t TagNum;
	string data{};
	smatch m; 
	map<string, vector<pair<string, string>>> TagAttributeMap;
	vector<pair<string, string>> TagVector;
	regex AttributeRegex("\\s+(\\w+)\\s+\\=\\s+(\\w+)");
	regex TagRegex("<(tag\\d+)(.+)>");
	regex TagWithoutAtt("<tag\\d+>");


	cout << "Girmek istedigniz Tag ve Query satir sayilarini aralarinda bir bosluk birakarak belirtiniz:(T Q)\n";
	cin >> TagNum >> QueryNum;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	for (size_t i = 0; i < TagNum; ++i) {

		bool TagFound{};
		bool AttFound{};
		cout << "Tag Giriniz:\n";
		getline(cin, data);

		string ClosingTagName{"</>"};

		if (regex_search(data, m, TagWithoutAtt)) {

			ClosingTagName.insert(2, m.str(1));
			if (!TagAttributeMap[m.str(1)].size())
				TagAttributeMap[m.str(1)].push_back(make_pair(string{}, string{}));
			else {
				cout << "Girilen tag halihazirda mevcut\n" << flush;
				--i;
				continue;
			}
		}
		else if (regex_search(data, m, TagRegex) && regex_search(data, AttributeRegex))
		{

			ClosingTagName.insert(2, m.str(1));

			if (TagVector.empty()|| find_if(TagVector.begin(), TagVector.end(), [&m](const auto& par) {return par.first == m.str(1); }) == TagVector.end())
				TagVector.push_back(make_pair(m.str(1), ClosingTagName));

				else  {
				cout << "Girilen tag halihazirda mevcut\n" << flush;
				--i;
				continue;
			}

			for (std::sregex_iterator i = std::sregex_iterator(data.begin(), data.end(), AttributeRegex);
				i != std::sregex_iterator();
				++i)
			{
				for (size_t k = 1; k < (*i).size(); k++)

					TagAttributeMap[m.str(1)].push_back(make_pair((*i).str(1), (*i).str(2)));
				
			}
		}

		else {
			cout << "bulunamadi\n";
			--i;
		}


		cout << TagVector.size() << "\n";

	}

	for (size_t i = TagVector.size(); i > 0; --i) {
		cout << "Tag Closing giriniz(Tag sirasina ters olarak):\n";
		getline(cin, data);

		if (data == TagVector[i-1].second) {
			cout << "Basarili\n";
		}
		else {
			cout << "Hatali Tag Girisi!\n";
			++i;
		}
	}

	for (size_t i = 0; i < QueryNum; ++i) {
		cout << "Query Giriniz:\n";
		getline(cin, data);
		
		if (data.find_first_of('.') != string::npos && TagNum > 1) {

			int cnt{ count(data.begin(), data.end(), '.') };

			string NestedQueryRegex("\\~(\\w+)");
			string temp = TagVector[cnt].first;
			NestedQueryRegex.insert(0, temp.insert(0, "(").append(")"));

			for (int k = cnt - 1; k >= 0; --k) {
				temp = TagVector[k].first;
				NestedQueryRegex.insert(0, (temp.insert(0, "(").append(")").append("\\.")));
			}



			bool found = regex_search(data,
				m,
				regex(NestedQueryRegex));


			if (found && TagAttributeMap.find(m.str(cnt + 1)) != TagAttributeMap.end()) {
				auto iter = find_if(TagAttributeMap[m.str(cnt + 1)].begin(), TagAttributeMap[m.str(cnt + 1)].end(), [&m,cnt](const auto& tag) {return tag.first == m.str(cnt + 2); });

				
				if (iter != TagAttributeMap[m.str(cnt + 1)].end())
					cout << iter->second << endl;
			}
			else {
				cout << "tag bulunamadi\n";
			}
		}
		else {

			string QueryRegex{ "()\\~(\\w+)" };

			QueryRegex.insert(1, TagVector.front().first);

			cout << QueryRegex <<endl;
			bool found = regex_search(data,
				m,
				regex(QueryRegex));

			if (found && TagAttributeMap.find(m.str(1)) != TagAttributeMap.end()) {
				auto iter = find_if(TagAttributeMap[m.str(1)].begin(), TagAttributeMap[m.str(1)].end(), [&m](const auto& tag) {return tag.first == m.str(2); });

				if (iter != TagAttributeMap[m.str(1)].end())
					cout << iter->second << endl;

				else {

					cout << "Not Found\n";
					--i;

				}
			}
			
		}
		}
		return 0;
}
	