/*11327155 黃宇謙*/
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>

struct Data {
    std::string schoolName; // 學校名稱
    std::string departmentName; // 科系名稱
    std::string dayOrNight; // 日間∕進修別
    std::string level; // 等級別
    int graduates; // 上學年度畢業生數
    int index; // 序號(後產生)
};

class MaxHeap {
  private:
    std::vector<std::pair<int, int>> heap; // (序號, 上學年度畢業生數)

    void ReheapDown(int pos) {
      int size = heap.size();
      while (true) {
        int left = 2 * pos + 1;
        int right = 2 * pos + 2;
        int largest = pos;
        if (left < size && heap[left].second > heap[largest].second) {
          largest = left;
        }
        if (right < size && heap[right].second > heap[largest].second) {
          largest = right;
        }
        if (largest != pos) {
          std::swap(heap[pos], heap[largest]);
          pos = largest;
        } else {
          break;
        }
      }
    }
  public:
    void insert(int index, int graduates) {
      heap.push_back(std::make_pair(index, graduates));
      int pos = heap.size() - 1;
      while (pos > 0 && heap[pos].second > heap[(pos - 1) / 2].second) {
        std::swap(heap[pos], heap[(pos - 1) / 2]);
        pos = (pos - 1) / 2;
      }
    }

    std::pair<int, int> getMax() {
      if (!heap.empty()) {
        return heap[0];
      }
      return std::make_pair(-1, -1);
    }

    std::pair<int, int> getBottom() {
      if (!heap.empty()) {
        return heap[heap.size() - 1];
      }
      return std::make_pair(-1, -1);
    }

    std::pair<int, int> getLeftBottom() {
      if (heap.empty())
        return std::make_pair(-1, -1);
      int pos = 0;
      while (2 * pos + 1 < heap.size()) {
        pos = 2 * pos + 1;
      }
      return heap[pos];
    }

};

class DEAP {
  private:
    std::vector<std::pair<int, int>> deap; // (序號, 上學年度畢業生數)

    bool isInMinHeap(int pos) {
      int posLevel = std::floor(std::log2(pos + 1));
      int leftOfMaxHeap = std::pow(2, posLevel - 1) * 3 - 1;
      return pos < leftOfMaxHeap;
    }

    void ReheapUpMin(int pos) {
      while (pos > 1 && deap[pos].second < deap[(pos - 1) / 2].second) {
        std::swap(deap[pos], deap[(pos - 1) / 2]);
        pos = (pos - 1) / 2;
      }
    }

    void ReheapUpMax(int pos) {
      while (pos > 2 && deap[pos].second > deap[(pos - 1) / 2].second) {
        std::swap(deap[pos], deap[(pos - 1) / 2]);
        pos = (pos - 1) / 2;
      }
    }

    int findCorrespondingNode(int pos) {
      int posLevel = std::floor(std::log2(pos + 1));
      int offset = std::pow(2, posLevel - 1);
      int corPos;
      if (isInMinHeap(pos)) {
        corPos = pos + offset;
      } else {
        corPos = pos - offset;
      }
      while (corPos >= deap.size()) {
        corPos = (corPos - 1) / 2;
      }
      return corPos;
    } 
  public:
    DEAP() {
      deap.push_back(std::make_pair(0, -1)); // index: 0
    }

    void insert(int index, int graduates) {
      deap.push_back(std::make_pair(index, graduates));
      int pos = deap.size() - 1;
      int corPos = findCorrespondingNode(pos);
      //  Examine the corresponding nodes (skip if corPos hit the dummy at index 0)
      if (corPos > 0) {
        if (isInMinHeap(pos)) {
          if (deap[pos].second > deap[corPos].second) {
            std::swap(deap[pos], deap[corPos]);
            pos = corPos;
          }
        } else {
          if (deap[pos].second < deap[corPos].second) {
            std::swap(deap[pos], deap[corPos]);
            pos = corPos;
          }
        }
      }
      // ReheapUp if necessary
      if (isInMinHeap(pos)) {
        ReheapUpMin(pos);
      } else {
        ReheapUpMax(pos);
      }
    }

    std::pair<int, int> getBottom() {
      if (deap.size() > 1) {
        return deap[deap.size() - 1];
      }
      return std::make_pair(-1, -1);
    }

    std::pair<int, int> getLeftBottom() {
      if (deap.size() <= 1)
        return std::make_pair(-1, -1);
      int pos = 0;
      while (2 * pos + 1 < deap.size()) {
        pos = 2 * pos + 1;
      }
      return deap[pos];
    }

};

class MinMaxHeap {
  private:
    std::vector<std::pair<int, int>> minMaxHeap; // (序號, 上學年度畢業生數)

