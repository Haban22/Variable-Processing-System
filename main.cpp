#include<iostream>
#include <list>
#include <fstream>
#include <string>
using namespace std;

class Variable
{
public:
    string dataType;
    string Name;
    string dataValue;

    Variable(string type, string name, string value):dataType(type),Name(name),dataValue(value){}
};

class VariableStack
{
    list<Variable> Stack;
public:
    VariableStack(){}
    ~VariableStack(){}

    void push(const Variable &variable)
    {
        Stack.push_back(variable);
    }

    bool isEmpty()
    {
        return Stack.empty();
    }

    void pop()
    {
        if(!Stack.empty())
            Stack.pop_back();
        else
            cout<<"ERROR: Stack is Empty"<<endl;
    }

    Variable& peak()
    {
        if(!Stack.empty())
            return Stack.back();
    }


    void clearStack()
    {
        Stack.clear();
    }

    void displayStack()
    {
        cout<<"Stack Contents:"<<endl;
        for(auto it = Stack.rbegin(); it != Stack.rend(); ++it)
        {
            cout<<"Name: "<<it->Name<<", Type: "<<it->dataType<<", Value: "<<it->dataValue<<endl;
        }
        cout << endl;
    }
};

string findVariables(string line, VariableStack Stack)
{
    while(!Stack.isEmpty())
    {
        string substring = Stack.peak().Name;
        if(line.find(substring) != string::npos && line.find(Stack.peak().dataType) == string::npos)
            return Stack.peak().Name;
        else
            Stack.pop();
    }
    return "";
}

string removeSpaces(string substring)
{
    int startNONspace = substring.find_first_not_of(" ");
    int lastNONspace = substring.find_last_not_of(" ");
    if(startNONspace == 0 && lastNONspace == substring.length()-1)
        return substring;
    else if(startNONspace == lastNONspace)
        return substring.substr(startNONspace, 1);
    else
        return substring.substr(startNONspace, lastNONspace);
}

string extractVariableName(string type, int length, string line)
{
    int start = line.find(type)+length;
    int last = line.find("=");
    return removeSpaces(line.substr(start, last-start));
}

string extractVariableValue(string line)
{
    int start = line.find("=")+1;
    int last = line.find(";");
    return removeSpaces(line.substr(start, last-start));
}

void declareVariable(const string& type, int length, const string& line, VariableStack& Stack) {
    if(line.find(type) != string::npos) {
        Variable variable(type, extractVariableName(type, length, line), extractVariableValue(line));
        Stack.push(variable);
        Stack.displayStack();
    }
}
void updateVariable(VariableStack &Stack, const string& variableName, const string& variableValue)
{
    VariableStack tempStack;

    while(!Stack.isEmpty())
    {
        if(Stack.peak().Name == variableName)
            Stack.peak().dataValue = variableValue;
        tempStack.push(Stack.peak());
        Stack.pop();
    }

    while(!tempStack.isEmpty())
    {
        Stack.push(tempStack.peak());
        tempStack.pop();
    }

    tempStack.clearStack();
}


void processLine(const string& line, VariableStack& Stack) {
    if(line.find("return") != string::npos) {
        Stack.displayStack();
        return;
    }

    if(line.find("int main")!= string::npos || line.find("{")!= string::npos || line.find("}")!= string::npos)
        return;

    if(line.find("int") != string::npos)
        declareVariable("int", 3, line, Stack);
    else if(line.find("float") != string::npos)
        declareVariable("float", 5, line, Stack);
    else if(line.find("double") != string::npos)
        declareVariable("double", 6, line, Stack);
    else if(line.find("char") != string::npos)
        declareVariable("char", 4, line, Stack);
    else if(findVariables(line, Stack) != "")
        updateVariable(Stack, findVariables(line, Stack), extractVariableValue(line));
}

void processFile(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile)
    {
        cout<< "Error opening file." <<endl;
        return;
    }

    string line;
    VariableStack Stack;

    while(getline(inputFile,line))
    {
        processLine(line, Stack);
    }
}

int main()
{
    processFile("code.txt");
    return 0;
}
