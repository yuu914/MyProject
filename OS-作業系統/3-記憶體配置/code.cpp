#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <unordered_set>
#include <sstream>
#include <limits>
#include <string>
#include <algorithm>
#include <map>
#include <iomanip>

using namespace std;

void readFile(const string &fileName, int &method, int &pageFrameSize, vector<int> &pageReferences) ;
void writeFile(const string &inputFileName, const vector<int> &pageReferences, int pageFaultCount, int pageReplacementCount, const vector<bool> &pageFault, const vector<vector<int>> &frameStates, int pageFrameSize, int method) ;

void FIFO(int pageFrameSize, const vector<int> &pageReferences, int &pageFaultCount, int &pageReplacementCount, vector<bool> &pageFault, vector<vector<int>> &frameStates) ;
void LRU(int pageFrameSize, const vector<int> &pageReferences, int &pageFaultCount, int &pageReplacementCount, vector<bool> &pageFault, vector<vector<int>> &frameStates) ;
void LFU_FIFO(int pageFrameSize, const vector<int> &pageReferences, int &pageFaultCount, int &pageReplacementCount, vector<bool> &pageFault, vector<vector<int>> &frameStates) ;
void MFU_FIFO(int pageFrameSize, const vector<int> &pageReferences, int &pageFaultCount, int &pageReplacementCount, vector<bool> &pageFault, vector<vector<int>> &frameStates) ;
void LFU_LRU(int pageFrameSize, const vector<int> &pageReferences, int &pageFaultCount, int &pageReplacementCount, vector<bool> &pageFault, vector<vector<int>> &frameStates) ;


void readFile(const string &fileName, int &method, int &pageFrameSize, vector<int> &pageReferences) {
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        cout << "Error opening file" << endl;
        exit(1);
    }

    inputFile >> method >> pageFrameSize;

    // Skip the rest of the first line if necessary
    inputFile.ignore(numeric_limits<streamsize>::max(), '\n');

    // Read the second line for page references
    string pageReferencesLine;
    getline(inputFile, pageReferencesLine);

    stringstream ss(pageReferencesLine);
    int page;
    
    // Read the page references from the stringstream
    while (ss >> page) {
        pageReferences.push_back(page);
        // To handle any type of delimiter (space, tab, etc.)
        if (ss.peek() == ' ' || ss.peek() == '\t') {
            ss.ignore();
        }
    }

    // If there are no spaces or tabs in the second line, read characters one by one
    if (pageReferences.empty()) {
        for (char ch : pageReferencesLine) {
            if (isdigit(ch)) {
                pageReferences.push_back(ch - '0');
            }
        }
    }

    inputFile.close();
}

void writeFile(const string &inputFileName, const vector<int> &pageReferences, int pageFaultCount, int pageReplacementCount, const vector<bool> &pageFault, const vector<vector<int>> &frameStates, int pageFrameSize, int method) {
    string outputFileName = "out_" + inputFileName;
    ofstream outputFile(outputFileName, ios::app);  // 使用 append 模式

    if (!outputFile.is_open()) {
        cout << "Error opening output file" << endl;
        exit(1);
    }

    switch (method) {
        case 1: outputFile << "--------------FIFO-----------------------" << endl; break;
        case 2: outputFile << "--------------LRU-----------------------" << endl; break;
        case 3: outputFile << "--------------Least Frequently Used Page Replacement-----------------------" << endl; break;
        case 4: outputFile << "--------------Most Frequently Used Page Replacement-----------------------" << endl; break;
        case 5: outputFile << "--------------Least Frequently Used LRU Page Replacement-----------------------" << endl; break;
        default: outputFile << "Unknown Method" << endl; break;
    }

    for (size_t i = 0; i < pageReferences.size(); ++i) {
        outputFile << pageReferences[i] << "\t\t";
        int q;
        for (int page : frameStates[i]) {
            q=i;
            outputFile << page ;
        }

        if(frameStates[q].size() < pageFrameSize) {
            for (int j = 0; j < pageFrameSize - frameStates[q].size(); ++j) {
                outputFile << " ";
            }
        }

        if (pageFault[i]) {
            outputFile << "\t\tF";
        }
        outputFile << endl;
    }

    outputFile << "Page Fault = " << pageFaultCount << "  Page Replaces = " << pageReplacementCount << "  Page Frames = " << pageFrameSize << endl;

    outputFile.close();
}

