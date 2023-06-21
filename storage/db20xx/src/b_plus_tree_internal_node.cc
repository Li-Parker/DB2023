#include "b_plus_tree_internal_node.h"

namespace db20xx
{
    BPlusTreeInternalNode::BPlusTreeInternalNode(db20xx::IndexNodeType node_type, int max_size)
    :BPlusTreeNode(node_type, max_size, 0){}
    BPlusTreeInternalNode::BPlusTreeInternalNode(db20xx::IndexNodeType node_type, int max_size,db20xx::Key key,
                                             db20xx::BPlusTreeNode *left_node,db20xx::BPlusTreeNode *right_node)
    :BPlusTreeNode(node_type, max_size, 2){
      array_.emplace_back("",left_node);
      array_.emplace_back(key, right_node);
    }
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
        if(IsFull())
            return false;
        int size = GetSize();
        if(size==0)
        {
            array_.emplace_back(key,node);
            IncreaseSize(1);
            return true;
        }
        int i;
        for(i = 1;i<size;i++)
        {
            if(KeyAt(i)>=key)
                break;
        }
        if(i==size||KeyAt(i)!=key){
            array_.insert(array_.begin()+i,InteralNodeMappingType(key, node));
            IncreaseSize(1);
            return true;
        }
        return false;
    }
    bool BPlusTreeInternalNode::PopNode() {
        if(GetSize()>0)
        {
            array_.pop_back();
            IncreaseSize(-1);
            return true;
        }
        return false;
    }

    
}