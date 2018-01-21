#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <tuple>
#include <vector>
#include <sstream>
#include <algorithm> //transform

using namespace std;

int file_one_sentence = 0;

tuple<int, int> return_total_sentences_number();
string return_sentence_of_file_one();
string return_sentence_of_file_two(int);
string unformatted_sentence(string);
int return_file_two_total_sentences();
vector<string> extract_words_from_sentence(string);
float similarity_of_each_sentence(vector<string>, vector<string>);
void show_max_similaritty_sentences(float &, vector<float>, string);

int main()
{
    int file_one_total_sentences_number, file_two_total_sentences_number, sentence_postion_of_file_one = 0;
    tie(file_one_total_sentences_number, file_two_total_sentences_number) = return_total_sentences_number();
    vector<float> final_similarity_result;
    while(sentence_postion_of_file_one < file_one_total_sentences_number) {
        string str1, str2;
        str1 = return_sentence_of_file_one();
        vector<string> sentence1 = extract_words_from_sentence(unformatted_sentence(str1));
        int sentence_postion_of_file_two = 0;
        vector<float> all_similarities;
        while(sentence_postion_of_file_two < file_two_total_sentences_number) {
            str2 = return_sentence_of_file_two(sentence_postion_of_file_two);
            vector<string> sentence2 = extract_words_from_sentence(unformatted_sentence(str2));
            float similarity = similarity_of_each_sentence(sentence1, sentence2);
            all_similarities.push_back(similarity);
            sentence_postion_of_file_two++;
        }
        /**Ѱ��һ�������ƶ�������ֵ**/
        float & max_similarity = all_similarities[0];
        for(int i = 1; i < all_similarities.size(); i++) {
            if(all_similarities[i] > max_similarity) {
                max_similarity = all_similarities[i];
            }
        }
        show_max_similaritty_sentences(max_similarity, all_similarities, str1);
        all_similarities.clear(); //����֮ǰ�����ݣ�����ѭ��¼��������
        cout << "Sentence" << sentence_postion_of_file_one + 1 << " of file one: " << max_similarity << "%" << endl;
        final_similarity_result.push_back(max_similarity);
        sentence_postion_of_file_one++;
    }
    /**�ó������������ƶ�ƽ��ֵ**/
    float final_result = 0;
    for(int i = 0; i < final_similarity_result.size(); i++) {
        final_result += final_similarity_result[i];
    }
    final_result /= final_similarity_result.size();
    cout << "Final: " << final_result << "%" << endl;
    return 0;
}

/**���ظ����ļ���������**/
tuple<int, int> return_total_sentences_number()
{
    int total_sentences_of_file_one = 0, total_sentences_of_file_two = 0;
    ifstream openFileOne, openFileTwo;
    char buffer1[500], buffer2[500];
    openFileOne.open("file1.txt", ios::in);
    openFileTwo.open("file2.txt", ios::in);
    if(openFileOne.is_open() && openFileTwo.is_open()) {
        /**ȥ���հ���**/
        string str1, str2;
        while(!openFileOne.eof()) {
            openFileOne.getline(buffer1, 500, '.');
            str1 = buffer1;
            if(!str1.empty()) {
                total_sentences_of_file_one++;
            }
        }
        while(!openFileTwo.eof()) {
            openFileTwo.getline(buffer2, 500, '.');
            str2 = buffer2;
            if(!str2.empty()) {
                total_sentences_of_file_two++;
            }
        }
        openFileOne.close();
        openFileTwo.close();
    }
    else {
        cout << "Could not open file or file does not exist." << endl;
    }
    return make_tuple(total_sentences_of_file_one, total_sentences_of_file_two);
}

/**��ȡ�������ļ�һ��ĳ������**/
string return_sentence_of_file_one()
{
    ifstream openFileOne;
    char buffer1[500];
    string str1;
    openFileOne.open("file1.txt", ios::in);
    if(openFileOne.is_open()) {
        for(int i = 0; i < file_one_sentence; i++){
            openFileOne.ignore(500, '.');
        }
        openFileOne.getline(buffer1, 500, '.');
        str1 = buffer1;
        openFileOne.close();
    }
    else {
        cout << "Could not open file or file does not exist." << endl;
    }
    file_one_sentence++;
    return str1;
}

string return_sentence_of_file_two(int sentence_postion)
{
    ifstream openFileTwo;
    char buffer2[500];
    string str2;
    openFileTwo.open("file2.txt", ios::in);
    if(openFileTwo.is_open()) {
        for(int i = 0; i < sentence_postion; i++){
            openFileTwo.ignore(500, '.');
        }
        openFileTwo.getline(buffer2, 500, '.');
        str2 = buffer2;
        openFileTwo.close();
    }
    else {
        cout << "Could not open file or file does not exist." << endl;
    }
    return str2;
}

/**ȥ�����ӵĸ�ʽ**/
string unformatted_sentence(string str)
{
    /**ȥ������(string)ǰ���Ŀհ�**/
    while(true) {
        if(isspace(str[0])) { //����string��һ��char�Ƿ�Ϊ�հ�
            str.erase(0, 1);
        }
        else {
            break;
        }
    }
    /**�Ѵ�дȫ��ת��ΪСд**/
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    /**ȥ�������еı�������**/
    int found = str.find_first_of(",.?");
    while (found != string::npos) {
        str[found] = ' ';
        found = str.find_first_of(",.?", found + 1);
    }
    return str;
}

/**��һ��������ȡΪһ��������**/
vector<string> extract_words_from_sentence(string str)
{
    vector<string> sentence;
    istringstream iss(str);
    do {
        string word;
        iss >> word;
        sentence.push_back(word);
    } while (iss);
    return sentence;
}

/**�Ƚ��������ӵ����ƶ�**/
float similarity_of_each_sentence(vector<string> sentence1, vector<string> sentence2)
{
    float same_words = 0, words_similarity, sentence_size;
    if(sentence1.size() >= sentence2.size()) {
        sentence_size = sentence1.size();
        for(int i = 0; i < sentence1.size(); i++) {
            for(int j = 0; j < sentence2.size(); j++) {
                if(sentence1[i] == sentence2[j]) {
                    same_words++;
                    break;
                }
            }
        }
    }
    else {
        sentence_size = sentence2.size();
        for(int i = 0; i < sentence2.size(); i++) {
            for(int j = 0; j < sentence1.size(); j++) {
                if(sentence2[i] == sentence1[j]) {
                    same_words++;
                    break;
                }
            }
        }
    }
    words_similarity = ((same_words - 1) / (sentence_size - 1)) * 100;
    return words_similarity;
}

/**��ʾ���ƶ���������������**/
void show_max_similaritty_sentences(float & max_similarity, vector<float> all_similarities, string str1)
{
    int pos = 0;
    for(int i = 0; i < all_similarities.size(); i++) {
        if(max_similarity == all_similarities[i]) {
            pos = i;
        }
    }
    /**ֻ��ʾ���ƶȴ���50%�ľ���**/
    if(max_similarity >= 50) {
        string str2 = return_sentence_of_file_two(pos);
        cout << "String1: " << str1 << endl;
        cout << "String2: " << str2 << endl;
    }
}