void FIFO(int pageFrameSize, const vector<int> &pageReferences, int &pageFaultCount, int &pageReplacementCount, vector<bool> &pageFault, vector<vector<int>> &frameStates) {
    deque<int> pageQueue;
    unordered_set<int> pagesInQ;  // 用來查找 QUEUE 中是否有該 PAGE

    pageFaultCount = 0;
    pageReplacementCount = 0;

    for (int pageRef : pageReferences) {
        bool pageFaultOccurred = (pagesInQ.find(pageRef) == pagesInQ.end());

        if (pageFaultOccurred) { // 代表這個 Page 不存在於 queue 中
            pageFaultCount++;
            pageFault.push_back(true);
            if (pageQueue.size() == static_cast<deque<int>::size_type>(pageFrameSize)) {
                // Remove the oldest page
                int oldestPage = pageQueue.front();
                pageQueue.pop_front();
                pagesInQ.erase(oldestPage);
                // Record page replacement
                pageReplacementCount++;
            }
            // Add the new page
            pageQueue.push_back(pageRef);
            pagesInQ.insert(pageRef);
        } else {
            pageFault.push_back(false);
        }

        // 記錄當前的 Frame 狀態
        vector<int> currentFrameState(pageQueue.begin(), pageQueue.end());
        reverse(currentFrameState.begin(), currentFrameState.end());
        frameStates.push_back(currentFrameState);
    }
} // FIFO

void LRU(int pageFrameSize, const vector<int> &pageReferences, int &pageFaultCount, int &pageReplacementCount, vector<bool> &pageFault, vector<vector<int>> &frameStates) {
    deque<int> pageQueue;
    unordered_set<int> pagesInQ;  // 用來查找 QUEUE 中是否有該 PAGE

    pageFaultCount = 0;
    pageReplacementCount = 0;

    for (int pageRef : pageReferences) {
        bool pageFaultOccurred = (pagesInQ.find(pageRef) == pagesInQ.end());

        if (pageFaultOccurred) { // 代表這個 Page 不存在於 queue 中
            pageFaultCount++;
            pageFault.push_back(true);
            if (pageQueue.size() == static_cast<deque<int>::size_type>(pageFrameSize)) {
                // Remove the oldest page
                int oldestPage = pageQueue.front();
                pageQueue.pop_front();
                pagesInQ.erase(oldestPage);
                // Record page replacement
                pageReplacementCount++;
            }
            // Add the new page
            pageQueue.push_back(pageRef);
            pagesInQ.insert(pageRef);
        } else {
            // 找到並移除已存在的頁面
            auto it = find(pageQueue.begin(), pageQueue.end(), pageRef);
            if (it != pageQueue.end()) {
                pageQueue.erase(it);
            }
            // 重新插入到頁面隊列的尾部
            pageQueue.push_back(pageRef);
            pageFault.push_back(false);
        }

        // 記錄當前的 Frame 狀態
        vector<int> currentFrameState(pageQueue.begin(), pageQueue.end());
        reverse(currentFrameState.begin(), currentFrameState.end());
        frameStates.push_back(currentFrameState);
    }
} // LRU

