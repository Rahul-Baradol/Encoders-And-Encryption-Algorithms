#include <bits/stdc++.h>

using namespace std;

struct Node {
   char data;
   long long freq;
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
   unordered_map<char, string> encoderMap;
   unordered_map<string, char> decoderMap;
   unordered_map<char, long long> frequencyMap;

   void buildHuffmanCodes(Node *node, string &path) {
      if (node == NULL) {
         return;
      }

      if (node -> data != '$') {
         encoderMap[node -> data] = path;
         decoderMap[path] = node -> data;
         return;
      }

      path.push_back('0');
      buildHuffmanCodes(node -> left, path);

      path.pop_back();

      path.push_back('1');
      buildHuffmanCodes(node -> right, path);

      path.pop_back();
   }

   string encode(string data) {
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
         node -> data = '$';
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

   string decode(string encodedString) {
      string tmp = "";
      string decodedString = "";
      for (char c: encodedString) {
         tmp.push_back(c);
         if (decoderMap.find(tmp) != decoderMap.end()) {
            decodedString += decoderMap[tmp];
            tmp = "";
         }
      }

      return decodedString;
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
      while (byte.size() < 8)
         byte += "0";
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

string numberToBinaryString(long long number, int size) {
   string s = "";
   while (number > 0) {
      bool rem = number % 2;
      number /= 2;

      s += '0' + rem;
   }

   while (s.size() < size) {
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
      frequency of character 1 ===> 1 byte
      ....
    
    * Main Data
      total size ===> 4 byte
      1101010101010........
   */

   string data = readFromFile("data.txt");

   HuffmanEncoder hf;

   string compressedData = hf.encode(data);

   // int i = 1;
   // for (auto tmp: hf.encoderMap) {
   //    cout << i << " " << tmp.first << " " << tmp.second << "\n";
   //    i++;
   // }

   int countOfUniqueCharacters = hf.frequencyMap.size();
   string mapData = numberToBinaryString(countOfUniqueCharacters, 8);

   // cout << countOfUniqueCharacters << "\n";
   // cout << mapData << "\n";

   cout << hf.frequencyMap.size() << "\n";
   for (auto tmp: hf.frequencyMap) {
      int ch = tmp.first - '\0';
      int f = tmp.second;

      cout << tmp.first << " " << tmp.second << "\n";

      string key = numberToBinaryString(ch, 8);
      string freq = numberToBinaryString(f, 8);
      mapData += key + freq;
   }

   string countOfBitsInData = numberToBinaryString(compressedData.size(), 32);

   string binaryData = mapData + countOfBitsInData + compressedData;

   cout << compressedData.size() << "\n";
   cout << compressedData << "\n";

   createCompressedBinary(binaryData);
   return 0;
}