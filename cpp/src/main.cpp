#include "bundle.h"
#include "nn.h"
#include <iostream>
#include <sstream>
#include <map>
using namespace std;

#define TRAIN_LABEL_PATH "data/train_labels"
#define TRAIN_VTR_PATH "data/train_vectors"
#define TEST_LABEL_PATH "data/test_labels"
#define TEST_VTR_PATH "data/test_vectors"

bool is_running = true;
map <string, void (*) (vector <string>)> cmds;
vector <Bundle> test_set;
vector <Bundle> train_set;

void nn_cmd(vector <string> args);
void test_cmd(vector <string> args);
void show_cmd(vector <string> args);
void exit_cmd(vector <string> args);

int main()
{
    ifstream train_label_in (TRAIN_LABEL_PATH, ios::binary);
    ifstream train_vtr_in (TRAIN_VTR_PATH, ios::binary);
    cout << "Loading Train Set...";
    cout.flush();
    if (load_set(train_set, train_label_in, train_vtr_in))
    {
        cout << "DONE" << endl;
    }
    train_label_in.close();
    train_vtr_in.close();
    ifstream test_label_in (TEST_LABEL_PATH, ios::binary);
    ifstream test_vtr_in (TEST_VTR_PATH, ios::binary);
    cout << "Loading Test Set...";
    cout.flush();
    if (load_set(test_set, test_label_in, test_vtr_in))
    {
        cout << "DONE" << endl;
    }
    test_label_in.close();
    test_vtr_in.close();
    cmds["nn"] = nn_cmd;
    cmds["test"] = test_cmd;
    cmds["show"] = show_cmd;
    cmds["exit"] = exit_cmd;
    for (string line; is_running && cout << ">>> " &&  getline(cin, line);)
    {
        cout.flush();
        stringstream ss;
        ss << line;
        string cmd;
        ss >> cmd;
        vector <string> args;
        for (string arg; ss >> arg;)
        {
            args.push_back(arg);
        }
        cmds[cmd](args);
    }
}

vector <int> range(int b, int e)
{
    vector <int> res;
    for (int i = b; i < e; i++)
    {
        res.push_back(i);
    }
    return res;
}

void nn_cmd(vector <string> args)
{
    for (int i = 0; i < args.size(); i++)
    {
        cout << args[i] << " -> ";
        cout.flush();
        cout << nn(test_set[stoi(args[i])], train_set, range(0, train_set.size())) << endl;
    }
}

void test_cmd(vector <string> args)
{
    int count = 0;
    vector <int> failed;
    for (int i = 0; i < test_set.size(); i++)
    {
        cout << i << " -> ";
        int l_nn = nn(test_set[i], train_set, range(0, train_set.size()));
        cout << l_nn;
        if (test_set[i].get_label() != train_set[l_nn].get_label())
        {
            cout << " X";
            failed.push_back(i);
            count++;
        }
        cout << endl;
    }
    cout << "acc: " << (count / (double)test_set.size()) << "%" << endl;
    cout << "failed: ";
    for (int i = 0;  i < failed.size(); i++)
    {
        if (i > 0) cout << " ";
        cout << failed[i];
    }
    cout << endl;
}

void show_cmd(vector <string> args)
{
    for (int i = 0; i < args.size(); i += 2)
    {
        Bundle * to_show;
        if (args[i] == "train")
        {
            to_show = &train_set[stoi(args[i + 1])];
        }
        else
        {
            to_show = &test_set[stoi(args[i + 1])];
        }
        cout << to_show->get_label() << endl;
        for (int y = 0; y < 28; y++)
        {
            for (int x = 0; x < 28; x++)
            {
                if (to_show->get_vtr()[28 * y + x] > 255 / 2)
                    cout << 0;
                else
                    cout << " ";
            }
            cout << endl;
        }
    }
}

void exit_cmd(vector <string> args)
{
    is_running = false;
}
