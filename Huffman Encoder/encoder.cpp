#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

struct Node {
   char data;
   long long int freq;
   Node *left;
   Node *right;
};

struct comparator {
   bool operator()(Node *a, Node *b) {
      return (a -> freq) > (b -> freq);
   }
};

class HuffmanEncoder {
public:
   priority_queue<Node*, vector<Node*>, comparator> pq;
   map<char, string> encoderMap;
   map<char, long long int> frequencyMap;

   void buildHuffmanCodes(Node *node, string &path) {
      if (node == NULL) {
         return;
      }

      if (node -> data != '\0') {
         encoderMap[node -> data] = path;
         return;
      }

      path.push_back('0');
      buildHuffmanCodes(node -> left, path);

      path.pop_back();

      path.push_back('1');
      buildHuffmanCodes(node -> right, path);

      path.pop_back();
   }

   string buildHuffmanTree_And_Encode(string &data) {
      for (char c: data) {
         frequencyMap[c]++;
      }

      for (auto tmp: frequencyMap) {
         char character = tmp.first;
         long long frequency = tmp.second;

         Node *node = (Node*) malloc(sizeof(Node));
         node -> data = character;
         node -> freq = frequency;
         node -> left = NULL;
         node -> right = NULL;
         pq.push(node);
      }

      while (pq.size() > 1) {
         Node *top1 = pq.top();
         pq.pop();

         Node *top2 = pq.top();
         pq.pop();

         Node *node = (Node*) malloc(sizeof(Node));
         node -> data = '\0';
         node -> freq = (top1 -> freq) + (top2 -> freq);
         node -> left = top2;
         node -> right = top1;
         pq.push(node);
      }

      Node *root = pq.top();
      string tmp = "";
      buildHuffmanCodes(root, tmp);

      string encodedString;
      for (char c: data) {
         encodedString += encoderMap[c];
      }
      return encodedString;
   }
};

void createCompressedBinary(string compressedData) {
   ofstream outFile("compressed.bin", ios::binary);
   string byte = "";

   for (char c : compressedData) {
      byte += c;
      if (byte.size() == 8) {
         char ch = stoi(byte, nullptr, 2);
         outFile.put(ch);
         byte.clear();
      }
   }

   if (!byte.empty()) {
      while (byte.size() < 8) {
         byte += "0";
      }
      char ch = stoi(byte, nullptr, 2);
      outFile.put(ch);
   }
   outFile.close();
}

string readFromFile(const string& filename) {
   ifstream file(filename, ios::binary);
   if (!file.is_open()) {
      cerr << "Error opening file for reading: " << filename << endl;
      return "";
   }
   string data((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
   file.close();
   return data;
}

string numberToBinaryString(long long int number, int byteCount) {
   string s = "";
   while (number > 0) {
      bool rem = number % 2;
      number /= 2;

      s += '0' + rem;
   }

   while (s.size() < (byteCount * 8)) {
      s.push_back('0');
   }

   reverse(s.begin(), s.end());
   return s;
}

int main() {
   /**
    * Map Data
      Count of unique characters ===> 1 byte
      character 1 ===> 1 byte
      frequency of character 1 ===> 8 bytes
      ....
    
    * Main Data
      total size ===> 8 bytes
      1101010101010........
   */ 
  
   string data = readFromFile("data.txt");

   HuffmanEncoder hf;
   string compressedData = hf.buildHuffmanTree_And_Encode(data);

   int countOfUniqueCharacters = hf.frequencyMap.size();
   string mapSize = numberToBinaryString(countOfUniqueCharacters, 1);

   string mapData = "";
   for (auto tmp: hf.frequencyMap) {
      int ch = tmp.first - '\0';
      int f = tmp.second;

      string key = numberToBinaryString(ch, 1);
      string freq = numberToBinaryString(f, 8);
      mapData += key + freq;
   }

   string dataSize_BitCount = numberToBinaryString(compressedData.size(), 8);

   string binaryData = mapSize + mapData + dataSize_BitCount + compressedData;

   createCompressedBinary(binaryData);
   return 0;
}