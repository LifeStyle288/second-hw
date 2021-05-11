#ifndef PRINTER_H
#define PRINTER_H

template <typename T>
void print_map(const T& container)
{
    for (const auto& val : container)
    {
        std::cout << val.first << ' ' << val.second << '\n';
    }
}

template <typename T>
void print_vec(const T& container)
{
    for (const auto& val : container)
    {
        std::cout << val << '\n';
    }
}

#endif // PRINTER_H
