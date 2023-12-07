// main.cpp : By Ayobami Usman
// Description: A program comparing searching in Arrays & Linked Lists
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include <chrono>
#include <vector>

using namespace std;


FILE *gameFilePointer = NULL;
GAME_ *parseGame(char *game);
int generateRandomNumber(int max);
void printGameItem(GAME_ *game);
void swap(GAME_ *a, GAME_ *b);
GAME_ *linearSearch(const vector<GAME_ *> &gamesArray, const char *targetName);
GAME_ *binarySearch(const vector<GAME_ *> &gamesArray, const char *targetName);
void insertionSort(vector<GAME_ *> &gamesArray);
int partition(vector<GAME_ *> &gamesArray, int low, int high);
void quickSort(vector<GAME_ *> &gamesArray, int low, int high);
long long getTimeForLinearSearch(char *gameName, vector<GAME_ *> games);
long long getTimeForBinarySearch(char *gameName, vector<GAME_ *> games);


class DoublyLinkedList
{
public:
    node *head, *tail;
    int length;

    DoublyLinkedList(GAME_ *game)
    {
        this->head = game != NULL ? new node(game) : NULL;
        this->tail = this->head;
        this->length = game != NULL ? 1 : 0;
    }

    int size()
    {
        return length;
    }

    bool empty()
    {
        return size() <= 0;
    }

    void insertTail(node *new_node)
    {
        if (empty())
        {
            head = new_node;
            tail = new_node;
            length = length + 1;
        }
        else
        {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
            length = length + 1;
        }
    };

    void deleteTail()
    {
        if (empty())
        {
            cout << "An attempt to delete was made while Linked List is empty";
        }
        if (size() == 1)
        {
            delete (head);
            head = NULL;
            tail = NULL;
            length = 0;
        }
        else
        {
            tail = tail->prev;
            tail->next->prev = NULL;
            delete (tail->next); // delete operator instead of free to deallocate memory for the deleted nodes
            tail->next = NULL;
            length = length - 1;
        }
    }

    void insertHead(node *new_node)
    {
        if (empty())
        {
            head = new_node;
            tail = new_node;
            length = length + 1;
        }
        else
        {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
            length = length + 1;
        }
    };

    void deleteHead()
    {
        if (empty())
        {
            cout << "An attempt to delete was made while Linked List is empty";
        }
        if (size() == 1)
        {
            delete (head);
            head = NULL;
            tail = NULL;
            length = 0;
        }
        else
        {
            head = head->next;
            head->prev->next = NULL;
            delete (head->prev); // delete operator instead of free to deallocate memory for the deleted nodes
            head->prev = NULL;
            length = length - 1;
        }
    }

    node *findNodeByIndex(int index)
    {
        if (empty())
        {
            return NULL;
        }
        if (index > length || index < 0)
        {
            return NULL;
        }
        node *currentNode = this->head;
        int counter = 0;
        while (counter != index)
        {
            currentNode = currentNode->next;
            counter++;
        }
        return currentNode;
    }

    vector<GAME_ *> toArray()
    {

        vector<GAME_ *> games;
        node *current = head;
        while (current != NULL)
        {
            games.push_back(current->game);
            current = current->next;
        }

        return games;
    }

    void printList()
    {
        node *current = head;
        while (current != NULL)
        {
            printGameItem(current->game);
            current = current->next;
        }
    }

    // Linear search
    node *findNodeByGameName(char *gameName)
    {
        if (empty())
        {
            return NULL;
        }
        node *currentNode = this->head;
        while (currentNode != NULL)
        {
            if (strcmp(currentNode->game->name, gameName) == 0)
            {
                return currentNode;
            }
            currentNode = currentNode->next;
        }
        return NULL;
    }

    void remove(node *target_node)
    {
        if (target_node != NULL)
        {
            if (target_node == head)
            {

                deleteHead();
            }
            else if (target_node == tail)
            {

                deleteTail();
            }
            else
            {
                node *prev = target_node->prev;
                node *next = target_node->next;
                prev->next = next;
                next->prev = prev;
                delete (target_node);
                length = length - 1;
            }
        }
    }
};


