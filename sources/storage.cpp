#include "storage.h"

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
                return Elem(*tmpNode);
            }
        }
    }
    std::cout << "Tree Put Error" << std::endl;
    return Elem();
}

//Tree algorithm to delete any element by some key
Elem Tree::Del(std::string KeyToDelete)
{
    TreeElem *tmpNodeCur = Root, *tmpNodePrev = NULL;
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

            tmpNodeCur -> DelElem(tmpNodePrev);
            
            return RetElem;
        }
    }
    return Elem();
}

//Initializing TreeElem by inheritated Elem class
TreeElem::TreeElem(Elem ElemToInit)
{
    this->Key = ElemToInit.Key;
    this->Value = ElemToInit.Value;

    this->Right = NULL;
    this->Left = NULL;
}

void TreeElem::CopyData(TreeElem ElemToCopy)
{
    this -> Key = ElemToCopy.Key;
    this -> Value = ElemToCopy.Value;
}

void TreeElem::DelElem(TreeElem *ParentElem)
{
    //If matched node has 0 childs
    if (this -> Right == NULL && this -> Left == NULL)
    {
        if (ParentElem -> Left == this)
            ParentElem -> Left = NULL;
        else if (ParentElem -> Right == this)
            ParentElem -> Right = NULL;
        delete this;
    }
    //If matched node has 2 childs -- replace matched node with the minimum node in the right part of tree
    else if (this -> Right != NULL && this -> Left != NULL)
    {
        TreeElem *tmpNodeRightMinPrev = this;
        TreeElem *tmpNodeRightMin = this -> Right;
        
        //Finding minimum node in the right part of tree
        while (tmpNodeRightMin -> Left != NULL)
        {
            tmpNodeRightMinPrev = tmpNodeRightMin;
            tmpNodeRightMin = tmpNodeRightMin -> Left;
        }

        this->CopyData(*tmpNodeRightMin);

        delete tmpNodeRightMin;
        tmpNodeRightMinPrev -> Left = NULL;

    }
    //If matched node has only 1 child
    else
    {
        TreeElem *tmpNodeChild = NULL;
        if (this -> Right != NULL)
            tmpNodeChild = this -> Right;
        else if (this -> Left != NULL)
            tmpNodeChild = this -> Left;

        if (ParentElem -> Right == this)
            ParentElem -> Right = tmpNodeChild;
        else if (ParentElem -> Left == this)
            ParentElem -> Left = tmpNodeChild;
        
        delete this;
    }
}

//Initialising empty Elem object
Elem::Elem()
{
    this -> Key = "";
    this -> Value = "";
}

//Initialising Elem with ElemTree object
Elem::Elem(TreeElem ElemToInit)
{
    this -> Key = ElemToInit.Key;
    this -> Value = ElemToInit.Value;
}