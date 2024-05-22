#include <iostream>

class TreeElem;
class Tree;
class Elem;

class StorageInterface
{
    virtual Elem Get(std::string);
    virtual Elem Put(struct Elem);
    virtual Elem Del(std::string);
};

class Storage
{
    int CountElems;
    StorageInterface &StorageInstance;

public:
    Storage(StorageInterface&);
    ~Storage();

    Elem* Get(std::string );
    bool Put(std::string, std::string);
    bool Del(std::string);
    Elem& operator[](std::string);

};


class Elem
{
    friend class TreeElem;
    friend class Tree;
protected:
    std::string Key;
    std::string Value;
public:
    Elem();
    Elem(std::string, std::string);
    Elem(TreeElem);
};

class TreeElem : public Elem
{
    friend class Tree;
protected:
    void CopyData(TreeElem);
    void DelElem(TreeElem*);
public:
    TreeElem* Left;
    TreeElem* Right;

    TreeElem(Elem);
};

class Tree : public StorageInterface
{
    TreeElem* Root;

public:
    Tree();
    ~Tree();

    Elem Get(std::string) override;
    Elem Put(struct Elem) override;
    Elem Del(std::string) override;
};