int main()
{
    gameFilePointer = fopen("games.csv", "r");
    DoublyLinkedList *gamesLinkedList = new DoublyLinkedList(NULL);

    char *line = NULL;
    size_t linecap = 0;
    ssize_t nbytes;
    int count = 0;
    /*
    1) Read in the row from the given input file and store them in a doubly-linked list, called gamesLinkedList.
     */
    while ((nbytes = getline(&line, &linecap, gameFilePointer)) > 0)
    {
        char *line_copy = line;
        line_copy[strlen(line) - 1] = 0;
        if (count > 0) // To skip the header
        {
            // Create a data structure to represent each game.
            GAME_ *game = parseGame(line_copy);
            // Store game in linkedList
            node *newGameNode = new node(game);
            // If there exist any duplicates based on name, store one with the higher “user rating count” only.
            node *duplicate = gamesLinkedList->findNodeByGameName(newGameNode->game->name);
            if (duplicate != NULL)
            {
                if (newGameNode->game->user_rating_count > duplicate->game->user_rating_count)
                {
                    // remove duplicate
                    gamesLinkedList->remove(duplicate);
                    // insert newGameNode
                    gamesLinkedList->insertTail(newGameNode);
                }
            }
            else
            {
                gamesLinkedList->insertTail(newGameNode);
            }
        }
        count++;
    }

    /*
    Randomly choose an item from the original linked list and then perform linear search based on the name of the item you chose.
    Report the item details you are searching for. Report the time spent searching for the item.
    */

    char *randomItem1 = gamesLinkedList->findNodeByIndex(generateRandomNumber(gamesLinkedList->length))->game->name;
    char *randomItem2 = gamesLinkedList->findNodeByIndex(generateRandomNumber(gamesLinkedList->length))->game->name;
    char *randomItem3 = gamesLinkedList->findNodeByIndex(generateRandomNumber(gamesLinkedList->length))->game->name;

    cout << "Number of elements in LinkedList: " << gamesLinkedList->length << endl
         << endl;
    cout << "*** Linear Search Test ***" << endl
         << endl;

    vector<GAME_ *> gamesArray = gamesLinkedList->toArray();
    // A copy for insertion sort
    vector<GAME_ *> gamesArrayForInsertionSort = gamesArray;

    cout << "Before sorting:" << endl;
    for (int i = 0; i < 5; i++)
    {
        printGameItem(gamesArray[i]);
    }
    cout << endl;
    cout << "Search number 1:" << endl;
    cout << "Searching for " << randomItem1 << "..." << endl;
    int lsTime1 = getTimeForLinearSearch(randomItem1, gamesArray);
    cout << "Single search time: " << lsTime1 << " nanoseconds." << endl;
    int totalLStime1 = 0;
    for (int i = 0; i < 5; i++)
    {
        int lsTimei = getTimeForLinearSearch(randomItem1, gamesArray);
        totalLStime1 = totalLStime1 + lsTimei;
    }
    int averageLSTime1 = (totalLStime1 + lsTime1) / 6;
    cout << "Average search time: " << averageLSTime1 << " nanoseconds." << endl
         << endl;

    cout << "Search number 2:" << endl;
    cout << "Searching for " << randomItem2 << "..." << endl;
    int lsTime2 = getTimeForLinearSearch(randomItem2, gamesArray);
    cout << "Single search time: " << lsTime2 << " nanoseconds." << endl;
    int totalLSTime2 = 0;
    for (int i = 0; i < 5; i++)
    {
        int lsTimei = getTimeForLinearSearch(randomItem2, gamesArray);
        totalLSTime2 = totalLSTime2 + lsTimei;
    }
    int averageLSTime2 = (totalLSTime2 + lsTime2) / 6;
    cout << "Average search time: " << averageLSTime2 << " nanoseconds." << endl
         << endl;

    cout << "Search number 3:" << endl;
    cout << "Searching for " << randomItem3 << "..." << endl;
    int lsTime3 = getTimeForLinearSearch(randomItem3, gamesArray);
    cout << "Single search time: " << lsTime3 << " nanoseconds." << endl;
    int totalLSTime3 = 0;
    for (int i = 0; i < 5; i++)
    {
        int lsTimei = getTimeForLinearSearch(randomItem3, gamesArray);
        totalLSTime3 = totalLSTime3 + lsTimei;
    }
    int averageLSTime3 = (totalLSTime3 + lsTime3) / 6;
    cout << "Average search time: " << averageLSTime3 << " nanoseconds." << endl
         << endl;

    // Record the time for insertion sort
    auto insertion_start_time = chrono::high_resolution_clock::now();
    // Perform insertion sort
    insertionSort(gamesArrayForInsertionSort);
    auto insertion_end_time = chrono::high_resolution_clock::now();
    auto insertion_duration = chrono::duration_cast<chrono::nanoseconds>(insertion_end_time - insertion_start_time);

    // Record the time for quicksort
    auto quicksort_start_time = chrono::high_resolution_clock::now();
    // Perform quicksort on a copy of the list
    quickSort(gamesArray, 0, gamesArray.size() - 1);
    auto quicksort_end_time = chrono::high_resolution_clock::now();
    auto quicksort_duration = chrono::duration_cast<chrono::nanoseconds>(quicksort_end_time - quicksort_start_time);

    cout << "After sorting:" << endl;
    for (int i = 0; i < 5; i++)
    {
        printGameItem(gamesArray[i]);
    }
    cout << endl;

    cout << "Time for insertion sort: " << insertion_duration.count() << " nanoseconds" << endl;
    cout << "Time for quick sort: " << quicksort_duration.count() << " nanoseconds" << endl
         << endl;

    cout << "*** Binary Search Test ***" << endl
         << endl;

    cout << "Search number 1:" << endl;
    cout << "Searching for " << randomItem1 << "..." << endl;
    int bsTime1 = getTimeForBinarySearch(randomItem1, gamesArray);
    cout << "Single search time: " << bsTime1 << " nanoseconds." << endl;
    int totalbsTime1 = 0;
    for (int i = 0; i < 5; i++)
    {
        int timeSpenti = getTimeForBinarySearch(randomItem1, gamesArray);
        totalbsTime1 = totalbsTime1 + timeSpenti;
    }
    int averageAfterSort1 = (totalbsTime1 + bsTime1) / 6;
    cout << "Average search time: " << averageAfterSort1 << " nanoseconds." << endl
         << endl;

    cout << "Search number 2:" << endl;
    cout << "Searching for " << randomItem2 << "..." << endl;
    int bsTime2 = getTimeForBinarySearch(randomItem2, gamesArray);
    cout << "Single search time: " << bsTime2 << " nanoseconds." << endl;
    int totalbsTime2 = 0;
    for (int i = 0; i < 5; i++)
    {
        int timeSpenti = getTimeForBinarySearch(randomItem2, gamesArray);
        totalbsTime2 = totalbsTime2 + timeSpenti;
    }
    int averageAfterSort2 = (totalbsTime2 + bsTime2) / 6;
    cout << "Average search time: " << averageAfterSort2 << " nanoseconds." << endl
         << endl;

    cout << "Search number 3:" << endl;
    cout << "Searching for " << randomItem3 << "..." << endl;
    int bsTime3 = getTimeForBinarySearch(randomItem3, gamesArray);
    cout << "Single search time: " << bsTime3 << " nanoseconds." << endl;
    int totalbsTime3 = 0;
    for (int i = 0; i < 5; i++)
    {
        int timeSpenti = getTimeForBinarySearch(randomItem3, gamesArray);
        totalbsTime3 = totalbsTime3 + timeSpenti;
    }
    int averageAfterSort3 = (totalbsTime3 + bsTime3) / 6;
    cout << "Average search time: " << averageAfterSort3 << " nanoseconds." << endl
         << endl;

    /*

    QS
    LS
    BS

    select a game,
    do a linear search = LS
    do a binary search = BS

    LST = LS
    BST = QS + BS
    m = 0;
    while(LST < BST){
        LST += LS;
        BST += BS
        m++;
    }

    Break point m = xxxx between repeated linear searches and sort-once & multiple binary searches.
     */

    int LS = lsTime1;
    int BS = bsTime1;

    int LST = lsTime1;
    int BST = quicksort_duration.count() + BS;
    int m = 0;
    while (LST < BST)
    {
        LST += LS;
        BST += BS;
        m++;
    }

    cout << "Break point m = " << m << " between repeated linear searches and sort-once & multiple binary searches." << endl;

    fclose(gameFilePointer);
    return 0;
}

