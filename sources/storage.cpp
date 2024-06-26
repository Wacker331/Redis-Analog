#include "storage.h"

//Inits storage class
Storage::Storage(StorageInterface &StorageStructure) : StorageStructure(StorageStructure)
{
    CountElems = 0;
}

//Puts element by providing only "key" and "value"
Elem Storage::Put(const std::string KeyToPut, const std::string ValueToPut) 
{
    return Storage::Put(Elem(KeyToPut, ValueToPut));
}

//Puts Elem class to storage
Elem Storage::Put(const Elem ElemToPut)
{
    Mutex.lock();
    Elem RetElem = StorageStructure.Put(ElemToPut);
    if (RetElem.Key == "")
        CountElems++;
    Mutex.unlock();
    return RetElem;
}

//Deletes element in storage by "key"
Elem Storage::Del(const std::string KeyToDel)
{
    Mutex.lock();
    Elem RetElem = StorageStructure.Del(KeyToDel);
    if (RetElem.Key != "")
        CountElems--;
    Mutex.unlock();
    return RetElem;
}

//Returns Elem class instance with number of elements in storage in "Value" field
Elem Storage::GetCount()
{
    Mutex.lock();
    Elem RetElem("", std::to_string(CountElems));
    Mutex.unlock();
    return RetElem;
}

//Gets element by "key" and returns Elem class
Elem Storage::operator[](const std::string KeyToGet)
{
    Mutex.lock();
    Elem RetElem = StorageStructure.Get(KeyToGet);
    Mutex.unlock();
    return RetElem;
}

//Dumps the whole storage to file named "filename"
void Storage::Dump(const std::string filename)
{
    std::ofstream OutputFile;

    Mutex.lock();
    OutputFile.open(filename, std::ios::out);

    if (OutputFile.is_open())
        StorageStructure.Dump(OutputFile);

    OutputFile.close();
    Mutex.unlock();
}

//Reads the "filename" and provide elements to storage
void Storage::ReadDump(const std::string filename)
{
    std::ifstream InputFile;
    InputFile.open(filename, std::ios::in);
    Elem NewElem;

    while(1)
    {
        NewElem = ParseDumpLine(InputFile);
        if (NewElem.Key == "")
            break;
        Put(NewElem);
    }

}

//Parses exact line, divides data and creates Elem structure
Elem Storage::ParseDumpLine(std::ifstream& InputFile)
{
    std::string Line, InputKey, InputValue;

    std::getline(InputFile, Line);

    size_t SeparatorPos = Line.find(":");
    if (SeparatorPos == std::string::npos) 
    {
        return Elem();
    }

    InputKey = Line.substr(1, SeparatorPos - 2);
    InputValue = Line.substr(SeparatorPos + 3, Line.length() - (SeparatorPos + 3) - 1);

    return Elem(InputKey, InputValue);
}

//Initialising empty Elem object
Elem::Elem()
{
    this -> Key = "";
    this -> Value = "";
}

//Initialising Elem class with data strings (mostly for testing)
Elem::Elem(const std::string KeyToInit, const std::string ValueToInit)
{
    this -> Key = KeyToInit;
    this -> Value = ValueToInit;
}

//Initialising Elem with ElemTree object
Elem::Elem(const TreeElem ElemToInit)
{
    this -> Key = ElemToInit.Key;
    this -> Value = ElemToInit.Value;
}

//Copy only data fields of node
void TreeElem::CopyData(const TreeElem ElemToCopy)
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

//Recursive write element by element in file for "DUMP" command
void TreeElem::Dump(std::ofstream& OutFile)
{
    OutFile << "\"" << this -> Key << "\": \"" << this -> Value << "\"" << std::endl;
    if (this -> Left != NULL)
        this -> Left -> Dump(OutFile);
    if (this -> Right != NULL)
        this -> Right -> Dump(OutFile);
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

//Initializing TreeElem by inheritated Elem class
TreeElem::TreeElem(const Elem ElemToInit)
{
    this->Key = ElemToInit.Key;
    this->Value = ElemToInit.Value;

    this->Right = NULL;
    this->Left = NULL;
}

//Empty tree initialising
Tree::Tree()
{
    this -> Root = NULL;
}

//Tree algorithm to get element in binary tree by some key
Elem Tree::Get(const std::string KeyToFind)
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
Elem Tree::Put(const struct Elem ElemToPut)
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
Elem Tree::Del(const std::string KeyToDelete)
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

//Inits the "DUMP" process in Tree structure
void Tree::Dump(std::ofstream& OutFile)
{
    if (Root != NULL)
        Root -> Dump(OutFile);
}

//Print tree (for debugging)
void Tree::Print()
{
    if (Root != NULL)
        Root->Print();
}
