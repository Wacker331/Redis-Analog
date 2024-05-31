#ifndef STORAGE_H
#define STORAGE_H
#include <iostream>
#include <mutex>
#include <fstream>

class TreeElem;
class Tree;
class Elem;

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

class Storage
{
    int CountElems;
    StorageInterface &StorageInstance;
    std::mutex Mutex;

public:
    Storage(StorageInterface&);
    // ~Storage();

    Elem Put(std::string, std::string);
    Elem Put(Elem);
    Elem Del(std::string);
    Elem GetCount();
    Elem operator[](std::string);

    void Dump(std::string);
    void ReadDump(std::string);
    Elem ParseDumpLine(std::ifstream&);

};


class Elem
{
public:
    std::string Key;
    std::string Value;
    Elem();
    Elem(std::string, std::string);
    Elem(TreeElem);
};

class TreeElem : public Elem
{
    friend class Tree;
protected:
    void CopyData(TreeElem);
    TreeElem* DelNode(TreeElem*);
public:
    TreeElem* Left;
    TreeElem* Right;

    TreeElem(Elem);
    void Dump(std::ofstream&);
    void Print();
};

class Tree : public StorageInterface
{
    TreeElem* Root;

public:
    Tree();

    Elem Get(std::string) override;
    Elem Put(struct Elem) override;
    Elem Del(std::string) override;
    void Dump(std::ofstream&) override;
    void Print();
};

#endif