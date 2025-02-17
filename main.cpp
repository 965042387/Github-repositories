#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <map>
#include <cctype> // ����std::isalpha, std::isspace��
#include <vector>
#include <cctype>
#include <unordered_map>
#include <algorithm>
#include <windows.h> // ����MAX_PATH�Ϳ�����Ҫ������Windows����
#include <direct.h>   // ����_getcwd����


using HuffmanCodes = std::map<char, std::string>;
using namespace std;

//��ȡ�û����벢���浽data.txt�ļ�
int fop(){
        std::string filename = "data.txt"; // Ҫ������ļ���
        std::ofstream outfile(filename.c_str()); // ����һ������ļ���

        // ����ļ��Ƿ�ɹ���
        if (!outfile.is_open()) {
            std::cerr << "�޷����ļ�1 " << filename << std::endl;
            return 1;
        }

        // ���û���ȡ����
        std::string data;
        std::string data_1;
        std::cout << "��������Ҫ���ܵ����ݣ�����'quit'�Խ�������" << std::endl;
        while (std::getline(std::cin, data) && data != "quit") {
            // ������д���ļ�
            outfile << data << std::endl;
        }

        // �ر��ļ�
        outfile.close();

        std::ifstream infile("data.txt"); // ���Զ�ȡ���ļ�

    if (!infile.is_open()) {
        std::cerr << "�޷����ļ��Զ�ȡ����" << std::endl;
        return 1;
    }

    while (std::getline(infile, data)) {
        std::cout << "\nԭ�����ݣ�" << data << std::endl;
    }

    infile.close(); // �رն�ȡ�ļ�

        // ��ȡ��ǰ����Ŀ¼�ľ���·����Windows��
        char cwd[MAX_PATH];
        if (_getcwd(cwd, sizeof(cwd)) != NULL) {
            std::string current_path = cwd;
            std::string absolute_filename = current_path + '\\' + filename; // ע��ʹ�÷�б��
            std::cout << "ԭ���ѳɹ����浽 " << absolute_filename << "\n"<< std::endl;
        } else {
        std::cerr << "�޷���ȡ��ǰ����Ŀ¼" << std::endl;
        }

        return 0;
}

// ���ͳ���ַ�Ƶ��
int calculateFrequencies(const std::string& filename){
    std::map<char, int> freqMap; // �ַ�Ƶ��ӳ��

    // ���ļ�
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "�޷����ļ�2�� " << filename << std::endl;
        return 1;
    }

    // ��ȡ��ͳ���ַ�Ƶ��
    char ch;
    while (file.get(ch)) {
        // ���Է���ĸ�ַ�����ѡ��
        if (std::isalpha(ch)) {
            // ����д��ĸת��ΪСд����ѡ���Ա㲻���ִ�Сд��
            ch = std::tolower(ch);
            // �����ַ�Ƶ��
            freqMap[ch]++;
        }
    }

    // �ر��ļ�
    file.close();

    // ����ַ�Ƶ��
    for (std::map<char, int>::const_iterator it = freqMap.begin(); it != freqMap.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    return 0;
}

// ���������ڵ�
struct HuffmanNode {
    char ch;        // �ַ�������Ҷ�ڵ㣩
    unsigned freq;  // Ƶ��
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char c, unsigned f) : ch(c), freq(f), left(nullptr), right(nullptr) {}

    // �ȽϺ������������ȶ���
    bool operator<(const HuffmanNode& other) const {
        return freq > other.freq; // ʹ������
    }
};

// ���ļ��ж�ȡ�ַ�Ƶ��
map<char, unsigned> readFrequencies(const string& filename) {
    map<char, unsigned> freqMap;
    ifstream file(filename);
    if (!file) {
        cerr << "�޷����ļ�3�� " << filename << endl;
        exit(EXIT_FAILURE);
    }
    char ch;
    while (file >> ch) {
        if (isalpha(ch)) { // ֻ������ĸ
            freqMap[tolower(ch)]++;
        }
    }
    file.close();
    return freqMap;
}

// ����������������������
HuffmanNode* buildHuffmanTree(priority_queue<HuffmanNode*>& pq) {
    if (pq.size() == 1) {
        HuffmanNode* root = pq.top();
        pq.pop();
        return root;
    }

    HuffmanNode* left = pq.top(); pq.pop();
    HuffmanNode* right = pq.top(); pq.pop();

    HuffmanNode* top = new HuffmanNode('$', left->freq + right->freq);
    top->left = left;
    top->right = right;

    pq.push(top);
    return buildHuffmanTree(pq);
}