void LFU_FIFO(int pageFrameSize, const vector<int> &pageReferences, int &pageFaultCount, int &pageReplacementCount, vector<bool> &pageFault, vector<vector<int>> &frameStates) {
    deque<int> pageQueue;
    unordered_set<int> pagesInQ;
    map<int, int> pageFrequency;

    pageFaultCount = 0;
    pageReplacementCount = 0;

    for (int pageRef : pageReferences) {
        bool pageFaultOccurred = (pagesInQ.find(pageRef) == pagesInQ.end());

        if (pageFaultOccurred) {
            pageFaultCount++;
            pageFault.push_back(true);
            if (pageQueue.size() == static_cast<deque<int>::size_type>(pageFrameSize)) {
                int lfuPage = pageQueue.front();
                for (int page : pageQueue) {
                    if (pageFrequency[page] < pageFrequency[lfuPage]) {
                        lfuPage = page;
                    } /* else if (pageFrequency[page] == pageFrequency[lfuPage]) {
                        // 如果頻率相同，選擇首次出現次序最小的頁面
                        if (pageOrder[page] < pageOrder[lfuPage]) {
                            lfuPage = page;
                        }
                        
                    }*/
                }
                pageQueue.erase(remove(pageQueue.begin(), pageQueue.end(), lfuPage), pageQueue.end());
                pagesInQ.erase(lfuPage);
                pageFrequency.erase(lfuPage);
                pageReplacementCount++;
            }
            pageQueue.push_back(pageRef);
            pagesInQ.insert(pageRef);
            pageFrequency[pageRef]++;
        } else {
            pageFault.push_back(false);
            pageFrequency[pageRef]++;
        }

        vector<int> currentFrameState(pageQueue.begin(), pageQueue.end());
        reverse(currentFrameState.begin(), currentFrameState.end());
        frameStates.push_back(currentFrameState);
    }
} // LFU_FIFO



void MFU_FIFO(int pageFrameSize, const vector<int> &pageReferences, int &pageFaultCount, int &pageReplacementCount, vector<bool> &pageFault, vector<vector<int>> &frameStates) {
    deque<int> pageQueue;
    unordered_set<int> pagesInQ;
    map<int, int> pageFrequency;

    pageFaultCount = 0;
    pageReplacementCount = 0;

    for (int pageRef : pageReferences) {
        bool pageFaultOccurred = (pagesInQ.find(pageRef) == pagesInQ.end());

        if (pageFaultOccurred) {
            pageFaultCount++;
            pageFault.push_back(true);
            if (pageQueue.size() == static_cast<deque<int>::size_type>(pageFrameSize)) {
                int mfuPage = pageQueue.front();
                for (int page : pageQueue) {
                    if (pageFrequency[page] > pageFrequency[mfuPage]) {
                        mfuPage = page;
                    } /*else if (pageFrequency[page] == pageFrequency[mfuPage]) {
                        // 如果頻率相同，選擇首次出現次序最小的頁面
                        if (pageOrder[page] < pageOrder[mfuPage]) {
                            mfuPage = page;
                        }
                    }*/
                }
                pageQueue.erase(remove(pageQueue.begin(), pageQueue.end(), mfuPage), pageQueue.end());
                pagesInQ.erase(mfuPage);
                pageFrequency.erase(mfuPage);
                pageReplacementCount++;
            }
            pageQueue.push_back(pageRef);
            pagesInQ.insert(pageRef);
            pageFrequency[pageRef]++;
        } else {
            pageFault.push_back(false);
            pageFrequency[pageRef]++;
        }

        vector<int> currentFrameState(pageQueue.begin(), pageQueue.end());
        reverse(currentFrameState.begin(), currentFrameState.end());
        frameStates.push_back(currentFrameState);
    }
}

