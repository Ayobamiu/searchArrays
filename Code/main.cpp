// main.cpp : By Ayobami Usman
// Description: A program comparing searching in Arrays & Linked Lists
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include <chrono>

using namespace std;

#define csv_header "ID,Name,Average User Rating,User Rating Count,Developer,Size \n"
FILE *gameFilePointer = NULL;
GAME_ *parseGame(char *game);
int generateRandomNumber(int max);
void printGameItem(GAME_ *game);

class node
{
public:
    node *next, *prev;
    GAME_ *game;
    node(GAME_ *game)
    {
        next = NULL;
        prev = NULL;
        this->game = game;
    }
};

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
            return nullptr;
        }
        node *currentNode = this->head;
        while (currentNode != nullptr)
        {
            if (strcmp(currentNode->game->name, gameName) == 0)
            {
                return currentNode;
            }
            currentNode = currentNode->next;
        }
        return nullptr;
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
    // Function to perform insertion sort
    void insertionSort()
    {
        node *sorted = NULL;
        node *current = head;

        while (current != NULL)
        {
            node *next = current->next;
            insertSorted(sorted, current);
            current = next;
        }

        head = sorted;
    }

    // Helper function to insert a node into a sorted list
    void insertSorted(node *&sorted, node *new_node)
    {
        if (sorted == NULL || strcmp(sorted->game->name, new_node->game->name) >= 0)
        {
            new_node->next = sorted;
            new_node->prev = NULL;
            if (sorted != NULL)
                sorted->prev = new_node;
            sorted = new_node;
        }
        else
        {
            node *current = sorted;
            while (current->next != NULL && strcmp(current->next->game->name, new_node->game->name) < 0)
            {
                current = current->next;
            }
            new_node->next = current->next;
            new_node->prev = current;
            if (current->next != NULL)
                current->next->prev = new_node;
            current->next = new_node;
        }
    }

    // Function to perform quicksort
    void quickSort()
    {
        head = quickSortRecursive(head, tail);
    }

private:
    // Helper function for quicksort (recursive)
    node *quickSortRecursive(node *start, node *end)
    {
        if (start == NULL || start == end)
            return start;

        node *pivot = partition(start, end);
        if (pivot != NULL && pivot != start)
            pivot->prev = quickSortRecursive(start, pivot->prev);
        if (pivot != NULL && pivot != end)
            pivot->next = quickSortRecursive(pivot->next, end);

        return pivot;
    }

    // Helper function for quicksort (partition)
    node *partition(node *start, node *end)
    {
        char *pivotName = end->game->name;
        node *i = start->prev;

        for (node *j = start; j != end; j = j->next)
        {
            if (strcmp(j->game->name, pivotName) < 0)
            {
                i = (i == NULL) ? start : i->next;
                swap(i->game, j->game);
            }
        }

        i = (i == NULL) ? start : i->next;
        swap(i->game, end->game);
        return i;
    }
};

class BSTNode
{
public:
    GAME_ *game;
    BSTNode *left;
    BSTNode *right;

    BSTNode(GAME_ *game) : game(game), left(nullptr), right(nullptr) {}
};

class BinarySearchTree
{
private:
    BSTNode *root;

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(GAME_ *game)
    {
        root = insertRecursive(root, game);
    }

    BSTNode *search(char *target)
    {
        return searchRecursive(root, target);
    }

private:
    BSTNode *insertRecursive(BSTNode *node, GAME_ *game)
    {
        if (node == nullptr)
        {
            return new BSTNode(game);
        }

        if (strcmp(game->name, node->game->name) < 0)
        {
            node->left = insertRecursive(node->left, game);
        }
        else if (strcmp(game->name, node->game->name) > 0)
        {
            node->right = insertRecursive(node->right, game);
        }

        return node;
    }

    BSTNode *searchRecursive(BSTNode *node, char *target)
    {
        if (node == nullptr || strcmp(target, node->game->name) == 0)
        {
            return node;
        }

        if (strcmp(target, node->game->name) < 0)
        {
            return searchRecursive(node->left, target);
        }
        else
        {
            return searchRecursive(node->right, target);
        }
    }
};

