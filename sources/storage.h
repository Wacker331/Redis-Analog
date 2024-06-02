#ifndef STORAGE_H
#define STORAGE_H
#include <iostream>
#include <mutex>
#include <fstream>

class TreeElem;
class Tree;
class Elem;

//Abstract class (interface) needed to easily replace the internal storage data structure
class StorageInterface
{
public:
    virtual Elem Get(std::string) = 0;
    virtual Elem Put(struct Elem) = 0;
    virtual Elem Del(std::string) = 0;
    virtual void Dump(std::ofstream&)
    {
        std::cout << "Dump not initialised" << std::endl;
    }

};

//Storage class for server
class Storage
{
    int CountElems;
    StorageInterface &StorageStructure;
    std::mutex Mutex;

    Elem ParseDumpLine(std::ifstream&);
public:
    Storage(StorageInterface&);

    Elem Put(const std::string, const std::string);
    Elem Put(const Elem);
    Elem Del(const std::string);
    Elem GetCount();
    Elem operator[](const std::string);

    void Dump(const std::string);
    void ReadDump(const std::string);
};

//Element in storage data structure
class Elem
{
public:
    std::string Key;
    std::string Value;
    Elem();
    Elem(const std::string, const std::string);
    Elem(const TreeElem);
};

//Element in binary tree data structure
class TreeElem : public Elem
{
    friend class Tree;
protected:
    void CopyData(const TreeElem);
    TreeElem* DelNode(TreeElem*);
    void Dump(std::ofstream&);
    void Print();
    TreeElem* Left;
    TreeElem* Right;
public:
    TreeElem(const Elem);
};

//Tree data structure for "Storage" that can be easily replaced to another data structure with "StorageInterface"
class Tree : public StorageInterface
{
    TreeElem* Root;

    friend class Storage;
    friend class Tree_Test;
protected:
    Elem Get(const std::string) override;
    Elem Put(const struct Elem) override;
    Elem Del(const std::string) override;
    void Dump(std::ofstream&) override;
    void Print();

public:
    Tree();
};

#endif