#include <bits/stdc++.h>
#include <fstream>

using namespace std;

struct Node {
   char data;
   int freq;
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

   void buildMaps(unordered_map<char, int> frequencyMap) {
      for (auto tmp: frequencyMap) {
         char character = tmp.first;
         int frequency = tmp.second;

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

std::string negativeIntToBinary(int number) {
    // Convert the negative number to its Two's Complement representation
    unsigned int twosComplement = static_cast<unsigned int>(number);
    twosComplement = ~twosComplement + 1; // Calculate Two's Complement

    // Convert the Two's Complement representation to 8-bit binary string
    return std::bitset<32>(twosComplement).to_string().substr(24); // Extract last 8 bits
}

std::string positiveIntToBinary(unsigned int number) {
    // Convert the positive number to binary string using std::bitset with size 8
    return std::bitset<8>(number).to_string();
}

int main() {
   ifstream file("compressed.bin", ios::binary);

   if (!file.is_open()) {
      std::cerr << "Error opening file!" << std::endl;
      return 1;
   }

   HuffmanEncoder hf;

   map<char, int> frequencyMap;

   vector<char> buffer(istreambuf_iterator<char>(file), {});
   
   int size = buffer[0];
   int i = 2;
   while (size--) {
      char ch = buffer[i-1];
      int freq = buffer[i];

      frequencyMap[ch] = freq;

      i += 2;
   }

   cout << frequencyMap.size() << "\n";
   for (auto tmp: frequencyMap) {
      cout << tmp.first << " " << tmp.second << "\n";
   }

   i--;

   int sizeOfData = 0;
   for (int l = 3; l >= 0; l--) {
      for (int k = 7; k >= 0; k--) {
         bool bit = int(buffer[i]) & (1 << k);
         if (bit)
            sizeOfData += 1 << (k + (8 * l));
      }
      i++;
   }
   cout << "count of bits: " << sizeOfData << "\n";

   string data = "";
   int currentBitCount = 0;

   for (; i < buffer.size(); i++) {
      // cout << int(buffer[i]) << "\n";
      for (int j = 7; j >= 0; j--) {
         currentBitCount++;
         if (currentBitCount > sizeOfData) break;
         bool bit = char(buffer[i]) & (1 << j);
         data += bit + '0';
      }
   }

   cout << data << "\n";

   file.close();
   return 0;
}