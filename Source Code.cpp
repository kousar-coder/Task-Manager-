#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;
class Task {
public:
    string Title;
    string Priority{};
    string Description;
    string DueDate;
    bool Status = false;
    string Importance;

    Task() {}
    Task(string a, string b, string c, string d, bool e, string f) :Title(a), Priority(b), Description(c), DueDate(d), Status(e), Importance(f) {}
    Task& operator=(const Task& temp) {
        if (this != &temp) {
            Title = temp.Title;
            Priority = temp.Priority;
            Description = temp.Description;
            DueDate = temp.DueDate;
            Status = temp.Status;
            Importance = temp.Importance;
        }
        return *this;
    }
};
template <class U>
class Stack {
    class StackNode {
    public:
        U data;
        StackNode* next;

        StackNode(U a) {
            data = a;
            next = NULL;
        }
    };

    StackNode* top;

public:
    Stack() {
        top = NULL;
    }

    bool isEmpty() {
        return top == NULL;
    }

    void Push(U a) {
        StackNode* newA = new StackNode(a);
        if (top == NULL) {
            top = newA;
            return;
        }
        newA->next = top;
        top = newA;
    }

    U Pop() {
        if (isEmpty()) {
            cout << "Cannot Pop as The Stack is Empty.\n";
            return U();
        }

        StackNode* temp = top;
        U tem = top->data;
        top = top->next;
        delete temp;
        temp = NULL;
        return tem;
    }

    void peek() {
        if (isEmpty()) {
            cout << "The stack is empty.\n";
        }
        else {
            cout << "The Top Element is: " << top->data << endl;
        }
    }
};
template<typename T>
class Queue {
public:
    class Node {
    public:
        Node* next;
        T data;
        Node(T a) : data(a), next(NULL) {}
    };
    Node* rear;
    Node* front;
    Queue() : rear(NULL), front(NULL) {}
    void push(T a) {
        if (front == NULL) {
            front = rear = new Node(a);
        }
        Node* temp = new Node(a);
        rear->next = temp;
        rear = temp;
    }
    T pop() {
        if (front == NULL) {
            cout << "Queue is Empty! ";
            return T();
        }
        Node* temp = front;
        front = front->next;
        if (front == NULL) {
            rear = NULL;
        }
        return temp->data;
    }
    T getFront() {
        return front->data;
    }
    bool isEmpty() {
        return (front == NULL && rear == NULL) ? true : false;
    }
};
class Node {
public:
    Task data;
    int height;
    Node* right, * left;
    Node(Task a) {
        data = a;
        height = 1;
        right = NULL;
        left = NULL;
    }
};
class AVL {
public:
    Node* root;
    AVL() {
        root = NULL;
    }

    int height(Node* root) {
        return (root == NULL) ? 0 : root->height;
    }

    int Balance(Node* root) {
        return (root == NULL) ? 0 : (height(root->left) - height(root->right));
    }

