#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
using namespace std;
#define int long long

int nextLeafPos = 1;
map<int, char> nextLeafPosToSym;

struct parseTree
{
    char cur_symbol;
    parseTree *left;
    parseTree *right;
    bool nullable;
    set<int> firstPos;
    set<int> lastPos;
    int position;

    parseTree()
    {
        cur_symbol = 0;
        left = NULL;
        right = NULL;
        position = 0;
    }
    parseTree(char ch)
    {
        cur_symbol = ch;
        left = NULL;
        right = NULL;
        position = nextLeafPos;
        nextLeafPosToSym[position] = ch;
        nextLeafPos++;
    }
};

bool isChar(char c)
{
    if (c == 'a')
    {
        return true;
    }
    else if (c == 'b')
    {
        return true;
    }
    else if (c == '#')
    {
        return true;
    }
    else
    {
        return false;
    }
}

int getMatchingBracket(string reg, int i)
{
    int o = 1;
    i++;
    for (; i < reg.length(); i++)
    {
        if (reg[i] == '(')
        {
            o++;
        }
        else if (reg[i] == ')')
        {
            o--;
        }
        if (o == 0)
        {
            return i;
        }
    }
    return -1;
}

string removeBracket(string w)
{
    int first_bracket = 0, last_bracket = 0;
    vector<int> ind_to_del;
    stack<int> st;
    for (int i = 0; i < w.length(); i++)
    {
        if (w[i] == '(')
        {
            st.push(i);
        }
        else if (w[i] == ')')
        {
            last_bracket = i;

            if (i - st.top() == 2)
            {
                ind_to_del.push_back(i);
                ind_to_del.push_back(st.top());
                st.pop();
            }
            else
            {
                st.pop();
            }
        }
    }
    sort(ind_to_del.rbegin(), ind_to_del.rend());
    for (int i = 0; i < ind_to_del.size(); i++)
    {
        w.erase(w.begin() + ind_to_del[i]);
    }
    //cout << w << "\n";
    while (!st.empty())
    {
        st.pop();
    }
    ind_to_del.clear();
    for (int i = 0; i < w.length(); i++)
    {
        if (w[i] == '(')
        {
            st.push(i);
        }
        else if (w[i] == ')')
        {
            if (i == w.length() - 1)
            {
                if (w[st.top() - 1] == '|')
                {
                    st.pop();
                }
                else
                {
                    ind_to_del.push_back(i);
                    ind_to_del.push_back(st.top());
                    st.pop();
                }
            }
            else if (w[i + 1] == '|' || w[i + 1] == '*' || w[i + 1] == '+')
            {
                st.pop();
            }
            else
            {
                if (w[st.top() - 1] == '|')
                {
                    st.pop();
                }
                else
                {
                    ind_to_del.push_back(i);
                    ind_to_del.push_back(st.top());
                    st.pop();
                }
            }
        }
    }
    sort(ind_to_del.rbegin(), ind_to_del.rend());
    for (int i = 0; i < ind_to_del.size(); i++)
    {
        w.erase(w.begin() + ind_to_del[i]);
    }
    //cout << "after removing all bracket " << w << "\n";
    while (!st.empty())
    {
        st.pop();
    }
    for (int i = 0; i < w.length() - 1; i++)
    {
        if (w[i] == '(')
        {
            st.push(i);
        }
        else if (w[i] == ')')
        {
            if (w[i + 1] == '+')
            {
                w[i + 1] = '*';
                string temp = w.substr(st.top(), i - st.top() + 1);
                w.insert(st.top(), temp);
                st.pop();
            }
            else
            {
                st.pop();
            }
        }
        else if (w[i] == '+')
        {
            w[i] = '*';
            string temp = w.substr(i - 1, 1);
            w.insert(i, temp);
        }
    }
    return w;
}

