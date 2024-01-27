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
      return (a -> freq) - (b -> freq);
   }
};

class HuffmanEncoder {
private:
   priority_queue<Node*, vector<Node*>, comparator> pq;
   map<char, string> encoderMap;
   map<string, char> decoderMap;
   string tmpPath;

   void encodedPath(Node *node, string &path, char target) {
      if (node == NULL) {
         return;
      }

      if (node -> data == target && node -> left == NULL && node -> right == NULL) {
         tmpPath = path;
         return;
      }

      path.push_back('0');
      encodedPath(node -> left, path, target);

      path.pop_back();

      path.push_back('1');
      encodedPath(node -> right, path, target);

      path.pop_back();
   }

public:
   string encode(string data) {
      map<char, int> frequencyMap;
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
      string tmpP = "";
      for (auto tmp: frequencyMap) {
         char character = tmp.first;
         encodedPath(root, tmpP, character);
         encoderMap[character] = tmpPath;
         decoderMap[tmpPath] = character;
      }  

      string encodedString = ""; // inefficient | string not to be used
      for (char c: data) {
         encodedString += encoderMap[c];
      }
      return encodedString;
   }

   string decode(string encodedString) {
      string tmp = "";
      string decodedString = "";
      for (char c: encodedString) {
         tmp += c;
         if (decoderMap.find(tmp) != decoderMap.end()) {
            decodedString += decoderMap[tmp];
            tmp = "";
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
   string encodedString = hf.encode(s);
   cout << encodedString << "\n";

   string decodedString = hf.decode(encodedString);
   cout << decodedString << "\n";
   return 0;
}