#include "storage.h"

Storage::Storage(StorageInterface &StorageStructure) : StorageInstance(StorageStructure)
{
    CountElems = 0;
}

Elem Storage::Put(std::string KeyToPut, std::string ValueToPut) 
{
    return Storage::Put(Elem(KeyToPut, ValueToPut));
}

Elem Storage::Put(Elem ElemToPut)
{
    return StorageInstance.Put(ElemToPut);
}

Elem Storage::Del(std::string KeyToDel)
{
    return StorageInstance.Del(KeyToDel);
}

Elem Storage::operator[](std::string KeyToGet)
{
    return StorageInstance.Get(KeyToGet);
}

//Initialising empty Elem object
Elem::Elem()
{
    this -> Key = "";
    this -> Value = "";
}

//Initialising Elem class with data strings (mostly for testing)
Elem::Elem(std::string KeyToInit, std::string ValueToInit)
{
    this -> Key = KeyToInit;
    this -> Value = ValueToInit;
}

//Initialising Elem with ElemTree object
Elem::Elem(TreeElem ElemToInit)
{
    this -> Key = ElemToInit.Key;
    this -> Value = ElemToInit.Value;
}

//Copy only data fields of node
void TreeElem::CopyData(TreeElem ElemToCopy)
{
    this -> Key = ElemToCopy.Key;
    this -> Value = ElemToCopy.Value;
}

//Delete exact node in data tree
TreeElem *TreeElem::DelNode(TreeElem *ParentElem)
{
    //If matched node has 0 childs
    if (this -> Right == NULL && this -> Left == NULL)
    {
        if (ParentElem != NULL)
        {
            if (ParentElem -> Left == this)
                ParentElem -> Left = NULL;
            else if (ParentElem -> Right == this)
                ParentElem -> Right = NULL;
        }
        delete this;
        return NULL;
    }
    //If matched node has 2 childs -- replace matched node with the minimum node in the right part of tree
    else if (this -> Right != NULL && this -> Left != NULL)
    {
        TreeElem *tmpNodeRightMinPrev = this;
        TreeElem *tmpNodeRightMin = this -> Right;
        bool LeftNodeFlag = 0;
        
        //Finding minimum node in the right part of tree
        while (tmpNodeRightMin -> Left != NULL)
        {
            tmpNodeRightMinPrev = tmpNodeRightMin;
            tmpNodeRightMin = tmpNodeRightMin -> Left;
            LeftNodeFlag = 1;
        }

        this->CopyData(*tmpNodeRightMin);

        delete tmpNodeRightMin;
        if (LeftNodeFlag)
            tmpNodeRightMinPrev -> Left = NULL;
        else
            tmpNodeRightMinPrev -> Right = NULL;

        return this;
    }
    //If matched node has only 1 child
    else
    {
        TreeElem *tmpNodeChild = NULL;
        if (this -> Right != NULL)
            tmpNodeChild = this -> Right;
        else if (this -> Left != NULL)
            tmpNodeChild = this -> Left;

        std::cout << "Child: " << std::endl;

        if (ParentElem != NULL)
        {
            if (ParentElem -> Right == this)
                ParentElem -> Right = tmpNodeChild;
            else if (ParentElem -> Left == this)
                ParentElem -> Left = tmpNodeChild;
        }
        
        delete this;
        return tmpNodeChild;
    }

    //Error
    return NULL;
}

//Initializing TreeElem by inheritated Elem class
TreeElem::TreeElem(Elem ElemToInit)
{
    this->Key = ElemToInit.Key;
    this->Value = ElemToInit.Value;

    this->Right = NULL;
    this->Left = NULL;
}

//Recursive print of data tree (for debugging)
void TreeElem::Print()
{
    std::cout << this << ": " << "left: " << this -> Left << "; right: " << this -> Right << "; key: " << this -> Key << "; value: " << this -> Value << std::endl;
    if (this -> Left != NULL)
        this -> Left -> Print();
    if (this -> Right != NULL)
        this -> Right -> Print();
}

//Empty tree initialising
Tree::Tree()
{
    this -> Root = NULL;
}

//Tree algorithm to get element in binary tree by some key
Elem Tree::Get(std::string KeyToFind)
{
    TreeElem *tmpNode = Root;
    //Binary tree search
    while (tmpNode != NULL)
    {
        if (tmpNode -> Key == KeyToFind)
        {
            return *tmpNode;
        }
        else if (tmpNode -> Key < KeyToFind)
        {
            tmpNode = tmpNode -> Left;
        }
        else if (tmpNode -> Key > KeyToFind)
        {
            tmpNode = tmpNode -> Right;
        }
    }
    //Not found
    return Elem();
}

//Tree algorithm to put element in binary tree
Elem Tree::Put(struct Elem ElemToPut)
{
    if (Root == NULL)
    {
        Root = new TreeElem(ElemToPut);
        return Elem();
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
                    tmpNode->Left = new TreeElem(ElemToPut);
                    return Elem();
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
                    tmpNode->Right = new TreeElem(ElemToPut);
                    return Elem();
                }
                else
                {
                    tmpNode = tmpNode->Right;
                }
            }
            else if (tmpNode-> Key == ElemToPut.Key)
            {
                Elem RetElem(*tmpNode);
                tmpNode -> CopyData(ElemToPut);
                return RetElem;
            }
        }
    }
    // std::cout << "Tree Put Error" << std::endl;
    return Elem();
}

//Tree algorithm to delete any element by some key
Elem Tree::Del(std::string KeyToDelete)
{
    TreeElem* tmpNodeCur = Root, *tmpNodePrev = NULL;
    while (tmpNodeCur != NULL)
    {
        if (tmpNodeCur -> Key < KeyToDelete)
        {
            tmpNodePrev = tmpNodeCur;
            tmpNodeCur = tmpNodeCur -> Left;
        }
        else if (tmpNodeCur -> Key > KeyToDelete)
        {
            tmpNodePrev = tmpNodeCur;
            tmpNodeCur = tmpNodeCur -> Right;
        }
        else if (tmpNodeCur -> Key == KeyToDelete)
        {
            Elem RetElem(*tmpNodeCur);

            if (tmpNodeCur == Root)
                Root = tmpNodeCur -> DelNode(tmpNodePrev);
            else 
                tmpNodeCur = tmpNodeCur -> DelNode(tmpNodePrev);
            return RetElem;
        }
    }
    return Elem();
}

//Print tree (for debugging)
void Tree::Print()
{
    if (Root != NULL)
        Root->Print();
}