void LFU_LRU(int pageFrameSize, const vector<int> &pageReferences, int &pageFaultCount, int &pageReplacementCount, 
    vector<bool> &pageFault, vector<vector<int>> &frameStates) {
    deque<int> pageQueue;
    unordered_set<int> pagesInQ;
    map<int, int> pageFrequency;

    pageFaultCount = 0;
    pageReplacementCount = 0;

    for (int pageRef : pageReferences) {
        bool pageFaultOccurred = (pagesInQ.find(pageRef) == pagesInQ.end());
        if (pageFaultOccurred) {
            pageFaultCount++;
            pageFault.push_back(true);
            if (pageQueue.size() == static_cast<deque<int>::size_type>(pageFrameSize)) {
                // 找到頻率最小的頁面
                int lfuPage = pageQueue.front();
                for (int page : pageQueue) {
                    if (pageFrequency[page] < pageFrequency[lfuPage]) {
                        lfuPage = page;
                    }
                }
                pageQueue.erase(remove(pageQueue.begin(), pageQueue.end(), lfuPage), pageQueue.end());
                pagesInQ.erase(lfuPage);
                pageFrequency.erase(lfuPage);
                pageReplacementCount++;
            }
            pageQueue.push_back(pageRef);
            pagesInQ.insert(pageRef);
            pageFrequency[pageRef]++;
        } else {
            // 找到並移除已存在的頁面
            auto it = find(pageQueue.begin(), pageQueue.end(), pageRef);
            if (it != pageQueue.end()) {
                pageQueue.erase(it);
            }
            // 重新插入到頁面隊列的尾部
            pageQueue.push_back(pageRef);
            pageFault.push_back(false);
            pageFrequency[pageRef]++;
        }

        vector<int> currentFrameState(pageQueue.begin(), pageQueue.end());
        reverse(currentFrameState.begin(), currentFrameState.end());
        frameStates.push_back(currentFrameState);
    }
}


int main() {
    string fileName;
    cout << "Enter the input file name: ";
    cin >> fileName;

    int method, pageFrameSize;
    vector<int> pageReferences;

    readFile(fileName, method, pageFrameSize, pageReferences);

    if ( method > 7 || method < 1 ) {
        cout << "Error input method number" << endl;
        return 0;
    }

    int pageFaultCount = 0;
    int pageReplacementCount = 0;
    vector<bool> pageFault;
    vector<vector<int>> frameStates;

    if( method == 1 ) FIFO(pageFrameSize, pageReferences, pageFaultCount, pageReplacementCount, pageFault, frameStates);
    if( method == 2 ) LRU(pageFrameSize, pageReferences, pageFaultCount, pageReplacementCount, pageFault, frameStates) ;
    if( method == 3 ) LFU_FIFO(pageFrameSize, pageReferences, pageFaultCount, pageReplacementCount, pageFault, frameStates);
    if( method == 4 ) MFU_FIFO(pageFrameSize, pageReferences, pageFaultCount, pageReplacementCount, pageFault, frameStates);
    if( method == 5 ) LFU_LRU(pageFrameSize, pageReferences, pageFaultCount, pageReplacementCount, pageFault, frameStates);

    if (method == 6) {
        vector<int> pageRefVec(5);
        vector<int> replacementCounts(5);
        vector<vector<bool>> pageFaults(5);
        vector<vector<vector<int>>> states(5);

        FIFO(pageFrameSize, pageReferences, pageRefVec[0], replacementCounts[0], pageFaults[0], states[0]);
        LRU(pageFrameSize, pageReferences, pageRefVec[1], replacementCounts[1], pageFaults[1], states[1]);
        LFU_FIFO(pageFrameSize, pageReferences, pageRefVec[2], replacementCounts[2], pageFaults[2], states[2]);
        MFU_FIFO(pageFrameSize, pageReferences, pageRefVec[3], replacementCounts[3], pageFaults[3], states[3]);
        LFU_LRU(pageFrameSize, pageReferences, pageRefVec[4], replacementCounts[4], pageFaults[4], states[4]);

        for (int i = 0; i < 5; ++i) {
            writeFile(fileName, pageReferences, pageRefVec[i], replacementCounts[i], pageFaults[i], states[i], pageFrameSize, i + 1);
        }
    } // if method == 6

    else  writeFile(fileName, pageReferences, pageFaultCount, pageReplacementCount, pageFault, frameStates, pageFrameSize, method);
    
    cout << "generate Output file Successfully" << endl;

    return 0;
}