    int level(int pos) {
      return std::floor(std::log2(pos + 1));
    }

    bool largerForMax(int cand, int best) {
      if (minMaxHeap[cand].second != minMaxHeap[best].second) {
        return minMaxHeap[cand].second > minMaxHeap[best].second;
      }
      // Assignment rule: if equal on the same level, pick the left node first.
      if (level(cand) == level(best)) {
        return cand < best;
      }
      return false;
    }

    int parent(int pos) {
      return (pos - 1) / 2;
    }

    int grandparent(int pos) {
      return (pos - 3) / 4;
    }

    bool isGrandchild(int node, int descendant) {
      return descendant >= 4 * node + 3;
    }

    void reheapUpMin(int pos) {
      while (pos >= 3) {
        int gp = grandparent(pos);
        if (minMaxHeap[pos].second < minMaxHeap[gp].second) {
          std::swap(minMaxHeap[pos], minMaxHeap[gp]);
          pos = gp;
        } else {
          break;
        }
      }
    }

    void reheapUpMax(int pos) {
      while (pos >= 3) {
        int gp = grandparent(pos);
        if (minMaxHeap[pos].second > minMaxHeap[gp].second) {
          std::swap(minMaxHeap[pos], minMaxHeap[gp]);
          pos = gp;
        } else {
          break;
        }
      }
    }

    int findSmallestDescendant(int pos) {
      int size = minMaxHeap.size();
      int candidates[6] = {2 * pos + 1, 2 * pos + 2, 4 * pos + 3, 4 * pos + 4, 4 * pos + 5, 4 * pos + 6};
      int smallest = -1;
      for (int i = 0; i < 6; ++i) {
        int index = candidates[i];
        if (index >= size)
          continue;
        if (smallest == -1 || minMaxHeap[index].second < minMaxHeap[smallest].second)
          smallest = index;
      }
      return smallest;
    }

    int findLargestDescendant(int pos) {
      int size = minMaxHeap.size();
      int candidates[6] = {2 * pos + 1, 2 * pos + 2, 4 * pos + 3, 4 * pos + 4, 4 * pos + 5, 4 * pos + 6};
      int largest = -1;
      for (int i = 0; i < 6; ++i) {
        int index = candidates[i];
        if (index >= size)
          continue;
        if (largest == -1 || largerForMax(index, largest))
          largest = index;
      }
      return largest;
    }

    void reheapDownMin(int pos) {
      while (true) {
        int smallest = findSmallestDescendant(pos);
        if (smallest == -1)
          break;
        if (isGrandchild(pos, smallest)) {
          if (minMaxHeap[smallest].second < minMaxHeap[pos].second) {
            std::swap(minMaxHeap[smallest], minMaxHeap[pos]);
            int p = parent(smallest);
            if (minMaxHeap[smallest].second > minMaxHeap[p].second) {
              std::swap(minMaxHeap[smallest], minMaxHeap[p]);
            }
            pos = smallest;
          } else {
            break;
          }
        } else {
          if (minMaxHeap[smallest].second < minMaxHeap[pos].second) {
            std::swap(minMaxHeap[smallest], minMaxHeap[pos]);
          }
          break;
        }
      }
    }

    void reheapDownMax(int pos) {
      while (true) {
        int largest = findLargestDescendant(pos);
        if (largest == -1)
          break;
        if (isGrandchild(pos, largest)) {
          if (minMaxHeap[largest].second > minMaxHeap[pos].second) {
            std::swap(minMaxHeap[largest], minMaxHeap[pos]);
            int p = parent(largest);
            if (minMaxHeap[largest].second < minMaxHeap[p].second) {
              std::swap(minMaxHeap[largest], minMaxHeap[p]);
            }
            pos = largest;
          } else {
            break;
          }
        } else {
          if (minMaxHeap[largest].second > minMaxHeap[pos].second) {
            std::swap(minMaxHeap[largest], minMaxHeap[pos]);
          }
          break;
        }
      }
    }

    bool inMinLevel(int pos) {
      int level = std::floor(std::log2(pos + 1));
      return level % 2 == 0;
    }
  public:
    void insert(int index, int graduates) {
      minMaxHeap.push_back(std::make_pair(index, graduates));
      int pos = minMaxHeap.size() - 1;
      if (pos == 0)
        return;

      int p = parent(pos);
      if (inMinLevel(pos)) {
        if (minMaxHeap[pos].second > minMaxHeap[p].second) {
          std::swap(minMaxHeap[pos], minMaxHeap[p]);
          reheapUpMax(p);
        } else {
          reheapUpMin(pos);
        }
      } else {
        if (minMaxHeap[pos].second < minMaxHeap[p].second) {
          std::swap(minMaxHeap[pos], minMaxHeap[p]);
          reheapUpMin(p);
        } else {
          reheapUpMax(pos);
        }
      }
    }
    
