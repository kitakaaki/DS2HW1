/*11327155 黃宇謙*/
/*二、作業內容
資料檔格式簡述：
(1) 取自教育部統計處的大學校系畢業生資訊，前三列為標題和欄名，第四列起為各校系某
班制的畢業生資訊，共 11 個欄位，以定位符號（'/t'）間隔，依「學校代碼」遞增排序。
(2) 欄位由左至右依序為：學校代碼、學校名稱、科系代碼、科系名稱、日間∕進修別、等
級別、學生數、教師數、上學年度畢業生數、縣市名稱、體系別，資料檔名如 input101.txt。
(3) 按照原始的存檔次序為每筆資料附上從 1 號開始的唯一『序號』。
規範：每個任務違反一項各扣 5 分
(1) 預先不知道資料筆數，禁止使用固定筆數的靜態陣列，必須採用動態陣列或 vector。
(2) 必須為主要資料結構定義專屬的 C++類別，至少要有二種指定堆積專用的類別
整合下列任務於單一程式，並遵循範例程式的輸入輸出介面，未整合、無法連續執行或沒有
輸入防呆措施，都各扣 5 分。若導致程式檢測系統無法正常運作，該任務以零分計。
必須(而且只限)出現在任務選單第一列：* Data Structures and Algorithms *
（任務一）建立最大堆積 max heap
輸入：原始資料檔，資料檔名如 input101.txt。
步驟：
(1) 每筆資料附上唯一序號後，以『上學年度畢業生數』建立一棵最大堆積，依照序號由小
到大一筆一筆新增至原有的堆積結構中，每個節點只存放一筆資料對應的（序號、上學
年度畢業生數）。
(2) 找到最大堆積的樹根，底部節點（bottom），以及最左下角的節點。
輸出：分別顯示前述三個節點對應的（序號、上學年度畢業生數）於螢幕上。
（任務二）建立雙堆積 DEAP
輸入：原始資料檔，資料檔名如 input101.txt。
步驟：
(1) 以『上學年度畢業生數』再建立一棵雙堆積，依照任務一所附加的序號由小到大逐筆新
增至原有的堆積結構，每個節點存放一筆資料對應的（序號、上學年度畢業生數）。
(2) 找到雙堆積的（右下角）底部節點（bottom）及最左下角的節點。
輸出：分別顯示前述三個節點對應的（序號、上學年度畢業生數）於螢幕上。*/
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

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
      if (!heap.empty()) {
        return heap[heap.size() / 2];
      }
      return std::make_pair(-1, -1);
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
      while (pos > 0 && deap[pos].second < deap[(pos - 1) / 2].second) {
        std::swap(deap[pos], deap[(pos - 1) / 2]);
        pos = (pos - 1) / 2;
      }
    }

    void ReheapUpMax(int pos) {
      while (pos > 0 && deap[pos].second > deap[(pos - 1) / 2].second) {
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
      //  Examine the corresponding nodes
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
      // ReheapUp if necessary
      if (isInMinHeap(pos)) {
        ReheapUpMin(pos);
      } else {
        ReheapUpMax(pos);
      }
    }

     std::pair<int, int> getMax() {
      if (!deap.empty()) {
        return deap[0];
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
      if (!deap.empty()) {
        return deap[deap.size() / 2];
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
        std::getline(linestream, data.schoolCode, '\t');
        std::getline(linestream, data.schoolName, '\t');
        std::getline(linestream, data.departmentCode, '\t');
        std::getline(linestream, data.departmentName, '\t');
        std::getline(linestream, data.dayOrNight, '\t');
        std::getline(linestream, data.level, '\t');
        linestream >> data.students >> data.teachers >> data.graduates;
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
          std::cout << std::endl << "Command does not exist!" << std::endl;
        }
      }
    }
};

int main() {
    system::run();
    return 0;
} 
