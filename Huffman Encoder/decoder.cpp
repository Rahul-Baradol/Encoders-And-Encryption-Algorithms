#include <iostream>
#include <map>
#include <vector>
#include <queue>
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

class HuffmanDecoder {
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

   void buildHuffmanTree(map<char, long long int> &frequencyMap) {
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
      cerr << "Error opening file!" << endl;
      return 1;
   }

   map<char, long long int> frequencyMap;

   vector<char> buffer(istreambuf_iterator<char>(file), {});
   
   int frequencyMapSize = 0;

   // First byte is frequency map's size
   for (int bitPosition = 7; bitPosition >= 0; bitPosition--) {
      bool bit = buffer[0] & (1 << bitPosition);
      if (bit) {
         frequencyMapSize += 1 << bitPosition;
      }
   }

   int bufferPtr = 2;
   while (frequencyMapSize--) {
      char ch = buffer[bufferPtr-1];
      long long int freq = 0;

      for (int byte = 7; byte >= 0; byte--) {
         for (int bitPosition = 7; bitPosition >= 0; bitPosition--) {
            bool bit = buffer[bufferPtr] & (1 << bitPosition);
            if (bit) {
               freq += 1 << (bitPosition + (8 * byte));
            }
         }
         bufferPtr++;
      }

      frequencyMap[ch] = freq;

      bufferPtr++;
   }

   bufferPtr--;

   long long int dataSize = 0;
   for (int byte = 7; byte >= 0; byte--) {
      for (int bitPosition = 7; bitPosition >= 0; bitPosition--) {
         bool bit = buffer[bufferPtr] & (1 << bitPosition);
         if (bit) {
            dataSize += 1 << (bitPosition + (8 * byte));
         }
      }
      bufferPtr++;
   }

   string data = "";
   long long int currentBitCount = 0;

   for (; bufferPtr < buffer.size(); bufferPtr++) {
      for (int bitPosition = 7; bitPosition >= 0; bitPosition--) {
         currentBitCount++;
         if (currentBitCount > dataSize) break;
         bool bit = buffer[bufferPtr] & (1 << bitPosition);
         data += bit + '0';
      }
   }

   HuffmanDecoder hf;
   hf.buildHuffmanTree(frequencyMap);

   string decompressedData = hf.decode(data);

   ofstream outFile("output.txt", ios::binary);
   for (char cc: decompressedData) {
      outFile.put(cc);
   }

   outFile.close();
   file.close();  
   return 0;
}