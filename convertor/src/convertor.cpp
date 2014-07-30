#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

#include "mnist.h"
#include "mini_mnist.h"
#include "eharmony.h"
#include "cifar.h"

string usage =
"\nUsage:\n"
"   convertor [mnist | mini_mnist | cifar10 | cifar100]\n\n";

vector<int> labels;

int main(int argc, char ** argv)
{
    if (argc == 1)
    {
        cout << usage;
        return 0;
    }
    for (int i = 1; i < argc; i++)
    {
        string target (argv[i]);
        if (target == "eharmony")
        {
           eharmony_generate();
        }
        if (target == "mnist")
        {
           mnist_generate();
        }
        if (target == "mini_mnist")
        {
            mini_mnist_generate();
        }
        if (target == "cifar")
        {
            cifar_generate();
        }
    }
}
