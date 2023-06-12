#include "b_plus_tree_internal_node.h"

namespace db20xx
{
    void BPlusTreeInternalNode::Init(int max_size){
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
    auto BPlusTreeInternalNode::ValueIndex(const InternalNodeValueType &value) const -> int{
        size_t size = GetSize();
        size_t i = 0;
        for (; i < size; i++)
        {
            if(array_[i].second==value)
                return int(i);
        }
        return -1;
    }
    auto BPlusTreeInternalNode::ValueAt(int index) const -> InternalNodeValueType{
        assert(index>=0);
        return array_[index].second;
    }
    bool BPlusTreeInternalNode::PutNode(db20xx::Key key, db20xx::BPlusTreeNode *node) {

        return false;
    }


    
}