    Node* LeftRotate(Node* root) {
        Node* y = root->right;
        Node* temp = y->left;
        y->left = root;
        root->right = temp;

        root->height = max(height(root->left), height(root->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    Node* RightRotate(Node* root) {
        Node* y = root->left;
        Node* temp = y->right;
        y->right = root;
        root->left = temp;

        root->height = max(height(root->left), height(root->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    Node* InsertRecursive(Node* root, Task a) {
        if (root == NULL) {
            return new Node(a);
        }

        if (a.Priority < root->data.Priority) {
            root->left = InsertRecursive(root->left, a);
        }
        else if (a.Priority > root->data.Priority) {
            root->right = InsertRecursive(root->right, a);
        }
        else {
            return root;
        }

        root->height = max(height(root->left), height(root->right)) + 1;
        int bal = Balance(root);

        // Left Left Case
        if (bal > 1 && a.Priority < root->left->data.Priority)
            return RightRotate(root);

        // Right Right Case
        if (bal < -1 && a.Priority > root->right->data.Priority)
            return LeftRotate(root);

        // Left Right Case
        if (bal > 1 && a.Priority > root->left->data.Priority) {
            root->left = LeftRotate(root->left);
            return RightRotate(root);
        }

        // Right Left Case
        if (bal < -1 && a.Priority < root->right->data.Priority) {
            root->right = RightRotate(root->right);
            return LeftRotate(root);
        }

        return root;
    }

    void Insert(Task a) {
        root = InsertRecursive(root, a);
    }

    Node* FindMax(Node* root) {
        while (root->right != nullptr) {
            root = root->right;
        }
        return root;
    }

    Node* Delete(Node* root, string priority) {
        if (root == nullptr) {
            return root;
        }

        if (priority < root->data.Priority) {
            root->left = Delete(root->left, priority);
        }
        else if (priority > root->data.Priority) {
            root->right = Delete(root->right, priority);
        }
        else {
            if (root->left == nullptr || root->right == nullptr) {
                Node* temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else {
                    *root = *temp;
                }
                delete temp;
            }
            else {
                Node* temp = FindMax(root->left);
                root->data = temp->data;
                root->left = Delete(root->left, temp->data.Priority);
            }
        }

        if (root == nullptr) {
            return root;
        }

        root->height = max(height(root->left), height(root->right)) + 1;
        int bal = Balance(root);

        // Left Left Case
        if (bal > 1 && Balance(root->left) >= 0) {
            return RightRotate(root);
        }

        // Left Right Case
        if (bal > 1 && Balance(root->left) < 0) {
            root->left = LeftRotate(root->left);
            return RightRotate(root);
        }

        // Right Right Case
        if (bal < -1 && Balance(root->right) <= 0) {
            return LeftRotate(root);
        }

        // Right Left Case
        if (bal < -1 && Balance(root->right) > 0) {
            root->right = RightRotate(root->right);
            return LeftRotate(root);
        }

        return root;
    }

    void LevelOrder() {
        if (root == NULL) {
            cout << "Tree is Empty\n";
            return;
        }
        Queue<Node*> q;
        q.push(root);
        while (!q.isEmpty()) {
            Node* temp = q.getFront();
            cout << " Title: " << temp->data.Title
                << "\n  Priority: " << temp->data.Priority
                << "\n  Description: " << temp->data.Description
                << "\n  Due Date: " << temp->data.DueDate
                << "\n  Importance: " << temp->data.Importance
                << "\n  Status: " << temp->data.Status << "\n";
            q.pop();
            if (temp->left) {
                q.push(temp->left);
            }
            if (temp->right) {
                q.push(temp->right);
            }
        }
        cout << endl;
    }
    Node* Search(const string& title) {
        return SearchRecursive(root, title);
    }

    Node* SearchRecursive(Node* root, const string& title) {
        if (root == nullptr || root->data.Title == title) {
            return root;
        }

        if (title < root->data.Title) {
            return SearchRecursive(root->left, title);
        }
        else {
            return SearchRecursive(root->right, title);
        }
    }
};
//Task manager is a Heap which during insertion creates a avl as well of quick searching
class TaskManager : public Task {
public:
    vector<Task> tasks;
    AVL tree;
    static int size;
    Stack<Task> undoStack;
    Stack<Task> redoStack;   
    
    void Input() {
    string Title, Description, DueDate, Importance, Priority;
    bool Status = false;
    cout << "Title: ";
    getline(cin, Title);
    cout << "Priority (1-3): ";
    getline(cin, Priority);
    cout << "Description: ";
    getline(cin, Description);
    cout << "Due Date (YYYY-MM-DD): ";
    getline(cin, DueDate);
    cout << "Importance (HIGH, NORMAL, LOW): ";
    getline(cin, Importance);
    Insert(Title, Description, DueDate, Importance, Priority, Status);
    }
    void Insert(string Title, string Description, string DueDate, string Importance, string Priority,bool Status) {
        tasks.push_back(Task(Title, Priority, Description, DueDate, Status, Importance));
		tree.Insert(Task(Title, Priority, Description, DueDate, Status, Importance));
		int index = size - 1;
        while (index > 0 && tasks[(index - 1) / 2].Priority < tasks[index].Priority) {
            swap(tasks[(index - 1) / 2], tasks[index]);
            index = (index - 1) / 2;
        }
        size++;

    }
    void DisplayTask() {
        system("cls");
		Queue<Task> q2;
        vector<Task> visitedTasks;
		cout<<"\n\n\n";
        if (tasks.empty()) {
            cout << "Tree is empty." << endl;
            return;
        }

        q2.push(tasks[0]);

        while (!q2.isEmpty()) {
            Task current = q2.pop();

            bool taskVisited = false;
            for (int i = 0; i < visitedTasks.size(); ++i) {
                const Task& visitedTask = visitedTasks[i];
                if (visitedTask.Title == current.Title && visitedTask.Priority == current.Priority) {
                    taskVisited = true;
                    break;
                }
            }
			
			cout<<"\t\t\tDISPLAYING TASK\n";
            if (!taskVisited) {
                visitedTasks.push_back(current);
                cout << " \t\t\t  Title: " << current.Title
                    << "\n\t\t\t  Priority: " << current.Priority
                    << "\n\t\t\t  Description: " << current.Description
                    << "\n\t\t\t  Due Date: " << current.DueDate
                    << "\n\t\t\t  Importance: " << current.Importance
                    << "\n\t\t\t  Status: " << current.Status << "\n\n\n\n";

                int leftChild = 2 * (visitedTasks.size() - 1) + 1;
                int rightChild = 2 * (visitedTasks.size() - 1) + 2;

                if (leftChild < tasks.size() && !taskVisited) {
                    q2.push(tasks[leftChild]);
                }

                if (rightChild < tasks.size() && !taskVisited) {
                    q2.push(tasks[rightChild]);
                }
            }
        }
    }
    void editTask(string titleToEdit, string newPriority, string newDescription, string newDueDate, bool newStatus, string newImportance) {
        for (int i = 0; i < tasks.size(); i++) {
            string priorityToEdit = tasks[i].Priority;
            if (tasks[i].Title == titleToEdit) {
                tasks[i].Priority = newPriority;
                tasks[i].Description = newDescription;
                tasks[i].DueDate = newDueDate;
                tasks[i].Status = newStatus;
                tasks[i].Importance = newImportance;

                tree.root = tree.Delete(tree.root, priorityToEdit);
                tree.Insert(tasks[i]);
                cout << "Task \"" << titleToEdit << "\" edited successfully.\n";
                return;
            }
            else
            {
                cout << "Task not found: \"" << titleToEdit << "\". Editing failed.\n";
                return;
            }
        }
    }
    // Searching is Done Through Avl which is time efficient of O(log(n))
    void searchTask(const string& titleToSearch) {
        Node* foundNode = tree.Search(titleToSearch);
		system("cls");
        if (foundNode != nullptr) {
            cout << "\t\t\tTask Found\nPrinting Task: \n";
            cout << " \t\t\tTitle: " << foundNode->data.Title
                <<"\n\t\t\tPriority: " << foundNode->data.Priority
                <<"\n\t\t\tDescription: " << foundNode->data.Description
                <<"\n\t\t\tDue Date: " << foundNode->data.DueDate
                <<"\n\t\t\tImportance: " << foundNode->data.Importance
                <<"\n\t\t\tStatus: " << foundNode->data.Status << "\n";
        }
        else {
            cout << "Task Title: " << titleToSearch << " Not Found!\n";
        }
    }
    void markTaskAsCompleted(const string& title) {

        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i].Title == title) {
                if (!tasks[i].Status) {
                    tasks[i].Status = true;
                    cout << "\t\t\tTask \"" << title << "\" marked as completed.\n";
                }
                else {
                    cout << "\t\t\tTask \"" << title << "\" is already completed.\n";
                }
                return;
            }
        }

        cout << "\t\t\tTask not found: \"" << title << "\".\n";
    }
    void taskStatistics() {
        int completedTasks = 0;
        int remainingTasks = 0;

        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i].Status) {
                completedTasks++;
            }
            else {
                remainingTasks++;
            }
        }
        cout << "\t\t\tTask Statistics:\n";
        cout << "\t\t\tCompleted Tasks: " << completedTasks << "\n";
        cout << "\t\t\tRemaining Tasks: " << remainingTasks << "\n";
        cout << "\t\t\tEnd of Task Statistics\n";
    }
    void InOrderTraversal(Node* node) {
        if (node != nullptr) {
            InOrderTraversal(node->left);
			
            cout << "\t\t\t   Title: " << node->data.Title
                << "\n\t\t\t  Priority: " << node->data.Priority
                << "\n\t\t\t  Description: " << node->data.Description
                << "\n\t\t\t  Due Date: " << node->data.DueDate
                << "\n\t\t\t  Importance: " << node->data.Importance
                << "\n\t\t\t  Status: " << node->data.Status << "\n";

            InOrderTraversal(node->right);
        }
    }
    //prints all task priority wise using avl tree in O(n);
    void DisplayTaskPriorityOrder() {
        if (tree.root == nullptr) {
            cout << "Tree is empty." << endl;
            return;
        }
		system("cls");
        cout << "\t\t\tTasks in Priority Order:\n";
        InOrderTraversal(tree.root);
    }
    //uses heap to print task of top priority in O(1)
    void DisplayHighestPriority() {
        cout << " \t\t\t  Title: " <<tasks[1].Title
            << "\n\t\t\t  Priority: " << tasks[1].Priority
            << "\n\t\t\t  Description: " << tasks[1].Description
            << "\n\t\t\t  Due Date: " << tasks[1].DueDate
            << "\n\t\t\t  Importance: " << tasks[1].Importance
            << "\n\t\t\t  Status: " << tasks[1].Status << "\n";
    }
    void DeleteTask(string taskTitle) {
        for (int i = 0; i < size; i++) {
            if (tasks[i].Title == taskTitle) {
                redoStack.Push(tasks[i]);
                tree.root = tree.Delete(tree.root, tasks[i].Priority);
                tasks.erase(tasks.begin()+i);
                size--;
                return;
            }
        }
        cout << "Task Not Found\n";
    }
    void Undo() {
        if (redoStack.isEmpty()) {
            cout << "Undo Can not be done!\n";
        }
        else {
            string Title, Description, DueDate, Importance, Priority;
            bool Status;
            Task temp = redoStack.Pop();
            Title = temp.Title;
            Priority = temp.Priority;
            Description = temp.Description;
            DueDate = temp.DueDate;
            Status = temp.Status;
            Importance = temp.Importance;
            
            Insert(Title, Description, DueDate, Importance, Priority, Status);

           undoStack.Push(temp);
        }
    }
    void Redo() {
        if (undoStack.isEmpty()) {
            cout << "Redo Cannot be Done!\n";
        }
        else {
            Task redoneTask = undoStack.Pop();
            string taskTitle = redoneTask.Title;
            for (int i = 0; i < size; i++) {
                if (tasks[i].Title == taskTitle) {
                    redoStack.Push(tasks[i]);
                    tree.root = tree.Delete(tree.root, tasks[i].Priority);
                    tasks.erase(tasks.begin() + i);
                    size--;
                    return;
                }
            }
        }
    }
    
