#include <iostream>
#include <fstream>
#include <unordered_map>
#include <list>
#include <set>
#include <sstream>
#include <climits>

using namespace std;

class StopWords {
public:
    StopWords() {
        ifstream file("stopwords.txt"); 
        string line;
        while (getline(file, line)) {
            stopWords.insert(line);
        }
        file.close();
    }

    bool isStopWord(const string& word) const {
        return stopWords.count(word) > 0;
    }

private:
    set<string> stopWords;
};

unordered_map<string, float> sentiment_lexicon;

float calculate_sentiment_score(const list<string>& tokens) {
    float sentiment_score = 0.0f;
    bool negate = false; 

    for (const string& token : tokens) {
        if (token == "not" || token == "no") {
            negate = true;
        } else if (sentiment_lexicon.count(token) > 0) {
            float score = sentiment_lexicon[token];
            if (negate) {
                score = -score;
                negate = false; 
            }
            sentiment_score += score;
        }
    }

    return sentiment_score;
}

int main() {
    ifstream lexiconFile("lexicon.txt");
    string line;
    while (getline(lexiconFile, line)) {
        string word;
        float score;
        istringstream iss(line);
        iss >> word >> score;
        sentiment_lexicon[word] = score;
    }
    lexiconFile.close();

    StopWords stopWords;

    int choice;
    string text;
    list<string> tokens;

    while (true) {
        cout << "Menu:\n";
        cout << "1. Enter text to analyze\n";
        cout << "2. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

    
        cin.ignore(INT_MAX, '\n');

        if (choice == 1) {
            cout << "Enter the text to analyze: ";
            getline(cin, text);
            istringstream iss(text);
            string word;
            while (iss >> word) {
                tokens.push_back(word);
            }
            tokens.remove_if([&](const string& token) {
                return stopWords.isStopWord(token);
            });

            float sentiment_score = calculate_sentiment_score(tokens);
            cout << "The sentiment score of the text is: " << sentiment_score << endl;
            if (sentiment_score > 0.0f) {
                cout << "Positive review" << endl;
            } else if (sentiment_score < 0.0f) {
                cout << "Negative review" << endl;
            } else {
                cout << "Neutral review" << endl;
            }
            tokens.clear(); 
        } else if (choice == 2) {
            cout << "Thank you for using our project Sentiment Analysis!!" << endl;
            break; 
        } else {
            cout << "Invalid choice. Please select a valid option." << endl;
        }
    }
    return 0;
}