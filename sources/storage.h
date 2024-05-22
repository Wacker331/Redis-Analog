#include <iostream>

class Storage
{
    int CountElems;
    StorageInterface& StorageInstance;

public:
    Storage(StorageInterface&);
    ~Storage();

    struct Elem* Get(std::string );
    bool Put(std::string, std::string);
    bool Del(std::string);
    struct Elem& operator[](std::string);

};

class StorageInterface
{
    virtual struct Elem Get(std::string);
    virtual struct Elem Put(struct Elem);
    virtual bool Del(std::string);
};

class Tree : public StorageInterface
{
    TreeElem* Root;
    
public:
    Tree();
    ~Tree();

    struct Elem Get(std::string) override;
    struct Elem Put(struct Elem) override;
    bool Del(std::string) override;
};

struct Elem
{
    std::string Key;
    std::string Value;
};

class TreeElem : public Elem
{
public:
    struct TreeElem* Left;
    struct TreeElem* Right;

    TreeElem(struct Elem);
};