GAME_ *parseGame(char *game_line)
{
    GAME_ *game_ = new GAME_;

    char *token = strsep(&game_line, ",");
    game_->id = atoi(token);

    token = strsep(&game_line, ",");
    if (token[0] == '"')
    {
        // Handling quoted names
        string name;
        name += token;
        while (name.back() != '"' && game_line != NULL)
        {
            token = strsep(&game_line, ",");
            name += ",";
            name += token;
        }
        game_->name = new char[name.length() + 1];
        strcpy(game_->name, name.substr(1, name.length() - 2).c_str()); // Remove leading and trailing quotes
    }
    else
    {
        game_->name = new char[strlen(token) + 1];
        strcpy(game_->name, token);
    }

    token = strsep(&game_line, ",");
    game_->average_user_rating = atof(token);

    token = strsep(&game_line, ",");
    game_->user_rating_count = atoi(token);

    token = strsep(&game_line, ",");
    if (token[0] == '"')
    {
        // Handling quoted developers
        string developer;
        developer += token;
        while (developer.back() != '"' && game_line != NULL)
        {
            token = strsep(&game_line, ",");
            developer += ",";
            developer += token;
        }
        game_->developer = new char[developer.length() + 1];
        strcpy(game_->developer, developer.substr(1, developer.length() - 2).c_str()); // Remove leading and trailing quotes
    }
    else
    {
        game_->developer = new char[strlen(token) + 1];
        strcpy(game_->developer, token);
    }

    token = strsep(&game_line, ",");
    game_->size = atoi(token);

    return game_;
}

