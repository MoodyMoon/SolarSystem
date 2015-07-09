#include "main_form.h"

int main(void)
{
    try
    {
        MainForm main_form;
    }
    catch(Exception &ex)
    {
        std::cerr << ex.what();
    }

    return 0;
}