    void displayDueTasks() {
        // Get the current system time
        time_t currentTime = time(0);
        tm* now = localtime(&currentTime);

        // Display the current date
        cout << "\t\t\tCurrent Date: " << (now->tm_year + 1900) << '-'
             << (now->tm_mon + 1) << '-' << now->tm_mday << endl;

        // Iterate through tasks and compare with the current date
        for (const Task& task : tasks) {
            tm dueDate;
            stringstream dateStream(task.DueDate);
            dateStream >> get_time(&dueDate, "%Y-%m-%d");
			
            // Compare due date with the current date
            if (dueDate.tm_year < now->tm_year ||
                (dueDate.tm_year == now->tm_year && dueDate.tm_mon < now->tm_mon) ||
                (dueDate.tm_year == now->tm_year && dueDate.tm_mon == now->tm_mon && dueDate.tm_mday < now->tm_mday)) {
                cout<<endl<<endl<<endl;
				cout << "\t\t\tTask \"" << task.Title << "\" is overdue!\n";
            } else {
                cout << "\t\t\tTask \"" << task.Title << "\" is not due yet.\n";
            }
        }
    }
};
int TaskManager::size = 1;
void HeapifyMax(vector<Task>& tasks, int size, int index) {
    int largest = index;
    int LeftChild = index * 2 + 1;
    int RightChild = index * 2 + 2;

    if (LeftChild < size && tasks[LeftChild].Priority > tasks[largest].Priority) {
        largest = LeftChild;
    }
    if (RightChild < size && tasks[RightChild].Priority > tasks[largest].Priority) {
        largest = RightChild;
    }
    if (largest != index) {
        swap(tasks[largest], tasks[index]);
        HeapifyMax(tasks, size, largest);
    }
}
void printLine(char symbol, int count) {
    for (int i = 0; i < count; ++i) {
        std::cout << symbol;
    }
    std::cout << std::endl;
}