void printGameItem(GAME_ *game)
{
    cout << "# " << game->id << ", " << game->name << ", " << game->average_user_rating << ", " << game->user_rating_count << ", " << game->developer << ", " << game->size << endl;
}

int generateRandomNumber(int max)
{
    static bool initialized = false;

    // Seed the random number generator only if not already initialized
    if (!initialized)
    {
        srand(static_cast<unsigned>(time(NULL)));
        initialized = true;
    }

    // Generate a random number between 0 and 89
    int random_number = rand() % (max + 1);
    return random_number;
}

void swap(GAME_ *a, GAME_ *b)
{
    GAME_ t = *a;
    *a = *b;
    *b = t;
}
GAME_ *linearSearch(const vector<GAME_ *> &gamesArray, const char *targetName)
{
    for (const auto &game : gamesArray)
    {
        if (strcmp(game->name, targetName) == 0)
        {
            return game;
        }
    }
    return NULL; // Not found
}

GAME_ *binarySearch(const vector<GAME_ *> &gamesArray, const char *targetName)
{
    int low = 0;
    int high = static_cast<int>(gamesArray.size()) - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        int comparisonResult = strcmp(gamesArray[mid]->name, targetName);

        if (comparisonResult == 0)
        {
            return gamesArray[mid]; // Found
        }
        else if (comparisonResult < 0)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return NULL; // Not found
}

void insertionSort(vector<GAME_ *> &gamesArray)
{
    for (int i = 1; i < gamesArray.size(); ++i)
    {
        GAME_ *key = gamesArray[i];
        int j = i - 1;

        while (j >= 0 && strcmp(gamesArray[j]->name, key->name) > 0)
        {
            gamesArray[j + 1] = gamesArray[j];
            --j;
        }

        gamesArray[j + 1] = key;
    }
}

int partition(vector<GAME_ *> &gamesArray, int low, int high)
{
    GAME_ *pivot = gamesArray[high];
    int i = low - 1;

    for (int j = low; j < high; ++j)
    {
        if (strcmp(gamesArray[j]->name, pivot->name) <= 0)
        {
            ++i;
            swap(gamesArray[i], gamesArray[j]);
        }
    }

    swap(gamesArray[i + 1], gamesArray[high]);
    return i + 1;
}


void quickSort(vector<GAME_ *> &gamesArray, int low, int high)
{
    if (low < high)
    {
        int pivotIndex = partition(gamesArray, low, high);

        quickSort(gamesArray, low, pivotIndex - 1);
        quickSort(gamesArray, pivotIndex + 1, high);
    }
}

long long getTimeForLinearSearch(char *gameName, vector<GAME_ *> games)
{
    auto start_time = chrono::high_resolution_clock::now();
    GAME_ *gameFound = linearSearch(games, gameName);
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
    return duration.count();
}

long long getTimeForBinarySearch(char *gameName, vector<GAME_ *> games)
{
    auto start_time = chrono::high_resolution_clock::now();
    binarySearch(games, gameName);
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
    return duration.count();
}