long long getTimeForLinearSearch(node *gameNode, DoublyLinkedList *lList)
{
    // Record the start time
    auto start_time = chrono::high_resolution_clock::now();

    // Perform the event or task you want to measure
    node *randomNodeFoundWithName = lList->findNodeByGameName(gameNode->game->name);

    // Record the end time
    auto end_time = chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);

    // Log the duration
    // cout << "Event duration: " << duration.count() << " nanoseconds." << endl;
    return duration.count();
}

long long getTimeForBinarySearch(node *gameNode, BinarySearchTree bst)
{
    // Record the start time
    auto start_time = chrono::high_resolution_clock::now();

    // Perform the event or task you want to measure
    bst.search(gameNode->game->name);

    // Record the end time
    auto end_time = chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);

    // Log the duration
    // cout << "Event duration: " << duration.count() << " nanoseconds." << endl;
    return duration.count();
}

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
                // printGameItem(duplicate->game);
                // printGameItem(newGameNode->game);
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

    node *randomNode1 = gamesLinkedList->findNodeByIndex(generateRandomNumber(gamesLinkedList->length));
    node *randomNode2 = gamesLinkedList->findNodeByIndex(generateRandomNumber(gamesLinkedList->length));
    node *randomNode3 = gamesLinkedList->findNodeByIndex(generateRandomNumber(gamesLinkedList->length));

    cout << "Number of elements in LinkedList: " << gamesLinkedList->length << endl;
    cout << "*** Linear Search Test ***" << endl;
    cout << "Before sorting:" << endl;
    cout << "# Print only first 5 elements from linked list, for example:" << endl;
    for (int i = 0; i < 5; i++)
    {
        printGameItem(gamesLinkedList->findNodeByIndex(i)->game);
    }

    cout << "Search number 1:" << endl;
    cout << "Searching for " << randomNode1->game->name << "..." << endl;
    int timeSpentBeforeSort1 = getTimeForLinearSearch(randomNode1, gamesLinkedList);
    cout << "Single search time: " << timeSpentBeforeSort1 << " nanoseconds." << endl;
    int totalTimeSpentBeforeSort1;
    for (int i = 0; i < 5; i++)
    {
        int timeSpenti = getTimeForLinearSearch(randomNode1, gamesLinkedList);
        totalTimeSpentBeforeSort1 = totalTimeSpentBeforeSort1 + timeSpenti;
    }
    int averageBeforeSort1 = (totalTimeSpentBeforeSort1 + timeSpentBeforeSort1) / 6;
    cout << "Average search time: " << averageBeforeSort1 << " nanoseconds." << endl
         << endl;

    cout << "Search number 2:" << endl;
    cout << "Searching for " << randomNode2->game->name << "..." << endl;
    int timeSpentBeforeSort2 = getTimeForLinearSearch(randomNode2, gamesLinkedList);
    cout << "Single search time: " << timeSpentBeforeSort2 << " nanoseconds." << endl;
    int totalTimeSpentBeforeSort2;
    for (int i = 0; i < 5; i++)
    {
        int timeSpenti = getTimeForLinearSearch(randomNode2, gamesLinkedList);
        totalTimeSpentBeforeSort2 = totalTimeSpentBeforeSort2 + timeSpenti;
    }
    int averageBeforeSort2 = (totalTimeSpentBeforeSort2 + timeSpentBeforeSort2) / 6;
    cout << "Average search time: " << averageBeforeSort2 << " nanoseconds." << endl
         << endl;

    cout << "Search number 3:" << endl;
    cout << "Searching for " << randomNode3->game->name << "..." << endl;
    int timeSpentBeforeSort3 = getTimeForLinearSearch(randomNode3, gamesLinkedList);
    cout << "Single search time: " << timeSpentBeforeSort3 << " nanoseconds." << endl;
    int totalTimeSpentBeforeSort3;
    for (int i = 0; i < 5; i++)
    {
        int timeSpenti = getTimeForLinearSearch(randomNode3, gamesLinkedList);
        totalTimeSpentBeforeSort3 = totalTimeSpentBeforeSort3 + timeSpenti;
    }
    int averageBeforeSort3 = (totalTimeSpentBeforeSort3 + timeSpentBeforeSort3) / 6;
    cout << "Average search time: " << averageBeforeSort3 << " nanoseconds." << endl
         << endl;

    // Record the start time for insertion sort
    auto insertion_start_time = chrono::high_resolution_clock::now();

    DoublyLinkedList *gamesLinkedListCopyForSort = gamesLinkedList;

    // Perform insertion sort
    gamesLinkedListCopyForSort->insertionSort();

    // Record the end time for insertion sort
    auto insertion_end_time = chrono::high_resolution_clock::now();

    // Calculate and print the time taken for insertion sort
    auto insertion_duration = chrono::duration_cast<chrono::nanoseconds>(insertion_end_time - insertion_start_time);
    cout << "Insertion Sort Time: " << insertion_duration.count() << " nanoseconds" << endl;

    // Record the start time for quicksort
    auto quicksort_start_time = chrono::high_resolution_clock::now();

    // Perform quicksort
    gamesLinkedList->quickSort();

    // Record the end time for quicksort
    auto quicksort_end_time = chrono::high_resolution_clock::now();

    // Calculate and print the time taken for quicksort
    auto quicksort_duration = chrono::duration_cast<chrono::nanoseconds>(quicksort_end_time - quicksort_start_time);
    cout << "Quick Sort Time: " << quicksort_duration.count() << " nanoseconds" << endl;

    cout << "After sorting:" << endl;
    cout << "# Print only first 5 elements from linked list, for example:" << endl;
    for (int i = 0; i < 5; i++)
    {
        printGameItem(gamesLinkedList->findNodeByIndex(i)->game);
    }
    cout << endl;
    cout << "*** Binary Search Test ***" << endl;

    BinarySearchTree bst;

    // Iterate through the DoublyLinkedList and insert each game into the BST
    node *current = gamesLinkedList->head;
    while (current != nullptr)
    {
        bst.insert(current->game);
        current = current->next;
    }

    cout << "Search number 1:" << endl;
    cout << "Searching for " << randomNode1->game->name << "..." << endl;
    int timeSpentAfterSort1 = getTimeForBinarySearch(randomNode1, bst);
    cout << "Single search time: " << timeSpentAfterSort1 << " nanoseconds." << endl;
    int totalTimeSpentAfterSort1;
    for (int i = 0; i < 5; i++)
    {
        int timeSpenti = getTimeForBinarySearch(randomNode1, bst);
        totalTimeSpentAfterSort1 = totalTimeSpentAfterSort1 + timeSpenti;
    }
    int averageAfterSort1 = (totalTimeSpentAfterSort1 + timeSpentAfterSort1) / 6;
    cout << "Average search time: " << averageAfterSort1 << " nanoseconds." << endl
         << endl;

    cout << "Search number 2:" << endl;
    cout << "Searching for " << randomNode2->game->name << "..." << endl;
    int timeSpentAfterSort2 = getTimeForBinarySearch(randomNode2, bst);
    cout << "Single search time: " << timeSpentAfterSort2 << " nanoseconds." << endl;
    int totalTimeSpentAfterSort2;
    for (int i = 0; i < 5; i++)
    {
        int timeSpenti = getTimeForBinarySearch(randomNode2, bst);
        totalTimeSpentAfterSort2 = totalTimeSpentAfterSort2 + timeSpenti;
    }
    int averageAfterSort2 = (totalTimeSpentAfterSort2 + timeSpentAfterSort2) / 6;
    cout << "Average search time: " << averageAfterSort2 << " nanoseconds." << endl
         << endl;

    cout << "Search number 3:" << endl;
    cout << "Searching for " << randomNode3->game->name << "..." << endl;
    int timeSpentAfterSort3 = getTimeForBinarySearch(randomNode3, bst);
    cout << "Single search time: " << timeSpentAfterSort3 << " nanoseconds." << endl;
    int totalTimeSpentAfterSort3;
    for (int i = 0; i < 5; i++)
    {
        int timeSpenti = getTimeForBinarySearch(randomNode3, bst);
        totalTimeSpentAfterSort3 = totalTimeSpentAfterSort3 + timeSpenti;
    }
    int averageAfterSort3 = (totalTimeSpentAfterSort3 + timeSpentAfterSort3) / 6;
    cout << "Average search time: " << averageAfterSort3 << " nanoseconds." << endl
         << endl;

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
        srand(static_cast<unsigned>(time(nullptr)));
        initialized = true;
    }

    // Generate a random number between 0 and 89
    int random_number = rand() % (max + 1);
    return random_number;
}
