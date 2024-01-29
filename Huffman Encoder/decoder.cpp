#include <bits/stdc++.h>
#include <fstream>

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
   map<string, char> decoderMap;

   void buildHuffmanCodes(Node *node, string &path) {
      if (node == NULL) {
         return;
      }

      if (node -> data != '\0') {
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

   void buildMaps(map<char, long long int> frequencyMap) {
      for (auto tmp: frequencyMap) {
         char character = tmp.first;
         long long int frequency = tmp.second;

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

int main() {
   ifstream file("compressed.bin", ios::binary);

   if (!file.is_open()) {
      std::cerr << "Error opening file!" << std::endl;
      return 1;
   }

   map<char, long long int> frequencyMap;

   vector<char> buffer(istreambuf_iterator<char>(file), {});
   
   int size = 0;

   for (int bit = 7; bit >= 0; bit--) {
      bool bbb = buffer[0] & (1 << bit);
      if (bbb) {
         size += 1 << bit;
      }
   }

   int i = 2;
   while (size--) {
      char ch = buffer[i-1];
      long long freq = 0;

      for (int l = 7; l >= 0; l--) {
         for (int k = 7; k >= 0; k--) {
            bool bit = buffer[i] & (1 << k);
            if (bit)
               freq += 1 << (k + (8 * l));
         }
         i++;
      }

      frequencyMap[ch] = freq;

      // i += 2;
      i++;
   }

   // cout << frequencyMap.size() << "\n";
   for (auto tmp: frequencyMap) {
      cout << tmp.first << " " << tmp.second << "\n";
   }

   i--;

   long long int sizeOfData = 0;
   for (int l = 7; l >= 0; l--) {
      for (int k = 7; k >= 0; k--) {
         bool bit = int(buffer[i]) & (1 << k);
         if (bit)
            sizeOfData += 1 << (k + (8 * l));
      }
      i++;
   }
   // cout << "count of bits: " << sizeOfData << "\n";

   string data = "";
   long long int currentBitCount = 0;

   for (; i < buffer.size(); i++) {
      // cout << int(buffer[i]) << "\n";
      for (int j = 7; j >= 0; j--) {
         currentBitCount++;
         if (currentBitCount > sizeOfData) break;
         bool bit = char(buffer[i]) & (1 << j);
         data += bit + '0';
      }
   }

   HuffmanEncoder hf;
   hf.buildMaps(frequencyMap);

   cout << "|||||||||||||||||||||||\n\n";

   for (auto tmp: hf.encoderMap) {
      cout << tmp.first << " " << tmp.second << "\n";
   }

   string decompressedData = hf.decode(data);
   cout << decompressedData;

   ofstream outFile("output.txt", ios::binary);
   for (char cc: decompressedData) {
      outFile.put(cc);
   }

   file.close();
   return 0;
}