// �������������ɹ��������루�ݹ飩
void generateHuffmanCodes(HuffmanNode* root, string code, map<char, string>& huffmanCodes) {
    if (root == nullptr) return;

    if (root->ch != '$') {
        huffmanCodes[root->ch] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// ����̬������ڴ�
void freeHuffmanTree(HuffmanNode* root) {
    if (root == nullptr) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    delete root;
}

// �������������
struct HuffmanCodeItem {
    char data;
    std::string code;
};

// �����������
struct HuffmanCodeTable {
    std::vector<HuffmanCodeItem> items;

    // ���������������ַ����ҹ���������
    const std::string& findCode(char c) const {
        for (const auto& item : items) {
            if (item.data == c) {
                return item.code;
            }
        }
        // ���û���ҵ���Ӧ�ı��룬����һ��Ĭ�ϵı�����׳��쳣
        static const std::string DEFAULT_CODE = "??";
        return DEFAULT_CODE;
    }
};

// ���������뺯��
std::string huffmanDecode(const std::string& encodedString, const std::map<char, std::string>& huffmanCodes) {
    std::string decodedString = "";
    std::string currentCode = "";

    // ���������ַ���
    for (char c : encodedString) {
        // ������ǰ�ַ��Ĺ���������
        currentCode += c;

        // ��鵱ǰ�����Ƿ�����֪�����������ǰ׺
        for (const auto& pair : huffmanCodes) {
            if (pair.second.find(currentCode) == 0) { // ��ǰ׺
                // �����ǰ�������������������룬������ַ��������ַ���
                if (pair.second == currentCode) {
                    decodedString += pair.first;
                    currentCode = ""; // ���õ�ǰ����
                    break; // �ҵ������ı��룬����ѭ��
                }
            }
        }

        // ���û���ҵ��κ�ǰ׺������ǰ���벻���κι����������ǰ׺����˵�������ַ�����������
        if (currentCode.size() > huffmanCodes.rbegin()->second.size()) { // ����Ƿ񳬹�����볤��
            std::cerr << "�����ַ��������𻵣��޷��������룡" << std::endl;
            return ""; // ���ؿ��ַ�����ʾ����ʧ��
        }
    }

    // ��������ַ�������ʣ��ĵ�ǰ���루�������ַ�����ĳ�������������ǰ׺�������������ʧ�ܣ�������ǡ�������һ���ַ��ı��룩
    if (!currentCode.empty() && currentCode != huffmanCodes.rbegin()->second) {
        std::cerr << "�����ַ������ܲ��������޷��������룡" << std::endl;
        return ""; // ���ؿ��ַ�����ʾ����ʧ��
    }

    return decodedString;
}

int main() {
    std::string filename = "data.txt"; // �����ļ���
    string outputFilename = "huffman_codes.txt";

    fop(); // �ȱ������ݵ��ļ�
    //calculateFrequencies(filename); // ���ͳ���ַ�Ƶ��

    map<char, unsigned> freqMap = readFrequencies(filename);// ���ļ��ж�ȡ�ַ�Ƶ��

    // �������ȶ��в����ڵ�
    priority_queue<HuffmanNode*> pq;
    for (auto& pair : freqMap) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // ������������
    HuffmanNode* root = buildHuffmanTree(pq);

    // ���ɹ���������
    map<char, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // �������������
/*
    for (auto& pair : huffmanCodes) {
        cout << pair.first << ": " << pair.second << endl;
    }
*/

    // ��ȡdata.txt�ļ����滻�ַ�Ϊ����������
    std::ifstream infile(filename);
    std::ofstream outfile(outputFilename, std::ios::binary); // ʹ�ö�����ģʽ�Ա��������е�'0'��'1'�ַ�

    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "�޷����ļ�4" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::string encodedLine;
        for (char c : line) {
            // ���Է��ַ�����ո񡢻��з��ȣ�������Ϊ��������ض��ı���
            if (huffmanCodes.find(c) != huffmanCodes.end()) {
                encodedLine += huffmanCodes[c];
            }
        }
        outfile << encodedLine << std::endl; // ����ÿ��һ���ַ��������߰���������޸�
        std::cout <<"-----���м���-----"<< std::endl;
        std::cout << "�������ݣ�"<<encodedLine << std::endl;
    }

    // ��ȡ��ǰ����Ŀ¼�ľ���·����Windows��
        char cwd[MAX_PATH];
        if (_getcwd(cwd, sizeof(cwd)) != NULL) {
            std::string current_path = cwd;
            std::string absolute_filename2 = current_path + '\\' + outputFilename; // ע��ʹ�÷�б��
            std::cout << "�����ѳɹ����浽 " << absolute_filename2 << "\n"<< std::endl;
        } else {
        std::cerr << "�޷���ȡ��ǰ����Ŀ¼" << std::endl;
        }

    //std::cout << "�����ѳɹ����浽 " << outputFilename << std::endl;

    infile.close();
    outfile.close();

    // ��huffman_codes.txt�ļ��ж�ȡ����
    std::string ciphertext;
    std::ifstream file("huffman_codes.txt");
    if (!file.is_open()) {
        std::cerr << "�޷����ļ�: huffman_codes.txt" << std::endl;
        return 1;
    }

    std::getline(file, ciphertext); // ��������ֻ��һ��
    file.close();

    // ����̬������ڴ�
    freeHuffmanTree(root);

    //����
    std::cout << "-----���н���----- "<< std::endl;
    std::string decodedString = huffmanDecode(ciphertext, huffmanCodes);
    std::cout << "����������: " << decodedString << std::endl;

    return 0;

}