parseTree *makeParseTree(string reg)
{
    stack<parseTree *> st;
    parseTree *temp;
    int i = 0;
    while (i < reg.length())
    {
        // cout<<reg<<reg<<"\n";
        if (isChar(reg[i]))
        {
            st.push(new parseTree(reg[i]));
        }
        else if (reg[i] == '*')
        {
            temp = new parseTree;
            temp->cur_symbol = '*';
            if (!st.empty())
            {
                temp->right = NULL;
                temp->left = st.top();
                st.pop();
                st.push(temp);
            }
            else
            {
                cout << "error in makeParseTree '*'\n";
                exit(1);
            }
        }
        else if (reg[i] == '|')
        {
            temp = new parseTree;
            temp->cur_symbol = '|';
            st.push(temp);
        }
        else if (reg[i] == '(')
        {
            int in = reg.length() - 1;
            in = getMatchingBracket(reg, i);
            if (in >= 0)
            {
                st.push(makeParseTree(reg.substr(i + 1, in - i - 1)));
                i = in + 1;
                continue;
            }
            else
            {
                cout << "invalid regex text" << reg[i] << "\n";
                exit(1);
            }
        }
        else
        {
            cout << "invalid regex abs" << reg[i] << "\n";
            exit(1);
        }

        i++;
    }

    parseTree *op1, *op2;
    stack<parseTree *> stRev;
    while (!st.empty())
    {
        stRev.push(st.top());
        st.pop();
    }
    op1 = NULL;
    op2 = NULL;
    while (!stRev.empty())
    {
        if (op1 != NULL)
        {
            op2 = stRev.top();
            stRev.pop();

            if (op2->cur_symbol == '|' && (op2->left == NULL || op2->right == NULL))
            {
                if (!stRev.empty())
                {
                    op2->left = op1;
                    op2->right = stRev.top();
                    stRev.pop();
                    op1 = op2;
                }
                else
                {
                    cout << "invalid regex in strev\n";
                    exit(1);
                }
            }
            else
            {
                temp = new parseTree;
                temp->cur_symbol = '.';
                temp->left = op1;
                temp->right = op2;
                op1 = temp;
            }
        }
        else
        {
            op1 = stRev.top();
            stRev.pop();
            continue;            
        }
    }
    return op1;
}

void init(parseTree *head)
{
    if (head != NULL)
    {
        init(head->left);
        init(head->right);
    }
    else
    {
        return;
    }
    char sym = head->cur_symbol;
    if (!isChar(sym))
    {
        if (sym == '.')
        {
            head->nullable = ((head->left)->nullable && (head->right)->nullable);
            set<int> &leftF = head->left->firstPos;
            set<int> &rightF = head->right->firstPos;
            set<int> &leftL = head->left->lastPos;
            set<int> &rightL = head->right->lastPos;

            head->firstPos.insert(leftF.begin(), leftF.end());
            if (head->left->nullable)
            {
                head->firstPos.insert(rightF.begin(), rightF.end());
            }
            head->lastPos.insert(rightL.begin(), rightL.end());
            if (head->right->nullable)
            {
                head->lastPos.insert(leftL.begin(), leftL.end());
            }
        }
        else if (sym == '*')
        {
            head->nullable = true;
            head->firstPos = (head->left)->firstPos;
            head->lastPos = (head->left)->lastPos;
        }
        else if (sym == '|')
        {
            head->nullable = ((head->left)->nullable || (head->right)->nullable);

            set<int> &leftF = head->left->firstPos;
            set<int> &rightF = head->right->firstPos;
            set<int> &leftL = head->left->lastPos;
            set<int> &rightL = head->right->lastPos;

            head->firstPos.insert(leftF.begin(), leftF.end());
            head->firstPos.insert(rightF.begin(), rightF.end());

            head->lastPos.insert(leftL.begin(), leftL.end());
            head->lastPos.insert(rightL.begin(), rightL.end());
        }
    }
    else
    {
        head->nullable = false;
        head->firstPos.insert(head->position);
        head->lastPos.insert(head->position);
    }
}

void createFollow(parseTree *head, map<int, set<int>> &follow)
{
    if (head == NULL)
    {
        return;
    }
    createFollow(head->left, follow);
    createFollow(head->right, follow);

    set<int> &lastPos = head->left->lastPos;
    set<int>::iterator itr;
    set<int> exch;
    if (head->cur_symbol == '.' && head->cur_symbol != '*')
    {
        exch = head->right->firstPos;
    }
    else if (head->cur_symbol == '*' && head->cur_symbol != '.')
    {
        exch = head->left->firstPos;
    }

    if (exch.empty())
    {
        return;
    }
    else
    {
        for (itr = lastPos.begin(); itr != lastPos.end(); itr++)
        {
            follow[*itr].insert(exch.begin(), exch.end());
        }
    }
}

