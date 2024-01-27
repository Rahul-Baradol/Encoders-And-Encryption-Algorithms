#include <bits/stdc++.h>

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
   unordered_map<char, vector<bool>> encoderMap;
   map<vector<bool>, char> decoderMap;

   void buildHuffmanCodes(Node *node, vector<bool> &path) {
      if (node == NULL) {
         return;
      }

      if (node -> data != '$') {
         encoderMap[node -> data] = path;
         decoderMap[path] = node -> data;
         return;
      }

      path.push_back(0);
      buildHuffmanCodes(node -> left, path);

      path.pop_back();

      path.push_back(1);
      buildHuffmanCodes(node -> right, path);

      path.pop_back();
   }

   vector<bool> encode(string data) {
      unordered_map<char, int> frequencyMap;
      for (char c: data) {
         frequencyMap[c]++;
      }

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
         node -> left = top1;
         node -> right = top2;
         pq.push(node);
      }

      Node *root = pq.top();
      vector<bool> tmp;
      buildHuffmanCodes(root, tmp);

      vector<bool> encodedString;
      for (char c: data) {
         for (bool bit: encoderMap[c]) {
            encodedString.push_back(bit);
         }
      }
      return encodedString;
   }

   string decode(vector<bool> encodedString) {
      vector<bool> tmp;
      string decodedString = "";
      for (bool c: encodedString) {
         tmp.push_back(c);
         if (decoderMap.find(tmp) != decoderMap.end()) {
            decodedString += decoderMap[tmp];
            tmp = vector<bool>();
         }
      }

      return decodedString;
   }
};

int main() {
   int numberOfWords;
   cin >> numberOfWords;

   string s = "";
   for (int i = 0; i < numberOfWords; i++) {
      string tmp;
      cin >> tmp;
      if (i != 0)
         s += " ";
      s += tmp;
   }

   HuffmanEncoder hf;

   cout << "Compressed Form: ";
   vector<bool> encodedString = hf.encode(s);

   for (bool ele: encodedString) {
      cout << ele;
   }
   cout << "\n";

   string decodedString = hf.decode(encodedString);
   cout << "Decompressed Form: " << decodedString << "\n";

   cout << "\n";

   cout << "Size of source data: " << s.size() * 8 << "\n";
   cout << "Size of compressed data: " << encodedString.size() << "\n";
   return 0;
}