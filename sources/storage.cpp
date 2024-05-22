#include "storage.h"

Tree::Tree()
{
    Root = NULL;
}

struct Elem Tree::Get(std::string KeyToFind)
{

}

struct Elem Tree::Put(struct Elem ElemToPut)
{
    if (Root == NULL)
    {
        Root = &TreeElem(ElemToPut);
        return;
    }
    else
    {
        TreeElem *tmpNode = Root;
        while(tmpNode != NULL)
        {
            if (tmpNode->Key < ElemToPut.Key)
            {
                if (tmpNode->Left == NULL)
                {
                    tmpNode->Left = &TreeElem(ElemToPut);
                    return;
                }
                else
                {
                    tmpNode = tmpNode->Left;
                }
            }
            else if (tmpNode->Key > ElemToPut.Key)
            {
                if (tmpNode->Right == NULL)
                {
                    tmpNode->Right = &TreeElem(ElemToPut);
                    return;
                }
                else
                {
                    tmpNode = tmpNode->Right;
                }
            }
            else if (tmpNode-> Key == ElemToPut.Key)
            {
                struct Elem tmpElem;
                tmpElem.Key = tmpNode -> Key;
                tmpElem.Value = tmpNode -> Value;
                tmpNode -> Value = ElemToPut.Value;
                return tmpElem;
            }
        }
    }
    std::cout << "Tree Put Error" << std::endl;
}
bool Tree::Del(std::string KeyToDelete)
{
}

TreeElem::TreeElem(struct Elem ElemToInit)
{
    this->Key = ElemToInit.Key;
    this->Value = ElemToInit.Value;

    this->Right = NULL;
    this->Left = NULL;
}