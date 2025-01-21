#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <regex>
#include <cctype>
#include <cmath>

class WordsFreqAnalyzator
{
private:
    std::string filename;
    std::string CSVfilename;
    std::list<const std::pair<const std::string, int>*> listOfMapEntries;
    std::map<std::string, int> mapWithWordsFrequency;
public:
    WordsFreqAnalyzator(const std::string &filename, const std::string &csvfilename) : 
        filename(filename), CSVfilename(csvfilename) {};

    WordsFreqAnalyzator(const std::string &filename) : 
        filename(filename), CSVfilename(filename + ".csv") {};

    ~WordsFreqAnalyzator() = default;

    void parse();
};

void WordsFreqAnalyzator::parse() 
{
    std::ifstream fileIStream(filename);
    std::ofstream fileOStream(CSVfilename);

    if (!fileIStream.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    if (!fileOStream.is_open())
    {
        std::cerr << "Error: Could not open file " << CSVfilename << "\n";
        return;
    }

    static const std::regex tokens("[a-zA-Z0-9]+");
    std::string line;
    while (getline(fileIStream, line))
    {
        auto wordsItStart = std::sregex_iterator(line.begin(), line.end(), tokens);
        auto wordsItEnd = std::sregex_iterator();

        for (std::sregex_iterator i = wordsItStart; i != wordsItEnd; i++)
        {
            std::string word = i->str();
            for (char &ch : word) 
            {
                ch = std::tolower(ch); 
            }
            mapWithWordsFrequency[word] += 1;
        }
    }

    for (const auto &pair : mapWithWordsFrequency)
    {
        listOfMapEntries.push_back(&pair);
    }

    listOfMapEntries.sort([](const std::pair<const std::string, int>* a, const std::pair<const std::string, int>* b) {
        return a->second > b->second; 
    });

    const int wordsNum = mapWithWordsFrequency.size();
    for (const auto &pair : listOfMapEntries)
    {
        const std::string word = pair->first;
        const double freq = pair->second;
        const double freqProcetage = (freq / wordsNum) * 100;

        fileOStream << word << "," << freq << "," << std::ceil(freqProcetage * 10) / 10.0 << "\n";
    }    
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> [output_file]\n";
        return 1;
    }

    std::string filename(argv[1]);
    std::string CSVfilename;

    if (argc == 3) {
        CSVfilename = argv[2];
    } else {
        CSVfilename = filename + ".csv";
    }

    WordsFreqAnalyzator analyzator(filename, CSVfilename);
    analyzator.parse();

    return 0;
}