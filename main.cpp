#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <map>
#include <cctype> // 包含std::isalpha, std::isspace等
#include <vector>
#include <cctype>
#include <unordered_map>
#include <algorithm>
#include <windows.h> // 包含MAX_PATH和可能需要的其他Windows定义
#include <direct.h>   // 包含_getcwd函数


using HuffmanCodes = std::map<char, std::string>;
using namespace std;

//获取用户输入并保存到data.txt文件
int fop(){
        std::string filename = "data.txt"; // 要保存的文件名
        std::ofstream outfile(filename.c_str()); // 创建一个输出文件流

        // 检查文件是否成功打开
        if (!outfile.is_open()) {
            std::cerr << "无法打开文件1 " << filename << std::endl;
            return 1;
        }

        // 从用户获取数据
        std::string data;
        std::string data_1;
        std::cout << "请输入需要加密的数据（输入'quit'以结束）：" << std::endl;
        while (std::getline(std::cin, data) && data != "quit") {
            // 将数据写入文件
            outfile << data << std::endl;
        }

        // 关闭文件
        outfile.close();

        std::ifstream infile("data.txt"); // 打开以读取的文件

    if (!infile.is_open()) {
        std::cerr << "无法打开文件以读取数据" << std::endl;
        return 1;
    }

    while (std::getline(infile, data)) {
        std::cout << "\n原文数据：" << data << std::endl;
    }

    infile.close(); // 关闭读取文件

        // 获取当前工作目录的绝对路径（Windows）
        char cwd[MAX_PATH];
        if (_getcwd(cwd, sizeof(cwd)) != NULL) {
            std::string current_path = cwd;
            std::string absolute_filename = current_path + '\\' + filename; // 注意使用反斜杠
            std::cout << "原文已成功保存到 " << absolute_filename << "\n"<< std::endl;
        } else {
        std::cerr << "无法获取当前工作目录" << std::endl;
        }

        return 0;
}

// 输出统计字符频率
int calculateFrequencies(const std::string& filename){
    std::map<char, int> freqMap; // 字符频率映射

    // 打开文件
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "无法打开文件2： " << filename << std::endl;
        return 1;
    }

    // 读取并统计字符频率
    char ch;
    while (file.get(ch)) {
        // 忽略非字母字符（可选）
        if (std::isalpha(ch)) {
            // 将大写字母转换为小写（可选，以便不区分大小写）
            ch = std::tolower(ch);
            // 更新字符频率
            freqMap[ch]++;
        }
    }

    // 关闭文件
    file.close();

    // 输出字符频率
    for (std::map<char, int>::const_iterator it = freqMap.begin(); it != freqMap.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    return 0;
}

// 哈夫曼树节点
struct HuffmanNode {
    char ch;        // 字符（对于叶节点）
    unsigned freq;  // 频率
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char c, unsigned f) : ch(c), freq(f), left(nullptr), right(nullptr) {}

    // 比较函数，用于优先队列
    bool operator<(const HuffmanNode& other) const {
        return freq > other.freq; // 使用最大堆
    }
};

// 从文件中读取字符频率
map<char, unsigned> readFrequencies(const string& filename) {
    map<char, unsigned> freqMap;
    ifstream file(filename);
    if (!file) {
        cerr << "无法打开文件3： " << filename << endl;
        exit(EXIT_FAILURE);
    }
    char ch;
    while (file >> ch) {
        if (isalpha(ch)) { // 只考虑字母
            freqMap[tolower(ch)]++;
        }
    }
    file.close();
    return freqMap;
}

// 辅助函数：构建哈夫曼树
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