    void deleteMax() {
      if (minMaxHeap.empty())
        return;
      if (minMaxHeap.size() == 1) {
        minMaxHeap.pop_back();
        return;
      }
      int finalPos = 1;
      if (minMaxHeap.size() > 2 && largerForMax(2, 1))
        finalPos = 2;

      minMaxHeap[finalPos] = minMaxHeap.back();
      minMaxHeap.pop_back();
      if (finalPos < (int)minMaxHeap.size())
        reheapDownMax(finalPos);
    }

    void clear() {
      minMaxHeap.clear();
    }

    int size() {
      return minMaxHeap.size();
    }

    bool empty() {
      return minMaxHeap.empty();
    }

    std::pair<int, int> getMin() {
      if (!minMaxHeap.empty()) {
        return minMaxHeap[0];
      }
      return std::make_pair(-1, -1);
    }

    std::pair<int, int> getBottom() {
      if (!minMaxHeap.empty()) {
        return minMaxHeap[minMaxHeap.size() - 1];
      }
      return std::make_pair(-1, -1);
    }

    std::pair<int, int> getLeftBottom() {
      if (minMaxHeap.empty())
        return std::make_pair(-1, -1);
      int pos = 0;
      while (2 * pos + 1 < minMaxHeap.size()) {
        pos = 2 * pos + 1;
      }
      return minMaxHeap[pos];
    }

    std::pair<int, int> getMax() {
      if (minMaxHeap.size() > 2) {
        return largerForMax(2, 1) ? minMaxHeap[2] : minMaxHeap[1];
      } else if (minMaxHeap.size() > 1) {
        return minMaxHeap[1];
      } else if (!minMaxHeap.empty()) {
        return minMaxHeap[0];
      }
      return std::make_pair(-1, -1);
    }
};

class IO {
  public:
    static std::vector<Data> readData(const std::string& filenumber) {  
      std::vector<Data> dataList;
      std::string filename = "input" + filenumber + ".txt";
      std::ifstream file(filename);
      if (!file.is_open()) 
        return dataList;
      std::string header;
      for (int i = 0; i < 3; ++i) {
        std::getline(file, header);
      }
      std::string line;
      int index = 1;
      while (std::getline(file, line)) {
        Data data;
        std::istringstream linestream(line);
        std::string unused;
        std::getline(linestream, unused, '\t'); // schoolCode
        std::getline(linestream, data.schoolName, '\t');
        std::getline(linestream, unused, '\t'); // departmentCode
        std::getline(linestream, data.departmentName, '\t');
        std::getline(linestream, data.dayOrNight, '\t');
        std::getline(linestream, data.level, '\t');
        std::getline(linestream, unused, '\t'); // students
        int unusedTeachers;
        linestream >> unusedTeachers >> data.graduates;
        linestream.ignore(); // skip tab after graduates
        std::getline(linestream, unused, '\t'); // city
        std::getline(linestream, unused); // system
        data.index = index++;
        dataList.push_back(data);
      }
      return dataList;
    }

    static void printNode(const std::pair<int, int>& node) {
      if (node.first != -1) {
        std::cout <<  "[" << node.first << "] " << node.second << std::endl;
      } else {
        std::cout << "no such node!" << std::endl;
      }
    }

    static void printData(const Data& data, int order) {
      std::cout << "Top" << std::setw(4) << std::right << order << ": [" << data.index << "] " << data.schoolName << data.departmentName << ", " 
                << data.dayOrNight << ", " << data.level << ", " << data.graduates << std::endl;
    }
};

class system {
  private:
    static void mission1() {
      std::string filenumber;
      std::vector<Data> dataList;
      while (true) {
        std::cout << std::endl << "Input a file number ([0] Quit): ";
        std::cin >> filenumber;
        if (filenumber == "0") {
          std::cout << std::endl;
          return;
        }
        dataList = IO::readData(filenumber);
        if (dataList.empty()) {
          std::cout << std::endl << "### input" << filenumber << ".txt does not exist! ###" << std::endl;
        } else {
          break;
        }
      }
      MaxHeap maxHeap;
      for (const auto& data : dataList) {
        maxHeap.insert(data.index, data.graduates);
      }
      std::cout << "<max heap>" << std::endl << "root: ";
      IO::printNode(maxHeap.getMax());
      std::cout << "bottom: ";
      IO::printNode(maxHeap.getBottom());
      std::cout << "leftmost bottom: ";
      IO::printNode(maxHeap.getLeftBottom());
      std::cout << std::endl;
    }

