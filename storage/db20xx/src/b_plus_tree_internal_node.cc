#include "b_plus_tree_internal_node.h"

namespace db20xx
{
    void BPlusTreeInternalNode::Init(int max_size = INTERNAL_NODE_SIZE){
        SetMaxSize(max_size);
    }
    auto BPlusTreeInternalNode::KeyAt(int index) const -> KeyType{
        return array_[index].first;
    }
    void BPlusTreeInternalNode::SetKeyAt(int index, const KeyType &key){
        if(index<0)
            std::cout << "Error: invalid index: "<< index << std::endl;
        else 
            array_[index].first = key;
    }
    auto BPlusTreeInternalNode::ValueIndex(const ValueType &value) const -> int{
        for (size_t i = 0; i < GetSize(); i++)
        {
            if(array_[i].second==value)
                return i;
        }
        return -1;
    }
    auto BPlusTreeInternalNode::ValueAt(int index) const -> ValueType{
        if (index<0)
        {
            std::cout << "Error: invalid index: "<< index << std::endl;
        }
        else
        {
            return array_[index].second;
        }
    }



    
}