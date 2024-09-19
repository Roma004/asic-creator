#include <cstring>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

#include "split.h"
#include "ioutil.h"

int main() {
    setlocale(LC_ALL, "ru_RU.UTF8");

    while (true) {
        try {
            std::wstring str;
            IOUtil::wstdin_get(L">>> ", str, L"\n");
            std::wcout << std::format(L"'{}'", word_syll_split(str))
                       << std::endl;
        } catch (IOUtil::eof_exception &e) {
            std::wcout << std::endl;
            break;
        } catch (const std::runtime_error &e) {
            std::cerr << "Caught error splitting given word:\n  " << e.what()
                       << std::endl;
        } catch (const std::bad_alloc &e) {
            std::cerr << "Caught allocation error:\n  " << e.what()
                       << std::endl;
        } catch (...) { std::cerr << "Caught unknown error" << std::endl; }
    }

    return 0;
}