    static void mission2() {
      std::string filenumber;
      std::vector<Data> dataList;
      while (true) {
        std::cout << std::endl << "Input a file number ([0] Quit): ";
        std::cin >> filenumber;
        if (filenumber == "0") {
          std::cout << std::endl;
          return;
        }
        dataList = IO::readData(filenumber);
        if (dataList.empty()) {
          std::cout << std::endl << "### input" << filenumber << ".txt does not exist! ###" << std::endl;
        } else {
          break;
        }
      }
      DEAP deap;
      for (const auto& data : dataList) {
        deap.insert(data.index, data.graduates);
      }
      std::cout << "<DEAP>" << std::endl << "bottom: ";
      IO::printNode(deap.getBottom());
      std::cout << "leftmost bottom: ";
      IO::printNode(deap.getLeftBottom());
      std::cout << std::endl;
    }

    static void mission3(MinMaxHeap& minMaxHeap, std::vector<Data>& minMaxHeapDataList) {
      std::string filenumber;
      while (true) {
        std::cout << std::endl << "Input a file number ([0] Quit): ";
        std::cin >> filenumber;
        if (filenumber == "0") {
          std::cout << std::endl;
          return;
        }
        minMaxHeapDataList = IO::readData(filenumber);
        if (minMaxHeapDataList.empty()) {
          std::cout << std::endl << "### input" << filenumber << ".txt does not exist! ###" << std::endl;
        } else {
          break;
        }
      }
      minMaxHeap.clear();
      for (const auto& data : minMaxHeapDataList) {
        minMaxHeap.insert(data.index, data.graduates);
      }
      std::cout << "<min-max heap>" << std::endl << "root: ";
      IO::printNode(minMaxHeap.getMin());
      std::cout << "bottom: ";
      IO::printNode(minMaxHeap.getBottom());
      std::cout << "leftmost bottom: ";
      IO::printNode(minMaxHeap.getLeftBottom());
      std::cout << std::endl;
    }

    static void mission4(MinMaxHeap& minMaxHeap, std::vector<Data>& minMaxHeapDataList) {
        if (minMaxHeapDataList.empty() || minMaxHeap.empty()) {
          std::cout << std::endl << "### Execute command 3 first! ###" << std::endl << std::endl;
          return;
        }
        std::string K;
        std::cout << std::endl << "Enter the value of K in [1,"<< minMaxHeap.size() << "]: ";
        std::cin >> K;
        try {
          int k = std::stoi(K);
          if (k < 1 || k > minMaxHeap.size()) {
            throw std::out_of_range("\n### The value of K is out of range! ###\n");
          }
          for (int i = 1; i <= k && !minMaxHeap.empty(); ++i) {
            std::pair<int, int> maxNode = minMaxHeap.getMax();
            if (maxNode.first > 0 && maxNode.first <= (int)minMaxHeapDataList.size()) {
              IO::printData(minMaxHeapDataList[maxNode.first - 1], i);
            }
            minMaxHeap.deleteMax();
          }
          if (minMaxHeap.empty()) {
            minMaxHeapDataList.clear();
          }
        } catch (const std::exception& e) {
          std::cout << "\n### The value of K is out of range! ###\n";
        }
        std::cout << std::endl;
    }
  public:
    static void run() {
      MinMaxHeap minMaxHeap;
      std::vector<Data> minMaxHeapDataList;
      while (true) {
        std::cout << "* Data Structures and Algorithms *" << std::endl;
        std::cout << "*** Heap Construction and Use ****" << std::endl;
        std::cout << "* 0. QUIT                        *" << std::endl;
        std::cout << "* 1. Build a max heap            *" << std::endl;
        std::cout << "* 2. Build a DEAP                *" << std::endl;
        std::cout << "* 3. Build a min-max heap        *" << std::endl;
        std::cout << "* 4: Top-K max from min-max heap *" << std::endl;
        std::cout << "**********************************" << std::endl;
        std::cout << "Input a choice(0, 1, 2, 3, 4): ";
        std::string choice;
        std::cin >> choice;
        if (choice == "0") {
          break;
        } else if (choice == "1") {
          mission1();
        } else if (choice == "2") {
          mission2();
        } else if (choice == "3") {
          mission3(minMaxHeap, minMaxHeapDataList);
        } else if (choice == "4") {
          mission4(minMaxHeap, minMaxHeapDataList);
        } else {
          std::cout << std::endl << "Command does not exist!" << std::endl << std::endl;
        }
      }
    }
};

int main() {
    system::run();
    return 0;
} 
