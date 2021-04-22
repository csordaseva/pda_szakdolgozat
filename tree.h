#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <QDebug>

template <typename T>
struct TreeNode {
    T data;
    std::vector<TreeNode<T>> children;

    void printRecursive(int depth = 0) const {
        std::cout << std::setw(depth) << "" << " - " << data << std::endl;
        for (auto child : children)
            child.printRecursive(depth + 1);
    }
    void printRecursiveHTML(int depth = 0) const {
        std::cout << std::setw(depth) << "" << "<li>" << data;
        if (!children.empty()) {
            std::cout << std::endl << std::setw(depth + 2) << "" << "<ul>" << std::endl;
            for (auto child : children)
                child.printRecursiveHTML(depth + 4);
            std::cout << std::setw(depth + 2) << "" << "</ul>" << std::endl << std::setw(depth) << "";
        }
        std::cout << "</li>" << std::endl;
    }
};

#endif