int32_t main()
{
    ifstream inputFile("input.txt");
    ofstream outputfile("output.txt");
    string reg, w;
    getline(inputFile, reg);
    getline(inputFile, w);
    reg = removeBracket(reg);
    if (reg[reg.length() - 1] == ' ')
    {
        reg.pop_back();
    }
    reg.push_back('#');
    cout<<reg<<"\n";
    map<int, set<int>> followPos;
    map<int, set<int>>::iterator followPos_itr;
    parseTree *root;
    root = makeParseTree(reg);
    init(root);
    createFollow(root, followPos);

    map<char, set<int>> nextState;
    map<char, set<int>>::iterator nextState_itr;

    map<set<int>, int> setToStateNum;

    // some useful variables
    int currStateNum = 0;
    int nextStateNum = 0;
    int stateNum = 0;
    char sym;

    // Storing set of positions for the current state, the next state and the final state
    set<int> currSet;
    set<int> nextSet;
    set<int> finalSet;
    set<int>::iterator set_itr;

    // queue for processing the states
    queue<set<int>> que;
    que.push(root->firstPos);

    // to find leaf position of the '#' character for the final set
    nextLeafPos--;

    // stores transitions with respective labels
    map<pair<int, int>, string> transition;
    map<pair<int, string>, int> transition_table;
    map<pair<int, int>, string>::iterator transition_itr;
    string temp;

    while (!que.empty())
    {

        currSet = que.front();
        que.pop();

        nextState.clear();

        // setting the statenumber if not already set. Basically mapping a set to a state number!
        if (setToStateNum.find(currSet) == setToStateNum.end())
            setToStateNum[currSet] = stateNum++;

        // finding the next positions that are possible for every symbol
        set_itr = currSet.begin();
        while (set_itr != currSet.end())
        {
            sym = nextLeafPosToSym[*set_itr];
            if (followPos.find(*set_itr) != followPos.end())
            {
                nextState[sym].insert(followPos[*set_itr].begin(), followPos[*set_itr].end());
            }
            set_itr++;
        }

        // checking if the current state is a final state
        currStateNum = setToStateNum[currSet];
        if (currSet.find(nextLeafPos) != currSet.end())
        {
            finalSet.insert(currStateNum);
        }
        nextState_itr = nextState.begin();
        while ( nextState_itr != nextState.end())
        {
            sym = nextState_itr->first;
            nextSet = nextState_itr->second;
            if (setToStateNum.find(nextSet) == setToStateNum.end())
            {
                que.push(nextSet);
                setToStateNum[nextSet] = stateNum++;
            }
            nextStateNum = setToStateNum[nextSet];
            if (transition.find(make_pair(currStateNum, nextStateNum)) == transition.end())
                temp = sym;
            else
                temp = string(",") + sym;

            transition[make_pair(currStateNum, nextStateNum)] += temp;
            nextState_itr++;
        }
    }

    // Printing transitions to draw

    //cout << "strict digraph{ " << endl;

    for (transition_itr = transition.begin(); transition_itr != transition.end(); transition_itr++)
    {
        currStateNum = (transition_itr->first).first;
        nextStateNum = (transition_itr->first).second;
        if (transition_itr->second.length() == 3)
        {
            transition_table[{currStateNum, transition_itr->second.substr(0, 1)}] = nextStateNum;
            transition_table[{currStateNum, transition_itr->second.substr(2, 1)}] = nextStateNum;
        }
        else
        {
            transition_table[{currStateNum, transition_itr->second}] = nextStateNum;
        }

        //cout << "\t\"Q" << currStateNum << "\" -> \"Q" << nextStateNum << "\" [ label = \"" << transition_itr->second << "\" ];" << endl;
    }
    for (auto it = transition_table.begin(); it != transition_table.end(); it++)
    {
        //cout << "Q" << (it->first).first << " with input " << (it->first).second << " gives " << it->second << "\n";
    }
    int cur_state = 0, start_ind = 0, longest_match_len = 0;
    int i = 0;

    //printing final state
    for(int i : finalSet)
    {
        cout<<"final state "<<i<<" ";
    }

    while(i<w.length()) // i++ left
    {
        if(transition_table.find(make_pair(cur_state, w.substr(i, 1))) != transition_table.end())
        {
            cur_state = transition_table[{cur_state, w.substr(i, 1)}];
            i++;
        }
        else
        {
            if(longest_match_len == 0)
            {
                outputfile<<"@"<<w[start_ind];
                cur_state = 0;
                start_ind++;
                i = start_ind;
               
            }
            else
            {
                outputfile<<"$"<<w.substr(start_ind, longest_match_len);
                cur_state = 0;
                start_ind = start_ind + longest_match_len;
                i = start_ind;
                longest_match_len = 0;
            }
        }

        if(finalSet.find(cur_state) != finalSet.end())
        {
            longest_match_len = i - start_ind;
        }
        if(i==w.length() && start_ind < w.length())
        {
            if(longest_match_len == 0)
            {
                outputfile<<"@"<<w[start_ind];
                start_ind++;
                i = start_ind;
                cur_state = 0;
                
            }
            else
            {
                outputfile<<"$"<<w.substr(start_ind, longest_match_len);
                start_ind = start_ind + longest_match_len;
                cur_state = 0;
                i = start_ind;
                longest_match_len = 0;
            }
        }
    }
    outputfile<<"#";
}

// a(ab*a|ba*b)*a
// ((a((((ab*))a|(ba*))b)*))a