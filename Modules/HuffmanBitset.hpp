#include <vector>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <algorithm>

class Node {
public:
    char data;
    int freq;
    Node *left, *right;
    Node(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

class Huffman {
public:
    std::unordered_map<char, std::string> codes;
    Node* root;

    Huffman() : root(nullptr) {}

    void buildTree(const std::string& input) {
        std::unordered_map<char, int> freqs;
        for (char ch : input) {
            freqs[ch]++;
        }

        std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

        for (auto pair : freqs) {
            pq.push(new Node(pair.first, pair.second));
        }

        while (pq.size() > 1) {
            Node* left = pq.top();
            pq.pop();

            Node* right = pq.top();
            pq.pop();

            int sum = left->freq + right->freq;
            Node* merge = new Node('\0', sum);
            merge->left = left;
            merge->right = right;

            pq.push(merge);
        }

        root = pq.top();

        generateCodes(root, "");
    }

    void generateCodes(Node* node, std::string str) {
        if (node == nullptr) return;

        if (!node->left && !node->right) {
            codes[node->data] = str;
        }

        generateCodes(node->left, str + "0");
        generateCodes(node->right, str + "1");
    }

/*    std::string encode(const std::string& input) {
        std::string encoded = "";
        for (char ch : input) {
            encoded += codes[ch];
        }

        if (encoded.size() >= input.size() * 8) {
            return input;
        }

        return encoded;
    }

    std::string decode(const std::string& input) {
        if (input.empty() || root == nullptr) return "";
        if (input.size() >= root->freq * 8) {
            return input;
        }
        Node* curr = root;
        std::string decoded = "";
        for (char ch : input) {
            if (ch == '0') curr = curr->left;
            else curr = curr->right;

            if (!curr->left && !curr->right) {
                decoded += curr->data;
                curr = root;
            }
        }
        return decoded;
    }
    */
    bool hasAllSameCharacters(const std::string& str) {
        return std::all_of(str.begin(), str.end(), [firstChar = str[0]](char ch) {
            return ch == firstChar;
        });
    }

    std::vector<uint8_t> encode(const std::string& input) {
        if (hasAllSameCharacters(input)) {
            return std::vector<uint8_t>(input.begin(), input.end());
        }
        std::string encoded = "";
        for (char ch : input) {
            encoded += codes[ch];
        }

        std::vector<uint8_t> bytes;
        int index = 0;
        uint8_t byte = 0;
        for (char ch : encoded) {
            byte |= (ch == '1') << (7 - index);
            index++;
            if (index == 8) {
                bytes.push_back(byte);
                byte = 0;
                index = 0;
            }
        }
        if (index != 0) {
            bytes.push_back(byte);
        }

        return bytes;
    }
    
    std::string trimToOriginalSize(const std::string& original, const std::string& decoded) {
        if (decoded.size() > original.size()) {
            return decoded.substr(0, original.size());
        }
        return decoded;
    }

    std::string decode(const std::vector<uint8_t>& inputBytes) {
        if (inputBytes.empty() || root == nullptr) return "";
        if (inputBytes.size() == root->freq) {
            return std::string(inputBytes.begin(), inputBytes.end());
        }
        std::string input = "";
        for (uint8_t byte : inputBytes) {
            for (int i = 7; i >= 0; i--) {
                input += ((byte >> i) & 1) ? '1' : '0';
            }
        }

        Node* curr = root;
        std::string decoded = "";
        for (char ch : input) {
            if (ch == '0') curr = curr->left;
            else curr = curr->right;

            if (!curr->left && !curr->right) {
                decoded += curr->data;
                curr = root;
            }
        }
        return decoded;
    }
};