// 辅助函数：生成哈夫曼编码（递归）
void generateHuffmanCodes(HuffmanNode* root, string code, map<char, string>& huffmanCodes) {
    if (root == nullptr) return;

    if (root->ch != '$') {
        huffmanCodes[root->ch] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// 清理动态分配的内存
void freeHuffmanTree(HuffmanNode* root) {
    if (root == nullptr) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    delete root;
}

// 哈夫曼编码表项
struct HuffmanCodeItem {
    char data;
    std::string code;
};

// 哈夫曼编码表
struct HuffmanCodeTable {
    std::vector<HuffmanCodeItem> items;

    // 辅助函数：根据字符查找哈夫曼编码
    const std::string& findCode(char c) const {
        for (const auto& item : items) {
            if (item.data == c) {
                return item.code;
            }
        }
        // 如果没有找到对应的编码，返回一个默认的编码或抛出异常
        static const std::string DEFAULT_CODE = "??";
        return DEFAULT_CODE;
    }
};

// 哈夫曼解码函数
std::string huffmanDecode(const std::string& encodedString, const std::map<char, std::string>& huffmanCodes) {
    std::string decodedString = "";
    std::string currentCode = "";

    // 遍历编码字符串
    for (char c : encodedString) {
        // 构建当前字符的哈夫曼编码
        currentCode += c;

        // 检查当前编码是否是已知哈夫曼编码的前缀
        for (const auto& pair : huffmanCodes) {
            if (pair.second.find(currentCode) == 0) { // 是前缀
                // 如果当前编码是整个哈夫曼编码，则添加字符到解码字符串
                if (pair.second == currentCode) {
                    decodedString += pair.first;
                    currentCode = ""; // 重置当前编码
                    break; // 找到完整的编码，跳出循环
                }
            }
        }

        // 如果没有找到任何前缀（即当前编码不是任何哈夫曼编码的前缀），说明编码字符串可能损坏了
        if (currentCode.size() > huffmanCodes.rbegin()->second.size()) { // 检查是否超过最长编码长度
            std::cerr << "编码字符串可能损坏，无法继续解码！" << std::endl;
            return ""; // 返回空字符串表示解码失败
        }
    }

    // 如果解码字符串后还有剩余的当前编码（即编码字符串以某个哈夫曼编码的前缀结束），则解码失败（除非它恰好是最后一个字符的编码）
    if (!currentCode.empty() && currentCode != huffmanCodes.rbegin()->second) {
        std::cerr << "编码字符串可能不完整，无法继续解码！" << std::endl;
        return ""; // 返回空字符串表示解码失败
    }

    return decodedString;
}

int main() {
    std::string filename = "data.txt"; // 定义文件名
    string outputFilename = "huffman_codes.txt";

    fop(); // 先保存数据到文件
    //calculateFrequencies(filename); // 输出统计字符频率

    map<char, unsigned> freqMap = readFrequencies(filename);// 从文件中读取字符频率

    // 创建优先队列并填充节点
    priority_queue<HuffmanNode*> pq;
    for (auto& pair : freqMap) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // 构建哈夫曼树
    HuffmanNode* root = buildHuffmanTree(pq);

    // 生成哈夫曼编码
    map<char, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // 输出哈夫曼编码
/*
    for (auto& pair : huffmanCodes) {
        cout << pair.first << ": " << pair.second << endl;
    }
*/

    // 读取data.txt文件并替换字符为哈夫曼编码
    std::ifstream infile(filename);
    std::ofstream outfile(outputFilename, std::ios::binary); // 使用二进制模式以保留编码中的'0'和'1'字符

    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "无法打开文件4" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::string encodedLine;
        for (char c : line) {
            // 忽略非字符（如空格、换行符等），或者为它们添加特定的编码
            if (huffmanCodes.find(c) != huffmanCodes.end()) {
                encodedLine += huffmanCodes[c];
            }
        }
        outfile << encodedLine << std::endl; // 假设每行一个字符串，或者按你的需求修改
        std::cout <<"-----进行加密-----"<< std::endl;
        std::cout << "密文数据："<<encodedLine << std::endl;
    }

    // 获取当前工作目录的绝对路径（Windows）
        char cwd[MAX_PATH];
        if (_getcwd(cwd, sizeof(cwd)) != NULL) {
            std::string current_path = cwd;
            std::string absolute_filename2 = current_path + '\\' + outputFilename; // 注意使用反斜杠
            std::cout << "密文已成功保存到 " << absolute_filename2 << "\n"<< std::endl;
        } else {
        std::cerr << "无法获取当前工作目录" << std::endl;
        }

    //std::cout << "密文已成功保存到 " << outputFilename << std::endl;

    infile.close();
    outfile.close();

    // 从huffman_codes.txt文件中读取密文
    std::string ciphertext;
    std::ifstream file("huffman_codes.txt");
    if (!file.is_open()) {
        std::cerr << "无法打开文件: huffman_codes.txt" << std::endl;
        return 1;
    }

    std::getline(file, ciphertext); // 假设密文只有一行
    file.close();

    // 清理动态分配的内存
    freeHuffmanTree(root);

    //解密
    std::cout << "-----进行解密----- "<< std::endl;
    std::string decodedString = huffmanDecode(ciphertext, huffmanCodes);
    std::cout << "解码后的数据: " << decodedString << std::endl;

    return 0;

}