void printWithDelay(const std::string& text, int delay) {
    for (char c : text) {
        std::cout << c << std::flush;  
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}
void printTaskListVisualization() {
    	 cout << "\t\t\t\t\t      +---------------------------------+      \n"
                 "\t\t\t\t\t      |        Task List        	|      \n"
                 "\t\t\t\t\t      +---------------------------------+      \n"
                 "\t\t\t\t\t      | 1. INSERT TASK          	|      \n"
                 "\t\t\t\t\t      | 2. DISPLAY TASK	 	    	|      \n"
                 "\t\t\t\t\t      | 3. EDIT TASK		    	|      \n"
                 "\t\t\t\t\t      | 4. SEARCH TASK          	|      \n"
                 "\t\t\t\t\t      | 5. MARK TASK  COMPLETED 	|      \n"
                 "\t\t\t\t\t      | 6. TASK STATISTICS      	|      \n"
                 "\t\t\t\t\t      | 7. DISPLAY BY PRIORITY  	|      \n"
                 "\t\t\t\t\t      | 8. DISPLAY HIGEST PRIORITY      |      \n"
                 "\t\t\t\t\t      | 9. DELETE TASK		    	|      \n"
				 "\t\t\t\t\t      | 10.UNDO TASK		    	|      \n"
				 "\t\t\t\t\t      | 11. REDO TASK		    	|      \n"
				 "\t\t\t\t\t      | 12. DISPLAY DUE TASK		|      \n"
				 "\t\t\t\t\t      +---------------------------------+      \n";
}

void intro(TaskManager& T)
{

	int choice;
	here:
	cout<<"\t\t\t\t ENTER YOUR CHOICE : ";
	cin>>choice;
	cin.ignore();
	
	switch(choice)
	{
	case 1:
		{
		

    T.Input();
    printTaskListVisualization();
    intro(T);
    break;
}
    
    
    case 2:
    {
		T.DisplayTask();
    	sleep(10);
		system("cls");
    	printTaskListVisualization();
    	intro(T);
		break;
    	
    case 3:
    string titleToEdit, newDescription, newDueDate, newImportance, newPriority;
    bool newStatus = false;
    cout << "Title: ";
    getline(cin, titleToEdit);
    cout << "Priority (1-3): ";
    getline(cin, newPriority);
    cout << "Description: ";
    getline(cin, newDescription);
    cout << "Due Date (YYYY-MM-DD): ";
    getline(cin, newDueDate);
    cout << "Importance (HIGH, NORMAL, LOW): ";
    getline(cin, newImportance);
    T.editTask( titleToEdit, newPriority,newDescription,newDueDate,newStatus,newImportance);
    sleep(10);
	system("cls");
    printTaskListVisualization();
	intro(T);
    break;
}
    case 4:
  {
    	string titleToSearch;
		cout << "Title: ";
        getline(cin,titleToSearch );
    	T.searchTask(titleToSearch);
    	sleep(10);
		system("cls");
    	printTaskListVisualization();
		intro(T);
		break;
}
    case 5:
{
    	string titletomark;
		cout << "Title: ";
        getline(cin,titletomark );
    	T.markTaskAsCompleted(titletomark);
    	sleep(10);
		system("cls");
    	printTaskListVisualization();
		intro(T);
		break;
}
    case 6:
{
    	T.taskStatistics();
    	sleep(10);
		system("cls");
    	printTaskListVisualization();
		intro(T);
		break;
}
    case 7:
{ 
    	T.DisplayTaskPriorityOrder();
    	sleep(10);
		system("cls");
    	printTaskListVisualization();
		intro(T);
		break;
}
    case 8:
{
    	T.DisplayHighestPriority();
    	sleep(10);
    	system("cls");
    	printTaskListVisualization();
		intro(T);
		break;
}
    case 9:
{
    	string tasktitle;
		cout << "Title: ";
        getline(cin,tasktitle );
    	T.DeleteTask(tasktitle);
    	sleep(10);
		system("cls");
    	printTaskListVisualization();
		intro(T);
		break;
}
    case 10:
{
    	T.Undo();
    	sleep(10);
    	system("cls");
    	printTaskListVisualization();
		intro(T);
		break;
}
    case 11:
{
    	T.Redo();
    	sleep(10);
    	system("cls");
    	printTaskListVisualization();
		intro(T);
		break;
}

	case 12:
		{
			T.displayDueTasks();
			sleep(10);
		}
    default :
{
    	system("cls");
    	printTaskListVisualization();
    	goto here;
}
    	
    	
    	
    }

}

int main() {
	
    cout << "\t\t\t\t";
    printLine('>', 50);
    printWithDelay("\t\t\t\t\t      WELCOME TO TASK MANAGER      \n", 100);
    cout << "\t\t\t\t";
    printLine('>', 50);
    cout<<"\n\n\n";
    printWithDelay( "\t\t\t\tUnleash the power of productivity with our\n"
            "\t\t\t\tcutting-edge Task Manager. Seamlessly manage\n"
            "\t\t\t\tyour tasks, deadlines, and projects with ease.\n"
            "\t\t\t\tStay organized, stay focused, and conquer your\n"
            "\t\t\t\tgoals. Let the journey to enhanced productivity\n"
            "\t\t\t\tbegin!\n",50);
    cout << "\t\t\t\t";
	printLine('-', 50);
	system("cls");
	TaskManager T;
	printTaskListVisualization();
	intro(T);
				
			
} 	
    
