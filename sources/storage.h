#include <iostream>

class TreeElem;
class Tree;
class Elem;

class StorageInterface
{
    virtual Elem Get(std::string) = 0;
    virtual Elem Put(struct Elem) = 0;
    virtual Elem Del(std::string) = 0;

};

class Storage
{
    int CountElems;
    // StorageInterface &StorageInstance;

public:
    // Storage(StorageInterface&);
    // ~Storage();

    // Elem* Get(std::string );
    // bool Put(std::string, std::string);
    // bool Del(std::string);
    // Elem& operator[](std::string);

};


class Elem
{
//     friend class TreeElem;
//     friend class Tree;
// protected:
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
    void Print();
};

class Tree : public StorageInterface
{
    TreeElem* Root;

public:
    Tree();
    // ~Tree();

    Elem Get(std::string) override;
    Elem Put(struct Elem) override;
    Elem Del(std::string) override;
    void Print();
};
