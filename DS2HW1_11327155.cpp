/*11327155 黃宇謙*/
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

struct Data {
    std::string schoolCode; // 學校代碼
    std::string schoolName; // 學校名稱
    std::string departmentCode; // 科系代碼
    std::string departmentName; // 科系名稱
    std::string dayOrNight; // 日間∕進修別
    std::string level; // 等級別
    int students; // 學生數
    int teachers; // 教師數
    int graduates; // 上學年度畢業生數
    std::string city; // 縣市名稱
    std::string system; // 體系別
    int index; // 序號(後產生)
};

class MaxHeap {
  private:
    std::vector<std::pair<int, int>> heap; // (序號, 上學年度畢業生數)

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

     std::pair<int, int> getMax() {
      if (!deap.empty()) {
        return deap[2];
      }
      return std::make_pair(-1, -1);
    }

    std::pair<int, int> getBottom() {
      if (!deap.empty()) {
        return deap[deap.size() - 1];
      }
      return std::make_pair(-1, -1);
    }

    std::pair<int, int> getLeftBottom() {
      if (deap.empty())
        return std::make_pair(-1, -1);
      int pos = 0;
      while (2 * pos + 1 < deap.size()) {
        pos = 2 * pos + 1;
      }
      return deap[pos];
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
        std::getline(linestream, data.schoolCode, '\t');
        std::getline(linestream, data.schoolName, '\t');
        std::getline(linestream, data.departmentCode, '\t');
        std::getline(linestream, data.departmentName, '\t');
        std::getline(linestream, data.dayOrNight, '\t');
        std::getline(linestream, data.level, '\t');
        std::string studentStr;
        std::getline(linestream, studentStr, '\t');
        studentStr.erase(std::remove(studentStr.begin(), studentStr.end(), ','), studentStr.end());
        studentStr.erase(std::remove(studentStr.begin(), studentStr.end(), '"'), studentStr.end());
        data.students = std::stoi(studentStr); // "1,032" or 1032 -> 1032
        linestream >> data.teachers >> data.graduates;
        linestream.ignore(); // 跳過graduates後的tab
        std::getline(linestream, data.city, '\t');
        std::getline(linestream, data.system);
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
};

class system {
  private:
    static void mission1() {
      std::string filenumber;
      std::vector<Data> dataList = IO::readData(filenumber);
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
      std::vector<Data> dataList = IO::readData(filenumber);
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

  public:
    static void run() {
      while (true) {
        std::cout << "* Data Structures and Algorithms *" << std::endl;
        std::cout << "*** Heap Construction and Use ****" << std::endl;
        std::cout << "* 0. QUIT                        *" << std::endl;
        std::cout << "* 1. Build a max heap            *" << std::endl;
        std::cout << "* 2. Build a DEAP                *" << std::endl;
        std::cout << "***************************** ****" << std::endl;
        std::cout << "Input a choice(0, 1, 2): ";
        std::string choice;
        std::cin >> choice;
        if (choice == "0") {
          break;
        } else if (choice == "1") {
          mission1();
        } else if (choice == "2") {
          mission2();
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
